#pragma once


// CVideoServerPathDlg �Ի���

class CVideoServerPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoServerPathDlg)

public:
	CVideoServerPathDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoServerPathDlg();

// �Ի�������
	enum { IDD = IDD_VIDEOSERVERPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strVideoServerPath;
};
