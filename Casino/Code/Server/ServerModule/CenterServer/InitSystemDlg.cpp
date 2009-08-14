// InitSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "InitSystemDlg.h"
#include "GameTypeInfoDlg.h"
#include "GameKindInfoDlg.h"
#include "GameServerInfoDlg.h"
#include ".\VideoGameTableInfoDlg.h"
#include "VideoServerInfoDlg.h"
#include "math.h"
#include ".\initsystemdlg.h"
#include "../../ShareModule/GameServiceParamFile/GameServiceParamFile.h"

// CInitSystemDlg 对话框

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

IMPLEMENT_DYNAMIC(CInitSystemDlg, CDialog)
CInitSystemDlg::CInitSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitSystemDlg::IDD, pParent)
{
}

CInitSystemDlg::~CInitSystemDlg()
{
}

void CInitSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerItemView);
}


BEGIN_MESSAGE_MAP(CInitSystemDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD_TYPE, OnBnClickedAddType)
	ON_BN_CLICKED(IDC_ADD_KIND, OnBnClickedAddKind)
	ON_BN_CLICKED(IDC_ADD_SERVER, OnBnClickedAddServer)
	ON_BN_CLICKED(IDC_DEL_NODE, OnBnClickedDelNode)
	ON_BN_CLICKED(IDC_OPTION_NODE, OnBnClickedOptionNode)
	ON_BN_CLICKED(IDC_REFRESH_NODE, OnBnClickedRefreshNode)
	ON_BN_CLICKED(IDC_ADDVIDEOGAMETABLE, OnBnClickedAddvideogametable)
	ON_BN_CLICKED(IDC_DELVIDEOGAMETABLE, OnBnClickedDelvideogametable)
	ON_BN_CLICKED(IDC_CONFIGVIDEOGAMETABLE, OnBnClickedConfigvideogametable)
	ON_NOTIFY(NM_CLICK, IDC_VIDEOGAMETABILEGRID, OnGridClick)
	ON_BN_CLICKED(IDC_GENGAMESERVICEPARAMFILE, OnBnClickedGengameserviceparamfile)
	ON_BN_CLICKED(IDC_SAVEGAMEMAININFO, OnBnClickedSaveGameMianInfo)
	ON_BN_CLICKED(IDC_SAVE_NOTIFY_MAN, OnBnClickedSaveNotifyMan)
	ON_BN_CLICKED(IDC_SAVE_NOTIFY_GAME, OnBnClickedSaveNotifyGame)
END_MESSAGE_MAP()


// CInitSystemDlg 消息处理程序

void CInitSystemDlg::OnBnClickedCancel()
{
	m_ServerListCenter->ResetServerList();
	ClearVideoGameTabeList();
	OnCancel();
}

void CInitSystemDlg::OnBnClickedOk()
{
	m_ServerListCenter->ResetServerList();
	ClearVideoGameTabeList();
	OnOK();
}

