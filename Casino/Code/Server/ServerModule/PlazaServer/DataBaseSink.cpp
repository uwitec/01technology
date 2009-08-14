#include "StdAfx.h"
#include "Commonality.h"
#include "DataBaseSink.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseSink::CDataBaseSink()
{
	m_pInitParamter=NULL;
	m_pIEventService=NULL;
	m_pITCPSocketEngine=NULL;
	m_dwLogonServerAddr = 0;
}

//析构函数
CDataBaseSink::~CDataBaseSink()
{
}

//接口查询
void * __cdecl CDataBaseSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseSink,Guid,dwQueryVer);
	return NULL;
}

//模块启动
bool __cdecl CDataBaseSink::StartService(IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(m_pInitParamter!=NULL);
	GT_ASSERT(m_pIEventService!=NULL);
	GT_ASSERT(m_AttemperEvent.IsValid()==true);

	//创建实例
	if (m_WorkDataBase.GetInterface()!=NULL==false)
	{
		if (m_WorkDataBase.CreateInstance()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("用户数据库对象创建失败"),Level_Exception);
			return false;
		}
	}

	try
	{
		//连接用户数据库
		m_WorkDataBase->SetConnectionInfo(m_pInitParamter->m_szUserDataBaseAddr,
			m_pInitParamter->m_szUserDataBasePipeName,
			m_pInitParamter->m_wUserDataBasePort,
			m_pInitParamter->m_szUserDataBaseName,
			m_pInitParamter->m_szUserDataBaseUser,
			m_pInitParamter->m_szUserDataBasePass);
		m_WorkDataBase->OpenConnection();

		return true;
	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	return false;
}

//模块关闭
bool __cdecl CDataBaseSink::StopService(IUnknownEx * pIUnknownEx)
{
	try
	{
		if (m_WorkDataBase.GetInterface()!=NULL) m_WorkDataBase->CloseConnection();

		return true;
	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	return false;
}

//数据操作处理
bool __cdecl CDataBaseSink::OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	switch (DataBaseEvent.wRequestID)
	{
	case DBR_GP_LOGON_BY_ACCOUNT:		//帐号登录
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_REGISTER_USER:			//注册帐号
		{
			return OnRequestRegister(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_SETUSERSCORE:		//存取额度
		{
			return OnRequestSetUserScore(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_USERCONGEALSTATE:	//状态设定
		{
			return OnRequestSetUserCongealState(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_BASICUSERINFO:		//基本信息设定
		{
			return OnRequestSetBasicUserInfo(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_DETAILUSERINFO:		//详细信息设定
		{
			return OnRequestSetDetailUserInfo(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_USERRIGHT:			//权限设定
		{
			return OnRequestSetUserRight(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_MYSELFBASICUSERINFO://自身资料修改
		{
			return OnRequestSetMySelfBasicUserInfo(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_SUBUSERCONGEALSTATE:	//子帐号状态设定
		{
			return OnRequestSetSubUserCongealState(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_REGISTER_SUBUSER:			//注册子帐号
		{
			return OnRequestRegisterSubUser(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_SET_BASICSUBUSERINFO:		//子帐号基本信息设定
		{
			return OnRequestSetBasicSubUserInfo(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_LOWUSERLIST:		 //获取下线帐号
		{
			return OnRequestGetLessUserList(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_SUBUSERLIST:		//获取子帐号
		{
			return OnRequestGetSubUserList(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_OPERATION:		//获取操作记录
		{
			return OnRequestGetOperationList(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_CHARTVIEWONE:		//获取报表记录方式-1
		{
			return OnRequestGetChartViewOneList(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_CHARTVIEWTWO:		//获取报表记录方式-2
		{
			return OnRequestGetChartViewTwoList(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_BETHISTORY:			//获取游戏局记录
		{
			return OnRequestGetBetHistoryList(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_DETAILBETSCORE:		//获取游戏局记录详细投注
		{
			return OnRequestGetDetailBetScore(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_GET_CHARTVIEWTHREE:		//获取报表记录方式-3
		{
			return OnRequestGetChartViewThree(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_UPDATE_GAMEMAININFO:	//更新游戏信息
		{
			return OnRequestUpdateGameMainInfo(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_LOGOUT_USER:		//注销帐号登陆
		{
			return OnRequestLogout(DataBaseEvent,pDataBuffer,wDataSize);
		}
	}

	return false;
}

//设置事件
bool CDataBaseSink::SetEventService(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);
	return (m_pIEventService!=NULL);
}
//设置接口
bool  CDataBaseSink::SetSocketEngine(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pITCPSocketEngine=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITCPSocketEngine);
	GT_ASSERT(m_pITCPSocketEngine!=NULL);
	return (m_pITCPSocketEngine!=NULL);
}
//配置函数
bool CDataBaseSink::InitDataBaseSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(pInitParamter!=NULL);

	//设置变量
	m_pInitParamter=pInitParamter;
	m_dwLogonServerAddr = inet_addr(m_pInitParamter->m_szLogonServerAddr);

	//查询接口
	IServiceEngine * pIServiceEngine=(IServiceEngine *)pIUnknownEx->QueryInterface(IID_IServiceEngine,VER_IServiceEngine);
	GT_ASSERT(pIServiceEngine!=NULL);
	if (pIServiceEngine==NULL) throw TEXT("服务引擎接口查询失败");

	//获取逻辑引擎
	IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)pIServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
	GT_ASSERT(pIAttemperEngine!=NULL);
	if (pIAttemperEngine==NULL) throw TEXT("调度引擎接口查询失败");

	//设置通知组件
	IUnknownEx * pIQueueService=(IUnknownEx *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
	if (m_AttemperEvent.SetQueueService(pIQueueService)==false) throw TEXT("逻辑引擎通知接口设置失败");

	return true;
}

//登录请求处理
bool CDataBaseSink::OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//登录处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_LOGON_BY_ACCOUNT:		//帐号登录
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_LogonByAccount));
				if (wDataSize!=sizeof(DBR_GP_LogonByAccount)) return false;

				//执行查询
				DBR_GP_LogonByAccount * pLogonByAccount=(DBR_GP_LogonByAccount *)pDataBuffer;
				lReturnValue=SPLogonByAccount(pLogonByAccount->szAccounts,
					pLogonByAccount->szPassWord,
					pLogonByAccount->dwClientIP,
					m_dwLogonServerAddr,
					m_pInitParamter->m_wListenPort);

				//结果判断
				if (lReturnValue==0L)
				{
					//登录成功
					DBR_GP_LogonSuccess LogonSuccess;
					memset(&LogonSuccess,0,sizeof(LogonSuccess));
					helper_DBGetFieldValue_UserData(m_WorkDataBase, LogonSuccess.UserData,
						&LogonSuccess.dwRealUserID,
						&LogonSuccess.cbUserType);

					if((lReturnValue = helper_AllowClientLogon(pLogonByAccount->cbClientType,
						LogonSuccess.cbUserType,LogonSuccess.UserData.cbLevel)) == 0)
					{
						m_AttemperEvent.PostDataBaseEvent(DBR_GP_LOGON_SUCCESS,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
							&LogonSuccess,sizeof(LogonSuccess));
						
						//操作记录
						tagOperationData OperationData;
						//初试化操作数据
						helper_InitOperationData(OperationData, OPT_Logon, pLogonByAccount->dwClientIP);
						//填充操作数据包头
						helper_FillOperationHeader(OperationData,
							LogonSuccess.UserData.dwUserID,
							LogonSuccess.cbUserType,
							LogonSuccess.UserData.szAccount,
							LogonSuccess.UserData.dwUserID,
							LogonSuccess.cbUserType,
							LogonSuccess.UserData.szAccount);
						//写入操作记录
						SPInsertOperationInfo(OperationData);
					}
					else
					{
						//执行注销登陆操作
						DBR_GP_LogoutUser LogoutUser;
						LogoutUser.cbUserType = LogonSuccess.cbUserType;
						LogoutUser.dwRealUserID = LogonSuccess.dwRealUserID;
						DBR_GP_LogoutUser * pLogoutUser=&LogoutUser;
						SPLogoutUser(m_dwLogonServerAddr,
							m_pInitParamter->m_wListenPort,pLogoutUser);

					}

				}

				if(lReturnValue != 0L)
				{
					//登录失败
					DBR_GP_LogonError LogonError;
					LogonError.wErrorCode=lReturnValue;
					
					m_AttemperEvent.PostDataBaseEvent(DBR_GP_LOGON_ERROR,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
						&LogonError,sizeof(LogonError));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		DBR_GP_LogonError LogonError;
		LogonError.wErrorCode=ErrCode_Database;
		m_AttemperEvent.PostDataBaseEvent(DBR_GP_LOGON_ERROR,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
			&LogonError,sizeof(LogonError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}

//注册请求处理
bool CDataBaseSink::OnRequestRegister(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;
	
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_REGISTER_USER:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_RegisterUser));
				if (wDataSize!=sizeof(DBR_GP_RegisterUser)) return false;

				//执行查询
				DBR_GP_RegisterUser * pRegisterUser=(DBR_GP_RegisterUser *)pDataBuffer;
				lReturnValue=SPRegisterUser(&pRegisterUser->UserData,pRegisterUser->szPassWord,
					pRegisterUser->dwClientIP);
			
				if(lReturnValue == 0)
				{
					//操作成功,发送结果
					CMD_GP_RegisterSuccess RegisterSuccess;
					helper_DBGetFieldValue_UserData(m_WorkDataBase, RegisterSuccess.UserData);

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_NewAccount, pRegisterUser->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						RegisterSuccess.UserData.dwUserID,
						USERTYPE_GAME,
						RegisterSuccess.UserData.szAccount,
						pRegisterUser->Operator.dwOperationUserID,
						pRegisterUser->Operator.cbOperationUserType,
						pRegisterUser->Operator.szOperationAccount);
					OperationData.Content.data_NewAccount.cbLevel = RegisterSuccess.UserData.cbLevel;
					OperationData.Content.data_NewAccount.dwBetLimit = RegisterSuccess.UserData.dwBetLimit;
					OperationData.Content.data_NewAccount.fOccupancyRate = RegisterSuccess.UserData.fOccupancyRate;
					OperationData.Content.data_NewAccount.cbOccupancyRate_NoFlag = RegisterSuccess.UserData.cbOccupancyRate_NoFlag;
					OperationData.Content.data_NewAccount.fRollbackRate  = RegisterSuccess.UserData.fRollbackRate;
					OperationData.Content.data_NewAccount.fTaxOccupancyRate = RegisterSuccess.UserData.fTaxOccupancyRate;
					//写入操作记录
					SPInsertOperationInfo(OperationData);

					ConvertACPToUtf8(RegisterSuccess.UserData.szAccount, NAME_LEN,
						RegisterSuccess.UserData.szAccount, NAME_LEN);
					ConvertACPToUtf8(RegisterSuccess.UserData.szName, NAME_LEN,
						RegisterSuccess.UserData.szName, NAME_LEN);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_REGISTER_SUCCESS,
						&RegisterSuccess,
						sizeof(CMD_GP_RegisterSuccess));

					if(pRegisterUser->UserData.UserScore.fScore > 0)
					{
						//存入额度
						NTY_DataBaseEvent DataBaseEvent_SetUserScore;
						memcpy(&DataBaseEvent_SetUserScore, &DataBaseEvent, sizeof(NTY_DataBaseEvent));
						DataBaseEvent_SetUserScore.wRequestID = DBR_GP_SET_SETUSERSCORE;
						
						DBR_GP_SetUserScore SetUserScore;
						memcpy(&SetUserScore.Operator, &pRegisterUser->Operator, sizeof(pRegisterUser->Operator));
						SetUserScore.RequestData.dwOperationUserID = pRegisterUser->dwOperationUserID;
						SetUserScore.RequestData.dwParentUserID	  = RegisterSuccess.UserData.dwParentUserID;
						SetUserScore.RequestData.dwUserID		  = RegisterSuccess.UserData.dwUserID;
						SetUserScore.RequestData.fScore			  = pRegisterUser->UserData.UserScore.fScore;
						//关闭记录集
						m_WorkDataBase->CloseRecordset();
						OnRequestSetUserScore(DataBaseEvent_SetUserScore, &SetUserScore, sizeof(DBR_GP_SetUserScore));
					}

					
				}
				else
				{
					//操作失败
					CMD_GP_RegisterError RegisterError;
					RegisterError.wErrorCode=lReturnValue + ErrCode_CMD_GP_RegisterError_UserScoreInvalid;

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_REGISTER_ERROR,
						&RegisterError,
						sizeof(CMD_GP_RegisterError));
	
				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_RegisterError RegisterError;
		RegisterError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_REGISTER_ERROR,
			&RegisterError,
			sizeof(CMD_GP_RegisterError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//存取额度请求处理
bool CDataBaseSink::OnRequestSetUserScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_SETUSERSCORE:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetUserScore));
				if (wDataSize!=sizeof(DBR_GP_SetUserScore)) return false;

				//执行查询操作
				DBR_GP_SetUserScore * pSetUserScore=(DBR_GP_SetUserScore *)pDataBuffer;
				lReturnValue=SPSetUserScore(pSetUserScore->RequestData.dwParentUserID,pSetUserScore->RequestData.dwUserID,
					pSetUserScore->RequestData.fScore);

				if(lReturnValue == 0)
				{
					DOUBLE fSetScoreOffset = 0;
					DOUBLE fScoreResult = 0;
					TCHAR  szAccount[NAME_LEN];
					m_WorkDataBase->GetFieldValue(TEXT("fSetScoreOffset"),fSetScoreOffset);
					m_WorkDataBase->GetFieldValue(TEXT("fScoreResult"),fScoreResult);
					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);
					
					CMD_GP_SetUserScoreSuccess SetUserScoreSuccess;
					
					SetUserScoreSuccess.dwParentUserID	  = pSetUserScore->RequestData.dwParentUserID;
					SetUserScoreSuccess.dwUserID		  = pSetUserScore->RequestData.dwUserID;
					SetUserScoreSuccess.fScore			  = fScoreResult;
					SetUserScoreSuccess.fScoreOffset	  = fSetScoreOffset;

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
							MDM_GP_EDIT_USERINFO,
							SUB_GP_SET_SETUSERSCORE_SUCCESS,
							&SetUserScoreSuccess,
							sizeof(CMD_GP_SetUserScoreSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetScore, pSetUserScore->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetUserScore->RequestData.dwUserID,
						USERTYPE_GAME,
						szAccount,
						pSetUserScore->Operator.dwOperationUserID,
						pSetUserScore->Operator.cbOperationUserType,
						pSetUserScore->Operator.szOperationAccount);
					OperationData.Content.data_SetScore.fScoreResult	= fScoreResult;
					OperationData.Content.data_SetScore.fSetScoreOffset	= fSetScoreOffset;
					
					//写入操作记录
					SPInsertOperationInfo(OperationData);

				}
				else
				{

					//操作失败
					CMD_GP_SetUserScoreError SetUserScoreError;
					SetUserScoreError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetUserScoreError_ScoreNegative;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_SETUSERSCORE_ERROR,
						&SetUserScoreError,
						sizeof(CMD_GP_SetUserScoreError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetUserScoreError SetUserScoreError;
		SetUserScoreError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_SET_SETUSERSCORE_ERROR,
			&SetUserScoreError,
			sizeof(CMD_GP_SetUserScoreError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//状态设定请求处理
bool CDataBaseSink::OnRequestSetUserCongealState(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_USERCONGEALSTATE:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetUserCongealState));
				if (wDataSize!=sizeof(DBR_GP_SetUserCongealState)) return false;
				DBR_GP_SetUserCongealState * pSetUserCongealState=(DBR_GP_SetUserCongealState *)pDataBuffer;
				GT_ASSERT(pSetUserCongealState->RequestData.cbLevel <= USERLEVEL_GAMEUSER);
				if(pSetUserCongealState->RequestData.cbLevel > USERLEVEL_GAMEUSER) return false;

				static const DWORD dwCongealStateMark[USERLEVEL_GAMEUSER + 1] = 
				{
					0x1,
					0x2,
					0x4,
					0x8,
					0x10,
					0x20
				};
				static const DWORD dwNormalStateMark[USERLEVEL_GAMEUSER + 1] = 
				{
					0xFFFFFFFE,
					0xFFFFFFFD,
					0xFFFFFFFB,
					0xFFFFFFF7,
					0xFFFFFFEF,
					0xFFFFFFDF,
				};
				DWORD dwStateMark;
				if(pSetUserCongealState->RequestData.cbState)
				{
					dwStateMark = dwCongealStateMark[pSetUserCongealState->RequestData.cbLevel];
				}
				else
				{
					dwStateMark = dwNormalStateMark[pSetUserCongealState->RequestData.cbLevel];
				}

				//执行操作
				
				lReturnValue=SPSetUserCongealState(pSetUserCongealState->RequestData.dwUserID,
					pSetUserCongealState->RequestData.cbLevel,
					pSetUserCongealState->RequestData.cbState,
					dwStateMark);

				if(lReturnValue == 0)
				{
					TCHAR  szAccount[NAME_LEN];
					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);

					CMD_GP_SetUserCongealStateSuccess SetUserCongealStateSuccess;

					SetUserCongealStateSuccess.dwUserID		  = pSetUserCongealState->RequestData.dwUserID;
					SetUserCongealStateSuccess.cbState		  = pSetUserCongealState->RequestData.cbState;
					SetUserCongealStateSuccess.dwStateMark	  = dwStateMark;

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_USERCONGEALSTATE_SUCCESS,
						&SetUserCongealStateSuccess,
						sizeof(CMD_GP_SetUserCongealStateSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetCongealState, pSetUserCongealState->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetUserCongealState->RequestData.dwUserID,
						USERTYPE_GAME,
						szAccount,
						pSetUserCongealState->Operator.dwOperationUserID,
						pSetUserCongealState->Operator.cbOperationUserType,
						pSetUserCongealState->Operator.szOperationAccount);
					OperationData.Content.data_SetCongealState.cbState	= pSetUserCongealState->RequestData.cbState;
					OperationData.Content.data_SetCongealState.dwStateMark	= dwStateMark;

					//写入操作记录
					SPInsertOperationInfo(OperationData);

				}
				else
				{

					//操作失败
					CMD_GP_SetUserCongealStateError SetUserCongealStateError;
					SetUserCongealStateError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_USERCONGEALSTATE_ERROR,
						&SetUserCongealStateError,
						sizeof(CMD_GP_SetUserCongealStateError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetUserCongealStateError SetUserCongealStateError;
		SetUserCongealStateError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_SET_USERCONGEALSTATE_ERROR,
			&SetUserCongealStateError,
			sizeof(CMD_GP_SetUserCongealStateError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//基本信息设定请求处理
bool CDataBaseSink::OnRequestSetBasicUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_BASICUSERINFO:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetBasicUserInfo));
				if (wDataSize!=sizeof(DBR_GP_SetBasicUserInfo)) return false;

				DBR_GP_SetBasicUserInfo * pSetBasicUserInfo=(DBR_GP_SetBasicUserInfo *)pDataBuffer;
			
				//执行操作
				lReturnValue=SPSetBaiscUserInfo(pSetBasicUserInfo->RequestData.dwUserID,
					pSetBasicUserInfo->RequestData.szName,
					pSetBasicUserInfo->RequestData.szPassWord,
					pSetBasicUserInfo->RequestData.cbFaceID);

				if(lReturnValue == 0)
				{
					TCHAR  szAccount[NAME_LEN];
					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);

					CMD_GP_SetBasicUserInfoSuccess SetBasicUserInfoSuccess;

					SetBasicUserInfoSuccess.dwUserID		  = pSetBasicUserInfo->RequestData.dwUserID;
					SetBasicUserInfoSuccess.cbFaceID		  = pSetBasicUserInfo->RequestData.cbFaceID;
					_tcscpy(SetBasicUserInfoSuccess.szName, pSetBasicUserInfo->RequestData.szName);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_BASICUSERINFO_SUCCESS,
						&SetBasicUserInfoSuccess,
						sizeof(CMD_GP_SetBasicUserInfoSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetBasicInfo, pSetBasicUserInfo->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetBasicUserInfo->RequestData.dwUserID,
						USERTYPE_GAME,
						szAccount,
						pSetBasicUserInfo->Operator.dwOperationUserID,
						pSetBasicUserInfo->Operator.cbOperationUserType,
						pSetBasicUserInfo->Operator.szOperationAccount);
					_tcscpy(OperationData.Content.data_SetBasicInfo.szName, pSetBasicUserInfo->RequestData.szName);

					//写入操作记录
					SPInsertOperationInfo(OperationData);

				}
				else
				{

					//操作失败
					CMD_GP_SetBasicUserInfoError SetBasicUserInfoError;
					SetBasicUserInfoError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_BASICUSERINFO_ERROR,
						&SetBasicUserInfoError,
						sizeof(CMD_GP_SetBasicUserInfoError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetBasicUserInfoError SetBasicUserInfoError;
		SetBasicUserInfoError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_SET_BASICUSERINFO_ERROR,
			&SetBasicUserInfoError,
			sizeof(CMD_GP_SetBasicUserInfoError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//详细信息设定请求处理
bool CDataBaseSink::OnRequestSetDetailUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_DETAILUSERINFO:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetDetailUserInfo));
				if (wDataSize!=sizeof(DBR_GP_SetDetailUserInfo)) return false;

				DBR_GP_SetDetailUserInfo * pSetDetailUserInfo=(DBR_GP_SetDetailUserInfo *)pDataBuffer;

				//执行操作
				lReturnValue=SPSetDetailUserInfo(pSetDetailUserInfo);

				if(lReturnValue == 0)
				{
					TCHAR		szAccount[NAME_LEN];
					DOUBLE		fOriOccupancyRate;						//占成比
					BYTE		cbOriOccupancyRate_NoFlag;				//不占成比标志
					DOUBLE		fOriLowOccupancyRate_Max;				//下线最高占成比
					BYTE		cbOriLowOccupancyRate_Max_NoFlag;		//下线最高不占成比标志
					DOUBLE		fOriLowOccupancyRate_Min;				//下线最低占成比
					BYTE		cbOriLowOccupancyRate_Min_NoFlag;		//下线最低不占成比标志
					DOUBLE		fOriTaxOccupancyRate;				//抽水占成比
					DOUBLE		fOriRollbackRate;						//洗码比
					DWORD		dwOriBetLimit;							//上下限

					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);
					m_WorkDataBase->GetFieldValue(TEXT("fOriOccupancyRate"),fOriOccupancyRate);
					m_WorkDataBase->GetFieldValue(TEXT("cbOriOccupancyRate_NoFlag"),cbOriOccupancyRate_NoFlag);
					m_WorkDataBase->GetFieldValue(TEXT("fOriLowOccupancyRate_Max"),fOriLowOccupancyRate_Max);
					m_WorkDataBase->GetFieldValue(TEXT("cbOriLowOccupancyRate_Max_NoFlag"),cbOriLowOccupancyRate_Max_NoFlag);
					m_WorkDataBase->GetFieldValue(TEXT("fOriLowOccupancyRate_Min"),fOriLowOccupancyRate_Min);
					m_WorkDataBase->GetFieldValue(TEXT("cbOriLowOccupancyRate_Min_NoFlag"),cbOriLowOccupancyRate_Min_NoFlag);
					m_WorkDataBase->GetFieldValue(TEXT("fOriTaxOccupancyRate"),fOriTaxOccupancyRate);
					m_WorkDataBase->GetFieldValue(TEXT("fOriRollbackRate"),fOriRollbackRate);
					m_WorkDataBase->GetFieldValue(TEXT("dwOriBetLimit"),dwOriBetLimit);

					CMD_GP_SetDetailUserInfoSuccess SetDetailUserInfoSuccess;

					SetDetailUserInfoSuccess.dwUserID			= pSetDetailUserInfo->RequestData.dwUserID;
					SetDetailUserInfoSuccess.fOccupancyRate		= pSetDetailUserInfo->RequestData.fOccupancyRate;
					SetDetailUserInfoSuccess.cbOccupancyRate_NoFlag		= pSetDetailUserInfo->RequestData.cbOccupancyRate_NoFlag;
					SetDetailUserInfoSuccess.fLowOccupancyRate_Max		= pSetDetailUserInfo->RequestData.fLowOccupancyRate_Max;
					SetDetailUserInfoSuccess.cbLowOccupancyRate_Max_NoFlag		= pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Max_NoFlag;
					SetDetailUserInfoSuccess.fLowOccupancyRate_Min		= pSetDetailUserInfo->RequestData.fLowOccupancyRate_Min;
					SetDetailUserInfoSuccess.cbLowOccupancyRate_Min_NoFlag		= pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Min_NoFlag;
					SetDetailUserInfoSuccess.fTaxOccupancyRate		= pSetDetailUserInfo->RequestData.fTaxOccupancyRate;
					SetDetailUserInfoSuccess.fRollbackRate		= pSetDetailUserInfo->RequestData.fRollbackRate;
					SetDetailUserInfoSuccess.dwBetLimit		= pSetDetailUserInfo->RequestData.dwBetLimit;
	
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_DETAILUSERINFO_SUCCESS,
						&SetDetailUserInfoSuccess,
						sizeof(CMD_GP_SetDetailUserInfoSuccess));

					if(pSetDetailUserInfo->RequestData.fOccupancyRate != fOriOccupancyRate ||
						pSetDetailUserInfo->RequestData.cbOccupancyRate_NoFlag != cbOriOccupancyRate_NoFlag ||
						pSetDetailUserInfo->RequestData.fLowOccupancyRate_Max != fOriLowOccupancyRate_Max ||
						pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Max_NoFlag != cbOriLowOccupancyRate_Max_NoFlag ||
						pSetDetailUserInfo->RequestData.fLowOccupancyRate_Min != fOriLowOccupancyRate_Min ||
						pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Min_NoFlag != cbOriLowOccupancyRate_Min_NoFlag||
						pSetDetailUserInfo->RequestData.fTaxOccupancyRate != fOriTaxOccupancyRate)
					{
						//操作记录
						tagOperationData OperationData;
						//初试化操作数据
						helper_InitOperationData(OperationData, OPT_SetOccupancyRate,
							pSetDetailUserInfo->Operator.dwIPAddress);
						//填充操作数据包头
						helper_FillOperationHeader(OperationData,
						pSetDetailUserInfo->RequestData.dwUserID,
						USERTYPE_GAME,
						szAccount,
						pSetDetailUserInfo->Operator.dwOperationUserID,
						pSetDetailUserInfo->Operator.cbOperationUserType,
						pSetDetailUserInfo->Operator.szOperationAccount);

						OperationData.Content.data_SetOccupancyRate.fOccupancyRate		= pSetDetailUserInfo->RequestData.fOccupancyRate;
						OperationData.Content.data_SetOccupancyRate.cbOccupancyRate_NoFlag		= pSetDetailUserInfo->RequestData.cbOccupancyRate_NoFlag;
						OperationData.Content.data_SetOccupancyRate.fLowOccupancyRate_Max		= pSetDetailUserInfo->RequestData.fLowOccupancyRate_Max;
						OperationData.Content.data_SetOccupancyRate.cbLowOccupancyRate_Max_NoFlag		= pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Max_NoFlag;
						OperationData.Content.data_SetOccupancyRate.fLowOccupancyRate_Min		= pSetDetailUserInfo->RequestData.fLowOccupancyRate_Min;
						OperationData.Content.data_SetOccupancyRate.cbLowOccupancyRate_Min_NoFlag		= pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Min_NoFlag;
						OperationData.Content.data_SetOccupancyRate.fTaxOccupancyRate		= pSetDetailUserInfo->RequestData.fTaxOccupancyRate;

						OperationData.Content.data_SetOccupancyRate.fOldOccupancyRate				= fOriOccupancyRate;
						OperationData.Content.data_SetOccupancyRate.cbOldOccupancyRate_NoFlag		= cbOriOccupancyRate_NoFlag;
						OperationData.Content.data_SetOccupancyRate.fOldLowOccupancyRate_Max		= fOriLowOccupancyRate_Max;
						OperationData.Content.data_SetOccupancyRate.cbOldLowOccupancyRate_Max_NoFlag= cbOriLowOccupancyRate_Max_NoFlag;
						OperationData.Content.data_SetOccupancyRate.fOldLowOccupancyRate_Min		= fOriLowOccupancyRate_Min;
						OperationData.Content.data_SetOccupancyRate.cbOldLowOccupancyRate_Min_NoFlag= cbOriLowOccupancyRate_Min_NoFlag;
						OperationData.Content.data_SetOccupancyRate.fOldTaxOccupancyRate		= fOriTaxOccupancyRate;

						//写入操作记录
						SPInsertOperationInfo(OperationData);
					}
					if(pSetDetailUserInfo->RequestData.fRollbackRate != fOriRollbackRate)
					{
						//操作记录
						tagOperationData OperationData;
						//初试化操作数据
						helper_InitOperationData(OperationData, OPT_SetRollbackRate,
							pSetDetailUserInfo->Operator.dwIPAddress);
						//填充操作数据包头
						helper_FillOperationHeader(OperationData,
							pSetDetailUserInfo->RequestData.dwUserID,
							USERTYPE_GAME,
							szAccount,
							pSetDetailUserInfo->Operator.dwOperationUserID,
							pSetDetailUserInfo->Operator.cbOperationUserType,
							pSetDetailUserInfo->Operator.szOperationAccount);

						OperationData.Content.data_SetRollbackRate.fRollbackRate		= pSetDetailUserInfo->RequestData.fRollbackRate;
						OperationData.Content.data_SetRollbackRate.fOldRollbackRate		= fOriRollbackRate;

						//写入操作记录
						SPInsertOperationInfo(OperationData);
					}

				}
				else
				{

					//操作失败
					CMD_GP_SetDetailUserInfoError SetDetailUserInfoError;
					SetDetailUserInfoError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_DETAILUSERINFO_ERROR,
						&SetDetailUserInfoError,
						sizeof(CMD_GP_SetDetailUserInfoError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetDetailUserInfoError SetDetailUserInfoError;
		SetDetailUserInfoError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_SET_DETAILUSERINFO_ERROR,
			&SetDetailUserInfoError,
			sizeof(CMD_GP_SetDetailUserInfoError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//权限设定请求处理
bool CDataBaseSink::OnRequestSetUserRight(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_USERRIGHT:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetUserRight));
				if (wDataSize!=sizeof(DBR_GP_SetUserRight)) return false;

				DBR_GP_SetUserRight * pSetUserRight=(DBR_GP_SetUserRight *)pDataBuffer;

				//执行操作
				lReturnValue=SPSetUserRight(pSetUserRight);

				if(lReturnValue == 0)
				{
					TCHAR  szAccount[NAME_LEN];
					DWORD dwOldUserRight;
					DWORD dwOldMasterRight;
					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);
					m_WorkDataBase->GetFieldValue(TEXT("dwOldUserRight"),dwOldUserRight);
					m_WorkDataBase->GetFieldValue(TEXT("dwOldMasterRight"),dwOldMasterRight);

					CMD_GP_SetUserRightSuccess SetUserRightSuccess;

					SetUserRightSuccess.dwUserID			= pSetUserRight->RequestData.dwUserID;
					SetUserRightSuccess.dwUserRight			= pSetUserRight->RequestData.dwUserRight;
					SetUserRightSuccess.dwMasterRight		= pSetUserRight->RequestData.dwMasterRight;
				

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_USERRIGHT_SUCCESS,
						&SetUserRightSuccess,
						sizeof(CMD_GP_SetUserRightSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetUserRight,
						pSetUserRight->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetUserRight->RequestData.dwUserID,
						USERTYPE_GAME,
						szAccount,
						pSetUserRight->Operator.dwOperationUserID,
						pSetUserRight->Operator.cbOperationUserType,
						pSetUserRight->Operator.szOperationAccount);

					OperationData.Content.data_SetUserRight.dwUserRight		= pSetUserRight->RequestData.dwUserID;
					OperationData.Content.data_SetUserRight.dwMasterRight	= pSetUserRight->RequestData.dwMasterRight;
					OperationData.Content.data_SetUserRight.dwOldUserRight		= dwOldUserRight;
					OperationData.Content.data_SetUserRight.dwOldMasterRight	= dwOldMasterRight;

					//写入操作记录
					SPInsertOperationInfo(OperationData);
				}
				else
				{

					//操作失败
					CMD_GP_SetUserRightError SetUserRightError;
					SetUserRightError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetUserRightError_AccoutNotExist-1;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_USERRIGHT_ERROR,
						&SetUserRightError,
						sizeof(CMD_GP_SetUserRightError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetUserRightError SetUserRightError;
		SetUserRightError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_SET_USERRIGHT_ERROR,
			&SetUserRightError,
			sizeof(CMD_GP_SetUserRightError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//自身基本信息设定请求处理
bool CDataBaseSink::OnRequestSetMySelfBasicUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_MYSELFBASICUSERINFO:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetMySelfBasicUserInfo));
				if (wDataSize!=sizeof(DBR_GP_SetMySelfBasicUserInfo)) return false;

				DBR_GP_SetMySelfBasicUserInfo * pSetBasicUserInfo=(DBR_GP_SetMySelfBasicUserInfo *)pDataBuffer;

				//执行操作
				lReturnValue=SPSetMySelfBaiscUserInfo(
					pSetBasicUserInfo->Operator.cbOperationUserType,
					pSetBasicUserInfo->RequestData.dwUserID,
					pSetBasicUserInfo->RequestData.szName,
					pSetBasicUserInfo->RequestData.szPassWord,
					pSetBasicUserInfo->RequestData.szNewPassWord,
					pSetBasicUserInfo->RequestData.cbFaceID);

				if(lReturnValue == 0)
				{
					CMD_GP_SetMySelfBasicUserInfoSuccess SetBasicUserInfoSuccess;

					SetBasicUserInfoSuccess.dwUserID		  = pSetBasicUserInfo->RequestData.dwUserID;
					SetBasicUserInfoSuccess.cbFaceID		  = pSetBasicUserInfo->RequestData.cbFaceID;
					_tcscpy(SetBasicUserInfoSuccess.szName, pSetBasicUserInfo->RequestData.szName);
					_tcscpy(SetBasicUserInfoSuccess.szNewRealityPassWord, pSetBasicUserInfo->RequestData.szNewRealityPassWord);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_MYSELFBASICUSERINFO_SUCCESS,
						&SetBasicUserInfoSuccess,
						sizeof(SetBasicUserInfoSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetBasicInfo,
						pSetBasicUserInfo->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetBasicUserInfo->RequestData.dwUserID,
						pSetBasicUserInfo->Operator.cbOperationUserType,
						pSetBasicUserInfo->Operator.szOperationAccount,
						pSetBasicUserInfo->Operator.dwOperationUserID,
						pSetBasicUserInfo->Operator.cbOperationUserType,
						pSetBasicUserInfo->Operator.szOperationAccount);

					_tcscpy(OperationData.Content.data_SetBasicInfo.szName,pSetBasicUserInfo->RequestData.szName);
				
					//写入操作记录
					SPInsertOperationInfo(OperationData);

				}
				else
				{

					//操作失败
					CMD_GP_SetMySelfBasicUserInfoError Error;
					Error.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_MYSELFBASICUSERINFO_ERROR,
						&Error,
						sizeof(Error));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetMySelfBasicUserInfoError Error;
		Error.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_USERINFO,
			SUB_GP_SET_MYSELFBASICUSERINFO_ERROR,
			&Error,
			sizeof(Error));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//注册子帐号请求处理
bool CDataBaseSink::OnRequestRegisterSubUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_REGISTER_SUBUSER:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_RegisterSubUserInfo));
				if (wDataSize!=sizeof(DBR_GP_RegisterSubUserInfo)) return false;

				DBR_GP_RegisterSubUserInfo * pRegisterSubUserInfo=(DBR_GP_RegisterSubUserInfo *)pDataBuffer;

				//执行操作
				lReturnValue=SPRegisterSubUser(pRegisterSubUserInfo->RequestData.dwParentUserID,
					pRegisterSubUserInfo->RequestData.szAccount,
					pRegisterSubUserInfo->RequestData.szName,
					pRegisterSubUserInfo->RequestData.szPassWord);

				if(lReturnValue == 0)
				{
					TCHAR szParentAccount[NAME_LEN];
					m_WorkDataBase->GetFieldValue(TEXT("MUParentAccount"),szParentAccount, NAME_LEN);

					CMD_GP_RegisterSubUserSuccess RegisterSubUserSuccess;
					helper_DBGetFieldValue_SubUserData(m_WorkDataBase, RegisterSubUserSuccess.UserData);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_REGISTER_SUBUSER_SUCCESS,
						&RegisterSubUserSuccess,
						sizeof(CMD_GP_RegisterSubUserSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_NewSubAccount, pRegisterSubUserInfo->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						RegisterSubUserSuccess.UserData.dwUserID,
						USERTYPE_SUB,
						pRegisterSubUserInfo->RequestData.szAccount,
						pRegisterSubUserInfo->Operator.dwOperationUserID,
						pRegisterSubUserInfo->Operator.cbOperationUserType,
						pRegisterSubUserInfo->Operator.szOperationAccount);
					_tcscpy(OperationData.Content.data_NewSubAccount.szName,RegisterSubUserSuccess.UserData.szName);
					_tcscpy(OperationData.Content.data_NewSubAccount.szParentAccount,szParentAccount);
					//写入操作记录
					SPInsertOperationInfo(OperationData);

				}
				else
				{

					//操作失败
					CMD_GP_RegisterSubUserError Error;
					Error.wErrorCode=lReturnValue + ErrCode_CMD_GP_RegisterSubUserError_NameEmpty;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_REGISTER_SUBUSER_ERROR,
						&Error,
						sizeof(Error));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_RegisterSubUserError Error;
		Error.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_SUBUSERINFO,
			SUB_GP_REGISTER_SUBUSER_ERROR,
			&Error,
			sizeof(Error));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//状态设定请求处理
bool CDataBaseSink::OnRequestSetSubUserCongealState(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_SUBUSERCONGEALSTATE:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetSubUserCongealState));
				if (wDataSize!=sizeof(DBR_GP_SetSubUserCongealState)) return false;
				DBR_GP_SetSubUserCongealState * pSetSubUserCongealState=(DBR_GP_SetSubUserCongealState *)pDataBuffer;

				static const DWORD dwCongealStateMark[1] = 
				{
						0x40
				};
				static const DWORD dwNormalStateMark[1] = 
				{
						0xFFFFFFBF,
				};
				DWORD dwStateMark;
				if(pSetSubUserCongealState->RequestData.cbState)
				{
					dwStateMark = dwCongealStateMark[0];
				}
				else
				{
					dwStateMark = dwNormalStateMark[0];
				}

				//执行操作

				lReturnValue=SPSetSubUserCongealState(pSetSubUserCongealState->RequestData.dwUserID,
					pSetSubUserCongealState->RequestData.cbState,
					dwStateMark);

				if(lReturnValue == 0)
				{
					DWORD dwParentUserID = 0;
					TCHAR szAccount[NAME_LEN];
					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);
					m_WorkDataBase->GetFieldValue(TEXT("dwParentUserID"),dwParentUserID);

					CMD_GP_SetSubUserCongealStateSuccess SetSubUserCongealStateSuccess;

					SetSubUserCongealStateSuccess.dwUserID		  = pSetSubUserCongealState->RequestData.dwUserID;
					SetSubUserCongealStateSuccess.cbState		  = pSetSubUserCongealState->RequestData.cbState;
					SetSubUserCongealStateSuccess.dwStateMark	  = dwStateMark;
					SetSubUserCongealStateSuccess.dwParentUserID  = dwParentUserID;

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_SET_SUBUSERCONGEALSTATE_SUCCESS,
						&SetSubUserCongealStateSuccess,
						sizeof(CMD_GP_SetSubUserCongealStateSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetCongealState, pSetSubUserCongealState->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetSubUserCongealState->RequestData.dwUserID,
						USERTYPE_SUB,
						szAccount,
						pSetSubUserCongealState->Operator.dwOperationUserID,
						pSetSubUserCongealState->Operator.cbOperationUserType,
						pSetSubUserCongealState->Operator.szOperationAccount);
					OperationData.Content.data_SetCongealState.cbState = pSetSubUserCongealState->RequestData.cbState;
					OperationData.Content.data_SetCongealState.dwStateMark = dwStateMark;
					//写入操作记录
					SPInsertOperationInfo(OperationData);
				}
				else
				{

					//操作失败
					CMD_GP_SetSubUserCongealStateError SetSubUserCongealStateError;
					SetSubUserCongealStateError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist-1;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR,
						&SetSubUserCongealStateError,
						sizeof(CMD_GP_SetSubUserCongealStateError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetSubUserCongealStateError SetSubUserCongealStateError;
		SetSubUserCongealStateError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_SUBUSERINFO,
			SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR,
			&SetSubUserCongealStateError,
			sizeof(CMD_GP_SetSubUserCongealStateError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//子帐号基本信息设定请求处理
bool CDataBaseSink::OnRequestSetBasicSubUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_SET_BASICSUBUSERINFO:
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_SetBasicSubUserInfo));
				if (wDataSize!=sizeof(DBR_GP_SetBasicSubUserInfo)) return false;

				DBR_GP_SetBasicSubUserInfo * pSetBasicSubUserInfo=(DBR_GP_SetBasicSubUserInfo *)pDataBuffer;

				//执行操作
				lReturnValue=SPSetBaiscSubUserInfo(pSetBasicSubUserInfo->RequestData.dwUserID,
					pSetBasicSubUserInfo->RequestData.szName,
					pSetBasicSubUserInfo->RequestData.szPassWord);

				if(lReturnValue == 0)
				{
					DWORD dwParentUserID = 0;
					TCHAR szAccount[NAME_LEN];
					m_WorkDataBase->GetFieldValue(TEXT("szAccount"),szAccount, NAME_LEN);
					m_WorkDataBase->GetFieldValue(TEXT("MUParentUserID"),dwParentUserID);

					CMD_GP_SetBasicSubUserInfoSuccess SetBasicSubUserInfoSuccess;

					SetBasicSubUserInfoSuccess.dwUserID				  = pSetBasicSubUserInfo->RequestData.dwUserID;
					SetBasicSubUserInfoSuccess.dwParentUserID		  = dwParentUserID;
					
					_tcscpy(SetBasicSubUserInfoSuccess.szName, pSetBasicSubUserInfo->RequestData.szName);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_SET_BASICSUBUSERINFO_SUCCESS,
						&SetBasicSubUserInfoSuccess,
						sizeof(CMD_GP_SetBasicSubUserInfoSuccess));

					//操作记录
					tagOperationData OperationData;
					//初试化操作数据
					helper_InitOperationData(OperationData, OPT_SetBasicInfo, pSetBasicSubUserInfo->Operator.dwIPAddress);
					//填充操作数据包头
					helper_FillOperationHeader(OperationData,
						pSetBasicSubUserInfo->RequestData.dwUserID,
						USERTYPE_SUB,
						szAccount,
						pSetBasicSubUserInfo->Operator.dwOperationUserID,
						pSetBasicSubUserInfo->Operator.cbOperationUserType,
						pSetBasicSubUserInfo->Operator.szOperationAccount);
					_tcscpy(OperationData.Content.data_SetBasicInfo.szName, pSetBasicSubUserInfo->RequestData.szName);
					//写入操作记录
					SPInsertOperationInfo(OperationData);

				}
				else
				{

					//操作失败
					CMD_GP_SetBasicSubUserInfoError SetBasicSubUserInfoError;
					SetBasicSubUserInfoError.wErrorCode=lReturnValue + ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty;
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_SET_BASICSUBUSERINFO_ERROR,
						&SetBasicSubUserInfoError,
						sizeof(CMD_GP_SetBasicSubUserInfoError));

				}

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_SetBasicSubUserInfoError SetBasicSubUserInfoError;
		SetBasicSubUserInfoError.wErrorCode=ErrCode_Database;
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_EDIT_SUBUSERINFO,
			SUB_GP_SET_BASICSUBUSERINFO_ERROR,
			&SetBasicSubUserInfoError,
			sizeof(CMD_GP_SetBasicSubUserInfoError));
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取下线请求处理
bool CDataBaseSink::OnRequestGetLessUserList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取子帐号处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_LOWUSERLIST:		//获取下线
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetLessUserList));
				if (wDataSize!=sizeof(DBR_GP_GetLessUserList)) return false;

				//执行查询
				DBR_GP_GetLessUserList * pGetLessUserList=(DBR_GP_GetLessUserList *)pDataBuffer;
				lReturnValue=SPGetLessUserList(pGetLessUserList->dwParentUserID);

				//结果判断
				if (lReturnValue>=0L)
				{
					tagUserDataArray UserDataArray;
					helper_DBGetFieldValue_UserDataArray(m_WorkDataBase, UserDataArray);
					GT_ASSERT(lReturnValue == UserDataArray.GetCount());

					//构造数据
					WORD wSendSize=0;
					tagUserData * pUserData=NULL;
					DWORD dwUserCount=(DWORD)UserDataArray.GetCount();

					CMD_GP_LessUserList* pLessUserList = (CMD_GP_LessUserList*)cbBuffer;
					pLessUserList->dwParentUserID = pGetLessUserList->dwParentUserID;
					pLessUserList->wUserCount = 0;
					wSendSize+=sizeof(CMD_GP_LessUserList);

					for (DWORD i=0;i<UserDataArray.GetCount();i++)
					{
						if ((wSendSize+sizeof(tagUserData))>sizeof(cbBuffer))
						{
							m_pITCPSocketEngine->SendData(wIndex,
								wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_LOWUSER,cbBuffer,wSendSize);
							wSendSize=0;

							pLessUserList = (CMD_GP_LessUserList*)cbBuffer;
							pLessUserList->dwParentUserID = pGetLessUserList->dwParentUserID;
							pLessUserList->wUserCount = 0;
							wSendSize+=sizeof(CMD_GP_LessUserList);
						}
						pUserData=&UserDataArray.GetAt(i);

						ConvertACPToUtf8(pUserData->szAccount, NAME_LEN,
							pUserData->szAccount, NAME_LEN);
						ConvertACPToUtf8(pUserData->szName, NAME_LEN,
							pUserData->szName, NAME_LEN);

						CopyMemory(cbBuffer+wSendSize,
							pUserData,
							sizeof(tagUserData));
						pLessUserList->wUserCount ++;

						wSendSize+=sizeof(tagUserData);
					}
					if (wSendSize>0) 
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_LOWUSER,cbBuffer,wSendSize);

					
					//发送完毕再次发送子帐号
					DBR_GP_GetSubUserList GetSubUserList;
					GetSubUserList.dwParentUserID=pGetLessUserList->dwParentUserID;

					m_AttemperEvent.PostDataBaseEvent(DBR_GP_GET_LOWUSERLISTCOMPLETE,
						DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
						&GetSubUserList,sizeof(GetSubUserList));
				
				}
				else
				{
					//操作失败
					CMD_GP_GetLessUserListError *pGetLessUserListError = (CMD_GP_GetLessUserListError*)cbBuffer;
					pGetLessUserListError->wErrorCode=ErrCode_Database;

					//发送结果
					WORD wSendSize=sizeof(CMD_GP_GetLessUserListError);
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_LOWUSERERROR,cbBuffer,wSendSize);
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

		
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_GetLessUserListError *pGetLessUserListError = (CMD_GP_GetLessUserListError*)cbBuffer;
		pGetLessUserListError->wErrorCode=ErrCode_Database;

		//发送结果
		WORD wSendSize=sizeof(CMD_GP_GetLessUserListError);
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_LOWUSERERROR,cbBuffer,wSendSize);

	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取子帐号处理
bool CDataBaseSink::OnRequestGetSubUserList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取子帐号处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_SUBUSERLIST:		//获取子帐号
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetSubUserList));
				if (wDataSize!=sizeof(DBR_GP_GetSubUserList)) return false;

				//执行查询
				DBR_GP_GetSubUserList * pGetSubUserList=(DBR_GP_GetSubUserList *)pDataBuffer;
				lReturnValue=SPGetSubUserList(pGetSubUserList->dwParentUserID);

				//结果判断
				if (lReturnValue>=0L)
				{
					tagSubUserDataArray SubUserDataArray;
					helper_DBGetFieldValue_SubUserDataArray(m_WorkDataBase, SubUserDataArray);
					GT_ASSERT(lReturnValue == SubUserDataArray.GetCount());

					//构造数据
					WORD wSendSize=0;
					tagSubUserData * pUserData=NULL;
					DWORD dwUserCount=(DWORD)SubUserDataArray.GetCount();

					CMD_GP_SubUserList* pSubUserList = (CMD_GP_SubUserList*)cbBuffer;
					pSubUserList->dwParentUserID = pGetSubUserList->dwParentUserID;
					pSubUserList->wSubUserCount = 0;
					wSendSize+=sizeof(CMD_GP_SubUserList);

					for (DWORD i=0;i<SubUserDataArray.GetCount();i++)
					{
						if ((wSendSize+sizeof(tagSubUserData))>sizeof(cbBuffer))
						{
							m_pITCPSocketEngine->SendData(wIndex,
								wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_SUBUSER,cbBuffer,wSendSize);
							wSendSize=0;

							pSubUserList = (CMD_GP_SubUserList*)cbBuffer;
							pSubUserList->dwParentUserID = pGetSubUserList->dwParentUserID;
							pSubUserList->wSubUserCount = 0;
							wSendSize+=sizeof(CMD_GP_SubUserList);
						}
						pUserData=&SubUserDataArray.GetAt(i);

						ConvertACPToUtf8(pUserData->szAccount, NAME_LEN,
							pUserData->szAccount, NAME_LEN);
						ConvertACPToUtf8(pUserData->szName, NAME_LEN,
							pUserData->szName, NAME_LEN);

						CopyMemory(cbBuffer+wSendSize,
							pUserData,
							sizeof(tagSubUserData));
						pSubUserList->wSubUserCount ++;

						wSendSize+=sizeof(tagSubUserData);
					}
					if (wSendSize>0) 
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_USER,
						SUB_GP_LIST_SUBUSER,cbBuffer,wSendSize);


				}
				else
				{
					//操作失败
					CMD_GP_GetLessUserListError *pGetLessUserListError = (CMD_GP_GetLessUserListError*)cbBuffer;
					pGetLessUserListError->wErrorCode=ErrCode_Database;

					//发送结果
					WORD wSendSize=sizeof(CMD_GP_GetLessUserListError);
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_LOWUSERERROR,cbBuffer,wSendSize);
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_GetLessUserListError *pGetLessUserListError = (CMD_GP_GetLessUserListError*)cbBuffer;
		pGetLessUserListError->wErrorCode=ErrCode_Database;

		//发送结果
		WORD wSendSize=sizeof(CMD_GP_GetLessUserListError);
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,MDM_GP_GET_USER,SUB_GP_LIST_LOWUSERERROR,cbBuffer,wSendSize);

	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取操作记录
bool CDataBaseSink::OnRequestGetOperationList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取操作记录处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_OPERATION:		//获取操作记录
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetOperation));
				if (wDataSize!=sizeof(DBR_GP_GetOperation)) return false;

				//执行查询
				DBR_GP_GetOperation * pGetOperation=(DBR_GP_GetOperation *)pDataBuffer;
				lReturnValue=SPGetOperationList(pGetOperation);

				//结果判断
				if (lReturnValue>=0L)
				{
					CMD_GP_OperationListConfig *pConfig = (CMD_GP_OperationListConfig*)cbBuffer;
					pConfig->wPageIndex = pGetOperation->RequestData.wPageIndex;
					pConfig->wPageSize  = pGetOperation->RequestData.wPageSize;	
					if(pGetOperation->RequestData.wPageIndex == 0xffff)
					{
						pConfig->wPageIndex = 0;
						m_WorkDataBase->GetFieldValue(TEXT("dwTotalCount"),pConfig->dwTotalResultCount);
						m_WorkDataBase->NextRecordset();
					}
					else
					{
						pConfig->dwTotalResultCount = 0xffffffff;
					}

					tagOperationDataArray OperationDataArray;
					helper_DBGetFieldValue_OperationDataArray(m_WorkDataBase, OperationDataArray);
					pConfig->wResultCount = OperationDataArray.GetCount();

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_OPERATIONCONFIG,cbBuffer,sizeof(CMD_GP_OperationListConfig));

					//构造数据
					DWORD dwOperationCount=(DWORD)OperationDataArray.GetCount();
					WORD wSendSize=0;

					tagOperationData* pOperationData = NULL;
					for (DWORD i=0;i<OperationDataArray.GetCount();i++)
					{
						if ((wSendSize+sizeof(tagOperationData))>sizeof(cbBuffer))
						{
							m_pITCPSocketEngine->SendData(wIndex,
								wRoundID,MDM_GP_GET_RECORD,SUB_GP_LIST_OPERATION,cbBuffer,wSendSize);
							wSendSize=0;

						}
						pOperationData=&OperationDataArray.GetAt(i);

						helper_ConvertACPToUtf8_OperationData(pOperationData);

						CopyMemory(cbBuffer+wSendSize,
							pOperationData,
							sizeof(tagOperationData));

						wSendSize+=sizeof(tagOperationData);
					}
					if (wSendSize>0) 
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_OPERATION,cbBuffer,wSendSize);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_OPERATIONFINISH,cbBuffer,0);

				}
				else
				{
					//操作失败
					CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
					pError->wErrorCode=ErrCode_Database;

					//发送结果
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_OPERATIONERROR,
						cbBuffer,
						sizeof(CMD_GP_Error));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
		pError->wErrorCode=ErrCode_Database;

		//发送结果
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_GET_RECORD,
			SUB_GP_LIST_OPERATIONERROR,
			cbBuffer,
			sizeof(CMD_GP_Error));

	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取报表记录方式-1
bool CDataBaseSink::OnRequestGetChartViewOneList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取报表记录方式-1处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_CHARTVIEWONE:		//获取报表记录方式-1
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetChartViewOne));
				if (wDataSize!=sizeof(DBR_GP_GetChartViewOne)) return false;

				//执行查询
				DBR_GP_GetChartViewOne * pGetChartViewOne=(DBR_GP_GetChartViewOne *)pDataBuffer;
				lReturnValue=SPGetChartViewOneList(pGetChartViewOne);

				//结果判断
				if (lReturnValue>=0L)
				{
					CMD_GP_ChartViewOneListConfig *pConfig = (CMD_GP_ChartViewOneListConfig*)cbBuffer;
					if(helper_DBGetFieldValue_ChartViewOneListConfig(m_WorkDataBase, pConfig) == false)
					{
						//操作失败
						CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
						pError->wErrorCode=ErrCode_AccountNotExist;

						//发送结果
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
							MDM_GP_GET_RECORD,
							SUB_GP_LIST_CHARTVIEWONEERROR,
							cbBuffer,
							sizeof(CMD_GP_Error));

						//关闭记录集
						m_WorkDataBase->CloseRecordset();

						return true;
					}
					m_WorkDataBase->NextRecordset();
					
					tagChartViewOneDataArray ChartViewOneDataArray;
					helper_DBGetFieldValue_ChartViewOneDataArray(m_WorkDataBase, ChartViewOneDataArray);
					pConfig->dwTotalResultCount = ChartViewOneDataArray.GetCount();

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWONECONFIG,cbBuffer,sizeof(CMD_GP_ChartViewOneListConfig));

					//构造数据
					DWORD dwChartViewOneCount=(DWORD)ChartViewOneDataArray.GetCount();
					WORD wSendSize=0;

					tagChartViewOneData* pChartViewOneData = NULL;
					for (DWORD i=0;i<ChartViewOneDataArray.GetCount();i++)
					{
						if ((wSendSize+sizeof(tagChartViewOneData))>sizeof(cbBuffer))
						{
							m_pITCPSocketEngine->SendData(wIndex,
								wRoundID,MDM_GP_GET_RECORD,SUB_GP_LIST_CHARTVIEWONE,cbBuffer,wSendSize);
							wSendSize=0;

						}
						pChartViewOneData=&ChartViewOneDataArray.GetAt(i);

						ConvertACPToUtf8(pChartViewOneData->szAccount, NAME_LEN,
							pChartViewOneData->szAccount, NAME_LEN);

						CopyMemory(cbBuffer+wSendSize,
							pChartViewOneData,
							sizeof(tagChartViewOneData));

						wSendSize+=sizeof(tagChartViewOneData);
					}
					if (wSendSize>0) 
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWONE,cbBuffer,wSendSize);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWONEFINISH,cbBuffer,0);

				}
				else
				{
					//操作失败
					CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
					pError->wErrorCode=ErrCode_Database;

					//发送结果
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWONEERROR,
						cbBuffer,
						sizeof(CMD_GP_Error));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
		pError->wErrorCode=ErrCode_Database;

		//发送结果
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_GET_RECORD,
			SUB_GP_LIST_CHARTVIEWONEERROR,
			cbBuffer,
			sizeof(CMD_GP_Error));

	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取报表记录方式-2
bool CDataBaseSink::OnRequestGetChartViewTwoList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取报表记录方式-2处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_CHARTVIEWTWO:		//获取报表记录方式-2
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetChartViewTwo));
				if (wDataSize!=sizeof(DBR_GP_GetChartViewTwo)) return false;

				//执行查询
				DBR_GP_GetChartViewTwo * pGetChartViewTwo=(DBR_GP_GetChartViewTwo *)pDataBuffer;
				lReturnValue=SPGetChartViewTwoList(pGetChartViewTwo);

				//结果判断
				if (lReturnValue>=0L)
				{
					CMD_GP_ChartViewTwoListConfig *pConfig = (CMD_GP_ChartViewTwoListConfig*)cbBuffer;
					pConfig->wPageIndex = pGetChartViewTwo->RequestData.wPageIndex;
					pConfig->wPageSize  = pGetChartViewTwo->RequestData.wPageSize;	
					if(pGetChartViewTwo->RequestData.wPageIndex == 0xffff)
					{
						pConfig->wPageIndex = 0;
						m_WorkDataBase->GetFieldValue(TEXT("dwTotalCount"),pConfig->dwTotalResultCount);
						m_WorkDataBase->NextRecordset();
					}
					else
					{
						pConfig->dwTotalResultCount = 0xffffffff;
					}

					tagChartViewTwoDataArray ChartViewTwoDataArray;
					helper_DBGetFieldValue_ChartViewTwoDataArray(m_WorkDataBase, ChartViewTwoDataArray);
					pConfig->wResultCount = ChartViewTwoDataArray.GetCount();

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWTWOCONFIG,cbBuffer,sizeof(CMD_GP_ChartViewTwoListConfig));

					//构造数据
					DWORD dwChartViewTwoCount=(DWORD)ChartViewTwoDataArray.GetCount();
					WORD wSendSize=0;

					tagChartViewTwoData* pChartViewTwoData = NULL;
					for (DWORD i=0;i<ChartViewTwoDataArray.GetCount();i++)
					{
						if ((wSendSize+sizeof(tagChartViewTwoData))>sizeof(cbBuffer))
						{
							m_pITCPSocketEngine->SendData(wIndex,
								wRoundID,MDM_GP_GET_RECORD,SUB_GP_LIST_CHARTVIEWTWO,cbBuffer,wSendSize);
							wSendSize=0;

						}
						pChartViewTwoData=&ChartViewTwoDataArray.GetAt(i);

						ConvertACPToUtf8(pChartViewTwoData->szAccount, NAME_LEN,
							pChartViewTwoData->szAccount, NAME_LEN);

						CopyMemory(cbBuffer+wSendSize,
							pChartViewTwoData,
							sizeof(tagChartViewTwoData));

						wSendSize+=sizeof(tagChartViewTwoData);
					}
					if (wSendSize>0) 
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWTWO,cbBuffer,wSendSize);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWTWOFINISH,cbBuffer,0);

				}
				else
				{
					//操作失败
					CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
					pError->wErrorCode=ErrCode_Database;

					//发送结果
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_CHARTVIEWTWOERROR,
						cbBuffer,
						sizeof(CMD_GP_Error));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
		pError->wErrorCode=ErrCode_Database;

		//发送结果
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_GET_RECORD,
			SUB_GP_LIST_CHARTVIEWTWOERROR,
			cbBuffer,
			sizeof(CMD_GP_Error));

	}
	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取游戏局记录
bool CDataBaseSink::OnRequestGetBetHistoryList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取游戏局记录处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_BETHISTORY:		//获取游戏局记录
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetBetHistory));
				if (wDataSize!=sizeof(DBR_GP_GetBetHistory)) return false;

				//执行查询
				DBR_GP_GetBetHistory * pGetBetHistory=(DBR_GP_GetBetHistory *)pDataBuffer;
				lReturnValue=SPGetBetHistoryList(pGetBetHistory);

				//结果判断
				if (lReturnValue>=0L)
				{
					CMD_GP_BetHistoryListConfig *pConfig = (CMD_GP_BetHistoryListConfig*)cbBuffer;
					pConfig->wPageIndex = pGetBetHistory->RequestData.wPageIndex;
					pConfig->wPageSize  = pGetBetHistory->RequestData.wPageSize;	
					if(pGetBetHistory->RequestData.wPageIndex == 0xffff)
					{
						pConfig->wPageIndex = 0;
						m_WorkDataBase->GetFieldValue(TEXT("dwTotalCount"),pConfig->dwTotalResultCount);
						m_WorkDataBase->NextRecordset();
					}
					else
					{
						pConfig->dwTotalResultCount = 0xffffffff;
					}

					tagBetHistoryDataArray BetHistoryDataArray;
					helper_DBGetFieldValue_BetHistoryDataArray(m_WorkDataBase, BetHistoryDataArray);
					pConfig->wResultCount = BetHistoryDataArray.GetCount();

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_BETHISTORYCONFIG,cbBuffer,sizeof(CMD_GP_BetHistoryListConfig));

					//构造数据
					DWORD dwBetHistoryCount=(DWORD)BetHistoryDataArray.GetCount();
					WORD wSendSize=0;

					tagBetHistoryData* pBetHistoryData = NULL;
					for (DWORD i=0;i<BetHistoryDataArray.GetCount();i++)
					{
						pBetHistoryData=&BetHistoryDataArray.GetAt(i);
						DWORD dwBetHistoryDataHeaderSize = sizeof(tagBetHistoryData) - sizeof(pBetHistoryData->cbEndData);
						DWORD dwDataSize = dwBetHistoryDataHeaderSize + pBetHistoryData->wEndDataSize;

						if ((wSendSize+dwDataSize)>sizeof(cbBuffer))
						{
							m_pITCPSocketEngine->SendData(wIndex,
								wRoundID,MDM_GP_GET_RECORD,SUB_GP_LIST_BETHISTORY,cbBuffer,wSendSize);
							wSendSize=0;

						}
						
						helper_ConvertACPToUtf8_BetHistoryData(pBetHistoryData);

						CopyMemory(cbBuffer+wSendSize,
							pBetHistoryData,
							 dwDataSize);

						wSendSize+=dwDataSize;
					}
					if (wSendSize>0) 
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_BETHISTORY,cbBuffer,wSendSize);

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_BETHISTORYFINISH,cbBuffer,0);

				}
				else
				{
					//操作失败
					CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
					pError->wErrorCode=ErrCode_Database;

					//发送结果
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_LIST_BETHISTORYERROR,
						cbBuffer,
						sizeof(CMD_GP_Error));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
		pError->wErrorCode=ErrCode_Database;

		//发送结果
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_GET_RECORD,
			SUB_GP_LIST_BETHISTORYERROR,
			cbBuffer,
			sizeof(CMD_GP_Error));

	}
	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取游戏局记录详细投注
