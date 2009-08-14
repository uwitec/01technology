#include "StdAfx.h"
#include "EventService.h"
#include "ServiceEngine.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CServiceEngine::CServiceEngine()
{
	m_bService=false;
	return;
}

//��������
CServiceEngine::~CServiceEngine()
{
	//ֹͣ����
	StopService();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CServiceEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServiceEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServiceEngine,Guid,dwQueryVer);
	return NULL;
}

//��ȡ��ʱ���ӿ�
void * __cdecl CServiceEngine::GetTimerEngine(const IID & Guid, DWORD dwQueryVer)
{
	return m_TimerEngine.QueryInterface(Guid,dwQueryVer);
}

//��ȡ��������ӿ�
void * __cdecl CServiceEngine::GetAttemperEngine(const IID & Guid, DWORD dwQueryVer)
{
	return m_AttemperEngine.QueryInterface(Guid,dwQueryVer);
}

//��ȡ��������ӿ�
void * __cdecl CServiceEngine::GetTCPSocketEngine(const IID & Guid, DWORD dwQueryVer)
{
	return m_TCPSocketEngine.QueryInterface(Guid,dwQueryVer);
}

//��ȡ���ݿ�����ӿ�
void * __cdecl CServiceEngine::GetDataBaseEngine(const IID & Guid, DWORD dwQueryVer)
{
	return m_DataBaseEngine.QueryInterface(Guid,dwQueryVer);
}

//��ȡ�첽����ӿ�
void * __cdecl CServiceEngine::GetAsynchronismEngine(const IID & Guid, DWORD dwQueryVer)
{
	return m_AsynchronismEngine.QueryInterface(Guid,dwQueryVer);
}

//��ȡ���ݿ���нӿ�
void * __cdecl CServiceEngine::GetDataBaseQueueService(const IID & Guid, DWORD dwQueryVer)
{
	return m_DataBaseEngine.GetQueueService(Guid,dwQueryVer);
}

//��ȡ���ȶ��нӿ�
void * __cdecl CServiceEngine::GetAttemperQueueService(const IID & Guid, DWORD dwQueryVer)
{
	return m_AttemperEngine.GetQueueService(Guid,dwQueryVer);
}

//�ⲿ����
bool __cdecl CServiceEngine::ControlService(void * pBuffer, WORD wDataSize)
{
	//Ч��״̬
	if (m_bService==false) 
	{
		CEventTrace::ShowEventNotify(TEXT("�������洦��ֹͣ״̬�У����Ʋ�������"),Level_Exception);
		return false;
	}

	//��������
	IQueueService * pIQueueService=(IQueueService *)m_AttemperEngine.GetQueueService(IID_IQueueService,VER_IQueueService);
	if (pIQueueService==NULL) 
	{
		CEventTrace::ShowEventNotify(TEXT("��ȡ�������津���ӿ�ʧ��"),Level_Exception);
		return false;
	}

	//Ͷ��ָ��
	if (pIQueueService->AddToQueue(EVENT_CONTROL,pBuffer,wDataSize)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("Ͷ�ݿ�����Ϣָ��ʧ��"),Level_Exception);
		return false;
	}

	return true;
}

//��������
bool __cdecl CServiceEngine::StartService(BYTE cbDBThreadCount,
										  BYTE cbAttemperThreadCount)
{
	//�ж�״̬
	if (m_bService==true)
	{
		CEventTrace::ShowEventNotify(TEXT("���������ظ�������������������"),Level_Warning);
		return true;
	}

	//�ӿڲ�ѯ
	IUnknownEx * pISocketEngine=GET_OBJECT_INTERFACE(m_TCPSocketEngine,IUnknownEx);
	IUnknownEx * pIAttemperEngine=(IUnknownEx *)m_AttemperEngine.GetQueueService(IID_IUnknownEx,VER_IUnknownEx);

	//�����
	if (m_AttemperEngine.SetSocketEngine(pISocketEngine)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("������������������ʧ��"),Level_Exception);
		return false;
	}
	if (m_TimerEngine.SetTimerEngineSink(pIAttemperEngine)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("��ʱ����������������ʧ��"),Level_Exception);
		return false;
	}
	if (m_TCPSocketEngine.SetSocketEngineSink(pIAttemperEngine)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("������������������ʧ��"),Level_Exception);
		return false;
	}

	//�����������
	if (m_TimerEngine.StartService()==false) return false;
	if (m_DataBaseEngine.StartService(cbDBThreadCount)==false) return false;
	if (m_AsynchronismEngine.StartService()==false) return false;
	if (m_AttemperEngine.StartService(cbAttemperThreadCount)==false) return false;
	if (m_TCPSocketEngine.StartService()==false) return false;

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CServiceEngine::StopService()
{

	if(m_bService == true)
	{
		//ɾ����ʱ��
		m_TimerEngine.KillAllTimer();

		//ֹͣ��������
		m_AttemperEngine.StopService();
		m_DataBaseEngine.StopService();
		m_TimerEngine.StopService();
		m_TCPSocketEngine.StopService();
		m_AsynchronismEngine.StopService();

		//���ñ���
		m_bService=false;
	}

	return true;
}

//�����¼�
bool __cdecl CServiceEngine::SetEventService(IUnknownEx * pIUnknownEx)
{
	return CEventTrace::SetEventService(pIUnknownEx);
}

//ע�ṳ��
bool __cdecl CServiceEngine::SetAttemperEngineSink(IUnknownEx * pIUnknownEx)
{
	return m_AttemperEngine.SetAttemperEngineSink(pIUnknownEx);
}

//ע�ṳ��
bool __cdecl CServiceEngine::SetDataBaseSink(IUnknownEx * pIUnknownEx)
{
	return m_DataBaseEngine.SetDataBaseSink(pIUnknownEx);
}

//ע�ṳ��
bool __cdecl CServiceEngine::RegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)
{
	return m_AsynchronismEngine.RegisterAsynchronismEngineSink(pIUnknownEx);
}

//��������
bool __cdecl CServiceEngine::InitServiceEngine(WORD wListenPort, WORD wMaxSocketItem)
{
	if (m_TCPSocketEngine.SetServicePort(wListenPort)==false) return false;
	if (m_TCPSocketEngine.SetMaxSocketItem(wMaxSocketItem)==false) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateServiceEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CServiceEngine * pServiceEngine=NULL;
	_BEGIN_TRY
	{
		pServiceEngine=new CServiceEngine();
		if (pServiceEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pServiceEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH


	//�������
	SafeDelete(pServiceEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
