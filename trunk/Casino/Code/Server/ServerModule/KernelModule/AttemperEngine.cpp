#include "StdAfx.h"
#include "EventService.h"
#include "AttemperEngine.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngine::CAttemperEngine(void)
{
	m_bService=false;
	m_pITCPSocketEngine=NULL;
	m_pIAttemperEngineSink=NULL;
	return;
}

//��������
CAttemperEngine::~CAttemperEngine(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CAttemperEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngine,Guid,dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngine,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CAttemperEngine::StartService(BYTE cbThreadCount)
{

	//�ж�״̬
	if (m_bService==true) 
	{
		CEventTrace::ShowEventNotify(TEXT("���������ظ�������������������"),Level_Warning);
		return true;
	}

	//��ҽӿ�
	if (m_pIAttemperEngineSink==NULL)
	{
		CEventTrace::ShowEventNotify(TEXT("����������ҷ��񲻴���"),Level_Exception);
		return false;
	}

	//���ö���
	if (m_RequestQueueService.SetQueueServiceSink(GET_MYSELF_INTERFACE(IUnknownEx))==false)
	{
		CEventTrace::ShowEventNotify(TEXT("������������з����ʧ��"),Level_Exception);
		return false;
	}

	//�������
	if (m_pIAttemperEngineSink->StartService(GET_MYSELF_INTERFACE(IUnknownEx))==false)
	{
		CEventTrace::ShowEventNotify(TEXT("����������ҷ�������ʧ��"),Level_Exception);
		return false;
	}

	//��������
	if (m_RequestQueueService.StartService(cbThreadCount)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("����������з�������ʧ��"),Level_Exception);
		return false;
	}

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CAttemperEngine::StopService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�������
	m_RequestQueueService.StopService();

	//ֹͣ���
	if (m_pIAttemperEngineSink!=NULL)
	{
		m_pIAttemperEngineSink->StopService(GET_MYSELF_INTERFACE(IUnknownEx));
	}

	return true;
}

//��������
bool __cdecl CAttemperEngine::SetSocketEngine(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pITCPSocketEngine=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITCPSocketEngine);
	ASSERT(m_pITCPSocketEngine!=NULL);
	return (m_pITCPSocketEngine!=NULL);
}

//ע�ṳ��
bool __cdecl CAttemperEngine::SetAttemperEngineSink(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx!=NULL);
	ASSERT(m_pIAttemperEngineSink==NULL);
	if (pIUnknownEx==NULL) return false;
	if (m_pIAttemperEngineSink!=NULL) return false;

	//��ѯ�ӿ�
	m_pIAttemperEngineSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAttemperEngineSink);
	if (m_pIAttemperEngineSink==NULL)
	{
		CEventTrace::ShowEventNotify(TEXT("����������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"),Level_Exception);
		return false;
	}

	return true;
}

//��ȡ�ӿ�
void * __cdecl CAttemperEngine::GetQueueService(const IID & Guid, DWORD dwQueryVer)
{
	return m_RequestQueueService.QueryInterface(Guid,dwQueryVer);
}

//���нӿ�
void __cdecl CAttemperEngine::OnQueueServiceSink(BYTE cbThreadIndex,WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	//�ں��¼�
	ASSERT(m_pIAttemperEngineSink!=NULL);
	switch (wIdentifier)
	{
	case EVENT_TIMER:			//��ʱ���¼�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(NTY_TimerEvent));
			if (wDataSize!=sizeof(NTY_TimerEvent)) return;

			//������Ϣ
			NTY_TimerEvent * pTimerEvent=(NTY_TimerEvent *)pBuffer;
			m_pIAttemperEngineSink->OnEventTimer(cbThreadIndex,pTimerEvent->wTimerID,pTimerEvent->wBindParam);

			return;
		}
	case EVENT_DATABASE:		//���ݿ��¼�
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;

			//������Ϣ
			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
			m_pIAttemperEngineSink->OnEventDataBase(cbThreadIndex,pDataBaseEvent+1,wDataSize-sizeof(NTY_DataBaseEvent),pDataBaseEvent);

			return;
		}
	case EVENT_SOCKET_ACCEPT:	//����Ӧ���¼�
		{
			//Ч���С
			ASSERT(wDataSize==sizeof(NTY_SocketAcceptEvent));
			if (wDataSize!=sizeof(NTY_SocketAcceptEvent)) return;

			//������Ϣ
			NTY_SocketAcceptEvent * pSocketAcceptEvent=(NTY_SocketAcceptEvent *)pBuffer;
			m_pIAttemperEngineSink->OnEventSocketAccept(cbThreadIndex,pSocketAcceptEvent);

			return;
		}
	case EVENT_SOCKET_READ:		//�����ȡ�¼�
		{
			//Ч���С
			NTY_SocketReadEvent * pSocketReadEvent=(NTY_SocketReadEvent *)pBuffer;
			ASSERT(wDataSize>=sizeof(NTY_SocketReadEvent));
			ASSERT(wDataSize==(sizeof(NTY_SocketReadEvent)+pSocketReadEvent->wDataSize));
			if (wDataSize<sizeof(NTY_SocketReadEvent)) return;
			if (wDataSize!=(sizeof(NTY_SocketReadEvent)+pSocketReadEvent->wDataSize)) return;

			//������Ϣ
			bool bSuccess=false;
			_BEGIN_TRY 
			{ 
				bSuccess=m_pIAttemperEngineSink->OnEventSocketRead(cbThreadIndex,pSocketReadEvent->Command,pSocketReadEvent+1,pSocketReadEvent->wDataSize,pSocketReadEvent);
			}
			CATCH_COMMON_EXCEPTION(;)
			CATCH_UNKNOWN_EXCEPTION(;)
			_END_CATCH
			if (bSuccess==false) 
			{
				GT_WARN(TEXT("���������ʧ��"));
				GT_INFO(GT_INFOBASIC, TEXT("Main CMD:%d Sub CMD:%d"), pSocketReadEvent->Command.wMainCmdID, pSocketReadEvent->Command.wSubCmdID);
				m_pITCPSocketEngine->CloseSocket(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID);
			}

			return;
		}
	case EVENT_SOCKET_CLOSE:	//����ر��¼�
		{
			//Ч���С
			ASSERT(wDataSize==sizeof(NTY_SocketCloseEvent));
			if (wDataSize!=sizeof(NTY_SocketCloseEvent)) return;

			//������Ϣ
			NTY_SocketCloseEvent * pSocketCloseEvent=(NTY_SocketCloseEvent *)pBuffer;
			m_pIAttemperEngineSink->OnEventSocketClose(cbThreadIndex,pSocketCloseEvent);

			return;
		}
	}

	//�����¼�
	m_pIAttemperEngineSink->OnAttemperEvent(cbThreadIndex,wIdentifier,pBuffer,wDataSize,dwInsertTime); 

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateAttemperEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CAttemperEngine * pAttemperEngine=NULL;
	_BEGIN_TRY
	{
		pAttemperEngine=new CAttemperEngine();
		if (pAttemperEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pAttemperEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pAttemperEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
