#include "StdAfx.h"
#include "Afxinet.h"
#include "DataBaseSink.h"

//构造函数
CDataBaseSink::CDataBaseSink()
{
	m_pInitParamter=NULL;
	m_pIEventService=NULL;
}

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
	GT_ASSERT(m_pInitParamter!=NULL);
	GT_ASSERT(m_pIEventService!=NULL);
	GT_ASSERT(m_AttemperEvent.IsValid()==true);

	//创建实例
	if ((m_CenterDataBase.IsValid()==false)&&(m_CenterDataBase.CreateInstance()==false))
	{
		m_pIEventService->ShowEventNotify(TEXT("中心数据库对象创建失败"),
									Level_Exception);
		return false;
	}

	try
	{
		//连接中心数据库
		m_CenterDataBase->SetConnectionInfo(m_pInitParamter->m_szServerDataBaseAddr,
									m_pInitParamter->m_szServerDataBasePipeName,
									m_pInitParamter->m_wServerDataBasePort,
									m_pInitParamter->m_szServerDataBaseName,
									m_pInitParamter->m_szServerDataBaseUser,
									m_pInitParamter->m_szServerDataBasePass);
		m_CenterDataBase->OpenConnection();

		return true;
	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}

	return false;
}

//调度模块关闭
bool __cdecl CDataBaseSink::StopService(IUnknownEx * pIUnknownEx)
{
	return false;
}

