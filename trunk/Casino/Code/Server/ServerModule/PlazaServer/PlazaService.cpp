#include "StdAfx.h"
#include "PlazaService.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaService::CPlazaService(void)
{
	m_pIEventService=NULL;
	m_pIUpdateEventSink=NULL;
}

//��������
CPlazaService::~CPlazaService(void)
{
}
//�ӿڲ�ѯ
void * __cdecl CPlazaService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseSink,Guid,dwQueryVer);
	return NULL;
}

//ģ������
bool __cdecl CPlazaService::StartService(IUnknownEx * pIUnknownEx)
{

	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].StartService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}

//ģ��ر�
bool __cdecl CPlazaService::StopService(IUnknownEx * pIUnknownEx)
{

	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].StopService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}

//���ݲ�������
bool __cdecl CPlazaService::OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	if(cbThreadIndex < 0 || cbThreadIndex > nDBThreadCount)
		return false;
	else
		return m_DataBaseSink[cbThreadIndex].OnDataBaseRequest(cbThreadIndex, DataBaseEvent, pDataBuffer, wDataSize);
}
//����״̬
bool CPlazaService::IsService()
{
	if (m_ServiceEngine.GetInterface() == NULL) return false;
	return m_ServiceEngine->IsService();
}

//��ʼ����
bool CPlazaService::StartLogonService()
{
	try
	{
		//��������
		bool bSuccess=false;

		//�����������
		if (m_ServiceEngine.GetInterface()==NULL)
		{
			if (m_ServiceEngine.CreateInstance()==false) throw m_ServiceEngine.GetErrorMessage();
			if (m_pIEventService!=NULL) m_ServiceEngine->SetEventService(m_pIEventService);

			//ע�������ҷ���
			IUnknownEx * pIAttemperEngine=GET_OBJECT_INTERFACE(m_AttemperSink,IUnknownEx);
			if (m_ServiceEngine->SetAttemperEngineSink(pIAttemperEngine)==false) throw 0;

			//ע�����ݿ���ҷ���
			IUnknownEx * pIDataBaseSink=GET_MYSELF_INTERFACE(IUnknownEx);
			if (m_ServiceEngine->SetDataBaseSink(pIDataBaseSink)==false) throw 0;
		}

		//�ж�״̬
		if (m_ServiceEngine->IsService()==true)
		{
			TraceEventString(TEXT("���������Ѿ�������"),Level_Warning);
			return true;
		}
		//��������
		ITCPSocketEngine* pITCPSocketEngine=(ITCPSocketEngine *)m_ServiceEngine->GetTCPSocketEngine(IID_ITCPSocketEngine,VER_ITCPSocketEngine);
		if (pITCPSocketEngine==NULL)
		{
			throw TEXT("��ȡ�����������ʧ��");
		}

		//���÷���
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

		//��ȡ��������
		IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)m_ServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
		GT_ASSERT(pIAttemperEngine!=NULL);
		if (pIAttemperEngine==NULL) throw TEXT("��������ӿڲ�ѯʧ��");

		//��ȡ���ȶ���
		IQueueService * pIQueueService=(IQueueService *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
		GT_ASSERT(pIQueueService!=NULL);
		if (pIQueueService==NULL) throw TEXT("����������з���ӿڲ�ѯʧ��");

		//��ȡ�첽����
		IAsynchronismEngine * pIAsynchronismEngine=(IAsynchronismEngine *)m_ServiceEngine->GetAsynchronismEngine(IID_IAsynchronismEngine,VER_IAsynchronismEngine);
		GT_ASSERT(pIAsynchronismEngine!=NULL);
		if (pIAsynchronismEngine==NULL) throw TEXT("�첽�������ӿڲ�ѯʧ��");

		//��������
		if (m_CenterSocket.SetQueueService(pIQueueService)==false) throw 0;
		if (m_CenterSocket.SetAsynchronismEngine(pIAsynchronismEngine)==false) throw 0;

		//ע�����
		IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECT_INTERFACE(m_CenterSocket,IAsynchronismEngineSink);
		if (pIAsynchronismEngine->RegisterAsynchronismEngineSink(pIAsynchronismEngineSink)==false) throw TEXT("���������첽����ע��ʧ��");

		//��������
		if (m_ServiceEngine->StartService(nDBThreadCount)==false) throw 0;

		//��λ��½��Ϣ
		ResetLockLogonInfo(inet_addr(m_InitParamter.m_szLogonServerAddr), m_InitParamter.m_wListenPort);

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		TraceEventString(pszErrorMsg,Level_Exception); 
		TraceEventString(TEXT("������������ʧ��"),Level_Exception); 
		StopLogonService();
	}
	catch (...)
	{ 
		TraceEventString(TEXT("������������ʧ��"),Level_Exception); 
		StopLogonService();
	}

	return false;
}

