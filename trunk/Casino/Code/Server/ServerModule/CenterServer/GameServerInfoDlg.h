#pragma once


// CGameServerInfoDlg �Ի���

class CGameServerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameServerInfoDlg)

public:
	CGameServerInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameServerInfoDlg();

// �Ի�������
	enum { IDD = IDD_GAMESERVERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strServerName;
	DWORD m_dwSortID;
	DWORD m_dwMaxUserCount;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
