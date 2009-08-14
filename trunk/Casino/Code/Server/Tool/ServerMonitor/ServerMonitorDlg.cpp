// ServerMonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerMonitor.h"
#include "ServerMonitorDlg.h"
#include ".\servermonitordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  IDC_GRIDGAMESERVICE 1001

CString inline GetMyAppPath()
{
	TCHAR szFolder[512];
	GetModuleFileName(NULL, szFolder, 512);
	CString strPath(szFolder);
	int pos = strPath.ReverseFind('\\');
	if(pos != -1)
		return strPath.Left(pos) + "\\" ;
	return strPath + "\\";
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CServerMonitorDlg �Ի���



CServerMonitorDlg::CServerMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRunning = FALSE;
}

void CServerMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerMonitorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_RUN, OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedButtonClose)
	ON_MESSAGE(WM_MSG_STATUS, OnMsgStatus)
END_MESSAGE_MAP()


// CServerMonitorDlg ��Ϣ�������

BOOL CServerMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	CRect rc;
	GetClientRect(&rc);
	rc.InflateRect(-5, -80,-5,-5);

	m_gridServerMonitor.Create(CRect(rc), this, IDC_GRIDGAMESERVICE);
	m_gridServerMonitor.SetEditable(FALSE);
	m_gridServerMonitor.SetSingleRowSelection(TRUE);
	m_gridServerMonitor.SetFixedRowSelection(FALSE);
	m_gridServerMonitor.SetListMode(TRUE);
	m_BtnDataBase.SetGrid( &m_gridServerMonitor);

	try 
	{
		m_gridServerMonitor.SetRowCount(1);
		m_gridServerMonitor.SetColumnCount(6);
		m_gridServerMonitor.SetFixedRowCount(1);
		m_gridServerMonitor.SetFixedColumnCount(0);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	int c = 0;
	m_gridServerMonitor.SetItemText(0, c, TEXT("����"));
	m_gridServerMonitor.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridServerMonitor.SetColumnWidth(c, 120);
	c ++;

	m_gridServerMonitor.SetItemText(0, c, TEXT("�����ַ"));
	m_gridServerMonitor.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridServerMonitor.SetColumnWidth(c, 110);
	c ++;

	m_gridServerMonitor.SetItemText(0, c, TEXT("IP��ַ"));
	m_gridServerMonitor.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridServerMonitor.SetColumnWidth(c, 120);
	c ++;

	m_gridServerMonitor.SetItemText(0, c, TEXT("����״̬"));
	m_gridServerMonitor.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridServerMonitor.SetColumnWidth(c, 70);
	c ++;

	m_gridServerMonitor.SetItemText(0, c, TEXT("�����ٶ�"));
	m_gridServerMonitor.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridServerMonitor.SetColumnWidth(c, 220);
	c ++;

	m_gridServerMonitor.SetItemText(0, c, TEXT("��Ϣ"));
	m_gridServerMonitor.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridServerMonitor.SetColumnWidth(c, 80);
	c ++;

	m_gridServerMonitor.SetRowHeight(0, 30);
	m_gridServerMonitor.SetDefCellHeight(26);

	CString strSerName,strSerAddr;
	strSerName = AfxGetApp()->GetProfileString(TEXT("ServerMonitor"), TEXT("SerName"), TEXT("������-1"));
	strSerAddr = AfxGetApp()->GetProfileString(TEXT("ServerMonitor"), TEXT("SerAddr"), TEXT("127.0.0.1"));
	SetDlgItemText(IDC_EDIT_SERNAME, strSerName);
	SetDlgItemText(IDC_EDIT_SERADDR, strSerAddr);

	CString strFilePath =  GetMyAppPath() + TEXT("MonitorItem.xml");
	LoadMonitorItem((LPCSTR)strFilePath);

	SetDlgItemInt(IDC_EDIT_SENDPERIOD, 5);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CServerMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CServerMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerMonitorDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CServerMonitorDlg::OnBnClickedCancel()
{
	CString strFilePath =  GetMyAppPath() + TEXT("MonitorItem.xml");
	SaveMonitorItem((LPCSTR)strFilePath);

	int r = m_gridServerMonitor.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		tagMonitorData *pMonitorData = 
			(tagMonitorData*)m_gridServerMonitor.GetItemData(i, 0);
		if(pMonitorData)
		{
			delete pMonitorData;
		}
	}

	CString strSerName;
	GetDlgItemText(IDC_EDIT_SERNAME, strSerName);
	CString strSerAddr;
	GetDlgItemText(IDC_EDIT_SERADDR, strSerAddr);
	AfxGetApp()->WriteProfileString(TEXT("ServerMonitor"), TEXT("SerName"), strSerName);
	AfxGetApp()->WriteProfileString(TEXT("ServerMonitor"), TEXT("SerAddr"), strSerAddr);
	OnCancel();
}

