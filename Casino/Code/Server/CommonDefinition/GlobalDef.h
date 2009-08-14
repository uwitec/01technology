#ifndef GLOBAL_DEF_HEAD_FILE
#define GLOBAL_DEF_HEAD_FILE

#pragma once
#pragma pack(push)
#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////////
//常量定义

const TCHAR	szProductName[]=TEXT("Casino");								//游戏名字
const TCHAR szSystemRegKey[]=TEXT("Casino");							//注册主键


/////////////////////////////////////////////////////////////////////////////////////////
//游戏标识

//通用游戏
#define ID_PLAZA						0									//大厅
//对战
#define ID_B_HOE						1									//锄大地
#define ID_B_LAND						2									//斗地主
#define ID_B_UNCOVERPIG					3									//拱猪
#define ID_B_BACK_JACK					4									//黑杰克
#define ID_B_UPGRADE					5									//拖拉机
#define ID_B_SPARROW					6							  		//麻将
#define ID_B_SHOWHAND					7									 //梭哈
#define ID_B_THREELEAF					8									//诈金花
#define ID_B_SICHUAN_LAND				9									//四川斗地主
//视频
#define ID_V_DRAGONTIGER				101									//龙虎
#define ID_V_BACCARAT					102									//百家乐
#define ID_V_SUPER6						103									//Super6
#define ID_V_SICBO						104									//骰宝
#define ID_V_ROULETTE					105									//轮盘
//电子
#define ID_P_SLOT_COOLFRUIT 	 		201									//水果拉霸机
#define ID_P_SLOT_POKER		 	 		202									//扑克拉霸机
#define ID_P_SLOT_MAHJONGBALL		 	203									//筒子拉霸机
#define ID_P_SLOT_SOCCER		 		204									//足球拉霸机
#define ID_P_SLOT_STAR97				205									//明星97拉霸机

#define ID_P_VIDEOPOKER_7PK				221									//7PK电动扑克
#define ID_P_VIDEOPOKER_5PK				222									//5PK电动扑克
#define ID_P_VIDEOPOKER_5PK1			223									//百搭二王电动扑克
#define ID_P_VIDEOPOKER_5PK2			224									//花牌电动扑克

#define ID_P_BIGGAMEMACHINE_FRUIT				241							//水果大转轮
#define ID_P_BIGGAMEMACHINE_LUCKYNUMBER			242							//数字大转轮
#define ID_P_BIGGAMEMACHINE_GUESSTRAIN			243							//钻石列车
#define ID_P_BIGGAMEMACHINE_MONSTERLEGEND		244							//圣兽传说
#define ID_P_BIGGAMEMACHINE_HORSERACE 			245							//跑马

#define ID_P_TABLEGAME_BLACKJACK		261									//21点
#define ID_P_TABLEGAME_WAR				262									//斗大
#define ID_P_TABLEGAME_REDDOG			263									//红狗
#define ID_P_TABLEGAME_ROULETTE			264									//轮盘
#define ID_P_TABLEGAME_DRAGONTIGER		265									//龙虎
#define ID_P_TABLEGAME_BACCARAT			266									//百家乐
#define ID_P_TABLEGAME_SICBO			267									//骰宝

//彩票
#define ID_L_MARKSIX 	 				301									//六合彩