BOOL CInitSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDIT_NOTIFY_GAME))->LimitText(510);
	((CEdit*)GetDlgItem(IDC_EDIT_NOTIFY_MAN))->LimitText(510);

	CRect tmpRc;	
	int x;
	int y;
	CString strLoadString;
	int width = 0;
	GetDlgItem(IDC_STATIC_VIDEOGAME)->GetWindowRect(&tmpRc);
	x = 650;
	y = 15;
	tmpRc.InflateRect(-10,-10);
	tmpRc.bottom -= 26;
	CRect rc(x, y, x + tmpRc.Width(), y + tmpRc.Height());
	m_VideoGameTableGrid.Create(rc, this, IDC_VIDEOGAMETABILEGRID);
	m_VideoGameTableGrid.SetSingleRowSelection(TRUE);
	m_VideoGameTableGrid.SetFixedRowSelection(FALSE);
	m_VideoGameTableGrid.SetListMode(TRUE);
	m_VideoGameTableGrid.SetEditable(FALSE);
	m_VideoGameTableGrid.SetRowCount(1);
	m_VideoGameTableGrid.SetColumnCount(5);
	m_VideoGameTableGrid.SetFixedRowCount(1);
	m_VideoGameTableGrid.SetFixedColumnCount(0);
	for(int c = 0; c < 5; c ++)
	{
		switch(c)
		{
		case 0:
			width = 50;
			strLoadString = "桌号";
			break;
		case 1:
			width = 50;
			strLoadString = "限紅";
			break;
		case 2:
			width = 70;
			strLoadString = "上下限";
			break;
		case 3:
			width = 100;
			strLoadString = "状态服务器";
			break;
		case 4:
			width = 70;
			strLoadString = "视频列表";
			break;
		default:
			break;
		}
		m_VideoGameTableGrid.SetItemText(0, c, strLoadString);
		m_VideoGameTableGrid.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
		m_VideoGameTableGrid.SetColumnWidth(c, width);
	}
	m_VideoGameTableGrid.SetRowHeight(0, 30);
	m_VideoGameTableGrid.SetDefCellHeight(25);
	m_BtnDataBase.SetGrid( &m_VideoGameTableGrid);

	EnableVideoGameTableOptionPane(FALSE);

	UpdateServerList();
	LoadGameMainInfo();
	LoadNotice();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void	CInitSystemDlg::EnableVideoGameTableOptionPane(BOOL bEnable)
{
	m_VideoGameTableGrid.EnableWindow(bEnable);
	GetDlgItem(IDC_ADDVIDEOGAMETABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_DELVIDEOGAMETABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CONFIGVIDEOGAMETABLE)->EnableWindow(bEnable);
}
bool	CInitSystemDlg::InitSystem(IEventService* pIEventService)
{
	m_pIEventService = pIEventService;

	//效验参数
	GT_ASSERT(m_pIEventService!=NULL);

	//创建组件
	if (m_ServerListCenter.GetInterface()==NULL)
	{
		if (m_ServerListCenter.CreateInstance()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("中心服务器列表组件创建失败"),
				Level_Exception);
			return false;
		}
		m_ServerListCenter->SetEventService(m_pIEventService);
	}

	m_InitParamter.LoadInitParamter();
	//加载列表
	tagDataBaseInfo DataBaseInfo;
	DataBaseInfo.wDataBasePort=m_InitParamter.m_wServerDataBasePort;
	DataBaseInfo.dwDataBaseAddr=inet_addr(m_InitParamter.m_szServerDataBaseAddr);
	lstrcpyn(DataBaseInfo.szDataBasePipeName,m_InitParamter.m_szServerDataBasePipeName,CountArray(DataBaseInfo.szDataBasePipeName));
	lstrcpyn(DataBaseInfo.szDataBaseUser,m_InitParamter.m_szServerDataBaseUser,CountArray(DataBaseInfo.szDataBaseUser));
	lstrcpyn(DataBaseInfo.szDataBasePass,m_InitParamter.m_szServerDataBasePass,CountArray(DataBaseInfo.szDataBasePass));
	lstrcpyn(DataBaseInfo.szDataBaseName,m_InitParamter.m_szServerDataBaseName,CountArray(DataBaseInfo.szDataBaseName));
	if (m_ServerListCenter->LoadAllServerList(DataBaseInfo)==false) 
	{
		m_pIEventService->ShowEventNotify(TEXT("游戏列表加载失败"),Level_Exception);
		return false;
	}
	return true;
}
void	CInitSystemDlg::UpdateServerList()
{
	m_ServerItemView.DeleteAllItems();
	m_ServerListManager.ClearAllItem();
	//设置控件
	m_ServerItemView.InitServerItemView(this);
	m_ServerListManager.InitServerListManager(&m_ServerItemView);

	//效验参数
	GT_ASSERT(m_pIEventService!=NULL);

	//创建组件
	if (m_ServerListCenter.GetInterface()==NULL)
	{
		if (m_ServerListCenter.CreateInstance()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("中心服务器列表组件创建失败"),
				Level_Exception);
			return ;
		}
		m_ServerListCenter->SetEventService(m_pIEventService);
	}
	m_ServerListCenter->ResetServerList();

	CInitParamter m_InitParamter;
	m_InitParamter.LoadInitParamter();
	//加载列表
	tagDataBaseInfo DataBaseInfo;
	DataBaseInfo.wDataBasePort=m_InitParamter.m_wServerDataBasePort;
	DataBaseInfo.dwDataBaseAddr=inet_addr(m_InitParamter.m_szServerDataBaseAddr);
	lstrcpyn(DataBaseInfo.szDataBasePipeName,m_InitParamter.m_szServerDataBasePipeName,CountArray(DataBaseInfo.szDataBasePipeName));
	lstrcpyn(DataBaseInfo.szDataBaseUser,m_InitParamter.m_szServerDataBaseUser,CountArray(DataBaseInfo.szDataBaseUser));
	lstrcpyn(DataBaseInfo.szDataBasePass,m_InitParamter.m_szServerDataBasePass,CountArray(DataBaseInfo.szDataBasePass));
	lstrcpyn(DataBaseInfo.szDataBaseName,m_InitParamter.m_szServerDataBaseName,CountArray(DataBaseInfo.szDataBaseName));
	if (m_ServerListCenter->LoadAllServerList(DataBaseInfo)==false) 
	{
		m_pIEventService->ShowEventNotify(TEXT("游戏列表加载失败"),Level_Exception);
		return ;
	}


	//列表信息
	CMD_CS_ListInfo ListInfo;
	memset(&ListInfo,0,sizeof(ListInfo));
	ListInfo.dwTypeCount=m_ServerListCenter->GetGameTypeCount();
	ListInfo.dwKindCount=m_ServerListCenter->GetGameKindCount();
	ListInfo.dwServerCount=m_ServerListCenter->GetGameServerCount();
	ListInfo.dwStationCount=m_ServerListCenter->GetGameStationCount();
	OnMainServerList(SUB_CS_LIST_INFO,&ListInfo,sizeof(ListInfo));

	//类型信息
	POSITION Pos=NULL;
	WORD wSendSize=0L;
	BYTE cbBuffer[SOCKET_PACKAGE];
	for (DWORD i=0;i<ListInfo.dwTypeCount;i++)
	{
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbBuffer))
		{
			OnMainServerList(SUB_CS_LIST_TYPE,cbBuffer,wSendSize);
			wSendSize=0;
		}
		tagGameTypeItem * pGameTypeItem=m_ServerListCenter->EmunGameTypeItem(Pos);
		CopyMemory(cbBuffer+wSendSize,&pGameTypeItem->GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}
	if (wSendSize>0) OnMainServerList(SUB_CS_LIST_TYPE,cbBuffer,wSendSize);

	//种类信息
	Pos=NULL;
	wSendSize=0L;
	for (DWORD i=0;i<ListInfo.dwKindCount;i++)
	{
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbBuffer))
		{
			OnMainServerList(SUB_CS_LIST_KIND,cbBuffer,wSendSize);
			wSendSize=0;
		}
		tagGameKindItem * pGameKindItem=m_ServerListCenter->EmunGameKindItem(Pos);
		CopyMemory(cbBuffer+wSendSize,&pGameKindItem->GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}
	if (wSendSize>0) OnMainServerList(SUB_CS_LIST_KIND,cbBuffer,wSendSize);

	//站点信息
	Pos=NULL;
	wSendSize=0L;
	for (DWORD i=0;i<ListInfo.dwStationCount;i++)
	{
		if ((wSendSize+sizeof(tagGameStation))>sizeof(cbBuffer))
		{
			OnMainServerList(SUB_CS_LIST_STATION,cbBuffer,wSendSize);
			wSendSize=0;
		}
		tagGameStationItem * pGameStationItem=m_ServerListCenter->EmunGameStationItem(Pos);
		CopyMemory(cbBuffer+wSendSize,&pGameStationItem->GameStation,sizeof(tagGameStation));
		wSendSize+=sizeof(tagGameStation);
	}
	if (wSendSize>0) OnMainServerList(SUB_CS_LIST_STATION,cbBuffer,wSendSize);

	//房间信息
	Pos=NULL;
	wSendSize=0L;
	for (DWORD i=0;i<ListInfo.dwServerCount;i++)
	{
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbBuffer))
		{
			OnMainServerList(SUB_CS_LIST_SERVER,cbBuffer,wSendSize);
			wSendSize=0;
		}
		tagGameServerItem * pGameServerItem=m_ServerListCenter->EmunGameServerItem(Pos);
		CopyMemory(cbBuffer+wSendSize,&pGameServerItem->GameServer,sizeof(tagGameServer));
		wSendSize+=sizeof(tagGameServer);
	}
	if (wSendSize>0) OnMainServerList(SUB_CS_LIST_SERVER,cbBuffer,wSendSize);

	OnMainServerList(SUB_CS_LIST_FINISH,0,0);

}
//左键单击
 void __cdecl CInitSystemDlg::OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
 {
	 UpdateVideoGameTabeList();
	
 }

//右键单击
 void __cdecl CInitSystemDlg::OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
 {
 }
//左键双击
 void __cdecl CInitSystemDlg::OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
 {
 }
//右键双击
 void __cdecl CInitSystemDlg::OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
 {
 }
//选择改变
 void __cdecl CInitSystemDlg::OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
 {
 }
