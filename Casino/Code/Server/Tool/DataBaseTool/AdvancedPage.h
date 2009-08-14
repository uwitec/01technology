#pragma once


// CAdvancedPage 对话框
#include "InitParamter.h"

class CAdvancedPage : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedPage)

public:
	CAdvancedPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdvancedPage();

// 对话框数据
	enum { IDD = IDD_ADVANCEDPAGE };
public:
	void	InitPage(CInitParamter	*pInitParamter);
protected:
	CInitParamter	* m_pInitParamter;

	LONG Database_Reset(BOOL bResetGTI,
		BOOL bResetUI);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReset();
};
