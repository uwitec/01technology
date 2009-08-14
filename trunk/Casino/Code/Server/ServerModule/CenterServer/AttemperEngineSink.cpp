#include "StdAfx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"
//定时器标识
#define IDI_READ_GAMEUSERUPDATEDTEMP				1							//读取用户更新信息临时表
#define IDI_CLEAR_GAMEUSERUPDATEDTEMP				2							//清除用户更新信息临时表
#define TIME_READ_GAMEUSERUPDATEDTEMP				5*30*1000					//读取用户更新信息临时表时间间隔
#define TIME_CLEAR_GAMEUSERUPDATEDTEMP				6*60*1000					//清除用户更新信息临时表时间间隔

CAttemperEngineSink::CAttemperEngineSink()
{
	m_pInitParamter=NULL;
	m_pITimerEngine=NULL;
	m_pIEventService=NULL;
	m_pITCPSocketEngine=NULL;
}

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

	//创建组件
	if (m_ServerListCenter.GetInterface()==NULL)
	{
		if (m_ServerListCenter.CreateInstance()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("中心服务器列表组件创建失败"),
											Level_Exception);
			return false;
		}
		m_ServerListCenter->SetEventService(m_pIEventService);
	}

	//加载列表
	tagDataBaseInfo DataBaseInfo;
	DataBaseInfo.wDataBasePort=m_pInitParamter->m_wServerDataBasePort;
	DataBaseInfo.dwDataBaseAddr=inet_addr(m_pInitParamter->m_szServerDataBaseAddr);
	lstrcpyn(DataBaseInfo.szDataBasePipeName,m_pInitParamter->m_szServerDataBasePipeName,CountArray(DataBaseInfo.szDataBasePipeName));
	lstrcpyn(DataBaseInfo.szDataBaseUser,m_pInitParamter->m_szServerDataBaseUser,CountArray(DataBaseInfo.szDataBaseUser));
	lstrcpyn(DataBaseInfo.szDataBasePass,m_pInitParamter->m_szServerDataBasePass,CountArray(DataBaseInfo.szDataBasePass));
	lstrcpyn(DataBaseInfo.szDataBaseName,m_pInitParamter->m_szServerDataBaseName,CountArray(DataBaseInfo.szDataBaseName));
	if (m_ServerListCenter->LoadServerList(DataBaseInfo)==false) 
	{
		m_pIEventService->ShowEventNotify(TEXT("游戏列表加载失败"),Level_Exception);
		return false;
	}
	m_pITimerEngine->SetTimer(IDI_READ_GAMEUSERUPDATEDTEMP,TIME_READ_GAMEUSERUPDATEDTEMP,DWORD(-1),NULL);
	m_pITimerEngine->SetTimer(IDI_CLEAR_GAMEUSERUPDATEDTEMP,TIME_CLEAR_GAMEUSERUPDATEDTEMP,DWORD(-1),NULL);
	return true;
}

//调度模块关闭
bool __cdecl CAttemperEngineSink::StopService(IUnknownEx * pIUnknownEx)
{
	//重置列表
	if (m_ServerListCenter.GetInterface()!=NULL) 
	{
		m_ServerListCenter->ResetServerList();
	}
	m_pITimerEngine->KillTimer(IDI_READ_GAMEUSERUPDATEDTEMP);
	m_pITimerEngine->KillTimer(IDI_CLEAR_GAMEUSERUPDATEDTEMP);
	return true;
}

//事件处理接口
bool __cdecl CAttemperEngineSink::OnAttemperEvent(BYTE cbThreadIndex,WORD wIdentifier, 
												  void * pDataBuffer, 
												  WORD wDataSize, 
												  DWORD dwInsertTime)
{
	return false;
}

//设置事件
bool CAttemperEngineSink::SetEventService(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);

	//设置组件
	if (m_ServerListCenter.GetInterface()!=NULL)
	{
		m_ServerListCenter->SetEventService(m_pIEventService);
	}

	return (m_pIEventService!=NULL);
}