//子项展开
 void __cdecl CInitSystemDlg::OnTreeItemexpanded(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
 {
 }
 //游戏列表消息
 bool CInitSystemDlg::OnMainServerList(WORD wSubCmdID, 
	 void * pBuffer, 
	 WORD wDataSize)
 {

	 switch (wSubCmdID)
	 {
		 //类型信息
	 case SUB_CS_LIST_TYPE:			
		 {
			 //效验参数
			 GT_ASSERT(wDataSize%sizeof(tagGameType)==0);
			 if (wDataSize%sizeof(tagGameType)!=0) return false;

			 //处理消息
			 tagGameType * pGameType=(tagGameType *)pBuffer;
			 WORD wItemCount=wDataSize/sizeof(tagGameType);
			m_ServerListManager.InsertTypeItem(pGameType,wItemCount);

			 return true;
		 }

		 //种类消息
	 case SUB_CS_LIST_KIND:			
		 {
			 //效验参数
			 GT_ASSERT(wDataSize%sizeof(tagGameKind)==0);
			 if (wDataSize%sizeof(tagGameKind)!=0) return false;

			 //处理消息
			 tagGameKind * pGameKind=(tagGameKind *)pBuffer;
			 WORD wItemCount=wDataSize/sizeof(tagGameKind);
			m_ServerListManager.InsertKindItem(pGameKind,wItemCount);

			 return true;
		 }


		 //站点消息
	 case SUB_CS_LIST_STATION:		
		 {
			 //效验参数
			 GT_ASSERT(wDataSize%sizeof(tagGameStation)==0);
			 if (wDataSize%sizeof(tagGameStation)!=0) return false;

			 //处理消息
			 tagGameStation * pGameStation=(tagGameStation *)pBuffer;
			 WORD wItemCount=wDataSize/sizeof(tagGameStation);
			m_ServerListManager.InsertStationItem(pGameStation,wItemCount);

			 return true;
		 }

		 //服务器房间
	 case SUB_CS_LIST_SERVER:		
		 {
			 //效验参数
			 GT_ASSERT(wDataSize%sizeof(tagGameServer)==0);
			 if (wDataSize%sizeof(tagGameServer)!=0) return false;

			 //处理消息
			 tagGameServer * pGameServer=(tagGameServer *)pBuffer;
			 WORD wItemCount=wDataSize/sizeof(tagGameServer);
			m_ServerListManager.InsertServerItem(pGameServer,wItemCount);

			 return true;
		 }

		 //列表发送完成
	 case SUB_CS_LIST_FINISH:		
		 {
			 INT_PTR nIndex=0;
			

			//展开类型
			 nIndex=0;
			CListType * pListType=NULL;
			do
			{
				pListType=m_ServerListManager.EnumTypeItem(nIndex++);
				if (pListType==NULL) break;
				m_ServerListManager.ExpandListItem(pListType);
			} while (true);

			nIndex=0;
			CListKind * pListKind=NULL;
			do
			{
				pListKind=m_ServerListManager.EnumKindItem(nIndex++);
				if (pListKind==NULL) break;
				m_ServerListManager.ExpandListItem(pListKind);
			} while (true);



			//展开列表
			nIndex=0;
			CListInside * pListInside=NULL;
			do
			{
				pListInside=m_ServerListManager.EnumInsideItem(nIndex++);
				if (pListInside==NULL) break;
				m_ServerListManager.ExpandListItem(pListInside);
			} while (true);
			 return true;
		 }


	 }

	 return true;
 }
void CInitSystemDlg::OnBnClickedAddType()
{
	CGameTypeInfoDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_strTypeName.IsEmpty())
			return;
		if(InsertGameType(dlg.m_nImageID,
			(LPCTSTR)(dlg.m_strTypeName),
			dlg.m_dwSortID)< 0)
		{
			AfxMessageBox(TEXT("操作失败"));
		}
		UpdateServerList();
	}
}

void CInitSystemDlg::OnBnClickedAddKind()
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
	if (hTreeItem!=NULL)
	{
		pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
	}
	if(pListItem == NULL || pListItem->GetItemGenre() != ItemGenre_Type)
	{
		AfxMessageBox(TEXT("请选择正确游戏类"));
		return;
	}
	GT_ASSERT(((CListType*)pListItem)->GetItemInfo());
	int nTypeID = ((CListType*)pListItem)->GetItemInfo()->wTypeID;


	CGameKindInfoDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_strKindName.IsEmpty())
			return;
		if(dlg.m_dwTaxRate > 1000)
			return;
		int nKindID;
		if((nKindID = InsertGameKind(nTypeID,
			(LPCTSTR)(dlg.m_strKindName),
			dlg.m_dwSortID,
			dlg.m_dwGameProcessType,
			0,
			dlg.m_dwTableCount,
			dlg.m_dwCellScore,
			dlg.m_dwHighScore,
			dlg.m_dwLessScore,
			((double)dlg.m_dwTaxRate / 1000.0),
			dlg.m_dwAIUserCount,
			dlg.m_dwAILevel))< 0)
		{
			AfxMessageBox(TEXT("操作失败"));
		}
		else
		{
			if(IsVideoGameProcessType(dlg.m_dwGameProcessType))
			{
				for(int i = 0; i < dlg.m_dwTableCount; i ++)
				{
					tagVideoGameTableParam param;
					param.dwHighScore				= dlg.m_dwHighScore;
					param.dwCellScore				= dlg.m_dwCellScore;
					param.fMaxUserRevenueSharing	= 200000;
					param.dwStateKernelServerAddr	= inet_addr("127.0.0.1");
					param.wStateKernelServerPort	= 9200;

					if(InsertGameTable(nKindID, nTypeID,
						i,
						(BYTE*)&param,sizeof(param))<0)
					{
						AfxMessageBox(TEXT("新增游戏桌失败"));
					}
				}
			}
			UpdateServerList();
		}
	}
}

void CInitSystemDlg::OnBnClickedAddServer()
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
	if (hTreeItem!=NULL)
	{
		pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
	}
	if(pListItem == NULL || pListItem->GetItemGenre() != ItemGenre_Kind)
	{
		AfxMessageBox(TEXT("请选择正确游戏"));
		return;
	}
	CListItem * pParentListItem=pListItem->GetParentItem();
	if(pParentListItem == NULL || pParentListItem->GetItemGenre() != ItemGenre_Type)
	{
		AfxMessageBox(TEXT("请选择正确游戏"));
		return;
	}

	GT_ASSERT(((CListKind*)pListItem)->GetItemInfo());
	int nKindID = ((CListKind*)pListItem)->GetItemInfo()->wKindID;

	GT_ASSERT(((CListType*)pParentListItem)->GetItemInfo());
	int nTypeID = ((CListType*)pParentListItem)->GetItemInfo()->wTypeID;

	CGameServerInfoDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_strServerName.IsEmpty())
			return;

		if(InsertGameServer(nKindID,
			nTypeID,
			(LPCTSTR)(dlg.m_strServerName),
			dlg.m_dwSortID,
			dlg.m_dwMaxUserCount)< 0)
		{
			AfxMessageBox(TEXT("操作失败"));
		}
		UpdateServerList();
	}
}

void CInitSystemDlg::OnBnClickedDelNode()
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
	if (hTreeItem!=NULL)
	{
		pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
	}
	if(pListItem == NULL )
	{
		return;
	}
	if(AfxMessageBox(TEXT("确定删除该节点吗?"), MB_YESNO) != IDYES)
		return;
	if(pListItem->GetItemGenre() == ItemGenre_Type)
	{
		GT_ASSERT(((CListType*)pListItem)->GetItemInfo());
		int nTypeID = ((CListType*)pListItem)->GetItemInfo()->wTypeID;
		DeleteGameType(nTypeID);
		UpdateServerList();
	}
	else if(pListItem->GetItemGenre() == ItemGenre_Kind)
	{
		GT_ASSERT(((CListKind*)pListItem)->GetItemInfo());
		int nKindID = ((CListKind*)pListItem)->GetItemInfo()->wKindID;
		DeleteGameKind(nKindID);
		UpdateServerList();
	}
	else if(pListItem->GetItemGenre() == ItemGenre_Server)
	{
		GT_ASSERT(((CListServer*)pListItem)->GetItemInfo());
		int nServerID = ((CListServer*)pListItem)->GetItemInfo()->wServerID;
		DeleteGameServer(nServerID);
		UpdateServerList();
	}
}

