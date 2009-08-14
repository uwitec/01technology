#ifndef LOGON_SERVICE_HEAD_FILE
#define LOGON_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CenterSocket.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////
const int nDBThreadCount = 12;

//����������
class CPlazaService : public IDataBaseSink
{
	//��������
protected:
	CCenterSocket					m_CenterSocket;						//��������
	CDataBaseSink					m_DataBaseSink[nDBThreadCount];	    //���ݿ⹳��
	CAttemperEngineSink				m_AttemperSink;						//���ȹ���

	//�������
protected:
	CServiceEngineHelper			m_ServiceEngine;					//��������

	//���ñ���
protected:
	CInitParamter					m_InitParamter;						//���ò���
	IEventService					* m_pIEventService;					//�¼�����
	IUpdateEventSink				* m_pIUpdateEventSink;				//�����¼��ӿ�

	//��������
public:
	//���캯��
	CPlazaService(void);
	//��������
	virtual ~CPlazaService(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid())  return true; else return false;}
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CPlazaService))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//���Ƚӿ�
public:
	//ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//���ܺ���
public:
	//����״̬
	bool IsService();
	//��ʼ����
	bool StartLogonService();
	//ֹͣ����
	bool StopLogonService();
	//�����¼�
	bool SetEventService(IUnknownEx * pIEventService);
	//���ø����¼�����
	bool SetUpdateEventSink(IUnknownEx * pIUpdateEventSink);
	//��λ��½��Ϣ
	LONG ResetLockLogonInfo(DWORD dwLogonServerAddr,
		WORD wListenPort);
	//ö���û�
	void EnumConnectUser(IUnknownEx * pIUnknownEx);
	//�����û���Ϣ
	bool SendUserMessge(WORD dwRealUserID,BYTE cbUserType,bool bKick, LPCSTR lpcsMsg = "");

	//�ڲ�����
private:
	//��ʾ����
	void TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel);
};

//////////////////////////////////////////////////////////////////////////

#endif