#pragma once
#include "..\..\ShareModule\gridctrl\gridctrl.h"


// CVideoServerInfoDlg �Ի���

class CVideoServerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoServerInfoDlg)

public:
	CVideoServerInfoDlg(WORD wTableID,
		CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoServerInfoDlg();

// �Ի�������
	enum { IDD = IDD_VIDEOSERVERINFO };

	WORD							m_wTableID;
	CGridCtrl						m_VideoServerGrid;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
