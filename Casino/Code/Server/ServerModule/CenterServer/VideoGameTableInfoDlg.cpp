// VideoGameTableInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "VideoGameTableInfoDlg.h"


// CVideoGameTableInfoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoGameTableInfoDlg, CDialog)
CVideoGameTableInfoDlg::CVideoGameTableInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoGameTableInfoDlg::IDD, pParent)
	, m_dwTableNumber(0)
	, m_ullMaxUserRevenseSharing(500000)
	, m_dwStateKernelServerAddr(ntohl(inet_addr("127.0.0.1")))
	, m_dwStateKernelServerPort(9200)
	, m_dwHighScore(2000)
	, m_dwCellScore(100)
{
}

CVideoGameTableInfoDlg::~CVideoGameTableInfoDlg()
{
}

void CVideoGameTableInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TABLENUMBER, m_dwTableNumber);
	DDX_Text(pDX, IDC_MAXUSERREVENUESHARING, m_ullMaxUserRevenseSharing);
	DDX_IPAddress(pDX, IDC_SERVER_STATEKERNEL_IP, m_dwStateKernelServerAddr);
	DDX_Text(pDX, IDC_SERVER_STATEKERNEL_PORT, m_dwStateKernelServerPort);
	DDX_Text(pDX, IDC_HIGHSCORE2, m_dwHighScore);
	DDX_Text(pDX, IDC_CELLSCORE, m_dwCellScore);
}


BEGIN_MESSAGE_MAP(CVideoGameTableInfoDlg, CDialog)
END_MESSAGE_MAP()


// CVideoGameTableInfoDlg 消息处理程序
