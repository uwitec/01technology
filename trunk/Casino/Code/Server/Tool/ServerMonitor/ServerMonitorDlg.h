// ServerMonitorDlg.h : ͷ�ļ�
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

// CServerMonitorDlg �Ի���
class CServerMonitorDlg : public CDialog
{
// ����
public:
	CServerMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVERMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	CGridCtrl						m_gridServerMonitor;
	CBtnDataBase					m_BtnDataBase;
	BOOL							m_bRunning;
	CPingThread						m_PingThread;

	// ���ɵ���Ϣӳ�亯��
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
