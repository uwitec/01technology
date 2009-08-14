#pragma once

#include "../../ShareModule/GridCtrl/GridCtrl.h"
#include "../../ShareModule/GridCtrl/NewCellTypes/GridBtnCell.h"


//���� �Ի���
class CPlazaService;
class CConnectUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectUserDlg)

public:
	CConnectUserDlg(CPlazaService *pLogonService,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnectUserDlg();
public:
	//�û�����
	void __cdecl OnUserData(const DWORD&	dwRealUserID,
		const BYTE&   cbUserType,
		const tagUserData& UserData,
		const DWORD&	dwClientIP);
	//�ǳ��û�
	void OnKickConectUser(int nRow);
protected:
	CGridCtrl			m_gridGameService;
	CBtnDataBase		m_BtnDataBase;
	CPlazaService *		m_pLogonService;

// �Ի�������
	enum { IDD = IDD_CONNECTUSER };
protected:
	void	UpdateConnectUser();
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGridClick (NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnDestroy();
};
