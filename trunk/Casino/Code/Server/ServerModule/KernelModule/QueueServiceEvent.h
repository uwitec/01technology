#ifndef QUEUE_SERVICE_EVENT_HEAD_FILE
#define QUEUE_SERVICE_EVENT_HEAD_FILE

#pragma once

#include "KernelModule.h"

//////////////////////////////////////////////////////////////////////////

//���ݶ����¼�
class CQueueServiceEvent
{
	//��������
public:
	CThreadLock						m_BufferLock;						//ͬ������
	IQueueService *					m_pIQueueService;					//���нӿ�
	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//���ջ���

	//��������
public:
	//���캯��
	CQueueServiceEvent() { m_pIQueueService=NULL; }
	//��������
	virtual ~CQueueServiceEvent() {}

	//������
public:
	//���ýӿ�
	bool SetQueueService(IUnknownEx * pIUnknownEx)
	{
		GT_ASSERT(pIUnknownEx!=NULL);
		m_pIQueueService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IQueueService);
		GT_ASSERT(m_pIQueueService!=NULL);
		return (m_pIQueueService!=NULL);
	}
	//��ȡ�ӿ�
	void * GetQueueService(const IID & Guid, DWORD dwQueryVer)
	{
		if (m_pIQueueService==NULL) return NULL;
		return m_pIQueueService->QueryInterface(Guid,dwQueryVer);
	}
	//�Ƿ���Ч
	bool IsValid() { return (m_pIQueueService!=NULL); }

	//֪ͨ����
public:
	//��ʱ���¼�
	bool PostTimerEvent(WORD wTimerID, WPARAM wBindParam)
	{
		//Ч�����
		GT_ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_TimerEvent * pTimerEvent=(NTY_TimerEvent *)m_cbBuffer;
		pTimerEvent->wTimerID=wTimerID;
		pTimerEvent->wBindParam=wBindParam;
		m_pIQueueService->AddToQueue(EVENT_TIMER,m_cbBuffer,sizeof(NTY_TimerEvent));

		return true;
	}
	//���ݿ��¼�
	bool PostDataBaseEvent(WORD wRequestID, WORD wIndex, WORD wRoundID, const void * pDataBuffer, WORD wDataSize)
	{
		//Ч�����
		GT_ASSERT(m_pIQueueService!=NULL);
		GT_ASSERT((wDataSize+sizeof(NTY_DataBaseEvent))<=MAX_QUEUE_PACKET);
		if (m_pIQueueService==NULL) return false;
		if ((wDataSize+sizeof(NTY_DataBaseEvent))>MAX_QUEUE_PACKET) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)m_cbBuffer;
		pDataBaseEvent->wIndex=wIndex;
		pDataBaseEvent->wRequestID=wRequestID;
		pDataBaseEvent->wRoundID=wRoundID;
		if (wDataSize>0)
		{
			GT_ASSERT(pDataBuffer!=NULL);
			CopyMemory(m_cbBuffer+sizeof(NTY_DataBaseEvent),pDataBuffer,wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_DATABASE,m_cbBuffer,sizeof(NTY_DataBaseEvent)+wDataSize);

		return true;
	}
	//����Ӧ���¼�
	bool PostSocketAcceptEvent(WORD wIndex, WORD wRoundID, DWORD dwClientIP)
	{
		//Ч�����
		GT_ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_SocketAcceptEvent * pSocketAcceptEvent=(NTY_SocketAcceptEvent *)m_cbBuffer;
		pSocketAcceptEvent->wIndex=wIndex;
		pSocketAcceptEvent->wRoundID=wRoundID;
		pSocketAcceptEvent->dwClientIP=dwClientIP;
		m_pIQueueService->AddToQueue(EVENT_SOCKET_ACCEPT,m_cbBuffer,sizeof(NTY_SocketAcceptEvent));

		return true;
	}
	//�����ȡ�¼�
	bool PostSocketReadEvent(WORD wIndex, WORD wRoundID, CMD_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//Ч�����
		GT_ASSERT(m_pIQueueService!=NULL);
		GT_ASSERT((wDataSize+sizeof(NTY_SocketReadEvent))<=MAX_QUEUE_PACKET);
		if (m_pIQueueService==NULL) return false;
		if ((wDataSize+sizeof(NTY_SocketReadEvent))>MAX_QUEUE_PACKET) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_SocketReadEvent * pSocketReadEvent=(NTY_SocketReadEvent *)m_cbBuffer;
		pSocketReadEvent->wIndex=wIndex;
		pSocketReadEvent->wRoundID=wRoundID;
		pSocketReadEvent->Command=Command;
		pSocketReadEvent->wDataSize=wDataSize;
		if (wDataSize>0)
		{
			GT_ASSERT(pDataBuffer!=NULL);
			CopyMemory(m_cbBuffer+sizeof(NTY_SocketReadEvent),pDataBuffer,wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_SOCKET_READ,m_cbBuffer,sizeof(NTY_SocketReadEvent)+wDataSize);

		return false;
	}
	//����ر��¼�
	bool PostSocketCloseEvent(WORD wIndex, WORD wRoundID, DWORD dwClientIP, DWORD dwConnectSecond)
	{
		//Ч�����
		GT_ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_SocketCloseEvent * pSocketCloseEvent=(NTY_SocketCloseEvent *)m_cbBuffer;
		pSocketCloseEvent->wIndex=wIndex;
		pSocketCloseEvent->wRoundID=wRoundID;
		pSocketCloseEvent->dwClientIP=dwClientIP;
		pSocketCloseEvent->dwConnectSecond=dwConnectSecond;
		m_pIQueueService->AddToQueue(EVENT_SOCKET_CLOSE,m_cbBuffer,sizeof(NTY_SocketCloseEvent));

		return true;
	}
};

//////////////////////////////////////////////////////////////////////////

#endif