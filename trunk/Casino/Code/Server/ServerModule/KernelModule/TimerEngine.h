#ifndef TIMERENGINE_HEAD_FILE
#define TIMERENGINE_HEAD_FILE

#pragma once

//ϵͳͷ�ļ�
#include "KernelModule.h"

//��˵��
class CTimerEngine;

//////////////////////////////////////////////////////////////////////////

//��ʱ���߳�
class CTimerThread : public CServiceThread
{
	//��������
protected:
	DWORD								m_dwTimerSpace;					//ʱ����
	CTimerEngine						* m_pTimerEngine;				//��ʱ������

	//��������
public:
	//���캯��
	CTimerThread(void);
	//��������
	virtual ~CTimerThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(CTimerEngine * pTimerEngine, DWORD dwTimerSpace);

	//���غ���
private:
	//Run����
	virtual bool RepetitionRun();
};

//////////////////////////////////////////////////////////////////////////

//��ʱ������
struct tagTimerItem
{
	WORD								wTimerID;						//��ʱ�� ID
	DWORD								dwElapse;						//��ʱʱ��
	DWORD								dwTimeLeave;					//����ʱ��
	DWORD								dwRepeatTimes;					//�ظ�����
	WPARAM								wBindParam;						//�󶨲���
};

//��ʱ���������鶨��
typedef CArrayTemplate<tagTimerItem *> CTimerItemPtr;

//////////////////////////////////////////////////////////////////////////

//��ʱ������
class CTimerEngine : public ITimerEngine, public ITimerEngineManager
{
	friend class CTimerThread;

	//���ö���
protected:
	DWORD								m_dwTimerSpace;					//ʱ����

	//״̬����
protected:
	bool								m_bService;						//Run��־
	DWORD								m_dwTimePass;					//����ʱ��
	DWORD								m_dwTimeLeave;					//����ʱ��
	CTimerItemPtr						m_TimerItemFree;				//��������
	CTimerItemPtr						m_TimerItemActive;				//�����

	//�������
protected:
	CThreadLock							m_ThreadLock;					//�߳���
	CTimerThread						m_TimerThread;					//��ʱ���߳�
	CQueueServiceEvent					m_AttemperEvent;				//֪ͨ���

	//��������
public:
	//���캯��
	CTimerEngine(void);
	//��������
	virtual ~CTimerEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTimerEngine))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//���ö�ʱ��
	virtual bool __cdecl SetTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wParam);
	//ɾ����ʱ��
	virtual bool __cdecl KillTimer(WORD wTimerID);
	//ɾ����ʱ��
	virtual bool __cdecl KillAllTimer();

	//����ӿ�
public:
	//��ʼ����
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl StopService();
	//���ýӿ�
	virtual bool __cdecl SetTimerEngineSink(IUnknownEx * pIUnknownEx);

	//�ڲ�����
private:
	//��ʱ��֪ͨ
	void OnTimerThreadSink();
};

//////////////////////////////////////////////////////////////////////////

#endif