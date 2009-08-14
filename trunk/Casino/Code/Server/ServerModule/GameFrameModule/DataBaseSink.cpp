#include "StdAfx.h"
#include "Afxinet.h"
#include "DataBaseSink.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseSink::CDataBaseSink()
{
	//设置变量
	m_pIEventService=NULL;
	m_pGameUserDBInfo=NULL;
	m_pGameScoreDBInfo=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	return;
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

//调度模块启动
bool __cdecl CDataBaseSink::StartService(IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(m_pGameUserDBInfo!=NULL);
	GT_ASSERT(m_pGameScoreDBInfo!=NULL);
	GT_ASSERT(m_pGameServiceOption!=NULL);
	GT_ASSERT(m_AttemperEvent.IsValid()==true);

	//创建实例
	if ((m_GameUserDB.GetInterface()==NULL)&&(m_GameUserDB.CreateInstance()==false))
	{
		m_pIEventService->ShowEventNotify(TEXT("用户数据库对象创建失败"),Level_Exception);
		return false;
	}

	//创建实例
	if ((m_GameScoreDB.GetInterface()==NULL)&&(m_GameScoreDB.CreateInstance()==false))
	{
		m_pIEventService->ShowEventNotify(TEXT("游戏数据库对象创建失败"),Level_Exception);
		return false;
	}

	try
	{
		//变量定义
		BYTE * pcbAddr=NULL;
		TCHAR szDataBaseAddr[16]=TEXT("");

		//连接用户数据库
		pcbAddr=(BYTE *)&m_pGameUserDBInfo->dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_GameUserDB->SetConnectionInfo(szDataBaseAddr,m_pGameUserDBInfo->szDataBasePipeName,
			m_pGameUserDBInfo->wDataBasePort,m_pGameUserDBInfo->szDataBaseName,
			m_pGameUserDBInfo->szDataBaseUser,m_pGameUserDBInfo->szDataBasePass);
		m_GameUserDB->OpenConnection();

		//连接游戏数据库
		pcbAddr=(BYTE *)&m_pGameScoreDBInfo->dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_GameScoreDB->SetConnectionInfo(szDataBaseAddr,m_pGameScoreDBInfo->szDataBasePipeName,
			m_pGameScoreDBInfo->wDataBasePort,m_pGameScoreDBInfo->szDataBaseName,
			m_pGameScoreDBInfo->szDataBaseUser,m_pGameScoreDBInfo->szDataBasePass);
		m_GameScoreDB->OpenConnection();

		return true;
	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	return false;
}

//调度模块关闭
bool __cdecl CDataBaseSink::StopService(IUnknownEx * pIUnknownEx)
{
	try
	{
		//关闭连接
		if (m_GameUserDB.GetInterface()) m_GameUserDB->CloseConnection();
		if (m_GameScoreDB.GetInterface()) m_GameScoreDB->CloseConnection();

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
	case DBR_GR_LOGON_BY_ACCOUNT:	//帐户登录
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_READ_ACCOUNT:		//帐户读取
		{
			return OnRequestReadUser(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_WRITE_GAMEROUND:	//写入游戏局记录操作
		{
			return OnRequestWriteGameRound(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_WRITE_USERCHART:	//写入用户报表记录操作
		{
			return OnRequestWriteUserChart(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE:	//申请游戏局记录-检测信用额度
		{
			return OnRequestAllocGameRoundAndCheckBetScore(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_ALLOC_GAMEROUND:	//申请游戏局记录
		{
			return OnRequestAllocGameRound(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_RESET_BETSCORE:		//复位投注
		{
			return OnRequestResetBetScore(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_INSERT_BETSCORE:	//插入投注
		{
			return OnRequestInsertBetScore(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_KICK_USER:			//踢出房间
		{
			return OnRequestKickUser(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GR_READ_AIUSER:		//装载机器人
		{
			return OnRequestReadAIUser(DataBaseEvent,pDataBuffer,wDataSize);
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

//配置函数
bool CDataBaseSink::InitDataBaseSink(tagDataBaseSinkParameter * pDataBaseSinkParameter, IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(pDataBaseSinkParameter!=NULL);

	//设置变量
	m_pGameUserDBInfo=pDataBaseSinkParameter->pGameUserDBInfo;
	m_pGameScoreDBInfo=pDataBaseSinkParameter->pGameScoreDBInfo;
	m_pGameServiceAttrib=pDataBaseSinkParameter->pGameServiceAttrib;
	m_pGameServiceOption=pDataBaseSinkParameter->pGameServiceOption;

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
		LPCTSTR pszPassword=NULL;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GR_LOGON_BY_ACCOUNT:		//帐户登录
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GR_LogonByAccount));
				if (wDataSize!=sizeof(DBR_GR_LogonByAccount)) return false;

				//执行查询
				DBR_GR_LogonByAccount * pLogonByAccount=(DBR_GR_LogonByAccount *)pDataBuffer;
				lReturnValue=SPLogonByAccount(pLogonByAccount->szAccounts,pLogonByAccount->szPassWord,pLogonByAccount->dwClientIP);
				pszPassword=pLogonByAccount->szPassWord;

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

		//登录失败
		if (lReturnValue!=0L)
		{
			DBR_GR_LogonError LogonError;
			LogonError.wErrorCode=lReturnValue;
			m_AttemperEvent.PostDataBaseEvent(DBR_GR_LOGON_ERROR,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,&LogonError,sizeof(LogonError));
			m_GameUserDB->CloseRecordset();
			return true;
		}

		//变量定义
		DBR_GR_LogonSuccess LogonSuccess;
		memset(&LogonSuccess,0,sizeof(LogonSuccess));
		helper_DBGetFieldValue_UserData(m_GameUserDB, LogonSuccess.UserData,
			LogonSuccess.decOccupancyRate,
			LogonSuccess.decTaxOccupancyRate,
			LogonSuccess.decRollbackRate,
			LogonSuccess.dwGameRight,
			&LogonSuccess.dwRealUserID, 
			&LogonSuccess.cbUserType);

		//投递调度通知
		m_AttemperEvent.PostDataBaseEvent(DBR_GR_LOGON_SUCCESS,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
			&LogonSuccess,sizeof(LogonSuccess));
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());

		//操作失败
		DBR_GR_LogonError LogonError;
		LogonError.wErrorCode=ErrCode_Database;
		m_AttemperEvent.PostDataBaseEvent(DBR_GR_LOGON_ERROR,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
			&LogonError,sizeof(LogonError));
	}

	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//读取请求
bool CDataBaseSink::OnRequestReadUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//登录处理
		LONG lReturnValue=-1;
		LPCTSTR pszPassword=NULL;
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GR_READ_ACCOUNT:		//帐户读取
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GR_ReadUser));
				if (wDataSize!=sizeof(DBR_GR_ReadUser)) return false;

				//执行查询
				DBR_GR_ReadUser * pReadUser=(DBR_GR_ReadUser *)pDataBuffer;
				lReturnValue=SPReadUser(pReadUser->dwUserID);

				break;
			}
		default :	//默认处理
			{
				GT_ASSERT(FALSE); 
				return false;
			}
		}

		//登录失败
		if (lReturnValue!=0L)
		{
			DBR_GR_ReadUserError ReadError;
			ReadError.wErrorCode=lReturnValue;
			m_AttemperEvent.PostDataBaseEvent(DBR_GR_READACCOUNT_ERROR,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,&ReadError,sizeof(ReadError));
			m_GameUserDB->CloseRecordset();
			return true;
		}

		//变量定义
		DBR_GR_ReadUserSuccess ReadSuccess;
		memset(&ReadSuccess,0,sizeof(ReadSuccess));
		helper_DBGetFieldValue_UserData(m_GameUserDB, ReadSuccess.UserData,
			ReadSuccess.decOccupancyRate,
			ReadSuccess.decTaxOccupancyRate,
			ReadSuccess.decRollbackRate,
			ReadSuccess.dwGameRight);
		ReadSuccess.cbUserType = USERTYPE_GAME;
		ReadSuccess.dwRealUserID = ReadSuccess.UserData.dwUserID;

		//投递调度通知
		m_AttemperEvent.PostDataBaseEvent(DBR_GR_READACCOUNT_SUCCESS,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
			&ReadSuccess,sizeof(ReadSuccess));
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
		//操作失败
		DBR_GR_ReadUserError ReadError;
		ReadError.wErrorCode=ErrCode_Database;
		m_AttemperEvent.PostDataBaseEvent(DBR_GR_READACCOUNT_ERROR,DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
			&ReadError,sizeof(ReadError));
	}

	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//写入游戏局记录请求
bool CDataBaseSink::OnRequestWriteGameRound(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_WriteGameRound));
		if (wDataSize!=sizeof(DBR_GR_WriteGameRound)) return false;

		//执行查询
		DBR_GR_WriteGameRound * pWriteGameRound=(DBR_GR_WriteGameRound *)pDataBuffer;
		LONG lReturnValue=SPWriteGameRound(pWriteGameRound);

		//结果处理
		if (lReturnValue!=0L)
		{
			//输出错误
			m_pIEventService->ShowEventNotify("写入游戏局记录请求失败,???",Level_Exception);
			GT_ASSERT(FALSE);
		}
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	return true;
}
//写入用户报表记录请求
bool CDataBaseSink::OnRequestWriteUserChart(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_WriteUserChart));
		if (wDataSize!=sizeof(DBR_GR_WriteUserChart)) return false;

		//执行查询
		DBR_GR_WriteUserChart * pWriteUserChart=(DBR_GR_WriteUserChart *)pDataBuffer;
		LONG lReturnValue=SPWriteUserChart(pWriteUserChart);

		//结果处理
		if (lReturnValue!=0L)
		{
			//输出错误
			m_pIEventService->ShowEventNotify("写入用户报表记录请求失败,???",Level_Exception);
			GT_ASSERT(FALSE);
		}
		else
		{
			if(pWriteUserChart->cbUpdateUserScoreFlag)
			{
				DBR_GR_UserScore UserScore;
				if(helper_DBGetFieldValue_UserScore(m_GameScoreDB,&UserScore))
				{
					UserScore.dwUserID = pWriteUserChart->dwUserID;
					//投递调度通知
					m_AttemperEvent.PostDataBaseEvent(DBR_GR_USERSCORE,
						DataBaseEvent.wIndex,
						DataBaseEvent.wRoundID,
						&UserScore,sizeof(UserScore));
				}
			}
		}
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}
	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//申请游戏局记录-检测信用额度请求
bool CDataBaseSink::OnRequestAllocGameRoundAndCheckBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_AllocGameRoundAndCheckBetScore));
		if (wDataSize!=sizeof(DBR_GR_AllocGameRoundAndCheckBetScore)) return false;

		//执行查询
		DBR_GR_AllocGameRoundAndCheckBetScore * pAllocGameRoundAndCheckBetScore=(DBR_GR_AllocGameRoundAndCheckBetScore *)pDataBuffer;
		ULONGLONG ullReturnValue=SPAllocGameRoundAndCheckBetScore(pAllocGameRoundAndCheckBetScore);
		
		DBR_GR_AllocGameRoundAndCheckBetScoreResult AllocGameRoundAndCheckBetScoreResult;
		memset(&AllocGameRoundAndCheckBetScoreResult, 0, sizeof(AllocGameRoundAndCheckBetScoreResult));
		AllocGameRoundAndCheckBetScoreResult.ullGameRoundID = ullReturnValue;
		AllocGameRoundAndCheckBetScoreResult.wServerID = pAllocGameRoundAndCheckBetScore->wServerID;
		AllocGameRoundAndCheckBetScoreResult.wTableID  = pAllocGameRoundAndCheckBetScore->wTableID;
		memcpy(AllocGameRoundAndCheckBetScoreResult.dwUserID,
			pAllocGameRoundAndCheckBetScore->dwUserID, sizeof(DWORD)*CountArray(AllocGameRoundAndCheckBetScoreResult.dwUserID));
		//结果处理
		if (ullReturnValue==0)
		{
			helper_DBGetFieldValue_AllocGameRoundAndCheckBetScoreResult(m_GameUserDB,AllocGameRoundAndCheckBetScoreResult);
		}
		//投递调度通知
		m_AttemperEvent.PostDataBaseEvent(DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE_RESULT,
			DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
			&AllocGameRoundAndCheckBetScoreResult,sizeof(AllocGameRoundAndCheckBetScoreResult));

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}
	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//申请游戏局记录
bool CDataBaseSink::OnRequestAllocGameRound(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_AllocGameRound));
		if (wDataSize!=sizeof(DBR_GR_AllocGameRound)) return false;

		//执行查询
		DBR_GR_AllocGameRound * pAllocGameRound=(DBR_GR_AllocGameRound *)pDataBuffer;
		ULONGLONG ullReturnValue=SPAllocGameRound(pAllocGameRound);

		//结果处理
		if (ullReturnValue==0)
		{
			GT_ASSERT(FALSE);
		}
		else
		{
			//构造数据
			DBR_GR_AllocGameRoundResult AllocGameRoundResult;
			memset(&AllocGameRoundResult, 0, sizeof(AllocGameRoundResult));
			AllocGameRoundResult.ullGameRoundID = ullReturnValue;
			AllocGameRoundResult.wServerID = pAllocGameRound->wServerID;
			AllocGameRoundResult.wTableID  = pAllocGameRound->wTableID;

			//投递调度通知
			m_AttemperEvent.PostDataBaseEvent(DBR_GR_ALLOC_GAMEROUND_RESULT,
				DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
				&AllocGameRoundResult,sizeof(AllocGameRoundResult));

		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}
	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//复位投注
bool CDataBaseSink::OnRequestResetBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_ResetBetScore));
		if (wDataSize!=sizeof(DBR_GR_ResetBetScore)) return false;

		//执行查询
		DBR_GR_ResetBetScore * pResetBetScore=(DBR_GR_ResetBetScore *)pDataBuffer;
		LONG lReturnValue=SPResetBetScore(pResetBetScore);

		//结果处理
		if (lReturnValue!=0L)
		{
			//输出错误
			m_pIEventService->ShowEventNotify("复位投注失败,???",Level_Exception);
			GT_ASSERT(FALSE);
		}
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	return true;
}
//插入投注
bool CDataBaseSink::OnRequestInsertBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_InsertBetScore));
		if (wDataSize!=sizeof(DBR_GR_InsertBetScore)) return false;

		//执行查询
		DBR_GR_InsertBetScore * pInsertBetScore=(DBR_GR_InsertBetScore *)pDataBuffer;
		LONG lReturnValue=SPInsertBetScore(pInsertBetScore);

		//结果处理
		if (lReturnValue!=0L)
		{
			//输出错误
			m_pIEventService->ShowEventNotify("投注失败,???",Level_Exception);
			GT_ASSERT(FALSE);
		}
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	return true;
}
//踢出房间
bool CDataBaseSink::OnRequestKickUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//效验参数
		GT_ASSERT(wDataSize==sizeof(DBR_GR_KickUser));
		if (wDataSize!=sizeof(DBR_GR_KickUser)) return false;

		//执行查询
		DBR_GR_KickUser * pKickUser=(DBR_GR_KickUser *)pDataBuffer;

		//操作记录
		tagOperationData OperationData;
		//初试化操作数据
		helper_InitOperationData(OperationData, OPT_KickUser, pKickUser->Operator.dwIPAddress);
		//填充操作数据包头
		helper_FillOperationHeader(OperationData,
			pKickUser->dwUserID,
			USERTYPE_GAME,
			pKickUser->szAccount,
			pKickUser->Operator.dwOperationUserID,
			pKickUser->Operator.cbOperationUserType,
			pKickUser->Operator.szOperationAccount);
	
		//
		LONG lReturnValue=SPReadGameTreeNodeName(m_pGameServiceOption->wTypeID,
									m_pGameServiceOption->wKindID,
									m_pGameServiceOption->wServerID);
		//结果处理
		if (lReturnValue!=0L)
		{
			//输出错误
			m_pIEventService->ShowEventNotify("写入 踢出房间 记录失败",Level_Exception);
			GT_ASSERT(FALSE);
		}
		else
		{
			helper_DBGetFieldValue_ReadGameTreeNodeName(m_GameUserDB,
				OperationData.Content.data_KickUser.szTypeName,
				OperationData.Content.data_KickUser.szKindName,
				OperationData.Content.data_KickUser.szServerName);

			//关闭记录集
			m_GameUserDB->CloseRecordset();

			OperationData.Content.data_KickUser.wTableID = pKickUser->RequestData.wTableID;

			
			//写入操作记录
			lReturnValue=SPInsertOperationInfo(OperationData);

			//结果处理
			if (lReturnValue!=0L)
			{
				//输出错误
				m_pIEventService->ShowEventNotify("写入 踢出房间 记录失败",Level_Exception);
				GT_ASSERT(FALSE);
			}

		}
	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//装载机器人
