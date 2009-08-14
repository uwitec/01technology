#include "Stdafx.h"
#include "AtlBase.h"
#include "PlazaServerDlg.h"
#include "ConnectUserDlg.h"

#define WM_SHOWUSERCOUNTNOTIRY WM_USER + 200
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPlazaServerDlg, CDialog)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_INIT_SERVICE, OnBnClickedInitService)
	ON_BN_CLICKED(IDC_ENUMCONNECTUSER, OnBnClickedEnumConnectUser)
	ON_MESSAGE(WM_REQUESTINFOOK, OnRequestInfoOk)
	ON_MESSAGE(WM_SHOWUSERCOUNTNOTIRY, OnShowUserCountNotify)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CSystemOptionDlg::CSystemOptionDlg() : CDialog(IDD_SYSTEM_OPTION)
{
}

//析构函数
CSystemOptionDlg::~CSystemOptionDlg()
{
}

//控件子类化
void CSystemOptionDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//初始化函数
BOOL CSystemOptionDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//限制输入
	((CEdit *)GetDlgItem(IDC_LISTEN_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_MAX_CONNECT))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_LOGONSERVERPASS))->LimitText(31);
	//服务地址
	TCHAR szHostName[64],szServerAddr[16];
	gethostname(szHostName,sizeof(szHostName));
	HOSTENT * pHostent=gethostbyname(szHostName);
	LPIN_ADDR * lpAddrList=(LPIN_ADDR *)pHostent->h_addr_list;
	CComboBox * pServerAddr=(CComboBox *)GetDlgItem(IDC_SERVER_ADDR);
	while (*lpAddrList!=NULL)
	{
		lstrcpyn(szServerAddr,inet_ntoa(**lpAddrList),CountArray(szServerAddr));
		int nIndex=pServerAddr->AddString(szServerAddr);
		pServerAddr->SetItemData(nIndex,(*lpAddrList)->S_un.S_addr);
		lpAddrList++;
	}
	(((CComboBox *)GetDlgItem(IDC_SERVER_ADDR)))->SetCurSel(0);


	//加载参数
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	//设置控件
	SetDlgItemInt(IDC_LISTEN_PORT,InitParamter.m_wListenPort,FALSE);
	SetDlgItemInt(IDC_MAX_CONNECT,InitParamter.m_wMaxConnect,FALSE);

	DWORD dwDataBaseIP;
	CIPAddressCtrl* pDataBaseIP;

	//服务地址
	pServerAddr=(CComboBox *)GetDlgItem(IDC_SERVER_ADDR);
	int nItemCount=pServerAddr->GetCount();
	for (int i=0;i<nItemCount;i++)
	{
		CString strTmp;
		pServerAddr->GetLBText(i, strTmp);
		if (InitParamter.m_szLogonServerAddr==strTmp) 
		{
			pServerAddr->SetCurSel(i);
			break;
		}
	}
	if (nItemCount==i)
	{
		pServerAddr->SetWindowText(InitParamter.m_szLogonServerAddr);
	}

	//CenterServer
	dwDataBaseIP=inet_addr(InitParamter.m_szCenterServerAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szCenterServerAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));


	//启动服务密码
	SetDlgItemText(IDC_LOGONSERVERPASS,InitParamter.m_szLogonServerPass);


	return TRUE;
}

//确定函数
void CSystemOptionDlg::OnOK()
{
	//获取输入
	CInitParamter InitParamter;
	InitParamter.m_wListenPort=GetDlgItemInt(IDC_LISTEN_PORT);
	InitParamter.m_wMaxConnect=GetDlgItemInt(IDC_MAX_CONNECT);

	BYTE *pAddrByte;
	DWORD dwDataBaseIP;

	//服务地址
	GetDlgItemText(IDC_SERVER_ADDR,InitParamter.m_szLogonServerAddr,CountArray(InitParamter.m_szLogonServerAddr));

	//CenterServer
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szCenterServerAddr,sizeof(InitParamter.m_szCenterServerAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//启动服务密码
	GetDlgItemText(IDC_LOGONSERVERPASS,InitParamter.m_szLogonServerPass,sizeof(InitParamter.m_szLogonServerPass));

	//保存设置
	InitParamter.SaveInitParamter(true);

	__super::OnOK();
}
//////////////////////////////////////////////////////////////////////////

class CUpdateEventSink: public IUpdateEventSink
{
public:
	CUpdateEventSink(CWnd *pWnd)
	{
		m_pNotifyWnd = pWnd;
	}
	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CUpdateEventSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer)
	{
		QUERYINTERFACE(IUpdateEventSink,Guid,dwQueryVer);
		QUERYINTERFACE_IUNKNOWNEX(IUpdateEventSink,Guid,dwQueryVer);
		return NULL;
	}
	//更新人数通知
	virtual void __cdecl ShowUserCountNotify(WORD wLogonUserCount,WORD wGameUserCount)
	{
		if(m_pNotifyWnd && ::IsWindow(m_pNotifyWnd->m_hWnd))
		{
			m_pNotifyWnd->PostMessage(WM_SHOWUSERCOUNTNOTIRY, wLogonUserCount, wGameUserCount);
		}
		return;
	}
public:
	CWnd *m_pNotifyWnd;
};
//////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaServerDlg::CPlazaServerDlg() : CDialog(IDD_LOGONSERVER_DIALOG)
{
	m_bAskQuit=true;
}

//析构函数
CPlazaServerDlg::~CPlazaServerDlg()
{
	delete m_pUpdateEventSink;
	m_pUpdateEventSink = NULL;
}

//控件子类化
void CPlazaServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditCtrl);
}

//初始化函数
BOOL CPlazaServerDlg::OnInitDialog()
{
	__super::OnInitDialog();
	

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	m_pUpdateEventSink = new CUpdateEventSink(this);
	//设置组件
	if (m_EventService.CreateInstance()==true)
	{
		IUnknownEx * pIUnknownEx=(IUnknownEx *)m_EventService->QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
		m_EventService->SetRichEditHwnd(m_RichEditCtrl.GetSafeHwnd());
		m_LogonService.SetEventService(pIUnknownEx);
		m_RequestSocket.SetEventService(pIUnknownEx);


		pIUnknownEx=(IUnknownEx *)m_pUpdateEventSink->QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
		m_LogonService.SetUpdateEventSink(pIUnknownEx);
	}
	m_RequestSocket.SetParentWnd(this);
	m_RichEditCtrl.SetBackgroundColor(FALSE,RGB(10,10,10));

	SetWindowText(TEXT("大厅服务器  (停止)"));
	UpdateUserCount(0,0);

	return TRUE;
}

//确定消息
void CPlazaServerDlg::OnOK()
{
	return;
}

//取消函数
void CPlazaServerDlg::OnCancel()
{
	//获取状态
	bool bRuning=m_LogonService.IsService();

	//询问退出
	if ((m_bAskQuit==true)&&(bRuning==true))
	{
		CString strMessage=TEXT("大厅服务器正在运行中，确实要退出吗？");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;
	}

	//停止服务
	m_bAskQuit=true;
	if (bRuning==true)
	{
		KillTimer(1000);
		m_LogonService.StopLogonService();
	}

	__super::OnCancel();
}

//消息解释
BOOL CPlazaServerDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//启动服务
void CPlazaServerDlg::OnBnClickedStartService()
{
	//加载参数
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();
	//CenterServer
	DWORD dwIP=inet_addr(InitParamter.m_szCenterServerAddr);
	if (dwIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szCenterServerAddr);
		if (lpHost!=NULL) dwIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	if(!m_RequestSocket.Connect(dwIP, PORT_CENTER_SERVER))
	{
		if (m_EventService.GetInterface()) m_EventService->ShowEventNotify(TEXT("大厅服务失败"),Level_Normal); 
	}
	SetTimer(1000, 5000, NULL);
	GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);
	return;
}

//停止服务
void CPlazaServerDlg::OnBnClickedStopService()
{
	KillTimer(1000);
	m_RequestSocket.CloseSocket(false);
	if (m_LogonService.StopLogonService()==true)
	{
		if (m_EventService.GetInterface()) m_EventService->ShowEventNotify(TEXT("大厅服务停止成功"),Level_Normal); 
	}
	GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);
	SetWindowText(TEXT("大厅服务器  (停止)"));
	return;
}

//系统配置
void CPlazaServerDlg::OnBnClickedInitService()
{
	CSystemOptionDlg SystemOptionDlg;
	SystemOptionDlg.DoModal();

	return;
}
void CPlazaServerDlg::OnBnClickedEnumConnectUser()
{
	CConnectUserDlg	dlg(&m_LogonService,this);
	dlg.DoModal();
}

//请求信息成功
LRESULT CPlazaServerDlg::OnRequestInfoOk(WPARAM w, LPARAM l)
{
	m_RequestSocket.CloseSocket(false);
	if (m_LogonService.StartLogonService()==true)
	{
		SetWindowText(TEXT("大厅服务器  (运行....)"));
		if (m_EventService.GetInterface()) m_EventService->ShowEventNotify(TEXT("大厅服务启动成功"),Level_Normal);
	}
	return 1;
}
//更新人数
LRESULT CPlazaServerDlg::OnShowUserCountNotify(WPARAM w, LPARAM l)
{
	UpdateUserCount(w,l);
	return 1;
}
//更新人数
void CPlazaServerDlg::UpdateUserCount(WORD wLogonUserCount,WORD wGameUserCount)
{
	TCHAR szDescribe[128]=TEXT("");
	_snprintf(szDescribe,sizeof(szDescribe),TEXT("【在线登陆人数 %d】【在线游戏人数 %d】"),wLogonUserCount,wGameUserCount);
	SetDlgItemText(IDC_STATIC_USERCOUONT,szDescribe);
}
//////////////////////////////////////////////////////////////////////////

void CPlazaServerDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1000)
	{
		if(m_LogonService.IsService() == false)
		{
			if (m_EventService.GetInterface()) m_EventService->ShowEventNotify(TEXT("启动服务超时失败"),Level_Warning);
			OnBnClickedStopService();
			if (m_EventService.GetInterface()) m_EventService->ShowEventNotify(TEXT("再次尝试启动服务"),Level_Normal); 
			OnBnClickedStartService();
		}
	}
}