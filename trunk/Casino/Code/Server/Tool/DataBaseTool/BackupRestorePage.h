#pragma once


// CBackupRestorePage �Ի���
#include "InitParamter.h"

class CBackupRestorePage : public CDialog
{
	DECLARE_DYNAMIC(CBackupRestorePage)

public:
	CBackupRestorePage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBackupRestorePage();

// �Ի�������
	enum { IDD = IDD_BACKUPRESOREPAGE };
public:
	void	InitPage(CInitParamter	*pInitParamter);
protected:
	LONG	Database_Backup(LPCSTR lpszPath, WORD dwPathSize);
	LONG	Database_Restore(LPCSTR lpszPath, WORD dwPathSize);
protected:
	CInitParamter	* m_pInitParamter;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowsebkpath();
	afx_msg void OnBnClickedButtonBrowsertpath();
	afx_msg void OnBnClickedButtonBackup();
	afx_msg void OnBnClickedButtonRestore();
};
