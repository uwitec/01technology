#include "StdAfx.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//宏定义
#define TIME_RECONNECT						15000L						//重连时间
#define TIME_UPDATE_LIST					20000L						//更新时间
#define TIME_UPDATE_GAMEMAININFO			600000L						//更新时间

//定时器 ID
#define IDI_CONNECT_CENTER_SERVER			1							//连接服务器
#define IDI_UPDATE_SERVER_LIST				2							//更新列表
#define IDI_UPDATE_GAMEMAININFO				3							//更新游戏信息

//////////////////////////////////////////////////////////////////////////

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//设置变量
	m_pInitParamter=NULL;
	m_pConnectItemInfo=NULL;

	//CenterServer
	m_pCenterSocket=NULL;
	m_SocketState=SocketState_NoConnect;

	//接口变量
	m_pITimerEngine=NULL;
	m_pIEventService=NULL;
	m_pITCPSocketEngine=NULL;
	m_pIUpdateEventSink=NULL;

	//辅助变量
	m_wLogonUserCount = 0;
	m_wOnLineInfoSize=0;
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));
	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//接口查询
void * __cdecl CAttemperEngineSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//调度模块启动
bool __cdecl CAttemperEngineSink::StartService(IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(m_pInitParamter!=NULL);
	GT_ASSERT(m_pIEventService!=NULL);
	GT_ASSERT(_tcslen(m_pInitParamter->m_szCenterServerAddr)!=0);

	if(_tcslen(m_pInitParamter->m_szCenterServerAddr)==0)
		return false;

	//设置变量
	m_wOnLineInfoSize=0;
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

	//创建辅助变量
	m_pConnectItemInfo=new tagConnectItemInfo [m_pInitParamter->m_wMaxConnect];
	if (m_pConnectItemInfo==NULL) return false;
	memset(m_pConnectItemInfo,0,sizeof(tagConnectItemInfo)*m_pInitParamter->m_wMaxConnect);
	for(int i = 0; i < m_pInitParamter->m_wMaxConnect; i ++)
		m_pConnectItemInfo[i].cbUserType = INVALID_USERTYPE;
	m_wLogonUserCount = 0;

	//连接CenterServer
	DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
	m_pCenterSocket->Connect(dwServerIP,PORT_CENTER_SERVER);

	//设置定时器
	m_pITimerEngine->SetTimer(IDI_UPDATE_GAMEMAININFO,TIME_UPDATE_GAMEMAININFO,TIMER_REPEAT_TIMER,NULL);
	//投递数据库
	m_DataBaseEvent.PostDataBaseEvent(DBR_UPDATE_GAMEMAININFO,0,
		0,NULL,0);

	return true;
}

//调度模块关闭
bool __cdecl CAttemperEngineSink::StopService(IUnknownEx * pIUnknownEx)
{
	//删除数据
	SafeDeleteArray(m_pConnectItemInfo);

	//停止定时器
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);


	//设置变量
	m_wOnLineInfoSize=0;
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

	m_pCenterSocket->CloseSocket(false);

	return true;
}

//事件处理接口
bool __cdecl CAttemperEngineSink::OnAttemperEvent(BYTE cbThreadIndex,WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	switch (wIdentifier)
	{
	case EVENT_CENTER_CONNECT:	//中心连接事件
		{

			//效验大小
			GT_ASSERT(wDataSize==sizeof(NTY_CenterConnect));
			if (wDataSize!=sizeof(NTY_CenterConnect)) return false;

			//消息处理
			NTY_CenterConnect * pCenterSocketConnect=(NTY_CenterConnect *)pDataBuffer;

			return OnEventCenterSocketConnect(pCenterSocketConnect->iErrorCode);
		}
	case EVENT_CENTER_READ:		//中心读取事件
		{
			//效验大小
			NTY_CenterRead * pCenterSocketRead=(NTY_CenterRead *)pDataBuffer;
			GT_ASSERT(wDataSize>=((sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer))));
			GT_ASSERT(wDataSize==(sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer)+pCenterSocketRead->wDataSize));
			if (wDataSize<(sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer))) return false;
			if (wDataSize!=sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer)+pCenterSocketRead->wDataSize) return false;

			//处理消息
			bool bSuccess=false;
			_BEGIN_TRY 
			{ 
				bSuccess=OnEventCenterSocketRead(pCenterSocketRead->Command,pCenterSocketRead->cbReadBuffer,pCenterSocketRead->wDataSize);
			}
			CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH
			if (bSuccess==false) m_pCenterSocket->CloseSocket(true);

			return true;
		}
	case EVENT_CENTER_CLOSE:		//中心关闭事件
		{
			//效验大小
			GT_ASSERT(wDataSize==sizeof(NTY_CenterClose));
			if (wDataSize!=sizeof(NTY_CenterClose)) return false;

			//消息处理
			NTY_CenterClose * pCenterSocketClose=(NTY_CenterClose *)pDataBuffer;
			OnEventCenterSocketClose(pCenterSocketClose->bCloseByServer);

			return true;
		}
	case MR_EVENT_SEND_USER_MESSAGE:	//发送用户消息
		{
			if(m_SocketState==SocketState_Connected)
			{
				//效验大小
				GT_ASSERT(wDataSize==sizeof(MR_EVENT_SendUserMessage));
				if (wDataSize!=sizeof(MR_EVENT_SendUserMessage)) return false;

				//消息处理
				MR_EVENT_SendUserMessage * pSendUserMessage=(MR_EVENT_SendUserMessage *)pDataBuffer;

				//构造数据
				SUB_CS_UserMessage	UserMessage;
				UserMessage.dwRealUserID = pSendUserMessage->dwRealUserID;
				UserMessage.cbUserType	 = pSendUserMessage->cbUserType;
				UserMessage.wMessageType = SMT_EJECT|SMT_INFO;
				if(pSendUserMessage->bKick)
				{
					UserMessage.wMessageType |= (SMT_CLOSE_GAME|SMT_INTERMIT_LINE);
					UserMessage.wErrorCode = ErrCode_Message_ForceUserLogout;
				}
				else
				{
					UserMessage.wErrorCode = 0;
				}
				UserMessage.wMessageLength = _tcslen(pSendUserMessage->szMessage);
				_tcscpy(UserMessage.szContent,pSendUserMessage->szMessage);

				//发送数据
				WORD wSendSize=sizeof(UserMessage)-sizeof(UserMessage.szContent)+UserMessage.wMessageLength*sizeof(TCHAR);
				m_pCenterSocket->SendData(MDM_CS_MANAGEMENT,SUB_CS_USER_MESSAGE,&UserMessage,wSendSize);

			}
			return true;
		}
	}

	return false;
}

//设置事件
bool CAttemperEngineSink::SetEventService(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);

	//设置接口
	m_ServerList.SetEventService(m_pIEventService);

	return (m_pIEventService!=NULL);
}
//设置更新事件钩子
bool CAttemperEngineSink::SetUpdateEventSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIUpdateEventSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IUpdateEventSink);
	GT_ASSERT(m_pIUpdateEventSink!=NULL);

	return (m_pIUpdateEventSink!=NULL);
}
//枚举用户
void CAttemperEngineSink::EnumConnectUser(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx!=NULL);
	IEnumUserSink* pIEnumUserSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEnumUserSink);
	GT_ASSERT(pIEnumUserSink!=NULL);
	if(pIEnumUserSink == NULL || m_pConnectItemInfo == NULL || m_pInitParamter->m_wMaxConnect == 0)
		return;
	for(int i = 0; i < m_pInitParamter->m_wMaxConnect; i ++)
	{
		if(m_pConnectItemInfo[i].cbUserType != INVALID_USERTYPE)
		{
			pIEnumUserSink->OnUserData(m_pConnectItemInfo[i].dwRealUserID,
				m_pConnectItemInfo[i].cbUserType,
				m_pConnectItemInfo[i].UserData,
				m_pConnectItemInfo[i].dwClientIP);
		}
	}
}
//判断是否与中心连接
bool CAttemperEngineSink::IsConnectedCenter()
{
	if(m_pCenterSocket == NULL || m_SocketState!=SocketState_Connected)
		return false;
	return true;
}
//配置函数
bool CAttemperEngineSink::InitAttemperSink(CInitParamter * pInitParamter, CCenterSocket * pCenterSocket, IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(pInitParamter!=NULL);
	GT_ASSERT(pCenterSocket!=NULL);

	//设置变量
	m_pInitParamter=pInitParamter;
	m_pCenterSocket=pCenterSocket;

	//服务引擎
	IServiceEngine * pIServiceEngine=(IServiceEngine *)pIUnknownEx->QueryInterface(IID_IServiceEngine,VER_IServiceEngine);
	if (pIServiceEngine==NULL)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取服务引擎服务失败"),Level_Exception);
		return false;
	}

	//定时器引擎
	m_pITimerEngine=(ITimerEngine *)pIServiceEngine->GetTimerEngine(IID_ITimerEngine,VER_ITimerEngine);
	if (m_pITimerEngine==NULL)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取定时器引擎服务失败"),Level_Exception);
		return false;
	}

	//网络引擎
	m_pITCPSocketEngine=(ITCPSocketEngine *)pIServiceEngine->GetTCPSocketEngine(IID_ITCPSocketEngine,VER_ITCPSocketEngine);
	if (m_pITCPSocketEngine==NULL)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取网络引擎服务失败"),Level_Exception);
		return false;
	}

	//数据库引擎
	IDataBaseEngine * pIDataBaseEngine=(IDataBaseEngine *)pIServiceEngine->GetDataBaseEngine(IID_IDataBaseEngine,VER_IDataBaseEngine);
	if (pIDataBaseEngine==NULL)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取数据库引擎服务失败"),Level_Exception);
		return false;
	}

	//数据库触发
	IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
	if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取数据库触发服务失败"),Level_Exception);
		return false;
	}

	//列表组件
	if (m_ServerList.SetSocketEngine(m_pITCPSocketEngine)==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("列表组件与网络引擎绑定失败"),Level_Exception);
		return false;
	}

	//初始化
	memset(&m_GameInfo, 0, sizeof(CMD_GP_GameInfo));
	memset(&m_ManagementNotice, 0, sizeof(CMD_GP_Notice));
	memset(&m_GameNotice, 0, sizeof(CMD_GP_Notice));

	return true;
}

