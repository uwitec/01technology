// ServerMonitorDlg.h : 头文件
//

#pragma once

#include "../../ShareModule/GridCtrl/GridCtrl.h"
#include "../../ShareModule/GridCtrl/NewCellTypes/GridBtnCell.h"
#include "../../ShareModule/MarkupSTL/MarkupSTL.h"
#include "PingThread.h"

struct tagMonitorData  
{
	CString strSerName;
	CString strSerAddr;
	CString strNetAddr;
	BOOL	bNetPassFlag;
	int		nNetDelayValue[5];
	int		nNetDelayValueIndex;

};

// CServerMonitorDlg 对话框
class CServerMonitorDlg : public CDialog
{
// 构造
public:
	CServerMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVERMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	CGridCtrl						m_gridServerMonitor;
	CBtnDataBase					m_BtnDataBase;
	BOOL							m_bRunning;
	CPingThread						m_PingThread;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	void AddMonitorItem(tagMonitorData *pMonitorData);

	bool LoadMonitorItem(const char* szFileName);
	bool SaveMonitorItem(const char* szFileName);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonClose();
	afx_msg LRESULT OnMsgStatus(WPARAM wParam, LPARAM lParam);
};
