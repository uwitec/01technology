#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Commonality.h"
#include "ServerList.h"
#include "InitParamter.h"
#include "CenterSocket.h"

//////////////////////////////////////////////////////////////////////////

//连接信息结构
struct tagConnectItemInfo
{
	BYTE							bConnect;							//是否连接
	WORD							wRoundID;							//循环索引
	DWORD							dwClientIP;							//连接地址
	tagUserData						UserData;							//用户数据
	BYTE							cbUserType;							//用户类型
	DWORD							dwRealUserID;						//用户ID
};
#define INVALID_USERTYPE (BYTE)-1

//////////////////////////////////////////////////////////////////////////

//调度引擎钩子
class CAttemperEngineSink : public IAttemperEngineSink
{
	//变量定义
protected:
	CInitParamter					* m_pInitParamter;					//配置参数
	tagConnectItemInfo				* m_pConnectItemInfo;				//连接信息
	WORD							m_wLogonUserCount;					//登陆人数

	//在线人数
protected:
	WORD							m_wOnLineInfoSize;					//信息长度
	tagOnLineCountInfo				m_OnLineCountInfo[256];				//在线信息

	//CenterServer
protected:
	enSocketState					m_SocketState;						//连接状态
	CCenterSocket					* m_pCenterSocket;					//中心连接

	//辅助组件
protected:
	CServerList						m_ServerList;						//服务器列表
	CQueueServiceEvent				m_DataBaseEvent;					//数据库通知

	//接口变量
protected:
	ITimerEngine					* m_pITimerEngine;					//定时器接口
	IEventService					* m_pIEventService;					//事件服务
	ITCPSocketEngine				* m_pITCPSocketEngine;				//网络接口
	IUpdateEventSink				* m_pIUpdateEventSink;				//更新事件接口

	//游戏变量
protected:
	CMD_GP_GameInfo					m_GameInfo;							//游戏信息
	CMD_GP_Notice					m_ManagementNotice;					//管理公告
	CMD_GP_Notice					m_GameNotice;						//游戏公告
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

	//配置函数 
public:
	//设置事件
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//设置更新事件钩子
	bool SetUpdateEventSink(IUnknownEx * pIUnknownEx);
	//配置函数
	bool InitAttemperSink(CInitParamter * pInitParamter, CCenterSocket * pCenterSocket, IUnknownEx * pIUnknownEx);

	//辅助函数
public:
	//枚举用户
	void EnumConnectUser(IUnknownEx * pIUnknownEx);
	//判断是否与中心连接
	bool IsConnectedCenter();

	//中心连接事件
public:
	//中心连接事件
	virtual bool __cdecl OnEventCenterSocketConnect(int iErrorCode);
	//中心读取事件
	virtual bool __cdecl OnEventCenterSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
	//中心关闭事件
	virtual bool __cdecl OnEventCenterSocketClose(bool bCloseByServer);

	//数据库函数
private:
	//用户登录成功
	bool OnDBLogonSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//用户登录失败
	bool OnDBLogonError(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//发送下线帐号完毕
	bool OnDBGetLessUserListComplete(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//更新游戏主信息
	bool OnDBUpdateGameMainInfoSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);

	//网络函数
private:
	//登录消息处理
	bool OnSocketMainLogon(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//编辑帐号处理
	bool OnSocketMainEditUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//编辑子帐号处理
	bool OnSocketMainEditSubUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//获取帐号处理
	bool OnSocketMainGetUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//获取记录
	bool OnSocketMainGetRecord(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);

	//中心网络
private:
	//列表消息处理
	bool OnCenterMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
	//更新信息消息处理
	bool OnCenterMainUpdateInfo(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
	//管理消息处理
	bool OnCenterMainManagement(CMD_Command Command, void * pDataBuffer, WORD wDataSize);

	//辅助函数
private:
	//检查帐号信息
	int SimpleCheck_CMD_GP_RegisterUser(CMD_GP_RegisterUser * pRegisterUser);
	//检查帐号基本信息
	int SimpleCheck_CMD_GP_BasicUserInfo(CMD_GP_BasicUserInfo * pBasicUserInfo);
	//检查帐号详细信息
	int SimpleCheck_CMD_GP_DetailUserInfo(CMD_GP_DetailUserInfo * pDetailUserInfo);
	//检查子帐号信息
	int SimpleCheck_CMD_GP_RegisterSubUser(CMD_GP_RegisterSubUser * pRegisterSubUserInfo);
	//检查子帐号基本信息
	int SimpleCheck_CMD_GP_BasicSubUserInfo(CMD_GP_BasicSubUserInfo * pBasicSubUserInfo);
	//检查自身帐号基本信息
	int SimpleCheck_CMD_GP_MySelfBasicUserInfo(CMD_GP_MySelfBasicUserInfo * pBasicUserInfo);
	//检查获取操作记录信息
	int SimpleCheck_CMD_GP_GetOperation(CMD_GP_GetOperation * pGetOperation);
	//检查获取报表记录方式-1信息
	int SimpleCheck_CMD_GP_GetChartViewOne(CMD_GP_GetChartViewOne * pGetChartViewOne);
	//检查获取报表记录方式-2信息
	int SimpleCheck_CMD_GP_GetChartViewTwo(CMD_GP_GetChartViewTwo * pGetChartViewTwo);
	//检查获取报表记录方式-3信息
	int SimpleCheck_CMD_GP_GetChartViewThree(CMD_GP_GetChartViewThree * pGetChartViewThree);
	//检查获取游戏记录信息
	int SimpleCheck_CMD_GP_GetBetHistory(CMD_GP_GetBetHistory * pGetBetHistory);
	//检查用户操作权限
	bool SimpleCheck_UserRight(BYTE& cbUserType,tagUserData& UserData);
};

//////////////////////////////////////////////////////////////////////////

#endif