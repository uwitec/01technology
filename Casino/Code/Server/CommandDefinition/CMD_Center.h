#ifndef CMD_CENTER_HEAD_FILE
#define CMD_CENTER_HEAD_FILE
#include "../CommonDefinition\GlobalService.h"
//
//游戏列表命令码
//
#pragma pack(push)
#pragma pack(1)

#define MDM_CS_SERVER_LIST				1								//列表信息

#define SUB_CS_GET_SERVER_LIST			1								//请求列表

#define SUB_CS_LIST_INFO				100								//列表信息
#define SUB_CS_LIST_TYPE				101								//类型列表
#define SUB_CS_LIST_KIND				102								//种类列表
#define SUB_CS_LIST_STATION				103								//站点列表
#define SUB_CS_LIST_SERVER				104								//房间列表
#define SUB_CS_LIST_FINISH				105								//列表完成

//列表信息
struct CMD_CS_ListInfo
{
	DWORD								dwTypeCount;					//类型数目
	DWORD								dwKindCount;					//种类数目
	DWORD								dwServerCount;					//房间数目
	DWORD								dwStationCount;					//站点数目
};
//////////////////////////////////////////////////////////////////////////
//
//房间管理命令码
//

#define MDM_CS_SERVER_MANAGER			2								//房间管理

#define SUB_CS_REG_GAME_SERVER			1								//注册房间
#define SUB_CS_UNREG_GAME_SERVER		2								//注销房间
#define SUB_CS_SERVER_ONLINE_COUNT		3								//房间人数

//注册房间
struct CMD_CS_RegGameServer
{
	tagGameServer						GameServer;						//房间信息
};

//注销房间
struct CMD_CS_UnRegGameServer
{
	WORD								wKindID;						//名称号码
	WORD								wServerID;						//房间号码
};

//房间人数
struct CMD_CS_ServerOnLineCount
{
	WORD								wKindID;						//名称号码
	WORD								wServerID;						//房间号码
	DWORD								dwOnLineCount;					//在线人数
};

//////////////////////////////////////////////////////////////////////////

//服务器请求命令码
//
#define MDM_CS_SERVER_REQUEST				3								//信息

#define SUB_CS_GET_LOGONSERVERINFO			1								//PlazaServer信息

#define SUB_CS_GET_LOGONSERVERINFO_SUCCESS	101								//获取PlazaServer信息成功
#define SUB_CS_GET_LOGONSERVERINFO_FAILD	102								//获取PlazaServer信息失败

//请求数据库信息
struct CMD_CS_GetLogonServerInfo
{
	TCHAR							szLogonServerPass[256];				//启动服务密码
};

//数据库信息
struct CMD_CS_LogonServerInfo
{
	WORD							wDataBasePort;						//数据库端口
	DWORD							dwDataBaseAddr;						//数据库地址
	TCHAR							szDataBasePipeName[32];				//数据库用户
	TCHAR							szDataBaseUser[32];					//数据库用户
	TCHAR							szDataBasePass[32];					//数据库密码
	TCHAR							szMainPage[32];						//游戏主站
};

#define SUB_CS_GET_LOADERSERVERINFO			2								//裝載服务器信息

#define SUB_CS_GET_LOADERSERVERINFO_SUCCESS	201								//获取服务器信息成功
#define SUB_CS_GET_LOADERSERVERINFO_FAILD	202								//获取服务器信息失败

//请求数据库信息
struct CMD_CS_GetLoaderServerInfo
{
	WORD							wServerID;								//服務ID
	TCHAR							szLoaderServerPass[256];				//启动服务密码
};


//服务参数结构
struct CMD_CS_LoaderServerInfo
{
	TCHAR								szModuleName[MODULE_LEN];			//模块名字
	DWORD								dwCenterAddr;						//中心地址
	tagDataBaseInfo						GameUserDBInfo;						//数据配置
	tagDataBaseInfo						GameScoreDBInfo;					//数据配置
	tagGameServiceOption				GameServiceOption;					//服务配置
};

//////////////////////////////////////////////////////////////////////////
//更新信息命令码
#define MDM_CS_UPDATE_INFO				4									//更新信息

#define SUB_CS_UPDATE_SERVERUSERDATA	1									//更新用户信息
struct CMD_CS_ServerUserData
{
	DWORD								dwUserID;							//用户ID
	DECIMAL								decOccupancyRate;					//占成比
	BYTE								cbOccupancyRate_NoFlag;				//不占成标记
	DECIMAL								decTaxOccupancyRate;			//抽水占成比
	DECIMAL								decRollbackRate;					//洗码比
	DWORD								dwBetLimit;							//游戏上下限
};
//////////////////////////////////////////////////////////////////////////
//管理消息
#define MDM_CS_MANAGEMENT				5									//管理消息

//发送用户消息
#define SUB_CS_USER_MESSAGE				1									//用户消息
//消息数据包
struct SUB_CS_UserMessage
{
	DWORD							dwRealUserID;						//用户帐号
	BYTE							cbUserType;							//用户类型
	WORD							wMessageType;						//消息类型
	WORD							wErrorCode;							//错误消息代码
	WORD							wMessageLength;						//消息长度
	TCHAR							szContent[1024];					//消息内容
};
#pragma pack(pop)

#endif


//-----------------------------------------------
//					the end
//-----------------------------------------------
