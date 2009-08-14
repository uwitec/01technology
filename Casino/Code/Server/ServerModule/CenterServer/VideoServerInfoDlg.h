#pragma once
#include "..\..\ShareModule\gridctrl\gridctrl.h"


// CVideoServerInfoDlg 对话框

class CVideoServerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoServerInfoDlg)

public:
	CVideoServerInfoDlg(WORD wTableID,
		CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoServerInfoDlg();

// 对话框数据
	enum { IDD = IDD_VIDEOSERVERINFO };

	WORD							m_wTableID;
	CGridCtrl						m_VideoServerGrid;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int UpdateVideoServerList();
	int DeleteVideoServer(WORD wVideoServerID);
	int InsertVideoServer(WORD wTableID, LPCSTR strVideoServerPath);

	afx_msg void OnBnClickedAddvideoserver();
	afx_msg void OnBnClickedDelvideoserver();
	afx_msg void OnBnClickedClearnouesedvideoserver();
	virtual BOOL OnInitDialog();
};
