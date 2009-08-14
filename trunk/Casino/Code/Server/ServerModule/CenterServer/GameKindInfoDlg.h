#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CGameKindInfoDlg �Ի���

class CGameKindInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameKindInfoDlg)

public:
	CGameKindInfoDlg(BOOL bNewGameKind = TRUE,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameKindInfoDlg();

// �Ի�������
	enum { IDD = IDD_GAMEKINDINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strKindName;
	DWORD m_dwSortID;
	DWORD m_dwTableCount;
	DWORD m_dwCellScore;
	DWORD m_dwTaxRate;
	CComboBox m_comboGameProcessType;
	DWORD m_dwGameProcessType;
	BOOL  m_bNewGameKind;
	DWORD m_dwAIUserCount;

	void UpdateGameKindInfo(WORD wProcessType);
	void UpdateGameBetLimit(WORD wProcessType,DWORD dwCellScore);
	void UpdateGameKindName(WORD wProcessType);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	DWORD m_dwHighScore;
	DWORD m_dwLessScore;
	afx_msg void OnCbnSelchangeGameprocesstype();
	afx_msg void OnEnChangeCellscore();
	afx_msg void OnEnChangeTablecount();
	CSliderCtrl m_sliderAILevel;
	int m_dwAILevel;
};
