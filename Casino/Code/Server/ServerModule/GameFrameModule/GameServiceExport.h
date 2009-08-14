#ifndef GAME_SERVICE_EXPORT_HEAD_FILE
#define GAME_SERVICE_EXPORT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//平台头文件
#include "..\..\Template\Template.h"
#include "..\..\CommonDefinition\GlobalDef.h"
#include "..\..\CommonDefinition\GlobalField.h"
#include "..\..\CommonDefinition\GlobalFrame.h"
#include "..\..\CommonDefinition\GlobalRight.h"
#include "..\..\CommonDefinition\GlobalService.h"
#include "..\..\CommonDefinition\GlobalAI.h"

//命令码文件
#include "..\..\CommandDefinition\CMD_Game.h"
#include "..\..\CommandDefinition\CMD_Plaza.h"
#include "..\..\CommandDefinition\CMD_Center.h"

//组件头文件
#include "..\..\Interface\IUnknownEx.h"
#include "..\..\ShareModule\SocketModule\SocketModule.h"
#include "..\..\ServerModule\KernelModule\KernelModule.h"

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifndef GAME_SERVICE_CLASS
	#ifdef  GAME_SERVICE_DLL
		#define GAME_SERVICE_CLASS _declspec(dllexport)
	#else
		#define GAME_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifdef _DEBUG
	#define GAME_SERVICE_DLL_NAME	TEXT("GameFrameModuleD.dll")				//组件 DLL 名字
#else
	#define GAME_SERVICE_DLL_NAME	TEXT("GameFrameModule.dll")					//组件 DLL 名字
#endif

//宏定义
#define IDI_MAX_TIME_ID					30									//极限定时器

//结束原因
#define GER_NORMAL						0x00								//常规结束
#define GER_DISMISS						0x01								//游戏解散
#define GER_USER_LEFT					0x02								//用户离开

#define INVALID_CALCULATEITEMPOS		0xffffffff							//无效结算单元位置

//////////////////////////////////////////////////////////////////////////
//枚举定义

//开始模式
enum enStartMode
{
	enStartMode_FullReady,				//满人开始
	enStartMode_AllReady,				//所有准备
	enStartMode_Symmetry,				//对称开始
	enStartMode_TimeControl,			//时间控制
};

//额度类型
enum enScoreKind
{
	enScoreKind_Win,					//胜
	enScoreKind_Lost,					//输
	enScoreKind_Draw,					//和
	enScoreKind_Flee,					//逃
};

//////////////////////////////////////////////////////////////////////////
//结构定义

//信用额度信息
struct tagScoreInfo
{
	DOUBLE								fScore;								//游戏信用额度
	enScoreKind							ScoreKind;							//额度类型
};

//用户规则
struct tagUserRule
{
	bool								bPassword;							//设置密码
	bool								bLimitWin;							//限制胜率
	bool								bLimitFlee;							//限制逃跑
	bool								bLimitScore;						//限制额度
	bool								bLimitDelay;						//限制延时
	bool								bCheckSameIP;						//效验地址
	WORD								wWinRate;							//胜率限制
	WORD								wFleeRate;							//限制逃跑
	WORD								wNetDelay;							//网络延时
	LONG								lMaxScore;							//最高额度 
	LONG								lLessScore;							//最低额度
	TCHAR								szPassword[PASS_LEN];				//桌子密码
};

//用户信息结构
struct tagServerUserData
{
	//属性信息
	tagUserData							UserData;							//用户信息
	BYTE								cbUserType;							//用户类型
	DWORD								dwRealUserID;						//用户ID

	DECIMAL								decOccupancyRate;					//占成比
	DECIMAL								decTaxOccupancyRate;			//抽水占成比
	DECIMAL								decRollbackRate;					//洗码比
	DWORD								dwGameRight;						//游戏权限

	//状态信息
	tagUserStatus						UserStatus;
};

//桌子参数结构
struct tagTableFrameParameter
{
	IUnknownEx							* pIGameServiceFrame;				//服务框架
	IUnknownEx							* pIGameServiceManager;				//服务管理
	IUnknownEx							* pIDataBaseEngine;					//数据库
	IUnknownEx							* pIServerUserManager;				//用户管理
	const tagGameServiceAttrib			* pGameServiceAttrib;				//桌子属性
	const tagGameServiceOption			* pGameServiceOption;				//配置参数
};

//服务参数结构
struct tagGameServiceParameter
{
	TCHAR								szModuleName[MODULE_LEN];			//模块名字
	DWORD								dwCenterAddr;						//中心地址
	tagDataBaseInfo						GameUserDBInfo;						//数据配置
	tagDataBaseInfo						GameScoreDBInfo;					//数据配置
	tagGameServiceOption				GameServiceOption;					//服务配置
};

//////////////////////////////////////////////////////////////////////////

#define VER_IServerUserItem INTERFACE_VERSION(2,1)
static const GUID IID_IServerUserItem={0x8fbde5db,0x99c5,0x4908,0x91,0xed,0x74,0xea,0xe4,0xc0,0xa7,0x8d};

//服务器用户接口
interface IServerUserItem : public IUnknownEx
{
	//信息接口
public:
	//是否激活
	virtual bool __cdecl IsActive()=NULL;
	//用户索引
	virtual WORD __cdecl GetUserIndex()=NULL;
	//获取地址
	virtual DWORD __cdecl GetClientIP()=NULL;
	//获取密码
	virtual LPCTSTR __cdecl GetPassword()=NULL;
	//获取规则
	virtual const tagUserRule * __cdecl GetUserRule()=NULL;
	//获取信用额度
	virtual  tagUserScore * __cdecl GetUserScore()=NULL;
	//获取信息
	virtual tagServerUserData * __cdecl GetUserData()=NULL;
	//获取父用户
	virtual IServerUserItem * __cdecl GetParent()=NULL;
	//设置父用户
	virtual bool  __cdecl SetParent(IServerUserItem* pParent)=NULL;

	//属性接口
public:
	//用户名字
	virtual LPCTSTR __cdecl GetAccounts()=NULL;
	//用户 I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//父用户 I D
	virtual DWORD __cdecl GetParentUserID()=NULL;
	//用户等级
	virtual BYTE __cdecl GetUserLevel()=NULL;
	//桌子号码
	virtual WORD __cdecl GetTableID()=NULL;
	//椅子号码
	virtual WORD __cdecl GetChairID()=NULL;
	//用户状态
	virtual BYTE __cdecl GetUserStatus()=NULL;
	//判断用户体系正确
	virtual bool __cdecl IsUserTreeFrameOK()=NULL;
	//判断机器人
	virtual bool __cdecl IsAIUser()=NULL;

	//统计接口
public:
	//游戏时间
	virtual DWORD __cdecl GetPlayTimeCount()=NULL;
	//在线时间
	virtual DWORD __cdecl GetOnlineTimeCount()=NULL;
	//信用额度信息
	virtual bool __cdecl GetUserScoreInfo(tagUserScore & ScoreResultInfo)=NULL;

	//引用接口
public:
	//增加引用
	virtual void __cdecl AddUserRefCount()=NULL;
	//减少引用
	virtual void __cdecl ReleaseUserRefCount()=NULL;
	//引用为空
	virtual bool __cdecl IsUserRefCountZero()=NULL;
	//功能接口
public:
	//设置规则
	virtual bool __cdecl SetUserRule(tagUserRule & UserRule)=NULL;
	//修改信用额度
    virtual bool __cdecl WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount)=NULL;
	//设置状态
	virtual bool __cdecl SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)=NULL;

	//计算辅助接口
public:
	virtual DWORD __cdecl GetCalculateItemPos(WORD wTableID)=NULL;
	virtual void  __cdecl SetCalculateItemPos(WORD wTableID, DWORD dwItemPos)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IServerUserManager INTERFACE_VERSION(2,1)
static const GUID IID_IServerUserManager={0xa27d1d5d,0x5999,0x488e,0xaa,0x4a,0x30,0x8a,0x1,0x11,0xf4,0x9e};

//服务器用户管理接口
interface IServerUserManager : public IUnknownEx
{
	//管理接口
public:
	//重置用户
	virtual bool __cdecl ResetUserManager()=NULL;
	//切换用户
	virtual bool __cdecl SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)=NULL;
	//激活用户
	virtual bool __cdecl ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)=NULL;
	//激活用户
	virtual IServerUserItem * __cdecl ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN])=NULL;
	//激活用户
	virtual IServerUserItem * __cdecl ActiveLinkUserItem(tagServerUserData & ServerUserData)=NULL;
	//删除用户
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//注册断线
	virtual bool __cdecl RegOffLineUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//删除断线
	virtual bool __cdecl UnRegOffLineUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//检测用户计数
	virtual bool __cdecl CheckGameUserRefCount(IServerUserItem * pIServerUserItem)=NULL;

	//查找接口