//定时器事件
bool CAttemperEngineSink::OnEventTimer(BYTE cbThreadIndex,WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_CONNECT_CENTER_SERVER:		//连接CenterServer
		{
			DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
			m_pCenterSocket->Connect(dwServerIP,PORT_CENTER_SERVER);
			return true;
		}
	case IDI_UPDATE_SERVER_LIST:		//更新服务器列表
		{
			if (m_SocketState==SocketState_Connected)
			{
				m_pCenterSocket->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);
			}
			return true;
		}
	case IDI_UPDATE_GAMEMAININFO:		//更新游戏主信息
		{
			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_UPDATE_GAMEMAININFO,0,
				0,NULL,0);
			return true;
		}
	}
	return false;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(BYTE cbThreadIndex,void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{
	switch (pDataBaseEvent->wRequestID)
	{
	case DBR_GP_LOGON_SUCCESS:		//登录成功
		{
			return OnDBLogonSuccess(pDataBuffer,wDataSize,pDataBaseEvent);
		}
	case DBR_GP_LOGON_ERROR:		//登录失败
		{
			return OnDBLogonError(pDataBuffer,wDataSize,pDataBaseEvent);
		}
	case DBR_GP_GET_LOWUSERLISTCOMPLETE:		//发送下线帐号完毕
		{
			return OnDBGetLessUserListComplete(pDataBuffer,wDataSize,pDataBaseEvent);
		}
	case DBR_UPDATE_GAMEMAININFO_SUCCESS:		//更新游戏主信息
		{
			return OnDBUpdateGameMainInfoSuccess(pDataBuffer,wDataSize,pDataBaseEvent);
		}
	}

	return false;
}

//用户登录成功
bool CAttemperEngineSink::OnDBLogonSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{
	
	//效验参数
	GT_ASSERT(pDataBaseEvent->wRequestID==DBR_GP_LOGON_SUCCESS);
	GT_ASSERT(wDataSize==sizeof(DBR_GP_LogonSuccess));
	if (wDataSize!=sizeof(DBR_GP_LogonSuccess)) return false;

	//判断在线
	GT_ASSERT(pDataBaseEvent->wIndex<m_pInitParamter->m_wMaxConnect);
	tagConnectItemInfo * pConnectItemInfo=(m_pConnectItemInfo+pDataBaseEvent->wIndex);
	if (pConnectItemInfo->bConnect==FALSE) return true;
	if (pConnectItemInfo->wRoundID!=pDataBaseEvent->wRoundID) return true;

	DBR_GP_LogonSuccess * pDBRLogonSuccess=(DBR_GP_LogonSuccess *)pDataBuffer;

	pConnectItemInfo->cbUserType = pDBRLogonSuccess->cbUserType;
	pConnectItemInfo->dwRealUserID=pDBRLogonSuccess->dwRealUserID;
	memcpy(&pConnectItemInfo->UserData, &pDBRLogonSuccess->UserData, sizeof(pDBRLogonSuccess->UserData));

	//构造数据
	BYTE cbBuffer[SOCKET_PACKAGE];
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbBuffer;
	memcpy(pCMDLogonSuccess, pDBRLogonSuccess, sizeof(DBR_GP_LogonSuccess));
	ConvertACPToUtf8(pCMDLogonSuccess->UserData.szAccount, NAME_LEN,
		pCMDLogonSuccess->UserData.szAccount, NAME_LEN);
	ConvertACPToUtf8(pCMDLogonSuccess->UserData.szName, NAME_LEN,
		pCMDLogonSuccess->UserData.szName, NAME_LEN);


	//发送登录结果
	WORD wIndex=pDataBaseEvent->wIndex;
	WORD wRoundID=pDataBaseEvent->wRoundID;
	m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbBuffer,sizeof(CMD_GP_LogonSuccess));

	//发送游戏信息	
	m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_GAMEINFO,&m_GameInfo,sizeof(m_GameInfo));
	//发送公告
	if(pConnectItemInfo->UserData.cbLevel == USERLEVEL_GAMEUSER)
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_NOTICE,&m_GameNotice,sizeof(m_GameNotice));
	else
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_NOTICE,&m_ManagementNotice,sizeof(m_ManagementNotice));

	
	//列表配置
	CMD_GP_ListConfig ListConfig;
	ListConfig.bShowOnLineCount=TRUE;
	m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_SERVER_LIST,SUB_GP_LIST_CONFIG,&ListConfig,sizeof(ListConfig));

	//发送游戏列表
	m_ServerList.SendGameTypeList(wIndex,wRoundID);
	m_ServerList.SendGameKindList(wIndex,wRoundID);
	m_ServerList.SendGameStationList(wIndex,wRoundID);
	m_ServerList.SendGameServerList(wIndex,wRoundID);

	//发送登录完成
	m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH);
	m_pITCPSocketEngine->AllowBatchSend(wIndex,wRoundID,true);

	return true;
}

