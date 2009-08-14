#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once
#include <afxtempl.h>
#include "Afxinet.h"
#include "Commonality.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////
typedef CArray<tagUserData,tagUserData> tagUserDataArray;
typedef CArray<tagSubUserData,tagSubUserData> tagSubUserDataArray;
typedef CArray<tagOperationData,tagOperationData> tagOperationDataArray;
typedef CArray<tagChartViewOneData,tagChartViewOneData> tagChartViewOneDataArray;
typedef CArray<tagChartViewTwoData,tagChartViewTwoData> tagChartViewTwoDataArray;
typedef CArray<tagBetHistoryData,tagBetHistoryData> tagBetHistoryDataArray;

//数据库引擎钩子
class CDataBaseSink : public IDataBaseSink
{
	//变量定义
protected:
	CInitParamter					* m_pInitParamter;					//配置参数
	IEventService					* m_pIEventService;					//事件服务
	DWORD							  m_dwLogonServerAddr;
	//组件变量
protected:
	CDataBaseHelper					m_WorkDataBase;						//用户数据库
	CQueueServiceEvent				m_AttemperEvent;					//调度通知
	ITCPSocketEngine				* m_pITCPSocketEngine;				//网络引擎

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
	//模块启动
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//模块关闭
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//数据操作处理
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//配置函数 
public:
	//设置事件
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//设置接口
	bool SetSocketEngine(IUnknownEx * pIUnknownEx);
	//配置函数
	bool InitDataBaseSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx);

	//处理函数
