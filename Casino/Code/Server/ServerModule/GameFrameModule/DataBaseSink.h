#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"
#include "..\..\CommonDefinition\GlobalOperation.h"
//////////////////////////////////////////////////////////////////////////
//操作者
struct tagOperationUserInfo
{
	DWORD			dwOperationUserID;					//操作用户ID
	BYTE			cbOperationUserType;				//操作用户类型
	TCHAR			szOperationAccount[NAME_LEN];		//操作帐号
	DWORD			dwIPAddress;						//操作地址
};
//////////////////////////////////////////////////////////////////////////
//结构定义

//数据库参数
struct tagDataBaseSinkParameter
{
	tagDataBaseInfo						* pGameUserDBInfo;				//连接信息
	tagDataBaseInfo						* pGameScoreDBInfo;				//连接信息
	tagGameServiceAttrib				* pGameServiceAttrib;			//服务属性
	tagGameServiceOption				* pGameServiceOption;			//配置信息
};

//////////////////////////////////////////////////////////////////////////
//数据库请求数据包

//数据库请求标识
#define	DBR_GR_LOGON_BY_ACCOUNT			1								//帐户登陆
#define	DBR_GR_READ_ACCOUNT				2								//帐户读取
#define DBR_GR_WRITE_GAMEROUND			3								//写入游戏局记录
#define DBR_GR_WRITE_USERCHART			4								//写入用户报表
#define DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE		5					//申请游戏局记录-检测信用额度
#define DBR_GR_ALLOC_GAMEROUND			6								//申请游戏局记录
#define DBR_GR_RESET_BETSCORE			7								//复位投注
#define DBR_GR_INSERT_BETSCORE			8								//插入投注
#define DBR_GR_KICK_USER				9								//踢出房间
#define DBR_GR_READ_AIUSER				20								//装载机器人

//数据库输出标识
#define DBR_GR_LOGON_SUCCESS			100								//登陆成功
#define DBR_GR_LOGON_ERROR				101								//登陆失败
#define DBR_GR_READACCOUNT_SUCCESS		200								//读取成功
#define DBR_GR_READACCOUNT_ERROR		201								//读取失败
#define DBR_GR_USERSCORE				400								//用户信用额度
#define DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE_RESULT		500			//申请游戏局记录-检测信用额度结果
#define DBR_GR_ALLOC_GAMEROUND_RESULT	600								//申请游戏局记录
#define DBR_GR_READ_AIAGENTUSER_SUCCESS	2000							//装载代理机器人成功
#define DBR_GR_READ_AIGAMEUSER_SUCCESS	2001							//装载玩家机器人成功
#define DBR_GR_READ_AIUSER_FINISH		2002							//装载机器人完成

//帐户登陆
struct DBR_GR_LogonByAccount
{
	DWORD								dwClientIP;						//连接地址
	TCHAR								szAccounts[NAME_LEN];			//登录帐号
	TCHAR								szPassWord[PASS_LEN];			//登录密码
};

//帐户读取
struct DBR_GR_ReadUser
{
	DWORD								dwUserID;						//用户 I D
};

//登陆成功
struct DBR_GR_LogonSuccess
{
	//基本信息
	tagUserData							UserData;
	DECIMAL								decOccupancyRate;					//占成比
	DECIMAL								decTaxOccupancyRate;			//抽水占成比
	DECIMAL								decRollbackRate;					//洗码比
	DWORD								dwGameRight;						//游戏权限
	BYTE								cbUserType;							//用户类型
	DWORD								dwRealUserID;						//用户ID
};

//登陆失败
struct DBR_GR_LogonError
{
	WORD								wErrorCode;						//错误代码
};
//读取成功
struct DBR_GR_ReadUserSuccess
{
	//基本信息
	tagUserData							UserData;
	DECIMAL								decOccupancyRate;					//占成比
	DECIMAL								decTaxOccupancyRate;			//抽水占成比
	DECIMAL								decRollbackRate;					//洗码比
	DWORD								dwGameRight;						//游戏权限
	BYTE								cbUserType;							//用户类型
	DWORD								dwRealUserID;						//用户ID
};
typedef CArray<DBR_GR_ReadUserSuccess,DBR_GR_ReadUserSuccess> Array_DBR_GR_ReadUserSuccess;

