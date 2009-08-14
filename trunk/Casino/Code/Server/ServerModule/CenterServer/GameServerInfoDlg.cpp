// GameServerInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "GameServerInfoDlg.h"
#include ".\gameserverinfodlg.h"


// CGameServerInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CGameServerInfoDlg, CDialog)
CGameServerInfoDlg::CGameServerInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameServerInfoDlg::IDD, pParent)
	, m_strServerName(_T("Ĭ��"))
	, m_dwSortID(100)
	, m_dwMaxUserCount(400)
{
}

CGameServerInfoDlg::~CGameServerInfoDlg()
{
}

void CGameServerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVERNAME, m_strServerName);
	DDX_Text(pDX, IDC_SORT_ID, m_dwSortID);
	DDX_Text(pDX, IDC_MAXUSERCOUNT, m_dwMaxUserCount);
}


BEGIN_MESSAGE_MAP(CGameServerInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CGameServerInfoDlg ��Ϣ�������

BOOL CGameServerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_SERVERNAME))->LimitText(31);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGameServerInfoDlg::OnBnClickedOk()
{
	UpdateData();
	if(m_strServerName.IsEmpty())
	{
		AfxMessageBox("���ֲ���Ϊ��");
		return ;
	}
	if(m_dwMaxUserCount <= 0)
	{
		AfxMessageBox("�������������� 0");
		return ;
	}
	OnOK();
}