bool CDataBaseSink::OnRequestGetDetailBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取游戏局记录详细投注处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_DETAILBETSCORE:		//获取游戏局记录详细投注
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetDetailBetScore));
				if (wDataSize!=sizeof(DBR_GP_GetDetailBetScore)) return false;

				//执行查询
				DBR_GP_GetDetailBetScore * pGetDetailBetScore=(DBR_GP_GetDetailBetScore *)pDataBuffer;
				lReturnValue=SPGetDetailBetScore(pGetDetailBetScore);

				//结果判断
				if (lReturnValue>=0L)
				{
					CMD_GP_GetDetailBetScoreSuccess *pGetDetailBetScoreSuccess = (CMD_GP_GetDetailBetScoreSuccess*)cbBuffer;

					if(helper_DBGetFieldValue_DetailBetScoreSuccess(m_WorkDataBase, pGetDetailBetScoreSuccess))
					{
						WORD wSendSize = sizeof(CMD_GP_GetDetailBetScoreSuccess) - sizeof(pGetDetailBetScoreSuccess->cbDetailBetScore)
						+ pGetDetailBetScoreSuccess->wDetailBetScoreSize;
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_GET_DETAILBETSCORESUCCESS,cbBuffer,wSendSize);
					}
					else
					{
						//操作失败
						CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
						pError->wErrorCode=ErrCode_Nothing;

						//发送结果
						m_pITCPSocketEngine->SendData(wIndex,wRoundID,
							MDM_GP_GET_RECORD,
							SUB_GP_GET_DETAILBETSCOREERROR,
							cbBuffer,
							sizeof(CMD_GP_Error));
					}

				}
				else
				{
					//操作失败
					CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
					pError->wErrorCode=ErrCode_Database;

					//发送结果
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_GET_DETAILBETSCOREERROR,
						cbBuffer,
						sizeof(CMD_GP_Error));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
		pError->wErrorCode=ErrCode_Database;

		//发送结果
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_GET_RECORD,
			SUB_GP_GET_DETAILBETSCOREERROR,
			cbBuffer,
			sizeof(CMD_GP_Error));

	}
	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//获取报表记录方式-3
