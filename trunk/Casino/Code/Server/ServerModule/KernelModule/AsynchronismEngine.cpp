#include "StdAfx.h"
#include "EventService.h"
#include "AsynchronismEngine.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define WM_ASYN_REQUEST					WM_USER+10						//�첽����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	ON_MESSAGE(WM_ASYN_REQUEST,OnAsynRequest)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd()
{
	m_pAsynchronismEngine=NULL;
}

//��������
CControlWnd::~CControlWnd()
{
}

//������Ϣ
HRESULT CControlWnd::OnAsynRequest(WPARAM wParam, LPARAM lParam)
{
	//Ч�����
	GT_ASSERT(m_pAsynchronismEngine!=NULL);

	//��ȡ����
	WORD wRequestID=(WORD)wParam;
	IAsynchronismEngineSink * pIAsynchronismEngineSink=(IAsynchronismEngineSink *)lParam;

	//������
	GT_ASSERT(pIAsynchronismEngineSink!=NULL);
	m_pAsynchronismEngine->OnAsynchronismRequest(wRequestID,pIAsynchronismEngineSink);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CMessageThread::CMessageThread()
{
	m_pAsynchronismEngine=NULL;
}

//��������
CMessageThread::~CMessageThread()
{
}

//Run����
bool CMessageThread::RepetitionRun()
{
	MSG Message;
	if (GetMessage(&Message,NULL,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		return true;
	}
	return false;
}

//ֹͣ�߳�
bool CMessageThread::StopThread(DWORD dwWaitSeconds)
{
	PostThreadMessage(WM_QUIT,0,0);
	return __super::StopThread(dwWaitSeconds);
}

//��ֹ�߳�
bool CMessageThread::TerminateThread(DWORD dwExitCode)
{
	PostThreadMessage(WM_QUIT,0,0);
	return __super::TerminateThread(dwExitCode);
}

//��ʼ�¼�
bool CMessageThread::OnThreadStratEvent()
{
	GT_ASSERT(m_pAsynchronismEngine!=NULL);
	return m_pAsynchronismEngine->OnMessageThreadStart();
}

//ֹͣ�¼�
bool CMessageThread::OnThreadStopEvent()
{
	GT_ASSERT(m_pAsynchronismEngine!=NULL);
	return m_pAsynchronismEngine->OnMessageThreadStop();
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CAsynchronismEngine::CAsynchronismEngine(void)
{
	//���ñ���
	m_bService=false;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	//�������
	m_ControlWnd.m_pAsynchronismEngine=this;
	m_MessageThread.m_pAsynchronismEngine=this;

	return;
}

//��������
CAsynchronismEngine::~CAsynchronismEngine(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CAsynchronismEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAsynchronismEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAsynchronismEngine,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CAsynchronismEngine::StartService()
{
	//�����߳�
	if (m_MessageThread.StartThead()==false) return false;

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CAsynchronismEngine::StopService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�߳�
	m_MessageThread.StopThread();

	//��������
	m_DataStorage.RemoveData(false);

	return true;
}

//��������
bool __cdecl CAsynchronismEngine::InsertRequest(WORD wRequestID, void * const pBuffer, WORD wDataSize, IUnknownEx * pIUnknownEx)
{
	//Ч��״̬
	GT_ASSERT(wDataSize<=sizeof(m_cbBuffer));
	if (m_bService==false) return false;
	if (wDataSize>sizeof(m_cbBuffer)) return false;

	//��ѯ�ӿ�
	IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAsynchronismEngineSink);
	GT_ASSERT(pIAsynchronismEngineSink!=NULL);
	if (pIAsynchronismEngineSink==NULL) return false;

	//��������
	CThreadLockHandle ThreadLockHandle(&m_ThreadLock);
	if (m_DataStorage.AddData(wRequestID,pBuffer,wDataSize)==false) return false;

	//Ͷ����Ϣ
	PostMessage(m_ControlWnd.m_hWnd,WM_ASYN_REQUEST,wRequestID,(LPARAM)pIAsynchronismEngineSink);
	
	return true;
}

//ע�ṳ��
bool __cdecl CAsynchronismEngine::RegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)
{
	//��ȡ�ӿ�
	IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAsynchronismEngineSink);
	GT_ASSERT(pIAsynchronismEngineSink!=NULL);
	if (pIAsynchronismEngineSink==NULL) return false;

	//�����ִ�
	IAsynchronismEngineSink * pIAsynchronismEngineSinkTemp=NULL;
	for (INT_PTR i=0;i<m_AsynchronismEngineSinkArray.GetCount();i++)
	{
		pIAsynchronismEngineSinkTemp=m_AsynchronismEngineSinkArray[0];
		GT_ASSERT(pIAsynchronismEngineSinkTemp!=NULL);
		if (pIAsynchronismEngineSinkTemp==pIAsynchronismEngineSink) return false;
	}

	//ע�ṳ��
	m_AsynchronismEngineSinkArray.Add(pIAsynchronismEngineSink);

	return true;
}

//ȡ��ע��
bool __cdecl CAsynchronismEngine::UnRegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)
{
	//��ȡ�ӿ�
	IAsynchronismEngineSink * pIAsynchronismEngineSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAsynchronismEngineSink);
	GT_ASSERT(pIAsynchronismEngineSink!=NULL);
	if (pIAsynchronismEngineSink==NULL) return false;

	//ɾ������
	IAsynchronismEngineSink * pIAsynchronismEngineSinkTemp=NULL;
	for (INT_PTR i=0;i<m_AsynchronismEngineSinkArray.GetCount();i++)
	{
		pIAsynchronismEngineSinkTemp=m_AsynchronismEngineSinkArray[0];
		GT_ASSERT(pIAsynchronismEngineSinkTemp!=NULL);
		if (pIAsynchronismEngineSinkTemp==pIAsynchronismEngineSink)
		{
			m_AsynchronismEngineSinkArray.RemoveAt(i);
			return true;
		}
	}

	return false;
}

//�߳�����
bool CAsynchronismEngine::OnMessageThreadStart()
{
	//��������
	if (m_ControlWnd.m_hWnd==NULL)
		m_ControlWnd.Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),CWnd::GetDesktopWindow(),10);

	//�¼�֪ͨ
	bool bSuccsee=true;
	IAsynchronismEngineSink * pIAsynchronismEngineSink=NULL;
	for (INT_PTR i=0;i<m_AsynchronismEngineSinkArray.GetCount();i++)
	{
		pIAsynchronismEngineSink=m_AsynchronismEngineSinkArray[0];
		GT_ASSERT(pIAsynchronismEngineSink!=NULL);
		_BEGIN_TRY 
		{ 
			if (pIAsynchronismEngineSink->OnAsynchronismEngineStart()==false) bSuccsee=false;
		}
		CATCH_COMMON_EXCEPTION(bSuccsee=false;)CATCH_UNKNOWN_EXCEPTION(bSuccsee=false;)_END_CATCH

	}

	return bSuccsee;
}

//�߳�ֹͣ
bool CAsynchronismEngine::OnMessageThreadStop()
{
	//�رմ���
	m_ControlWnd.DestroyWindow();

	//�¼�֪ͨ
	bool bSuccsee=true;
	IAsynchronismEngineSink * pIAsynchronismEngineSink=NULL;
	for (INT_PTR i=0;i<m_AsynchronismEngineSinkArray.GetCount();i++)
	{
		pIAsynchronismEngineSink=m_AsynchronismEngineSinkArray[0];
		GT_ASSERT(pIAsynchronismEngineSink!=NULL);
		_BEGIN_TRY	
		{ 
			if (pIAsynchronismEngineSink->OnAsynchronismEngineStop()==false) bSuccsee=false;
		}
		CATCH_COMMON_EXCEPTION(bSuccsee=false;)CATCH_UNKNOWN_EXCEPTION(bSuccsee=false;)_END_CATCH

	}

	return bSuccsee;
}

//�첽����
void CAsynchronismEngine::OnAsynchronismRequest(WORD wRequestID, IAsynchronismEngineSink * pIAsynchronismEngineSink)
{
	//������Դ
	CThreadLockHandle ThreadLockHandle(&m_ThreadLock);

	//��ȡ����
	tagDataHead DataHead;
	m_DataStorage.GetData(DataHead,m_cbBuffer,sizeof(m_cbBuffer));

	//��Ϣ����
	_BEGIN_TRY
	{
		GT_ASSERT(DataHead.wIdentifier==wRequestID);
		pIAsynchronismEngineSink->OnAsynchronismRequest(wRequestID,m_cbBuffer,DataHead.wDataSize);
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateAsynchronismEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CAsynchronismEngine * pAsynchronismEngine=NULL;
	_BEGIN_TRY
	{
		pAsynchronismEngine=new CAsynchronismEngine();
		if (pAsynchronismEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pAsynchronismEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pAsynchronismEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
