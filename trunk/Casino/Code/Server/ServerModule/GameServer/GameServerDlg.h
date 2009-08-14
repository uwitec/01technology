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

//配置对话框
class CSystemOptionDlg : public CDialog
{
	//构造函数
public:
	//构造程序
	CSystemOptionDlg(CInitParamter* pOutInitParamter=NULL);
	//析构函数
	virtual ~CSystemOptionDlg();

	CInitParamter* m_pOutInitParamter;
	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	//控件子类化
	virtual void DoDataExchange(CDataExchange * pDX);
	//确定函数
	virtual void OnOK();

	//消息映射
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//程序主窗口
class CGameServerDlg : public CDialog
{
	//组件变量
public:

	//控件变量
public:
	CRichEditCtrl					m_RichEditCtrl;						//信息窗口
	CGridCtrl						m_gridGameService;
	CBtnDataBase					m_BtnDataBase;
	CEventServiceHelper				m_EventService;						//事件服务

	BOOL							m_bSetStartGameServiceTimer;
	//函数定义
public:
	//构造函数
	CGameServerDlg();
	//析构函数
	virtual ~CGameServerDlg();

	//显示错误
	void ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel);	
	void ShowDescribeMessage(int nGameServiceHelperIndex,LPCTSTR pszString);
	void NotifyServiceStatus(int nGameServiceHelperIndex,bool bRunning);

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK();
	//取消函数
	virtual void OnCancel();

	//内部函数
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
	//消息映射
protected:
	//启动服务
	afx_msg void OnBnClickedStart();
	//停止服务
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