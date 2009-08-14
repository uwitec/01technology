// ConnectUserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlazaServer.h"
#include "ConnectUserDlg.h"
#include "PlazaService.h"
#include ".\connectuserdlg.h"

#define  IDC_GRIDCONNECTUSER 1002

struct tagConnectUser
{
	DWORD	dwRealUserID;
	BYTE    cbUserType;
};

//////////////////////////////////////////////////////////////////////////
class CEnumUserSink: public IEnumUserSink
{
public:
	CEnumUserSink(CConnectUserDlg *pWnd)
	{
		m_pNotifyWnd = pWnd;
	}
	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CEnumUserSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer)
	{
		QUERYINTERFACE(IEnumUserSink,Guid,dwQueryVer);
		QUERYINTERFACE_IUNKNOWNEX(IEnumUserSink,Guid,dwQueryVer);
		return NULL;
	}
	//�û�����
	virtual void __cdecl OnUserData(const DWORD&	dwRealUserID,
		const BYTE&   cbUserType,
		const tagUserData& UserData,
		const DWORD&	dwClientIP)
	{
		m_pNotifyWnd->OnUserData(dwRealUserID, cbUserType, UserData, dwClientIP);
	}
public:
	CConnectUserDlg *m_pNotifyWnd;
};
//////////////////////////////////////////////////////////////////////////

// CConnectUserDlg �Ի���

IMPLEMENT_DYNAMIC(CConnectUserDlg, CDialog)
CConnectUserDlg::CConnectUserDlg(CPlazaService *pLogonService,CWnd* pParent /*=NULL*/)
	: CDialog(CConnectUserDlg::IDD, pParent)
{
	m_pLogonService = pLogonService;
}

CConnectUserDlg::~CConnectUserDlg()
{
}

void CConnectUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConnectUserDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRIDCONNECTUSER, OnGridClick)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CConnectUserDlg ��Ϣ�������
//��ʼ������
BOOL CConnectUserDlg::OnInitDialog()
{
	__super::OnInitDialog();

	CRect rc;
	GetClientRect(&rc);
	rc.InflateRect(-12, -40);
	rc.top = 10;

	m_gridGameService.Create(CRect(rc), this, IDC_GRIDCONNECTUSER);
	m_gridGameService.SetEditable(FALSE);
	m_gridGameService.SetSingleRowSelection(TRUE);
	m_gridGameService.SetFixedRowSelection(FALSE);
	m_gridGameService.SetListMode(TRUE);
	m_BtnDataBase.SetGrid( &m_gridGameService);

	try 
	{
		m_gridGameService.SetRowCount(1);
		m_gridGameService.SetColumnCount(6);
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
	m_gridGameService.SetItemText(0, c, TEXT("�ʺ�"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, rc.Width() * 0.26);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("����"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, rc.Width() * 0.15);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("����"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, rc.Width() * 0.08);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("���ö��"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, rc.Width() * 0.15);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("IP��ַ"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, rc.Width() * 0.15);
	c ++;

	m_gridGameService.SetItemText(0, c, TEXT("����"));
	m_gridGameService.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	m_gridGameService.SetColumnWidth(c, rc.Width() * 0.15);
	c ++;

	m_gridGameService.SetRowHeight(0, 32);
	m_gridGameService.SetDefCellHeight(24);

	UpdateConnectUser();

	return TRUE;
}
void	CConnectUserDlg::UpdateConnectUser()
{
	 CEnumUserSink *pEnumUserSink = new CEnumUserSink(this);
	 IUnknownEx * pIUnknownEx =GET_OBJECTPTR_INTERFACE(pEnumUserSink,IUnknownEx);
	 m_pLogonService->EnumConnectUser(pIUnknownEx);
	 delete pEnumUserSink;
}
LPCSTR GetUserTypeName(const BYTE&   cbUserType)
{
	switch(cbUserType) 
	{
	case USERTYPE_GAME:
		return TEXT("��ͨ�ʺ�");
	case USERTYPE_SUB:
		return TEXT("���ʺ�");
	case USERTYPE_DEALER:
		return TEXT("Dealer�ʺ�");
	default:
		break;
	}
	return "";
}
LPCSTR GetUserLevelName(const BYTE&   cbUserLevel)
{
	switch(cbUserLevel) 
	{
	case 0:
		return TEXT("��˾");
	case 1:
		return TEXT("��ɶ�");
	case 2:
		return TEXT("�ɶ�");
	case 3:
		return TEXT("�ܴ�");
	case 4:
		return TEXT("����");
	case 5:
		return TEXT("��Ա");
	default:
		break;
	}
	return "";
}
void __cdecl CConnectUserDlg::OnUserData(const DWORD&	dwRealUserID,
						const BYTE&   cbUserType,
						const tagUserData& UserData,
						const DWORD&	dwClientIP)
{
	CString strTmp;
	int r = m_gridGameService.GetRowCount();
	m_gridGameService.SetRowCount(r + 1);

	strTmp.Format(TEXT("%s (%s)"),UserData.szAccount,GetUserTypeName(cbUserType));
	m_gridGameService.SetItemText(r, 0, strTmp);
	m_gridGameService.SetItemText(r, 1, UserData.szName);
	m_gridGameService.SetItemText(r, 2, GetUserLevelName(UserData.cbLevel));
	strTmp.Format(TEXT("%.2f"),UserData.UserScore.fScore);
	m_gridGameService.SetItemText(r, 3, strTmp);
	struct in_addr in;
	in.S_un.S_addr = dwClientIP;
	m_gridGameService.SetItemText(r, 4, inet_ntoa(in));
	if (m_gridGameService.SetCellType(r,5, RUNTIME_CLASS(CGridBtnCell)))
	{
		CGridBtnCell *pGridBtnCell = (CGridBtnCell*) m_gridGameService.GetCell(r,5);
		pGridBtnCell->SetBtnDataBase( &m_BtnDataBase);

		pGridBtnCell->SetupBtns(
			0,        
			DFC_BUTTON, 
			DFCS_BUTTONPUSH, 
			(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
			0,  
			0,  
			TEXT("�ǳ�"));
	}

	tagConnectUser *pConnectUser = new tagConnectUser;
	pConnectUser->cbUserType = cbUserType;
	pConnectUser->dwRealUserID = dwRealUserID;
	m_gridGameService.SetItemData(r, 0, (LPARAM)pConnectUser);
}
void CConnectUserDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
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

	case 5: 
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
				OnKickConectUser(nRow);
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
//�ǳ��û�
void CConnectUserDlg::OnKickConectUser(int nRow)
{
	tagConnectUser *pConnectUser = 
		(tagConnectUser*)m_gridGameService.GetItemData(nRow, 0);
	if(pConnectUser)
	{
		if(m_pLogonService->SendUserMessge(pConnectUser->dwRealUserID, pConnectUser->cbUserType, true))
		{
			delete pConnectUser;
			m_gridGameService.DeleteRow(nRow);
			m_gridGameService.Refresh();
		}
	}
}
void CConnectUserDlg::OnDestroy()
{
	CDialog::OnDestroy();

	int r = m_gridGameService.GetRowCount();
	for(int i = 1; i < r; i ++)
	{
		tagConnectUser *pConnectUser = 
			(tagConnectUser*)m_gridGameService.GetItemData(i, 0);
		if(pConnectUser)
		{
			delete pConnectUser;
		}
	}
}