void CInitSystemDlg::OnBnClickedOptionNode()
{
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
	if (hTreeItem!=NULL)
	{
		pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
	}
	if(pListItem == NULL )
	{
		return;
	}
	
	if(pListItem->GetItemGenre() == ItemGenre_Type)
	{
		GT_ASSERT(((CListType*)pListItem)->GetItemInfo());
		int nTypeID = ((CListType*)pListItem)->GetItemInfo()->wTypeID;
		CGameTypeInfoDlg dlg;
		dlg.m_dwSortID = ((CListType*)pListItem)->GetItemInfo()->wSortID;
		dlg.m_nImageID = ((CListType*)pListItem)->GetItemInfo()->nbImageID;
		dlg.m_strTypeName=((CListType*)pListItem)->GetItemInfo()->szTypeName;
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_strTypeName.IsEmpty())
				return;
			if(UpdateGameType(nTypeID,
				dlg.m_nImageID,
				(LPCTSTR)(dlg.m_strTypeName),
				dlg.m_dwSortID)< 0)
			{
				AfxMessageBox(TEXT("操作失败"));
			}
			UpdateServerList();
		}
		
	}
	else if(pListItem->GetItemGenre() == ItemGenre_Kind)
	{
		GT_ASSERT(((CListKind*)pListItem)->GetItemInfo());
		int nTypeID = ((CListKind*)pListItem)->GetItemInfo()->wTypeID;
		int nKindID = ((CListKind*)pListItem)->GetItemInfo()->wKindID;
		CGameKindInfoDlg dlg(FALSE);
		dlg.m_strKindName = ((CListKind*)pListItem)->GetItemInfo()->szKindName;
		dlg.m_dwSortID = ((CListKind*)pListItem)->GetItemInfo()->wSortID;
		dlg.m_dwGameProcessType = ((CListKind*)pListItem)->GetItemInfo()->wProcessType;
		dlg.m_dwTableCount = ((CListKind*)pListItem)->GetItemInfo()->wTableCount;
		dlg.m_dwCellScore = ((CListKind*)pListItem)->GetItemInfo()->dwCellScore;
		dlg.m_dwHighScore = ((CListKind*)pListItem)->GetItemInfo()->dwHighScore;
		dlg.m_dwLessScore = ((CListKind*)pListItem)->GetItemInfo()->dwLessScore;
		dlg.m_dwTaxRate = ceil(((CListKind*)pListItem)->GetItemInfo()->fTaxRate * 1000.0);
		dlg.m_dwAIUserCount = ((CListKind*)pListItem)->GetItemInfo()->wAIUserCount;
		dlg.m_dwAILevel		 = ((CListKind*)pListItem)->GetItemInfo()->cbAILevel;
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_strKindName.IsEmpty())
				return;
			if(dlg.m_dwTaxRate > 1000)
				return;
			if(UpdateGameKind(nKindID,
				nTypeID,
				(LPCTSTR)(dlg.m_strKindName),
				dlg.m_dwSortID,
				dlg.m_dwGameProcessType,
				0,
				dlg.m_dwTableCount,
				dlg.m_dwCellScore,
				dlg.m_dwHighScore,
				dlg.m_dwLessScore,
				((double)dlg.m_dwTaxRate / 1000.0),
				dlg.m_dwAIUserCount,
				dlg.m_dwAILevel)< 0)
			{
			}
			else
				UpdateServerList();
		}
	}
	else if(pListItem->GetItemGenre() == ItemGenre_Server)
	{
		GT_ASSERT(((CListServer*)pListItem)->GetItemInfo());
		int nServerID = ((CListServer*)pListItem)->GetItemInfo()->wServerID;
		int nTypeID = ((CListServer*)pListItem)->GetItemInfo()->wTypeID;
		int nKindID = ((CListServer*)pListItem)->GetItemInfo()->wKindID;
		CGameServerInfoDlg dlg;
		dlg.m_strServerName = ((CListServer*)pListItem)->GetItemInfo()->szServerName;
		dlg.m_dwSortID = ((CListServer*)pListItem)->GetItemInfo()->wSortID;
		dlg.m_dwMaxUserCount = ((CListServer*)pListItem)->GetItemInfo()->wMaxConnect;
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_strServerName.IsEmpty())
				return;

			if(UpdateGameServer(nServerID,
				nKindID,
				nTypeID,
				(LPCTSTR)(dlg.m_strServerName),
				dlg.m_dwSortID,
				dlg.m_dwMaxUserCount)< 0)
			{
				AfxMessageBox(TEXT("操作失败"));
			}
			UpdateServerList();
		}
	}
}

