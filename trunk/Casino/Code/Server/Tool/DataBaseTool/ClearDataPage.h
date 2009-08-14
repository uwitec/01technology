#pragma once


// CClearDataPage 对话框
#include "InitParamter.h"
#include "afxdtctl.h"

class CClearDataPage : public CDialog
{
	DECLARE_DYNAMIC(CClearDataPage)

public:
	CClearDataPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClearDataPage();

// 对话框数据
	enum { IDD = IDD_CLEARDATAPAGE };
public:
	void	InitPage(CInitParamter	*pInitParamter);
protected:
	LONG	Database_Clear(LONGLONG llTimeGRC,
							LONGLONG llTimeOP,
							BOOL bClearBC,
							BOOL bClearLL);
protected:
	CInitParamter	* m_pInitParamter;
	CDateTimeCtrl m_dpGRC;
	CDateTimeCtrl m_dpOP;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCleardata();
};