bool CDataBaseSink::OnRequestReadAIUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		LONG lReturnValue = 0;

		//执行装载机器人
		lReturnValue=SPReadAIUser(m_pGameServiceOption->wAIUserCount);

		//结果处理
		if (lReturnValue!=0L)
		{
			//输出错误
			m_pIEventService->ShowEventNotify("装载机器人失败",Level_Exception);
			GT_ASSERT(FALSE);
		}
		else
		{
			//读取信息
			Array_DBR_GR_ReadUserSuccess arrUserAgent;
			Array_DBR_GR_ReadUserSuccess arrUserGame;
			arrUserAgent.SetSize(5);

			if(helper_DBGetFieldValue_AIUser(m_GameUserDB,arrUserAgent,arrUserGame))
			{
				GT_ASSERT(arrUserAgent.GetCount() == 5);
				GT_ASSERT(arrUserGame.GetCount() == m_pGameServiceOption->wAIUserCount);

				BYTE cbBuffer[MAX_QUEUE_PACKET];
				DWORD wDataSize = 0;

				//代理用户通知调度
				for(int i =0; i < arrUserAgent.GetCount(); i ++)
				{
					if((wDataSize+sizeof(DBR_GR_ReadUserSuccess))>(MAX_QUEUE_PACKET-sizeof(NTY_DataBaseEvent)))
					{
						//投递调度通知
						m_AttemperEvent.PostDataBaseEvent(DBR_GR_READ_AIAGENTUSER_SUCCESS,
							DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
							cbBuffer,wDataSize);
						wDataSize = 0;
					}
					memcpy(cbBuffer + wDataSize,
						&arrUserAgent.GetAt(i), sizeof(DBR_GR_ReadUserSuccess));
					wDataSize += sizeof(DBR_GR_ReadUserSuccess);
				}
				if(wDataSize > 0 )
				{
					//投递调度通知
					m_AttemperEvent.PostDataBaseEvent(DBR_GR_READ_AIAGENTUSER_SUCCESS,
						DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
						cbBuffer,wDataSize);
					wDataSize = 0;
				}
				//玩家用户通知调度
				for(int i =0; i < arrUserGame.GetCount(); i ++)
				{
					if((wDataSize+sizeof(DBR_GR_ReadUserSuccess))>(MAX_QUEUE_PACKET-sizeof(NTY_DataBaseEvent)))
					{
						//投递调度通知
						m_AttemperEvent.PostDataBaseEvent(DBR_GR_READ_AIGAMEUSER_SUCCESS,
							DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
							cbBuffer,wDataSize);
						wDataSize = 0;
					}
					memcpy(cbBuffer + wDataSize,
						&arrUserGame.GetAt(i), sizeof(DBR_GR_ReadUserSuccess));
					wDataSize += sizeof(DBR_GR_ReadUserSuccess);
				}
				if(wDataSize > 0 )
				{
					//投递调度通知
					m_AttemperEvent.PostDataBaseEvent(DBR_GR_READ_AIGAMEUSER_SUCCESS,
						DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
						cbBuffer,wDataSize);
					wDataSize = 0;
				}

				m_AttemperEvent.PostDataBaseEvent(DBR_GR_READ_AIUSER_FINISH,
					DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
					NULL,0);
				
			}

		

		}

	}
	catch (IADOError * pIADOError)
	{
		//输出错误
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
		GT_TRACE((LPTSTR)pIADOError->GetErrorDescribe());
	}

	//关闭记录集
	m_GameUserDB->CloseRecordset();

	return true;
}
//////////////////////////////////////////////////////////////////////////

