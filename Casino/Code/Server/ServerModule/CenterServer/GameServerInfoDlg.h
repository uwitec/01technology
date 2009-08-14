#pragma once


// CGameServerInfoDlg 对话框

class CGameServerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameServerInfoDlg)

public:
	CGameServerInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGameServerInfoDlg();

// 对话框数据
	enum { IDD = IDD_GAMESERVERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strServerName;
	DWORD m_dwSortID;
	DWORD m_dwMaxUserCount;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