bool CDataBaseSink::OnRequestGetChartViewThree(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;
	BYTE cbBuffer[SOCKET_PACKAGE];

	try
	{
		//获取报表记录方式-3处理
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_GET_CHARTVIEWTHREE:		//获取报表记录方式-3
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_GetChartViewThree));
				if (wDataSize!=sizeof(DBR_GP_GetChartViewThree)) return false;

				//执行查询
				DBR_GP_GetChartViewThree * pGetChartViewThree=(DBR_GP_GetChartViewThree *)pDataBuffer;
				lReturnValue=SPGetChartViewThree(pGetChartViewThree);

				//结果判断
				if (lReturnValue>=0L)
				{

					WORD wSendSize = sizeof(CMD_GP_ChartViewThreeSucess);	
					CMD_GP_ChartViewThreeSucess* pChartViewThreeSucess = (CMD_GP_ChartViewThreeSucess*)cbBuffer;
					if(helper_DBGetFieldValue_ChartViewThreeSucess(m_WorkDataBase, pChartViewThreeSucess) == false)
					{
						memset(pChartViewThreeSucess, 0, wSendSize);
					}

					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_CHARTVIEWTHREESUCCESS,cbBuffer,wSendSize);

				}
				else
				{
					//操作失败
					CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
					pError->wErrorCode=ErrCode_Database;

					//发送结果
					m_pITCPSocketEngine->SendData(wIndex,wRoundID,
						MDM_GP_GET_RECORD,
						SUB_GP_CHARTVIEWTHREEERROR,
						cbBuffer,
						sizeof(CMD_GP_Error));
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		CMD_GP_Error *pError = (CMD_GP_Error*)cbBuffer;
		pError->wErrorCode=ErrCode_Database;

		//发送结果
		m_pITCPSocketEngine->SendData(wIndex,wRoundID,
			MDM_GP_GET_RECORD,
			SUB_GP_CHARTVIEWTHREEERROR,
			cbBuffer,
			sizeof(CMD_GP_Error));

	}
	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//更新游戏信息
