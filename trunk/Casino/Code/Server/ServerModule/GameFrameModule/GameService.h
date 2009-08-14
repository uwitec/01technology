#ifndef GAME_SERVICE_HEAD_FILE
#define GAME_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////
const int nDBThreadCount = 4;
//���ݿ⹳�ӽӿ�
class CDataBaseSinkContainer : public IDataBaseSink
{
	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid())  return true; else return false;}
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CDataBaseSinkContainer))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//���Ƚӿ�
public:
	//���ݿ�ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//���ݿ�ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//���ú��� 
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ú���
	bool InitDataBaseSink(tagDataBaseSinkParameter * pDataBaseSinkParameter, IUnknownEx * pIUnknownEx);

protected:
	CDataBaseSink					m_DataBaseSink[nDBThreadCount];		//���ݿ⹳��
};
//GameService��
class CGameService : public IGameService
{
	//��������
protected:
	CCenterSocket					m_CenterSocket;						//��������
	CAttemperEngineSink				m_AttemperSink;						//���ȹ���
	CServiceEngineHelper			m_ServiceEngine;					//��������
	CDataBaseSinkContainer			m_DataBaseSink;						//���ݿ⹳��
	//���ñ���
protected:
	TCHAR							m_szModuleName[MODULE_LEN];			//ģ������
	tagDataBaseInfo					m_GameUserDBInfo;					//��������
	tagDataBaseInfo					m_GameScoreDBInfo;					//��������
	tagGameServiceAttrib			m_GameServiceAttrib;				//��������
	tagGameServiceOption			m_GameServiceOption;				//��������

	//Interface
protected:
	HINSTANCE						m_hDllInstance;						//��Ϸʵ��
	IEventService					* m_pIEventService;					//�¼�����
	IGameServiceManager				* m_pIGameServiceManager;			//����ӿ�

	//��������
public:
	//���캯��
	CGameService(void);
	//��������
	virtual ~CGameService(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CGameService))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//����״̬
	virtual bool __cdecl IsService();
	//�����¼�
	virtual bool __cdecl SetEventService(IUnknownEx * pIEventService);
	//��ʼ����
	virtual bool __cdecl StartService(tagGameServiceParameter * pGameServiceParameter);
	//ֹͣ����
	virtual bool __cdecl StopService();

	//�ڲ�����
private:
	//�ͷ�ģ��
	bool UnLoadGameServiceModule();
	//����ģ��
	bool LoadGameServiceModule(LPCTSTR pszModuleName);
	//��ʾ����
	void ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel);
};

//////////////////////////////////////////////////////////////////////////

#endif