#include "StdAfx.h"
#include "AtlBase.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////

CInitParamter::CInitParamter() 
{ 
	InitParamter();
}

CInitParamter::~CInitParamter()
{
}

//��ʼ��
void CInitParamter::InitParamter()
{

	//��Ϣ���ݿ�
	m_wServerDataBasePort=1433;
	m_szServerDataBaseAddr[0]=0;
	m_szServerDataBaseUser[0]=0;
	m_szServerDataBasePass[0]=0;
	m_szServerDataBasePipeName[0]=0;
	lstrcpyn(m_szServerDataBaseName,
				szServerInfoDB,
				CountArray(m_szServerDataBaseName));

	m_szDatabaseBackupPath[0]=0;
	return ;
}

//��������
void CInitParamter::LoadInitParamter()
{
	//��ʼ������
	InitParamter();

	//��ע���
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_sntprintf(szRegName,
				sizeof(szRegName),
				TEXT("Software\\%s\\CenterServer"),
				szSystemRegKey);
	if (RegParamter.Open(HKEY_CURRENT_USER,szRegName,KEY_READ)!=ERROR_SUCCESS) 
	{
		return ;
	}

	//��������
	TCHAR szReadData[1024]=TEXT("");
	DWORD dwDataType,dwReadData,dwDataSize;
	LONG lErrorCode;

	//���ݿ�˿�
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		m_wServerDataBasePort=(WORD)dwReadData;
	}

	//���ݿ��ַ
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBaseAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS)
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseAddr,sizeof(m_szServerDataBaseAddr));
	}

	//���ݿ�ͨ��
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePipeName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBasePipeName,sizeof(m_szServerDataBasePipeName));
	}

	//���ݿ��û���
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBaseUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseUser,sizeof(m_szServerDataBaseUser));
	}

#ifdef _DEBUG
	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBasePass,sizeof(m_szServerDataBasePass));
	}
#endif

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBaseName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseName,sizeof(m_szServerDataBaseName));
	}

	//����·��
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("DatabaseBackupPath"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szDatabaseBackupPath,sizeof(m_szDatabaseBackupPath));
	}


	return;
}

//��������
void CInitParamter::SaveInitParamter(bool bSaveAll)
{
	//��ע���
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_sntprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\CenterServer"),szSystemRegKey);

	if (RegParamter.Create(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return;

	//��������
	TCHAR szWriteBuffer[1024]=TEXT("");

	//��Ϣ���ݿ�
	RegParamter.SetDWORDValue(TEXT("ServerDataBasePort"),m_wServerDataBasePort);
	CXOREncrypt::EncryptData(m_szServerDataBaseAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerDataBaseAddr"));
	CXOREncrypt::EncryptData(m_szServerDataBaseUser,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerDataBaseUser"));
	CXOREncrypt::EncryptData(m_szServerDataBasePipeName,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerDataBasePipeName"));
	//CXOREncrypt::EncryptData(m_szServerDataBasePass,szWriteBuffer,sizeof(szWriteBuffer));
	//RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerDataBasePass"));
	CXOREncrypt::EncryptData(m_szDatabaseBackupPath,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("DatabaseBackupPath"));

	if (bSaveAll==true)
	{
		//��Ϣ���ݿ�
		CXOREncrypt::EncryptData(m_szServerDataBaseName,szWriteBuffer,sizeof(szWriteBuffer));
		RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerDataBaseName"));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
