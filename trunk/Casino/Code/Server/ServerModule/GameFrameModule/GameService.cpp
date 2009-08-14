#include "StdAfx.h"
#include "GameService.h"

//////////////////////////////////////////////////////////////////////////

void * __cdecl CDataBaseSinkContainer::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseSink,Guid,dwQueryVer);
	return NULL;
}
//���ݿ�ģ������
bool __cdecl CDataBaseSinkContainer::StartService(IUnknownEx * pIUnknownEx)
{
	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].StartService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}
//���ݿ�ģ��ر�
bool __cdecl CDataBaseSinkContainer::StopService(IUnknownEx * pIUnknownEx)
{
	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].StopService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}
//���ݲ�������
bool __cdecl CDataBaseSinkContainer::OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	if(cbThreadIndex < 0 || cbThreadIndex > nDBThreadCount)
	{
		GT_ASSERT(FALSE);
		return false;
	}
	else
		return m_DataBaseSink[cbThreadIndex].OnDataBaseRequest(cbThreadIndex, DataBaseEvent, pDataBuffer, wDataSize);

}
//�����¼�
bool CDataBaseSinkContainer::SetEventService(IUnknownEx * pIUnknownEx)
{
	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].SetEventService(pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}
//���ú���
bool CDataBaseSinkContainer::InitDataBaseSink(tagDataBaseSinkParameter * pDataBaseSinkParameter, IUnknownEx * pIUnknownEx)
{
	for(int i = 0; i < nDBThreadCount; i ++)
	{
		if (m_DataBaseSink[i].InitDataBaseSink(pDataBaseSinkParameter,pIUnknownEx)==false) 
			throw 0;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameService::CGameService()
{
	m_szModuleName[0]=0;
	m_hDllInstance=NULL;
	m_pIEventService=NULL;
	m_pIGameServiceManager=NULL;
	memset(&m_GameUserDBInfo,0,sizeof(m_GameUserDBInfo));
	memset(&m_GameScoreDBInfo,0,sizeof(m_GameScoreDBInfo));
	memset(&m_GameServiceAttrib,0,sizeof(m_GameServiceAttrib));
	memset(&m_GameServiceOption,0,sizeof(m_GameServiceOption));
}

//��������
CGameService::~CGameService()
{
	UnLoadGameServiceModule();
	return;
}

//�ӿڲ�ѯ
void * __cdecl CGameService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameService,Guid,dwQueryVer);
	return NULL;
}

//��ʼ����
bool __cdecl CGameService::StartService(tagGameServiceParameter * pGameServiceParameter)
{
	//Ч�����
	GT_ASSERT(pGameServiceParameter!=NULL);
	if (pGameServiceParameter==NULL) return false;

	try
	{
		//���ñ���
		bool bSuccess=false;
		lstrcpyn(m_szModuleName,pGameServiceParameter->szModuleName,CountArray(m_szModuleName));
		CopyMemory(&m_GameUserDBInfo,&pGameServiceParameter->GameUserDBInfo,sizeof(m_GameUserDBInfo));
		CopyMemory(&m_GameScoreDBInfo,&pGameServiceParameter->GameScoreDBInfo,sizeof(m_GameScoreDBInfo));
		CopyMemory(&m_GameServiceOption,&pGameServiceParameter->GameServiceOption,sizeof(m_GameServiceOption));

		//�����������
		if (m_ServiceEngine.GetInterface()==NULL)
		{
			if (m_ServiceEngine.CreateInstance()==false) throw m_ServiceEngine.GetErrorMessage();
			if (m_pIEventService!=NULL) m_ServiceEngine->SetEventService(m_pIEventService);

			//ע�������ҷ���
			IUnknownEx * pIAttemperEngine=GET_OBJECT_INTERFACE(m_AttemperSink,IUnknownEx);
			if (m_ServiceEngine->SetAttemperEngineSink(pIAttemperEngine)==false) throw 0;

			//ע�����ݿ���ҷ���
			IUnknownEx * pIDataBaseSink=GET_OBJECT_INTERFACE(m_DataBaseSink,IUnknownEx);
			if (m_ServiceEngine->SetDataBaseSink(pIDataBaseSink)==false) throw 0;
		}

		//�ж�״̬
		if (m_ServiceEngine->IsService()==true)
		{
			ShowErrorMessasge(TEXT("GameService�Ѿ���������"),Level_Warning);
			return true;
		}

		//����ģ��
		GT_ASSERT(m_szModuleName[0]!=0);
		if (LoadGameServiceModule(m_szModuleName)==false)
		{
			ShowErrorMessasge(TEXT("GameServiceģ���������ʧ��"),Level_Exception);
			throw 0;
		}

		//��������
		WORD wLessConnect=m_GameServiceOption.wTableCount*m_GameServiceAttrib.wChairCount+50;
		m_GameServiceOption.wMaxConnect=__max(m_GameServiceOption.wMaxConnect,wLessConnect);

		//��������
		if (m_ServiceEngine->InitServiceEngine(m_GameServiceOption.wServerPort,m_GameServiceOption.wMaxConnect)==false) throw 0;

		//�������ݿ�
		tagDataBaseSinkParameter DataBaseSinkParameter;
		memset(&DataBaseSinkParameter,0,sizeof(DataBaseSinkParameter));
		DataBaseSinkParameter.pGameUserDBInfo=&m_GameUserDBInfo;
		DataBaseSinkParameter.pGameScoreDBInfo=&m_GameScoreDBInfo;
		DataBaseSinkParameter.pGameServiceAttrib=&m_GameServiceAttrib;
		DataBaseSinkParameter.pGameServiceOption=&m_GameServiceOption;
		if (m_DataBaseSink.InitDataBaseSink(&DataBaseSinkParameter,m_ServiceEngine.GetInterface())==false)
		{
			ShowErrorMessasge(TEXT("���ݿ�������ҷ�������ʧ��"),Level_Exception);
			throw 0;
		}

		//���õ���
		tagAttemperSinkParameter AttemperSinkParameter;
		memset(&AttemperSinkParameter,0,sizeof(AttemperSinkParameter));
		AttemperSinkParameter.pCenterSocket=&m_CenterSocket;
		AttemperSinkParameter.pGameServiceAttrib=&m_GameServiceAttrib;
		AttemperSinkParameter.pGameServiceOption=&m_GameServiceOption;
		AttemperSinkParameter.pIGameServiceManager=m_pIGameServiceManager;
		AttemperSinkParameter.dwCenterServer=pGameServiceParameter->dwCenterAddr;
		if (m_AttemperSink.InitAttemperSink(&AttemperSinkParameter,m_ServiceEngine.GetInterface())==false)
		{
			ShowErrorMessasge(TEXT("����������ҷ�������ʧ��"),Level_Exception);
			throw 0;
		}

		//��ȡ��������
		IAttemperEngine * pIAttemperEngine=(IAttemperEngine *)m_ServiceEngine->GetAttemperEngine(IID_IAttemperEngine,VER_IAttemperEngine);
		GT_ASSERT(pIAttemperEngine!=NULL);
		if (pIAttemperEngine==NULL)
		{
			ShowErrorMessasge(TEXT("��������ӿڲ�ѯʧ��"),Level_Exception);
			throw 0;
		}

		//��ȡ���ȶ���
		IQueueService * pIQueueService=(IQueueService *)pIAttemperEngine->GetQueueService(IID_IQueueService,VER_IQueueService);
		GT_ASSERT(pIQueueService!=NULL);
		if (pIQueueService==NULL) 
		{
			ShowErrorMessasge(TEXT("����������з���ӿڲ�ѯʧ��"),Level_Exception);
			throw 0;
		}

		//��ȡ�첽����
		IAsynchronismEngine * pIAsynchronismEngine=(IAsynchronismEngine *)m_ServiceEngine->GetAsynchronismEngine(IID_IAsynchronismEngine,VER_IAsynchronismEngine);
		GT_ASSERT(pIAsynchronismEngine!=NULL);
		if (pIAsynchronismEngine==NULL) 
		{
			ShowErrorMessasge(TEXT("�첽�������ӿڲ�ѯʧ��"),Level_Exception);
			throw 0;
		}

		//��������
		if (m_CenterSocket.SetQueueService(pIQueueService)==false) throw 0;
		if (m_CenterSocket.SetAsynchronismEngine(pIAsynchronismEngine)==false) throw 0;

		//ע�����
		IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECT_INTERFACE(m_CenterSocket,IAsynchronismEngineSink);
		if (pIAsynchronismEngine->RegisterAsynchronismEngineSink(pIAsynchronismEngineSink)==false) 
		{
			ShowErrorMessasge(TEXT("���������첽����ע��ʧ��"),Level_Exception);
			throw 0;
		}

		//��������
		if (m_ServiceEngine->StartService(nDBThreadCount, 1)==false) throw 0;

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		ShowErrorMessasge(pszErrorMsg,Level_Exception); 
		ShowErrorMessasge(TEXT("GameService����ʧ��"),Level_Exception); 
		StopService();
	}
	catch (...)
	{ 
		ShowErrorMessasge(TEXT("�����쳣����GameService����ʧ��"),Level_Exception); 
		StopService();
	}

	return false;
}