bool CDataBaseSink::OnRequestUpdateGameMainInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		//更新游戏信息
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_UPDATE_GAMEMAININFO:		//更新游戏信息
			{
				lReturnValue=SPLoadGameMainInfo();

				//结果判断
				if (lReturnValue>=0L)
				{
					DBR_Update_GameMainInfo GameMainInfo;

					if(helper_DBGetFieldValue_GameMainInfo(m_WorkDataBase, GameMainInfo))
					{
						m_AttemperEvent.PostDataBaseEvent(DBR_UPDATE_GAMEMAININFO_SUCCESS,
						DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
						&GameMainInfo,sizeof(GameMainInfo));
					}

				}
				else
				{
					//do nothing
				}

				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//注销帐号登陆
bool CDataBaseSink::OnRequestLogout(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	WORD wIndex=DataBaseEvent.wIndex;
	WORD wRoundID=DataBaseEvent.wRoundID;

	try
	{
		//注销帐号登陆
		LONG lReturnValue=-1;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GP_LOGOUT_USER:		//注销帐号登陆
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GP_LogoutUser));
				if (wDataSize!=sizeof(DBR_GP_LogoutUser)) return false;

				//执行操作
				DBR_GP_LogoutUser * pLogoutUser=(DBR_GP_LogoutUser *)pDataBuffer;
				lReturnValue=SPLogoutUser(m_dwLogonServerAddr,
					m_pInitParamter->m_wListenPort,pLogoutUser);
				break;
			}

		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}


	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		UNREFERENCED_PARAMETER(pIADOError);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	//关闭记录集
	m_WorkDataBase->CloseRecordset();

	return true;
}
//////////////////////////////////////////////////////////////////////////
//帐号存储过程
LONG CDataBaseSink::SPLogonByAccount(LPCTSTR pszAccounts, 
									 LPCTSTR pszPassword, 
									 DWORD dwClientIP,
									 DWORD dwLogonServerAddr, 
									 WORD wListenPort)
{
	//效验参数
	GT_ASSERT(pszAccounts!=NULL);
	GT_ASSERT(pszPassword!=NULL);


	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_LogonByAccount");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@strAccounts"),adParamInput,adVarChar,CountString(pszAccounts),_variant_t(pszAccounts));
	m_WorkDataBase->AddParamter(TEXT("@strPassword"),adParamInput,adChar,CountString(pszPassword),_variant_t(pszPassword));
	m_WorkDataBase->AddParamter(TEXT("@nClientIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwClientIP));
	m_WorkDataBase->AddParamter(TEXT("@dwLogonServerIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwLogonServerAddr));
	m_WorkDataBase->AddParamter(TEXT("@wLogonServerListenPort"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wListenPort));
	m_WorkDataBase->AddParamter(TEXT("@cbLogonPlaza"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)1));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}

//更新存储过程
LONG CDataBaseSink::SPRegisterUser(tagUserData* pUserData, LPCTSTR pszPassword, DWORD dwClientIP)
{
	//效验参数
	GT_ASSERT(pUserData!=NULL);
	GT_ASSERT(pszPassword!=NULL);

	DOUBLE tmRegisterTime = helper_GetNowTime();

	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_RegisterUser");

	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwParentUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pUserData->dwParentUserID));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(pUserData->szAccount),_variant_t(pUserData->szAccount));
	m_WorkDataBase->AddParamter(TEXT("@szName"),adParamInput,adVarChar,CountString(pUserData->szName),_variant_t(pUserData->szName));
	m_WorkDataBase->AddParamter(TEXT("@cbFaceID"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pUserData->cbFaceID));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fOccupancyRate"),adParamInput,pUserData->fOccupancyRate);
	m_WorkDataBase->AddParamter(TEXT("@cbOccupancyRate_NoFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pUserData->cbOccupancyRate_NoFlag));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fLowOccupancyRate_Max"),adParamInput,pUserData->fLowOccupancyRate_Max);
	m_WorkDataBase->AddParamter(TEXT("@cbLowOccupancyRate_Max_NoFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pUserData->cbLowOccupancyRate_Max_NoFlag));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fLowOccupancyRate_Min"),adParamInput,pUserData->fLowOccupancyRate_Min);
	m_WorkDataBase->AddParamter(TEXT("@cbLowOccupancyRate_Min_NoFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pUserData->cbLowOccupancyRate_Min_NoFlag));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fTaxOccupancyRate"),adParamInput,pUserData->fTaxOccupancyRate);
	m_WorkDataBase->AddDecimalParamter(TEXT("@fRollbackRate"),adParamInput,pUserData->fRollbackRate);
	m_WorkDataBase->AddParamter(TEXT("@dwBetLimit"),adParamInput,adInteger,sizeof(long),_variant_t((long)pUserData->dwBetLimit));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fScore"),adParamInput,pUserData->UserScore.fScore);
	m_WorkDataBase->AddParamter(TEXT("@szPassword"),adParamInput,adVarChar,CountString(pszPassword),_variant_t(pszPassword));
	m_WorkDataBase->AddParamter(TEXT("@fRegisterTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)tmRegisterTime));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}

//获取下线存储过程
LONG CDataBaseSink::SPGetLessUserList(DWORD	dwParentUserID)
{

	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetLessUserList");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@nParentUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwParentUserID));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//获取子帐号存储过程
LONG CDataBaseSink::SPGetSubUserList(DWORD	dwParentUserID)
{

	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetSubUserList");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@nParentUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwParentUserID));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}

//存取额度存储过程
LONG CDataBaseSink::SPSetUserScore(DWORD dwParentUserID,
									  DWORD dwUserID,
									  DOUBLE fScore)
{

	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetUserScore");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwParentUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwParentUserID));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fScore"),adParamInput,fScore);
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//状态设定存储过程
LONG CDataBaseSink::SPSetUserCongealState(DWORD dwUserID,
							  BYTE cbLevel, BYTE cbState, DWORD dwStateMark)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetUserCongealState");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@cbLevel"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbLevel));
	m_WorkDataBase->AddParamter(TEXT("@cbState"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbState));
	m_WorkDataBase->AddParamter(TEXT("@dwStateMark"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwStateMark));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//基本信息设定存储过程
LONG CDataBaseSink::SPSetBaiscUserInfo(DWORD dwUserID,
										  LPCTSTR pszName,LPCTSTR pszPassword,BYTE cbFaceID)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetBaiscUserInfo");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@szName"),adParamInput,adVarChar,CountString(pszName),_variant_t(pszName));
	m_WorkDataBase->AddParamter(TEXT("@szPassword"),adParamInput,adVarChar,CountString(pszPassword),_variant_t(pszPassword));
	m_WorkDataBase->AddParamter(TEXT("@cbFaceID"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbFaceID));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//详细信息设定存储过程
LONG CDataBaseSink::SPSetDetailUserInfo(DBR_GP_SetDetailUserInfo * pSetDetailUserInfo)
{
	//效验参数
	GT_ASSERT(pSetDetailUserInfo!=NULL);


	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetDetailUserInfo");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pSetDetailUserInfo->RequestData.dwUserID));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fOccupancyRate"),adParamInput,pSetDetailUserInfo->RequestData.fOccupancyRate);
	m_WorkDataBase->AddParamter(TEXT("@cbOccupancyRate_NoFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pSetDetailUserInfo->RequestData.cbOccupancyRate_NoFlag));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fLowOccupancyRate_Max"),adParamInput,pSetDetailUserInfo->RequestData.fLowOccupancyRate_Max);
	m_WorkDataBase->AddParamter(TEXT("@cbLowOccupancyRate_Max_NoFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Max_NoFlag));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fLowOccupancyRate_Min"),adParamInput,pSetDetailUserInfo->RequestData.fLowOccupancyRate_Min);
	m_WorkDataBase->AddParamter(TEXT("@cbLowOccupancyRate_Min_NoFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pSetDetailUserInfo->RequestData.cbLowOccupancyRate_Min_NoFlag));
	m_WorkDataBase->AddDecimalParamter(TEXT("@fTaxOccupancyRate"),adParamInput,pSetDetailUserInfo->RequestData.fTaxOccupancyRate);
	m_WorkDataBase->AddDecimalParamter(TEXT("@fRollbackRate"),adParamInput,pSetDetailUserInfo->RequestData.fRollbackRate);
	m_WorkDataBase->AddParamter(TEXT("@dwBetLimit"),adParamInput,adInteger,sizeof(long),_variant_t((long)pSetDetailUserInfo->RequestData.dwBetLimit));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//权限设定存储过程
LONG CDataBaseSink::SPSetUserRight(DBR_GP_SetUserRight * pSetUserRight)
{
	//效验参数
	GT_ASSERT(pSetUserRight!=NULL);


	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetUserRight");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pSetUserRight->RequestData.dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@dwUserRight"),adParamInput,adInteger,sizeof(long),_variant_t((long)pSetUserRight->RequestData.dwUserRight));
	m_WorkDataBase->AddParamter(TEXT("@dwMasterRight"),adParamInput,adInteger,sizeof(long),_variant_t((long)pSetUserRight->RequestData.dwMasterRight));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//基本信息设定存储过程
LONG CDataBaseSink::SPSetMySelfBaiscUserInfo(BYTE  cbUserType,
											 DWORD dwUserID,
											 LPCTSTR pszName,
											 LPCTSTR pszPassword,
											 LPCTSTR pszNewPassword,
											 BYTE cbFaceID)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetMySelfBaiscUserInfo");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@cbUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbUserType));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@szName"),adParamInput,adVarChar,CountString(pszName),_variant_t(pszName));
	m_WorkDataBase->AddParamter(TEXT("@szPassword"),adParamInput,adVarChar,CountString(pszPassword),_variant_t(pszPassword));
	m_WorkDataBase->AddParamter(TEXT("@szNewPassword"),adParamInput,adVarChar,CountString(pszNewPassword),_variant_t(pszNewPassword));
	m_WorkDataBase->AddParamter(TEXT("@cbFaceID"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbFaceID));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//子帐号状态设定存储过程
LONG CDataBaseSink::SPSetSubUserCongealState(DWORD dwUserID,BYTE cbState, DWORD dwStateMark)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetSubUserCongealState");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@cbState"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbState));
	m_WorkDataBase->AddParamter(TEXT("@dwStateMark"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwStateMark));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//注册子帐号存储过程