//用户登录失败
bool CAttemperEngineSink::OnDBLogonError(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{
	
	//效验参数
	GT_ASSERT(pDataBaseEvent->wRequestID==DBR_GP_LOGON_ERROR);
	GT_ASSERT(wDataSize==sizeof(DBR_GP_LogonError));

	//判断在线
	GT_ASSERT(pDataBaseEvent->wIndex<m_pInitParamter->m_wMaxConnect);
	tagConnectItemInfo * pConnectItemInfo=(m_pConnectItemInfo+pDataBaseEvent->wIndex);
	if (pConnectItemInfo->bConnect==FALSE) return true;
	if (pConnectItemInfo->wRoundID!=pDataBaseEvent->wRoundID) return true;

	//变量定义
	CMD_GP_LogonError LogonError;
	DBR_GP_LogonError * pLogonError=(DBR_GP_LogonError *)pDataBuffer;

	//构造数据
	LogonError.wErrorCode=pLogonError->wErrorCode;


	//发送数据
	WORD wSendSize=sizeof(LogonError);
	m_pITCPSocketEngine->SendData(pDataBaseEvent->wIndex,pDataBaseEvent->wRoundID,MDM_GP_LOGON,
		SUB_GP_LOGON_ERROR,&LogonError,wSendSize);

	//关闭连接
	m_pITCPSocketEngine->ShutDownSocket(pDataBaseEvent->wIndex,pDataBaseEvent->wRoundID);

	return true;
}
//发送下线帐号完毕
bool CAttemperEngineSink::OnDBGetLessUserListComplete(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{

	//效验参数
	GT_ASSERT(pDataBaseEvent->wRequestID==DBR_GP_GET_LOWUSERLISTCOMPLETE);
	GT_ASSERT(wDataSize==sizeof(DBR_GP_GetSubUserList));

	//判断在线
	GT_ASSERT(pDataBaseEvent->wIndex<m_pInitParamter->m_wMaxConnect);
	tagConnectItemInfo * pConnectItemInfo=(m_pConnectItemInfo+pDataBaseEvent->wIndex);
	if (pConnectItemInfo->bConnect==FALSE) return true;
	if (pConnectItemInfo->wRoundID!=pDataBaseEvent->wRoundID) return true;

	//变量定义
	DBR_GP_GetSubUserList * pGetSubUserList=(DBR_GP_GetSubUserList *)pDataBuffer;

	//投递数据库
	m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_SUBUSERLIST,pDataBaseEvent->wIndex,
		pDataBaseEvent->wRoundID,pGetSubUserList,sizeof(DBR_GP_GetSubUserList));

	return true;
}
//更新游戏主信息
bool CAttemperEngineSink::OnDBUpdateGameMainInfoSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{

	//效验参数
	GT_ASSERT(pDataBaseEvent->wRequestID==DBR_UPDATE_GAMEMAININFO_SUCCESS);
	GT_ASSERT(wDataSize==sizeof(DBR_Update_GameMainInfo));

	//变量定义
	DBR_Update_GameMainInfo * pGameMainInfo=(DBR_Update_GameMainInfo *)pDataBuffer;
	pGameMainInfo->GameInfo.szFieldLiveVideoAddress[CountArray(pGameMainInfo->GameInfo.szFieldLiveVideoAddress)-1]=0;
	pGameMainInfo->GameInfo.szMainWebPageAddress[CountArray(pGameMainInfo->GameInfo.szMainWebPageAddress)-1]=0;
	pGameMainInfo->GameInfo.szVideoTapeAddress[CountArray(pGameMainInfo->GameInfo.szVideoTapeAddress)-1]=0;
	pGameMainInfo->ManagementNotice.szNotice[CountArray(pGameMainInfo->ManagementNotice.szNotice)-1]=0;
	pGameMainInfo->GameNotice.szNotice[CountArray(pGameMainInfo->GameNotice.szNotice)-1]=0;

	ConvertACPToUtf8(pGameMainInfo->GameInfo.szFieldLiveVideoAddress, 
		CountArray(pGameMainInfo->GameInfo.szFieldLiveVideoAddress),
		pGameMainInfo->GameInfo.szFieldLiveVideoAddress,
		CountArray(pGameMainInfo->GameInfo.szFieldLiveVideoAddress));

	ConvertACPToUtf8(pGameMainInfo->GameInfo.szMainWebPageAddress, 
		CountArray(pGameMainInfo->GameInfo.szMainWebPageAddress),
		pGameMainInfo->GameInfo.szMainWebPageAddress,
		CountArray(pGameMainInfo->GameInfo.szMainWebPageAddress));

	ConvertACPToUtf8(pGameMainInfo->GameInfo.szVideoTapeAddress, 
		CountArray(pGameMainInfo->GameInfo.szVideoTapeAddress),
		pGameMainInfo->GameInfo.szVideoTapeAddress,
		CountArray(pGameMainInfo->GameInfo.szVideoTapeAddress));

	ConvertACPToUtf8(pGameMainInfo->ManagementNotice.szNotice, 
		CountArray(pGameMainInfo->ManagementNotice.szNotice),
		pGameMainInfo->ManagementNotice.szNotice,
		CountArray(pGameMainInfo->ManagementNotice.szNotice));

	ConvertACPToUtf8(pGameMainInfo->GameNotice.szNotice, 
		CountArray(pGameMainInfo->GameNotice.szNotice),
		pGameMainInfo->GameNotice.szNotice,
		CountArray(pGameMainInfo->GameNotice.szNotice));

	memcpy(&m_GameInfo, &pGameMainInfo->GameInfo, sizeof(pGameMainInfo->GameInfo));
	memcpy(&m_ManagementNotice, &pGameMainInfo->ManagementNotice, sizeof(pGameMainInfo->ManagementNotice));
	memcpy(&m_GameNotice, &pGameMainInfo->GameNotice, sizeof(pGameMainInfo->GameNotice));

	return true;
}

//网络应答事件
bool __cdecl CAttemperEngineSink::OnEventSocketAccept(BYTE cbThreadIndex,NTY_SocketAcceptEvent * pSocketAcceptEvent)
{
	//效验参数
	GT_ASSERT(pSocketAcceptEvent->wIndex<m_pInitParamter->m_wMaxConnect);

	//设置数据
	tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketAcceptEvent->wIndex;
	GT_ASSERT(pConnectItemInfo->bConnect == FALSE);

	pConnectItemInfo->bConnect=TRUE;
	pConnectItemInfo->wRoundID=pSocketAcceptEvent->wRoundID;
	pConnectItemInfo->dwClientIP=pSocketAcceptEvent->dwClientIP;

	pConnectItemInfo->cbUserType=INVALID_USERTYPE;
	memset(&pConnectItemInfo->UserData, 0, sizeof(pConnectItemInfo->UserData));
	pConnectItemInfo->dwRealUserID=0;

	m_wLogonUserCount ++;

	return true;
}

//网络读取事件
bool __cdecl CAttemperEngineSink::OnEventSocketRead(BYTE cbThreadIndex,
													CMD_Command Command, 
													void * pDataBuffer, 
													WORD wDataSize, 
													NTY_SocketReadEvent * pSocketReadEvent)
{
	switch (Command.wMainCmdID)
	{
	//登录消息
	case MDM_GP_LOGON:				
		{
			return OnSocketMainLogon(Command,pDataBuffer,wDataSize,pSocketReadEvent);
		}
	default:
		{
			//效验参数
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);
			
			//检查登陆
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pConnectItemInfo->cbUserType != INVALID_USERTYPE);
			if(pConnectItemInfo->cbUserType == INVALID_USERTYPE)
				return false;


			switch (Command.wMainCmdID)
			{
				//编辑帐号
			case MDM_GP_EDIT_USERINFO:				
				{
					return OnSocketMainEditUser(Command,pDataBuffer,wDataSize,pSocketReadEvent);
				}
				//编辑子帐号
			case MDM_GP_EDIT_SUBUSERINFO:				
				{
					return OnSocketMainEditSubUser(Command,pDataBuffer,wDataSize,pSocketReadEvent);
				}
				//获取帐号
			case MDM_GP_GET_USER:				
				{
					return OnSocketMainGetUser(Command,pDataBuffer,wDataSize,pSocketReadEvent);
				}
				//获取记录
			case MDM_GP_GET_RECORD:
				{
					return OnSocketMainGetRecord(Command,pDataBuffer,wDataSize,pSocketReadEvent);
				}
			default:
				break;
			}

			break;
		}
	
	}

	return false;
}

//网络关闭事件
bool __cdecl CAttemperEngineSink::OnEventSocketClose(BYTE cbThreadIndex,
													 NTY_SocketCloseEvent * pSocketCloseEvent)
{
	//效验参数
	GT_ASSERT(pSocketCloseEvent->wIndex<m_pInitParamter->m_wMaxConnect);

	//设置数据
	tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketCloseEvent->wIndex;

	//取消登陆
	if(pConnectItemInfo->cbUserType != INVALID_USERTYPE)
	{
		DBR_GP_LogoutUser LogoutUser;
		LogoutUser.dwRealUserID	= pConnectItemInfo->dwRealUserID;
		LogoutUser.cbUserType		= pConnectItemInfo->cbUserType;
		//投递数据库
		m_DataBaseEvent.PostDataBaseEvent(DBR_GP_LOGOUT_USER,0,
			0,&LogoutUser,sizeof(LogoutUser));
	}


	pConnectItemInfo->wRoundID++;
	pConnectItemInfo->dwClientIP=0;
	pConnectItemInfo->bConnect=FALSE;
	pConnectItemInfo->cbUserType=INVALID_USERTYPE;
	memset(&pConnectItemInfo->UserData, 0, sizeof(pConnectItemInfo->UserData));
	pConnectItemInfo->dwRealUserID=0;

	m_wLogonUserCount --;

	return true;
}

//中心连接事件
bool __cdecl CAttemperEngineSink::OnEventCenterSocketConnect(int iErrorCode)
{
	//错误判断
	if (iErrorCode!=0)
	{
		m_SocketState=SocketState_NoConnect;
		m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);
		return true;
	}

	//设置变量
	m_SocketState=SocketState_Connected;

	//获取列表
	m_pCenterSocket->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

	//设置定时器
	m_pITimerEngine->SetTimer(IDI_UPDATE_SERVER_LIST,TIME_UPDATE_LIST,TIMER_REPEAT_TIMER,NULL);

	return true;
}

//中心读取事件
bool __cdecl CAttemperEngineSink::OnEventCenterSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:	//列表消息
		{
			return OnCenterMainServerList(Command,pDataBuffer,wDataSize);
		}
	case MDM_CS_UPDATE_INFO:	//更新信息
		{
			return OnCenterMainUpdateInfo(Command,pDataBuffer,wDataSize);
		}
	case MDM_CS_MANAGEMENT:		//管理消息
		{
			return OnCenterMainManagement(Command,pDataBuffer,wDataSize);
		}
	}
	return true;
}

//中心关闭事件
bool __cdecl CAttemperEngineSink::OnEventCenterSocketClose(bool bCloseByServer)
{
	//设置变量
	m_SocketState=SocketState_NoConnect;
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);

	//重新连接
	if (bCloseByServer==true) m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

	return true;
}

