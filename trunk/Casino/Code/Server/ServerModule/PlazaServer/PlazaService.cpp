#include "StdAfx.h"
#include "PlazaService.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaService::CPlazaService(void)
{
	m_pIEventService=NULL;
	m_pIUpdateEventSink=NULL;
}

//析构函数
CPlazaService::~CPlazaService(void)
{
}
//接口查询
void * __cdecl CPlazaService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseSink,Guid,dwQueryVer);
	return NULL;
}

//模块启动
bool __cdecl CPlazaService::StartService(IUnknownEx * pIUnknownEx)
{

	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].StartService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}

//模块关闭
bool __cdecl CPlazaService::StopService(IUnknownEx * pIUnknownEx)
{

	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].StopService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}

//数据操作处理
bool __cdecl CPlazaService::OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	if(cbThreadIndex < 0 || cbThreadIndex > nDBThreadCount)
		return false;
	else
		return m_DataBaseSink[cbThreadIndex].OnDataBaseRequest(cbThreadIndex, DataBaseEvent, pDataBuffer, wDataSize);
}
//服务状态
bool CPlazaService::IsService()
{
	if (m_ServiceEngine.GetInterface() == NULL) return false;
	return m_ServiceEngine->IsService();
}

//开始服务
bool CPlazaService::StartLogonService()
{
	try
	{
		//变量定义
		bool bSuccess=false;

		//创建服务对象
		if (m_ServiceEngine.GetInterface()==NULL)
		{
			if (m_ServiceEngine.CreateInstance()==false) throw m_ServiceEngine.GetErrorMessage();
			if (m_pIEventService!=NULL) m_ServiceEngine->SetEventService(m_pIEventService);

			//注册调度外挂服务
			IUnknownEx * pIAttemperEngine=GET_OBJECT_INTERFACE(m_AttemperSink,IUnknownEx);
			if (m_ServiceEngine->SetAttemperEngineSink(pIAttemperEngine)==false) throw 0;

			//注册数据库外挂服务
			IUnknownEx * pIDataBaseSink=GET_MYSELF_INTERFACE(IUnknownEx);
			if (m_ServiceEngine->SetDataBaseSink(pIDataBaseSink)==false) throw 0;
		}

		//判断状态
		if (m_ServiceEngine->IsService()==true)
		{
			TraceEventString(TEXT("大厅服务已经启动了"),Level_Warning);
			return true;
		}
		//网络引擎
		ITCPSocketEngine* pITCPSocketEngine=(ITCPSocketEngine *)m_ServiceEngine->GetTCPSocketEngine(IID_ITCPSocketEngine,VER_ITCPSocketEngine);
		if (pITCPSocketEngine==NULL)
		{
			throw TEXT("获取网络引擎服务失败");
		}

		//配置服务
		m_InitParamter.LoadInitParamter();
		for(int i = 0; i < nDBThreadCount; i ++)
		{
			if (m_DataBaseSink[i].InitDataBaseSink(&m_InitParamter,m_ServiceEngine.GetInterface())==false) 
				throw 0;
			if (m_DataBaseSink[i].SetSocketEngine(pITCPSocketEngine)==false) 
				throw 0;
		}
		if (m_AttemperSink.InitAttemperSink(&m_InitParamter,&m_CenterSocket,m_ServiceEngine.GetInterface())==false) throw 0;
		if (m_ServiceEngine->InitServiceEngine(m_InitParamter.m_wListenPort,m_InitParamter.m_wMaxConnect)==false) throw 0;

		//获取调度引擎
		IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)m_ServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
		GT_ASSERT(pIAttemperEngine!=NULL);
		if (pIAttemperEngine==NULL) throw TEXT("调度引擎接口查询失败");

		//获取调度队列
		IQueueService * pIQueueService=(IQueueService *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
		GT_ASSERT(pIQueueService!=NULL);
		if (pIQueueService==NULL) throw TEXT("调度引擎队列服务接口查询失败");

		//获取异步引擎
		IAsynchronismEngine * pIAsynchronismEngine=(IAsynchronismEngine *)m_ServiceEngine->GetAsynchronismEngine(IID_IAsynchronismEngine,VER_IAsynchronismEngine);
		GT_ASSERT(pIAsynchronismEngine!=NULL);
		if (pIAsynchronismEngine==NULL) throw TEXT("异步引擎队务接口查询失败");

		//中心连接
		if (m_CenterSocket.SetQueueService(pIQueueService)==false) throw 0;
		if (m_CenterSocket.SetAsynchronismEngine(pIAsynchronismEngine)==false) throw 0;

		//注册对象
		IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECT_INTERFACE(m_CenterSocket,IAsynchronismEngineSink);
		if (pIAsynchronismEngine->RegisterAsynchronismEngineSink(pIAsynchronismEngineSink)==false) throw TEXT("中心连接异步对象注册失败");

		//启动服务
		if (m_ServiceEngine->StartService(nDBThreadCount)==false) throw 0;

		//复位登陆信息
		ResetLockLogonInfo(inet_addr(m_InitParamter.m_szLogonServerAddr), m_InitParamter.m_wListenPort);

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		TraceEventString(pszErrorMsg,Level_Exception); 
		TraceEventString(TEXT("大厅服务启动失败"),Level_Exception); 
		StopLogonService();
	}
	catch (...)
	{ 
		TraceEventString(TEXT("大厅服务启动失败"),Level_Exception); 
		StopLogonService();
	}

	return false;
}

