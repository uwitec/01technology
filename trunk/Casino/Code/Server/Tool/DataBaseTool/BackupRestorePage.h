#pragma once


// CBackupRestorePage 对话框
#include "InitParamter.h"

class CBackupRestorePage : public CDialog
{
	DECLARE_DYNAMIC(CBackupRestorePage)

public:
	CBackupRestorePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBackupRestorePage();

// 对话框数据
	enum { IDD = IDD_BACKUPRESOREPAGE };
public:
	void	InitPage(CInitParamter	*pInitParamter);
protected:
	LONG	Database_Backup(LPCSTR lpszPath, WORD dwPathSize);
	LONG	Database_Restore(LPCSTR lpszPath, WORD dwPathSize);
protected:
	CInitParamter	* m_pInitParamter;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowsebkpath();
	afx_msg void OnBnClickedButtonBrowsertpath();
	afx_msg void OnBnClickedButtonBackup();
	afx_msg void OnBnClickedButtonRestore();
};
