#ifndef SERVER_KERNEL_HELPER_HEAD_FILE
#define SERVER_KERNEL_HELPER_HEAD_FILE

#pragma once

#include "KernelModule.h"

//////////////////////////////////////////////////////////////////////////

//�������渨����
class CQueueServiceHelper : public CTempldateHelper<IQueueServiceEngine>
{
	//��������
public:
	//���캯��
	CQueueServiceHelper(void) : CTempldateHelper<IQueueServiceEngine>(IID_IQueueServiceEngine,
		VER_IQueueServiceEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateQueueService")) { }
};

//////////////////////////////////////////////////////////////////////////

//�¼���������
class CEventServiceHelper : public CTempldateHelper<IEventService>
{
	//��������
public:
	//���캯��
	CEventServiceHelper(void) : CTempldateHelper<IEventService>(IID_IEventService,
		VER_IEventService,SERVER_KERNEL_DLL_NAME,TEXT("CreateEventService")) { }
};

//////////////////////////////////////////////////////////////////////////

//�������渨����
class CTCPSocketEngineHelper : public CTempldateHelper<ITCPSocketEngine>
{
	//��������
public:
	//���캯��
	CTCPSocketEngineHelper(void) : CTempldateHelper<ITCPSocketEngine>(IID_ITCPSocketEngine,
		VER_ITCPSocketEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPSocketEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//��ʱ�����渨����
class CTimerEngineHelper : public CTempldateHelper<ITimerEngine>
{
	//��������
public:
	//���캯��
	CTimerEngineHelper(void) : CTempldateHelper<ITimerEngine>(IID_ITimerEngine,
		VER_ITimerEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTimerEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����渨����
class CDataBaseHelper : public CTempldateHelper<IDataBase>
{
	//��������
public:
	//���캯��
	CDataBaseHelper(void) : CTempldateHelper<IDataBase>(IID_IDataBase,
		VER_IDataBase,SERVER_KERNEL_DLL_NAME,TEXT("CreateDataBase")) { }
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����渨����
class CDataBaseEngineHelper : public CTempldateHelper<IDataBaseEngine>
{
	//��������
public:
	//���캯��
	CDataBaseEngineHelper(void) : CTempldateHelper<IDataBaseEngine>(IID_IDataBaseEngine,
		VER_IDataBaseEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateDataBaseEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//�������渨����
class CAttemperEngineHelper : public CTempldateHelper<IAttemperEngine>
{
	//��������
public:
	//���캯��
	CAttemperEngineHelper(void) : CTempldateHelper<IAttemperEngine>(IID_IAttemperEngine,
		VER_IAttemperEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateAttemperEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//�������渨����
class CServiceEngineHelper : public CTempldateHelper<IServiceEngine>
{
	//��������
public:
	//���캯��
	CServiceEngineHelper(void) : CTempldateHelper<IServiceEngine>(IID_IServiceEngine,
		VER_IServiceEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateServiceEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//�첽���渨����
class CAsynchronismEngineHelper : public CTempldateHelper<IAsynchronismEngine>
{
	//��������
public:
	//���캯��
	CAsynchronismEngineHelper(void) : CTempldateHelper<IAsynchronismEngine>(IID_IAsynchronismEngine,
		VER_IAsynchronismEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateAsynchronismEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//�Զ��������������
class CAutoCleanParameters
{
	//��������
private:
	CDataBaseHelper						& m_DataBaseHelper;				//���ݿ����

	//��������
public:
	//���캯��
	CAutoCleanParameters(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
	//��������	
	virtual ~CAutoCleanParameters()
	{
		if (m_DataBaseHelper.IsValid()) m_DataBaseHelper->ClearAllParameters();
	}
};

//�Զ��رռ�¼��������
class CAutoCloseRecord
{
	//��������
private:
	CDataBaseHelper						& m_DataBaseHelper;				//���ݿ����

	//��������
public:
	//���캯��
	CAutoCloseRecord(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
	//��������	
	virtual ~CAutoCloseRecord()
	{
		if (m_DataBaseHelper.IsValid()) m_DataBaseHelper->CloseRecordset();
	}
};

//�Ͽ����ݿ����Ӹ�����
class CAutoCloseDataBase
{
	//��������
private:
	CDataBaseHelper						& m_DataBaseHelper;				//���ݿ����

	//��������
public:
	//���캯��
	CAutoCloseDataBase(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
	//��������	
	virtual ~CAutoCloseDataBase()
	{
		if (m_DataBaseHelper.IsValid()) m_DataBaseHelper->CloseConnection();
	}
};

//////////////////////////////////////////////////////////////////////////

#endif