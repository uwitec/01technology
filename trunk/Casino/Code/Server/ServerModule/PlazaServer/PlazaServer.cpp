#include "Stdafx.h"
#include "PlazaServer.h"
#include "PlazaServerDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CPlazaServerApp theApp;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlazaServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaServerApp::CPlazaServerApp()
{
}

//��ʼ������
BOOL CPlazaServerApp::InitInstance()
{
	__super::InitInstance();

	//��ʼ�����
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szSystemRegKey);

	//��ʾ������
	CPlazaServerDlg ServerDlg;
	m_pMainWnd=&ServerDlg;
	ServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