//登录消息处理
bool CAttemperEngineSink::OnSocketMainLogon(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_GP_LOGON);
	switch (Command.wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNT:		//帐号登录
		{
			//效验参数
			int  sizeof_CMD_GP_LogonByAccount = sizeof(CMD_GP_LogonByAccount);
			GT_ASSERT(wDataSize>=sizeof_CMD_GP_LogonByAccount);
			if (wDataSize<sizeof_CMD_GP_LogonByAccount) return false;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pConnectItemInfo+pSocketReadEvent->wIndex)->dwClientIP;
			GT_ASSERT((m_pConnectItemInfo+pSocketReadEvent->wIndex)->cbUserType == INVALID_USERTYPE);

			//处理消息
			CMD_GP_LogonByAccount * pLogonByAccount=(CMD_GP_LogonByAccount *)pDataBuffer;
			pLogonByAccount->szAccounts[CountArray(pLogonByAccount->szAccounts)-1]=0;
			pLogonByAccount->szPassWord[CountArray(pLogonByAccount->szPassWord)-1]=0;
			pLogonByAccount->szRealityPass[CountArray(pLogonByAccount->szRealityPass)-1]=0;
			ConvertUtf8ToACP(pLogonByAccount->szAccounts, NAME_LEN,
				pLogonByAccount->szAccounts, NAME_LEN);
			ConvertUtf8ToACP(pLogonByAccount->szPassWord, PASS_LEN,
				pLogonByAccount->szPassWord, PASS_LEN);
			ConvertUtf8ToACP(pLogonByAccount->szRealityPass, PASS_LEN,
				pLogonByAccount->szRealityPass, PASS_LEN);

			if(CAIUtil::IsAIUser(pLogonByAccount->szAccounts))
			{
				//变量定义
				CMD_GP_LogonError LogonError;

				//构造数据
				LogonError.wErrorCode=ErrCode_CMD_GP_LogonError_Password;

				//发送数据
				WORD wSendSize=sizeof(LogonError);
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_LOGON,
					SUB_GP_LOGON_ERROR,&LogonError,wSendSize);

				//关闭连接
				m_pITCPSocketEngine->ShutDownSocket(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID);
				return true;
			}
			
			//效验版本
			if (pLogonByAccount->dwPlazaVersion!=VER_PLAZA_FRAME)
			{
				//获取版本
				WORD wLow=LOWORD(pLogonByAccount->dwPlazaVersion);
				WORD wHigh=HIWORD(pLogonByAccount->dwPlazaVersion);

				//构造数据
				CMD_GP_Version Version;
				Version.bNewVersion=TRUE;
				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;

				//发送数据
				WORD wIndex=pSocketReadEvent->wIndex;
				WORD wRountID=pSocketReadEvent->wRoundID;
				m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));

				//判断关闭
				if (Version.bAllowConnect==FALSE)
				{
					m_pITCPSocketEngine->ShutDownSocket(wIndex,wRountID);
					return true;
				}
			}

			//构造数据
			DBR_GP_LogonByAccount LogonByAccount;
			memset(&LogonByAccount,0,sizeof(LogonByAccount));
			LogonByAccount.dwClientIP=dwClientAddr;
			lstrcpyn(LogonByAccount.szAccounts,pLogonByAccount->szAccounts,sizeof(LogonByAccount.szAccounts));
			lstrcpyn(LogonByAccount.szPassWord,pLogonByAccount->szPassWord,sizeof(LogonByAccount.szPassWord));
			lstrcpyn(LogonByAccount.szRealityPass,pLogonByAccount->szRealityPass,sizeof(LogonByAccount.szRealityPass));
			LogonByAccount.cbClientType = pLogonByAccount->cbClientType;
			
			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_LOGON_BY_ACCOUNT,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&LogonByAccount,sizeof(LogonByAccount));

			return true;
		}
	}
	return false;
}

