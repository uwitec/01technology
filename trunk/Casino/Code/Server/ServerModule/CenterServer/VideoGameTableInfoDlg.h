#pragma once


// CVideoGameTableInfoDlg �Ի���

class CVideoGameTableInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoGameTableInfoDlg)

public:
	CVideoGameTableInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoGameTableInfoDlg();

// �Ի�������
	enum { IDD = IDD_VIDEOGAMETABLEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwTableNumber;
	ULONGLONG m_ullMaxUserRevenseSharing;
	DWORD m_dwStateKernelServerAddr;
	DWORD m_dwStateKernelServerPort;
	DWORD m_dwHighScore;
	DWORD m_dwCellScore;
};