//停止服务
bool CPlazaService::StopLogonService()
{
	try
	{
		if (m_ServiceEngine.GetInterface()!=NULL) 
		{
			//停止服务
			m_ServiceEngine->StopService();
		
			//获取异步引擎
			IAsynchronismEngine * pIAsynchronismEngine=(IAsynchronismEngine *)m_ServiceEngine->GetAsynchronismEngine(IID_IAsynchronismEngine,VER_IAsynchronismEngine);
			GT_ASSERT(pIAsynchronismEngine!=NULL);
			if (pIAsynchronismEngine==NULL) return false;

			//注销组件
			IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECT_INTERFACE(m_CenterSocket,IAsynchronismEngineSink);
			pIAsynchronismEngine->UnRegisterAsynchronismEngineSink(pIAsynchronismEngineSink);
		
			//复位登陆信息
			ResetLockLogonInfo(inet_addr(m_InitParamter.m_szLogonServerAddr), m_InitParamter.m_wListenPort);

		}

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		TraceEventString(pszErrorMsg,Level_Exception); 
		TraceEventString(TEXT("大厅服务停止失败"),Level_Exception); 
	}
	catch (...)
	{ 
		TraceEventString(TEXT("大厅服务停止失败"),Level_Exception); 
	}

	return false;
}

//设置事件
bool CPlazaService::SetEventService(IUnknownEx * pIEventService)
{
	//获取接口
	GT_ASSERT(pIEventService!=NULL);
	m_pIEventService=(IEventService *)pIEventService->QueryInterface(IID_IEventService,VER_IEventService);
	GT_ASSERT(m_pIEventService!=NULL);

	//设置接口
	m_CenterSocket.SetEventService(pIEventService);
	for(int i = 0; i < nDBThreadCount; i++)
		m_DataBaseSink[i].SetEventService(pIEventService);
	m_AttemperSink.SetEventService(pIEventService);
	if (m_ServiceEngine.GetInterface()) m_ServiceEngine->SetEventService(pIEventService);

	return (m_pIEventService!=NULL);
}
//设置更新事件钩子
bool CPlazaService::SetUpdateEventSink(IUnknownEx * pIUpdateEventSink)
{
	//获取接口
	GT_ASSERT(pIUpdateEventSink!=NULL);
	m_pIUpdateEventSink=(IUpdateEventSink *)pIUpdateEventSink->QueryInterface(IID_IUpdateEventSink,VER_IUpdateEventSink);
	GT_ASSERT(m_pIUpdateEventSink!=NULL);

	//设置接口
	m_AttemperSink.SetUpdateEventSink(pIUpdateEventSink);

	return (m_pIUpdateEventSink!=NULL);
}

//显示错误
void CPlazaService::TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pszString,TraceLevel);
	return;
}
//复位登陆信息
LONG CPlazaService::ResetLockLogonInfo(DWORD dwLogonServerAddr,
									   WORD wListenPort)
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
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szUserDataBaseAddr,
			m_InitParamter.m_szUserDataBasePipeName,
			m_InitParamter.m_wUserDataBasePort,
			m_InitParamter.m_szUserDataBaseName,
			m_InitParamter.m_szUserDataBaseUser,
			m_InitParamter.m_szUserDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}
		
		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_ResetLockLogonInfo"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@dwLogonServerIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwLogonServerAddr));
		ServerInfoDB->AddParamter(TEXT("@wLogonServerListenPort"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wListenPort));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
//枚举用户
void CPlazaService::EnumConnectUser(IUnknownEx * pIUnknownEx)
{
	if(IsService() == false)
		return;
	m_AttemperSink.EnumConnectUser(pIUnknownEx);
}
//发送用户消息
bool  CPlazaService::SendUserMessge(WORD dwRealUserID,BYTE cbUserType,bool bKick, LPCSTR lpcsMsg)
{
	if(IsService() == false || m_AttemperSink.IsConnectedCenter() == false)
		return false;

	//获取调度引擎
	IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)m_ServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
	GT_ASSERT(pIAttemperEngine!=NULL);
	if (pIAttemperEngine==NULL) return false;

	//获取调度队列
	IQueueService * pIQueueService=(IQueueService *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
	GT_ASSERT(pIQueueService!=NULL);
	if (pIQueueService==NULL) return false;

	//构造数据
	MR_EVENT_SendUserMessage  SendUserMessage;
	SendUserMessage.szMessage[0] = 0;
	SendUserMessage.dwRealUserID = dwRealUserID;
	SendUserMessage.cbUserType = cbUserType;
	SendUserMessage.bKick = bKick;
	_tcscpy(SendUserMessage.szMessage, lpcsMsg);

	//投递消息
	pIQueueService->AddToQueue(MR_EVENT_SEND_USER_MESSAGE,&SendUserMessage,sizeof(SendUserMessage));

	return true;
}
//////////////////////////////////////////////////////////////////////////
