#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParamter.h"
#include "ConnectInfoManager.h"

//////////////////////////////////////////////////////////////////////////

//调度引擎钩子
class CAttemperEngineSink : public IAttemperEngineSink
{
	//组件定义
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//数据库通知
	CConnectInfoManager				m_ConnectInfoManager;				//连接管理
	CServerListCenterHelper			m_ServerListCenter;					//列表组件

	//接口变量
protected:
	CInitParamter					* m_pInitParamter;					//配置参数
	ITimerEngine					* m_pITimerEngine;					//定时器接口
	IEventService					* m_pIEventService;					//事件服务
	ITCPSocketEngine				* m_pITCPSocketEngine;				//网络接口

	//函数定义
public:
	//构造函数
	CAttemperEngineSink();
	//析构函数
	virtual ~CAttemperEngineSink();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAttemperEngineSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//调度接口
public:
	//调度模块启动
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//调度模块关闭
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//事件处理接口
	virtual bool __cdecl OnAttemperEvent(BYTE cbThreadIndex,WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime);

	//配置函数 
public:
	//设置事件
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//配置函数
	bool InitAttemperSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx);

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnEventTimer(BYTE cbThreadIndex,WORD wTimerID, WPARAM wBindParam);
	//数据库事件
	virtual bool __cdecl OnEventDataBase(BYTE cbThreadIndex,void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//网络应答事件
	virtual bool __cdecl OnEventSocketAccept(BYTE cbThreadIndex,NTY_SocketAcceptEvent * pSocketAcceptEvent);
	//网络读取事件
	virtual bool __cdecl OnEventSocketRead(BYTE cbThreadIndex,CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//网络关闭事件
	virtual bool __cdecl OnEventSocketClose(BYTE cbThreadIndex,NTY_SocketCloseEvent * pSocketCloseEvent);

	//网络函数
private:
	//列表消息处理
	bool OnSocketMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//房间消息处理
	bool OnSocketMainServerManager(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//请求消息处理
	bool OnSocketMainServerRequest(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//管理消息处理
	bool OnSocketMainManagement(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);

	//辅助函数
private:
	//读取GameService信息
	int ReadGameServerOption(WORD wServerID, tagGameServiceOption *pOption,
		int& ProcessType);
	//获取游戏模块名字
	TCHAR* GetGameModuleNameByGameProcess(WORD wProcessType);

	//数据库函数
private:
	//更新用户信息
	bool OnDBUpdateServerUserData(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);

};

//////////////////////////////////////////////////////////////////////////

#endif