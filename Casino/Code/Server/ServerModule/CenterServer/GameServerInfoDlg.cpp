// GameServerInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "GameServerInfoDlg.h"
#include ".\gameserverinfodlg.h"


// CGameServerInfoDlg 对话框

IMPLEMENT_DYNAMIC(CGameServerInfoDlg, CDialog)
CGameServerInfoDlg::CGameServerInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameServerInfoDlg::IDD, pParent)
	, m_strServerName(_T("默认"))
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


// CGameServerInfoDlg 消息处理程序

BOOL CGameServerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_SERVERNAME))->LimitText(31);



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGameServerInfoDlg::OnBnClickedOk()
{
	UpdateData();
	if(m_strServerName.IsEmpty())
	{
		AfxMessageBox("名字不能为空");
		return ;
	}
	if(m_dwMaxUserCount <= 0)
	{
		AfxMessageBox("最大人数必须大于 0");
		return ;
	}
	OnOK();
}