//读取失败
struct DBR_GR_ReadUserError
{
	WORD								wErrorCode;						//错误代码
};
//写入游戏局记录
struct DBR_GR_WriteGameRound
{
	ULONGLONG							ullGameRoundID;					//游戏局记录ID
	ULONGLONG							ullStartTime;					//游戏开始时间
	BYTE								cbCalculatedFlag;				//结算标记
	BYTE								cbValidFlag;					//有效标记
	WORD								wServerID;						//GameProjectID
	WORD								wTableID;						//桌子ID
	BYTE								cbEndReason;					//游戏结束原因
	WORD								wEndDataSize;					//游戏结束信息大小
	BYTE								cbEndData[1536];				//游戏结束信息
};

//写入用户报表
struct DBR_GR_WriteUserChart
{
	BYTE								cbUpdateUserScoreFlag;			//修改用户信用额度

	ULONGLONG							ullGameRoundID;					//游戏局记录ID

	DWORD								dwUserID;						//用户ID
	WORD								wChairID;						//椅子ID

	DECIMAL								decTotalBetScore;				//总投注
	DECIMAL								decTotalWinScore;				//总派彩
	DECIMAL								decTotalTaxScore;				//总抽水

	DECIMAL								decWinScoreOccupancy_High;		//上线占成派彩
	DECIMAL								decWinScoreOccupancy_Self;		//自身占成派彩
	DECIMAL								decWinScoreOccupancy_Less;		//下线占成派彩

	DECIMAL								decTaxScoreOccupancy_High;		//上线占成抽水
	DECIMAL								decTaxScoreOccupancy_Self;		//自身占成抽水
	DECIMAL								decTaxScoreOccupancy_Less;		//下线占成抽水

	DECIMAL								decValidBetScore_Total;			//总洗码货量
	DECIMAL								decValidBetScore_High;			//上线洗码货量
	DECIMAL								decValidBetScore_HighRollback;	//上线洗码
	DECIMAL								decValidBetScore_Less;			//下线洗码货量
	DECIMAL								decValidBetScore_LessRollback;	//下线洗码

	DECIMAL								decPaidScore_High;				//上线交收
	DECIMAL								decPaidScore_Less;				//下线交收

	WORD								wDetailBetScoreSize;			//投注详细大小
	BYTE								cbDetailBetScore[1536];			//投注详细
};
//申请游戏局记录-检测信用额度
struct DBR_GR_AllocGameRoundAndCheckBetScore
{
	ULONGLONG							ullStartTime;					//游戏开始时间
	WORD								wServerID;						//服务ID
	WORD								wTableID;						//桌子ID
	DWORD								dwUserID[8];					//用户ID	
	DECIMAL								decBetScore[8];					//信用额度
};
//申请游戏局记录-检测信用额度结果
struct DBR_GR_AllocGameRoundAndCheckBetScoreResult
{
	ULONGLONG							ullGameRoundID;					//游戏局ID
	WORD								wServerID;						//服务ID
	WORD								wTableID;						//桌子ID
	DWORD								dwUserID[8];					//用户ID	
	BYTE								bCheckScoreFlag[8];				//检测标记
	DECIMAL								decUserScore[8];				//信用额度
};
//申请游戏局记录
struct DBR_GR_AllocGameRound
{
	ULONGLONG							ullStartTime;					//游戏开始时间
	WORD								wServerID;						//服务ID
	WORD								wTableID;						//桌子ID
};
//申请游戏局记录
struct DBR_GR_AllocGameRoundResult
{
	ULONGLONG							ullGameRoundID;					//游戏局ID
	WORD								wServerID;						//服务ID
	WORD								wTableID;						//桌子ID
};
//复位投注
struct DBR_GR_ResetBetScore
{
	WORD								wServerID;						//服务ID
	WORD								wTableID;						//桌子ID
};
//插入投注
struct DBR_GR_InsertBetScore
{
	DWORD								dwUserID;						//用户ID	
	WORD								wServerID;						//服务ID
	WORD								wTableID;						//桌子ID
	DECIMAL								decTotalBetScore;				//信用额度
};
//踢出房间
struct DBR_GR_KickUser
{
	tagOperationUserInfo				Operator;							//操作者
	DWORD								dwUserID;							//操作用户ID
	BYTE								cbUserType;							//操作用户类型
	TCHAR								szAccount[NAME_LEN];				//操作帐号
	tagOperationData_KickUser			RequestData;						//请求数据
};
//用户额度
struct DBR_GR_UserScore
{
	DWORD								dwUserID;						//用户ID	
	DECIMAL								decScore;						//信用额度
};
//////////////////////////////////////////////////////////////////////////