void CInitSystemDlg::OnBnClickedRefreshNode()
{
	UpdateServerList();
}
void CInitSystemDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (pItem->iRow < 1)
		return;


	int nRow = pItem->iRow;
	int nCol = pItem->iColumn;

	*pResult = 0;

	if( nRow < m_VideoGameTableGrid.GetFixedRowCount() )
		return;

	CGridBtnCell* pGridBtnCell;

	switch( nCol)
	{

	case 4:  
		pGridBtnCell = (CGridBtnCell*) m_VideoGameTableGrid.GetCell( nRow, nCol);
		if( pGridBtnCell)
		{
			if( !pGridBtnCell->IsKindOf( RUNTIME_CLASS(CGridBtnCell) ) )
			{
				return;
			}
			switch(pGridBtnCell->GetLastCtlClicked())
			{
			case 0:
				OnConfigVideoGameServer(nRow);
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
void CInitSystemDlg::OnConfigVideoGameServer(int nRow)
{
	tagGameTable* pParam = (tagGameTable*)m_VideoGameTableGrid.GetItemData(nRow, 0);
	if(pParam)
	{
		CVideoServerInfoDlg dlg(pParam->wTableID);
		dlg.DoModal();
	}
}
void CInitSystemDlg::OnBnClickedAddvideogametable()
{
	//变量定义
	CListItem * pListItem=NULL;
	HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
	if (hTreeItem!=NULL)
	{
		pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
	}
	if(pListItem == NULL || pListItem->GetItemGenre() != ItemGenre_Kind)
	{
		AfxMessageBox(TEXT("请选择正确游戏"));
		return;
	}
	CListItem * pParentListItem=pListItem->GetParentItem();
	if(pParentListItem == NULL || pParentListItem->GetItemGenre() != ItemGenre_Type)
	{
		AfxMessageBox(TEXT("请选择正确游戏"));
		return;
	}

	GT_ASSERT(((CListKind*)pListItem)->GetItemInfo());
	int nKindID = ((CListKind*)pListItem)->GetItemInfo()->wKindID;

	GT_ASSERT(((CListType*)pParentListItem)->GetItemInfo());
	int nTypeID = ((CListType*)pParentListItem)->GetItemInfo()->wTypeID;


	CVideoGameTableInfoDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		for(int i = 1; i < m_VideoGameTableGrid.GetRowCount(); i ++)
		{
			tagGameTable* pParam = (tagGameTable*)m_VideoGameTableGrid.GetItemData(i, 0);
			if(pParam && pParam->wTableNumber == dlg.m_dwTableNumber)
			{
				AfxMessageBox(TEXT("该游戏桌号码已经存在"));
				return ;
			}
		}

		tagVideoGameTableParam param;
		param.dwHighScore				= dlg.m_dwHighScore;
		param.dwCellScore				= dlg.m_dwCellScore;
		param.fMaxUserRevenueSharing	= dlg.m_ullMaxUserRevenseSharing;
		param.dwStateKernelServerAddr	= ntohl(dlg.m_dwStateKernelServerAddr);
		param.wStateKernelServerPort	= dlg.m_dwStateKernelServerPort;

		if(InsertGameTable(nKindID, nTypeID,
			dlg.m_dwTableNumber,
			(BYTE*)&param,sizeof(param))<0)
		{
			AfxMessageBox(TEXT("新增游戏桌失败"));
		}

		UpdateVideoGameTabeList();
	}
}

void CInitSystemDlg::OnBnClickedDelvideogametable()
{
	CCellRange cr = m_VideoGameTableGrid.GetSelectedCellRange();
	if(cr.GetMinRow() > 0)
	{
		tagGameTable* pParam = (tagGameTable*)m_VideoGameTableGrid.GetItemData(cr.GetMinRow(), 0);
		if(pParam)
		{
			if(AfxMessageBox(TEXT("确定删除该游戏桌?"), MB_YESNO) == IDYES)
			{
				DeleteGameTable(pParam->wTableID);
				UpdateVideoGameTabeList();
			}
		}
	}
}

void CInitSystemDlg::OnBnClickedConfigvideogametable()
{
	CCellRange cr = m_VideoGameTableGrid.GetSelectedCellRange();
	if(cr.GetMinRow() > 0)
	{
		tagGameTable* pParam = (tagGameTable*)m_VideoGameTableGrid.GetItemData(cr.GetMinRow(), 0);
		if(pParam)
		{
			//变量定义
			CListItem * pListItem=NULL;
			HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
			if (hTreeItem!=NULL)
			{
				pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
			}
			if(pListItem == NULL || pListItem->GetItemGenre() != ItemGenre_Kind)
			{
				AfxMessageBox(TEXT("请选择正确游戏"));
				return;
			}
			CListItem * pParentListItem=pListItem->GetParentItem();
			if(pParentListItem == NULL || pParentListItem->GetItemGenre() != ItemGenre_Type)
			{
				AfxMessageBox(TEXT("请选择正确游戏"));
				return;
			}

			GT_ASSERT(((CListKind*)pListItem)->GetItemInfo());
			int nKindID = ((CListKind*)pListItem)->GetItemInfo()->wKindID;

			GT_ASSERT(((CListType*)pParentListItem)->GetItemInfo());
			int nTypeID = ((CListType*)pParentListItem)->GetItemInfo()->wTypeID;
			
			tagVideoGameTableParam *ptagVideoGameTableParam = (tagVideoGameTableParam*)pParam->nbTalbeParam;

			CVideoGameTableInfoDlg dlg;
			dlg.m_dwTableNumber				= pParam->wTableNumber;
			dlg.m_dwHighScore				= ptagVideoGameTableParam->dwHighScore;
			dlg.m_dwCellScore				= ptagVideoGameTableParam->dwCellScore;
			dlg.m_ullMaxUserRevenseSharing	= ptagVideoGameTableParam->fMaxUserRevenueSharing;
			dlg.m_dwStateKernelServerAddr	= ntohl(ptagVideoGameTableParam->dwStateKernelServerAddr);
			dlg.m_dwStateKernelServerPort	= ptagVideoGameTableParam->wStateKernelServerPort;
			if(dlg.DoModal() == IDOK)
			{
				for(int i = 1; i < m_VideoGameTableGrid.GetRowCount(); i ++)
				{
					tagGameTable* pParam1 = (tagGameTable*)m_VideoGameTableGrid.GetItemData(i, 0);
					if(pParam && pParam1->wTableNumber == dlg.m_dwTableNumber &&
						pParam1->wTableID != pParam->wTableID)
					{
						AfxMessageBox(TEXT("该游戏桌号码已经存在"));
						return ;
					}
				}

				tagVideoGameTableParam param;
				param.dwHighScore				= dlg.m_dwHighScore;
				param.dwCellScore				= dlg.m_dwCellScore;
				param.fMaxUserRevenueSharing	= dlg.m_ullMaxUserRevenseSharing;
				param.dwStateKernelServerAddr	= ntohl(dlg.m_dwStateKernelServerAddr);
				param.wStateKernelServerPort	= dlg.m_dwStateKernelServerPort;

				if(UpdateGameTable(pParam->wTableID,
					nKindID, nTypeID,
					dlg.m_dwTableNumber,
					(BYTE*)&param,sizeof(param))<0)
				{
					AfxMessageBox(TEXT("新增游戏桌失败"));
				}

				UpdateVideoGameTabeList();
			}
		}
	}
}
void CInitSystemDlg::UpdateVideoGameTabeList()
{
	ClearVideoGameTabeList();

	CListItem* pListItem =NULL;
	HTREEITEM hTreeItem=m_ServerItemView.GetSelectedItem();
	if (hTreeItem!=NULL)
	{
		pListItem=(CListItem *)m_ServerItemView.GetItemData(hTreeItem);
	}
	BOOL bEnable = FALSE;
	WORD wKindID = -1;
	if(pListItem&&hTreeItem)
	{
		if(pListItem->GetItemGenre() != ItemGenre_Kind)
			bEnable = FALSE;
		else
		{
			CListItem *pParentListItem = pListItem->GetParentItem();
			GT_ASSERT(pParentListItem);
			if(pParentListItem)
			{
				GT_ASSERT(((CListKind*)pListItem)->GetItemInfo());
				WORD wProcessType = ((CListKind*)pListItem)->GetItemInfo()->wProcessType;
				if(IsVideoGameProcessType(wProcessType))
				{
					wKindID = ((CListKind*)pListItem)->GetItemInfo()->wKindID;
					ReadGameTable(wKindID);
					bEnable = TRUE;
				}
			}
		}
	}

	EnableVideoGameTableOptionPane(bEnable);

	m_VideoGameTableGrid.Refresh();
}
int		CInitSystemDlg::InsertGameType(BYTE cbImageID,
									   LPCTSTR strTypeName,
									   int nSortID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_InsertGameType"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@cbImageID"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)cbImageID));
		ServerInfoDB->AddParamter(TEXT("@strTypeName"),adParamInput,adChar,CountString(strTypeName),_variant_t(strTypeName));
		ServerInfoDB->AddParamter(TEXT("@nSortID"),adParamInput,adInteger,sizeof(long),_variant_t(nSortID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::UpdateGameType(int nTypeID,
									   BYTE cbImageID,
									   LPCTSTR strTypeName,
									   int nSortID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_UpdateGameType"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t(nTypeID));
		ServerInfoDB->AddParamter(TEXT("@cbImageID"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)cbImageID));
		ServerInfoDB->AddParamter(TEXT("@strTypeName"),adParamInput,adChar,CountString(strTypeName),_variant_t(strTypeName));
		ServerInfoDB->AddParamter(TEXT("@nSortID"),adParamInput,adInteger,sizeof(long),_variant_t(nSortID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::DeleteGameType(int nTypeID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_DeleteGameType"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t(nTypeID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::InsertGameKind(int nTypeID, 
									   LPCTSTR strKindName,
									   int nSortID,
									   int nProcessType,
									   int nMaxVersion,
									   int nTableCount,
									   int nCellScore,
									   int nHighScore,
									   int nLessScore,
									   double fTaxRate,
									   int nAIUserCount,
									   int nAILevel)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_InsertGameKind"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTypeID));
		ServerInfoDB->AddParamter(TEXT("@strKindName"),adParamInput,adChar,CountString(strKindName),_variant_t(strKindName));
		ServerInfoDB->AddParamter(TEXT("@nSortID"),adParamInput,adInteger,sizeof(long),_variant_t(nSortID));
		ServerInfoDB->AddParamter(TEXT("@nProcessType"),adParamInput,adInteger,sizeof(long),_variant_t(nProcessType));
		ServerInfoDB->AddParamter(TEXT("@MaxVersion"),adParamInput,adInteger,sizeof(long),_variant_t(nMaxVersion));
		ServerInfoDB->AddParamter(TEXT("@nTableCount"),adParamInput,adInteger,sizeof(long),_variant_t(nTableCount));
		ServerInfoDB->AddParamter(TEXT("@nCellScore"),adParamInput,adInteger,sizeof(long),_variant_t(nCellScore));
		ServerInfoDB->AddParamter(TEXT("@nHighScore"),adParamInput,adInteger,sizeof(long),_variant_t(nHighScore));
		ServerInfoDB->AddParamter(TEXT("@nLessScore"),adParamInput,adInteger,sizeof(long),_variant_t(nLessScore));
		ServerInfoDB->AddDecimalParamter(TEXT("@fTaxRate"),adParamInput,fTaxRate);
		ServerInfoDB->AddParamter(TEXT("@wAIUserCount"),adParamInput,adSmallInt,sizeof(WORD),_variant_t((WORD)nAIUserCount));
		ServerInfoDB->AddParamter(TEXT("@cbAILevel"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)nAILevel));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断

		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		if(lReturnCode < 0)
		{
			TCHAR szErrorDescribe[128];
			ServerInfoDB->GetFieldValue(TEXT("ErrorDescribe"),szErrorDescribe,sizeof(szErrorDescribe));
		}

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::UpdateGameKind(int nKindID,
									   int nTypeID, 
									   LPCTSTR strKindName,
									   int nSortID,
									   int nProcessType,
									   int nMaxVersion,
									   int nTableCount,
									   int nCellScore,
									   int nHighScore,
									   int nLessScore,
									   double fTaxRate,
									   int nAIUserCount,
									   int nAILevel)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_UpdateGameKind"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nKindID));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTypeID));
		ServerInfoDB->AddParamter(TEXT("@strKindName"),adParamInput,adChar,CountString(strKindName),_variant_t(strKindName));
		ServerInfoDB->AddParamter(TEXT("@nSortID"),adParamInput,adInteger,sizeof(long),_variant_t(nSortID));
		ServerInfoDB->AddParamter(TEXT("@MaxVersion"),adParamInput,adInteger,sizeof(long),_variant_t(nMaxVersion));
		ServerInfoDB->AddParamter(TEXT("@nTableCount"),adParamInput,adInteger,sizeof(long),_variant_t(nTableCount));
		ServerInfoDB->AddParamter(TEXT("@nCellScore"),adParamInput,adInteger,sizeof(long),_variant_t(nCellScore));
		ServerInfoDB->AddParamter(TEXT("@nHighScore"),adParamInput,adInteger,sizeof(long),_variant_t(nHighScore));
		ServerInfoDB->AddParamter(TEXT("@nLessScore"),adParamInput,adInteger,sizeof(long),_variant_t(nLessScore));
		ServerInfoDB->AddDecimalParamter(TEXT("@fTaxRate"),adParamInput,fTaxRate);
		ServerInfoDB->AddParamter(TEXT("@wAIUserCount"),adParamInput,adSmallInt,sizeof(WORD),_variant_t((WORD)nAIUserCount));
		ServerInfoDB->AddParamter(TEXT("@cbAILevel"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)nAILevel));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断

		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		if(lReturnCode < 0)
		{
			TCHAR szErrorDescribe[128];
			ServerInfoDB->GetFieldValue(TEXT("ErrorDescribe"),szErrorDescribe,sizeof(szErrorDescribe));
		}

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}


int		CInitSystemDlg::DeleteGameKind(int nKindID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_DeleteGameKind"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t(nKindID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}

int		CInitSystemDlg::InsertGameServer(
							int nKindID,
							int nTypeID, 
							LPCTSTR strServerName,
							int nSortID,
							int nMaxUserCount)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_InsertGameServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nKindID));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTypeID));
		ServerInfoDB->AddParamter(TEXT("@strServerName"),adParamInput,adChar,CountString(strServerName),_variant_t(strServerName));
		ServerInfoDB->AddParamter(TEXT("@nSortID"),adParamInput,adInteger,sizeof(long),_variant_t(nSortID));
		ServerInfoDB->AddParamter(TEXT("@nMaxUserCount"),adParamInput,adInteger,sizeof(long),_variant_t((long)nMaxUserCount));
		
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::UpdateGameServer(
	int nServerID,
	int nKindID,
	int nTypeID, 
	LPCTSTR strServerName,
	int nSortID,
	int nMaxUserCount)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_UpdateGameServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nServerID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nServerID));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nKindID));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTypeID));
		ServerInfoDB->AddParamter(TEXT("@strServerName"),adParamInput,adChar,CountString(strServerName),_variant_t(strServerName));
		ServerInfoDB->AddParamter(TEXT("@nSortID"),adParamInput,adInteger,sizeof(long),_variant_t(nSortID));
		ServerInfoDB->AddParamter(TEXT("@nMaxUserCount"),adParamInput,adInteger,sizeof(long),_variant_t((long)nMaxUserCount));

		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::DeleteGameServer(int nServerID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_DeleteGameServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nServerID"),adParamInput,adInteger,sizeof(long),_variant_t(nServerID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::InsertGameTable(int nKindID,
										int nTypeID, 
										int nTableNumber,
										BYTE *pParam,
										int nParamSize)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_InsertGameTable"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nKindID));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTypeID));
		ServerInfoDB->AddParamter(TEXT("@nTableNumber"),adParamInput,adInteger,sizeof(long),_variant_t(nTableNumber));
	
		VARIANT			 varBlob;   
		SAFEARRAY*		 psa = NULL;   
		SAFEARRAYBOUND   safebound[1];         
		if(nParamSize)   
		{   
			safebound[0].lLbound	 =0;   
			safebound[0].cElements   =nParamSize;   
			psa = SafeArrayCreate(VT_UI1,1,safebound); 
			GT_ASSERT(psa);
			for(long i = 0; i < (long)nParamSize; i++)   
				SafeArrayPutElement(psa,&i,pParam++);   
			varBlob.vt		 = VT_ARRAY|VT_UI1;   
			varBlob.parray   = psa;   
		}   
		ServerInfoDB->AddParamter(TEXT("@nbTableParam"),adParamInput,adVarBinary,nParamSize,_variant_t(varBlob));

		ServerInfoDB->ExecuteCommand(true);

		SafeArrayDestroy(psa);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::UpdateGameTable(int nTableID,
										int nKindID,
										int nTypeID, 
										int nTableNumber,
										BYTE *pParam,
										int nParamSize)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_UpdateGameTable"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nTableID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTableID));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nKindID));
		ServerInfoDB->AddParamter(TEXT("@nTypeID"),adParamInput,adInteger,sizeof(long),_variant_t((long)nTypeID));
		ServerInfoDB->AddParamter(TEXT("@nTableNumber"),adParamInput,adInteger,sizeof(long),_variant_t(nTableNumber));

		VARIANT			 varBlob;   
		SAFEARRAY*		 psa = NULL;   
		SAFEARRAYBOUND   safebound[1];         
		if(nParamSize)   
		{   
			safebound[0].lLbound	 =0;   
			safebound[0].cElements   =nParamSize;   
			psa = SafeArrayCreate(VT_UI1,1,safebound); 
			GT_ASSERT(psa);
			for(long i = 0; i < (long)nParamSize; i++)   
				SafeArrayPutElement(psa,&i,pParam++);   
			varBlob.vt		 = VT_ARRAY|VT_UI1;   
			varBlob.parray   = psa;   
		}   
		ServerInfoDB->AddParamter(TEXT("@nbTableParam"),adParamInput,adVarBinary,nParamSize,_variant_t(varBlob));

		ServerInfoDB->ExecuteCommand(true);

		SafeArrayDestroy(psa);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int		CInitSystemDlg::DeleteGameTable(int nTableID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_DeleteGameTable"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nTableID"),adParamInput,adInteger,sizeof(long),_variant_t(nTableID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int CInitSystemDlg::LoadGameMainInfo()
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_LoadGameMainInfo"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		if (ServerInfoDB->IsEndRecordset()==false)
		{
			//读取信息
			TCHAR szTmp[1024];
			ServerInfoDB->GetFieldValue(TEXT("GMMainWebPageAddress"),szTmp, 128);
			SetDlgItemText(IDC_MAINWEBPAGEADDR, szTmp);
			ServerInfoDB->GetFieldValue(TEXT("GMFieldLiveVideoAddress"),szTmp, 64);
			SetDlgItemText(IDC_FIELDLIVEVIDEOADDR, szTmp);
			ServerInfoDB->GetFieldValue(TEXT("GMVideoTapeAddress"),szTmp, 64);
			SetDlgItemText(IDC_VIDEOTAPEADDR, szTmp);

		}
		else
		{
			SetDlgItemText(IDC_MAINWEBPAGEADDR, "http://");
			SetDlgItemText(IDC_FIELDLIVEVIDEOADDR, "rtmp://");
			SetDlgItemText(IDC_VIDEOTAPEADDR, "rtmp://");
		}

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int CInitSystemDlg::LoadNotice()
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_LoadGameMainInfo"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		if (ServerInfoDB->IsEndRecordset()==false)
		{
			//读取信息
			TCHAR szTmp[512];
			ServerInfoDB->GetFieldValue(TEXT("GMCurManagementNotice"),szTmp, 512);
			SetDlgItemText(IDC_EDIT_NOTIFY_MAN, szTmp);
			ServerInfoDB->GetFieldValue(TEXT("GMCurGameNotice"),szTmp, 512);
			SetDlgItemText(IDC_EDIT_NOTIFY_GAME, szTmp);

		}
		else
		{
			
		}

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int CInitSystemDlg::UpdateGameMainInfo(LPCSTR strMainWebPageAddress,
					   LPCSTR strFieldLiveVideoAddress,
					   LPCSTR strVideoTapeAddress)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_UpdateGameMainInfo"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@szMainWebPageAddress"),adParamInput,adChar,CountString(strMainWebPageAddress),_variant_t(strMainWebPageAddress));
		ServerInfoDB->AddParamter(TEXT("@szFieldLiveVideoAddress"),adParamInput,adChar,CountString(strFieldLiveVideoAddress),_variant_t(strFieldLiveVideoAddress));
		ServerInfoDB->AddParamter(TEXT("@szVideoTapeAddress"),adParamInput,adChar,CountString(strVideoTapeAddress),_variant_t(strVideoTapeAddress));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();


		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
