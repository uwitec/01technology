#ifndef GLOBAL_SERVICE_HEAD_FILE
#define GLOBAL_SERVICE_HEAD_FILE

#pragma once
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////

//数据库名字
const TCHAR szGameUserDB[]=TEXT("GServerInfo");						//用户数据库
const TCHAR	szTreasureDB[]=TEXT("GServerInfo");						//游戏数据库
const TCHAR szServerInfoDB[]=TEXT("GServerInfo");					//房间数据库

//数据库表名
const TCHAR szTableScore[]=TEXT("GameScore");							//游戏额度表
const TCHAR szTableAccounts[]=TEXT("GameUserInfo");						//用户帐户表

//////////////////////////////////////////////////////////////////////////

//数据库信息
struct tagDataBaseInfo
{
	WORD							wDataBasePort;						//数据库端口
	DWORD							dwDataBaseAddr;						//数据库地址
	TCHAR							szDataBasePipeName[32];				//数据库通道
	TCHAR							szDataBaseUser[32];					//数据库用户
	TCHAR							szDataBasePass[32];					//数据库密码
	TCHAR							szDataBaseName[32];					//数据库名字
};

//////////////////////////////////////////////////////////////////////////

//GameService属性
struct tagGameServiceAttrib
{
	WORD							wKindID;							//名称号码
	WORD							wChairCount;						//椅子数目
	WORD							wSupporType;						//支持类型
	DWORD							dwServerVersion;					//版本信息
	DWORD							dwMaxClientVersion;					//版本信息
	DWORD							dwLessClientVersion;				//版本信息
	TCHAR							szDescription[128];					//模块描述
	TCHAR							szKindName[KIND_LEN];				//类型名字
	TCHAR							szServerModuleName[MODULE_LEN];		//模块名字
	WORD							wProcessType;						//进程类型
	WORD							wBetRegionCount;					//投注区域数目
	BOOL							bCheckCellScoreBeforeStartGame;		//检查额度开始游戏前
	BOOL							bCheckSameIP;						//检查相同IP
};

//GameService配置
struct tagGameServiceOption
{
	//房间属性
	WORD							wTypeID;							//类型标识
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间号码
	WORD							wStationID;							//站点号码
	WORD							wTableCount;						//桌子数目
	WORD							wServerType;						//房间类型
	WORD							wServerPort;						//房间端口
	DWORD							dwServerAddr;						//房间地址
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称

	//额度限制
	DWORD							dwCellScore;						//单位额度
	DWORD							dwHighScore;						//最高额度
	DWORD							dwLessScore;						//最低额度

	//AI属性
	WORD							wAIUserCount;						//机器人数目
	BYTE							cbAILevel;							//机器人智能等级

	//扩展配置
	WORD							wMaxConnect;						//最大连接
	DWORD							dwUserRight;						//附加权限
	tagOptionBuffer					OptionBuffer;						//配置缓冲

	//抽水
	DECIMAL							decTaxRate;							//CT++
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif