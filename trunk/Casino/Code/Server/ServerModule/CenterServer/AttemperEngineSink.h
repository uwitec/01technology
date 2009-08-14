#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParamter.h"
#include "ConnectInfoManager.h"

//////////////////////////////////////////////////////////////////////////

//�������湳��
class CAttemperEngineSink : public IAttemperEngineSink
{
	//�������
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//���ݿ�֪ͨ
	CConnectInfoManager				m_ConnectInfoManager;				//���ӹ���
	CServerListCenterHelper			m_ServerListCenter;					//�б����

	//�ӿڱ���
protected:
	CInitParamter					* m_pInitParamter;					//���ò���
	ITimerEngine					* m_pITimerEngine;					//��ʱ���ӿ�
	IEventService					* m_pIEventService;					//�¼�����
	ITCPSocketEngine				* m_pITCPSocketEngine;				//����ӿ�

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAttemperEngineSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���Ƚӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//����ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//�¼�����ӿ�
	virtual bool __cdecl OnAttemperEvent(BYTE cbThreadIndex,WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime);

	//���ú��� 
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ú���
	bool InitAttemperSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(BYTE cbThreadIndex,WORD wTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(BYTE cbThreadIndex,void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//����Ӧ���¼�
	virtual bool __cdecl OnEventSocketAccept(BYTE cbThreadIndex,NTY_SocketAcceptEvent * pSocketAcceptEvent);
	//�����ȡ�¼�
	virtual bool __cdecl OnEventSocketRead(BYTE cbThreadIndex,CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//����ر��¼�
	virtual bool __cdecl OnEventSocketClose(BYTE cbThreadIndex,NTY_SocketCloseEvent * pSocketCloseEvent);

	//���纯��
private:
	//�б���Ϣ����
	bool OnSocketMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//������Ϣ����
	bool OnSocketMainServerManager(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//������Ϣ����
	bool OnSocketMainServerRequest(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//������Ϣ����
	bool OnSocketMainManagement(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);

	//��������
private:
	//��ȡGameService��Ϣ
	int ReadGameServerOption(WORD wServerID, tagGameServiceOption *pOption,
		int& ProcessType);
	//��ȡ��Ϸģ������
	TCHAR* GetGameModuleNameByGameProcess(WORD wProcessType);

	//���ݿ⺯��
private:
	//�����û���Ϣ
	bool OnDBUpdateServerUserData(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);

};

//////////////////////////////////////////////////////////////////////////

#endif