// DataBaseToolDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"

#include "TabSheet.h"
#include "BackupRestorePage.h"
#include "ClearDataPage.h"
#include "AdvancedPage.h"
#include "InitParamter.h"

// CDataBaseToolDlg �Ի���
class CDataBaseToolDlg : public CDialog
{
// ����
public:
	CDataBaseToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATABASETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON			   m_hIcon;
	CTabSheet		   m_Sheet;
	CBackupRestorePage m_dlgBackupRestorePage;
	CClearDataPage	   m_dlgClearDataPage;
	CAdvancedPage	   m_dlgAdvancedPage;

	CInitParamter	   m_InitParamter;

	// ���ɵ���Ϣӳ�亯��
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
