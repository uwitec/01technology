#pragma once
#include "afxcmn.h"
#include "ServerItemView.h"
#include "InitParamter.h"
#include "..\..\ShareModule\gridctrl\gridctrl.h"
#include "..\..\ShareModule\GridCtrl\NewCellTypes/GridBtnCell.h"

// CInitSystemDlg 对话框

class CInitSystemDlg : public CDialog,public ITreeCtrlSink
{
	DECLARE_DYNAMIC(CInitSystemDlg)

public:
	CInitSystemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitSystemDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INITSYSTEM };

	CServerItemView m_ServerItemView;
	CServerListManager m_ServerListManager;

	CServerListCenterHelper			m_ServerListCenter;					//列表组件
	IEventService					* m_pIEventService;					//事件服务
	CInitParamter					m_InitParamter;

	CGridCtrl						m_VideoGameTableGrid;
	CBtnDataBase					m_BtnDataBase;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool	InitSystem(IEventService* pIEventService);
	void	UpdateServerList();
	void	UpdateVideoGameTabeList();
	void	ClearVideoGameTabeList();
	void	EnableVideoGameTableOptionPane(BOOL bEnable);
	void	OnConfigVideoGameServer(int nRow);
	
	bool	OnMainServerList(WORD wSubCmdID, 
		void * pBuffer, 
		WORD wDataSize);

	int		InsertGameType(BYTE  cbImageID,
							LPCTSTR strTypeName,
							int nSortID);
	int		UpdateGameType(int nTypeID,
			BYTE  cbImageID,
			LPCTSTR strTypeName,
			int nSortID);
	int		DeleteGameType(int nTypeID);

	int		InsertGameKind(int nTypeID, 
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
						int nAILevel);
	int		UpdateGameKind(int nKindID,
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
		int nAILevel);
	int		DeleteGameKind(int nKindID);
	
	int		InsertGameServer(
		int nKindID,
		int nTypeID, 
		LPCTSTR strServerName,
		int nSortID,
		int nMaxUserCount);
	int		UpdateGameServer(
		int nServerID,
		int nKindID,
		int nTypeID, 
		LPCTSTR strServerName,
		int nSortID,
		int nMaxUserCount);
	int		DeleteGameServer(int nServerID);

	int		InsertGameTable(
		int nKindID,
		int nTypeID, 
		int nTableNumber,
		BYTE *pParam,
		int nParamSize);
	int		UpdateGameTable(
		int nTableID,
		int nKindID,
		int nTypeID, 
		int nTableNumber,
		BYTE *pParam,
		int nParamSize);
	int		DeleteGameTable(int nTableID);
	int		ReadGameTable(int wKindID);

	int LoadGameMainInfo();
	int UpdateGameMainInfo(LPCSTR strMainWebPageAddress,
							LPCSTR strFieldLiveVideoAddress,
							LPCSTR strVideoTapeAddress);
	int LoadNotice();
	int UpdateNotice(LPCSTR szNotice, __time64_t tmUpdateTime, BYTE cbUpdateFlag = 0);

	
	//左键单击
	virtual void __cdecl OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//右键单击
	virtual void __cdecl OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//左键双击
	virtual void __cdecl OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//右键双击
	virtual void __cdecl OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//选择改变
	virtual void __cdecl OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//子项展开
	virtual void __cdecl OnTreeItemexpanded(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAddType();
	afx_msg void OnBnClickedAddKind();
	afx_msg void OnBnClickedAddServer();
	afx_msg void OnBnClickedDelNode();
	afx_msg void OnBnClickedOptionNode();
	afx_msg void OnBnClickedRefreshNode();
	afx_msg void OnBnClickedAddvideogametable();
	afx_msg void OnBnClickedDelvideogametable();
	afx_msg void OnBnClickedConfigvideogametable();
	afx_msg void OnGridClick (NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedGengameserviceparamfile();
	afx_msg void OnBnClickedSaveGameMianInfo();
	afx_msg void OnBnClickedSaveNotifyMan();
	afx_msg void OnBnClickedSaveNotifyGame();
};
//配置对话框
class CGameServiceParamDlg : public CDialog
{
	//构造函数
public:
	//构造程序
	CGameServiceParamDlg();
	//析构函数
	virtual ~CGameServiceParamDlg();

	CString m_strCenterAddr;
	DWORD	m_dwGameBeginPort;
	CString m_strGameBindAddr;
	CString m_strFilePath;

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	//控件子类化
	virtual void DoDataExchange(CDataExchange * pDX);
	//确定函数
	virtual void OnOK();
	
	afx_msg void OnBnBrowse();

	//消息映射
	DECLARE_MESSAGE_MAP()
};
