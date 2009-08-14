#include "StdAfx.h"
#include "Math.h"
#include "EventService.h"
#include "DataBaseEngine.h"
#include "../../CommonDefinition\GlobalService.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
_COM_SMARTPTR_TYPEDEF(IADORecordBinding,__uuidof(IADORecordBinding));

//Ч������
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//���캯��
CADOError::CADOError()
{
	m_enErrorType=ErrorType_Nothing;
}

//��������
CADOError::~CADOError()
{
}

//�ӿڲ�ѯ
void * __cdecl CADOError::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IADOError,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IADOError,Guid,dwQueryVer);
	return NULL;
}

//���ô���
void CADOError::SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe)
{
	//���ô���
	m_enErrorType=enErrorType;
	m_strErrorDescribe=pszDescribe;

	//�׳�����
	throw GET_MYSELF_INTERFACE(IADOError);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBase::CDataBase() : m_dwResumeConnectCount(10L),m_dwResumeConnectTime(10L)
{
	//״̬����
	m_dwConnectCount=0;
	m_dwConnectErrorTime=0L;

	//��������
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//Ч������
	GT_ASSERT(m_DBCommand!=NULL);
	GT_ASSERT(m_DBRecordset!=NULL);
	GT_ASSERT(m_DBConnection!=NULL);
	if (m_DBCommand==NULL) throw TEXT("���ݿ�������󴴽�ʧ��");
	if (m_DBRecordset==NULL) throw TEXT("���ݿ��¼�����󴴽�ʧ��");
	if (m_DBConnection==NULL) throw TEXT("���ݿ����Ӷ��󴴽�ʧ��");

	//���ñ���
	m_DBCommand->CommandType=adCmdStoredProc;

	return;
}

//��������
CDataBase::~CDataBase()
{
	//�ر�����
	CloseConnection();

	//�ͷŶ���
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CDataBase::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBase,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBase,Guid,dwQueryVer);
	return NULL;
}

//������
bool __cdecl CDataBase::OpenConnection()
{
	//�������ݿ�
	try
	{
		//�ر�����
		CloseConnection();

		//�������ݿ�
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect),L"",L"",adConnectUnspecified));
		m_DBConnection->CursorLocation=adUseClient;
		m_DBCommand->ActiveConnection=m_DBConnection;

		//���ñ���
		m_dwConnectCount=0L;
		m_dwConnectErrorTime=0L;

		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//�رռ�¼