private:
	//登录请求处理
	bool OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//注册请求处理
	bool OnRequestRegister(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//存取额度请求处理
	bool OnRequestSetUserScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//状态设定请求处理
	bool OnRequestSetUserCongealState(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//基本信息设定请求处理
	bool OnRequestSetBasicUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//详细信息设定请求处理
	bool OnRequestSetDetailUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//权限设定请求处理
	bool OnRequestSetUserRight(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//自身基本信息设定请求处理
	bool OnRequestSetMySelfBasicUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//子帐号状态设定请求处理
	bool OnRequestSetSubUserCongealState(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//注册子帐号请求处理
	bool OnRequestRegisterSubUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//子帐号基本信息设定请求处理
	bool OnRequestSetBasicSubUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取下线处理
	bool OnRequestGetLessUserList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取子帐号处理
	bool OnRequestGetSubUserList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取操作记录
	bool OnRequestGetOperationList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取报表记录方式-1
	bool OnRequestGetChartViewOneList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取报表记录方式-2
	bool OnRequestGetChartViewTwoList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取游戏局记录
	bool OnRequestGetBetHistoryList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取游戏局记录详细投注
	bool OnRequestGetDetailBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//获取报表记录方式-3
	bool OnRequestGetChartViewThree(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	
	//更新游戏信息
	bool OnRequestUpdateGameMainInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//注销帐号登陆
	bool OnRequestLogout(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//存储过程
private:
	//帐号存储过程
	LONG SPLogonByAccount(LPCTSTR pszAccounts,
		LPCTSTR pszPassword, DWORD dwClientIP,
		DWORD dwLogonServerAddr, 
		WORD wListenPort);

	//注册存储过程
	LONG SPRegisterUser(tagUserData* pUserData, 
		LPCTSTR pszPassword, DWORD dwClientIP);

	//获取下线存储过程
	LONG SPGetLessUserList(DWORD	dwParentUserID);

	//获取子帐号存储过程
	LONG SPGetSubUserList(DWORD	dwParentUserID);

	//存取额度存储过程
	LONG SPSetUserScore(DWORD dwParentUserID,DWORD dwUserID,
		DOUBLE fScore);

	//状态设定存储过程
	LONG SPSetUserCongealState(DWORD dwUserID,
		BYTE cbLevel, BYTE cbState, DWORD dwStateMark);

	//基本信息设定存储过程
	LONG SPSetBaiscUserInfo(DWORD dwUserID,
		LPCTSTR pszName,LPCTSTR pszPassword,BYTE cbFaceID);

	//详细信息设定存储过程
	LONG SPSetDetailUserInfo(DBR_GP_SetDetailUserInfo * pSetDetailUserInfo);

	//权限设定存储过程
	LONG SPSetUserRight(DBR_GP_SetUserRight * pSetUserRight);

	//自身基本信息设定存储过程
	LONG SPSetMySelfBaiscUserInfo(
		BYTE  cbUserType,
		DWORD dwUserID,
		LPCTSTR pszName,
		LPCTSTR pszPassword,
		LPCTSTR pszNewPassword,
		BYTE cbFaceID);

	//子帐号状态设定存储过程
	LONG SPSetSubUserCongealState(DWORD dwUserID,
		BYTE cbState, DWORD dwStateMark);

	//注册子帐号存储过程
	LONG SPRegisterSubUser(DWORD dwParentUserID,
		LPCTSTR pszAccount,LPCTSTR pszName,LPCTSTR pszPassword);

	//子帐号基本信息设定存储过程
	LONG SPSetBaiscSubUserInfo(DWORD dwUserID,
		LPCTSTR pszName,LPCTSTR pszPassword);

	//写入操作记录存储过程
	LONG SPInsertOperationInfo(tagOperationData& OperationData);

	//查询操作记录存储过程
	LONG SPGetOperationList(DBR_GP_GetOperation * pGetOperation);

	//查询报表记录方式-1存储过程
	LONG SPGetChartViewOneList(DBR_GP_GetChartViewOne * pGetChartViewOne);

	//查询报表记录方式-2存储过程
	LONG SPGetChartViewTwoList(DBR_GP_GetChartViewTwo * pGetChartViewTwo);

	//查询游戏局记录存储过程
	LONG SPGetBetHistoryList(DBR_GP_GetBetHistory * pGetBetHistory);

	//查询游戏局记录详细投注存储过程
	LONG SPGetDetailBetScore(DBR_GP_GetDetailBetScore * pGetDetailBetScore);

	//查询报表记录方式-3存储过程
	LONG SPGetChartViewThree(DBR_GP_GetChartViewThree * pGetChartViewThree);

	//查询游戏主信息存储过程
	LONG SPLoadGameMainInfo();

	//注销帐号登陆存储过程
	LONG SPLogoutUser(DWORD dwLogonServerAddr,
		WORD wListenPort,DBR_GP_LogoutUser * pLogoutUser);

	//辅助函数
private:
	//读取帐号信息
	inline bool helper_DBGetFieldValue_UserData(CDataBaseHelper &WorkDataBase, 
		tagUserData& UserData,DWORD* pdwRealUserID = NULL,BYTE* pcbUserType = NULL);
	//读取子帐号信息
	inline bool helper_DBGetFieldValue_SubUserData(CDataBaseHelper &WorkDataBase, tagSubUserData& UserData);
	//读取帐号列表信息
	inline bool helper_DBGetFieldValue_UserDataArray(CDataBaseHelper &WorkDataBase, tagUserDataArray& UserDataArray);
	//读取子帐号列表信息
	inline bool helper_DBGetFieldValue_SubUserDataArray(CDataBaseHelper &WorkDataBase, tagSubUserDataArray& UserDataArray);
	//初始化操作记录数据
	inline void helper_InitOperationData(tagOperationData& OperationData,DWORD dwOPType, DWORD dwIPAddress);
	//填充操作记录包头
	inline void helper_FillOperationHeader(tagOperationData& OperationData,
		DWORD			dwUserID,
		BYTE			cbUserType,
		LPCTSTR			szAccount,
		DWORD			dwOperationUserID,
		BYTE			cbOperationUserType,
		LPCTSTR			szOperationAccount);
	//获取当前时间
	inline DOUBLE helper_GetNowTime();
	//转换字符代码
	inline void helper_ConvertACPToUtf8_OperationData(tagOperationData* pOperationData);
	//转换字符代码
	inline void helper_ConvertACPToUtf8_BetHistoryData(tagBetHistoryData* pBetHistoryData);
	//读取操作记录信息
	inline bool helper_DBGetFieldValue_OperationData(CDataBaseHelper &WorkDataBase, tagOperationData& OperationData);
	//读取操作记录列表信息
	inline bool helper_DBGetFieldValue_OperationDataArray(CDataBaseHelper &WorkDataBase, tagOperationDataArray& OperationDataArray);
	//读取报表记录方式-1信息
	inline bool helper_DBGetFieldValue_ChartViewOneData(CDataBaseHelper &WorkDataBase, tagChartViewOneData& ChartViewOneData);
	//读取报表记录方式-1列表信息
	inline bool helper_DBGetFieldValue_ChartViewOneDataArray(CDataBaseHelper &WorkDataBase, tagChartViewOneDataArray& ChartViewOneDataArray);
	//读取报表记录方式-1列表配置信息
	inline bool helper_DBGetFieldValue_ChartViewOneListConfig(CDataBaseHelper &WorkDataBase, CMD_GP_ChartViewOneListConfig* pConfig);
	//读取报表记录方式-2信息
	inline bool helper_DBGetFieldValue_ChartViewTwoData(CDataBaseHelper &WorkDataBase, tagChartViewTwoData& ChartViewTwoData);
	//读取报表记录方式-2列表信息
	inline bool helper_DBGetFieldValue_ChartViewTwoDataArray(CDataBaseHelper &WorkDataBase, tagChartViewTwoDataArray& ChartViewTwoDataArray);
	//读取局记录信息
	inline bool helper_DBGetFieldValue_BetHistoryData(CDataBaseHelper &WorkDataBase, tagBetHistoryData& BetHistoryData);
	//读取游戏局记录详细投注
	inline bool helper_DBGetFieldValue_DetailBetScoreSuccess(CDataBaseHelper &WorkDataBase, CMD_GP_GetDetailBetScoreSuccess *pGetDetailBetScoreSuccess);
	//读取局记录列表信息
	inline bool helper_DBGetFieldValue_BetHistoryDataArray(CDataBaseHelper &WorkDataBase, tagBetHistoryDataArray& BetHistoryDataArray);
	//读取游戏主信息
	inline bool helper_DBGetFieldValue_GameMainInfo(CDataBaseHelper &WorkDataBase,DBR_Update_GameMainInfo &GameMainInfo);
	//读取报表记录方式-3信息
	inline bool helper_DBGetFieldValue_ChartViewThreeSucess(CDataBaseHelper &WorkDataBase, CMD_GP_ChartViewThreeSucess* pChartViewThreeSucess);
	//检测客户端登陆
	inline LONG helper_AllowClientLogon(BYTE cbClientType,BYTE cbUserType,BYTE cbUserLevel);

};

//////////////////////////////////////////////////////////////////////////

#endif