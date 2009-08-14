#pragma once
#include "afxcmn.h"
#include "ServerItemView.h"
#include "InitParamter.h"
#include "..\..\ShareModule\gridctrl\gridctrl.h"
#include "..\..\ShareModule\GridCtrl\NewCellTypes/GridBtnCell.h"

// CInitSystemDlg �Ի���

class CInitSystemDlg : public CDialog,public ITreeCtrlSink
{
	DECLARE_DYNAMIC(CInitSystemDlg)

public:
	CInitSystemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInitSystemDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INITSYSTEM };

	CServerItemView m_ServerItemView;
	CServerListManager m_ServerListManager;

	CServerListCenterHelper			m_ServerListCenter;					//�б����
	IEventService					* m_pIEventService;					//�¼�����
	CInitParamter					m_InitParamter;

	CGridCtrl						m_VideoGameTableGrid;
	CBtnDataBase					m_BtnDataBase;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	
	//�������
	virtual void __cdecl OnTreeLeftClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//�Ҽ�����
	virtual void __cdecl OnTreeRightClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//���˫��
	virtual void __cdecl OnTreeLeftDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//�Ҽ�˫��
	virtual void __cdecl OnTreeRightDBClick(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//ѡ��ı�
	virtual void __cdecl OnTreeSelectChanged(CListItem * pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl);
	//����չ��
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
//���öԻ���
class CGameServiceParamDlg : public CDialog
{
	//���캯��
public:
	//�������
	CGameServiceParamDlg();
	//��������
	virtual ~CGameServiceParamDlg();

	CString m_strCenterAddr;
	DWORD	m_dwGameBeginPort;
	CString m_strGameBindAddr;
	CString m_strFilePath;

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();
	
	afx_msg void OnBnBrowse();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};