struct GameProcessItem
{
	WORD	wProcessType;
	TCHAR*	szTypeName;
	TCHAR*	szModuleName;
};
//游戏表
const static GameProcessItem g_GameProcessItem[] = 
{
	{ID_B_HOE,			TEXT("对战-锄大地"),		TEXT("B_Hoe.dll")},
	{ID_B_LAND,			TEXT("对战-斗地主"),		TEXT("B_Land.dll")},
	{ID_B_UNCOVERPIG,	TEXT("对战-拱猪"),			TEXT("B_UncoverPig.dll")},
	{ID_B_BACK_JACK,	TEXT("对战-黑杰克"),		TEXT("B_BlackJack.dll")},
	{ID_B_UPGRADE,		TEXT("对战-升级"),			TEXT("B_UpGrade.dll")},
	{ID_B_SPARROW,		TEXT("对战-麻将"),			TEXT("B_Sparrow.dll")},
	{ID_B_SHOWHAND,		TEXT("对战-梭哈"),			TEXT("B_ShowHand.dll")},
	{ID_B_THREELEAF,	TEXT("对战-诈金花"),		TEXT("B_GoldFlower.dll")},
	{ID_B_SICHUAN_LAND,	TEXT("对战-四川斗地主"),	TEXT("B_SiChuan_Land.dll")},

	{ID_V_DRAGONTIGER,	TEXT("视频-龙虎"),			TEXT("V_DragonTiger.dll")},
	{ID_V_BACCARAT,		TEXT("视频-百家乐"),		TEXT("V_Baccarat.dll")},
	{ID_V_SUPER6,		TEXT("视频-Super6"),		TEXT("V_Super6.dll")},
	{ID_V_SICBO,		TEXT("视频-骰宝"),			TEXT("V_Sicbo.dll")},
	{ID_V_ROULETTE,		TEXT("视频-轮盘"),			TEXT("V_Roulette.dll")},

	{ID_P_SLOT_COOLFRUIT,	TEXT("电子-水果拉霸机"),		TEXT("P_Slot_CoolFruit.dll")},
	{ID_P_SLOT_POKER,		TEXT("电子-扑克拉霸机"),		TEXT("P_Slot_Poker.dll")},
	{ID_P_SLOT_MAHJONGBALL, TEXT("电子-筒子拉霸机"),		TEXT("P_Slot_MahjongBall.dll")},
	{ID_P_SLOT_SOCCER,		TEXT("电子-足球拉霸机"),		TEXT("P_Slot_Soccer.dll")},
	{ID_P_SLOT_STAR97,		TEXT("电子-明星97拉霸机"),		TEXT("P_Slot_Star97.dll")},
	{ID_P_VIDEOPOKER_7PK,	TEXT("电子-7PK电动扑克"),		TEXT("P_VideoPoker_7PK.dll")},
	{ID_P_VIDEOPOKER_5PK,	TEXT("电子-5PK电动扑克"),		TEXT("P_VideoPoker_5PK.dll")},
	{ID_P_VIDEOPOKER_5PK1,	TEXT("电子-百搭二王电动扑克"),	TEXT("P_VideoPoker_5PK1.dll")},
	{ID_P_VIDEOPOKER_5PK2,	TEXT("电子-花牌电动扑克"),		TEXT("P_VideoPoker_5PK2.dll")},
	{ID_P_BIGGAMEMACHINE_FRUIT,			TEXT("电子-水果大转轮"),	TEXT("P_BigGameMachine_Fruit.dll")},
	{ID_P_BIGGAMEMACHINE_LUCKYNUMBER,	TEXT("电子-数字大转轮"),	TEXT("P_BigGameMachine_LuckyNumber.dll")},
	{ID_P_BIGGAMEMACHINE_GUESSTRAIN,	TEXT("电子-钻石列车"),		TEXT("P_BigGameMachine_GuessTrain.dll")},
	{ID_P_BIGGAMEMACHINE_MONSTERLEGEND, TEXT("电子-圣兽传说"),		TEXT("P_BigGameMachine_MonsterLegend.dll")},
	{ID_P_BIGGAMEMACHINE_HORSERACE,		TEXT("电子-跑马"),			TEXT("P_BigGameMachine_HorseRace.dll")},
	{ID_P_TABLEGAME_BLACKJACK,	TEXT("电子-黑杰克"),		TEXT("P_TableGame_BlackJack.dll")},
	{ID_P_TABLEGAME_WAR,		TEXT("电子-斗大"),			TEXT("P_TableGame_War.dll")},
	{ID_P_TABLEGAME_REDDOG,		TEXT("电子-红狗"),			TEXT("P_TableGame_RedDog.dll")},
	{ID_P_TABLEGAME_ROULETTE,	TEXT("电子-轮盘"),			TEXT("P_TableGame_Roulette.dll")},
	{ID_P_TABLEGAME_DRAGONTIGER,TEXT("电子-龙虎"),			TEXT("P_TableGame_DragonTiger.dll")},
	{ID_P_TABLEGAME_BACCARAT,	TEXT("电子-百家乐"),		TEXT("P_TableGame_Baccarat.dll")},
	{ID_P_TABLEGAME_SICBO,		TEXT("电子-骰宝"),			TEXT("P_TableGame_Sicbo.dll")},

	{ID_L_MARKSIX,		  TEXT("彩票-六合彩"),				TEXT("L_MarkSix.dll")},
};
//游戏类型掩码
const DWORD dwGameTypeMark_Battle = 0x1;
const DWORD dwGameTypeMark_Video  = 0x2;
const DWORD	dwGameTypeMark_Pokie  = 0x4;
const DWORD dwGameTypeMark_Lottery= 0x8;
//判断对战游戏
inline bool IsBattleGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_B_HOE && wProcessType <= ID_B_SICHUAN_LAND)
		return true;
	else
		return false;
}
//判断视频游戏
inline bool IsVideoGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_V_DRAGONTIGER && wProcessType <= ID_V_ROULETTE)
		return true;
	else
		return false;
}
//判断电子游戏
inline bool IsPokieGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_P_SLOT_COOLFRUIT && wProcessType <= ID_P_TABLEGAME_SICBO)
		return true;
	else
		return false;
}
//判断彩票游戏
inline bool IsLotteryGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_L_MARKSIX && wProcessType <= ID_L_MARKSIX)
		return true;
	else
		return false;
}
//查找游戏类型名称
inline const TCHAR* GetGameTypeName(WORD wProcessType)
{
	for(int i=0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		if(g_GameProcessItem[i].wProcessType == wProcessType)
			return g_GameProcessItem[i].szTypeName;
	}
	return "";
}
//查找游戏类型名称
inline const TCHAR* GetGameTypeNameByModuleName(TCHAR* szModuleName)
{
	for(int i=0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		if(_tcscmp(g_GameProcessItem[i].szModuleName,szModuleName) == 0)
			return g_GameProcessItem[i].szTypeName;
	}
	return "";
}
/////////////////////////////////////////////////////////////////////////////////////////
//宏定义

#define MAX_CHAIR						8										//最大游戏者
#define MAX_CHAT_LEN					512										//聊天长度
#define INVALID_TABLE					((WORD)(-1))							//无效桌子号
#define INVALID_CHAIR					((WORD)(-1))							//无效椅子号
#define MAIN_DLG_CLASSNAME				TEXT("CasinoGamePlaza")					//窗口注册名

//端口定义
#define PORT_LOGON_SERVER				9000									//PlazaServer
#define PORT_CENTER_SERVER				9010									//CenterServer

//网络数据定义
#define SOCKET_VER						68										//数据包版本
#define SOCKET_PACKAGE					2048									//最大网络包
#define SOCKET_BUFFER					(sizeof(CMD_Head)+SOCKET_PACKAGE+2*sizeof(DWORD))

/////////////////////////////////////////////////////////////////////////////////////////

//数据包结构信息
struct CMD_Info
{
	WORD								wDataSize;								//数据大小
	BYTE								cbCheckCode;							//效验字段
	BYTE								cbMessageVer;							//版本标识
};

//数据包命令信息
struct CMD_Command
{
	WORD								wMainCmdID;								//主命令码
	WORD								wSubCmdID;								//子命令码
};

//数据包传递包头
struct CMD_Head
{
	CMD_Info							CmdInfo;								//基础结构
	CMD_Command							CommandInfo;							//命令信息
};

//网络数据包缓冲
struct CMD_Buffer
{
	CMD_Head							Head;									//数据包头
	BYTE								cbBuffer[SOCKET_PACKAGE];				//数据缓冲
};

//内核命令码
#define MDM_KN_COMMAND					0										//内核命令
#define SUB_KN_DETECT_SOCKET			1										//检测命令

//检测结构信息
struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;						//发送时间
	DWORD								dwRecvTickCount;						//接收时间
};


//////////////////////////////////////////////////////////////////////////

//长度宏定义
#define TYPE_LEN						32										//种类长度
#define KIND_LEN						32										//类型长度
#define STATION_LEN						32										//站点长度
#define SERVER_LEN						32										//房间长度
#define MODULE_LEN						128										//进程长度

