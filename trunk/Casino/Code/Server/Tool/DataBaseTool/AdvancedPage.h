#pragma once


// CAdvancedPage �Ի���
#include "InitParamter.h"

class CAdvancedPage : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedPage)

public:
	CAdvancedPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvancedPage();

// �Ի�������
	enum { IDD = IDD_ADVANCEDPAGE };
public:
	void	InitPage(CInitParamter	*pInitParamter);
protected:
	CInitParamter	* m_pInitParamter;

	LONG Database_Reset(BOOL bResetGTI,
		BOOL bResetUI);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReset();
};
