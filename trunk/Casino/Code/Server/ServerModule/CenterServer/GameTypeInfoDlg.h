#pragma once


// CGameTypeInfoDlg �Ի���

class CGameTypeInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameTypeInfoDlg)

public:
	CGameTypeInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameTypeInfoDlg();

// �Ի�������
	enum { IDD = IDD_GAMETYPEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTypeName;
	DWORD m_dwSortID;
	int m_nImageID;
	CString m_strImageID;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
