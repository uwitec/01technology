#pragma once

#include "../../ShareModule/GridCtrl/GridCtrl.h"
#include "../../ShareModule/GridCtrl/NewCellTypes/GridBtnCell.h"


//在线 对话框
class CPlazaService;
class CConnectUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectUserDlg)

public:
	CConnectUserDlg(CPlazaService *pLogonService,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectUserDlg();
public:
	//用户数据
	void __cdecl OnUserData(const DWORD&	dwRealUserID,
		const BYTE&   cbUserType,
		const tagUserData& UserData,
		const DWORD&	dwClientIP);
	//登出用户
	void OnKickConectUser(int nRow);
protected:
	CGridCtrl			m_gridGameService;
	CBtnDataBase		m_BtnDataBase;
	CPlazaService *		m_pLogonService;

// 对话框数据
	enum { IDD = IDD_CONNECTUSER };
protected:
	void	UpdateConnectUser();
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGridClick (NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnDestroy();
};