//游戏类型
#define GAME_GENRE_SCORE				0x0001									//点值类型
#define GAME_GENRE_GOLD					0x0002									//金币类型
#define GAME_GENRE_MATCH				0x0004									//比赛类型
#define GAME_GENRE_EDUCATE				0x0008									//训练类型

//游戏类型结构
struct tagGameType
{
	WORD								wSortID;								//排序号码
	WORD								wTypeID;								//种类号码
	BYTE								nbImageID;								//圖標號碼
	TCHAR								szTypeName[TYPE_LEN];					//种类名字
};

//游戏名称结构
struct tagGameKind
{
	WORD								wSortID;								//排序号码
	WORD								wTypeID;								//类型号码
	WORD								wKindID;								//名称号码
	DWORD								dwOnLineCount;							//在线数目
	DWORD								dwMaxVersion;							//最新版本
	WORD								wProcessType;							//进程类型
	WORD								wTableCount;							//桌子数目
	DWORD								dwCellScore;							//单位额度
	DWORD								dwHighScore;							//最高额度
	DWORD								dwLessScore;							//最低额度
	DOUBLE								fTaxRate;								//抽水比
	TCHAR								szKindName[KIND_LEN];					//游戏名字
	//AI属性
	WORD								wAIUserCount;							//机器人数目
	BYTE								cbAILevel;								//机器人智能等级
};

//游戏进程结构
struct tagGameProcess
{
	WORD								wSortID;								//排序号码
	WORD								wTypeID;								//类型号码
	WORD								wKindID;								//名称号码
	WORD								wServerPort;							//房间端口
	DWORD								dwServerAddr;							//房间地址
	DWORD								dwMaxVersion;							//最新版本
	DWORD								dwOnLineCount;							//在线数目
	TCHAR								szKindName[KIND_LEN];					//游戏名字
};

//游戏站点结构
struct tagGameStation
{
	WORD								wSortID;								//排序号码
	WORD								wKindID;								//名称号码
	WORD								wJoinID;								//挂接号码
	WORD								wStationID;								//站点号码
	TCHAR								szStationName[STATION_LEN];				//站点名称
};

//游戏房间列表结构
struct tagGameServer
{
	WORD								wSortID;								//排序号码
	WORD								wKindID;								//游戏号码
	WORD								wTypeID;								//類型号码
	WORD								wServerID;								//服務号码
	WORD								wStationID;								//站点号码
	WORD								wServerPort;							//房间端口
	DWORD								dwServerAddr;							//房间地址
	DWORD								dwOnLineCount;							//在线人数
	WORD								wMaxConnect;							//最大连接
	TCHAR								szServerName[SERVER_LEN];				//房间名称
};
//游戏桌列表结构
struct tagGameTable 
{
	WORD								wTableID;								//桌号码
	WORD								wKindID;								//游戏号码
	WORD								wTypeID;								//類型号码
	WORD								wTableNumber;							//桌索引
	BYTE								nbTalbeParam[128];						//桌参数
};
//视频游戏桌参数结构
struct tagVideoGameTableParam 
{
	DWORD								dwHighScore;
	DWORD								dwCellScore;
	DOUBLE								fMaxUserRevenueSharing;
	WORD								wStateKernelServerPort;
	DWORD								dwStateKernelServerAddr;
};
//////////////////////////////////////////////////////////////////////////

//用户状态定义
#define US_NULL							0x00									//没有状态
#define US_FREE							0x01									//站立状态
#define US_SIT							0x02									//坐下状态
#define US_READY						0x03									//同意状态
#define US_LOOKON						0x04									//旁观状态
#define US_PLAY							0x05									//游戏状态
#define US_OFFLINE						0x06									//断线状态

//长度宏定义
#define NAME_LEN						32										//名字长度
#define PASS_LEN						33										//密码长度

//用户等级
#define USERLEVEL_GAMEUSER				5										//会员
//用户类型
#define USERTYPE_GAME					0										//普通帐号
#define USERTYPE_SUB					1										//子帐号
#define USERTYPE_DEALER					2										//Dealer帐号
//客户端类型
#define CLIENTTYPE_GAME					0										//游戏端
#define CLIENTTYPE_MAN					1										//管理端
#define CLIENTTYPE_SERVICE				2										//客户中心
#define CLIENTTYPE_CONTROL				3										//控制端

