#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"
#include "GameServiceHelperContainer.h"


#define IDC_GRIDGAMESERVICE 2000

//////////////////////////////////////////////////////////////////////////
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

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CGameServerDlg, CDialog)
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_NEWGAMESERVICE, OnBnNewGameService)
	ON_BN_CLICKED(IDC_DELGAMESERVICE, OnBnDelGameService)
	ON_BN_CLICKED(IDC_SAVEGAMESERVICE, OnBnSaveGameService)
	ON_WM_QUERYENDSESSION()
	ON_NOTIFY(NM_CLICK, IDC_GRIDGAMESERVICE, OnGridClick)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSystemOptionDlg::CSystemOptionDlg(CInitParamter* pOutInitParamter) : CDialog(IDD_SYSTEM_OPTION)
{
	m_pOutInitParamter = pOutInitParamter;
}

//��������
CSystemOptionDlg::~CSystemOptionDlg()
{
}

//�ؼ����໯
void CSystemOptionDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CSystemOptionDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	((CEdit *)GetDlgItem(IDC_LOADERSERVERPASS))->LimitText(31);
	//�����ַ
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

	//���ز���
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	//��¼���ݿ��ַ
	CIPAddressCtrl * pDataBaseIP;
	
	//CenterServer
	DWORD dwDataBaseIP=inet_addr(InitParamter.m_szCenterServerAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szCenterServerAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	//�����˿�
	if (InitParamter.m_wGameNetPort==0) SetDlgItemText(IDC_PORT,TEXT("11000"));
	else SetDlgItemInt(IDC_PORT,InitParamter.m_wGameNetPort,FALSE);

	//�����ַ
	pServerAddr=(CComboBox *)GetDlgItem(IDC_SERVER_ADDR);
	int nItemCount=pServerAddr->GetCount();
	for (int i=0;i<nItemCount;i++)
	{
		CString strTmp;
		pServerAddr->GetLBText(i, strTmp);
		if (InitParamter.m_szGameNetAddr==strTmp) 
		{
			pServerAddr->SetCurSel(i);
			break;
		}
	}
	if (nItemCount==i)
	{
		pServerAddr->SetWindowText(InitParamter.m_szGameNetAddr);
	}

	//����ID
	if (InitParamter.m_wServerID==0) SetDlgItemText(IDC_SERVERID,TEXT("100"));
	else SetDlgItemInt(IDC_SERVERID,InitParamter.m_wServerID,FALSE);

	//��������
	SetDlgItemText(IDC_LOADERSERVERPASS,InitParamter.m_szLoaderServerPass);

	if(m_pOutInitParamter)
	{
		dwDataBaseIP = inet_addr(m_pOutInitParamter->m_szCenterServerAddr);
		pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));
		SetDlgItemInt(IDC_PORT,m_pOutInitParamter->m_wGameNetPort,FALSE);
		pServerAddr->SetWindowText(m_pOutInitParamter->m_szGameNetAddr);
		SetDlgItemInt(IDC_SERVERID,m_pOutInitParamter->m_wServerID,FALSE);
		SetDlgItemText(IDC_LOADERSERVERPASS,m_pOutInitParamter->m_szLoaderServerPass);
	}

	return TRUE;
}

//ȷ������
void CSystemOptionDlg::OnOK()
{
	//��ȡ����
	CInitParamter InitParamter;

	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE* pAddrByte;

	//CenterServer
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szCenterServerAddr,sizeof(InitParamter.m_szCenterServerAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//�����˿�
	InitParamter.m_wGameNetPort = GetDlgItemInt(IDC_PORT,FALSE);
	//�����ַ
	GetDlgItemText(IDC_SERVER_ADDR,InitParamter.m_szGameNetAddr,CountArray(InitParamter.m_szGameNetAddr));
	//����ID
	InitParamter.m_wServerID = GetDlgItemInt(IDC_SERVERID,FALSE);
	//��������
	GetDlgItemText(IDC_LOADERSERVERPASS,InitParamter.m_szLoaderServerPass,CountArray(InitParamter.m_szLoaderServerPass));

	
	//��������
	InitParamter.SaveInitParamter(false);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerDlg::CGameServerDlg() : CDialog(IDD_SERVICELOADER_DIALOG)
{
	m_bSetStartGameServiceTimer = FALSE;
	
}

//��������
CGameServerDlg::~CGameServerDlg()
{
}

//�ؼ���
void CGameServerDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditCtrl);
}

//��ʼ������
BOOL CGameServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//�������
	if (m_EventService.CreateInstance()==true)
	{
		IUnknownEx * pIUnknownEx=(IUnknownEx *)m_EventService->QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
		m_EventService->SetRichEditHwnd(m_RichEditCtrl.GetSafeHwnd());
		m_EventService->ShowEventNotify(TEXT("���¼����񡱴����ɹ�"),Level_Normal);
	}
	else 
	{
		AfxMessageBox(TEXT("��ϵͳ��ʾ�������¼����񡱴���ʧ�ܻ������������\n"));
	}
	m_RichEditCtrl.SetBackgroundColor(FALSE,RGB(10,10,10));

	CRect rc;
	GetClientRect(&rc);
	rc.InflateRect(-12, -20);
	rc.top += 35;
	rc.bottom = rc.top + 380;

	m_gridGameService.Create(CRect(rc), this, IDC_GRIDGAMESERVICE);
	m_gridGameService.SetEditable(FALSE);
	m_gridGameService.SetSingleRowSelection(TRUE);
	m_gridGameService.SetFixedRowSelection(FALSE);
	m_gridGameService.SetListMode(TRUE);
	m_BtnDataBase.SetGrid( &m_gridGameService);

	try 
	{
		m_gridGameService.SetRowCount(1);
		m_gridGameService.SetColumnCount(3);
		m_gridGameService.SetFixedRowCount(1);
		m_gridGameService.SetFixedColumnCount(0);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	int c = 0;
	m_gridGameService.SetItemText(0, c, TEXT("����"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, 540);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("��Ϣ"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, 270);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("����"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, 150);
	c ++;

	m_gridGameService.SetRowHeight(0, 40);
	m_gridGameService.SetDefCellHeight(24);

	LoadGameServiceParamFile();

	return TRUE;
}

//��Ϣ����
BOOL CGameServerDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȷ����Ϣ
void CGameServerDlg::OnOK()
{
	return;
}

//ȡ������
void CGameServerDlg::OnCancel()
{

	//ѯ���˳�
	bool bRuning=HaveGameServiceRunning();
	if (bRuning==true)
	{
		CString strMessage=TEXT("��Ϸ����������������У�ȷʵҪ�˳���");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;
	}

	//ֹͣ����
	if (bRuning==true) StopAllGameService();
	ClearGameService();

	__super::OnCancel();
}

void CGameServerDlg::OnBnNewGameService()
{
	CSystemOptionDlg SystemOptionDlg;
	if(SystemOptionDlg.DoModal() == IDOK)
	{
		GameServiceParam param;
		memset(&param, 0, sizeof(GameServiceParam));

		CInitParamter InitParamter;
		InitParamter.LoadInitParamter();
		_tcscpy(param.m_szCenterServerAddr, InitParamter.m_szCenterServerAddr);
		_tcscpy(param.m_szCenterServerAddr, InitParamter.m_szCenterServerAddr);
		param.m_wServerID = InitParamter.m_wServerID;
		param.m_wGameNetPort = InitParamter.m_wGameNetPort;
		_tcscpy(param.m_szGameNetAddr, InitParamter.m_szGameNetAddr);

		NewGameService(param);
	}

	return;
}
void CGameServerDlg::OnBnDelGameService()
{

	CCellRange cell = m_gridGameService.GetSelectedCellRange();
	if(cell.GetMinRow() != -1 && cell.GetMinRow() != 0)
	{
		CString strMessage=TEXT("ȷʵҪɾ������Ϸ������");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;

		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(cell.GetMinRow(), 0);
		if(pGameService)
		{
			pGameService->RequstStart(FALSE);
			pGameService->Stop();
			delete pGameService;
		}

		m_gridGameService.DeleteRow(cell.GetMinRow());
		m_gridGameService.Refresh();

	}
	return;
}
void CGameServerDlg::OnBnSaveGameService()
{
	SaveGameServiceParamFile();
}
//��ʾ����
void CGameServerDlg::ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (m_EventService.GetInterface()) m_EventService->ShowEventNotify(pszString,TraceLevel);
}
void CGameServerDlg::ShowDescribeMessage(int nGameServiceHelperIndex,LPCTSTR pszString)
{
	if(nGameServiceHelperIndex < 0 || nGameServiceHelperIndex > m_gridGameService.GetRowCount() - 1)
		return;
	m_gridGameService.SetItemText(nGameServiceHelperIndex + 1, 1, pszString);
	m_gridGameService.Refresh();
}
void CGameServerDlg::NotifyServiceStatus(int nGameServiceHelperIndex,bool bRunning)
{
	if(nGameServiceHelperIndex < 0 || nGameServiceHelperIndex > m_gridGameService.GetRowCount() - 1)
		return;
	CGridBtnCell *pGridBtnCell = (CGridBtnCell*) m_gridGameService.GetCell(nGameServiceHelperIndex + 1,2);

	pGridBtnCell->SetupBtns(
		0,        
		DFC_BUTTON, 
		bRunning ? (DFCS_BUTTONPUSH | DFCS_INACTIVE) : DFCS_BUTTONPUSH , 
		(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
		0,  
		0,  
		TEXT("����"));

	pGridBtnCell->SetupBtns(
		1,        
		DFC_BUTTON, 
		bRunning ? DFCS_BUTTONPUSH : (DFCS_BUTTONPUSH | DFCS_INACTIVE), 
		(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
		0,  
		0,  
		TEXT("ֹͣ"));

	m_gridGameService.Refresh();
}
BOOL CGameServerDlg::HaveGameServiceRunning()
{
	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
		if(pGameService)
		{
			if(pGameService->IsRunning())
				return TRUE;
		}
	}
	return FALSE;
}
BOOL CGameServerDlg::IsAllGameServiceRunning()
{
	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
		if(pGameService)
		{
			if(pGameService->IsRequstStart() &&
				pGameService->IsRunning() == FALSE)
				return FALSE;
		}
	}
	return TRUE;
}
void CGameServerDlg::ClearGameService()
{
	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
		if(pGameService)
		{
			delete pGameService;
		}
	}
}
void CGameServerDlg::NewGameService(GameServiceParam& param)
{
	CString strTmp;
	int r = m_gridGameService.GetRowCount();
	m_gridGameService.SetRowCount(r + 1);

	strTmp.Format(TEXT("���ķ�������ַ:%s   ���� ID:%d   �����ַ:%s   ����˿�:%d"),
		param.m_szCenterServerAddr,
		(int)param.m_wServerID, 
		param.m_szGameNetAddr,
		(int)param.m_wGameNetPort);
	m_gridGameService.SetItemText(r, 0, strTmp);
	m_gridGameService.SetItemText(r, 1, TEXT("ֹͣ"));

	if (m_gridGameService.SetCellType(r,2, RUNTIME_CLASS(CGridBtnCell)))
	{
		CGridBtnCell *pGridBtnCell = (CGridBtnCell*) m_gridGameService.GetCell(r,2);
		pGridBtnCell->SetBtnDataBase( &m_BtnDataBase);

		pGridBtnCell->SetupBtns(
			0,        
			DFC_BUTTON, 
			DFCS_BUTTONPUSH, 
			(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
			0,  
			0,  
			TEXT("����"));

		pGridBtnCell->SetupBtns(
			1,        
			DFC_BUTTON, 
			DFCS_BUTTONPUSH | DFCS_INACTIVE, 
			(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
			0,  
			0,  
			TEXT("ֹͣ"));

		pGridBtnCell->SetupBtns(
			2,        
			DFC_BUTTON, 
			DFCS_BUTTONPUSH, 
			(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
			0,  
			0,  
			TEXT("����"));

	}
	TCHAR szData[1024]=TEXT("");
	DWORD dwDataSize=sizeof(szData);
	CXOREncrypt::CrevasseData(param.m_szLoaderServerPass,szData,sizeof(szData));
	CGameServiceHelperContainer *pHelperContainer = new CGameServiceHelperContainer;
	pHelperContainer->Init(this, r-1, m_EventService.GetInterface(), param.m_szCenterServerAddr,
		szData, param.m_wServerID, param.m_wGameNetPort, param.m_szGameNetAddr);

	m_gridGameService.SetItemData(r, 0, (LPARAM)pHelperContainer);

	m_gridGameService.Refresh();
}
void CGameServerDlg::LoadGameServiceParamFile()
{
	CString strFilePath =  GetMyAppPath() + TEXT("GameServiceParam.xml");
	GameServiceParamArray array;
	CGameServiceParamFile saveFile;
	saveFile.Load((LPCSTR)strFilePath, array);

	for(int i = 0; i < array.GetCount(); i ++)
	{
		GameServiceParam param = array.GetAt(i);

		NewGameService(param);
	}

}
void CGameServerDlg::SaveGameServiceParamFile()
{
	GameServiceParamArray array;

	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
		if(pGameService)
		{
			GameServiceParam param;
			memset(&param, 0, sizeof(GameServiceParam));

			_tcscpy(param.m_szCenterServerAddr, pGameService->GetInitParamter().m_szCenterServerAddr);
			_tcscpy(param.m_szCenterServerAddr, pGameService->GetInitParamter().m_szCenterServerAddr);
			param.m_wServerID = pGameService->GetInitParamter().m_wServerID;
			param.m_wGameNetPort = pGameService->GetInitParamter().m_wGameNetPort;
			_tcscpy(param.m_szGameNetAddr, pGameService->GetInitParamter().m_szGameNetAddr);

			array.Add(param);
		}
	}
	CString strFilePath =  GetMyAppPath() + TEXT("GameServiceParam.xml");
	CGameServiceParamFile saveFile;
	saveFile.Save((LPCSTR)strFilePath, array);
}


//////////////////////////////////////////////////////////////////////////

BOOL CGameServerDlg::OnQueryEndSession()
{
	//ֹͣ����
	if (HaveGameServiceRunning())
	{
		CString strMessage=TEXT("��Ϸ����������������У�ϵͳҪ��ע���ػ�����ʧ��");
		ShowErrorMessasge(strMessage,Level_Warning);

		return FALSE;
	}
	return TRUE;
}

//����´���
void CGameServerDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (pItem->iRow < 1)
		return;


	int nRow = pItem->iRow;
	int nCol = pItem->iColumn;

	*pResult = 0;

	if( nRow < m_gridGameService.GetFixedRowCount() )
		return;

	CGridBtnCell* pGridBtnCell;

	switch( nCol)
	{

	case 2:  // Radio buttons
		pGridBtnCell = (CGridBtnCell*) m_gridGameService.GetCell( nRow, nCol);
		if( pGridBtnCell)
		{
			if( !pGridBtnCell->IsKindOf( RUNTIME_CLASS(CGridBtnCell) ) )
			{
				return;
			}
			switch(pGridBtnCell->GetLastCtlClicked())
			{
			case 0:
				OnRunGameServerService(nRow);
				break;
			case 1:
				OnStopGameServerService(nRow);
				break;
			case 2:
				OnConfigGameServerService(nRow);
				break;
			default:
				break;
			}
		}
		break;

	default:
		break;
	}
}

//���÷���
void CGameServerDlg::OnConfigGameServerService(int nRow)
{
	CGameServiceHelperContainer *pGameService = 
		(CGameServiceHelperContainer*)m_gridGameService.GetItemData(nRow, 0);
	if(pGameService)
	{
		CInitParamter InitParamter;
		InitParamter.LoadInitParamter();
		_tcscpy(InitParamter.m_szCenterServerAddr, pGameService->GetInitParamter().m_szCenterServerAddr);
		_tcscpy(InitParamter.m_szCenterServerAddr, pGameService->GetInitParamter().m_szCenterServerAddr);
		InitParamter.m_wServerID = pGameService->GetInitParamter().m_wServerID;
		InitParamter.m_wGameNetPort = pGameService->GetInitParamter().m_wGameNetPort;
		_tcscpy(InitParamter.m_szGameNetAddr, pGameService->GetInitParamter().m_szGameNetAddr);
	
		CSystemOptionDlg SystemOptionDlg(&InitParamter);
		if(SystemOptionDlg.DoModal() == IDOK)
		{
			InitParamter.LoadInitParamter();

			pGameService->SetParam(InitParamter.m_szCenterServerAddr,
				InitParamter.m_szLoaderServerPass, InitParamter.m_wServerID,
				InitParamter.m_wGameNetPort, InitParamter.m_szGameNetAddr);

			CString strTmp;
			strTmp.Format(TEXT("���ķ�������ַ:%s   ���� ID:%d   �����ַ:%s   ����˿�:%d"),
				InitParamter.m_szCenterServerAddr,
				(int)InitParamter.m_wServerID, 
				InitParamter.m_szGameNetAddr,
				(int)InitParamter.m_wGameNetPort);
			m_gridGameService.SetItemText(nRow, 0, strTmp);
			m_gridGameService.Refresh();
		}
	}
}

//����ȫ������
void CGameServerDlg::OnBnClickedStart()
{
	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
		if(pGameService && 
			pGameService->IsRequstStart() == FALSE &&
			pGameService->IsRunning() == FALSE)
		{
			pGameService->RequstStart(TRUE);
		}
		
	}
	CheckStartGameServiceTimer();
}

//��������
void CGameServerDlg::OnRunGameServerService(int nRow)
{
	CGameServiceHelperContainer *pGameService = 
		(CGameServiceHelperContainer*)m_gridGameService.GetItemData(nRow, 0);
	if(pGameService && 
		pGameService->IsRequstStart() == FALSE)
	{
		pGameService->RequstStart(TRUE);
	}

	CheckStartGameServiceTimer();
}

//ֹͣ����
void CGameServerDlg::OnStopGameServerService(int nRow)
{
	CGameServiceHelperContainer *pGameService = 
		(CGameServiceHelperContainer*)m_gridGameService.GetItemData(nRow, 0);
	if(pGameService)
	{
		CString strMessage=TEXT("ȷʵҪֹͣ����Ϸ������");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;
		pGameService->RequstStart(FALSE);
		pGameService->Stop();
	}
}

//ֹͣ����
void CGameServerDlg::OnBnClickedStop()
{
	CString strMessage=TEXT("ȷʵҪֹͣ������Ϸ������");
	int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
	if (iRetCode!=IDYES) return;
	StopAllGameService();
	KillStartGameServiceTimer();
}

//ֹͣ���з���
void CGameServerDlg::StopAllGameService()
{
	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		CGameServiceHelperContainer *pGameService = 
			(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
		if(pGameService)
		{
			pGameService->RequstStart(FALSE);
			if(pGameService->IsRunning())
			{
				pGameService->Stop();
			}
		}
	}
}

//������ʱ��
void CGameServerDlg::CheckStartGameServiceTimer()
{
	if(m_bSetStartGameServiceTimer == FALSE)
	{
		m_bSetStartGameServiceTimer = TRUE;
		SetTimer(1000,2000,NULL);
	}
}

//ֹͣ��ʱ��
void CGameServerDlg::KillStartGameServiceTimer()
{
	if(m_bSetStartGameServiceTimer == TRUE)
	{
		KillTimer(1000);
		m_bSetStartGameServiceTimer = FALSE;
	}
}

//��ʱ����Ϣ����
 void CGameServerDlg::OnTimer (UINT nIDEvent)
 {
	if(nIDEvent == 1000)
	{
		int r = m_gridGameService.GetRowCount();
		for(int i = 1; i < r; i ++)
		{
			CGameServiceHelperContainer *pGameService = 
				(CGameServiceHelperContainer*)m_gridGameService.GetItemData(i, 0);
			if(pGameService && 
				pGameService->IsRequstStart() &&
				pGameService->IsRunning() == FALSE)
			{
				pGameService->Stop();
				pGameService->Start();
			}
		}

		if(IsAllGameServiceRunning())
			KillStartGameServiceTimer();
	}
 }