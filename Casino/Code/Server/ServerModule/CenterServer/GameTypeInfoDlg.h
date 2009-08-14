#pragma once


// CGameTypeInfoDlg 对话框

class CGameTypeInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameTypeInfoDlg)

public:
	CGameTypeInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGameTypeInfoDlg();

// 对话框数据
	enum { IDD = IDD_GAMETYPEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTypeName;
	DWORD m_dwSortID;
	int m_nImageID;
	CString m_strImageID;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