bool __cdecl CDataBase::CloseRecordset()
{
	try 
	{
		if (IsRecordsetOpened()) EfficacyResult(m_DBRecordset->Close());
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//�ر�����
bool __cdecl CDataBase::CloseConnection()
{
	try
	{
		CloseRecordset();
		if ((m_DBConnection!=NULL)&&(m_DBConnection->GetState()!=adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��������
bool __cdecl CDataBase::TryConnectAgain(bool bFocusConnect, CComError * pComError)
{
	try
	{
		//�ж�����
		bool bReConnect=bFocusConnect;
		if (bReConnect==false)
		{
			DWORD dwNowTime=(DWORD)time(NULL);
			if ((m_dwConnectErrorTime+m_dwResumeConnectTime)>dwNowTime) bReConnect=true;
		}
		if ((bReConnect==false)&&(m_dwConnectCount>m_dwResumeConnectCount)) bReConnect=true;

		//���ñ���
		m_dwConnectCount++;
		m_dwConnectErrorTime=(DWORD)time(NULL);
		if (bReConnect==false)
		{
			if (pComError!=NULL) SetErrorInfo(ErrorType_Connect,GetComErrorDescribe(*pComError));
			return false;
		}

		//��������
		OpenConnection();
		return true;
	}
	catch (IADOError * pIADOError)
	{
		//�������Ӵ���
		if (pComError!=NULL) SetErrorInfo(ErrorType_Connect,GetComErrorDescribe(*pComError));
		else throw pIADOError;
	}

	return false;
}

//������Ϣ
bool __cdecl CDataBase::SetConnectionInfo(LPCTSTR szIP, LPCTSTR szPipeName,
										  WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass)
{
	//Ч����� 
	GT_ASSERT(szIP!=NULL);
	GT_ASSERT(szData!=NULL);
	GT_ASSERT(szName!=NULL);
	GT_ASSERT(szPass!=NULL);

	//���������ַ���

	//�ж����ݿ�ͨ��
	if(_tcslen(szPipeName)==0)
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;\
User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPass,szName,szData,szIP,wPort);
	else
		m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;\
User ID=%s;Initial Catalog=%s;Data Source=%s\\%s,%ld;"),
								 szPass,szName,szData,szIP,szPipeName,wPort);

	return true;
}

//�Ƿ����Ӵ���
bool __cdecl CDataBase::IsConnectError()
{
	try 
	{
		//״̬�ж�
		if (m_DBConnection==NULL) return true;
		if (m_DBConnection->GetState()==adStateClosed) return true;

		//�����ж�
		long lErrorCount=m_DBConnection->Errors->Count;
		if (lErrorCount>0L)
		{
	        ErrorPtr pError=NULL;
			for(long i=0;i<lErrorCount;i++)
			{
				pError=m_DBConnection->Errors->GetItem(i);
				if (pError->Number==0x80004005) return true;
			}
		}

		return false;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//�Ƿ��
bool __cdecl CDataBase::IsRecordsetOpened()
{
	if (m_DBRecordset==NULL) return false;
	if (m_DBRecordset->GetState()==adStateClosed) return false;
	return true;
}

//�����ƶ�
void __cdecl CDataBase::MoveToNext()
{
	try 
	{ 
		m_DBRecordset->MoveNext(); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//�Ƶ���ͷ
void __cdecl CDataBase::MoveToFirst()
{
	try 
	{ 
		m_DBRecordset->MoveFirst(); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//�Ƿ����
bool __cdecl CDataBase::IsEndRecordset()
{
	try 
	{
		return (m_DBRecordset->EndOfFile==VARIANT_TRUE); 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return true;
}

//��ȡ��Ŀ
long __cdecl CDataBase::GetRecordCount()
{
	try
	{
		if (m_DBRecordset==NULL) return 0;
		return m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return 0;
}

//��ȡ��С
long __cdecl CDataBase::GetActualSize(LPCTSTR pszParamName)
{
	GT_ASSERT(pszParamName!=NULL);
	try 
	{
		return m_DBRecordset->Fields->Item[pszParamName]->ActualSize;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return -1;
}

//�󶨶���
bool __cdecl CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	GT_ASSERT(pBind!=NULL);
	try 
	{
        IADORecordBindingPtr pIBind(m_DBRecordset);
		pIBind->BindToRecordset(pBind);
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}
//��һ��¼��
bool __cdecl CDataBase::NextRecordset()
{
	GT_ASSERT(m_DBRecordset != NULL);
	try 
	{
		m_DBRecordset = m_DBRecordset->NextRecordset(NULL);

		return IsRecordsetOpened();
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}
//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue)
{
	try
	{
		bValue=0;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				bValue=(vtFld.boolVal!=0)?1:0;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				bValue=vtFld.iVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				bValue=0;
				break;
			}
		default: bValue=(BYTE)vtFld.iVal;
		}	
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	try
	{
		ulValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) ulValue=vtFld.lVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{	
	try
	{
		dbValue=0.0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:
			{
				dbValue=vtFld.fltVal;
				break;
			}
		case VT_R8:
			{
				dbValue=vtFld.dblVal;
				break;
			}
		case VT_DECIMAL:
			{
				//dbValue=vtFld.decVal.Lo64;
				//dbValue*=(vtFld.decVal.sign==128)?-1:1;
				//dbValue/=pow((float)10,vtFld.decVal.scale); 
				VarR8FromDec(&vtFld.decVal, &dbValue);
				break;
			}
		case VT_UI1:
			{
				dbValue=vtFld.iVal;
				break;
			}
		case VT_I2:
		case VT_I4:
			{
				dbValue=vtFld.lVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				dbValue=0.0L;
				break;
			}
		default: dbValue=vtFld.dblVal;
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	try
	{
		lValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) lValue=vtFld.lVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	try
	{
		ulValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) ulValue=vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	try
	{
		nValue=0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				nValue = vtFld.boolVal;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				nValue = vtFld.iVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				nValue = 0;
				break;
			}
		default: nValue = vtFld.iVal;
		}	
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	try
	{
		llValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) llValue=vtFld.llVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}
//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, ULONGLONG & ullValue)
{
	try
	{
		ullValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) ullValue=vtFld.llVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}
//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, DECIMAL & decValue)
{	
	try
	{
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:
			{
				VarDecFromR4(vtFld.fltVal,&decValue);
				break;
			}
		case VT_R8:
			{
				VarDecFromR8(vtFld.dblVal,&decValue);
				break;
			}
		case VT_DECIMAL:
			{
				memcpy(&decValue, &vtFld.decVal, sizeof(DECIMAL));
				break;
			}
		case VT_UI1:
			{
				VarDecFromUI1(vtFld.bVal,&decValue);
				break;
			}
		case VT_I2:
		case VT_I4:
			{
				VarDecFromUI4(vtFld.lVal,&decValue);
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				memset(&decValue, 0, sizeof(DECIMAL));
				break;
			}
		default: VarDecFromR8(vtFld.dblVal,&decValue);
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}
inline void KillBlankChar(TCHAR szBuffer[], UINT uSize)
{
	for(int i = uSize-1; i>=0; i --)
	{
		if(szBuffer[i] == 32)
		{
			szBuffer[i] = 0;
		}
		else
			break;
	}
}
//��ȡ����
UINT __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize)
{
	try
	{
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		int DataSize = m_DBRecordset->Fields->GetItem(lpFieldName)->ActualSize;
		if (vtFld.vt==VT_BSTR)
		{
			lstrcpy(szBuffer,(char*)_bstr_t(vtFld));
			KillBlankChar(szBuffer, DataSize > uSize ? uSize : DataSize);
			return true;
		}
		else if(vtFld.vt == (VT_ARRAY|VT_UI1))
		{
			if(DataSize)
			{
				char *buff;
				SafeArrayAccessData(vtFld.parray,(void**)&buff);
				if(DataSize > uSize)
				{
					memcpy(szBuffer, buff, uSize);
				}
				else
				{
					memcpy(szBuffer, buff, DataSize);

				}
				SafeArrayUnaccessData(vtFld.parray);
			}

			return DataSize > uSize ? uSize : DataSize;
		}
		return -1;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return -1;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	try
	{
		wValue=0L;
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY)) wValue=(WORD)vtFld.ulVal;
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
			{
				COleDateTime TempTime(vtFld);
				Time=TempTime;
				break;
			}
		case VT_EMPTY:
		case VT_NULL:
			{
				Time.SetStatus(COleDateTime::null);
				break;
			}
		default: return false;
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ����
bool __cdecl CDataBase::GetFieldValue(LPCTSTR lpFieldName, bool & bValue)
{
	try
	{
		_variant_t vtFld=m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_BOOL:
			{
				bValue=(vtFld.boolVal==0)?false:true;
				break;
			}
		case VT_EMPTY:
		case VT_NULL:
			{
				bValue = false;
				break;
			}
		default:return false;
		}
		return true;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return false;
}

//��ȡ������ֵ
long __cdecl CDataBase::GetReturnValue()
{
	try 
	{
        _ParameterPtr Parameter;
		long lParameterCount=m_DBCommand->Parameters->Count;
		for (long i=0;i<lParameterCount;i++)
		{
			Parameter=m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction==adParamReturnValue) return Parameter->Value.lVal;
		}
		GT_ASSERT(FALSE);
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return 0;
}

//ɾ������
void __cdecl CDataBase::ClearAllParameters()
{
	try 
	{
		long lParameterCount=m_DBCommand->Parameters->Count;
		if (lParameterCount>0L)
		{
			for (long i=lParameterCount;i>0;i--)
			{
				m_DBCommand->Parameters->Delete(i-1);
			}
		}
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//���ô洢����
void __cdecl CDataBase::SetSPName(LPCTSTR pszSpName)
{
	GT_ASSERT(pszSpName!=NULL);
	try 
	{ 
		m_DBCommand->CommandText=pszSpName; 
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//��ò���
void __cdecl CDataBase::GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue)
{
	//Ч�����
	GT_ASSERT(pszParamName!=NULL);

	//��ȡ����
	try 
	{
		vtValue.Clear();
		vtValue=m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}

//�������
void __cdecl CDataBase::AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue)
{
	GT_ASSERT(pszName!=NULL);
	try 
	{
        _ParameterPtr Parameter=m_DBCommand->CreateParameter(pszName,Type,Direction,lSize,vtValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }

	return;
}
//�������
void __cdecl CDataBase::AddDecimalParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, DOUBLE& dbValue, 
										   unsigned char Precision , unsigned char NumericScale)
{
	DECIMAL dec;
	VarDecFromR8(dbValue, &dec);
	AddDecimalParamter(pszName, Direction, dec, Precision, NumericScale);
}
//�������
void __cdecl CDataBase::AddDecimalParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, DECIMAL& decValue, 
										   unsigned char Precision , unsigned char NumericScale)
{
	GT_ASSERT(pszName!=NULL);
	try 
	{
		_ParameterPtr Parameter=m_DBCommand->CreateParameter(pszName,adDecimal,Direction,sizeof(DECIMAL),_variant_t(decValue));
		Parameter->NumericScale = NumericScale;
		Parameter->Precision    = Precision;
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }
}
//ִ�����
bool __cdecl CDataBase::Execute(LPCTSTR pszCommand)
{
	GT_ASSERT(pszCommand!=NULL);
	try
	{
		m_DBConnection->CursorLocation=adUseClient;
		m_DBConnection->Execute(pszCommand,NULL,adExecuteNoRecords);
		return true;
	}
	catch (CComError & ComError) 
	{ 
		if (IsConnectError()==true)	TryConnectAgain(false,&ComError);
		else SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}

	return false;
}

//ִ������
bool __cdecl CDataBase::ExecuteCommand(bool bRecordset)
{
	try 
	{
		//�رռ�¼��
		CloseRecordset();

		//ִ������
		if (bRecordset==true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation=adUseClient;
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand,vtMissing,adOpenForwardOnly,adLockReadOnly,adOptionUnspecified));
		}
		else 
		{
			m_DBConnection->CursorLocation=adUseClient;
			EfficacyResult(m_DBCommand->Execute(NULL,NULL,adExecuteNoRecords));
		}
		return true;
	}
	catch (CComError & ComError) 
	{ 
		if (IsConnectError()==true)	TryConnectAgain(false,&ComError);
		else SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}

	return false;
}

//��ȡ����
LPCTSTR CDataBase::GetComErrorDescribe(CComError & ComError)
{
	_bstr_t bstrDescribe(ComError.Description());
	m_strErrorDescribe.Format(TEXT("ADO ����0x%8x��%s"),ComError.Error(),(LPCTSTR)bstrDescribe);
	return m_strErrorDescribe;
}

//���ô���
void CDataBase::SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe)
{
	m_ADOError.SetErrorInfo(enErrorType,pszDescribe);
	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngine::CDataBaseEngine(void)
{
	//���ñ���
	m_bService=false;
	m_pIDataBaseSink=NULL;

	return;
}

//��������
CDataBaseEngine::~CDataBaseEngine(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CDataBaseEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngine,Guid,dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngine,Guid,dwQueryVer);
	return NULL;
}

//ע��ӿ�
bool __cdecl CDataBaseEngine::SetDataBaseSink(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	GT_ASSERT(pIUnknownEx!=NULL);
	GT_ASSERT(m_pIDataBaseSink==NULL);
	if (pIUnknownEx==NULL) return false;
	if (m_pIDataBaseSink!=NULL) return false;

	//��ѯ�ӿ�
	m_pIDataBaseSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IDataBaseSink);
	if (m_pIDataBaseSink==NULL)
	{
		CEventTrace::ShowEventNotify(TEXT("���ݿ�������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"),Level_Exception);
		return false;
	}

	return true;
}

//��ȡ�ӿ�
void * __cdecl CDataBaseEngine::GetQueueService(const IID & Guid, DWORD dwQueryVer)
{
	return m_RequestQueueService.QueryInterface(Guid,dwQueryVer);
}

//��������
bool __cdecl CDataBaseEngine::StartService(BYTE cbThreadCount)
{
	//�ж�״̬
	if (m_bService==true) 
	{
		CEventTrace::ShowEventNotify(TEXT("���ݿ������ظ�������������������"),Level_Warning);
		return true;
	}

	//��ҽӿ�
	if (m_pIDataBaseSink==NULL)
	{
		CEventTrace::ShowEventNotify(TEXT("���ݿ�������ҷ��񲻴���"),Level_Exception);
		return false;
	}

	//���ö���
	if (m_RequestQueueService.SetQueueServiceSink(GET_MYSELF_INTERFACE(IUnknownEx))==false)
	{
		CEventTrace::ShowEventNotify(TEXT("���ݿ���������з����ʧ��"),Level_Exception);
		return false;
	}

	//�������
	if (m_pIDataBaseSink->StartService(GET_MYSELF_INTERFACE(IUnknownEx))==false)
	{
		CEventTrace::ShowEventNotify(TEXT("���ݿ�������ҷ�������ʧ��"),Level_Exception);
		return false;
	}

	//��������
	if (m_RequestQueueService.StartService(cbThreadCount)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("���ݿ�������з�������ʧ��"),Level_Exception);
		return false;
	}

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CDataBaseEngine::StopService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�������
	m_RequestQueueService.StopService();

	//ֹͣ���
	if (m_pIDataBaseSink!=NULL)
	{
		m_pIDataBaseSink->StopService(GET_MYSELF_INTERFACE(IUnknownEx));
	}

	return true;
}

//���нӿ�
void __cdecl CDataBaseEngine::OnQueueServiceSink(BYTE cbThreadIndex,WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	//�ж�״̬
	if (m_bService==false) return;

	//������
	switch (wIdentifier)
	{
	case EVENT_DATABASE:
		{
			//Ч�����
			GT_ASSERT(pBuffer!=NULL);
			GT_ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;

			//��������
			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
			WORD wHandleBuffer=wDataSize-sizeof(NTY_DataBaseEvent);

			//��������
			GT_ASSERT(m_pIDataBaseSink!=NULL);
			m_pIDataBaseSink->OnDataBaseRequest(cbThreadIndex,*pDataBaseEvent,pDataBaseEvent+1,wHandleBuffer);

			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDataBaseEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDataBaseEngine * pDataBaseEngine=NULL;
	_BEGIN_TRY
	{
		pDataBaseEngine=new CDataBaseEngine();
		if (pDataBaseEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pDataBaseEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pDataBaseEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDataBase(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDataBase * pDataBase=NULL;
	_BEGIN_TRY
	{
		pDataBase=new CDataBase();
		if (pDataBase==NULL) throw TEXT("����ʧ��");
		void * pObject=pDataBase->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pDataBase);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