//ֹͣ����
bool __cdecl CGameService::StopService()
{
	try
	{
		//ֹͣ����
		if (m_ServiceEngine.GetInterface()!=NULL) 
		{
			//ֹͣ����
			m_ServiceEngine->StopService();

			//ע������
			IAsynchronismEngine * pIAsynchronismEngine=(IAsynchronismEngine *)m_ServiceEngine->GetAsynchronismEngine(IID_IAsynchronismEngine,VER_IAsynchronismEngine);
			IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECT_INTERFACE(m_CenterSocket,IAsynchronismEngineSink);
			if (pIAsynchronismEngine!=NULL) pIAsynchronismEngine->UnRegisterAsynchronismEngineSink(pIAsynchronismEngineSink);
		}

		//ж��ģ��
		UnLoadGameServiceModule();

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		ShowErrorMessasge(pszErrorMsg,Level_Exception); 
		ShowErrorMessasge(TEXT("GameServiceֹͣʧ��"),Level_Exception); 
	}
	catch (...)
	{ 
		ShowErrorMessasge(TEXT("GameServiceֹͣʧ��"),Level_Exception); 
	}

	return false;
}

//����״̬
bool CGameService::IsService()
{
	if (m_ServiceEngine.GetInterface()!=NULL==false) return false;
	return m_ServiceEngine->IsService();
}

//�����¼�
bool CGameService::SetEventService(IUnknownEx * pIEventService)
{
	//��ȡ�ӿ�
	GT_ASSERT(pIEventService!=NULL);
	m_pIEventService=(IEventService *)pIEventService->QueryInterface(IID_IEventService,VER_IEventService);
	GT_ASSERT(m_pIEventService!=NULL);

	//���ýӿ�
	m_CenterSocket.SetEventService(pIEventService);
	m_DataBaseSink.SetEventService(pIEventService);
	m_AttemperSink.SetEventService(pIEventService);
	if (m_ServiceEngine.GetInterface()!=NULL) m_ServiceEngine->SetEventService(pIEventService);

	return (m_pIEventService!=NULL);
}

//�ͷ�ģ��
bool CGameService::UnLoadGameServiceModule()
{
	_BEGIN_TRY
	{
		if (m_pIGameServiceManager!=NULL)
		{
			m_pIGameServiceManager->Release();
			m_pIGameServiceManager=NULL;
		}
		if (m_hDllInstance!=NULL)
		{
			AfxFreeLibrary(m_hDllInstance);
			m_hDllInstance=NULL;
		}
		return true;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	return false;
}

//����ģ��
bool CGameService::LoadGameServiceModule(LPCTSTR pszModuleName)
{
	//Ч�����
	GT_ASSERT(pszModuleName!=NULL);
	GT_ASSERT(m_hDllInstance==NULL);

	_BEGIN_TRY
	{
		//����ģ��
		m_hDllInstance=AfxLoadLibrary(pszModuleName);
		if (m_hDllInstance==NULL) 
		{
			TCHAR szDescribe[256]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s��ģ�����ʧ��"),pszModuleName);
			ShowErrorMessasge(szDescribe,Level_Exception);
			throw 0;
		}
		ModuleCreateFunc * CreateFunc=(ModuleCreateFunc *)GetProcAddress(m_hDllInstance,TEXT("CreateGameServiceManager"));
		if (CreateFunc==NULL) 
		{
			TCHAR szDescribe[256]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����������������"),pszModuleName);
			ShowErrorMessasge(szDescribe,Level_Exception);
			throw 0;
		}
		m_pIGameServiceManager=(IGameServiceManager *)CreateFunc(IID_IGameServiceManager,VER_IGameServiceManager);
		if (m_pIGameServiceManager==NULL) 
		{
			TCHAR szDescribe[256]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ����ӿڴ���ʧ��"),pszModuleName);
			ShowErrorMessasge(szDescribe,Level_Exception);
			throw 0;
		}

		//��ȡ����
		m_pIGameServiceManager->GetGameServiceAttrib(m_GameServiceAttrib);

		return true;
	}
	CATCH_COMMON_EXCEPTION(UnLoadGameServiceModule();)CATCH_UNKNOWN_EXCEPTION(UnLoadGameServiceModule();)_END_CATCH

	return false;
}

//��ʾ����
void CGameService::ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pszString,TraceLevel);
	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateGameService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CGameService * pGameService=NULL;
	_BEGIN_TRY
	{
		pGameService=new CGameService();
		if (pGameService==NULL) throw TEXT("����ʧ��");
		void * pObject=pGameService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH


	//�������
	SafeDelete(pGameService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
