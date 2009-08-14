#ifndef ATTEMPER_ENGINE_HEAD_FILE
#define ATTEMPER_ENGINE_HEAD_FILE

#pragma once

//���ͷ�ļ�
#include "KernelModule.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//���ȹ�����
class CAttemperEngine : public IAttemperEngine, public IQueueServiceSink
{
	//���ı���
protected:
	bool							m_bService;							//Run��־
	CQueueService					m_RequestQueueService;				//���ж���

	//�ӿڱ���
protected:
	ITCPSocketEngine				* m_pITCPSocketEngine;				//��������
	IAttemperEngineSink				* m_pIAttemperEngineSink;			//�ҽӽӿ�

	//��������
public:
	//���캯��
	CAttemperEngine(void);
	//��������
	virtual ~CAttemperEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAttemperEngine))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService(BYTE cbThreadCount);
	//ֹͣ����
	virtual bool __cdecl StopService();
	//��������
	virtual bool __cdecl SetSocketEngine(IUnknownEx * pIUnknownEx);
	//ע�ṳ��
	virtual bool __cdecl SetAttemperEngineSink(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual void * __cdecl GetQueueService(const IID & Guid, DWORD dwQueryVer);

	//���нӿ�
public:
	//�����ӿ�
	virtual void __cdecl OnQueueServiceSink(BYTE cbThreadIndex,WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
};

//////////////////////////////////////////////////////////////////////////

#endif