//配置函数
bool CAttemperEngineSink::InitAttemperSink(CInitParamter * pInitParamter, 
										   IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(pInitParamter!=NULL);

	//设置变量
	m_pInitParamter=pInitParamter;

	//服务引擎
	IServiceEngine * pIServiceEngine=(IServiceEngine *)pIUnknownEx->QueryInterface(IID_IServiceEngine,VER_IServiceEngine);
	if (pIServiceEngine==NULL)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取服务引擎服务失败"),
										Level_Exception);
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
		m_pIEventService->ShowEventNotify(TEXT("获取网络引擎服务失败"),
										Level_Exception);
		return false;
	}

	//数据库引擎
	IDataBaseEngine * pIDataBaseEngine=(IDataBaseEngine *)pIServiceEngine->GetDataBaseEngine(IID_IDataBaseEngine,VER_IDataBaseEngine);
	if (pIDataBaseEngine==NULL)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取数据库引擎服务失败"),
										Level_Exception);
		return false;
	}

	//数据库触发
	IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
	if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("获取数据库触发服务失败"),
											Level_Exception);
		return false;
	}

	return true;
}

//定时器事件
bool __cdecl CAttemperEngineSink::OnEventTimer(BYTE cbThreadIndex,WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_READ_GAMEUSERUPDATEDTEMP:		//读取用户更新信息临时表
		{
			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GC_READ_GAMEUSERUPDATEDTEMP,0,
				0,0,0);
			return true;
		}
	case IDI_CLEAR_GAMEUSERUPDATEDTEMP:		//清除用户更新信息临时表
		{
			__time64_t tm = _time64(NULL) - TIME_CLEAR_GAMEUSERUPDATEDTEMP/1000;

			DBR_GC_Clear_GameUserUpdatedTemp dbr;
			dbr.tmBeforeTime = tm;
			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GC_CLEAR_GAMEUSERUPDATEDTEMP,0,
				0,&dbr,sizeof(DBR_GC_Clear_GameUserUpdatedTemp));
			return true;
		}

	}
	return false;
}

//数据库事件
bool __cdecl CAttemperEngineSink::OnEventDataBase(BYTE cbThreadIndex,
												  void * pDataBuffer, 
												  WORD wDataSize, 
												  NTY_DataBaseEvent * pDataBaseEvent)
{
	switch(pDataBaseEvent->wRequestID)
	{
	case DBR_GC_UPDATE_SERVERUSERDATA://更新用户信息
			return OnDBUpdateServerUserData(pDataBuffer,wDataSize,pDataBaseEvent);

	default:
		break;
	}
	return false;
}

//网络应答事件
bool __cdecl CAttemperEngineSink::OnEventSocketAccept(BYTE cbThreadIndex,NTY_SocketAcceptEvent * pSocketAcceptEvent)
{
	m_pITCPSocketEngine->AllowBatchSend(pSocketAcceptEvent->wIndex, pSocketAcceptEvent->wRoundID, TRUE);
	return true;
}

//网络读取事件
bool __cdecl CAttemperEngineSink::OnEventSocketRead(BYTE cbThreadIndex,CMD_Command Command, 
													void * pDataBuffer, 
													WORD wDataSize, 
													NTY_SocketReadEvent * pSocketReadEvent)
{
	switch (Command.wMainCmdID)
	{
	//列表消息
	case MDM_CS_SERVER_LIST:			
		{
			return OnSocketMainServerList(Command,
									pDataBuffer,
									wDataSize,
									pSocketReadEvent);
		}

	//房间管理
	case MDM_CS_SERVER_MANAGER:			
		{
			return OnSocketMainServerManager(Command,
									pDataBuffer,
									wDataSize,
									pSocketReadEvent);
		}
	//请求信息
	case MDM_CS_SERVER_REQUEST:			
		{
			return OnSocketMainServerRequest(Command,
									pDataBuffer,
									wDataSize,
									pSocketReadEvent);
		}
	//管理消息
	case MDM_CS_MANAGEMENT:	
		{
			return OnSocketMainManagement(Command,
									pDataBuffer,
									wDataSize,
									pSocketReadEvent);
		}

	}

	return false;
}

//网络关闭事件
bool __cdecl CAttemperEngineSink::OnEventSocketClose(BYTE cbThreadIndex,NTY_SocketCloseEvent * pSocketCloseEvent)
{
	return true;
}

//列表消息处理
bool CAttemperEngineSink::OnSocketMainServerList(CMD_Command Command, 
												 void * pDataBuffer, 
												 WORD wDataSize, 
												 NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_CS_SERVER_LIST);
	switch (Command.wSubCmdID)
	{
	//获取列表
	case SUB_CS_GET_SERVER_LIST:	
		{
			//变量定义
			WORD wIndex=pSocketReadEvent->wIndex;
			WORD wRountID=pSocketReadEvent->wRoundID;

			//列表信息
			CMD_CS_ListInfo ListInfo;
			memset(&ListInfo,0,sizeof(ListInfo));
			ListInfo.dwTypeCount=m_ServerListCenter->GetGameTypeCount();
			ListInfo.dwKindCount=m_ServerListCenter->GetGameKindCount();
			ListInfo.dwServerCount=m_ServerListCenter->GetGameServerCount();
			ListInfo.dwStationCount=m_ServerListCenter->GetGameStationCount();
			m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_INFO,&ListInfo,sizeof(ListInfo));

			//类型信息
			POSITION Pos=NULL;
			WORD wSendSize=0L;
			BYTE cbBuffer[SOCKET_PACKAGE];
			for (DWORD i=0;i<ListInfo.dwTypeCount;i++)
			{
				if ((wSendSize+sizeof(tagGameType))>sizeof(cbBuffer))
				{
					m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameTypeItem * pGameTypeItem=m_ServerListCenter->EmunGameTypeItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameTypeItem->GameType,sizeof(tagGameType));
				wSendSize+=sizeof(tagGameType);
			}
			if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);

			//种类信息
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwKindCount;i++)
			{
				if ((wSendSize+sizeof(tagGameKind))>sizeof(cbBuffer))
				{
					m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameKindItem * pGameKindItem=m_ServerListCenter->EmunGameKindItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameKindItem->GameKind,sizeof(tagGameKind));
				wSendSize+=sizeof(tagGameKind);
			}
			if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);

			//站点信息
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwStationCount;i++)
			{
				if ((wSendSize+sizeof(tagGameStation))>sizeof(cbBuffer))
				{
					m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameStationItem * pGameStationItem=m_ServerListCenter->EmunGameStationItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameStationItem->GameStation,sizeof(tagGameStation));
				wSendSize+=sizeof(tagGameStation);
			}
			if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);

			//房间信息
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwServerCount;i++)
			{
				if ((wSendSize+sizeof(tagGameServer))>sizeof(cbBuffer))
				{
					m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameServerItem * pGameServerItem=m_ServerListCenter->EmunGameServerItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameServerItem->GameServer,sizeof(tagGameServer));
				wSendSize+=sizeof(tagGameServer);
			}
			if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);

			//发送完成
			m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_FINISH);

			return true;	
		}
	}

	return false;
}

