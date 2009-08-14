#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../CommandDefinition/CMD_UNCOVERPIG.H"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//结算回调类
class CTableFrameSink;
class CCalculateSink:public ICalculateSink
{
public:
	CCalculateSink();
	~CCalculateSink();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCalculateSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ICalculateSink接口
public:
	//计算派彩
	virtual bool __cdecl CalculateResult(void* pCalculateContext,
		WORD wChairID,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		DECIMAL* pPartedWinScore,
		DECIMAL* pPartedTaxScore,
		WORD	 wBetRegionCount);
	//调整洗码
	virtual bool __cdecl RectifyValidBetScore(void* pCalculateContext,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		WORD	 wBetRegionCount,
		DECIMAL* pValidBetScore);
	//
public:
	//初始化
	bool __cdecl InitCalculateSink(ITableFrame	*pITableFrame,
		CTableFrameSink *pTableFrameSink);
	//初始化计算上下文
	bool __cdecl InitCalculateContext(CMD_S_GameEnd* pGameEnd);
	//获取计算上下文
	void* __cdecl GetCalculateContext();
protected:
	ITableFrame						* m_pITableFrame;					//框架接口
	CTableFrameSink					* m_pTableFrameSink;				//游戏桌子类
	ICalculateFrame					* m_pICalculateFrame;				//计算接口
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//属性参数
	const tagGameServiceOption		* m_pGameServiceOption;				//配置参数
	CMD_S_GameEnd					* m_pGameEnd;						//计算上下文
	DECIMAL							  m_decAfterTax;					//抽水基数
};
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink
{
	//游戏变量
protected:
	WORD							m_wCurrentUser;						//当前玩家
	WORD	                        m_wShowCardUser[GAME_PLAYER];       //亮牌玩家
	BYTE                            m_bShowCard[GAME_PLAYER][4];        //玩家亮牌
	BYTE                            m_bShowCardUserCount;               //玩家数目
	BYTE                            m_bShowCardTime;                    //亮牌次数
	BYTE                            m_bTotalShowCardCount;              //亮牌数目
	BYTE							m_bScoreCard[GAME_PLAYER][16];      //分数扑克
	BYTE                            m_bScoreCardCount[GAME_PLAYER];     //扑克数目
	BYTE							m_bShowCardCount[GAME_PLAYER];		//亮牌数目
	BYTE	                        m_bShowCardFlag[GAME_PLAYER];       //亮牌玩家

	//Run信息
protected:
	WORD							m_wTurnWiner;						//胜利玩家
	WORD                            m_wFirstOutUser;                    //首先出牌
    WORD                            m_wGetGpadeUser;                    //得黑桃Q
	BYTE							m_bTurnCardCount;					//桌面牌数
	BYTE							m_bTurnCardData[GAME_PLAYER];		//桌面出牌

	//扑克信息
protected:
	BYTE							m_bCardCount[GAME_PLAYER];			//扑克数目
	BYTE							m_bHandCardData[GAME_PLAYER][HAND_CARD_COUNT];	//手上扑克

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	ITableFrame						* m_pITableFrame;					//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;				//配置参数
	ICalculateFrame					* m_pICalculateFrame;				//计算接口
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//属性参数
	CCalculateSink					m_CalculateSink;					//计算回调

	//属性变量
protected:
	static const WORD				m_wPlayerCount;						//游戏人数
	static const enStartMode		m_GameStartMode;					//开始模式

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void __cdecl RepositTableFrameSink();

	//信息接口
public:
	//开始模式
	virtual enStartMode __cdecl GetGameStartMode();
	//游戏状态
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//游戏事件
public:
	//游戏开始
	virtual bool __cdecl OnEventGameStart();
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);
	//人工智能游戏动作
	virtual bool __cdecl OnPerformAIGameAction();

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//游戏事件
protected:
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE bCardData, BYTE bCardCount);
	//玩家亮牌
	bool OnUserShowCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);

	//人工智能函数
protected:
	//判断亮牌
	BYTE AI_CanShowCard(WORD wChairID, IServerUserItem * pIServerUserItem,BYTE bShowCardData[4]);
	//自动出牌
	bool AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem);
	//出牌判断
	bool AI_VerdictOutCard(WORD wChairID, IServerUserItem * pIServerUserItem,
		bool bAutoOutCard,
		BYTE bCardData[HAND_CARD_COUNT],BYTE bShootCount,
		BYTE bHandCardData[HAND_CARD_COUNT],BYTE bHandCardCount);
	//第一轮花色
	bool AI_VerdictFirstGameColor(BYTE bFirstOutColor , 
		BYTE bCardData , 
		bool bAutoOutCard,
		BYTE bHandCardData[HAND_CARD_COUNT],BYTE bHandCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif