#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "PlazaService.h"
#include "RequestSocket.h"

//////////////////////////////////////////////////////////////////////////
class CUpdateEventSink;

//配置对话框
class CSystemOptionDlg : public CDialog
{
	//构造函数
public:
	//构造程序
	CSystemOptionDlg();
	//析构函数
	virtual ~CSystemOptionDlg();

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

//主对话框
class CPlazaServerDlg : public CDialog
{
	//变量定义
protected:
	bool							m_bAskQuit;							//退出询问

	//组件变量
public:
	CPlazaService					m_LogonService;						//大厅服务
	CEventServiceHelper				m_EventService;						//事件服务
	CRequestSocket					m_RequestSocket;					//请求Socket
	CUpdateEventSink				*m_pUpdateEventSink;					//更新事件钩子

	//控件变量
public:
	CRichEditCtrl					m_RichEditCtrl;						//事件记录

	//构造函数
public:
	//构造程序
	CPlazaServerDlg();
	//析构函数
	virtual ~CPlazaServerDlg();
	//辅助函数
protected:
	//更新人数
	void UpdateUserCount(WORD wLogonUserCount,WORD wGameUserCount);

	//重载函数
protected:
	//控件子类化
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK();
	//取消函数
	virtual void OnCancel();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//消息函数
protected:
	//启动服务
	afx_msg void OnBnClickedStartService();
	//停止服务
	afx_msg void OnBnClickedStopService();
	//配置服务
	afx_msg void OnBnClickedInitService();
	//在线用户
	afx_msg void OnBnClickedEnumConnectUser();
	//请求信息成功
	afx_msg LRESULT OnRequestInfoOk(WPARAM w, LPARAM l);
	//更新人数
	afx_msg LRESULT OnShowUserCountNotify(WPARAM w, LPARAM l);
	//定时
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