public:
	//枚举用户
	virtual IServerUserItem * __cdecl EnumOnLineUser(WORD wEnumIndex)=NULL;
	//枚举用户
	virtual IServerUserItem * __cdecl EnumOffLineUser(WORD wEnumIndex)=NULL;
	//查找用户
	virtual IServerUserItem * __cdecl SearchOnLineUser(DWORD dwUserID)=NULL;
	//查找用户
	virtual IServerUserItem * __cdecl SearchOffLineUser(DWORD dwUserID)=NULL;
	//查找用户
	virtual IServerUserItem * __cdecl SearchUser(DWORD dwUserID)=NULL;

	//信息接口
public:
	//在线人数
	virtual DWORD __cdecl GetOnLineCount()=NULL;
	//断线人数
	virtual DWORD __cdecl GetOffLineCount()=NULL;
};
//////////////////////////////////////////////////////////////////////////
#define VER_ICalculateItem INTERFACE_VERSION(1,1)
// {F1803198-BEA8-48a4-B410-2A7F8624CEF8}
static const GUID IID_ICalculateItem = { 0xf1803198, 0xbea8, 0x48a4, { 0xb4, 0x10, 0x2a, 0x7f, 0x86, 0x24, 0xce, 0xf8 } };

//计算框架单元接口
interface ICalculateItem : public IUnknownEx
{
	//用户 I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//父用户 I D
	virtual DWORD __cdecl GetParentUserID()=NULL;
	//用户等级
	virtual BYTE __cdecl GetUserLevel()=NULL;
	//获取信息
	virtual tagServerUserData * __cdecl GetUserData()=NULL;
	//获取用户椅子ID
	virtual WORD __cdecl GetChairID()=NULL;
	//用户体系正确
	virtual bool  __cdecl IsUserTreeFrameOK()=NULL;

	//获取总投注
	virtual bool __cdecl GetTotalBetScore(DECIMAL* pTotalBetScore)=NULL;
	//获取总派彩
	virtual bool __cdecl GetTotalWinScore(DECIMAL* pTotalWinScore)=NULL;
	//获取总抽水
	virtual bool __cdecl GetTotalTaxScore(DECIMAL* pTotalTaxScore)=NULL;

	//获取派彩占成
	virtual bool __cdecl GetWinScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore)=NULL;
	//获取抽水占成
	virtual bool __cdecl GetTaxScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore)=NULL;
	//获取洗码与交收额度
	virtual bool __cdecl GetValidBetScoreAndPaidScore(DECIMAL* pTotalScore,
		DECIMAL* pHighScore, DECIMAL *pHighRollbackScore,
		DECIMAL* pLessScore, DECIMAL *pLessRollbackScore,
		DECIMAL* pHighPaidScore, DECIMAL* pLessPaidScore)=NULL;

	//获取投注详细信息
	virtual bool __cdecl GetDetailBetScore(BYTE *pDetailBetScore,
		WORD& wDetailBetScoreSize)=NULL;
};
//////////////////////////////////////////////////////////////////////////
//计算框架回调接口
#define VER_ICalculateSink INTERFACE_VERSION(1,1)
// {F8B5E4B3-A39A-4f59-8501-18D90A7B0A78}
static const GUID IID_ICalculateSink = { 0xf8b5e4b3, 0xa39a, 0x4f59, { 0x85, 0x1, 0x18, 0xd9, 0xa, 0x7b, 0xa, 0x78 } };

//计算框架回调接口
interface ICalculateSink : public IUnknownEx
{
	//计算派彩
	virtual bool __cdecl CalculateResult(void* pCalculateContext,
		WORD wChairID,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		DECIMAL* pPartedWinScore,
		DECIMAL* pPartedTaxScore,
		WORD	 wBetRegionCount)=NULL;

	//调整洗码
	virtual bool __cdecl RectifyValidBetScore(void* pCalculateContext,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		WORD	 wBetRegionCount,
		DECIMAL* pValidBetScore)=NULL;
};
//////////////////////////////////////////////////////////////////////////
//投注信息
struct tagBetScoreInfo
{
	IServerUserItem *	pIServerUserItem;	
	WORD				wBetRegionIndex;
	DWORD				dwBetScore;
};

