#ifndef SERVIVE_LOADER_DLG_HEAD_FILE
#define SERVIVE_LOADER_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "InitParamter.h"
#include "RequestSocket.h"
#include "../../ShareModule/GridCtrl/GridCtrl.h"
#include "../../ShareModule/GridCtrl/NewCellTypes/GridBtnCell.h"
#include "../../ShareModule/GameServiceParamFile/GameServiceParamFile.h"

//////////////////////////////////////////////////////////////////////////

//���öԻ���
class CSystemOptionDlg : public CDialog
{
	//���캯��
public:
	//�������
	CSystemOptionDlg(CInitParamter* pOutInitParamter=NULL);
	//��������
	virtual ~CSystemOptionDlg();

	CInitParamter* m_pOutInitParamter;
	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//����������
class CGameServerDlg : public CDialog
{
	//�������
public:

	//�ؼ�����
public:
	CRichEditCtrl					m_RichEditCtrl;						//��Ϣ����
	CGridCtrl						m_gridGameService;
	CBtnDataBase					m_BtnDataBase;
	CEventServiceHelper				m_EventService;						//�¼�����

	BOOL							m_bSetStartGameServiceTimer;
	//��������
public:
	//���캯��
	CGameServerDlg();
	//��������
	virtual ~CGameServerDlg();

	//��ʾ����
	void ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel);	
	void ShowDescribeMessage(int nGameServiceHelperIndex,LPCTSTR pszString);
	void NotifyServiceStatus(int nGameServiceHelperIndex,bool bRunning);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();

	//�ڲ�����
protected:
	void LoadGameServiceParamFile();
	void SaveGameServiceParamFile();

	void StopAllGameService();
	BOOL HaveGameServiceRunning();
	BOOL IsAllGameServiceRunning();
	void ClearGameService();
	void NewGameService(GameServiceParam& param);

	void OnRunGameServerService(int nRow);
	void OnStopGameServerService(int nRow);
	void OnConfigGameServerService(int nRow);

	void CheckStartGameServiceTimer();
	void KillStartGameServiceTimer();
	//��Ϣӳ��
protected:
	//��������
	afx_msg void OnBnClickedStart();
	//ֹͣ����
	afx_msg void OnBnClickedStop();

	afx_msg void OnBnNewGameService();
	afx_msg void OnBnDelGameService();
	afx_msg void OnBnSaveGameService();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnGridClick (NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTimer (UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////

#endif