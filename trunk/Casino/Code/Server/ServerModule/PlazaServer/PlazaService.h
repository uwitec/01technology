#ifndef LOGON_SERVICE_HEAD_FILE
#define LOGON_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CenterSocket.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////
const int nDBThreadCount = 12;

//大厅服务类
class CPlazaService : public IDataBaseSink
{
	//变量定义
protected:
	CCenterSocket					m_CenterSocket;						//中心连接
	CDataBaseSink					m_DataBaseSink[nDBThreadCount];	    //数据库钩子
	CAttemperEngineSink				m_AttemperSink;						//调度钩子

	//组件变量
protected:
	CServiceEngineHelper			m_ServiceEngine;					//服务引擎

	//配置变量
protected:
	CInitParamter					m_InitParamter;						//配置参数
	IEventService					* m_pIEventService;					//事件服务
	IUpdateEventSink				* m_pIUpdateEventSink;				//更新事件接口

	//函数定义
public:
	//构造函数
	CPlazaService(void);
	//析构函数
	virtual ~CPlazaService(void);

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid())  return true; else return false;}
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CPlazaService))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//调度接口
public:
	//模块启动
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//模块关闭
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//数据操作处理
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//功能函数
public:
	//服务状态
	bool IsService();
	//开始服务
	bool StartLogonService();
	//停止服务
	bool StopLogonService();
	//设置事件
	bool SetEventService(IUnknownEx * pIEventService);
	//设置更新事件钩子
	bool SetUpdateEventSink(IUnknownEx * pIUpdateEventSink);
	//复位登陆信息
	LONG ResetLockLogonInfo(DWORD dwLogonServerAddr,
		WORD wListenPort);
	//枚举用户
	void EnumConnectUser(IUnknownEx * pIUnknownEx);
	//发送用户消息
	bool SendUserMessge(WORD dwRealUserID,BYTE cbUserType,bool bKick, LPCSTR lpcsMsg = "");

	//内部函数
private:
	//显示错误
	void TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel);
};

//////////////////////////////////////////////////////////////////////////

#endif