//ֹͣ����
bool CPlazaService::StopLogonService()
{
	try
	{
		if (m_ServiceEngine.GetInterface()!=NULL) 
		{
			//ֹͣ����
			m_ServiceEngine->StopService();
		
			//��ȡ�첽����
			IAsynchronismEngine * pIAsynchronismEngine=(IAsynchronismEngine *)m_ServiceEngine->GetAsynchronismEngine(IID_IAsynchronismEngine,VER_IAsynchronismEngine);
			GT_ASSERT(pIAsynchronismEngine!=NULL);
			if (pIAsynchronismEngine==NULL) return false;

			//ע�����
			IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECT_INTERFACE(m_CenterSocket,IAsynchronismEngineSink);
			pIAsynchronismEngine->UnRegisterAsynchronismEngineSink(pIAsynchronismEngineSink);
		
			//��λ��½��Ϣ
			ResetLockLogonInfo(inet_addr(m_InitParamter.m_szLogonServerAddr), m_InitParamter.m_wListenPort);

		}

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		TraceEventString(pszErrorMsg,Level_Exception); 
		TraceEventString(TEXT("��������ֹͣʧ��"),Level_Exception); 
	}
	catch (...)
	{ 
		TraceEventString(TEXT("��������ֹͣʧ��"),Level_Exception); 
	}

	return false;
}

//�����¼�
bool CPlazaService::SetEventService(IUnknownEx * pIEventService)
{
	//��ȡ�ӿ�
	GT_ASSERT(pIEventService!=NULL);
	m_pIEventService=(IEventService *)pIEventService->QueryInterface(IID_IEventService,VER_IEventService);
	GT_ASSERT(m_pIEventService!=NULL);

	//���ýӿ�
	m_CenterSocket.SetEventService(pIEventService);
	for(int i = 0; i < nDBThreadCount; i++)
		m_DataBaseSink[i].SetEventService(pIEventService);
	m_AttemperSink.SetEventService(pIEventService);
	if (m_ServiceEngine.GetInterface()) m_ServiceEngine->SetEventService(pIEventService);

	return (m_pIEventService!=NULL);
}
//���ø����¼�����
bool CPlazaService::SetUpdateEventSink(IUnknownEx * pIUpdateEventSink)
{
	//��ȡ�ӿ�
	GT_ASSERT(pIUpdateEventSink!=NULL);
	m_pIUpdateEventSink=(IUpdateEventSink *)pIUpdateEventSink->QueryInterface(IID_IUpdateEventSink,VER_IUpdateEventSink);
	GT_ASSERT(m_pIUpdateEventSink!=NULL);

	//���ýӿ�
	m_AttemperSink.SetUpdateEventSink(pIUpdateEventSink);

	return (m_pIUpdateEventSink!=NULL);
}

//��ʾ����
void CPlazaService::TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pszString,TraceLevel);
	return;
}
//��λ��½��Ϣ
LONG CPlazaService::ResetLockLogonInfo(DWORD dwLogonServerAddr,
									   WORD wListenPort)
{
	//�������ݿ�
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("���ݿ��������ʧ�ܣ��޷���ȡ���ݿ���Ϣ"),Level_Exception);
		return -1;
	}
	try
	{
		//�������ݿ�
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szUserDataBaseAddr,
			m_InitParamter.m_szUserDataBasePipeName,
			m_InitParamter.m_wUserDataBasePort,
			m_InitParamter.m_szUserDataBaseName,
			m_InitParamter.m_szUserDataBaseUser,
			m_InitParamter.m_szUserDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("��Ϣ���ݿ�����ʧ�ܣ��޷���ȡ���ݿ���Ϣ"),Level_Exception);
			return -1;
		}
		
		//ִ�в�ѯ
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_ResetLockLogonInfo"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@dwLogonServerIP"),adParamInput,adInteger,sizeof(long),_variant_t((long)dwLogonServerAddr));
		ServerInfoDB->AddParamter(TEXT("@wLogonServerListenPort"),adParamInput,adSmallInt,sizeof(WORD),_variant_t(wListenPort));
		ServerInfoDB->ExecuteCommand(true);

		//����ж�
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
//ö���û�
void CPlazaService::EnumConnectUser(IUnknownEx * pIUnknownEx)
{
	if(IsService() == false)
		return;
	m_AttemperSink.EnumConnectUser(pIUnknownEx);
}
//�����û���Ϣ
bool  CPlazaService::SendUserMessge(WORD dwRealUserID,BYTE cbUserType,bool bKick, LPCSTR lpcsMsg)
{
	if(IsService() == false || m_AttemperSink.IsConnectedCenter() == false)
		return false;

	//��ȡ��������
	IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)m_ServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
	GT_ASSERT(pIAttemperEngine!=NULL);
	if (pIAttemperEngine==NULL) return false;

	//��ȡ���ȶ���
	IQueueService * pIQueueService=(IQueueService *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
	GT_ASSERT(pIQueueService!=NULL);
	if (pIQueueService==NULL) return false;

	//��������
	MR_EVENT_SendUserMessage  SendUserMessage;
	SendUserMessage.szMessage[0] = 0;
	SendUserMessage.dwRealUserID = dwRealUserID;
	SendUserMessage.cbUserType = cbUserType;
	SendUserMessage.bKick = bKick;
	_tcscpy(SendUserMessage.szMessage, lpcsMsg);

	//Ͷ����Ϣ
	pIQueueService->AddToQueue(MR_EVENT_SEND_USER_MESSAGE,&SendUserMessage,sizeof(SendUserMessage));

	return true;
}
//////////////////////////////////////////////////////////////////////////