void CServerMonitorDlg::OnBnClickedButtonAdd()
{
	CString strSerName;
	GetDlgItemText(IDC_EDIT_SERNAME, strSerName);
	CString strSerAddr;
	GetDlgItemText(IDC_EDIT_SERADDR, strSerAddr);
	if(strSerName.GetLength() == 0 ||
		strSerAddr.GetLength() == 0)
		return;
	tagMonitorData *pMonitorData = new tagMonitorData;
	pMonitorData->strSerName = strSerName;
	pMonitorData->strSerAddr = strSerAddr;
	pMonitorData->strNetAddr = "δ֪";
	pMonitorData->bNetPassFlag = FALSE;
	pMonitorData->nNetDelayValueIndex = 0;
	memset(pMonitorData->nNetDelayValue,0, sizeof(pMonitorData->nNetDelayValue)/sizeof(pMonitorData->nNetDelayValue[0]) * sizeof(int));

	AddMonitorItem(pMonitorData);
}

void CServerMonitorDlg::OnBnClickedButtonDel()
{
	if(m_bRunning == TRUE)
	{
		AfxMessageBox(TEXT("��������,����ɾ��"));
		return;
	}
	CCellRange cell = m_gridServerMonitor.GetSelectedCellRange();
	if(cell.GetMinRow() != -1 && cell.GetMinRow() != 0)
	{
		CString strMessage=TEXT("ȷʵҪɾ����ServerMonitor��");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;

		tagMonitorData *pMonitorData = 
			(tagMonitorData*)m_gridServerMonitor.GetItemData(cell.GetMinRow(), 0);
		if(pMonitorData)
		{
			
			delete pMonitorData;
		}

		m_gridServerMonitor.DeleteRow(cell.GetMinRow());
		m_gridServerMonitor.Refresh();

	}
}

void CServerMonitorDlg::OnBnClickedButtonRun()
{
	if(m_bRunning)
		return;
	m_bRunning = TRUE;
	m_PingThread.InitPing(this->m_hWnd, &m_bRunning);

	int r = m_gridServerMonitor.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		tagMonitorData *pMonitorData = 
			(tagMonitorData*)m_gridServerMonitor.GetItemData(i, 0);
		if(pMonitorData )
		{
			pMonitorData->strNetAddr = "δ֪";
			pMonitorData->bNetPassFlag = FALSE;
			pMonitorData->nNetDelayValueIndex = 0;
			memset(pMonitorData->nNetDelayValue,0, sizeof(pMonitorData->nNetDelayValue)/sizeof(pMonitorData->nNetDelayValue[0]) * sizeof(int)/sizeof(pMonitorData->nNetDelayValue[0]) * sizeof(int));
			m_gridServerMonitor.SetItemText(i, 3, pMonitorData->bNetPassFlag ? TEXT("��ͨ"):TEXT("����"));
			CString strTmp;
			strTmp.Format("%d - %d - %d - %d - %d", pMonitorData->nNetDelayValue[0],
				pMonitorData->nNetDelayValue[1],
				pMonitorData->nNetDelayValue[2],
				pMonitorData->nNetDelayValue[3],
				pMonitorData->nNetDelayValue[4]);
			m_gridServerMonitor.SetItemText(i, 4, strTmp);
			m_gridServerMonitor.SetItemText(i, 5, TEXT("��"));
			for(int c = 0; c < m_gridServerMonitor.GetColumnCount(); c ++)
				m_gridServerMonitor.SetItemBkColour(i, c, RGB(200,10,10));
			
			m_PingThread.AddTask(pMonitorData->strSerAddr, i);
		}
	}
	m_gridServerMonitor.Refresh();
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

	m_PingThread.StartThead();
}

void CServerMonitorDlg::OnBnClickedButtonStop()
{
	if(!m_bRunning)
		return;
	m_bRunning = FALSE;
	m_PingThread.StopThread();

	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
}

void CServerMonitorDlg::OnBnClickedButtonClose()
{
	OnBnClickedButtonStop();
	OnBnClickedCancel();
}
void CServerMonitorDlg::AddMonitorItem(tagMonitorData *pMonitorData)
{
	int r = m_gridServerMonitor.GetRowCount();
	m_gridServerMonitor.SetRowCount(r + 1);

	m_gridServerMonitor.SetItemText(r, 0, pMonitorData->strSerName);
	m_gridServerMonitor.SetItemText(r, 1, pMonitorData->strSerAddr);
	m_gridServerMonitor.SetItemText(r, 2, pMonitorData->strNetAddr);
	m_gridServerMonitor.SetItemText(r, 3, pMonitorData->bNetPassFlag ? TEXT("��ͨ"):TEXT("����"));
	CString strTmp;
	strTmp.Format("%d - %d - %d - %d - %d", pMonitorData->nNetDelayValue[0],
		pMonitorData->nNetDelayValue[1],
		pMonitorData->nNetDelayValue[2],
		pMonitorData->nNetDelayValue[3],
		pMonitorData->nNetDelayValue[4]);
	m_gridServerMonitor.SetItemText(r, 4, strTmp);
	m_gridServerMonitor.SetItemText(r, 5, "��");
	m_gridServerMonitor.SetItemData(r, 0, (LPARAM)pMonitorData);
	for(int c = 0; c < m_gridServerMonitor.GetColumnCount(); c ++)
		m_gridServerMonitor.SetItemBkColour(r, c, RGB(200,10,10));
	
	if(m_bRunning)
	{
		m_PingThread.AddTask(pMonitorData->strSerAddr, r);
	}
}
bool CServerMonitorDlg::LoadMonitorItem(const char* szFileName)
{
	CMarkupSTL xml;
	xml.Load(szFileName);
	while ( xml.FindElem("MonitorItem") )
	{
		tagMonitorData *pMonitorData = new tagMonitorData;
		xml.FindChildElem( "ServerName");
		pMonitorData->strSerName = xml.GetChildData().c_str();
		xml.FindChildElem( "ServerAddr");
		pMonitorData->strSerAddr = xml.GetChildData().c_str();
		pMonitorData->strNetAddr = "δ֪";
		pMonitorData->bNetPassFlag = FALSE;
		pMonitorData->nNetDelayValueIndex = 0;
		memset(pMonitorData->nNetDelayValue,0, sizeof(pMonitorData->nNetDelayValue)/sizeof(pMonitorData->nNetDelayValue[0]) * sizeof(int));

		AddMonitorItem(pMonitorData);
	}
	return true;
}
bool CServerMonitorDlg::SaveMonitorItem(const char* szFileName)
{
	CMarkupSTL xml;

	int r = m_gridServerMonitor.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		tagMonitorData *pMonitorData = 
			(tagMonitorData*)m_gridServerMonitor.GetItemData(i, 0);
		if(pMonitorData)
		{
			xml.AddElem("MonitorItem");
			xml.IntoElem();
			xml.AddElem("ServerName", (LPCSTR)pMonitorData->strSerName);
			xml.AddElem("ServerAddr", (LPCSTR)pMonitorData->strSerAddr);
			xml.OutOfElem();
		}
	}


	return xml.Save(szFileName);
}
LRESULT CServerMonitorDlg::OnMsgStatus(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam >= sizeof(tagNoticeHeader));
	tagNoticeHeader *pHeader = (tagNoticeHeader*)lParam;
	ASSERT(pHeader->dwContext > 0 && pHeader->dwContext < m_gridServerMonitor.GetRowCount());

	switch(pHeader->wNoticeCode)
	{
	case wNoticeCode_Error:
		{
			tagNoticeError* pNotice = (tagNoticeError*)lParam;
			m_gridServerMonitor.SetItemText(pHeader->dwContext, 5, pNotice->szError);
			tagMonitorData *pMonitorData = 
				(tagMonitorData*)m_gridServerMonitor.GetItemData(pHeader->dwContext, 0);
			if(pMonitorData->bNetPassFlag == TRUE)
			{
				pMonitorData->bNetPassFlag = FALSE;
				m_gridServerMonitor.SetItemText(pHeader->dwContext, 3, pMonitorData->bNetPassFlag ? TEXT("��ͨ"):TEXT("����"));
				delete pNotice;
				for(int c = 0; c < m_gridServerMonitor.GetColumnCount(); c ++)
					m_gridServerMonitor.SetItemBkColour(pHeader->dwContext, c, RGB(200,10,10));
			}
			delete pNotice;
		}
		break;
	case wNoticeCode_UpdateNetAddr:
		{
			tagNoticeUpdateNetAddr* pNotice = (tagNoticeUpdateNetAddr*)lParam;
			m_gridServerMonitor.SetItemText(pHeader->dwContext, 2, pNotice->szNetAddr);
			delete pNotice;
		}
		break;
	case wNoticeCode_RequestTimedOut:
		{
			tagNoticeUpdateNetDelay* pNotice = (tagNoticeUpdateNetDelay*)lParam;
			
			tagMonitorData *pMonitorData = 
				(tagMonitorData*)m_gridServerMonitor.GetItemData(pHeader->dwContext, 0);
			if(pMonitorData->bNetPassFlag == TRUE)
			{
				pMonitorData->bNetPassFlag = FALSE;
				m_gridServerMonitor.SetItemText(pHeader->dwContext, 3, pMonitorData->bNetPassFlag ? TEXT("��ͨ"):TEXT("����"));
				for(int c = 0; c < m_gridServerMonitor.GetColumnCount(); c ++)
					m_gridServerMonitor.SetItemBkColour(pHeader->dwContext, c, RGB(200,10,10));
			}
			delete pNotice;
		}
		break;
	case wNoticeCode_UpdateNetDelay:
		{
			tagNoticeUpdateNetDelay* pNotice = (tagNoticeUpdateNetDelay*)lParam;
			tagMonitorData *pMonitorData = 
				(tagMonitorData*)m_gridServerMonitor.GetItemData(pHeader->dwContext, 0);
			pMonitorData->nNetDelayValue[pMonitorData->nNetDelayValueIndex++] = pNotice->dwElapsed;
			pMonitorData->nNetDelayValueIndex = pMonitorData->nNetDelayValueIndex % 5;
			CString strTmp;
			strTmp.Format("%d - %d - %d - %d - %d", pMonitorData->nNetDelayValue[0],
				pMonitorData->nNetDelayValue[1],
				pMonitorData->nNetDelayValue[2],
				pMonitorData->nNetDelayValue[3],
				pMonitorData->nNetDelayValue[4]);
			m_gridServerMonitor.SetItemText(pHeader->dwContext, 4, strTmp);
			if(pMonitorData->bNetPassFlag == FALSE)
			{
				pMonitorData->bNetPassFlag = TRUE;
				m_gridServerMonitor.SetItemText(pHeader->dwContext, 3, pMonitorData->bNetPassFlag ? TEXT("��ͨ"):TEXT("����"));
				for(int c = 0; c < m_gridServerMonitor.GetColumnCount(); c ++)
					m_gridServerMonitor.SetItemBkColour(pHeader->dwContext, c);
			}
			delete pNotice;
		}
		break;
	default:
		break;
	}
	m_gridServerMonitor.Refresh();
	return 1;
}