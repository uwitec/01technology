#include "StdAfx.h"
#include "AtlBase.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CInitParamter::CInitParamter() 
{ 
	InitParamter();
}

//析构函数
CInitParamter::~CInitParamter()
{
}

//初始化
void CInitParamter::InitParamter()
{
	m_szAccreditID[0]=0;
	m_szCenterServerAddr[0]=0;
	m_szLoaderServerPass[0]=0;

	m_wGameUserDBPort=1433;
	m_szGameUserDBAddr[0]=0;
	m_szGameUserDBUser[0]=0;
	m_szGameUserDBPass[0]=0;
	lstrcpyn(m_szGameUserDBName,szGameUserDB,CountArray(m_szGameUserDBName));

	m_wServerInfoDBPort=1433;
	m_szServerInfoDBAddr[0]=0;
	m_szServerInfoDBUser[0]=0;
	m_szServerInfoDBPass[0]=0;
	lstrcpyn(m_szServerInfoDBName,szServerInfoDB,CountArray(m_szServerInfoDBName));

	m_wServerID = 0;
	m_wGameNetPort = 11000;
	m_szGameNetAddr[0]=0;

	return;
}

//加载配置
void CInitParamter::LoadInitParamter()
{
	//初始化参数
	InitParamter();

	//打开注册表
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\GameServer"),szSystemRegKey);
	if (RegParamter.Open(HKEY_CURRENT_USER,szRegName,KEY_READ)!=ERROR_SUCCESS) return;

	//变量定义
	TCHAR szReadData[1024]=TEXT("");
	DWORD dwDataType,dwReadData,dwDataSize;

	//CenterServer
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	LONG lErrorCode=RegParamter.QueryValue(TEXT("CenterServerAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szCenterServerAddr,sizeof(m_szCenterServerAddr));

	//授权号码
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("AccreditID"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szAccreditID,sizeof(m_szAccreditID));

	//数据库端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wGameUserDBPort=(WORD)dwReadData;

	//数据库地址
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBAddr,sizeof(m_szGameUserDBAddr));

	//数据库用户名
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBUser,sizeof(m_szGameUserDBUser));

	//数据库密码
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBPass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBPass,sizeof(m_szGameUserDBPass));

	//数据库名字
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBName,sizeof(m_szGameUserDBName));

	//数据库端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wServerInfoDBPort=(WORD)dwReadData;

	//数据库地址
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBAddr,sizeof(m_szServerInfoDBAddr));

	//数据库用户名
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBUser,sizeof(m_szServerInfoDBUser));

	//数据库密码
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBPass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBPass,sizeof(m_szServerInfoDBPass));

	//数据库名字
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBName,sizeof(m_szServerInfoDBName));

	//服务密码
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("LoaderPasswd"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szLoaderServerPass,sizeof(m_szLoaderServerPass));

	//数据库端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerID"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wServerID=(WORD)dwReadData;

	//游戏端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameNetPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wGameNetPort=(WORD)dwReadData;

	//游戏地址
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameNetAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameNetAddr,sizeof(m_szGameNetAddr));

	return;
}

//保存配置
void CInitParamter::SaveInitParamter(bool bSaveAll)
{
	//打开注册表
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\GameServer"),szSystemRegKey);
	if (RegParamter.Create(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return;

	//变量定义
	TCHAR szWriteBuffer[1024]=TEXT("");

	//CenterServer
	CXOREncrypt::EncryptData(m_szCenterServerAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("CenterServerAddr"));

	//授权号码
	CXOREncrypt::EncryptData(m_szAccreditID,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("AccreditID"));

	//登录数据库
	RegParamter.SetDWORDValue(TEXT("GameUserDBPort"),m_wGameUserDBPort);
	CXOREncrypt::EncryptData(m_szGameUserDBAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameUserDBAddr"));
	CXOREncrypt::EncryptData(m_szGameUserDBUser,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameUserDBUser"));
	CXOREncrypt::EncryptData(m_szGameUserDBPass,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameUserDBPass"));

	//房间数据库
	RegParamter.SetDWORDValue(TEXT("ServerInfoDBPort"),m_wServerInfoDBPort);
	CXOREncrypt::EncryptData(m_szServerInfoDBAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBAddr"));
	CXOREncrypt::EncryptData(m_szServerInfoDBUser,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBUser"));
	CXOREncrypt::EncryptData(m_szServerInfoDBPass,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBPass"));

	CXOREncrypt::EncryptData(m_szLoaderServerPass,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("LoaderPasswd"));

	RegParamter.SetDWORDValue(TEXT("ServerID"),m_wServerID);
	//游戏端口
	RegParamter.SetDWORDValue(TEXT("GameNetPort"),m_wGameNetPort);

	//游戏地址
	CXOREncrypt::EncryptData(m_szGameNetAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameNetAddr"));


	if (bSaveAll==true)
	{
		//登录数据库
		CXOREncrypt::EncryptData(m_szServerInfoDBName,szWriteBuffer,sizeof(szWriteBuffer));
		RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBName"));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
