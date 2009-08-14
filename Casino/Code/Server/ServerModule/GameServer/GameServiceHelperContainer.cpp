#include "stdafx.h"
#include ".\gameservicehelpercontainer.h"
#include "GameServerDlg.h"

CGameServiceHelperContainer::CGameServiceHelperContainer(void)
{
	m_pServiceLoaderDlg = NULL;
	m_nGameServiceHelperIndex = -1;
	m_pIEventService = NULL;
	memset(&m_GameServiceParameter, 0, sizeof(m_GameServiceParameter));
	m_bRequstStart = FALSE;
}

CGameServiceHelperContainer::~CGameServiceHelperContainer(void)
{
}
void CGameServiceHelperContainer::Init(CGameServerDlg*	pServiceLoaderDlg,
									   int nGameServiceHelperIndex,
									   IEventService * pIEventService,
									   TCHAR*	szCenterServerAddr,
									   TCHAR*	szLoaderServerPass,
									   WORD wServerID,
									   WORD wGameNetPort,
									   TCHAR *szGameNetAddr)
{
	m_pServiceLoaderDlg			 = pServiceLoaderDlg;
	m_nGameServiceHelperIndex	 = nGameServiceHelperIndex;
	IUnknownEx * pIUnknownEx=(IUnknownEx *)pIEventService->QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
	m_RequestSocket.SetEventService(pIUnknownEx);
	m_pIEventService  = pIEventService;

	//加载参数
	m_InitParamter.LoadInitParamter();
	SetParam(szCenterServerAddr, szLoaderServerPass, wServerID, wGameNetPort,szGameNetAddr );
	m_RequestSocket.SetParentContainer(this, &m_GameServiceParameter, &m_InitParamter);
}
void CGameServiceHelperContainer::SetParam(TCHAR*	szCenterServerAddr,
			  TCHAR*	szLoaderServerPass,
			  WORD wServerID,
			  WORD wGameNetPort,
			  TCHAR *szGameNetAddr)
{
	_tcscpy(m_InitParamter.m_szCenterServerAddr, szCenterServerAddr);
	_tcscpy(m_InitParamter.m_szCenterServerAddr, szCenterServerAddr);
	m_InitParamter.m_wServerID = wServerID;
	m_InitParamter.m_wGameNetPort = wGameNetPort;
	_tcscpy(m_InitParamter.m_szGameNetAddr, szGameNetAddr);
}
BOOL CGameServiceHelperContainer::Start()
{
	CWaitCursor wc;
	//CenterServer
	DWORD dwIP=inet_addr(m_InitParamter.m_szCenterServerAddr);
	if (dwIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(m_InitParamter.m_szCenterServerAddr);
		if (lpHost!=NULL) dwIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	if(!m_RequestSocket.Connect(dwIP, PORT_CENTER_SERVER))
	{
		TCHAR szDescribe[256]=TEXT("");
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("【服务ID %ld】登录服务失败"),(LONG)m_InitParamter.m_wServerID);
		ShowErrorMessasge(szDescribe,Level_Normal); 
		return false;
	}
	else
		return true;
}
BOOL CGameServiceHelperContainer::IsRunning()
{
	if ((m_GameService.GetInterface()!=NULL)&&(m_GameService->IsService()==true))  return TRUE;
	else return FALSE;
}
void CGameServiceHelperContainer::Stop()
{
	CWaitCursor wc;
	m_RequestSocket.CloseSocket(false);
	if ((m_GameService.GetInterface()!=NULL)&&
		(m_GameService->IsService()==true) &&
		(m_GameService->StopService()==true))
	{
		//NotifyServiceStatus(false);
		TCHAR szDescribe [256]=TEXT("");

		_snprintf(szDescribe,sizeof(szDescribe),TEXT("停止【%s】【%s】"),
			GetGameTypeNameByModuleName(m_GameServiceParameter.szModuleName),
			m_GameServiceParameter.GameServiceOption.szGameRoomName);
		ShowDescribeMessage(szDescribe);

		_snprintf(szDescribe,sizeof(szDescribe),TEXT("【服务ID %ld】停止成功"),(LONG)m_InitParamter.m_wServerID);
		ShowErrorMessasge(szDescribe,Level_Normal);
	}
}
void CGameServiceHelperContainer::ShowErrorMessasge (LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if(m_pServiceLoaderDlg)
		m_pServiceLoaderDlg->ShowErrorMessasge(pszString,TraceLevel);
}
void CGameServiceHelperContainer::ShowDescribeMessage(LPCTSTR pszString)
{
	if(m_pServiceLoaderDlg)
		m_pServiceLoaderDlg->ShowDescribeMessage(m_nGameServiceHelperIndex, pszString);
}
void CGameServiceHelperContainer::NotifyServiceStatus(bool bRunning)
{
	if(m_pServiceLoaderDlg)
		m_pServiceLoaderDlg->NotifyServiceStatus(m_nGameServiceHelperIndex, bRunning);
}

