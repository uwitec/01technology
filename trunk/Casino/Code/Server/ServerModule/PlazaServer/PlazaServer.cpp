#include "Stdafx.h"
#include "PlazaServer.h"
#include "PlazaServerDlg.h"

//////////////////////////////////////////////////////////////////////////

//应用程序对象
CPlazaServerApp theApp;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaServerApp::CPlazaServerApp()
{
}

//初始化函数
BOOL CPlazaServerApp::InitInstance()
{
	__super::InitInstance();

	//初始化组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szSystemRegKey);

	//显示主窗口
	CPlazaServerDlg ServerDlg;
	m_pMainWnd=&ServerDlg;
	ServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
