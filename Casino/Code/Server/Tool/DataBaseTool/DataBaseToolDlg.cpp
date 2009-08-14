// DataBaseToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "DataBaseToolDlg.h"
#include ".\databasetooldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDataBaseToolDlg 对话框



CDataBaseToolDlg::CDataBaseToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataBaseToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataBaseToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABSHEET, m_Sheet);
}

BEGIN_MESSAGE_MAP(CDataBaseToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_SAVECONFIG, OnBnClickedSaveconfig)
END_MESSAGE_MAP()


// CDataBaseToolDlg 消息处理程序

BOOL CDataBaseToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_NAME))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PIPE))->LimitText(31);

	//加载参数
	m_InitParamter.LoadInitParamter();

	//信息数据库
	SetDlgItemInt(IDC_SERVER_DATABASE_PORT,m_InitParamter.m_wServerDataBasePort,FALSE);
	SetDlgItemText(IDC_SERVER_DATABASE_USER,m_InitParamter.m_szServerDataBaseUser);
	SetDlgItemText(IDC_SERVER_DATABASE_PASS,m_InitParamter.m_szServerDataBasePass);
	SetDlgItemText(IDC_SERVER_DATABASE_NAME,m_InitParamter.m_szServerDataBaseName);
	SetDlgItemText(IDC_SERVER_DATABASE_PIPE,m_InitParamter.m_szServerDataBasePipeName);

	//信息数据库地址
	DWORD dwDataBaseIP=inet_addr(m_InitParamter.m_szServerDataBaseAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(m_InitParamter.m_szServerDataBaseAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	CIPAddressCtrl * pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	m_Sheet.AddPage(TEXT("备份与还原"), &m_dlgBackupRestorePage, IDD_BACKUPRESOREPAGE);
	m_Sheet.AddPage(TEXT("清除历史数据"), &m_dlgClearDataPage, IDD_CLEARDATAPAGE);
	m_Sheet.AddPage(TEXT("高级"), &m_dlgAdvancedPage, IDD_ADVANCEDPAGE);
	m_Sheet.Show();

	m_dlgBackupRestorePage.InitPage(&m_InitParamter);
	m_dlgClearDataPage.InitPage(&m_InitParamter);
	m_dlgAdvancedPage.InitPage(&m_InitParamter);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDataBaseToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataBaseToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDataBaseToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataBaseToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDataBaseToolDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CDataBaseToolDlg::OnBnClickedClose()
{
	OnBnClickedCancel();
}

void CDataBaseToolDlg::OnBnClickedSaveconfig()
{
	//信息数据库
	m_InitParamter.m_wServerDataBasePort=GetDlgItemInt(IDC_SERVER_DATABASE_PORT);
	GetDlgItemText(IDC_SERVER_DATABASE_USER,m_InitParamter.m_szServerDataBaseUser,sizeof(m_InitParamter.m_szServerDataBaseUser));
	GetDlgItemText(IDC_SERVER_DATABASE_PASS,m_InitParamter.m_szServerDataBasePass,sizeof(m_InitParamter.m_szServerDataBasePass));
	GetDlgItemText(IDC_SERVER_DATABASE_NAME,m_InitParamter.m_szServerDataBaseName,sizeof(m_InitParamter.m_szServerDataBaseName));
	GetDlgItemText(IDC_SERVER_DATABASE_PIPE,m_InitParamter.m_szServerDataBasePipeName,sizeof(m_InitParamter.m_szServerDataBasePipeName));

	//信息数据库地址
	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE * pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_sntprintf(m_InitParamter.m_szServerDataBaseAddr,sizeof(m_InitParamter.m_szServerDataBaseAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	m_InitParamter.SaveInitParamter(true);

}