//帐号存储过程
LONG CDataBaseSink::SPLogonByAccount(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP)
{
	GT_ASSERT(pszAccounts!=NULL);
	GT_ASSERT(pszPassword!=NULL);


	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GP_LogonByAccount");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@strAccounts"),adParamInput,adVarChar,CountString(pszAccounts),_variant_t(pszAccounts));
	m_GameUserDB->AddParamter(TEXT("@strPassword"),adParamInput,adVarChar,CountString(pszPassword),_variant_t(pszPassword));
	m_GameUserDB->AddParamter(TEXT("@nClientIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwClientIP));
	m_GameUserDB->AddParamter(TEXT("@dwLogonServerIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@wLogonServerListenPort"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(0));
	m_GameUserDB->AddParamter(TEXT("@cbLogonPlaza"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)0));
	m_GameUserDB->ExecuteCommand(true);

	return m_GameUserDB->GetReturnValue();
}
//帐号存储过程
LONG CDataBaseSink::SPReadUser(DWORD dwUserID)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GR_ReadUser");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@nUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwUserID));
	m_GameUserDB->ExecuteCommand(true);

	return m_GameUserDB->GetReturnValue();
}
//写游戏局记录
LONG CDataBaseSink::SPWriteGameRound(DBR_GR_WriteGameRound* pWriteGameRound)
{
	//效验参数
	GT_ASSERT(pWriteGameRound!=NULL);


	//执行存储过程
	m_GameScoreDB->ClearAllParameters();
	m_GameScoreDB->SetSPName("GSP_GR_UpdateGameRound");
	m_GameScoreDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
	m_GameScoreDB->AddParamter(TEXT("@ullGameRoundID"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pWriteGameRound->ullGameRoundID));
	m_GameScoreDB->AddParamter(TEXT("@ullStartTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pWriteGameRound->ullStartTime));
	m_GameScoreDB->AddParamter(TEXT("@cbCalculatedFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pWriteGameRound->cbCalculatedFlag));
	m_GameScoreDB->AddParamter(TEXT("@cbValidFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pWriteGameRound->cbValidFlag));
	m_GameScoreDB->AddParamter(TEXT("@wServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pWriteGameRound->wServerID));
	m_GameScoreDB->AddParamter(TEXT("@wTableID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pWriteGameRound->wTableID));
	m_GameScoreDB->AddParamter(TEXT("@cbEndReason"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pWriteGameRound->cbEndReason));
		
	DWORD nParamSize = pWriteGameRound->wEndDataSize;
	BYTE* pParam     = (BYTE*)pWriteGameRound->cbEndData;

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

	m_GameScoreDB->AddParamter(TEXT("@cbEndData"),adParamInput,adVarBinary,nParamSize,_variant_t(varBlob));
	m_GameScoreDB->ExecuteCommand(false);

	SafeArrayDestroy(psa);

	return m_GameScoreDB->GetReturnValue();
}

//写用户报表记录
LONG CDataBaseSink::SPWriteUserChart(DBR_GR_WriteUserChart* pWriteUserChart)
{
	//效验参数
	GT_ASSERT(pWriteUserChart!=NULL);


	//执行存储过程
	m_GameScoreDB->ClearAllParameters();
	m_GameScoreDB->SetSPName("GSP_GR_WriteUserChart");
	m_GameScoreDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
	
	m_GameScoreDB->AddParamter(TEXT("@cbUpdateUserScoreFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(pWriteUserChart->cbUpdateUserScoreFlag));
	
	m_GameScoreDB->AddParamter(TEXT("@ullGameRoundID"),adParamInput,adBigInt,sizeof(ULONGLONG),_variant_t(pWriteUserChart->ullGameRoundID));
	m_GameScoreDB->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pWriteUserChart->dwUserID));
	m_GameScoreDB->AddParamter(TEXT("@wChairID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pWriteUserChart->wChairID));
	
	m_GameScoreDB->AddDecimalParamter(TEXT("@decTotalBetScore"),		     adParamInput,(pWriteUserChart->decTotalBetScore));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decTotalWinScore"),		     adParamInput,(pWriteUserChart->decTotalWinScore));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decTotalTaxScore"),		     adParamInput,(pWriteUserChart->decTotalTaxScore));

	m_GameScoreDB->AddDecimalParamter(TEXT("@decWinScoreOccupancy_High"),	 adParamInput,(pWriteUserChart->decWinScoreOccupancy_High));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decWinScoreOccupancy_Self"),	 adParamInput,(pWriteUserChart->decWinScoreOccupancy_Self));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decWinScoreOccupancy_Less"),	 adParamInput,(pWriteUserChart->decWinScoreOccupancy_Less));

	m_GameScoreDB->AddDecimalParamter(TEXT("@decTaxScoreOccupancy_High"),	 adParamInput,(pWriteUserChart->decTaxScoreOccupancy_High));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decTaxScoreOccupancy_Self"),	 adParamInput,(pWriteUserChart->decTaxScoreOccupancy_Self));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decTaxScoreOccupancy_Less"),	 adParamInput,(pWriteUserChart->decTaxScoreOccupancy_Less));

	m_GameScoreDB->AddDecimalParamter(TEXT("@decValidBetScore_Total"),		 adParamInput,(pWriteUserChart->decValidBetScore_Total));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decValidBetScore_High"),		 adParamInput,(pWriteUserChart->decValidBetScore_High));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decValidBetScore_HighRollback"),adParamInput,(pWriteUserChart->decValidBetScore_HighRollback));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decValidBetScore_Less"),		 adParamInput,(pWriteUserChart->decValidBetScore_Less));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decValidBetScore_LessRollback"),adParamInput,(pWriteUserChart->decValidBetScore_LessRollback));

	m_GameScoreDB->AddDecimalParamter(TEXT("@decPaidScore_High"),			 adParamInput,(pWriteUserChart->decPaidScore_High));
	m_GameScoreDB->AddDecimalParamter(TEXT("@decPaidScore_Less"),			 adParamInput,(pWriteUserChart->decPaidScore_Less));

	DWORD nParamSize = pWriteUserChart->wDetailBetScoreSize;
	BYTE* pParam     = (BYTE*)pWriteUserChart->cbDetailBetScore;

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

	m_GameScoreDB->AddParamter(TEXT("@cbDetailBetScore"),adParamInput,adVarBinary,nParamSize,_variant_t(varBlob));
	m_GameScoreDB->ExecuteCommand(true);

	SafeArrayDestroy(psa);

	return m_GameScoreDB->GetReturnValue();
}

//申请游戏局记录-检测信用额度存储过程
ULONGLONG CDataBaseSink::SPAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScore * pAllocGameRoundAndCheckBetScore)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GR_AllocGameRoundAndCheckBetScore");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adBigInt,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@ullStartTime"),adParamInput,adBigInt,sizeof(ULONGLONG),_variant_t((ULONGLONG)pAllocGameRoundAndCheckBetScore->ullStartTime));
	m_GameUserDB->AddParamter(TEXT("@wServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->wServerID));
	m_GameUserDB->AddParamter(TEXT("@wTableID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pAllocGameRoundAndCheckBetScore->wTableID));
	
	m_GameUserDB->AddParamter(TEXT("@dwUserID0"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[0]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore0"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[0]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID1"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[1]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore1"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[1]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID2"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[2]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore2"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[2]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID3"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[3]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore3"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[3]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID4"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[4]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore4"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[4]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID5"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[5]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore5"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[5]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID6"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[6]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore6"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[6]);
	m_GameUserDB->AddParamter(TEXT("@dwUserID7"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRoundAndCheckBetScore->dwUserID[7]));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore7"),adParamInput,pAllocGameRoundAndCheckBetScore->decBetScore[7]);
	
	m_GameUserDB->ExecuteCommand(true);

	return m_GameUserDB->GetReturnValue();
}
//申请游戏局记录
ULONGLONG CDataBaseSink::SPAllocGameRound(DBR_GR_AllocGameRound * pAllocGameRound)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GR_AllocGameRound");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adBigInt,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@ullStartTime"),adParamInput,adBigInt,sizeof(ULONGLONG),_variant_t((ULONGLONG)pAllocGameRound->ullStartTime));
	m_GameUserDB->AddParamter(TEXT("@wServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pAllocGameRound->wServerID));
	m_GameUserDB->AddParamter(TEXT("@wTableID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pAllocGameRound->wTableID));

	m_GameUserDB->ExecuteCommand(true);

	return m_GameUserDB->GetReturnValue();
}
//复位投注存储过程
LONG CDataBaseSink::SPResetBetScore(DBR_GR_ResetBetScore * pResetBetScore)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GR_ResetBetScore");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adBigInt,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@wServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pResetBetScore->wServerID));
	m_GameUserDB->AddParamter(TEXT("@wTableID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pResetBetScore->wTableID));

	m_GameUserDB->ExecuteCommand(false);

	return m_GameUserDB->GetReturnValue();
}
//插入投注存储过程
LONG CDataBaseSink::SPInsertBetScore(DBR_GR_InsertBetScore * pInsertBetScore)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GR_InsertBetScore");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adBigInt,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pInsertBetScore->dwUserID));
	m_GameUserDB->AddParamter(TEXT("@wServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)pInsertBetScore->wServerID));
	m_GameUserDB->AddParamter(TEXT("@wTableID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(pInsertBetScore->wTableID));
	m_GameUserDB->AddDecimalParamter(TEXT("@decBetScore"),adParamInput,pInsertBetScore->decTotalBetScore);

	m_GameUserDB->ExecuteCommand(false);

	return m_GameUserDB->GetReturnValue();
}
//写入操作记录存储过程
LONG CDataBaseSink::SPInsertOperationInfo(tagOperationData& OperationData)
{
	//执行存储过程
	m_GameUserDB->ClearAllParameters();
	m_GameUserDB->SetSPName("GSP_GP_InsertOperationInfo");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@dwUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwUserID));
	m_GameUserDB->AddParamter(TEXT("@cbUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(OperationData.Header.cbUserType));
	m_GameUserDB->AddParamter(TEXT("@szAccount"),adParamInput,adVarChar,CountString(OperationData.Header.szAccount),_variant_t(OperationData.Header.szAccount));
	m_GameUserDB->AddParamter(TEXT("@dwOperationUserID"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwOperationUserID));
	m_GameUserDB->AddParamter(TEXT("@cbOperationUserType"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(OperationData.Header.cbOperationUserType));
	m_GameUserDB->AddParamter(TEXT("@szOperationAccount"),adParamInput,adVarChar,CountString(OperationData.Header.szOperationAccount),_variant_t(OperationData.Header.szOperationAccount));
	m_GameUserDB->AddParamter(TEXT("@dwOPType"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwOPType));
	m_GameUserDB->AddParamter(TEXT("@fTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)OperationData.Header.fTime));

	m_GameUserDB->AddParamter(TEXT("@dwIPAddress"),adParamInput,adInteger,sizeof(long),_variant_t((long)OperationData.Header.dwIPAddress));

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

	m_GameUserDB->AddParamter(TEXT("@nbContent"),adParamInput,adVarBinary,nParamSize,_variant_t(varBlob));
	m_GameUserDB->ExecuteCommand(true);

	SafeArrayDestroy(psa);

	return m_GameUserDB->GetReturnValue();
}
//读取游戏节点名字存储过程
LONG CDataBaseSink::SPReadGameTreeNodeName(WORD wTypeID,
										   WORD wKindID,
										   WORD wServerID)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();

	m_GameUserDB->SetSPName("GSP_GR_ReadGameTreeNodeName");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adBigInt,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@wTypeID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wTypeID));
	m_GameUserDB->AddParamter(TEXT("@wKindID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wKindID));
	m_GameUserDB->AddParamter(TEXT("@wServerID"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wServerID));

	m_GameUserDB->ExecuteCommand(true);

	
	return m_GameUserDB->GetReturnValue();
}
//读取机器人帐号
LONG CDataBaseSink::SPReadAIUser(WORD wAIUserCount)
{

	//执行存储过程
	m_GameUserDB->ClearAllParameters();

	m_GameUserDB->SetSPName("GSP_GR_ReadAIUser");
	m_GameUserDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adBigInt,4,_variant_t((long)0));
	m_GameUserDB->AddParamter(TEXT("@wAIUserCount"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wAIUserCount));

	m_GameUserDB->ExecuteCommand(true);


	return m_GameUserDB->GetReturnValue();
}
//////////////////////////////////////////////////////////////////////////
bool CDataBaseSink::helper_DBGetFieldValue_UserData(CDataBaseHelper &WorkDataBase, 
													tagUserData& UserData,
													DECIMAL& decOccupancyRate,
													DECIMAL& decTaxOccupancyRate,
													DECIMAL& decRollbackRate,
													DWORD& dwGameRight,
													DWORD* pdwRealUserID,
													BYTE* pcbUserType)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	if(WorkDataBase->IsEndRecordset()==false)
	{
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
		WorkDataBase->GetFieldValue(TEXT("GUExtend_UserRight"),dwGameRight);
		dwGameRight = ((dwGameRight&0xff000000) >> 24) | 
					  ((dwGameRight&0x00ff0000) >> 16) | 
					  ((dwGameRight&0x0000ff00) >> 8) | 
					  ((dwGameRight&0x000000ff)) | 
						(UserData.dwUserRight &0x000000ff);

		WorkDataBase->GetFieldValue(TEXT("GUOccupancyRate"),decOccupancyRate);
		WorkDataBase->GetFieldValue(TEXT("GUTaxOccupancyRate"),decTaxOccupancyRate);
		WorkDataBase->GetFieldValue(TEXT("GURollbackRate"),decRollbackRate);

		if(pdwRealUserID != NULL)
			WorkDataBase->GetFieldValue(TEXT("RealUserID"),*pdwRealUserID);
		if(pcbUserType != NULL)
			WorkDataBase->GetFieldValue(TEXT("UserType"),*pcbUserType);

		//随机头像
		UserData.cbFaceID = rand() % 2;

		return true;
	}
	else
		return false;
}
//读取申请游戏局记录-检测信用额度结果信息
bool CDataBaseSink::helper_DBGetFieldValue_AllocGameRoundAndCheckBetScoreResult(CDataBaseHelper &WorkDataBase, 
																 DBR_GR_AllocGameRoundAndCheckBetScoreResult& AllocGameRoundAndCheckBetScoreResult)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	if(WorkDataBase->IsEndRecordset()==false)
	{
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag0"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[0]);
		WorkDataBase->GetFieldValue(TEXT("UserScore0"),AllocGameRoundAndCheckBetScoreResult.decUserScore[0]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag1"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[1]);
		WorkDataBase->GetFieldValue(TEXT("UserScore1"),AllocGameRoundAndCheckBetScoreResult.decUserScore[1]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag2"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[2]);
		WorkDataBase->GetFieldValue(TEXT("UserScore2"),AllocGameRoundAndCheckBetScoreResult.decUserScore[2]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag3"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[3]);
		WorkDataBase->GetFieldValue(TEXT("UserScore3"),AllocGameRoundAndCheckBetScoreResult.decUserScore[3]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag4"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[4]);
		WorkDataBase->GetFieldValue(TEXT("UserScore4"),AllocGameRoundAndCheckBetScoreResult.decUserScore[4]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag5"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[5]);
		WorkDataBase->GetFieldValue(TEXT("UserScore5"),AllocGameRoundAndCheckBetScoreResult.decUserScore[5]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag6"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[6]);
		WorkDataBase->GetFieldValue(TEXT("UserScore6"),AllocGameRoundAndCheckBetScoreResult.decUserScore[6]);
		WorkDataBase->GetFieldValue(TEXT("CheckScoreFlag7"),AllocGameRoundAndCheckBetScoreResult.bCheckScoreFlag[7]);
		WorkDataBase->GetFieldValue(TEXT("UserScore7"),AllocGameRoundAndCheckBetScoreResult.decUserScore[7]);

		return true;

	}
	else
		return false;
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
//读取游戏节点名字
bool CDataBaseSink::helper_DBGetFieldValue_ReadGameTreeNodeName(CDataBaseHelper &WorkDataBase, 
																TCHAR *szTypeName,
																TCHAR *szKindName,
																TCHAR *szServerName)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	if(WorkDataBase->IsEndRecordset()==false)
	{

		WorkDataBase->GetFieldValue(TEXT("TypeName"),szTypeName, NAME_LEN);
		WorkDataBase->GetFieldValue(TEXT("KindName"),szKindName, NAME_LEN);
		WorkDataBase->GetFieldValue(TEXT("ServerName"),szServerName, NAME_LEN);

		return true;
	}
	else
		return false;
}
//读取信用额度
bool CDataBaseSink::helper_DBGetFieldValue_UserScore(CDataBaseHelper &WorkDataBase, 
													 DBR_GR_UserScore *pUserScore)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	if(WorkDataBase->IsEndRecordset()==false)
	{

		WorkDataBase->GetFieldValue(TEXT("GUMeScore"),pUserScore->decScore);

		return true;
	}
	else
		return false;
}
//读取AI帐号信息
bool CDataBaseSink::helper_DBGetFieldValue_AIUser(CDataBaseHelper &WorkDataBase,
								   Array_DBR_GR_ReadUserSuccess &arrUserAgent,
								   Array_DBR_GR_ReadUserSuccess &arrUserGame)
{
	GT_ASSERT(WorkDataBase.GetInterface());
	while(WorkDataBase->IsEndRecordset()==false)
	{
		//变量定义
		DBR_GR_ReadUserSuccess ReadSuccess;
		memset(&ReadSuccess,0,sizeof(ReadSuccess));
		helper_DBGetFieldValue_UserData(m_GameUserDB, ReadSuccess.UserData,
			ReadSuccess.decOccupancyRate,
			ReadSuccess.decTaxOccupancyRate,
			ReadSuccess.decRollbackRate,ReadSuccess.dwGameRight);
		ReadSuccess.cbUserType = USERTYPE_GAME;
		ReadSuccess.dwRealUserID = ReadSuccess.UserData.dwUserID;

		if(ReadSuccess.UserData.cbLevel == USERLEVEL_GAMEUSER)
		{
			arrUserGame.Add(ReadSuccess);
		}
		else
		{
			arrUserAgent.SetAt(ReadSuccess.UserData.cbLevel, ReadSuccess);
		}

		WorkDataBase->MoveToNext();
		
	}

	return true;
}