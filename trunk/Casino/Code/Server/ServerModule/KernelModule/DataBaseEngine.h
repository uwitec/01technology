#ifndef DATA_BASE_ENGINE_HEAD_FILE
#define DATA_BASE_ENGINE_HEAD_FILE

#pragma once

//���ͷ�ļ�
#include "KernelModule.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//ADO ������
class CADOError : public IADOError
{
	//��������
protected:
	enADOErrorType					m_enErrorType;						//�������
	CString							m_strErrorDescribe;					//������Ϣ

	//��������
public:
	//���캯��
	CADOError();
	//��������
	virtual ~CADOError();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CADOError))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//��������
	virtual enADOErrorType __cdecl GetErrorType() { return m_enErrorType; }
	//��������
	virtual LPCTSTR __cdecl GetErrorDescribe() { return m_strErrorDescribe; }

	//���ܺ���
public:
	//���ô���
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����
class CDataBase : public IDataBase
{
	//��Ϣ����
protected:
	CADOError						m_ADOError;							//�������
	CString							m_strConnect;						//�����ַ���
	CString							m_strErrorDescribe;					//������Ϣ

	//״̬����
protected:
	DWORD							m_dwConnectCount;					//���Դ���
	DWORD							m_dwConnectErrorTime;				//����ʱ��
	const DWORD						m_dwResumeConnectCount;				//�ָ�����
	const DWORD						m_dwResumeConnectTime;				//�ָ�ʱ��

	//�ں˱���
protected:
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_DBRecordset;						//��¼������
	_ConnectionPtr					m_DBConnection;						//���ݿ����

	//��������
public:
	//���캯��
	CDataBase();
	//��������
	virtual ~CDataBase();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CDataBase))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//������
	virtual bool __cdecl OpenConnection();
	//�رռ�¼
	virtual bool __cdecl CloseRecordset();
	//�ر�����
	virtual bool __cdecl CloseConnection();
	//��������
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, CComError * pComError);
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szIP,LPCTSTR szPipeName, WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass);

	//״̬�ӿ�
public:
	//�Ƿ����Ӵ���
	virtual bool __cdecl IsConnectError();
	//�Ƿ��
	virtual bool __cdecl IsRecordsetOpened();

	//��¼���ӿ�
public:
	//�����ƶ�
	virtual void __cdecl MoveToNext();
	//�Ƶ���ͷ
	virtual void __cdecl MoveToFirst();
	//�Ƿ����
	virtual bool __cdecl IsEndRecordset();
	//��ȡ��Ŀ
	virtual long __cdecl GetRecordCount();
	//��ȡ��С
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName);
	//�󶨶���
	virtual bool __cdecl BindToRecordset(CADORecordBinding * pBind);
	//��һ��¼��
	virtual bool __cdecl NextRecordset();

	//�ֶνӿ�
public:
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, ULONGLONG & ullValue);
	//��ȡ����
	virtual UINT __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DECIMAL & decValue);

	//�������ӿ�
public:
	//���ô洢����
	virtual void __cdecl SetSPName(LPCTSTR pszSpName);
	//�������
	virtual void __cdecl AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue);
	//�������
	virtual void __cdecl AddDecimalParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, DOUBLE& dbValue, 
		unsigned char Precision = 24, unsigned char NumericScale = 4);
	//�������
	virtual void __cdecl AddDecimalParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, DECIMAL& decValue, 
		unsigned char Precision = 24, unsigned char NumericScale = 4);
	//ɾ������
	virtual void __cdecl ClearAllParameters();
	//��ò���
	virtual void __cdecl GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue);
	//��ȡ������ֵ
	virtual long __cdecl GetReturnValue();

	//ִ�нӿ�
public:
	//ִ�����
	virtual bool __cdecl Execute(LPCTSTR pszCommand);
	//ִ������
	virtual bool __cdecl ExecuteCommand(bool bRecordset);

	//�ڲ�����
private:
	//��ȡ����
	LPCTSTR GetComErrorDescribe(CComError & ComError);
	//���ô���
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);
};

//////////////////////////////////////////////////////////////////////////

//���ݿ������
class CDataBaseEngine : public IDataBaseEngine, public IQueueServiceSink
{
	//�ں˱���
protected:
	bool							m_bService;							//Run��־
	CQueueService					m_RequestQueueService;				//���ж���
	IDataBaseSink					* m_pIDataBaseSink;					//֪ͨ����

	//��������
public:
	//���캯��
	CDataBaseEngine(void);
	//��������
	virtual ~CDataBaseEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CDataBaseEngine))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService(BYTE cbThreadCount);
	//ֹͣ����
	virtual bool __cdecl StopService();
	//ע�ṳ��
	virtual bool __cdecl SetDataBaseSink(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual void * __cdecl GetQueueService(const IID & Guid, DWORD dwQueryVer);

	//���нӿ�
public:
	//���нӿ�
	virtual void __cdecl OnQueueServiceSink(BYTE cbThreadIndex,WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
};

//////////////////////////////////////////////////////////////////////////

#endif