//计算框架接口
#define VER_ICalculateFrame INTERFACE_VERSION(1,1)
// {85FBFF21-B4FB-4b17-A5B2-3A7E262E0EC7}
static const GUID IID_ICalculateFrame = { 0x85fbff21, 0xb4fb, 0x4b17, { 0xa5, 0xb2, 0x3a, 0x7e, 0x26, 0x2e, 0xe, 0xc7 } };

//计算框架接口
interface ICalculateFrame : public IUnknownEx
{
	//枚举计算单元
	virtual ICalculateItem* __cdecl EnumCalculateItem(WORD wEnumIndex)=NULL;
	//获取计算单元数目
	virtual DWORD	__cdecl GetCalculateItemCount()=NULL;

	//获取总投注
	virtual bool __cdecl GetFrameTotalBetScore(DECIMAL* pTotalBetScore)=NULL;
	//获取总派彩
	virtual bool __cdecl GetFrameTotalWinScore(DECIMAL* pTotalWinScore)=NULL;

	//用户坐下
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//用户起来
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;

	//复位计算
	virtual bool __cdecl Reset()=NULL;
	//构建计算信息
	virtual bool __cdecl StructCalculateItem()=NULL;

	//投注
	virtual bool __cdecl BetScore(IServerUserItem * pIServerUserItem,
		tagBetScoreInfo *pBetScoreInfo, WORD wCount)=NULL;
	//检查投注
	virtual BYTE __cdecl CheckBetScore(IServerUserItem * pIServerUserItem,
		tagBetScoreInfo *pBetScoreInfo, WORD wCount)=NULL;


	//计算代理对会员方式
	virtual bool __cdecl CalculateCTP(void* pCalculateContext,
		ICalculateSink* pCalculateSink)=NULL;

	//计算会员对会员方式
	virtual bool __cdecl CalculatePTP(tagBetScoreInfo *pBetScoreInfo, WORD wCount)=NULL;

};
//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrame INTERFACE_VERSION(5,1)
static const GUID IID_ITableFrame={0x1aee9bf4,0x8629,0x45dc,0xb0,0xfa,0x2c,0xa7,0x4f,0xbe,0x18,0xc1};

//游戏桌子接口
interface ITableFrame : public IUnknownEx
{
	//信息接口
public:
	//桌子号码
	virtual WORD __cdecl GetTableID()=NULL;
	//游戏人数
	virtual WORD __cdecl GetChairCount()=NULL;
	//空位置数目
	virtual WORD __cdecl GetNullChairCount()=NULL;
	//空位置号码
	virtual WORD __cdecl GetNullChairID()=NULL;
	//普通玩家人数
	virtual WORD __cdecl GetGameUserCount()=NULL;
	//相同IP地址位置ID
	virtual WORD __cdecl GetSameIPChairID(IServerUserItem * pIServerUserItem)=NULL;
	//游戏时间
	virtual ULONGLONG __cdecl GetPlayTimeCount()=NULL;

	//AI信息接口
public:
	//AI玩家人数
	virtual WORD __cdecl GetAIGameUserCount()=NULL;
	//获取AI对杀的普通玩家座号码
	virtual WORD __cdecl GetAIKillGameUserChairID()=NULL;
	//是否AI对杀普通玩家
	virtual bool __cdecl IsAIKillGameUser()=NULL; 

	//用户接口
public:
	//枚举用户
	virtual IServerUserItem * __cdecl EnumLookonUserItem(WORD wIndex)=NULL;
	//获取用户
	virtual IServerUserItem * __cdecl GetServerUserItem(WORD wChairID)=NULL;

	//状态接口
public:
	//获取状态
	virtual BYTE __cdecl GetGameStatus()=NULL;
	//设置状态
	virtual void __cdecl SetGameStatus(BYTE bGameStatus)=NULL;
	//获取属性
	virtual const tagGameServiceAttrib * __cdecl GetGameServiceAttrib()=NULL; 
	//获取配置
	virtual const tagGameServiceOption * __cdecl GetGameServiceOption()=NULL;

	//定时器接口
public:
	//设置定时器
	virtual bool __cdecl SetGameTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)=NULL;
	//删除定时器
	virtual bool __cdecl KillGameTimer(WORD wTimerID)=NULL;

	//网络接口
