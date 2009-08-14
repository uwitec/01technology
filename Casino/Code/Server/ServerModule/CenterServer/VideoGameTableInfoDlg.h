#pragma once


// CVideoGameTableInfoDlg 对话框

class CVideoGameTableInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoGameTableInfoDlg)

public:
	CVideoGameTableInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoGameTableInfoDlg();

// 对话框数据
	enum { IDD = IDD_VIDEOGAMETABLEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwTableNumber;
	ULONGLONG m_ullMaxUserRevenseSharing;
	DWORD m_dwStateKernelServerAddr;
	DWORD m_dwStateKernelServerPort;
	DWORD m_dwHighScore;
	DWORD m_dwCellScore;
};
