// DataBaseToolDlg.h : 头文件
//

#pragma once

#include "afxcmn.h"

#include "TabSheet.h"
#include "BackupRestorePage.h"
#include "ClearDataPage.h"
#include "AdvancedPage.h"
#include "InitParamter.h"

// CDataBaseToolDlg 对话框
class CDataBaseToolDlg : public CDialog
{
// 构造
public:
	CDataBaseToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATABASETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON			   m_hIcon;
	CTabSheet		   m_Sheet;
	CBackupRestorePage m_dlgBackupRestorePage;
	CClearDataPage	   m_dlgClearDataPage;
	CAdvancedPage	   m_dlgAdvancedPage;

	CInitParamter	   m_InitParamter;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSaveconfig();
};
