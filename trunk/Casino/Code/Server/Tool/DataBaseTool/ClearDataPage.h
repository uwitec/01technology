#pragma once


// CClearDataPage �Ի���
#include "InitParamter.h"
#include "afxdtctl.h"

class CClearDataPage : public CDialog
{
	DECLARE_DYNAMIC(CClearDataPage)

public:
	CClearDataPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClearDataPage();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCleardata();
};