//用户信用额度信息
struct tagUserScore
{
	DOUBLE								fScore;
};

//用户状态信息
struct tagUserStatus
{
	WORD								wTableID;								//桌子号码
	WORD								wChairID;								//椅子位置
	WORD								wNetDelay;								//网络延时
	BYTE								cbUserStatus;							//用户状态
};

//用户基本信息结构
struct tagUserInfoHead
{
	//用户属性
	WORD								wFaceID;								//头像索引
	DWORD								dwUserID;								//用户 I D
	TCHAR								szName[NAME_LEN]; 
	//用户运行时信息
	tagUserScore						UserScore;								//信用额度
	tagUserStatus						UserStatus;								//用户状态
};

//用户信息结构
struct tagUserData
{
	  DWORD								dwUserID;								//用户ID
	  DWORD								dwParentUserID;							//用户父ID-上一级
	  DWORD								dwHighUserID0;							//用户父ID-公司
      DWORD								dwHighUserID1;							//用户父ID-大股东
	  DWORD								dwHighUserID2;							//用户父ID-股东
	  DWORD								dwHighUserID3;							//用户父ID-总代理
	  DWORD								dwHighUserID4;							//用户父ID-代理
	  TCHAR								szAccount[NAME_LEN];					//用户帐号
	  BYTE								cbLevel;								//用户级别
      BYTE								cbState;								//用户状态
	  TCHAR								szName[NAME_LEN];						//用户名称
      BYTE								cbFaceID;								//用户头像
	  BYTE								cbStateCongealFlag;						//用户状态 
      DWORD								dwUserRight;							//用户游戏权限
	  DWORD								dwMasterRight;							//用户管理权限
      DOUBLE							fMidScore;								//用户体系额度
	  DOUBLE							fLowScore;								//会员额度
	  DOUBLE							fOccupancyRate;							//输赢占成比
      BYTE								cbOccupancyRate_NoFlag;					//输赢不占成标记
	  DOUBLE							fLowOccupancyRate_Max;					//输赢下线最高占成比
      BYTE								cbLowOccupancyRate_Max_NoFlag;			//输赢下线最高不占成比标记
      DOUBLE							fLowOccupancyRate_Min;					//输赢下线最低占成比
      BYTE								cbLowOccupancyRate_Min_NoFlag;			//输赢下线最低占成比
	  DOUBLE							fTaxOccupancyRate;					//抽水占成比	
	  DOUBLE							fRollbackRate;							//洗码比
      DWORD								dwBetLimit;								//游戏上下限
	  DOUBLE							fRegisterTime;							//注册时间
	  WORD								wLessUserCount;							//下线人数

	 tagUserScore						UserScore;								//信用额度
};
//子用户信息结构
struct tagSubUserData
{
	DWORD								dwUserID;								//用户ID
	DWORD								dwParentUserID;							//用户父ID
	DWORD								dwHighUserID0;							//用户父ID
	DWORD								dwHighUserID1;							//用户父ID
	DWORD								dwHighUserID2;							//用户父ID
	DWORD								dwHighUserID3;							//用户父ID
	DWORD								dwHighUserID4;							//用户父ID
	TCHAR								szAccount[NAME_LEN];					//用户帐号
	BYTE								cbState;								//用户状态
	TCHAR								szName[NAME_LEN];						//用户名称
	BYTE								cbStateCongealFlag;						//用户状态
	DWORD								dwUserRight;							//用户游戏权限
	DWORD								dwMasterRight;							//用户管理权限
	DOUBLE								fRegisterTime;							//注册时间
};
//////////////////////////////////////////////////////////////////////////

//机器序列号结构
struct tagClientSerial
{
	DWORD								dwComputerID[3];						//机器序列
};

//配置缓冲结构
struct tagOptionBuffer
{
	BYTE								cbBufferLen;							//数据长度
	BYTE								cbOptionBuf[32];						//设置缓冲
};

/////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#include "GlobalError.h"
#include "GlobalTrace.h"

#endif