int CInitSystemDlg::UpdateNotice(LPCSTR szNotice, __time64_t tmUpdateTime, BYTE cbUpdateFlag)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_UpdateNotice"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@szNotice"),adParamInput,adChar,CountString(szNotice),_variant_t(szNotice));
		ServerInfoDB->AddParamter(TEXT("@ullNoticeValidTimestamp"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t(tmUpdateTime));
		ServerInfoDB->AddParamter(TEXT("@cbUpdateFlag"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t(cbUpdateFlag));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();


		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}
void CInitSystemDlg::ClearVideoGameTabeList()
{
	for(int i = 1; i < m_VideoGameTableGrid.GetRowCount(); i ++)
	{
		tagGameTable* pParam = (tagGameTable*)m_VideoGameTableGrid.GetItemData(i, 0);
		if(pParam)
			delete pParam;
	}
	m_VideoGameTableGrid.SetRowCount(1);
}
int		CInitSystemDlg::ReadGameTable(int wKindID)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		m_pIEventService->ShowEventNotify(TEXT("数据库组件创建失败，无法获取数据库信息"),Level_Exception);
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerDataBaseAddr,
			m_InitParamter.m_szServerDataBasePipeName,
			m_InitParamter.m_wServerDataBasePort,
			m_InitParamter.m_szServerDataBaseName,
			m_InitParamter.m_szServerDataBaseUser,
			m_InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			m_pIEventService->ShowEventNotify(TEXT("信息数据库连接失败，无法获取数据库信息"),Level_Exception);
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_ReadGameTable"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nKindID"),adParamInput,adInteger,sizeof(long),_variant_t(wKindID));
		ServerInfoDB->ExecuteCommand(true);

		//读取信息
		while (ServerInfoDB->IsEndRecordset()==false)
		{
			//读取信息
			tagGameTable* GameTable = new tagGameTable;
			memset(GameTable,0,sizeof(tagGameTable));

			ServerInfoDB->GetFieldValue(TEXT("TableID"),GameTable->wTableID);
			ServerInfoDB->GetFieldValue(TEXT("KindID"),GameTable->wKindID);
			ServerInfoDB->GetFieldValue(TEXT("TypeID"),GameTable->wTypeID);
			ServerInfoDB->GetFieldValue(TEXT("TableNumber"),GameTable->wTableNumber);
			ServerInfoDB->GetFieldValue(TEXT("TableParam"),(TCHAR*)GameTable->nbTalbeParam, 128);

			tagVideoGameTableParam* pVideoGameTableParam = (tagVideoGameTableParam*)GameTable->nbTalbeParam;
			int nRowIndex = m_VideoGameTableGrid.GetRowCount();
			m_VideoGameTableGrid.SetRowCount(nRowIndex + 1);
			CString strTmp;
			strTmp.Format("%d", GameTable->wTableNumber);
			m_VideoGameTableGrid.SetItemText(nRowIndex, 0, strTmp);
			strTmp.Format("%I64d", (ULONGLONG)pVideoGameTableParam->fMaxUserRevenueSharing);
			m_VideoGameTableGrid.SetItemText(nRowIndex, 1, strTmp);
			strTmp.Format("%d/%d", pVideoGameTableParam->dwHighScore,pVideoGameTableParam->dwCellScore);
			m_VideoGameTableGrid.SetItemText(nRowIndex, 2, strTmp);
			struct in_addr in;
			in.S_un.S_addr = pVideoGameTableParam->dwStateKernelServerAddr;
			strTmp.Format("%s:%d", inet_ntoa(in), pVideoGameTableParam->wStateKernelServerPort);
			m_VideoGameTableGrid.SetItemText(nRowIndex, 3, strTmp);

			if (m_VideoGameTableGrid.SetCellType(nRowIndex,4, RUNTIME_CLASS(CGridBtnCell)))
			{
				CGridBtnCell *pGridBtnCell = (CGridBtnCell*) m_VideoGameTableGrid.GetCell(nRowIndex,4);
				pGridBtnCell->SetBtnDataBase( &m_BtnDataBase);

				pGridBtnCell->SetupBtns(
					0,        
					DFC_BUTTON, 
					DFCS_BUTTONPUSH, 
					(CGridBtnCellBase::CTL_ALIGN)CGridBtnCellBase::CTL_ALIGN_RIGHT,
					0,  
					0,  
					"配置");

			}
			m_VideoGameTableGrid.SetItemData(nRowIndex, 0, (LPARAM)GameTable);
		
			ServerInfoDB->MoveToNext();		
		}
		ServerInfoDB->CloseRecordset();

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}
	return -1;
}

