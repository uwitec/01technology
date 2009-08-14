#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"

//Ӧ�ó������
CGameServerApp theApp;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerApp::CGameServerApp() : CWinApp(TEXT("GameService"))
{
}

//��ʼ������
BOOL CGameServerApp::InitInstance()
{
	__super::InitInstance();

	//�ڲ���ʼ��
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();
	SetRegistryKey(szSystemRegKey);

	//���ù���Ŀ¼
	TCHAR szDirWork[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szDirWork,sizeof(szDirWork));
	int nModuleLen=lstrlen(szDirWork);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	szDirWork[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(szDirWork);


	//��������
	CGameServerDlg ServiceLoaderDlg;
	ServiceLoaderDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