public:
	//发送数据
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//发送数据
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;

	//消息接口
public:
	//发送房间消息
	virtual bool __cdecl SendRoomMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//发送游戏消息
	virtual bool __cdecl SendGameMessage(IServerUserItem * pIServerUserItem, WORD wErrCode, LPCTSTR lpszMessage, WORD wMessageType)=NULL;

	//游戏接口
public:
	//解散游戏
	virtual bool __cdecl DismissGame()=NULL;
	//结束游戏
	virtual bool __cdecl ConcludeGame()=NULL;
	//结算
	virtual bool __cdecl WriteGameEnd(BYTE cbReason,
		BYTE *pEndData,
		WORD wEndDataSize,
		void* pCalculateContext,
		ICalculateSink* pCalculateSink)=NULL;
	//发送场景
	virtual bool __cdecl SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize)=NULL;
	//计算接口
public:
	//获取计算框架
	virtual ICalculateFrame* __cdecl GetICalculateFrame()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameManager INTERFACE_VERSION(2,1)
static const GUID IID_ITableFrameManager={0xc0994158,0xf4b4,0x462f,0xa8,0x84,0x85,0x13,0xe2,0xac,0xfe,0x31};

//框架管理接口
interface ITableFrameManager : public ITableFrame
{
	//配置接口
public:
	//初始化
	virtual bool __cdecl InitTableFrame(WORD wTableID, 
		tagTableFrameParameter * pTableFrameParameter)=NULL;

	//信息接口
public:
	//锁定状态
	virtual bool __cdecl IsTableLocked()=NULL;
	//游戏状态
	virtual bool __cdecl IsGameStarted()=NULL;
	//游戏状态
	virtual bool __cdecl IsUserPlaying(IServerUserItem * pIServerUserItem)=NULL;

	//执行接口
public:
	//坐下动作
	virtual bool __cdecl PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//旁观动作
	virtual bool __cdecl PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//离开动作
	virtual bool __cdecl PerformStandUpAction(IServerUserItem * pIServerUserItem)=NULL;
	//人工智能游戏动作
	virtual bool __cdecl PerformAIGameAction()=NULL;

	//用户事件
public:
	//断线事件
	virtual bool __cdecl OnUserOffLine(WORD wChairID)=NULL;
	//重进事件
	virtual bool __cdecl OnUserReConnect(WORD wChairID)=NULL;
	//请求断线
	virtual bool __cdecl OnUserReqOffLine(WORD wChairID)=NULL;

	//系统事件
public:
	//定时器事件
	virtual bool __cdecl OnEventTimer(WORD wTimerID, WPARAM wBindParam)=NULL;
	//游戏事件
	virtual bool __cdecl OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//框架事件
	virtual bool __cdecl OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameSink INTERFACE_VERSION(3,1)
static const GUID IID_ITableFrameSink={0x53d985c5,0x623a,0x41df,0x89,0x76,0x27,0xb1,0x8d,0xc1,0x97,0x37};

//游戏桌子回调接口
interface ITableFrameSink : public IUnknownEx
{
	//管理接口
public:
	//初始化
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx)=NULL;
	//复位桌子
	virtual void __cdecl RepositTableFrameSink()=NULL;

	//信息接口
public:
	//开始模式
	virtual enStartMode __cdecl GetGameStartMode()=NULL;
	//游戏状态
	virtual bool __cdecl IsUserPlaying(WORD wChairID)=NULL;

	//游戏事件
public:
	//游戏开始
	virtual bool __cdecl OnEventGameStart()=NULL;
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)=NULL;
	//人工智能游戏动作
	virtual bool __cdecl OnPerformAIGameAction()=NULL;

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam)=NULL;
	//游戏消息
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//框架消息
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameEvent INTERFACE_VERSION(1,1)
static const GUID IID_ITableFrameEvent={0xb16717de,0xc243,0x4686,0xbb,0x8a,0xd0,0x37,0xb8,0xc2,0xc,0xf6};

//框架事件接口
interface ITableFrameEvent : public IUnknownEx
{
	//请求事件
public:
	//请求同意
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//请求断线
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem, WORD wOffLineCount)=NULL;
	//请求重入
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//请求坐下
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon)=NULL;
	//请求起来
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon)=NULL;

	//动作事件
public:
	//用户同意
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//用户断线
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//用户重入
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//用户坐下
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//用户起来
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IGameServiceFrame INTERFACE_VERSION(1,1)
static const GUID IID_IGameServiceFrame={0xafccfa53,0x6431,0x4b2a,0x98,0x29,0xf5,0xcf,0x15,0xdf,0xd5,0x3b};

//GameService框架
interface IGameServiceFrame : public IUnknownEx
{
	//状态接口
public:
	//关闭查询
	virtual bool __cdecl IsShallClose()=NULL;
	//私聊查询
	virtual bool __cdecl IsAllowWisper()=NULL;
	//聊天查询
	virtual bool __cdecl IsAllowRoomChat()=NULL;
	//聊天查询
	virtual bool __cdecl IsAllowGameChat()=NULL;
	//进入查询
	virtual bool __cdecl IsAllowEnterRoom()=NULL;
	//进入查询
	virtual bool __cdecl IsAllowEnterGame()=NULL;

	//状态通知
public:
	//发送状态
	virtual bool __cdecl SendTableStatus(WORD wTableID)=NULL;
	//发送额度
	virtual bool __cdecl SendUserScore(IServerUserItem * pIServerUserItem)=NULL;
	//发送状态
	virtual bool __cdecl SendUserStatus(IServerUserItem * pIServerUserItem)=NULL;

	//网络接口
public:
	//发送数据
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//发送数据
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;

	//定时器接口
public:
	//设置定时器
	virtual bool __cdecl SetTableTimer(WORD wTableID, WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)=NULL; 
	//删除定时器
	virtual bool __cdecl KillTableTimer(WORD wTableID, WORD wTimerID)=NULL;

	//管理接口
public:
	//删除用户
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//检测用户计数
	virtual bool __cdecl CheckGameUserRefCount(IServerUserItem * pIServerUserItem)=NULL;
	//输出信息
	virtual void __cdecl ExportInformation(LPCTSTR pszString, enTraceLevel TraceLevel)=NULL;
};
//////////////////////////////////////////////////////////////////////////

#define VER_IGameAIManager INTERFACE_VERSION(2,1)
static const GUID IID_IGameAIManager={0x1a6b9efa,0x3c56,0x4ee1,0x96,0xce,0x90,0xe1,0x20,0x48,0xe5,0xe7};

//游戏人工智能管理接口
interface IGameAIManager : public IUnknownEx
{
	//机器人随即操作
	virtual bool __cdecl ExecAI_User_RandOperation()=NULL;
	//游戏桌随即操作
	virtual bool __cdecl ExecAI_Table_RandOperation()=NULL;

};
//////////////////////////////////////////////////////////////////////////

#define VER_IGameServiceManager INTERFACE_VERSION(2,1)
static const GUID IID_IGameServiceManager={0xda6b9efa,0x3c56,0x4ee1,0x96,0xce,0x90,0xe1,0x20,0x48,0xe5,0xe7};

//游戏服务器管理接口
interface IGameServiceManager : public IUnknownEx
{
	//创建桌子
	virtual void * __cdecl CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)=NULL;
	//获取属性
	virtual void __cdecl GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)=NULL;
	//修改参数
	virtual bool __cdecl RectifyServiceOption(tagGameServiceOption * pGameServiceOption)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IGameService INTERFACE_VERSION(2,1)
static const GUID IID_IGameService={0x1239db23,0x4d2e,0x4f35,0xbb,0x4e,0x8c,0x4c,0x5d,0x39,0xe1,0xa0};

//GameService接口
interface IGameService : public IUnknownEx
{
	//服务状态
	virtual bool __cdecl IsService()=NULL;
	//设置事件
	virtual bool __cdecl SetEventService(IUnknownEx * pIEventService)=NULL;
	//开始服务
	virtual bool __cdecl StartService(tagGameServiceParameter * pGameServiceParameter)=NULL;
	//停止服务
	virtual bool __cdecl StopService()=NULL;
};

//////////////////////////////////////////////////////////////////////////

//GameService组件辅助类
class CGameServiceHelper : public CTempldateHelper<IGameService>
{
	//函数定义
public:
	//构造函数
	CGameServiceHelper(void) : CTempldateHelper<IGameService>(IID_IGameService,
		VER_IGameService,GAME_SERVICE_DLL_NAME,TEXT("CreateGameService")) { }
};

//////////////////////////////////////////////////////////////////////////

#endif