//数据库引擎钩子
class CDataBaseSink : public IDataBaseSink
{
	//配置变量
protected:
	tagDataBaseInfo						* m_pGameUserDBInfo;			//连接信息
	tagDataBaseInfo						* m_pGameScoreDBInfo;			//连接信息
	tagGameServiceAttrib				* m_pGameServiceAttrib;			//服务属性
	tagGameServiceOption				* m_pGameServiceOption;			//配置信息

	//接口变量
protected:
	IEventService						* m_pIEventService;				//事件服务

	//组件变量
protected:
	CDataBaseHelper						m_GameUserDB;					//用户数据库
	CDataBaseHelper						m_GameScoreDB;					//游戏数据库
	CQueueServiceEvent					m_AttemperEvent;				//调度通知

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
	//WriteGameRound
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//配置函数
	bool InitDataBaseSink(tagDataBaseSinkParameter * pDataBaseSinkParameter, IUnknownEx * pIUnknownEx);

	//处理函数
private:
	//登录请求
	bool OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//读取请求
	bool OnRequestReadUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//写入游戏局记录请求
	bool OnRequestWriteGameRound(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//写入用户报表记录请求
	bool OnRequestWriteUserChart(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//申请游戏局记录-检测信用额度请求
	bool OnRequestAllocGameRoundAndCheckBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//申请游戏局记录
	bool OnRequestAllocGameRound(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//复位投注
	bool OnRequestResetBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//插入投注
	bool OnRequestInsertBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//踢出房间
	bool OnRequestKickUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//装载机器人
	bool OnRequestReadAIUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//存储过程
protected:
	//帐号存储过程
	LONG SPLogonByAccount(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP);
	//帐号读取过程
	LONG SPReadUser(DWORD dwUserID);
	//写游戏局记录存储过程
	LONG SPWriteGameRound(DBR_GR_WriteGameRound* pWriteGameRound);
	//写用户报表记录存储过程
	LONG SPWriteUserChart(DBR_GR_WriteUserChart* pWriteUserChart);
	//申请游戏局记录-检测信用额度存储过程
	ULONGLONG SPAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScore * pAllocGameRoundAndCheckBetScore);
	//申请游戏局记录存储过程
	ULONGLONG SPAllocGameRound(DBR_GR_AllocGameRound * pAllocGameRound);
	//复位投注存储过程
	LONG SPResetBetScore(DBR_GR_ResetBetScore * pResetBetScore);
	//插入投注存储过程
	LONG SPInsertBetScore(DBR_GR_InsertBetScore * pInsertBetScore);
	//写入操作记录存储过程
	LONG SPInsertOperationInfo(tagOperationData& OperationData);
	//读取游戏节点名字存储过程
	LONG SPReadGameTreeNodeName(WORD wTypeID,
		WORD wKindID,
		WORD wServerID);
	//读取机器人帐号
	LONG SPReadAIUser(WORD wAIUserCount);

	//辅助函数
private:
	//读取帐号信息
	bool helper_DBGetFieldValue_UserData(CDataBaseHelper &WorkDataBase, 
		tagUserData& UserData,
		DECIMAL& decOccupancyRate,
		DECIMAL& decTaxOccupancyRate,
		DECIMAL& decRollbackRate,
		DWORD& dwGameRight,
		DWORD* pdwRealUserID = NULL,
		BYTE* pcbUserType = NULL);
	//读取申请游戏局记录-检测信用额度结果信息
	bool helper_DBGetFieldValue_AllocGameRoundAndCheckBetScoreResult(CDataBaseHelper &WorkDataBase, 
		DBR_GR_AllocGameRoundAndCheckBetScoreResult& AllocGameRoundAndCheckBetScoreResult);
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

	//读取游戏节点名字
	bool helper_DBGetFieldValue_ReadGameTreeNodeName(CDataBaseHelper &WorkDataBase, 
		TCHAR *szTypeName,
		TCHAR *szKindName,
		TCHAR *szServerName);
	//读取信用额度
	bool helper_DBGetFieldValue_UserScore(CDataBaseHelper &WorkDataBase, 
		DBR_GR_UserScore *pUserScore);
	//读取AI帐号信息
	bool helper_DBGetFieldValue_AIUser(CDataBaseHelper &WorkDataBase,
		Array_DBR_GR_ReadUserSuccess &arrUserAgent,
		Array_DBR_GR_ReadUserSuccess &arrUserGame);
};

//////////////////////////////////////////////////////////////////////////

#endif