//编辑帐号处理
bool CAttemperEngineSink::OnSocketMainEditUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_GP_EDIT_USERINFO);
	switch (Command.wSubCmdID)
	{

	case SUB_GP_REGISTER_USER:		//注册帐号
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_RegisterUser));
			if (wDataSize<sizeof(CMD_GP_RegisterUser)) return false;
			//用户操作安全判断
			CMD_GP_RegisterUser * pRegisterUser=(CMD_GP_RegisterUser *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pRegisterUser->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pRegisterUser->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanRegisterUser(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_REGISTER_ERROR,&Error,sizeof(Error));
				return true;
			}

			//处理消息
			pRegisterUser->UserData.szAccount[CountArray(pRegisterUser->UserData.szAccount)-1]=0;
			pRegisterUser->szPassWord[CountArray(pRegisterUser->szPassWord)-1]=0;

			ConvertUtf8ToACP(pRegisterUser->UserData.szAccount, NAME_LEN,
				pRegisterUser->UserData.szAccount, NAME_LEN);
			ConvertUtf8ToACP(pRegisterUser->UserData.szName, NAME_LEN,
				pRegisterUser->UserData.szName, NAME_LEN);
			ConvertUtf8ToACP(pRegisterUser->szPassWord, PASS_LEN,
				pRegisterUser->szPassWord, PASS_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_RegisterUser(pRegisterUser)) != 0)
			{
				//变量定义
				CMD_GP_RegisterError RegisterError;
				RegisterError.wErrorCode = wErrorCode;
				//发送数据
				WORD wSendSize=sizeof(RegisterError);
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
				SUB_GP_REGISTER_ERROR,&RegisterError,wSendSize);
				return true;
			}


			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pConnectItemInfo+pSocketReadEvent->wIndex)->dwClientIP;

	
			//构造数据
			DBR_GP_RegisterUser RegisterUser;
			memset(&RegisterUser,0,sizeof(RegisterUser));
			RegisterUser.dwOperationUserID = pRegisterUser->dwOperationUserID;
			RegisterUser.dwClientIP=dwClientAddr;
			memcpy(&RegisterUser.UserData, &pRegisterUser->UserData, sizeof(tagUserData));
			_tcscpy(RegisterUser.szPassWord, pRegisterUser->szPassWord);

			RegisterUser.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			RegisterUser.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(RegisterUser.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			RegisterUser.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_REGISTER_USER,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&RegisterUser,sizeof(RegisterUser));

			return true;
		}
	case SUB_GP_SET_SETUSERSCORE://存取额度
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_SetUserScore));
			if (wDataSize<sizeof(CMD_GP_SetUserScore)) return false;
			//用户操作安全判断
			CMD_GP_SetUserScore * pSetUserScore=(CMD_GP_SetUserScore *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pSetUserScore->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pSetUserScore->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetUserScore(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_SETUSERSCORE_ERROR,&Error,sizeof(Error));
				return true;
			}

			//处理消息
			
			if(pSetUserScore->fScore < 0)
			{
				//变量定义
				CMD_GP_SetUserScoreError SetUserScoreError;
				SetUserScoreError.wErrorCode = ErrCode_CMD_GP_SetUserScoreError_ScoreNegative;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_SETUSERSCORE_ERROR,&SetUserScoreError,sizeof(CMD_GP_SetUserScoreError));
				return true;

			}

			//构造数据
			DBR_GP_SetUserScore SetUserScore;
			memcpy(&SetUserScore.RequestData, pSetUserScore,sizeof(CMD_GP_SetUserScore));
			SetUserScore.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetUserScore.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetUserScore.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetUserScore.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_SETUSERSCORE,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetUserScore,sizeof(DBR_GP_SetUserScore));

			return true;
		}
	case SUB_GP_SET_USERCONGEALSTATE://状态设定停用启用
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_SetUserCongealState));
			if (wDataSize<sizeof(CMD_GP_SetUserCongealState)) return false;
			//用户操作安全判断
			CMD_GP_SetUserCongealState * pSetUserCongealState=(CMD_GP_SetUserCongealState *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pSetUserCongealState->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pSetUserCongealState->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetUserCongealState(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_USERCONGEALSTATE_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			if(pSetUserCongealState->cbLevel > USERLEVEL_GAMEUSER)
			{
				//变量定义
				CMD_GP_SetUserCongealStateError SetUserCongealStateError;
				SetUserCongealStateError.wErrorCode = ErrCode_CMD_GP_SetUserScoreError_ScoreNegative;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_USERCONGEALSTATE_ERROR,&SetUserCongealStateError,sizeof(CMD_GP_SetUserCongealStateError));
				return true;

			}

			//构造数据
			DBR_GP_SetUserCongealState SetUserCongealState;
			memcpy(&SetUserCongealState.RequestData, pSetUserCongealState,sizeof(CMD_GP_SetUserCongealState));
			SetUserCongealState.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetUserCongealState.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetUserCongealState.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetUserCongealState.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_USERCONGEALSTATE,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetUserCongealState,sizeof(DBR_GP_SetUserCongealState));

			return true;
		}
	case SUB_GP_SET_BASICUSERINFO://基本信息修改
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_BasicUserInfo));
			if (wDataSize<sizeof(CMD_GP_BasicUserInfo)) return false;
			//用户操作安全判断
			CMD_GP_BasicUserInfo * pBasicUserInfo=(CMD_GP_BasicUserInfo *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pBasicUserInfo->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pBasicUserInfo->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetBaiscUserInfo(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_BASICUSERINFO_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			pBasicUserInfo->szName[CountArray(pBasicUserInfo->szName)-1]=0;
			pBasicUserInfo->szPassWord[CountArray(pBasicUserInfo->szPassWord)-1]=0;

			ConvertUtf8ToACP(pBasicUserInfo->szName, NAME_LEN,
				pBasicUserInfo->szName, NAME_LEN);
			ConvertUtf8ToACP(pBasicUserInfo->szPassWord, PASS_LEN,
				pBasicUserInfo->szPassWord, PASS_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_BasicUserInfo(pBasicUserInfo)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError SetBasicUserInfoError;
				SetBasicUserInfoError.wErrorCode = wErrorCode;
				//发送数据
	
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_BASICUSERINFO_ERROR,&SetBasicUserInfoError,sizeof(CMD_GP_SetBasicUserInfoError));
				return true;
			}
			//构造数据
			DBR_GP_SetBasicUserInfo SetBasicUserInfo;
			memcpy(&SetBasicUserInfo.RequestData, pBasicUserInfo,sizeof(CMD_GP_BasicUserInfo));
			SetBasicUserInfo.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetBasicUserInfo.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetBasicUserInfo.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetBasicUserInfo.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_BASICUSERINFO,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetBasicUserInfo,sizeof(DBR_GP_SetBasicUserInfo));

			return true;
		}
	case SUB_GP_SET_DETAILUSERINFO://详细资料修改
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_DetailUserInfo));
			if (wDataSize<sizeof(CMD_GP_DetailUserInfo)) return false;
			//用户操作安全判断
			CMD_GP_DetailUserInfo * pDetailUserInfo=(CMD_GP_DetailUserInfo *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pDetailUserInfo->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pDetailUserInfo->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetDetailUserInfo(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_DETAILUSERINFO_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_DetailUserInfo(pDetailUserInfo)) != 0)
			{
				//变量定义
				CMD_GP_SetDetailUserInfoError SetDetailUserInfoError;
				SetDetailUserInfoError.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_DETAILUSERINFO_ERROR,&SetDetailUserInfoError,sizeof(CMD_GP_SetDetailUserInfoError));
				return true;
			}
			//构造数据
			DBR_GP_SetDetailUserInfo SetDetailUserInfo;
			memcpy(&SetDetailUserInfo.RequestData, pDetailUserInfo,sizeof(CMD_GP_DetailUserInfo));
			SetDetailUserInfo.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetDetailUserInfo.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetDetailUserInfo.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetDetailUserInfo.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_DETAILUSERINFO,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetDetailUserInfo,sizeof(DBR_GP_SetDetailUserInfo));
			return true;
		}
	case SUB_GP_SET_USERRIGHT://权限设定
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_SetUserRight));
			if (wDataSize<sizeof(CMD_GP_SetUserRight)) return false;
			//用户操作安全判断
			CMD_GP_SetUserRight * pSetUserRight=(CMD_GP_SetUserRight *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pSetUserRight->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pSetUserRight->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetUserRight(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_USERRIGHT_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息

			//构造数据
			DBR_GP_SetUserRight SetUserRight;
			memcpy(&SetUserRight.RequestData, pSetUserRight,sizeof(CMD_GP_SetUserRight));
			SetUserRight.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetUserRight.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetUserRight.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetUserRight.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_USERRIGHT,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetUserRight,sizeof(DBR_GP_SetUserRight));

			return true;
		}
	case SUB_GP_SET_MYSELFBASICUSERINFO://自身基本资料修改
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_MySelfBasicUserInfo));
			if (wDataSize<sizeof(CMD_GP_MySelfBasicUserInfo)) return false;
			//用户操作安全判断
			CMD_GP_MySelfBasicUserInfo * pBasicUserInfo=(CMD_GP_MySelfBasicUserInfo *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pBasicUserInfo->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pBasicUserInfo->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanMyselfDataSet(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_MYSELFBASICUSERINFO_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			pBasicUserInfo->szName[CountArray(pBasicUserInfo->szName)-1]=0;
			pBasicUserInfo->szPassWord[CountArray(pBasicUserInfo->szPassWord)-1]=0;
			pBasicUserInfo->szNewPassWord[CountArray(pBasicUserInfo->szNewPassWord)-1]=0;
			pBasicUserInfo->szNewRealityPassWord[CountArray(pBasicUserInfo->szNewRealityPassWord)-1]=0;

			ConvertUtf8ToACP(pBasicUserInfo->szName, NAME_LEN,
				pBasicUserInfo->szName, NAME_LEN);
			ConvertUtf8ToACP(pBasicUserInfo->szPassWord, PASS_LEN,
				pBasicUserInfo->szPassWord, PASS_LEN);
			ConvertUtf8ToACP(pBasicUserInfo->szNewPassWord, PASS_LEN,
				pBasicUserInfo->szNewPassWord, PASS_LEN);
			ConvertUtf8ToACP(pBasicUserInfo->szNewRealityPassWord, PASS_LEN,
				pBasicUserInfo->szNewRealityPassWord, PASS_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_MySelfBasicUserInfo(pBasicUserInfo)) != 0)
			{
				//变量定义
				CMD_GP_SetMySelfBasicUserInfoError SetBasicUserInfoError;
				SetBasicUserInfoError.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_MYSELFBASICUSERINFO_ERROR,&SetBasicUserInfoError,sizeof(CMD_GP_SetMySelfBasicUserInfoError));
				return true;
			}
			//构造数据
			DBR_GP_SetMySelfBasicUserInfo SetBasicUserInfo;
			memcpy(&SetBasicUserInfo.RequestData, pBasicUserInfo,sizeof(CMD_GP_MySelfBasicUserInfo));
			SetBasicUserInfo.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetBasicUserInfo.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetBasicUserInfo.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetBasicUserInfo.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_MYSELFBASICUSERINFO,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetBasicUserInfo,sizeof(DBR_GP_SetMySelfBasicUserInfo));

			return true;
		}
	}
	return false;
}
//编辑子帐号处理
bool CAttemperEngineSink::OnSocketMainEditSubUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_GP_EDIT_SUBUSERINFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GP_REGISTER_SUBUSER://注册子帐号
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_RegisterSubUser));
			if (wDataSize<sizeof(CMD_GP_RegisterSubUser)) return false;

			//用户操作安全判断
			CMD_GP_RegisterSubUser * pRegisterSubUserInfo=(CMD_GP_RegisterSubUser *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pRegisterSubUserInfo->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pRegisterSubUserInfo->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanRegisterSubUser(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_SUBUSERINFO,
					SUB_GP_REGISTER_SUBUSER_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			
			pRegisterSubUserInfo->szAccount[CountArray(pRegisterSubUserInfo->szAccount)-1]=0;
			pRegisterSubUserInfo->szName[CountArray(pRegisterSubUserInfo->szName)-1]=0;
			pRegisterSubUserInfo->szPassWord[CountArray(pRegisterSubUserInfo->szPassWord)-1]=0;

			ConvertUtf8ToACP(pRegisterSubUserInfo->szAccount, NAME_LEN,
				pRegisterSubUserInfo->szAccount, NAME_LEN);
			ConvertUtf8ToACP(pRegisterSubUserInfo->szName, NAME_LEN,
				pRegisterSubUserInfo->szName, NAME_LEN);
			ConvertUtf8ToACP(pRegisterSubUserInfo->szPassWord, PASS_LEN,
				pRegisterSubUserInfo->szPassWord, PASS_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_RegisterSubUser(pRegisterSubUserInfo)) != 0)
			{
				//变量定义
				CMD_GP_RegisterSubUserError RegisterSubUserError;
				RegisterSubUserError.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_SUBUSERINFO,
					SUB_GP_REGISTER_SUBUSER_ERROR,&RegisterSubUserError,sizeof(CMD_GP_RegisterSubUserError));
				return true;
			}
			//构造数据
			DBR_GP_RegisterSubUserInfo RegisterSubUserInfo;
			memcpy(&RegisterSubUserInfo.RequestData, pRegisterSubUserInfo,sizeof(CMD_GP_RegisterSubUser));
			RegisterSubUserInfo.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			RegisterSubUserInfo.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(RegisterSubUserInfo.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			RegisterSubUserInfo.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_REGISTER_SUBUSER,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&RegisterSubUserInfo,sizeof(DBR_GP_RegisterSubUserInfo));
			return true;
		}
	case SUB_GP_SET_BASICSUBUSERINFO://子帐号基本信息修改
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_BasicSubUserInfo));
			if (wDataSize<sizeof(CMD_GP_BasicSubUserInfo)) return false;
			//用户操作安全判断
			CMD_GP_BasicSubUserInfo * pBasicSubUserInfo=(CMD_GP_BasicSubUserInfo *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pBasicSubUserInfo->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pBasicSubUserInfo->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetBaiscSubUserInfo(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_SUBUSERINFO,
					SUB_GP_SET_BASICSUBUSERINFO_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			
			pBasicSubUserInfo->szName[CountArray(pBasicSubUserInfo->szName)-1]=0;
			pBasicSubUserInfo->szPassWord[CountArray(pBasicSubUserInfo->szPassWord)-1]=0;

			ConvertUtf8ToACP(pBasicSubUserInfo->szName, NAME_LEN,
				pBasicSubUserInfo->szName, NAME_LEN);
			ConvertUtf8ToACP(pBasicSubUserInfo->szPassWord, PASS_LEN,
				pBasicSubUserInfo->szPassWord, PASS_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_BasicSubUserInfo(pBasicSubUserInfo)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError Error;
				Error.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_SUBUSERINFO,
					SUB_GP_SET_BASICSUBUSERINFO_ERROR,&Error,sizeof(Error));
				return true;
			}
			//构造数据
			DBR_GP_SetBasicSubUserInfo SetBasicSubUserInfo;
			memcpy(&SetBasicSubUserInfo.RequestData, pBasicSubUserInfo,sizeof(CMD_GP_BasicSubUserInfo));
			SetBasicSubUserInfo.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetBasicSubUserInfo.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetBasicSubUserInfo.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetBasicSubUserInfo.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_BASICSUBUSERINFO,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetBasicSubUserInfo,sizeof(DBR_GP_SetBasicSubUserInfo));

			return true;
		}
	case SUB_GP_SET_SUBUSERCONGEALSTATE://子帐号状态设定停用启用
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_SetSubUserCongealState));
			if (wDataSize<sizeof(CMD_GP_SetSubUserCongealState)) return false;
			//用户操作安全判断
			CMD_GP_SetSubUserCongealState * pSetSubUserCongealState=(CMD_GP_SetSubUserCongealState *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pSetSubUserCongealState->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pSetSubUserCongealState->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanSetSubUserCongealState(pConnectItemInfo->UserData.dwMasterRight) == false ||
				SimpleCheck_UserRight(pConnectItemInfo->cbUserType, pConnectItemInfo->UserData) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_EDIT_SUBUSERINFO,
					SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			
			
			//构造数据
			DBR_GP_SetSubUserCongealState SetSubUserCongealState;
			memcpy(&SetSubUserCongealState.RequestData, pSetSubUserCongealState,sizeof(CMD_GP_SetSubUserCongealState));
			SetSubUserCongealState.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			SetSubUserCongealState.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(SetSubUserCongealState.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			SetSubUserCongealState.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_SET_SUBUSERCONGEALSTATE,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&SetSubUserCongealState,sizeof(DBR_GP_SetSubUserCongealState));

			return true;
		}
	}
	return false;
}
//获取帐号处理
bool CAttemperEngineSink::OnSocketMainGetUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_GP_GET_USER);
	switch (Command.wSubCmdID)
	{

	case SUB_GP_GET_LOWUSERLIST:		//获取帐号
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetLessUserList));
			if (wDataSize<sizeof(CMD_GP_GetLessUserList)) return false;

			//处理消息
			CMD_GP_GetLessUserList * pGetUser=(CMD_GP_GetLessUserList *)pDataBuffer;
			
			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_LOWUSERLIST,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,pGetUser,sizeof(CMD_GP_GetLessUserList));

			return true;
		}
	}
	return false;
}
//获取记录
bool CAttemperEngineSink::OnSocketMainGetRecord(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_GP_GET_RECORD);
	switch (Command.wSubCmdID)
	{

	case SUB_GP_GET_OPERATION:
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetOperation));
			if (wDataSize<sizeof(CMD_GP_GetOperation)) return false;

			//处理消息	
			//用户操作安全判断
			CMD_GP_GetOperation * pGetOperation=(CMD_GP_GetOperation *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pGetOperation->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pGetOperation->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanOperationView(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_OPERATIONERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			pGetOperation->szAccount[CountArray(pGetOperation->szAccount)-1]=0;
			ConvertUtf8ToACP(pGetOperation->szAccount, NAME_LEN,
				pGetOperation->szAccount, NAME_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_GetOperation(pGetOperation)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError Error;
				Error.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_OPERATIONERROR,&Error,sizeof(Error));
				return true;
			}
			//构造数据
			DBR_GP_GetOperation GetOperation;
			memcpy(&GetOperation.RequestData, pGetOperation,sizeof(CMD_GP_GetOperation));
			GetOperation.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			GetOperation.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(GetOperation.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			GetOperation.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_OPERATION,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&GetOperation,sizeof(DBR_GP_GetOperation));

			return true;
		}
	case SUB_GP_GET_CHARTVIEWONE:
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetChartViewOne));
			if (wDataSize<sizeof(CMD_GP_GetChartViewOne)) return false;

			//处理消息	
			//用户操作安全判断
			CMD_GP_GetChartViewOne * pGetChartViewOne=(CMD_GP_GetChartViewOne *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pGetChartViewOne->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pGetChartViewOne->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanChartView(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_CHARTVIEWONEERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			pGetChartViewOne->szAccount[CountArray(pGetChartViewOne->szAccount)-1]=0;
			ConvertUtf8ToACP(pGetChartViewOne->szAccount, NAME_LEN,
				pGetChartViewOne->szAccount, NAME_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_GetChartViewOne(pGetChartViewOne)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError Error;
				Error.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_CHARTVIEWONEERROR,&Error,sizeof(Error));
				return true;
			}
			//构造数据
			DBR_GP_GetChartViewOne GetChartViewOne;
			memcpy(&GetChartViewOne.RequestData, pGetChartViewOne,sizeof(CMD_GP_GetChartViewOne));
			GetChartViewOne.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			GetChartViewOne.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(GetChartViewOne.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			GetChartViewOne.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_CHARTVIEWONE,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&GetChartViewOne,sizeof(DBR_GP_GetChartViewOne));

			return true;
		}
	case SUB_GP_GET_CHARTVIEWTWO:
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetChartViewTwo));
			if (wDataSize<sizeof(CMD_GP_GetChartViewTwo)) return false;

			//处理消息	
			//用户操作安全判断
			CMD_GP_GetChartViewTwo * pGetChartViewTwo=(CMD_GP_GetChartViewTwo *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pGetChartViewTwo->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pGetChartViewTwo->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanChartView(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_CHARTVIEWTWOERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			pGetChartViewTwo->szAccount[CountArray(pGetChartViewTwo->szAccount)-1]=0;
			ConvertUtf8ToACP(pGetChartViewTwo->szAccount, NAME_LEN,
				pGetChartViewTwo->szAccount, NAME_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_GetChartViewTwo(pGetChartViewTwo)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError Error;
				Error.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_CHARTVIEWTWOERROR,&Error,sizeof(Error));
				return true;
			}
			//构造数据
			DBR_GP_GetChartViewTwo GetChartViewTwo;
			memcpy(&GetChartViewTwo.RequestData, pGetChartViewTwo,sizeof(CMD_GP_GetChartViewTwo));
			GetChartViewTwo.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			GetChartViewTwo.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(GetChartViewTwo.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			GetChartViewTwo.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_CHARTVIEWTWO,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&GetChartViewTwo,sizeof(DBR_GP_GetChartViewTwo));

			return true;
		}
	case SUB_GP_GET_BETHISTORY:
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetBetHistory));
			if (wDataSize<sizeof(CMD_GP_GetBetHistory)) return false;

			//处理消息	
			//用户操作安全判断
			CMD_GP_GetBetHistory * pGetBetHistory=(CMD_GP_GetBetHistory *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pGetBetHistory->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pGetBetHistory->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanBetHistoryView(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_BETHISTORYERROR,&Error,sizeof(Error));
				return true;
			}
			//处理消息
			pGetBetHistory->szAccount[CountArray(pGetBetHistory->szAccount)-1]=0;
			ConvertUtf8ToACP(pGetBetHistory->szAccount, NAME_LEN,
				pGetBetHistory->szAccount, NAME_LEN);

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_GetBetHistory(pGetBetHistory)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError Error;
				Error.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_LIST_BETHISTORYERROR,&Error,sizeof(Error));
				return true;
			}
			//构造数据
			DBR_GP_GetBetHistory GetBetHistory;
			memcpy(&GetBetHistory.RequestData, pGetBetHistory,sizeof(CMD_GP_GetBetHistory));
			GetBetHistory.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			GetBetHistory.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(GetBetHistory.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			GetBetHistory.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_BETHISTORY,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&GetBetHistory,sizeof(DBR_GP_GetBetHistory));

			return true;
		}
	case SUB_GP_GET_DETAILBETSCORE:
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetDetailBetScore));
			if (wDataSize<sizeof(CMD_GP_GetDetailBetScore)) return false;

			//处理消息	
			//用户操作安全判断
			CMD_GP_GetDetailBetScore * pGetDetailBetScore=(CMD_GP_GetDetailBetScore *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pGetDetailBetScore->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pGetDetailBetScore->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanBetHistoryView(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_GET_DETAILBETSCOREERROR,&Error,sizeof(Error));
				return true;
			}

			//构造数据
			DBR_GP_GetDetailBetScore GetDetailBetScore;
			memcpy(&GetDetailBetScore.RequestData, pGetDetailBetScore,sizeof(CMD_GP_GetDetailBetScore));
			GetDetailBetScore.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			GetDetailBetScore.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(GetDetailBetScore.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			GetDetailBetScore.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_DETAILBETSCORE,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&GetDetailBetScore,sizeof(DBR_GP_GetDetailBetScore));

			return true;
		}
	case SUB_GP_GET_CHARTVIEWTHREE:
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(CMD_GP_GetChartViewThree));
			if (wDataSize<sizeof(CMD_GP_GetChartViewThree)) return false;

			//处理消息	
			//用户操作安全判断
			CMD_GP_GetChartViewThree * pGetChartViewThree=(CMD_GP_GetChartViewThree *)pDataBuffer;
			tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketReadEvent->wIndex;
			GT_ASSERT(pGetChartViewThree->dwOperationUserID == pConnectItemInfo->dwRealUserID);
			if(pGetChartViewThree->dwOperationUserID != pConnectItemInfo->dwRealUserID) return false;
			//判断权限
			if(CMasterRight::CanChartView(pConnectItemInfo->UserData.dwMasterRight) == false)
			{
				//变量定义
				CMD_GP_Error Error;
				Error.wErrorCode = ErrCode_LimitRight;
				//发送数据
				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_CHARTVIEWTHREEERROR,&Error,sizeof(Error));
				return true;
			}

			WORD wErrorCode;
			if((wErrorCode = SimpleCheck_CMD_GP_GetChartViewThree(pGetChartViewThree)) != 0)
			{
				//变量定义
				CMD_GP_SetBasicUserInfoError Error;
				Error.wErrorCode = wErrorCode;
				//发送数据

				m_pITCPSocketEngine->SendData(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID,MDM_GP_GET_RECORD,
					SUB_GP_CHARTVIEWTHREEERROR,&Error,sizeof(Error));
				return true;
			}
			//构造数据
			DBR_GP_GetChartViewThree GetChartViewThree;
			memcpy(&GetChartViewThree.RequestData, pGetChartViewThree,sizeof(CMD_GP_GetChartViewThree));
			GetChartViewThree.Operator.dwOperationUserID   = pConnectItemInfo->dwRealUserID;
			GetChartViewThree.Operator.cbOperationUserType = pConnectItemInfo->cbUserType;
			_tcscpy(GetChartViewThree.Operator.szOperationAccount,pConnectItemInfo->UserData.szAccount);
			GetChartViewThree.Operator.dwIPAddress = pConnectItemInfo->dwClientIP;

			//连接信息
			GT_ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_GET_CHARTVIEWTHREE,pSocketReadEvent->wIndex,
				pSocketReadEvent->wRoundID,&GetChartViewThree,sizeof(DBR_GP_GetChartViewThree));

			return true;
		}
	}
	return false;
}
//列表消息处理
bool CAttemperEngineSink::OnCenterMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize)
{
	GT_ASSERT(Command.wMainCmdID==MDM_CS_SERVER_LIST);
	switch (Command.wSubCmdID)
	{
	case SUB_CS_LIST_INFO:		//列表信息
		{
			//效验参数
			GT_ASSERT(wDataSize==sizeof(CMD_CS_ListInfo));
			if (wDataSize!=sizeof(CMD_CS_ListInfo)) return false;

			//消息处理
			m_ServerList.ResetServerListBuffer();

			return true;
		}
	case SUB_CS_LIST_TYPE:		//种类信息
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(tagGameType));
			GT_ASSERT(wDataSize%sizeof(tagGameType)==0);

			//消息处理
			DWORD dwCount=wDataSize/sizeof(tagGameType);
			tagGameType * pGameType=(tagGameType *)pDataBuffer;
			m_ServerList.AppendGameTypeBuffer(pGameType,dwCount);

			return true;
		}
	case SUB_CS_LIST_KIND:		//类型信息
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(tagGameKind));
			GT_ASSERT(wDataSize%sizeof(tagGameKind)==0);

			//消息处理
			DWORD dwCount=wDataSize/sizeof(tagGameKind);
			tagGameKind * pGameKind=(tagGameKind *)pDataBuffer;
			m_ServerList.AppendGameKindBuffer(pGameKind,dwCount);

			return true;
		}
	case SUB_CS_LIST_STATION:	//站点信息
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(tagGameStation));
			GT_ASSERT(wDataSize%sizeof(tagGameStation)==0);

			//消息处理
			DWORD dwCount=wDataSize/sizeof(tagGameStation);
			tagGameStation * pGameStation=(tagGameStation *)pDataBuffer;
			m_ServerList.AppendGameStationBuffer(pGameStation,dwCount);

			return true;
		}
	case SUB_CS_LIST_SERVER:	//房间信息
		{
			//效验参数
			GT_ASSERT(wDataSize>=sizeof(tagGameServer));
			GT_ASSERT(wDataSize%sizeof(tagGameServer)==0);

			//消息处理
			DWORD dwCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pDataBuffer;
			m_ServerList.AppendGameServerBuffer(pGameServer,dwCount);

			return true;
		}
	case SUB_CS_LIST_FINISH:	//更新完成
		{
			//消息处理
			m_ServerList.ActiveServerListBuffer();
			//更新人数
			WORD wInfoCount=m_ServerList.GetOnLineCountInfo(m_OnLineCountInfo,CountArray(m_OnLineCountInfo));
			m_wOnLineInfoSize=wInfoCount*sizeof(tagOnLineCountInfo);

			//发送信息
			if (m_wOnLineInfoSize>0)
			{
				GT_ASSERT(m_pITCPSocketEngine!=NULL);
				m_pITCPSocketEngine->SendDataBatch(MDM_GP_SERVER_INFO,SUB_GP_ONLINE_COUNT_INFO,&m_OnLineCountInfo,m_wOnLineInfoSize);
			}

			if(m_pIUpdateEventSink)
			{		
				WORD wGameUserCount = 0;
				for(int i = 0; i < wInfoCount; i ++)
				{
					wGameUserCount += m_OnLineCountInfo[i].dwOnLineCount;
				}
				m_pIUpdateEventSink->ShowUserCountNotify(m_wLogonUserCount, wGameUserCount);
			}

			return true;
		}
	}

	return true;
}
//更新信息消息处理
bool CAttemperEngineSink::OnCenterMainUpdateInfo(CMD_Command Command, void * pDataBuffer, WORD wDataSize)
{
	return true;
}
//管理消息处理
bool CAttemperEngineSink::OnCenterMainManagement(CMD_Command Command, void * pDataBuffer, WORD wDataSize)
{
	GT_ASSERT(Command.wMainCmdID==MDM_CS_MANAGEMENT);
	switch (Command.wSubCmdID)
	{
	case SUB_CS_USER_MESSAGE:
		{
			//效验参数
			GT_ASSERT(wDataSize>=(sizeof(SUB_CS_UserMessage)-1024));
			if(wDataSize<(sizeof(SUB_CS_UserMessage)-1024))return false;

			SUB_CS_UserMessage * pUserMessage=(SUB_CS_UserMessage *)pDataBuffer;
			
			CMD_GF_Message		Message;
			Message.wErrorCode	= pUserMessage->wErrorCode;
			Message.wMessageLength	= pUserMessage->wMessageLength;
			Message.wMessageType	= pUserMessage->wMessageType;
			_tcscpy(Message.szContent, pUserMessage->szContent);
			ConvertACPToUtf8(Message.szContent, sizeof(Message.szContent),
				Message.szContent, sizeof(Message.szContent));
			WORD wSendSize = sizeof(Message) - sizeof(Message.szContent) + Message.wMessageLength;

			if(pUserMessage->dwRealUserID == 0)
			{
				m_pITCPSocketEngine->SendDataBatch(MDM_GP_SYSTEM,
					SUB_GF_MESSAGE,&Message,wSendSize);
			}
			else
			{
				for(int i = 0; i < m_pInitParamter->m_wMaxConnect; i ++)
				{
					if(m_pConnectItemInfo[i].cbUserType != INVALID_USERTYPE &&
						m_pConnectItemInfo[i].dwRealUserID == pUserMessage->dwRealUserID &&
						m_pConnectItemInfo[i].cbUserType == pUserMessage->cbUserType)
					{

						m_pITCPSocketEngine->SendData(i,
							m_pConnectItemInfo[i].wRoundID,
							MDM_GP_SYSTEM,
							SUB_GF_MESSAGE,&Message,wSendSize);
						break;
					}
				}				
			}

			return true;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//检查帐号信息
int  CAttemperEngineSink::SimpleCheck_CMD_GP_RegisterUser(CMD_GP_RegisterUser * pRegisterUser)
{
	if(_tcslen(pRegisterUser->szPassWord) == 0)
	{
		return ErrCode_CMD_GP_RegisterError_PasswdEmpty;
	}
	if(_tcslen(pRegisterUser->UserData.szAccount) == 0)
	{
		return ErrCode_CMD_GP_RegisterError_AccountEmpty;
	}
	if(CAIUtil::IsAIUser(pRegisterUser->UserData.szAccount) == true)
	{
		return ErrCode_CMD_GP_RegisterError_AccountEmpty;
	}
	if(_tcslen(pRegisterUser->UserData.szName) == 0)
	{
		return ErrCode_CMD_GP_RegisterError_NameEmpty;
	}
	if(pRegisterUser->UserData.cbLevel > USERLEVEL_GAMEUSER)
	{
		return ErrCode_CMD_GP_RegisterError_LevelInvalid;
	}
	if(pRegisterUser->UserData.fOccupancyRate > 1 || 
		pRegisterUser->UserData.fOccupancyRate < 0)
	{
		return ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid;
	}
	if(pRegisterUser->UserData.fLowOccupancyRate_Max > 1 || 
		pRegisterUser->UserData.fLowOccupancyRate_Max < 0)
	{
		return ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid;
	}
	if(pRegisterUser->UserData.fLowOccupancyRate_Min > 1 || 
		pRegisterUser->UserData.fLowOccupancyRate_Min < 0)
	{
		return ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid;
	}
	if(pRegisterUser->UserData.fLowOccupancyRate_Min > 
		pRegisterUser->UserData.fLowOccupancyRate_Max )
	{
		return ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid;
	}
	if(pRegisterUser->UserData.fRollbackRate > 1 || 
		pRegisterUser->UserData.fRollbackRate < 0)
	{
		return ErrCode_CMD_GP_RegisterError_RollbackRateInvalid;
	}
	if(pRegisterUser->UserData.UserScore.fScore < 0)
	{
		return ErrCode_CMD_GP_RegisterError_UserScoreInvalid;
	}
	if(pRegisterUser->UserData.fTaxOccupancyRate > 1 || 
		pRegisterUser->UserData.fTaxOccupancyRate < 0)
	{
		return ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid;
	}
	return 0;
}

//检查帐号基本信息
int  CAttemperEngineSink::SimpleCheck_CMD_GP_BasicUserInfo(CMD_GP_BasicUserInfo * pBasicUserInfo)
{
	if(_tcslen(pBasicUserInfo->szPassWord) == 0)
	{
		return ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty;
	}
	
	if(_tcslen(pBasicUserInfo->szName) == 0)
	{
		return ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty;
	}

	return 0;
}
//检查帐号详细信息
int  CAttemperEngineSink::SimpleCheck_CMD_GP_DetailUserInfo(CMD_GP_DetailUserInfo * pDetailUserInfo)
{
	if(pDetailUserInfo->fOccupancyRate > 1 || 
		pDetailUserInfo->fOccupancyRate < 0)
	{
		return ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid;
	}
	if(pDetailUserInfo->fLowOccupancyRate_Max > 1 || 
		pDetailUserInfo->fLowOccupancyRate_Max < 0)
	{
		return ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid;
	}
	if(pDetailUserInfo->fLowOccupancyRate_Min > 1 || 
		pDetailUserInfo->fLowOccupancyRate_Min < 0)
	{
		return ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid;
	}
	if(pDetailUserInfo->fLowOccupancyRate_Min > 
		pDetailUserInfo->fLowOccupancyRate_Max )
	{
		return ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid;
	}
	if(pDetailUserInfo->fRollbackRate > 1 || 
		pDetailUserInfo->fRollbackRate < 0)
	{
		return ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid;
	}
	if(pDetailUserInfo->fTaxOccupancyRate > 1 || 
		pDetailUserInfo->fTaxOccupancyRate < 0)
	{
		return ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid;
	}
	return 0;
}
//检查子帐号基本信息
int  CAttemperEngineSink::SimpleCheck_CMD_GP_RegisterSubUser(CMD_GP_RegisterSubUser * pRegisterSubUserInfo)
{
	if(_tcslen(pRegisterSubUserInfo->szAccount) == 0)
	{
		return ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty;
	}

	if(_tcslen(pRegisterSubUserInfo->szPassWord) == 0)
	{
		return ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty;
	}

	if(_tcslen(pRegisterSubUserInfo->szName) == 0)
	{
		return ErrCode_CMD_GP_RegisterSubUserError_NameEmpty;
	}

	return 0;
}
//检查子帐号基本信息
int  CAttemperEngineSink::SimpleCheck_CMD_GP_BasicSubUserInfo(CMD_GP_BasicSubUserInfo * pBasicSubUserInfo)
{
	if(_tcslen(pBasicSubUserInfo->szPassWord) == 0)
	{
		return ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty;
	}

	if(_tcslen(pBasicSubUserInfo->szName) == 0)
	{
		return ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty;
	}

	return 0;
}
//检查自身帐号基本信息
int  CAttemperEngineSink::SimpleCheck_CMD_GP_MySelfBasicUserInfo(CMD_GP_MySelfBasicUserInfo * pBasicUserInfo)
{
	if(_tcslen(pBasicUserInfo->szPassWord) == 0)
	{
		return ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty;
	}
	if(_tcslen(pBasicUserInfo->szNewPassWord) == 0)
	{
		return ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty;
	}
	if(_tcslen(pBasicUserInfo->szNewRealityPassWord) == 0)
	{
		return ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty;
	}
	if(_tcslen(pBasicUserInfo->szName) == 0)
	{
		return ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty;
	}

	return 0;
}
//检查获取操作记录信息
int CAttemperEngineSink::SimpleCheck_CMD_GP_GetOperation(CMD_GP_GetOperation * pGetOperation)
{
	if(_tcslen(pGetOperation->szAccount) == 0)
	{
		return ErrCode_CMD_GP_OperationListError_AccountEmpty;
	}
	if(pGetOperation->wPageSize == 0)
	{
		return ErrCode_CMD_GP_OperationListError_InvalidPageSize;
	}
	if(pGetOperation->fBeginTime >= pGetOperation->fEndTime)
	{
		return ErrCode_CMD_GP_OperationListError_InvalidTime;
	}

	return 0;
}
//检查获取报表记录方式-1信息
int CAttemperEngineSink::SimpleCheck_CMD_GP_GetChartViewOne(CMD_GP_GetChartViewOne * pGetChartViewOne)
{
	if(_tcslen(pGetChartViewOne->szAccount) == 0)
	{
		return ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty;
	}
	if(pGetChartViewOne->fBeginTime >= pGetChartViewOne->fEndTime)
	{
		return ErrCode_CMD_GP_ChartViewOneListError_InvalidTime;
	}

	return 0;
}
//检查获取报表记录方式-2信息
int CAttemperEngineSink::SimpleCheck_CMD_GP_GetChartViewTwo(CMD_GP_GetChartViewTwo * pGetChartViewTwo)
{
	if(_tcslen(pGetChartViewTwo->szAccount) == 0)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty;
	}
	if(pGetChartViewTwo->fBeginTime >= pGetChartViewTwo->fEndTime)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime;
	}
	if(pGetChartViewTwo->wPageSize == 0)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize;
	}
	return 0;
}
//检查获取报表记录方式-3信息
int CAttemperEngineSink::SimpleCheck_CMD_GP_GetChartViewThree(CMD_GP_GetChartViewThree * pGetChartViewThree)
{

	if(pGetChartViewThree->fBeginTime >= pGetChartViewThree->fEndTime)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime;
	}

	return 0;
}
//检查获取游戏记录信息
int CAttemperEngineSink::SimpleCheck_CMD_GP_GetBetHistory(CMD_GP_GetBetHistory * pGetBetHistory)
{
	if(_tcslen(pGetBetHistory->szAccount) == 0)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty;
	}
	if(pGetBetHistory->fBeginTime >= pGetBetHistory->fEndTime)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime;
	}
	if(pGetBetHistory->wPageSize == 0)
	{
		return ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize;
	}
	return 0;
}
//检查用户操作权限
bool CAttemperEngineSink::SimpleCheck_UserRight(BYTE& cbUserType,tagUserData& UserData)
{
	if(cbUserType == USERTYPE_SUB && UserData.cbLevel != 0)
		return false;
	return true;
}