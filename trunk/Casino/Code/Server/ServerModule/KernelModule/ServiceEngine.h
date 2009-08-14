#ifndef SERVICE_ENGINE_HEAD_FILE
#define SERVICE_ENGINE_HEAD_FILE

#pragma once

#include "KernelModule.h"
#include "TimerEngine.h"
#include "SocketEngine.h"
#include "AttemperEngine.h"
#include "DataBaseEngine.h"
#include "AsynchronismEngine.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CServiceEngine : public IServiceEngine
{
	//��������
protected:
	bool								m_bService;						//Run��־

	//�������
protected:
	CTimerEngine						m_TimerEngine;					//��ʱ������
	CDataBaseEngine						m_DataBaseEngine;				//���ݿ�����
	CAttemperEngine						m_AttemperEngine;				//��������
	CTCPSocketEngine					m_TCPSocketEngine;				//��������
	CAsynchronismEngine					m_AsynchronismEngine;			//�첽����

	//��������
public:
	//���캯��
	CServiceEngine();
	//��������
	virtual ~CServiceEngine();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CServiceEngine))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService(BYTE cbDBThreadCount = 1,
									BYTE cbAttemperThreadCount = 1);
	//ֹͣ����
	virtual bool __cdecl StopService();

	//���ýӿ�
public:
	//�����¼�
	virtual bool __cdecl SetEventService(IUnknownEx * pIUnknownEx);
	//���ù���
	virtual bool __cdecl SetDataBaseSink(IUnknownEx * pIUnknownEx);
	//���ù���
	virtual bool __cdecl SetAttemperEngineSink(IUnknownEx * pIUnknownEx);
	//ע�ṳ��
	virtual bool __cdecl RegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx);
	//��������
	virtual bool __cdecl InitServiceEngine(WORD wListenPort, WORD wMaxSocketItem);

	//�����ѯ
public:
	//��ȡ��ʱ������ӿ�
	virtual void * __cdecl GetTimerEngine(const IID & Guid, DWORD dwQueryVer);
	//��ȡ���ݿ�����ӿ�
	virtual void * __cdecl GetDataBaseEngine(const IID & Guid, DWORD dwQueryVer);
	//��ȡ��������ӿ�
	virtual void * __cdecl GetAttemperEngine(const IID & Guid, DWORD dwQueryVer);
	//��ȡ��������ӿ�
	virtual void * __cdecl GetTCPSocketEngine(const IID & Guid, DWORD dwQueryVer);
	//��ȡ�첽����ӿ�
	virtual void * __cdecl GetAsynchronismEngine(const IID & Guid, DWORD dwQueryVer);
	//��ȡ���ݿ���нӿ�
	virtual void * __cdecl GetDataBaseQueueService(const IID & Guid, DWORD dwQueryVer);
	//��ȡ���ȶ��нӿ�
	virtual void * __cdecl GetAttemperQueueService(const IID & Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//����״̬
	virtual bool __cdecl IsService() { return m_bService; }
	//�ⲿ����
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif 