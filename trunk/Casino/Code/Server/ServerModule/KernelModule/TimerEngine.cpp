#include "StdAfx.h"
#include "TimerEngine.h"
#include "EventService.h"

//�궨��
#define NO_TIME_LEFT						DWORD(-1)				//����Ӧʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerThread::CTimerThread(void)
{
	m_dwTimerSpace=0L;
	m_pTimerEngine=NULL;
}

//��������
CTimerThread::~CTimerThread(void)
{
}

//���ú���
bool CTimerThread::InitThread(CTimerEngine * pTimerEngine, DWORD dwTimerSpace)
{
	//Ч�����
	GT_ASSERT(dwTimerSpace>=10L);
	GT_ASSERT(pTimerEngine!=NULL);
	if (pTimerEngine==NULL) return false;

	//���ñ���
	m_dwTimerSpace=dwTimerSpace;
	m_pTimerEngine=pTimerEngine;

	return true;
}

//Run����
bool CTimerThread::RepetitionRun()
{
	GT_ASSERT(m_pTimerEngine!=NULL);
	Sleep(m_dwTimerSpace);
	m_pTimerEngine->OnTimerThreadSink();
	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerEngine::CTimerEngine(void)
{
	m_bService=false;
	m_dwTimePass=0L;
	m_dwTimerSpace=25L;
	m_dwTimeLeave=NO_TIME_LEFT;
}

//��������
CTimerEngine::~CTimerEngine(void)
{
	INT_PTR i = 0;
	//ֹͣ����
	StopService();

	//�����ڴ�
	tagTimerItem * pTimerItem=NULL;
	for (i=0;i<m_TimerItemFree.GetCount();i++)
	{
		pTimerItem=m_TimerItemFree[i];
		GT_ASSERT(pTimerItem!=NULL);
		SafeDelete(pTimerItem);
	}
	for (i=0;i<m_TimerItemActive.GetCount();i++)
	{
		pTimerItem=m_TimerItemActive[i];
		GT_ASSERT(pTimerItem!=NULL);
		SafeDelete(pTimerItem);
	}
	m_TimerItemFree.RemoveAll();
	m_TimerItemActive.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTimerEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITimerEngine,Guid,dwQueryVer);
	QUERYINTERFACE(ITimerEngineManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITimerEngine,Guid,dwQueryVer);
	return NULL;
}

//���ö�ʱ��
bool __cdecl CTimerEngine::SetTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wParam)
{
	//������Դ
	CThreadLockHandle LockHandle(&m_ThreadLock);

	//Ч�����
	GT_ASSERT(dwRepeat>0L);
	if (dwRepeat==0) return false;
	dwElapse=(dwElapse+m_dwTimerSpace-1)/m_dwTimerSpace*m_dwTimerSpace;

	//���Ҷ�ʱ��
	bool bTimerExist=false;
	tagTimerItem * pTimerItem=NULL;
	for (INT_PTR i=0;i<m_TimerItemActive.GetCount();i++)
	{
		pTimerItem=m_TimerItemActive[i];
		GT_ASSERT(pTimerItem!=NULL);
		if (pTimerItem->wTimerID==wTimerID) 
		{
			bTimerExist=true;
			break;
		}
	}

	//������ʱ��
	if (bTimerExist==false)
	{
		INT_PTR nFreeCount=m_TimerItemFree.GetCount();
		if (nFreeCount>0)
		{
			pTimerItem=m_TimerItemFree[nFreeCount-1];
			GT_ASSERT(pTimerItem!=NULL);
			m_TimerItemFree.RemoveAt(nFreeCount-1);
		}
		else
		{
			_BEGIN_TRY
			{
				pTimerItem=new tagTimerItem;
				GT_ASSERT(pTimerItem!=NULL);
				if (pTimerItem==NULL) return false;
			}
			CATCH_COMMON_EXCEPTION(return false)CATCH_UNKNOWN_EXCEPTION(return false)_END_CATCH
		}
	}

	//���ò���
	GT_ASSERT(pTimerItem!=NULL);
	pTimerItem->wTimerID=wTimerID;
	pTimerItem->wBindParam=wParam;
	pTimerItem->dwElapse=dwElapse;
	pTimerItem->dwRepeatTimes=dwRepeat;
	pTimerItem->dwTimeLeave=dwElapse+m_dwTimePass;

	//���ʱ��
	m_dwTimeLeave=__min(m_dwTimeLeave,dwElapse);
	if (bTimerExist==false) m_TimerItemActive.Add(pTimerItem);

	return false;
}

//ɾ����ʱ��
bool __cdecl CTimerEngine::KillTimer(WORD wTimerID)
{
	//������Դ
	CThreadLockHandle LockHandle(&m_ThreadLock);

	//���Ҷ�ʱ��
	tagTimerItem * pTimerItem=NULL;
	for (INT_PTR i=0;i<m_TimerItemActive.GetCount();i++)
	{
		pTimerItem=m_TimerItemActive[i];
		GT_ASSERT(pTimerItem!=NULL);
		if (pTimerItem->wTimerID==wTimerID) 
		{
			m_TimerItemActive.RemoveAt(i);
			m_TimerItemFree.Add(pTimerItem);
			if (m_TimerItemActive.GetCount()==0) 
			{
				m_dwTimePass=0L;
				m_dwTimeLeave=NO_TIME_LEFT;
			}
			return true;;
		}
	}

	return false;
}

//ɾ����ʱ��
bool __cdecl CTimerEngine::KillAllTimer()
{
	//������Դ
	CThreadLockHandle LockHandle(&m_ThreadLock);

	//ɾ����ʱ��
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;

	return true;
}

//��ʼ����
bool __cdecl CTimerEngine::StartService()
{
	//Ч��״̬
	if (m_bService==true) 
	{
		CEventTrace::ShowEventNotify(TEXT("��ʱ�������ظ�������������������"),Level_Warning);
		return true;
	}

	//�жϰ�
	if (m_AttemperEvent.IsValid()==false)
	{
		CEventTrace::ShowEventNotify(TEXT("��ʱ������δ�봥�������"),Level_Exception);
		return false;
	}

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;
	if (m_TimerThread.InitThread(this,m_dwTimerSpace)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("��ʱ�������̷߳����ʼ��ʧ��"),Level_Exception);
		return false;
	}

	//��������
	if (m_TimerThread.StartThead()==false)
	{
		CEventTrace::ShowEventNotify(TEXT("��ʱ�������̷߳�������ʧ��"),Level_Exception);
		return false;
	}

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CTimerEngine::StopService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�߳�
	m_TimerThread.StopThread();

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	return true;
}

//���ýӿ�
bool __cdecl CTimerEngine::SetTimerEngineSink(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(m_bService==false);
	if (m_bService==true) return false;
	if (pIUnknownEx==NULL) return false;

	//���ýӿ�
	if (m_AttemperEvent.SetQueueService(pIUnknownEx)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("��ʱ�������봥�������ʧ��"),Level_Exception);
		return false;
	}
	
	return true;
}

//��ʱ��֪ͨ
void CTimerEngine::OnTimerThreadSink()
{
	//������Դ
	CThreadLockHandle LockHandle(&m_ThreadLock);

	//����ʱ��
	if (m_dwTimeLeave==NO_TIME_LEFT) 
	{
		GT_ASSERT(m_TimerItemActive.GetCount()==0);
		return;
	}

	//����ʱ��
	GT_ASSERT(m_dwTimeLeave>=m_dwTimerSpace);
	m_dwTimeLeave-=m_dwTimerSpace;
	m_dwTimePass+=m_dwTimerSpace;

	//��ѯ��ʱ��
	if (m_dwTimeLeave==0)
	{
		bool bKillTimer=false;
		tagTimerItem * pTimerItem=NULL;
		DWORD dwTimeLeave=NO_TIME_LEFT;
		for (INT_PTR i=0;i<m_TimerItemActive.GetCount();)
		{
			//Ч�����
			pTimerItem=m_TimerItemActive[i];
			GT_ASSERT(pTimerItem!=NULL);
			GT_ASSERT(pTimerItem->dwTimeLeave>=m_dwTimePass);

			//��ʱ������
			bKillTimer=false;
			pTimerItem->dwTimeLeave-=m_dwTimePass;
			if (pTimerItem->dwTimeLeave==0L)
			{
				//����֪ͨ
				m_AttemperEvent.PostTimerEvent(pTimerItem->wTimerID,pTimerItem->wBindParam);

				//���ô���
				if (pTimerItem->dwRepeatTimes!=TIMER_REPEAT_TIMER)
				{
					GT_ASSERT(pTimerItem->dwRepeatTimes>0);
					if (pTimerItem->dwRepeatTimes==1L)
					{
						bKillTimer=true;
						m_TimerItemActive.RemoveAt(i);
						m_TimerItemFree.Add(pTimerItem);
					}
					else pTimerItem->dwRepeatTimes--;
				}

				//����ʱ��
				if (bKillTimer==false) pTimerItem->dwTimeLeave=pTimerItem->dwElapse;
			}

			//��������
			if (bKillTimer==false) 
			{
				i++;
				dwTimeLeave=__min(dwTimeLeave,pTimerItem->dwTimeLeave);
				GT_ASSERT(dwTimeLeave%m_dwTimerSpace==0);
			}
		}

		//������Ӧ
		m_dwTimePass=0L;
		m_dwTimeLeave=dwTimeLeave;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateTimerEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTimerEngine * pTimerEngine=NULL;
	_BEGIN_TRY
	{
		pTimerEngine=new CTimerEngine();
		if (pTimerEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pTimerEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pTimerEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