int CGameServiceHelperContainer::OnRequestInfoOk(WPARAM w, LPARAM l)
{
	m_RequestSocket.CloseSocket(false);

	GT_ASSERT(IsRunning() == FALSE);
	if(IsRunning())
		return 1;

	//变量定义
	tagGameServiceParameter GameServiceParameter;
	memcpy(&GameServiceParameter,&m_GameServiceParameter,sizeof(GameServiceParameter));

	//构造参数
	GameServiceParameter.dwCenterAddr=inet_addr(m_InitParamter.m_szCenterServerAddr);
	GameServiceParameter.GameServiceOption.wServerPort = m_InitParamter.m_wGameNetPort;
	GameServiceParameter.GameServiceOption.dwServerAddr = inet_addr(m_InitParamter.m_szGameNetAddr);

	//用户数据库
	GameServiceParameter.GameUserDBInfo.wDataBasePort=m_InitParamter.m_wGameUserDBPort;
	GameServiceParameter.GameUserDBInfo.dwDataBaseAddr=inet_addr(m_InitParamter.m_szGameUserDBAddr);
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBaseUser,m_InitParamter.m_szGameUserDBUser,CountArray(GameServiceParameter.GameUserDBInfo.szDataBaseUser));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBasePass,m_InitParamter.m_szGameUserDBPass,CountArray(GameServiceParameter.GameUserDBInfo.szDataBasePass));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBaseName,m_InitParamter.m_szGameUserDBName,CountArray(GameServiceParameter.GameUserDBInfo.szDataBaseName));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBasePipeName,m_InitParamter.m_szGameUserDBPipeName,CountArray(GameServiceParameter.GameUserDBInfo.szDataBasePipeName));

	//游戏数据库
	GameServiceParameter.GameScoreDBInfo.wDataBasePort=m_InitParamter.m_wServerInfoDBPort;
	GameServiceParameter.GameScoreDBInfo.dwDataBaseAddr=inet_addr(m_InitParamter.m_szServerInfoDBAddr);
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBaseUser,m_InitParamter.m_szServerInfoDBUser,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBaseUser));
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBasePass,m_InitParamter.m_szServerInfoDBPass,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBasePass));
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBaseName,m_InitParamter.m_szServerInfoDBName,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBaseName));
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBasePipeName,m_InitParamter.m_szServerInfoDBPipeName,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBasePipeName));

	TCHAR szDescribe[256]=TEXT("");

	//创建服务
	if (m_GameService.GetInterface()==NULL)
	{
		if (m_GameService.CreateInstance()==true)
		{
			if (m_pIEventService!=NULL) m_GameService->SetEventService(m_pIEventService);
		}
		else 
		{
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("【服务ID %ld】GameService组件创建失败"),(LONG)m_InitParamter.m_wServerID);
			ShowErrorMessasge(szDescribe,Level_Exception);
			return 1;
		}
	}

	//启动服务
	if (m_GameService->StartService(&GameServiceParameter)==true)
	{
		//NotifyServiceStatus(true);

		//提示信息
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("【服务ID %ld】站点 %s 启动成功"),
			(LONG)GameServiceParameter.GameServiceOption.wServerID,
			GameServiceParameter.GameServiceOption.szGameRoomName);
		ShowErrorMessasge(szDescribe,Level_Normal);

		_snprintf(szDescribe,sizeof(szDescribe),TEXT("运行【%s】【%s】"),
			GetGameTypeNameByModuleName(m_GameServiceParameter.szModuleName),
			m_GameServiceParameter.GameServiceOption.szGameRoomName);
		ShowDescribeMessage(szDescribe);
	}
	else 
	{
		//NotifyServiceStatus(false);

		_snprintf(szDescribe,sizeof(szDescribe),TEXT("【服务ID %ld】站点 %s 启动失败"),(LONG)GameServiceParameter.GameServiceOption.wServerID,GameServiceParameter.GameServiceOption.szGameRoomName);
		ShowErrorMessasge(szDescribe,Level_Exception);
	}

	return 1;
}
void CGameServiceHelperContainer::RequstStart(BOOL b)
{
	m_bRequstStart = b;
	if(b)
		NotifyServiceStatus(true);
	else
		NotifyServiceStatus(false);
}
BOOL CGameServiceHelperContainer::IsRequstStart()
{
	return m_bRequstStart;
}