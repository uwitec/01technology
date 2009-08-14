#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParamter.h"
//���ݿ������ʶ
#define DBR_GC_READ_GAMEUSERUPDATEDTEMP						1	//��ȡ�û�������Ϣ��ʱ��
#define DBR_GC_CLEAR_GAMEUSERUPDATEDTEMP					2	//����û�������Ϣ��ʱ��
//���ݿ������ʶ
#define DBR_GC_UPDATE_SERVERUSERDATA						10	//�����û���Ϣ

struct DBR_GC_Clear_GameUserUpdatedTemp
{
	__time64_t		tmBeforeTime;
};

//���ݿ����湳��
class CDataBaseSink : public IDataBaseSink
{
	//��������
protected:
	CInitParamter					* m_pInitParamter;					//���ò���
	IEventService					* m_pIEventService;					//�¼�����

	//�������
protected:
	CDataBaseHelper					m_CenterDataBase;					//�������ݿ�
	CQueueServiceEvent				m_AttemperEvent;					//����֪ͨ

	//��������
public:
	//���캯��
	CDataBaseSink();
	//��������
	virtual ~CDataBaseSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CDataBaseSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���Ƚӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//����ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//���ú��� 
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ú���
	bool InitDataBaseSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx);

	//������
protected:
	//�����ȡ�û�������Ϣ��ʱ��
	bool OnRequestReadGameUserUpdatedTemp(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��������û�������Ϣ��ʱ��
	bool OnRequestClearGameUserUpdatedTemp(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//�洢����
protected:
	//��ȡ�û�������Ϣ��ʱ��洢����
	LONG SPReadGameUserUpdatedTemp();
	//����û�������Ϣ��ʱ��洢����
	LONG SPClearGameUserUpdatedTemp(DBR_GC_Clear_GameUserUpdatedTemp * pClearGameUserUpdatedTemp);
};


#endif

