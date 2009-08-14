#ifndef CENTER_SERVICE_HEAD_FILE
#define CENTER_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//���ķ�����
class CCenterService
{
	//��������
protected:
	CDataBaseSink					m_DataBaseSink;						//���ݿ⹳��
	CAttemperEngineSink				m_AttemperSink;						//���ȹ���
	CServiceEngineHelper			m_ServiceEngine;					//��������

	//���ñ���
protected:
	CInitParamter					m_InitParamter;						//���ò���
	IEventService					* m_pIEventService;					//�¼�����

	//��������
public:
	//���캯��
	CCenterService();
	//��������
	virtual ~CCenterService();

	//���ܺ���
public:
	//��ʼ����
	bool StartService();
	//ֹͣ����
	bool StopService();
	//����״̬
	bool IsService();
	//�����¼�
	bool SetEventService(IUnknownEx * pIEventService);

	//�ڲ�����
private:
	//��ʾ����
	void TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel);
};


//////////////////////////////////////////////////////////////////////////

#endif
