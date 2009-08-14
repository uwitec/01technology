#include "StdAfx.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CQueueServiceThread::CQueueServiceThread(void)
{
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
	m_cbThreadIndex = 0;
}

//��������
CQueueServiceThread::~CQueueServiceThread(void)
{
}

//���ú���
bool CQueueServiceThread::InitThread(HANDLE hCompletionPort,BYTE cbThreadIndex)
{
	//Ч�����
	GT_ASSERT(IsRuning()==false);
	GT_ASSERT(m_hCompletionPort==NULL);

	//���ñ���
	m_hCompletionPort=hCompletionPort;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
	m_cbThreadIndex = cbThreadIndex;

	return true;
}

//ȡ������
bool CQueueServiceThread::UnInitThread()
{
	//Ч�����
	GT_ASSERT(IsRuning()==false);

	//���ñ���
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	return true;
}

//Run����
bool CQueueServiceThread::RepetitionRun()
{
	//Ч�����
	GT_ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;
	OVERLAPPED * pOverLapped=NULL;
	CQueueService * pQueueService=NULL;

	//�ȴ���ɶ˿�
	if (GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pQueueService,&pOverLapped,INFINITE))
	{
		//�ж��˳�
		if (pQueueService==NULL) return false;

		//��ȡ����
		tagDataHead DataHead;
		bool bSuccess=pQueueService->GetData(DataHead,m_cbBuffer,sizeof(m_cbBuffer));
		GT_ASSERT(bSuccess==true);

		//��������
		if (bSuccess==true) pQueueService->OnQueueServiceThread(m_cbThreadIndex, DataHead,m_cbBuffer,DataHead.wDataSize);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CQueueService::CQueueService(void)
{
	m_bService=false;
	m_hCompletionPort=NULL;
	m_QueueServiceThread=NULL;
	m_pIQueueServiceSink=NULL;
	m_cbThreadCount=1;
}

//��������
CQueueService::~CQueueService(void)
{
	//ֹͣ����
	StopService();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CQueueService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IQueueService,Guid,dwQueryVer);
	QUERYINTERFACE(IQueueServiceEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IQueueServiceEngine,Guid,dwQueryVer);
	return NULL;
}

//���ýӿ�
bool __cdecl CQueueService::SetQueueServiceSink(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIQueueServiceSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IQueueServiceSink);
	GT_ASSERT(m_pIQueueServiceSink!=NULL);
	return (m_pIQueueServiceSink!=NULL);
}

//������Ϣ
bool __cdecl CQueueService::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetBurthenInfo(BurthenInfo);
}

//��������
bool __cdecl CQueueService::AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)
{
	GT_ASSERT(m_cbThreadCount);
	CThreadLockHandle LockHandle(&m_ThreadLock);
	m_DataStorage.AddData(wIdentifier,pBuffer,wDataSize);
	BYTE n = rand() % m_cbThreadCount;
	PostQueuedCompletionStatus(m_hCompletionPort[n],wDataSize,(ULONG_PTR)this,NULL);
	return true;
}

//��ʼ����
bool __cdecl CQueueService::StartService(BYTE cbThreadCount)
{
	//Ч�����
	GT_ASSERT(m_bService==false);
	GT_ASSERT(m_hCompletionPort==NULL);
	GT_ASSERT(m_pIQueueServiceSink!=NULL);

	m_cbThreadCount = cbThreadCount;

	m_hCompletionPort	 = new HANDLE[m_cbThreadCount];
	memset(m_hCompletionPort, 0, sizeof(HANDLE) * m_cbThreadCount);
	m_QueueServiceThread = new CQueueServiceThread[m_cbThreadCount];

	for(BYTE i = 0; i < m_cbThreadCount; i ++)
	{
		//������ɶ˿�
		m_hCompletionPort[i]=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,1);
		if (m_hCompletionPort[i]==NULL) throw TEXT("���ж�����ɶ˿ڴ���ʧ��");

		//�����߳�
		if (m_QueueServiceThread[i].InitThread(m_hCompletionPort[i], i)==false) throw TEXT("���ж����̳߳�ʼ��ʧ��");
		if (m_QueueServiceThread[i].StartThead()==false) throw TEXT("���ж����߳�����ʧ��");

	}

	//���ò���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CQueueService::StopService()
{
	//���ñ���
	m_bService=false;

	if(m_hCompletionPort)
	{
		//ֹͣ�߳�
		for(BYTE i = 0; i < m_cbThreadCount; i ++)
		{
			if (m_hCompletionPort[i]!=NULL) PostQueuedCompletionStatus(m_hCompletionPort[i],0,NULL,NULL);
			m_QueueServiceThread[i].StopThread();
			m_QueueServiceThread[i].UnInitThread();
		}

		for( i = 0; i < m_cbThreadCount; i ++)
		{
			//�ر���ɶ˿�
			if (m_hCompletionPort[i]!=NULL)
			{
				CloseHandle(m_hCompletionPort[i]);
				m_hCompletionPort[i]=NULL;
			}
		}
		delete[] m_hCompletionPort;
		m_hCompletionPort = NULL;
		delete[] m_QueueServiceThread;
		m_QueueServiceThread = NULL;
	}

	//��������
	m_DataStorage.RemoveData(false);

	return true;
}

//��ȡ����
bool CQueueService::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetData(DataHead,pBuffer,wBufferSize);
}

//������Ϣ
void CQueueService::OnQueueServiceThread(BYTE cbThreadIndex, const tagDataHead & DataHead, void * pBuffer, WORD wDataSize)
{
	GT_ASSERT(m_pIQueueServiceSink!=NULL);
	_BEGIN_TRY	
	{ 
		m_pIQueueServiceSink->OnQueueServiceSink(cbThreadIndex,DataHead.wIdentifier,pBuffer,DataHead.wDataSize,DataHead.dwInsertTime); 
	}
	CATCH_COMMON_EXCEPTION(;)
		CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH
	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateQueueService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CQueueService * pQueueService=NULL;
	_BEGIN_TRY
	{
		pQueueService=new CQueueService();
		if (pQueueService==NULL) throw TEXT("����ʧ��");
		void * pObject=pQueueService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pQueueService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
