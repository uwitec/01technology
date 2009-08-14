#pragma once


// CVideoServerPathDlg 对话框

class CVideoServerPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoServerPathDlg)

public:
	CVideoServerPathDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoServerPathDlg();

// 对话框数据
	enum { IDD = IDD_VIDEOSERVERPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strVideoServerPath;
};