//数据操作处理
bool __cdecl CDataBaseSink::OnDataBaseRequest(BYTE cbThreadIndex,
											  const NTY_DataBaseEvent & DataBaseEvent, 
											  void * pDataBuffer, 
											  WORD wDataSize)
{
	switch(DataBaseEvent.wRequestID) 
	{
	case DBR_GC_READ_GAMEUSERUPDATEDTEMP:
			return OnRequestReadGameUserUpdatedTemp(DataBaseEvent,pDataBuffer,wDataSize);
	case DBR_GC_CLEAR_GAMEUSERUPDATEDTEMP:
			return OnRequestClearGameUserUpdatedTemp(DataBaseEvent,pDataBuffer,wDataSize);
	default:
		break;
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
bool CDataBaseSink::InitDataBaseSink(CInitParamter * pInitParamter, 
									 IUnknownEx * pIUnknownEx)
{
	//效验参数
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(pInitParamter!=NULL);

	//设置变量
	m_pInitParamter=pInitParamter;

	//查询接口
	IServiceEngine * pIServiceEngine=(IServiceEngine *)pIUnknownEx->QueryInterface(IID_IServiceEngine,VER_IServiceEngine);
	GT_ASSERT(pIServiceEngine!=NULL);
	if (pIServiceEngine==NULL) 
	{
		throw TEXT("服务引擎接口查询失败");
	}

	//获取逻辑引擎
	IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)pIServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
	GT_ASSERT(pIAttemperEngine!=NULL);
	if (pIAttemperEngine==NULL) 
	{
		throw TEXT("调度引擎接口查询失败");
	}

	//设置通知组件
	IUnknownEx * pIQueueService=(IUnknownEx *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
	if (m_AttemperEvent.SetQueueService(pIQueueService)==false) 
	{
		throw TEXT("逻辑引擎通知接口设置失败");
	}

	return true;
}

//请求读取用户更新信息临时表
bool CDataBaseSink::OnRequestReadGameUserUpdatedTemp(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//处理
		LONG lReturnValue=-1;
		BYTE cbBuffer[SOCKET_PACKAGE];
		
		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GC_READ_GAMEUSERUPDATEDTEMP:
			{
				lReturnValue=SPReadGameUserUpdatedTemp();
				
				if(lReturnValue == 0)
				{
					WORD wSendSize=0;
					CMD_CS_ServerUserData tmpUserData;
					while(m_CenterDataBase->IsEndRecordset()==false)
					{
						memset(&tmpUserData, 0, sizeof(tmpUserData));
						m_CenterDataBase->GetFieldValue(TEXT("GUUTUserID"),tmpUserData.dwUserID);
						m_CenterDataBase->GetFieldValue(TEXT("GUUTOccupancyRate"),tmpUserData.decOccupancyRate);
						m_CenterDataBase->GetFieldValue(TEXT("GUUTOccupancyRate_NoFlag"),tmpUserData.cbOccupancyRate_NoFlag);
						m_CenterDataBase->GetFieldValue(TEXT("GUUTTaxOccupancyRate"),tmpUserData.decTaxOccupancyRate);
						m_CenterDataBase->GetFieldValue(TEXT("GUUTRollbackRate"),tmpUserData.decRollbackRate);
						m_CenterDataBase->MoveToNext();

						if ((wSendSize+sizeof(CMD_CS_ServerUserData))>sizeof(cbBuffer))
						{
							//投递调度通知
							m_AttemperEvent.PostDataBaseEvent(DBR_GC_UPDATE_SERVERUSERDATA,
								DataBaseEvent.wIndex,
								DataBaseEvent.wRoundID,
								cbBuffer,wSendSize);
							wSendSize=0;
						}
						CopyMemory(cbBuffer+wSendSize,
							&tmpUserData,
							sizeof(CMD_CS_ServerUserData));
						
						wSendSize+=sizeof(CMD_CS_ServerUserData);

					}
					if (wSendSize>0) 
						//投递调度通知
						m_AttemperEvent.PostDataBaseEvent(DBR_GC_UPDATE_SERVERUSERDATA,
						DataBaseEvent.wIndex,DataBaseEvent.wRoundID,
						cbBuffer,wSendSize);
				
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
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}

	//关闭记录集
	m_CenterDataBase->CloseRecordset();

	return true;
}
//请求清除用户更新信息临时表
bool CDataBaseSink::OnRequestClearGameUserUpdatedTemp(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	try
	{
		//处理
		LONG lReturnValue=-1;

		switch (DataBaseEvent.wRequestID)
		{
		case DBR_GC_CLEAR_GAMEUSERUPDATEDTEMP:	
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(DBR_GC_Clear_GameUserUpdatedTemp));
				if (wDataSize!=sizeof(DBR_GC_Clear_GameUserUpdatedTemp)) return false;

				DBR_GC_Clear_GameUserUpdatedTemp * pClearGameUserUpdatedTemp=(DBR_GC_Clear_GameUserUpdatedTemp *)pDataBuffer;

				lReturnValue=SPClearGameUserUpdatedTemp(pClearGameUserUpdatedTemp);

				if(lReturnValue != 0)
				{
					GT_ASSERT(FALSE);
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
		if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}

	//关闭记录集
	m_CenterDataBase->CloseRecordset();

	return true;
}
//读取用户更新信息临时表存储过程
LONG CDataBaseSink::SPReadGameUserUpdatedTemp()
{
	//执行存储过程
	m_CenterDataBase->ClearAllParameters();
	m_CenterDataBase->SetSPName("GSP_GC_ReadGameUserUpdatedTemp");
	m_CenterDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_CenterDataBase->ExecuteCommand(true);

	return m_CenterDataBase->GetReturnValue();
}
//清除用户更新信息临时表存储过程
LONG CDataBaseSink::SPClearGameUserUpdatedTemp(DBR_GC_Clear_GameUserUpdatedTemp * pClearGameUserUpdatedTemp)
{
	//执行存储过程
	m_CenterDataBase->ClearAllParameters();
	m_CenterDataBase->SetSPName("GSP_GC_ClearGameUserUpdatedTemp");
	m_CenterDataBase->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
	m_CenterDataBase->AddParamter(TEXT("@llBeforeTime"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t((LONGLONG)pClearGameUserUpdatedTemp->tmBeforeTime));
	m_CenterDataBase->ExecuteCommand(false);

	return m_CenterDataBase->GetReturnValue();
}