void CInitSystemDlg::OnBnClickedGengameserviceparamfile()
{
	CGameServiceParamDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CInitParamter m_InitParamter;
		m_InitParamter.LoadInitParamter();
		TCHAR szData[1024]=TEXT("");
		CXOREncrypt::EncryptData(m_InitParamter.m_szLogonServerPass,szData,sizeof(szData));

		GameServiceParamArray array;

		DWORD dwServerCount=m_ServerListCenter->GetGameServerCount();
		POSITION Pos=NULL;
		for (DWORD i=0;i<dwServerCount;i++)
		{
			tagGameServerItem * pGameServerItem=m_ServerListCenter->EmunGameServerItem(Pos);
			GameServiceParam param;
			memset(&param, 0, sizeof(GameServiceParam));

			_tcscpy(param.m_szCenterServerAddr, (LPCSTR)dlg.m_strCenterAddr);
			_tcscpy(param.m_szLoaderServerPass, szData);
			param.m_wServerID = pGameServerItem->GameServer.wServerID;
			param.m_wGameNetPort = dlg.m_dwGameBeginPort + i * 2;
			_tcscpy(param.m_szGameNetAddr, (LPCSTR)dlg.m_strGameBindAddr);

			array.Add(param);
		}
		

		CGameServiceParamFile saveFile;
		saveFile.Save((LPCSTR)dlg.m_strFilePath, array);
	}
}
void CInitSystemDlg::OnBnClickedSaveGameMianInfo()
{
	TCHAR szMainWebPageAddress[128];
	GetDlgItemText(IDC_MAINWEBPAGEADDR, szMainWebPageAddress, 128);

	TCHAR szFieldLiveVideoAddress[64];
	GetDlgItemText(IDC_FIELDLIVEVIDEOADDR, szFieldLiveVideoAddress, 64);

	TCHAR szVideoTapeAddress[64];
	GetDlgItemText(IDC_VIDEOTAPEADDR, szVideoTapeAddress, 64);

	UpdateGameMainInfo(szMainWebPageAddress,
		szFieldLiveVideoAddress,
		szVideoTapeAddress);
}
void CInitSystemDlg::OnBnClickedSaveNotifyMan()
{
	TCHAR szNotice[512];
	GetDlgItemText(IDC_EDIT_NOTIFY_MAN, szNotice, 512);
	
	UpdateNotice(szNotice, _time64(NULL), 1);
}

void CInitSystemDlg::OnBnClickedSaveNotifyGame()
{
	TCHAR szNotice[512];
	GetDlgItemText(IDC_EDIT_NOTIFY_GAME, szNotice, 512);

	UpdateNotice(szNotice, _time64(NULL), 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServiceParamDlg::CGameServiceParamDlg() : CDialog(IDD_GAMESERVICEPARAM)
{
}

//析构函数
CGameServiceParamDlg::~CGameServiceParamDlg()
{
}
BEGIN_MESSAGE_MAP(CGameServiceParamDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSH, OnBnBrowse)
END_MESSAGE_MAP()


//控件子类化
void CGameServiceParamDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//初始化函数
BOOL CGameServiceParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//服务地址
	TCHAR szHostName[64],szServerAddr[16];
	gethostname(szHostName,sizeof(szHostName));
	HOSTENT * pHostent=gethostbyname(szHostName);
	LPIN_ADDR * lpAddrList=(LPIN_ADDR *)pHostent->h_addr_list;
	CComboBox * pCenterServerAddr=(CComboBox *)GetDlgItem(IDC_CENTERSERVER_ADDR);
	CComboBox * pGameServerAddr=(CComboBox *)GetDlgItem(IDC_GAMEBINDADDR);
	while (*lpAddrList!=NULL)
	{
		lstrcpyn(szServerAddr,inet_ntoa(**lpAddrList),CountArray(szServerAddr));
		int nIndex=pCenterServerAddr->AddString(szServerAddr);
		pCenterServerAddr->SetItemData(nIndex,(*lpAddrList)->S_un.S_addr);
		nIndex=pGameServerAddr->AddString(szServerAddr);
		pGameServerAddr->SetItemData(nIndex,(*lpAddrList)->S_un.S_addr);
		lpAddrList++;
	}
	(((CComboBox *)GetDlgItem(IDC_CENTERSERVER_ADDR)))->SetCurSel(0);
	(((CComboBox *)GetDlgItem(IDC_GAMEBINDADDR)))->SetCurSel(0);

	SetDlgItemInt(IDC_GAMEBEGINPORT, 9600);

	CString strFilePath = GetMyAppPath() + TEXT("GameServiceParam.xml");
	SetDlgItemText(IDC_FILEPATH, strFilePath);

	return TRUE;
}

//确定函数
void CGameServiceParamDlg::OnOK()
{
	(((CComboBox *)GetDlgItem(IDC_CENTERSERVER_ADDR)))->GetWindowText(m_strCenterAddr);
	m_dwGameBeginPort = GetDlgItemInt(IDC_GAMEBEGINPORT);
	(((CComboBox *)GetDlgItem(IDC_GAMEBINDADDR)))->GetWindowText(m_strGameBindAddr);
	 GetDlgItemText(IDC_FILEPATH,m_strFilePath);
	__super::OnOK();
}
void CGameServiceParamDlg::OnBnBrowse()
{
	CString strFilePath;
	GetDlgItemText(IDC_FILEPATH, strFilePath);
	int pos = strFilePath.ReverseFind('\\');
	if(pos != -1)
		strFilePath = strFilePath.Left(pos) + "\\" ;

	CFileDialog fd(FALSE, TEXT("*.xml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("xml files(*.xml)|*.xml|All files (*.*)|*.*||")); 
	fd.m_ofn.lpstrInitialDir=strFilePath; 
	if(fd.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_FILEPATH, fd.GetPathName());
	}
}
