#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParamter.h"
//数据库请求标识
#define DBR_GC_READ_GAMEUSERUPDATEDTEMP						1	//读取用户更新信息临时表
#define DBR_GC_CLEAR_GAMEUSERUPDATEDTEMP					2	//清除用户更新信息临时表
//数据库输出标识
#define DBR_GC_UPDATE_SERVERUSERDATA						10	//更新用户信息

struct DBR_GC_Clear_GameUserUpdatedTemp
{
	__time64_t		tmBeforeTime;
};

//数据库引擎钩子
class CDataBaseSink : public IDataBaseSink
{
	//变量定义
protected:
	CInitParamter					* m_pInitParamter;					//配置参数
	IEventService					* m_pIEventService;					//事件服务

	//组件变量
protected:
	CDataBaseHelper					m_CenterDataBase;					//中心数据库
	CQueueServiceEvent				m_AttemperEvent;					//调度通知

	//函数定义
public:
	//构造函数
	CDataBaseSink();
	//析构函数
	virtual ~CDataBaseSink();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CDataBaseSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//调度接口
public:
	//调度模块启动
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//调度模块关闭
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//数据操作处理
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//配置函数 
public:
	//设置事件
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//配置函数
	bool InitDataBaseSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx);

	//处理函数
protected:
	//请求读取用户更新信息临时表
	bool OnRequestReadGameUserUpdatedTemp(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//请求清除用户更新信息临时表
	bool OnRequestClearGameUserUpdatedTemp(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//存储过程
protected:
	//读取用户更新信息临时表存储过程
	LONG SPReadGameUserUpdatedTemp();
	//清除用户更新信息临时表存储过程
	LONG SPClearGameUserUpdatedTemp(DBR_GC_Clear_GameUserUpdatedTemp * pClearGameUserUpdatedTemp);
};


#endif

