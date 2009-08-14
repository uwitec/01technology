#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"
#include "AttemperEngineSink.h"
#include "CalculateFrame.h"

//////////////////////////////////////////////////////////////////////////

//用户接口数组
typedef CArrayTemplate<IServerUserItem *> CUserItemPtrArray;

//游戏桌子框架
class CTableFrame : public ITableFrameManager
{
	//属性变量
protected:
	WORD							m_wTableID;							//桌子号码
	WORD							m_wChairCount;						//椅子数目

	//状态变量
protected:
	bool							m_bLocked;							//锁定标志
	bool							m_bStarted;							//开始标志
	BYTE							m_bGameStatus;						//游戏状态
	ULONGLONG						m_ullTimeStart;						//开始时间
	DWORD							m_dwTableOwnerID;					//台主 I D
	TCHAR							m_szPassword[PASS_LEN];				//桌子密码
	WORD							m_wUserCount;						//用户数目
	WORD							m_wAIGameUserCount;					//AI用户数目

	//额度信息
protected:
	bool							m_bWriteScore;						//是否写分
	DWORD							* m_dwPlayerID;						//玩家 I D

	//用户信息
protected:
	bool							* m_bCheckBetScoreFlag;				
	bool							* m_bAllowLookon;					//旁观标志
	WORD							* m_wOffLineCount;					//断线次数
	IServerUserItem					** m_pIUserItem;					//用户指针
	CUserItemPtrArray				m_LookonUserItemPtr;				//旁观用户

	//配置信息
protected:
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//服务属性
	const tagGameServiceOption		* m_pGameServiceOption;				//服务配置

	//对象接口
protected:
	ITableFrameSink					* m_pITableFrameSink;				//桌子接口
	ITableFrameEvent				* m_pITableFrameEvent;				//事件接口
	IGameServiceFrame				* m_pIGameServiceFrame;				//功能接口

	//组件变量
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//数据库通知

	//结算信息
protected:
	CCalculateFrame					m_CalculateFrame;					//结算框架
	ULONGLONG						m_ullGameRoundID;					//游戏局记录ID

	//函数定义
public:
	//构造函数
	CTableFrame();
	//析构函数
	virtual ~CTableFrame();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrame))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//配置接口
public:
	//初始化
	virtual bool __cdecl InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter);

	//信息接口
public:
	//桌子号码
	virtual WORD __cdecl GetTableID() { return m_wTableID; }
	//游戏人数
	virtual WORD __cdecl GetChairCount() { return m_wChairCount; }
	//游戏时间
	virtual ULONGLONG __cdecl GetPlayTimeCount() { return (ULONGLONG)_time64(NULL)-m_ullTimeStart; }
	//空位置数目
	virtual WORD __cdecl GetNullChairCount(){return m_wChairCount - m_wUserCount;}
	//空位置号码
	virtual WORD __cdecl GetNullChairID();
	//相同IP地址位置ID
	virtual WORD __cdecl GetSameIPChairID(IServerUserItem * pIServerUserItem);
	//普通玩家人数
	virtual WORD __cdecl GetGameUserCount(){return m_wUserCount - m_wAIGameUserCount;}

	//AI信息接口
public:
	//AI玩家人数
	virtual WORD __cdecl GetAIGameUserCount(){return m_wAIGameUserCount;}
	//获取AI对杀的普通玩家座号码
	virtual WORD __cdecl GetAIKillGameUserChairID();
	//是否AI对杀普通玩家
	virtual bool __cdecl IsAIKillGameUser(); 

	//用户接口
public:
	//枚举用户
	virtual IServerUserItem * __cdecl EnumLookonUserItem(WORD wIndex);
	//获取用户
	virtual IServerUserItem * __cdecl GetServerUserItem(WORD wChairID);

	//状态接口
public:
	//获取状态
	virtual BYTE __cdecl GetGameStatus() { return m_bGameStatus; }
	//设置状态
	virtual void __cdecl SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }
	//获取属性
	virtual const tagGameServiceAttrib * __cdecl GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//获取配置
	virtual const tagGameServiceOption * __cdecl GetGameServiceOption() { return m_pGameServiceOption; }

	//定时器接口
public:
	//设置定时器
	virtual bool __cdecl SetGameTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam);
	//删除定时器
	virtual bool __cdecl KillGameTimer(WORD wTimerID);

	//网络接口
public:
	//发送数据
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//发送数据
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize);
	//发送数据
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID);
	//发送数据
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//发送数据
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID);
	//发送数据
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//消息接口
public:
	//发送房间消息
	virtual bool __cdecl SendRoomMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType);
	//发送游戏消息
	virtual bool __cdecl SendGameMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType);

	//游戏接口
public:
	//解散游戏
	virtual bool __cdecl DismissGame();
	//结束游戏
	virtual bool __cdecl ConcludeGame();
	//结算
	virtual bool __cdecl WriteGameEnd(BYTE cbReason,
						BYTE *pEndData,
						WORD wEndDataSize,
						void* pCalculateContext,
						ICalculateSink* pCalculateSink);
	//发送场景
	virtual bool __cdecl SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize);
	//计算接口
public:
	//获取计算框架
	virtual ICalculateFrame* __cdecl GetICalculateFrame();

	//执行接口
public:
	//坐下动作
	virtual bool __cdecl PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//旁观动作
	virtual bool __cdecl PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//离开动作
	virtual bool __cdecl PerformStandUpAction(IServerUserItem * pIServerUserItem);
	//人工智能游戏动作
	virtual bool __cdecl PerformAIGameAction();

	//用户事件
public:
	//断线事件
	virtual bool __cdecl OnUserOffLine(WORD wChairID);
	//重进事件
	virtual bool __cdecl OnUserReConnect(WORD wChairID);
	//请求断线
	virtual bool __cdecl OnUserReqOffLine(WORD wChairID);

	//系统事件
public:
	//定时器事件
	virtual bool __cdecl OnEventTimer(WORD wTimerID, WPARAM wBindParam);
	//游戏事件处理
	virtual bool __cdecl OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架事件处理
	virtual bool __cdecl OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//信息接口
public:
	//锁定状态
	virtual bool __cdecl IsTableLocked() { return m_bLocked; }
	//游戏状态
	virtual bool __cdecl IsGameStarted() { return m_bStarted; }
	//游戏状态
	virtual bool __cdecl IsUserPlaying(IServerUserItem * pIServerUserItem);
	
	//其他事件
public:
	//申请游戏局记录-检测信用额度事件
	void OnEventAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScoreResult * pAllocGameRoundAndCheckBetScoreResult);
	//申请游戏局记录事件
	void OnEventAllocGameRoundResult(DBR_GR_AllocGameRoundResult * pAllocGameRoundResult);

	//内部函数
private:
	//尝试开始游戏
	void TryStartGame();
	//开始游戏
	void StartGame();
	//开始判断
	bool StartVerdict();
	//全部机器人判断
	bool IsAllAIGameUser();

	//辅助函数
protected:
	//发送坐下失败
	void SendSitFailedPacket(IServerUserItem * pIServerUserItem, WORD wFailedCode);
};

//////////////////////////////////////////////////////////////////////////

#endif