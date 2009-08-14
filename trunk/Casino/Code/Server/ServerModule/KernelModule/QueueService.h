#ifndef QUEUE_SERVICE_HEAD_FILE
#define QUEUE_SERVICE_HEAD_FILE

#include "KernelModule.h"

#pragma once

//////////////////////////////////////////////////////////////////////////

//�����߳���
class CQueueServiceThread : public CServiceThread
{
	//��Ա����
	friend class CQueueService;

	//���ö���
protected:
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	BYTE							m_cbThreadIndex;					//�̴߳�������
	//��������
private:
	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//���ջ���

	//��������
protected:
	//���캯��
	CQueueServiceThread(void);
	//��������
	virtual ~CQueueServiceThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort,BYTE cbThreadIndex);
	//ȡ������
	bool UnInitThread();

	//���غ���
private:
	//Run����
	virtual bool RepetitionRun();
};

//////////////////////////////////////////////////////////////////////////

//���ݶ�����
class CQueueService : public IQueueServiceEngine, public IQueueService 
{
	friend class CQueueServiceThread;

	//��������
protected:
	bool							m_bService;							//�����־
	IQueueServiceSink				* m_pIQueueServiceSink;				//�ص��ӿ�

	//�������
protected:
	CThreadLock						m_ThreadLock;						//�߳���
	CDataStorage					m_DataStorage;						//���ݴ洢
	CQueueServiceThread*			m_QueueServiceThread;				//�����߳�
	HANDLE*							m_hCompletionPort;					//��ɶ˿�
	BYTE							m_cbThreadCount;					//�̴߳�����Ŀ
	//��������
public:
	//���캯��
	CQueueService(void);
	//��������
	virtual ~CQueueService(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CQueueService))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���нӿ�
public:
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize);

	//����ӿ�
public:
	//��ʼ����
	virtual bool __cdecl StartService(BYTE cbThreadCount);
	//ֹͣ����
	virtual bool __cdecl StopService();
	//���ýӿ�
	virtual bool __cdecl SetQueueServiceSink(IUnknownEx * pIUnknownEx);
	//������Ϣ
	virtual bool __cdecl GetBurthenInfo(tagBurthenInfo & BurthenInfo);

	//��������
private:
	//��ȡ����
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//������Ϣ
	void OnQueueServiceThread(BYTE cbThreadIndex, const tagDataHead & DataHead, void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif