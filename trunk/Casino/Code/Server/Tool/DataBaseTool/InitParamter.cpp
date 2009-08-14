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

//初始化
void CInitParamter::InitParamter()
{

	//信息数据库
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

//加载配置
void CInitParamter::LoadInitParamter()
{
	//初始化参数
	InitParamter();

	//打开注册表
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

	//变量定义
	TCHAR szReadData[1024]=TEXT("");
	DWORD dwDataType,dwReadData,dwDataSize;
	LONG lErrorCode;

	//数据库端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		m_wServerDataBasePort=(WORD)dwReadData;
	}

	//数据库地址
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBaseAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS)
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseAddr,sizeof(m_szServerDataBaseAddr));
	}

	//数据库通道
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePipeName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBasePipeName,sizeof(m_szServerDataBasePipeName));
	}

	//数据库用户名
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBaseUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseUser,sizeof(m_szServerDataBaseUser));
	}

#ifdef _DEBUG
	//数据库密码
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBasePass,sizeof(m_szServerDataBasePass));
	}
#endif

	//数据库名字
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBaseName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseName,sizeof(m_szServerDataBaseName));
	}

	//备份路径
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("DatabaseBackupPath"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		CXOREncrypt::CrevasseData(szReadData,m_szDatabaseBackupPath,sizeof(m_szDatabaseBackupPath));
	}


	return;
}

//保存配置
void CInitParamter::SaveInitParamter(bool bSaveAll)
{
	//打开注册表
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_sntprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\CenterServer"),szSystemRegKey);

	if (RegParamter.Create(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return;

	//变量定义
	TCHAR szWriteBuffer[1024]=TEXT("");

	//信息数据库
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
		//信息数据库
		CXOREncrypt::EncryptData(m_szServerDataBaseName,szWriteBuffer,sizeof(szWriteBuffer));
		RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerDataBaseName"));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