//房间消息处理
bool CAttemperEngineSink::OnSocketMainServerManager(CMD_Command Command, 
													void * pDataBuffer, 
													WORD wDataSize, 
													NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_CS_SERVER_MANAGER);
	switch (Command.wSubCmdID)
	{
	//房间注册
	case SUB_CS_REG_GAME_SERVER:		
		{
			//效验参数
			GT_ASSERT(wDataSize==sizeof(CMD_CS_RegGameServer));
			if (wDataSize!=sizeof(CMD_CS_RegGameServer)) return false;

			//消息处理
			CMD_CS_RegGameServer * pRegGameServer=(CMD_CS_RegGameServer *)pDataBuffer;
			tagGameServer * pGameServer=&pRegGameServer->GameServer;
			tagGameServerItem * pGameServerItem=m_ServerListCenter->SearchGameServer(pGameServer->wKindID,pGameServer->wServerID);
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
				pGameServerItem->wIndex =pSocketReadEvent->wIndex;
				pGameServerItem->wRoundID =pSocketReadEvent->wRoundID;
				CopyMemory(&pGameServerItem->GameServer,pGameServer,sizeof(tagGameServer));
			}
			else
			{
				m_ServerListCenter->InsertGameServer(pGameServer, pSocketReadEvent->wIndex, pSocketReadEvent->wRoundID);
				//m_pITCPSocketEngine->AllowBatchSend(pSocketReadEvent->wIndex, pSocketReadEvent->wRoundID, TRUE);
			}

			return true;
		}

	//注销房间
	case SUB_CS_UNREG_GAME_SERVER:		
		{
			//效验参数
			GT_ASSERT(wDataSize==sizeof(CMD_CS_UnRegGameServer));
			if (wDataSize!=sizeof(CMD_CS_UnRegGameServer)) return false;

			//消息处理
			CMD_CS_UnRegGameServer * pUnRegGameServer=(CMD_CS_UnRegGameServer *)pDataBuffer;
			m_ServerListCenter->DeleteGameServer(pUnRegGameServer->wKindID,pUnRegGameServer->wServerID);
			//m_pITCPSocketEngine->AllowBatchSend(pSocketReadEvent->wIndex, pSocketReadEvent->wRoundID, FALSE);

			return true;
		}

	//更新人数
	case SUB_CS_SERVER_ONLINE_COUNT:	
		{
			//效验参数
			GT_ASSERT(wDataSize==sizeof(CMD_CS_ServerOnLineCount));
			if (wDataSize!=sizeof(CMD_CS_ServerOnLineCount)) return false;

			//消息处理
			CMD_CS_ServerOnLineCount * pServerOnLineCount=(CMD_CS_ServerOnLineCount *)pDataBuffer;
			WORD wKindID=pServerOnLineCount->wKindID;
			WORD wServerID=pServerOnLineCount->wServerID;
			m_ServerListCenter->UpdateServerOnLineCount(wKindID,wServerID,pServerOnLineCount->dwOnLineCount);

			return true;
		}
	}

	return false;
}
//房间消息处理
bool CAttemperEngineSink::OnSocketMainServerRequest(CMD_Command Command, 
													void * pDataBuffer, 
													WORD wDataSize, 
													NTY_SocketReadEvent * pSocketReadEvent)
{
	GT_ASSERT(Command.wMainCmdID==MDM_CS_SERVER_REQUEST);
	switch (Command.wSubCmdID)
	{
		//PlazaServer信息
	case SUB_CS_GET_LOGONSERVERINFO:
		{
			GT_ASSERT(wDataSize == sizeof(CMD_CS_GetLogonServerInfo));
			if(wDataSize == sizeof(CMD_CS_GetLogonServerInfo))
			{
				//变量定义
				WORD wIndex=pSocketReadEvent->wIndex;
				WORD wRountID=pSocketReadEvent->wRoundID;

				CMD_CS_GetLogonServerInfo *pRecvData = (CMD_CS_GetLogonServerInfo*)pDataBuffer;
				//变量定义
				TCHAR szReadData[1024]=TEXT("");
				CXOREncrypt::CrevasseData(pRecvData->szLogonServerPass,szReadData,sizeof(szReadData));
				if(_tcscmp(szReadData, m_pInitParamter->m_szLogonServerPass) == 0)
				{
					//变量定义
					TCHAR szWriteBuffer[800]=TEXT("");

					CMD_CS_LogonServerInfo sendData;
					memset(&sendData, 0, sizeof(CMD_CS_LogonServerInfo));

					CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBaseUser,szWriteBuffer,sizeof(szWriteBuffer));
					lstrcpyn(sendData.szDataBaseUser,szWriteBuffer,CountArray(sendData.szDataBaseUser));
					CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBasePipeName,szWriteBuffer,sizeof(szWriteBuffer));
					lstrcpyn(sendData.szDataBasePipeName,szWriteBuffer,CountArray(sendData.szDataBasePipeName));

					CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBasePass,szWriteBuffer,sizeof(szWriteBuffer));
					lstrcpyn(sendData.szDataBasePass,szWriteBuffer,CountArray(sendData.szDataBasePass));

					sendData.wDataBasePort = m_pInitParamter->m_wServerDataBasePort;
					sendData.dwDataBaseAddr= inet_addr(m_pInitParamter->m_szServerDataBaseAddr);

					m_pITCPSocketEngine->SendData(wIndex,wRountID,
						MDM_CS_SERVER_REQUEST,
						SUB_CS_GET_LOGONSERVERINFO_SUCCESS,
						&sendData,
						sizeof(CMD_CS_LogonServerInfo));
				}
				else
				{
					m_pITCPSocketEngine->SendData(wIndex,wRountID,
						MDM_CS_SERVER_REQUEST,
						SUB_CS_GET_LOGONSERVERINFO_FAILD,
						0,
						0);
				}

				return true;
			}


		}
		break;
		//游戏服务器信息
	case SUB_CS_GET_LOADERSERVERINFO:
		{
			GT_ASSERT(wDataSize == sizeof(CMD_CS_GetLoaderServerInfo));
			if(wDataSize == sizeof(CMD_CS_GetLoaderServerInfo))
			{
				//变量定义
				bool bSuccees = false;
				WORD wIndex=pSocketReadEvent->wIndex;
				WORD wRountID=pSocketReadEvent->wRoundID;
				CMD_CS_GetLoaderServerInfo *pRecvData = (CMD_CS_GetLoaderServerInfo*)pDataBuffer;
				//变量定义
				TCHAR szReadData[1024]=TEXT("");
				CXOREncrypt::CrevasseData(pRecvData->szLoaderServerPass,szReadData,sizeof(szReadData));
				if(_tcscmp(szReadData, m_pInitParamter->m_szLogonServerPass) == 0)
				{
					//变量定义
					TCHAR szWriteBuffer[800]=TEXT("");

					CMD_CS_LoaderServerInfo sendData;
					memset(&sendData, 0, sizeof(CMD_CS_LoaderServerInfo));

					int ProcessType = 0;
					if(ReadGameServerOption(pRecvData->wServerID, 
						&sendData.GameServiceOption,
						ProcessType) == 0 && ProcessType)
					{
						_tcscpy(sendData.szModuleName, GetGameModuleNameByGameProcess(ProcessType));
						sendData.GameServiceOption.wServerType = GAME_GENRE_GOLD;

						CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBasePipeName,szWriteBuffer,sizeof(szWriteBuffer));
						lstrcpyn(sendData.GameUserDBInfo.szDataBasePipeName,szWriteBuffer,CountArray(sendData.GameUserDBInfo.szDataBasePipeName));
						CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBaseUser,szWriteBuffer,sizeof(szWriteBuffer));
						lstrcpyn(sendData.GameUserDBInfo.szDataBaseUser,szWriteBuffer,CountArray(sendData.GameUserDBInfo.szDataBaseUser));
						CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBasePass,szWriteBuffer,sizeof(szWriteBuffer));
						lstrcpyn(sendData.GameUserDBInfo.szDataBasePass,szWriteBuffer,CountArray(sendData.GameUserDBInfo.szDataBasePass));
						sendData.GameUserDBInfo.wDataBasePort = m_pInitParamter->m_wServerDataBasePort;
						sendData.GameUserDBInfo.dwDataBaseAddr= inet_addr(m_pInitParamter->m_szServerDataBaseAddr);

						CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBasePipeName,szWriteBuffer,sizeof(szWriteBuffer));
						lstrcpyn(sendData.GameScoreDBInfo.szDataBasePipeName,szWriteBuffer,CountArray(sendData.GameScoreDBInfo.szDataBasePipeName));
						CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBaseUser,szWriteBuffer,sizeof(szWriteBuffer));
						lstrcpyn(sendData.GameScoreDBInfo.szDataBaseUser,szWriteBuffer,CountArray(sendData.GameScoreDBInfo.szDataBaseUser));
						CXOREncrypt::EncryptData(m_pInitParamter->m_szServerDataBasePass,szWriteBuffer,sizeof(szWriteBuffer));
						lstrcpyn(sendData.GameScoreDBInfo.szDataBasePass,szWriteBuffer,CountArray(sendData.GameScoreDBInfo.szDataBasePass));
						sendData.GameScoreDBInfo.wDataBasePort = m_pInitParamter->m_wServerDataBasePort;
						sendData.GameScoreDBInfo.dwDataBaseAddr= inet_addr(m_pInitParamter->m_szServerDataBaseAddr);

						m_pITCPSocketEngine->SendData(wIndex,wRountID,
							MDM_CS_SERVER_REQUEST,
							SUB_CS_GET_LOADERSERVERINFO_SUCCESS,
							&sendData,
							sizeof(CMD_CS_LoaderServerInfo));
						bSuccees = true;
						

					}

				}

				if(!bSuccees)
				{
					m_pITCPSocketEngine->SendData(wIndex,wRountID,
						MDM_CS_SERVER_REQUEST,
						SUB_CS_GET_LOADERSERVERINFO_FAILD,
						0,
						0);
				}

				return true;

			}
		}
		break;
	}
	return false;
}
//管理消息处理
bool CAttemperEngineSink::OnSocketMainManagement(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
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
			m_pITCPSocketEngine->SendDataBatch(MDM_CS_MANAGEMENT,SUB_CS_USER_MESSAGE,pUserMessage,wDataSize);

			return true;
		}
	}
	return false;
}
int CAttemperEngineSink::ReadGameServerOption(WORD wServerID, 
											  tagGameServiceOption *pOption,
											  int& ProcessType)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_pInitParamter->m_szServerDataBaseAddr,
			m_pInitParamter->m_szServerDataBasePipeName,
			m_pInitParamter->m_wServerDataBasePort,
			m_pInitParamter->m_szServerDataBaseName,
			m_pInitParamter->m_szServerDataBaseUser,
			m_pInitParamter->m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_ReadGameServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)wServerID));

		ServerInfoDB->ExecuteCommand(true);
		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();
		
		if(lReturnCode == 0)
		{
			pOption->wServerID = wServerID;
			ServerInfoDB->GetFieldValue(TEXT("KindID"),pOption->wKindID);
			ServerInfoDB->GetFieldValue(TEXT("TypeID"),pOption->wTypeID);
			ServerInfoDB->GetFieldValue(TEXT("ServerName"),pOption->szGameRoomName, sizeof(pOption->szGameRoomName));
			ServerInfoDB->GetFieldValue(TEXT("MaxUserCount"),pOption->wMaxConnect);
			ServerInfoDB->GetFieldValue(TEXT("TableCount"),pOption->wTableCount);
			ServerInfoDB->GetFieldValue(TEXT("CellScore"),pOption->dwCellScore);
			ServerInfoDB->GetFieldValue(TEXT("HighScore"),pOption->dwHighScore);
			ServerInfoDB->GetFieldValue(TEXT("LessScore"),pOption->dwLessScore);
			ServerInfoDB->GetFieldValue(TEXT("TaxRate"),pOption->decTaxRate);
			ServerInfoDB->GetFieldValue(TEXT("ProcessType"),ProcessType);
			ServerInfoDB->GetFieldValue(TEXT("AIUserCount"),pOption->wAIUserCount);
			ServerInfoDB->GetFieldValue(TEXT("AILevel"),pOption->cbAILevel);

		}
		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
TCHAR* CAttemperEngineSink::GetGameModuleNameByGameProcess(WORD wProcessType)
{
	for(int i = 0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		if(g_GameProcessItem[i].wProcessType == wProcessType)
			return g_GameProcessItem[i].szModuleName; 
	}
	return TEXT("");
}
//更新用户信息
bool CAttemperEngineSink::OnDBUpdateServerUserData(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{
	GT_ASSERT(wDataSize > 0);
	m_pITCPSocketEngine->SendDataBatch(MDM_CS_UPDATE_INFO,
		SUB_CS_UPDATE_SERVERUSERDATA,pDataBuffer,wDataSize);
	return true;
}

//-----------------------------------------------
//					the end
//-----------------------------------------------