LONG CDataBaseSink::SPRegisterSubUser(DWORD dwParentUserID,
									  LPCTSTR pszAccount,LPCTSTR pszName,LPCTSTR pszPassword)
{
	DOUBLE tmRegisterTime = helper_GetNowTime();

	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_RegisterSubUser");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwParentUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwParentUserID));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(pszAccount),_variant_t(pszAccount));
	m_WorkDataBase->AddParamter(TEXT("@szName"),adParamInput,adVarChar,CountString(pszName),_variant_t(pszName));
	m_WorkDataBase->AddParamter(TEXT("@szPassword"),adParamInput,adVarChar,CountString(pszPassword),_variant_t(pszPassword));
	m_WorkDataBase->AddParamter(TEXT("@fRegisterTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)tmRegisterTime));
	m_WorkDataBase->AddParamter(TEXT("@cbUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(1));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//子帐号基本信息设定存储过程
LONG CDataBaseSink::SPSetBaiscSubUserInfo(DWORD dwUserID,
									   LPCTSTR pszName,LPCTSTR pszPassword)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_SetBaiscSubUserInfo");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@szName"),adParamInput,adVarChar,CountString(pszName),_variant_t(pszName));
	m_WorkDataBase->AddParamter(TEXT("@szPassword"),adParamInput,adVarChar,CountString(pszPassword),_variant_t(pszPassword));
	m_WorkDataBase->ExecuteCommand(true);

	return m_WorkDataBase->GetReturnValue();
}
//写入操作记录存储过程
LONG CDataBaseSink::SPInsertOperationInfo(tagOperationData& OperationData)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_InsertOperationInfo");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@cbUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(OperationData.Header.cbUserType));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(OperationData.Header.szAccount),_variant_t(OperationData.Header.szAccount));
	m_WorkDataBase->AddParamter(TEXT("@dwOperationUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwOperationUserID));
	m_WorkDataBase->AddParamter(TEXT("@cbOperationUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(OperationData.Header.cbOperationUserType));
	m_WorkDataBase->AddParamter(TEXT("@szOperationAccount"),adParamInput,adVarChar,CountString(OperationData.Header.szOperationAccount),_variant_t(OperationData.Header.szOperationAccount));
	m_WorkDataBase->AddParamter(TEXT("@dwOPType"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwOPType));
	m_WorkDataBase->AddParamter(TEXT("@fTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)OperationData.Header.fTime));
	
	m_WorkDataBase->AddParamter(TEXT("@dwIPAddress"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwIPAddress));
	
	DWORD nParamSize = sizeof(OperationData.Content);
	BYTE* pParam     = (BYTE*)OperationData.Content.data_Nothing;

	SAFEARRAYBOUND   safebound[1];           
	safebound[0].lLbound	 = 0;   
	safebound[0].cElements   = nParamSize;   

	SAFEARRAY*	psa = SafeArrayCreate(VT_UI1,1,safebound);   
	GT_ASSERT(psa);
	for(long i = 0; i < (long)nParamSize; i++)   
		SafeArrayPutElement(psa,&i,pParam++);   

	VARIANT			 varBlob;   
	varBlob.vt		 = VT_ARRAY|VT_UI1;   
	varBlob.parray   = psa;   
	   
	m_WorkDataBase->AddParamter(TEXT("@nbContent"),adParamInput,adVarBinary,nParamSize,_variant_t(varBlob));
	m_WorkDataBase->ExecuteCommand(true);
	
	SafeArrayDestroy(psa);

	return m_WorkDataBase->GetReturnValue();
}
//查询操作记录存储过程
LONG CDataBaseSink::SPGetOperationList(DBR_GP_GetOperation * pGetOperation)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetOperationList");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	WORD wPageIndex = pGetOperation->RequestData.wPageIndex;
	if(wPageIndex == 0xffff)
		wPageIndex = 0;
	m_WorkDataBase->AddParamter(TEXT("@wPageIndex"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wPageIndex));
	m_WorkDataBase->AddParamter(TEXT("@wPageSize"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetOperation->RequestData.wPageSize));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(pGetOperation->RequestData.szAccount),_variant_t(pGetOperation->RequestData.szAccount));
	m_WorkDataBase->AddParamter(TEXT("@cbUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetOperation->RequestData.cbUserType));
	m_WorkDataBase->AddParamter(TEXT("@dwOPType"),adParamInput,adInteger,sizeof(long),_variant_t(pGetOperation->RequestData.dwOPType));
	m_WorkDataBase->AddParamter(TEXT("@fBeginTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetOperation->RequestData.fBeginTime));
	m_WorkDataBase->AddParamter(TEXT("@fEndTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetOperation->RequestData.fEndTime));
	BYTE cbGetTotalCount = pGetOperation->RequestData.wPageIndex == 0xffff ? 1 : 0;
	m_WorkDataBase->AddParamter(TEXT("@cbGetTotalCount"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)cbGetTotalCount));
	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}

//查询报表记录方式-1存储过程
LONG CDataBaseSink::SPGetChartViewOneList(DBR_GP_GetChartViewOne * pGetChartViewOne)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetChartViewOneList");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@cbParentFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewOne->RequestData.cbParentFlag));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(pGetChartViewOne->RequestData.szAccount),_variant_t(pGetChartViewOne->RequestData.szAccount));
	m_WorkDataBase->AddParamter(TEXT("@fBeginTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetChartViewOne->RequestData.fBeginTime));
	m_WorkDataBase->AddParamter(TEXT("@fEndTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetChartViewOne->RequestData.fEndTime));
	m_WorkDataBase->AddParamter(TEXT("@wGameType"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewOne->RequestData.wGameType));
	m_WorkDataBase->AddParamter(TEXT("@wGameKind"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewOne->RequestData.wGameKind));
	m_WorkDataBase->AddParamter(TEXT("@cbSortType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewOne->RequestData.cbSortType));
	m_WorkDataBase->AddParamter(TEXT("@cbGameRoundType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewOne->RequestData.cbGameRoundType));

	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}

//查询报表记录方式-2存储过程
LONG CDataBaseSink::SPGetChartViewTwoList(DBR_GP_GetChartViewTwo * pGetChartViewTwo)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetChartViewTwoList");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	WORD wPageIndex = pGetChartViewTwo->RequestData.wPageIndex;
	if(wPageIndex == 0xffff)
		wPageIndex = 0;
	m_WorkDataBase->AddParamter(TEXT("@wPageIndex"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wPageIndex));
	m_WorkDataBase->AddParamter(TEXT("@wPageSize"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewTwo->RequestData.wPageSize));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(pGetChartViewTwo->RequestData.szAccount),_variant_t(pGetChartViewTwo->RequestData.szAccount));
	m_WorkDataBase->AddParamter(TEXT("@fBeginTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetChartViewTwo->RequestData.fBeginTime));
	m_WorkDataBase->AddParamter(TEXT("@fEndTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetChartViewTwo->RequestData.fEndTime));
	m_WorkDataBase->AddParamter(TEXT("@cbLevel"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewTwo->RequestData.cbLevel));
	m_WorkDataBase->AddParamter(TEXT("@wGameType"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewTwo->RequestData.wGameType));
	m_WorkDataBase->AddParamter(TEXT("@wGameKind"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewTwo->RequestData.wGameKind));
	m_WorkDataBase->AddParamter(TEXT("@cbSortType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewTwo->RequestData.cbSortType));
	m_WorkDataBase->AddParamter(TEXT("@cbGameRoundType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewTwo->RequestData.cbGameRoundType));
	BYTE cbGetTotalCount = pGetChartViewTwo->RequestData.wPageIndex == 0xffff ? 1 : 0;
	m_WorkDataBase->AddParamter(TEXT("@cbGetTotalCount"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)cbGetTotalCount));
	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}

//查询游戏局记录存储过程
LONG CDataBaseSink::SPGetBetHistoryList(DBR_GP_GetBetHistory * pGetBetHistory)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetBetHistoryList");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	WORD wPageIndex = pGetBetHistory->RequestData.wPageIndex;
	if(wPageIndex == 0xffff)
		wPageIndex = 0;
	m_WorkDataBase->AddParamter(TEXT("@wPageIndex"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wPageIndex));
	m_WorkDataBase->AddParamter(TEXT("@wPageSize"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetBetHistory->RequestData.wPageSize));
	m_WorkDataBase->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(pGetBetHistory->RequestData.szAccount),_variant_t(pGetBetHistory->RequestData.szAccount));
	m_WorkDataBase->AddParamter(TEXT("@fBeginTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetBetHistory->RequestData.fBeginTime));
	m_WorkDataBase->AddParamter(TEXT("@fEndTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetBetHistory->RequestData.fEndTime));
	m_WorkDataBase->AddParamter(TEXT("@wGameType"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetBetHistory->RequestData.wGameType));
	m_WorkDataBase->AddParamter(TEXT("@wGameKind"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetBetHistory->RequestData.wGameKind));
	m_WorkDataBase->AddParamter(TEXT("@cbSortType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetBetHistory->RequestData.cbSortType));
	m_WorkDataBase->AddParamter(TEXT("@cbGameRoundType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetBetHistory->RequestData.cbGameRoundType));
	BYTE cbGetTotalCount = pGetBetHistory->RequestData.wPageIndex == 0xffff ? 1 : 0;
	m_WorkDataBase->AddParamter(TEXT("@cbGetTotalCount"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)cbGetTotalCount));
	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}
//查询游戏局记录详细投注存储过程
LONG CDataBaseSink::SPGetDetailBetScore(DBR_GP_GetDetailBetScore * pGetDetailBetScore)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetDetailBetScore");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@llUCID"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetDetailBetScore->RequestData.fUCID));
	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}
//查询报表记录方式-3存储过程
LONG CDataBaseSink::SPGetChartViewThree(DBR_GP_GetChartViewThree * pGetChartViewThree)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_GetChartViewThree");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(LONG),_variant_t((LONG)pGetChartViewThree->RequestData.dwUserID));
	m_WorkDataBase->AddParamter(TEXT("@fBeginTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetChartViewThree->RequestData.fBeginTime));
	m_WorkDataBase->AddParamter(TEXT("@fEndTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pGetChartViewThree->RequestData.fEndTime));
	m_WorkDataBase->AddParamter(TEXT("@wGameType"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewThree->RequestData.wGameType));
	m_WorkDataBase->AddParamter(TEXT("@wGameKind"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pGetChartViewThree->RequestData.wGameKind));
	m_WorkDataBase->AddParamter(TEXT("@cbGameRoundType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pGetChartViewThree->RequestData.cbGameRoundType));

	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}
//查询游戏主信息
LONG CDataBaseSink::SPLoadGameMainInfo()
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_LoadGameMainInfo");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->ExecuteCommand(true);
	return m_WorkDataBase->GetReturnValue();
}
//注销帐号登陆存储过程
LONG CDataBaseSink::SPLogoutUser(DWORD dwLogonServerAddr,
								 WORD wListenPort,
								 DBR_GP_LogoutUser * pLogoutUser)
{
	//执行存储过程
	m_WorkDataBase->ClearAllParameters();
	m_WorkDataBase->SetSPName("GSP_GP_LogoutUser");
	m_WorkDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_WorkDataBase->AddParamter(TEXT("@dwLogonServerIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwLogonServerAddr));
	m_WorkDataBase->AddParamter(TEXT("@wLogonServerListenPort"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wListenPort));
	m_WorkDataBase->AddParamter(TEXT("@dwRealUserID"),adParamInput,adInteger,sizeof(long),_variant_t(pLogoutUser->dwRealUserID));
	m_WorkDataBase->AddParamter(TEXT("@cbUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pLogoutUser->cbUserType));
	m_WorkDataBase->ExecuteCommand(false);
	return m_WorkDataBase->GetReturnValue();

}
//////////////////////////////////////////////////////////////////////////
//读取帐号信息
bool CDataBaseSink::helper_DBGetFieldValue_UserData(CDataBaseHelper &WorkDataBase, 
													tagUserData& UserData,DWORD* pdwRealUserID,BYTE* pcbUserType)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	WorkDataBase->GetFieldValue(TEXT("GUUserID"),UserData.dwUserID);
	WorkDataBase->GetFieldValue(TEXT("GUParentUserID"),UserData.dwParentUserID);
	WorkDataBase->GetFieldValue(TEXT("GUHighUserID0"),UserData.dwHighUserID0);
	WorkDataBase->GetFieldValue(TEXT("GUHighUserID1"),UserData.dwHighUserID1);
	WorkDataBase->GetFieldValue(TEXT("GUHighUserID2"),UserData.dwHighUserID2);
	WorkDataBase->GetFieldValue(TEXT("GUHighUserID3"),UserData.dwHighUserID3);
	WorkDataBase->GetFieldValue(TEXT("GUHighUserID4"),UserData.dwHighUserID4);
	WorkDataBase->GetFieldValue(TEXT("GUAccount"),UserData.szAccount, NAME_LEN);
	WorkDataBase->GetFieldValue(TEXT("GULevel"),UserData.cbLevel);
	WorkDataBase->GetFieldValue(TEXT("GUState"),UserData.cbState);
	WorkDataBase->GetFieldValue(TEXT("GUName"),UserData.szName, NAME_LEN);
	WorkDataBase->GetFieldValue(TEXT("GUFaceID"),UserData.cbFaceID);
	WorkDataBase->GetFieldValue(TEXT("GUStateCongealFlag"),UserData.cbStateCongealFlag);
	WorkDataBase->GetFieldValue(TEXT("GUUserRight"),UserData.dwUserRight);
	WorkDataBase->GetFieldValue(TEXT("GUMasterRight"),UserData.dwMasterRight);
	WorkDataBase->GetFieldValue(TEXT("GUMeScore"),UserData.UserScore.fScore);
	WorkDataBase->GetFieldValue(TEXT("GUMidScore"),UserData.fMidScore);
	WorkDataBase->GetFieldValue(TEXT("GULowScore"),UserData.fLowScore);
	WorkDataBase->GetFieldValue(TEXT("GUOccupancyRate"),UserData.fOccupancyRate);
	WorkDataBase->GetFieldValue(TEXT("GUOccupancyRate_NoFlag"),UserData.cbOccupancyRate_NoFlag);
	WorkDataBase->GetFieldValue(TEXT("GULowOccupancyRate_Max"),UserData.fLowOccupancyRate_Max);
	WorkDataBase->GetFieldValue(TEXT("GULowOccupancyRate_Max_NoFlag"),UserData.cbLowOccupancyRate_Max_NoFlag);
	WorkDataBase->GetFieldValue(TEXT("GULowOccupancyRate_Min"),UserData.fLowOccupancyRate_Min);
	WorkDataBase->GetFieldValue(TEXT("GULowOccupancyRate_Min_NoFlag"),UserData.cbLowOccupancyRate_Min_NoFlag);
	WorkDataBase->GetFieldValue(TEXT("GUTaxOccupancyRate"),UserData.fTaxOccupancyRate);
	WorkDataBase->GetFieldValue(TEXT("GURollbackRate"),UserData.fRollbackRate);
	WorkDataBase->GetFieldValue(TEXT("GUBetLimit"),UserData.dwBetLimit);
	WorkDataBase->GetFieldValue(TEXT("GURegisterTime"),UserData.fRegisterTime);
	WorkDataBase->GetFieldValue(TEXT("GULessUserCount"),UserData.wLessUserCount);

	if(pdwRealUserID != NULL)
		WorkDataBase->GetFieldValue(TEXT("RealUserID"),*pdwRealUserID);
	if(pcbUserType != NULL)
		WorkDataBase->GetFieldValue(TEXT("UserType"),*pcbUserType);

	return true;
}
//读取子帐号信息
bool CDataBaseSink::helper_DBGetFieldValue_SubUserData(CDataBaseHelper &WorkDataBase, tagSubUserData& UserData)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	WorkDataBase->GetFieldValue(TEXT("MUUserID"),UserData.dwUserID);
	WorkDataBase->GetFieldValue(TEXT("MUParentUserID"),UserData.dwParentUserID);
	WorkDataBase->GetFieldValue(TEXT("MUHighUserID0"),UserData.dwHighUserID0);
	WorkDataBase->GetFieldValue(TEXT("MUHighUserID1"),UserData.dwHighUserID1);
	WorkDataBase->GetFieldValue(TEXT("MUHighUserID2"),UserData.dwHighUserID2);
	WorkDataBase->GetFieldValue(TEXT("MUHighUserID3"),UserData.dwHighUserID3);
	WorkDataBase->GetFieldValue(TEXT("MUHighUserID4"),UserData.dwHighUserID4);
	WorkDataBase->GetFieldValue(TEXT("MUAccount"),UserData.szAccount, NAME_LEN);
	WorkDataBase->GetFieldValue(TEXT("MUState"),UserData.cbState);
	WorkDataBase->GetFieldValue(TEXT("MUName"),UserData.szName, NAME_LEN);
	WorkDataBase->GetFieldValue(TEXT("MUStateCongealFlag"),UserData.cbStateCongealFlag);
	WorkDataBase->GetFieldValue(TEXT("MUUserRight"),UserData.dwUserRight);
	WorkDataBase->GetFieldValue(TEXT("MUMasterRight"),UserData.dwMasterRight);
	WorkDataBase->GetFieldValue(TEXT("MURegisterTime"),UserData.fRegisterTime);
	return true;
}
//读取帐号列表信息
bool CDataBaseSink::helper_DBGetFieldValue_UserDataArray(CDataBaseHelper &WorkDataBase, tagUserDataArray& UserDataArray)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while (WorkDataBase->IsEndRecordset()==false)
	{
		tagUserData UserData;
		//读取信息
		helper_DBGetFieldValue_UserData(WorkDataBase,UserData);
		//保存信息
		UserDataArray.Add(UserData);

		WorkDataBase->MoveToNext();

	}
	
	return true;
}
//读取子帐号列表信息
bool CDataBaseSink::helper_DBGetFieldValue_SubUserDataArray(CDataBaseHelper &WorkDataBase, tagSubUserDataArray& UserDataArray)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while (WorkDataBase->IsEndRecordset()==false)
	{
		tagSubUserData UserData;
		//读取信息
		helper_DBGetFieldValue_SubUserData(WorkDataBase,UserData);
		//保存信息
		UserDataArray.Add(UserData);

		WorkDataBase->MoveToNext();

	}

	return true;
}
//初始化操作记录数据
void CDataBaseSink::helper_InitOperationData(tagOperationData& OperationData,DWORD dwOPType, DWORD dwIPAddress)
{

	memset(&OperationData, 0,sizeof(OperationData));
	OperationData.Header.dwOPType	= dwOPType;
	OperationData.Header.fTime		= helper_GetNowTime();
	OperationData.Header.dwIPAddress= dwIPAddress;
}
//填充操作记录包头
void CDataBaseSink::helper_FillOperationHeader(tagOperationData& OperationData,
											   DWORD			dwUserID,
											   BYTE			cbUserType,
											   LPCTSTR			szAccount,
											   DWORD			dwOperationUserID,
											   BYTE			cbOperationUserType,
											   LPCTSTR			szOperationAccount)
{
	OperationData.Header.dwUserID				= dwUserID;
	OperationData.Header.cbUserType				= cbUserType;
	_tcscpy(OperationData.Header.szAccount, szAccount);
	OperationData.Header.dwOperationUserID		= dwOperationUserID;
	OperationData.Header.cbOperationUserType	= cbOperationUserType;
	_tcscpy(OperationData.Header.szOperationAccount, szOperationAccount);
}
//获取当前时间
DOUBLE CDataBaseSink::helper_GetNowTime()
{
	__time64_t tmTime;
	_time64(&tmTime);
	return (DOUBLE)tmTime;
}
//转换字符代码
void CDataBaseSink::helper_ConvertACPToUtf8_OperationData(tagOperationData* pOperationData)
{
	ConvertACPToUtf8(pOperationData->Header.szAccount, NAME_LEN,
		pOperationData->Header.szAccount, NAME_LEN);
	ConvertACPToUtf8(pOperationData->Header.szOperationAccount, NAME_LEN,
		pOperationData->Header.szOperationAccount, NAME_LEN);
	switch(pOperationData->Header.dwOPType)
	{
	case OPT_NewSubAccount:
		ConvertACPToUtf8(pOperationData->Content.data_NewSubAccount.szName, NAME_LEN,
			pOperationData->Content.data_NewSubAccount.szName, NAME_LEN);
		break;
	case OPT_SetBasicInfo:
		ConvertACPToUtf8(pOperationData->Content.data_SetBasicInfo.szName, NAME_LEN,
			pOperationData->Content.data_SetBasicInfo.szName, NAME_LEN);
		break;
	case OPT_KickUser:
		{
			ConvertACPToUtf8(pOperationData->Content.data_KickUser.szTypeName, NAME_LEN,
				pOperationData->Content.data_KickUser.szTypeName, NAME_LEN);
			ConvertACPToUtf8(pOperationData->Content.data_KickUser.szKindName, NAME_LEN,
				pOperationData->Content.data_KickUser.szKindName, NAME_LEN);
			ConvertACPToUtf8(pOperationData->Content.data_KickUser.szServerName, NAME_LEN,
				pOperationData->Content.data_KickUser.szServerName, NAME_LEN);
		}
		break;
	default:
		break;
	}
}
//转换字符代码
void CDataBaseSink::helper_ConvertACPToUtf8_BetHistoryData(tagBetHistoryData* pBetHistoryData)
{
	ConvertACPToUtf8(pBetHistoryData->szTypeName, TYPE_LEN,
		pBetHistoryData->szTypeName, TYPE_LEN);
	ConvertACPToUtf8(pBetHistoryData->szKindName, KIND_LEN,
		pBetHistoryData->szKindName, KIND_LEN);
	ConvertACPToUtf8(pBetHistoryData->szServerName, SERVER_LEN,
		pBetHistoryData->szServerName, SERVER_LEN);

}
//读取操作记录信息
bool CDataBaseSink::helper_DBGetFieldValue_OperationData(CDataBaseHelper &WorkDataBase, tagOperationData& OperationData)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	WorkDataBase->GetFieldValue(TEXT("OHUserID"),OperationData.Header.dwUserID);
	WorkDataBase->GetFieldValue(TEXT("OHUserType"),OperationData.Header.cbUserType);
	WorkDataBase->GetFieldValue(TEXT("OHAccount"),OperationData.Header.szAccount, NAME_LEN);
	WorkDataBase->GetFieldValue(TEXT("OHOperationUserID"),OperationData.Header.dwOperationUserID);
	WorkDataBase->GetFieldValue(TEXT("OHOperationUserType"),OperationData.Header.cbOperationUserType);
	WorkDataBase->GetFieldValue(TEXT("OHOperationAccount"),OperationData.Header.szOperationAccount, NAME_LEN);
	WorkDataBase->GetFieldValue(TEXT("OHOPType"),OperationData.Header.dwOPType);
	WorkDataBase->GetFieldValue(TEXT("OHTime"),OperationData.Header.fTime);
	WorkDataBase->GetFieldValue(TEXT("OHIPAddress"),OperationData.Header.dwIPAddress);
	WorkDataBase->GetFieldValue(TEXT("OPContent"),(TCHAR*)&OperationData.Content, sizeof(OperationData.Content));
	return true;
}
//读取操作记录列表信息
bool CDataBaseSink::helper_DBGetFieldValue_OperationDataArray(CDataBaseHelper &WorkDataBase, 
															  tagOperationDataArray& OperationDataArray)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while (WorkDataBase->IsEndRecordset()==false)
	{
		tagOperationData OperationData;
		//读取信息
		helper_DBGetFieldValue_OperationData(WorkDataBase,OperationData);
		//保存信息
		OperationDataArray.Add(OperationData);

		WorkDataBase->MoveToNext();

	}
	return true;
}
//读取报表记录方式-1列表配置信息
bool CDataBaseSink::helper_DBGetFieldValue_ChartViewOneListConfig(CDataBaseHelper &WorkDataBase, CMD_GP_ChartViewOneListConfig* pConfig)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	if(WorkDataBase->IsEndRecordset()==false)
	{
		WorkDataBase->GetFieldValue(TEXT("OUT_GUAccount"),pConfig->szAccount,NAME_LEN);
		WorkDataBase->GetFieldValue(TEXT("OUT_GULevel"),pConfig->cbLevel);
		return true;
	}
	else
		return false;
}
//读取报表记录方式-1信息
bool CDataBaseSink::helper_DBGetFieldValue_ChartViewOneData(CDataBaseHelper &WorkDataBase, tagChartViewOneData& ChartViewOneData)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	WorkDataBase->GetFieldValue(TEXT("GUAccount"),ChartViewOneData.szAccount,NAME_LEN);

	WorkDataBase->GetFieldValue(TEXT("GameRoundCount"),ChartViewOneData.dwGameRoundCount);
	WorkDataBase->GetFieldValue(TEXT("TotalBetScore"),ChartViewOneData.fTotalBetScore);
	WorkDataBase->GetFieldValue(TEXT("TotalWinScore"),ChartViewOneData.fTotalWinScore);
	WorkDataBase->GetFieldValue(TEXT("TotalTaxScore"),ChartViewOneData.fTotalTaxScore);

	WorkDataBase->GetFieldValue(TEXT("WinScoreOccupancy_High"),ChartViewOneData.fWinScore_High);
	WorkDataBase->GetFieldValue(TEXT("WinScoreOccupancy_Self"),ChartViewOneData.fWinScore_Self);
	WorkDataBase->GetFieldValue(TEXT("WinScoreOccupancy_Less"),ChartViewOneData.fWinScore_Less);

	WorkDataBase->GetFieldValue(TEXT("TaxScoreOccupancy_High"),ChartViewOneData.fTaxScore_High);
	WorkDataBase->GetFieldValue(TEXT("TaxScoreOccupancy_Self"),ChartViewOneData.fTaxScore_Self);
	WorkDataBase->GetFieldValue(TEXT("TaxScoreOccupancy_Less"),ChartViewOneData.fTaxScore_Less);

	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_Total"),ChartViewOneData.fValidScore_Total);
	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_High"),ChartViewOneData.fValidScore_High);
	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_HighRollback"),ChartViewOneData.fValidScore_High_Rollback);
	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_Less"),ChartViewOneData.fValidScore_Less);
	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_LessRollback"),ChartViewOneData.fValidScore_Less_Rollback);

	WorkDataBase->GetFieldValue(TEXT("PaidScore_High"),ChartViewOneData.fPaidScore_High);
	WorkDataBase->GetFieldValue(TEXT("PaidScore_Less"),ChartViewOneData.fPaidScore_Less);

	return true;
}
//读取报表记录方式-1列表信息
bool CDataBaseSink::helper_DBGetFieldValue_ChartViewOneDataArray(CDataBaseHelper &WorkDataBase, 
															  tagChartViewOneDataArray& ChartViewOneDataArray)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while (WorkDataBase->IsEndRecordset()==false)
	{
		tagChartViewOneData ChartViewOneData;
		//读取信息
		helper_DBGetFieldValue_ChartViewOneData(WorkDataBase,ChartViewOneData);
		//保存信息
		ChartViewOneDataArray.Add(ChartViewOneData);

		WorkDataBase->MoveToNext();

	}
	return true;
}
//读取报表记录方式-2信息
bool CDataBaseSink::helper_DBGetFieldValue_ChartViewTwoData(CDataBaseHelper &WorkDataBase, tagChartViewTwoData& ChartViewTwoData)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	WorkDataBase->GetFieldValue(TEXT("GUAccount"),ChartViewTwoData.szAccount,NAME_LEN);

	WorkDataBase->GetFieldValue(TEXT("GameRoundCount"),ChartViewTwoData.dwGameRoundCount);
	WorkDataBase->GetFieldValue(TEXT("TotalBetScore"),ChartViewTwoData.fTotalBetScore);
	WorkDataBase->GetFieldValue(TEXT("TotalWinScore"),ChartViewTwoData.fTotalWinScore);
	WorkDataBase->GetFieldValue(TEXT("TotalTaxScore"),ChartViewTwoData.fTotalTaxScore);

	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_Total"),ChartViewTwoData.fValidScore_Total);
	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_LessRollback"),ChartViewTwoData.fValidScore_Less_Rollback);


	return true;
}
//读取报表记录方式-2列表信息
bool CDataBaseSink::helper_DBGetFieldValue_ChartViewTwoDataArray(CDataBaseHelper &WorkDataBase, 
																 tagChartViewTwoDataArray& ChartViewTwoDataArray)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while (WorkDataBase->IsEndRecordset()==false)
	{
		tagChartViewTwoData ChartViewTwoData;
		//读取信息
		helper_DBGetFieldValue_ChartViewTwoData(WorkDataBase,ChartViewTwoData);
		//保存信息
		ChartViewTwoDataArray.Add(ChartViewTwoData);

		WorkDataBase->MoveToNext();

	}
	return true;
}
//读取局记录信息
bool CDataBaseSink::helper_DBGetFieldValue_BetHistoryData(CDataBaseHelper &WorkDataBase, tagBetHistoryData& BetHistoryData)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	WorkDataBase->GetFieldValue(TEXT("UCID"),BetHistoryData.fUCID);

	WorkDataBase->GetFieldValue(TEXT("GameRoundID"),BetHistoryData.fGameRoundID);
	WorkDataBase->GetFieldValue(TEXT("StartTime"),BetHistoryData.fStartTime);
	WorkDataBase->GetFieldValue(TEXT("ProcessType"),BetHistoryData.wProcessType);

	WorkDataBase->GetFieldValue(TEXT("TypeName"),BetHistoryData.szTypeName, TYPE_LEN);
	WorkDataBase->GetFieldValue(TEXT("KindName"),BetHistoryData.szKindName, KIND_LEN);
	WorkDataBase->GetFieldValue(TEXT("ServerName"),BetHistoryData.szServerName, SERVER_LEN);
	WorkDataBase->GetFieldValue(TEXT("TableID"),BetHistoryData.wTableID);

	WorkDataBase->GetFieldValue(TEXT("TotalBetScore"),BetHistoryData.fTotalBetScore);
	WorkDataBase->GetFieldValue(TEXT("TotalWinScore"),BetHistoryData.fTotalWinScore);
	WorkDataBase->GetFieldValue(TEXT("TotalTaxScore"),BetHistoryData.fTotalTaxScore);
	WorkDataBase->GetFieldValue(TEXT("ValidBetScore_LessRollback"),BetHistoryData.fValidScore_Less_Rollback);

	WorkDataBase->GetFieldValue(TEXT("EndReason"),BetHistoryData.cbEndReason);
	BetHistoryData.wEndDataSize = WorkDataBase->GetFieldValue(TEXT("EndData"),(TCHAR*)&BetHistoryData.cbEndData, sizeof(BetHistoryData.cbEndData));

	return true;
}
//读取游戏局记录详细投注
bool CDataBaseSink::helper_DBGetFieldValue_DetailBetScoreSuccess(CDataBaseHelper &WorkDataBase, CMD_GP_GetDetailBetScoreSuccess *pGetDetailBetScoreSuccess)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	if (WorkDataBase->IsEndRecordset()==false)
	{
		pGetDetailBetScoreSuccess->wDetailBetScoreSize = WorkDataBase->GetFieldValue(TEXT("DetailBetScore"),(TCHAR*)pGetDetailBetScoreSuccess->cbDetailBetScore, sizeof(pGetDetailBetScoreSuccess->cbDetailBetScore));
		
		return true;
	}
	else
	return false;
}
//读取报表记录方式-3信息
bool CDataBaseSink::helper_DBGetFieldValue_ChartViewThreeSucess(CDataBaseHelper &WorkDataBase, CMD_GP_ChartViewThreeSucess* pChartViewThreeSucess)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	if (WorkDataBase->IsEndRecordset()==false)
	{
		WorkDataBase->GetFieldValue(TEXT("GameRoundCount"),pChartViewThreeSucess->dwGameRoundCount);
		WorkDataBase->GetFieldValue(TEXT("TotalBetScore"),pChartViewThreeSucess->fTotalBetScore);
		WorkDataBase->GetFieldValue(TEXT("TotalWinScore"),pChartViewThreeSucess->fTotalWinScore);

		WorkDataBase->GetFieldValue(TEXT("ValidBetScore_LessRollback"),pChartViewThreeSucess->fValidScore_Less_Rollback);

		return true;
	}
	else
		return false;
}
//读取局记录列表信息
bool CDataBaseSink::helper_DBGetFieldValue_BetHistoryDataArray(CDataBaseHelper &WorkDataBase, 
															   tagBetHistoryDataArray& BetHistoryDataArray)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while (WorkDataBase->IsEndRecordset()==false)
	{
		tagBetHistoryData BetHistoryData;
		//读取信息
		helper_DBGetFieldValue_BetHistoryData(WorkDataBase,BetHistoryData);
		//保存信息
		BetHistoryDataArray.Add(BetHistoryData);

		WorkDataBase->MoveToNext();

	}
	return true;
}
//读取游戏主信息
bool CDataBaseSink::helper_DBGetFieldValue_GameMainInfo(CDataBaseHelper &WorkDataBase,DBR_Update_GameMainInfo &GameMainInfo)
{
	GT_ASSERT(WorkDataBase.GetInterface());

	if(WorkDataBase->IsEndRecordset()==false)
	{
		WorkDataBase->GetFieldValue(TEXT("GMMainWebPageAddress"),GameMainInfo.GameInfo.szMainWebPageAddress, CountArray(GameMainInfo.GameInfo.szMainWebPageAddress));
		WorkDataBase->GetFieldValue(TEXT("GMFieldLiveVideoAddress"),GameMainInfo.GameInfo.szFieldLiveVideoAddress, CountArray(GameMainInfo.GameInfo.szFieldLiveVideoAddress));
		WorkDataBase->GetFieldValue(TEXT("GMVideoTapeAddress"),GameMainInfo.GameInfo.szVideoTapeAddress,CountArray(GameMainInfo.GameInfo.szVideoTapeAddress));
		WorkDataBase->GetFieldValue(TEXT("GMCurManagementNotice"),GameMainInfo.ManagementNotice.szNotice,CountArray(GameMainInfo.ManagementNotice.szNotice));
		WorkDataBase->GetFieldValue(TEXT("GMCurGameNotice"),GameMainInfo.GameNotice.szNotice, CountArray(GameMainInfo.GameNotice.szNotice));
		return true;
	}
	return false;
}
//检测客户端登陆
inline LONG CDataBaseSink::helper_AllowClientLogon(BYTE cbClientType,BYTE cbUserType,BYTE cbUserLevel)
{
	switch(cbClientType)
	{
		case CLIENTTYPE_GAME://游戏端
			{
				if(cbUserType == USERTYPE_SUB ||
					cbUserLevel != USERLEVEL_GAMEUSER)
					return ErrCode_CMD_GP_LogonError_Password;
			}
			break;
		case CLIENTTYPE_MAN://管理端
			{
				if(cbUserLevel == USERLEVEL_GAMEUSER)
					return ErrCode_CMD_GP_LogonError_Password;
			}
			break;
		case CLIENTTYPE_SERVICE://客户中心
			{
				if(cbUserLevel != 0)
					return ErrCode_CMD_GP_LogonError_Password;
			}
			break;
		case CLIENTTYPE_CONTROL://控制端
			{
				if(cbUserLevel != 0 ||
					cbUserType != USERTYPE_DEALER)
					return ErrCode_CMD_GP_LogonError_Password;
			}
			break;
		default:
			break;
	}
	return 0;
}