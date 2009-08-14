#pragma once
#include "InitParamter.h"
#include "RequestSocket.h"

class CGameServerDlg;
//游戏服务容器
class CGameServiceHelperContainer
{
public:
	CGameServiceHelperContainer(void);
	~CGameServiceHelperContainer(void);
	//初始化
	void Init(CGameServerDlg*	pServiceLoaderDlg,
				int nGameServiceHelperIndex,
				IEventService * pIEventService,
				TCHAR*	szCenterServerAddr,
				TCHAR*	szLoaderServerPass,
				WORD wServerID,
				WORD wGameNetPort,
				TCHAR *szGameNetAddr);
	//运行
	BOOL Start();
	//停止
	void Stop();
	//判断运行
	BOOL IsRunning();
	//设置参数
	void SetParam(TCHAR*	szCenterServerAddr,
		TCHAR*	szLoaderServerPass,
		WORD wServerID,
		WORD wGameNetPort,
		TCHAR *szGameNetAddr);
	//获取配置参数
	CInitParamter& GetInitParamter(){return m_InitParamter;}
	//请求运行
	void RequstStart(BOOL b);
	//判断请求运行
	BOOL IsRequstStart();
	//请求信息成功
	int  OnRequestInfoOk(WPARAM w, LPARAM l);
	
protected:
	//打印错误消息
	void ShowErrorMessasge (LPCTSTR pszString, enTraceLevel TraceLevel);
	//打印描述消息
	void ShowDescribeMessage(LPCTSTR pszString);
	//通知服务状态
	void NotifyServiceStatus(bool bRunning);
public:
	CGameServiceHelper				m_GameService;						//GameService
	IEventService *					m_pIEventService;					//事件服务
	tagGameServiceParameter			m_GameServiceParameter;				//游戏服务参数

	CInitParamter					m_InitParamter;						//配置参数
	CRequestSocket					m_RequestSocket;					//请求消息网络组件

	CGameServerDlg*					m_pServiceLoaderDlg;				//主窗口
	int								m_nGameServiceHelperIndex;			//请求服务索引
	
	BOOL							m_bRequstStart;						//请求开始标记
};
