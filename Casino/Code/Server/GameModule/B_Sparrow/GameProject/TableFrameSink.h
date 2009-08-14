#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//枚举定义

//效验类型
enum enEstimatKind
{
	EstimatKind_OutCard,			//出牌效验
	EstimatKind_GangCard,			//杠牌效验
};
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
class CTableFrameSink : public ITableFrameSink, public ITableFrameEvent
{
	//游戏变量
protected:
	WORD							m_wSiceCount;							//骰子点数
	WORD							m_wBankerUser;							//庄家用户
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//用户扑克

	//出牌信息
protected:
	WORD							m_wOutCardUser;							//出牌用户
	BYTE							m_cbOutCardData;						//出牌扑克
	BYTE							m_cbOutCardCount;						//出牌数目
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//丢弃数目
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//丢弃记录

	//发牌信息
protected:
	BYTE							m_cbSendCardData;						//发牌扑克
	BYTE							m_cbSendCardCount;						//发牌数目
	BYTE							m_cbLeftCardCount;						//剩余数目
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//库存扑克

	//Run变量
protected:
	WORD							m_wResumeUser;							//还原用户
	WORD							m_wCurrentUser;							//当前用户
	WORD							m_wProvideUser;							//供应用户
	BYTE							m_cbProvideCard;						//供应扑克

	//状态变量
protected:
	bool							m_bSendStatus;							//发牌状态
	bool							m_bGangStatus;							//抢杆状态
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//禁止吃胡
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//禁止吃碰

	//用户状态
public:
	bool							m_bResponse[GAME_PLAYER];				//响应标志
	BYTE							m_cbUserAction[GAME_PLAYER];			//用户动作
	BYTE							m_cbOperateCard[GAME_PLAYER];			//操作扑克
	BYTE							m_cbPerformAction[GAME_PLAYER];			//执行动作

	//组合扑克
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];		//组合扑克

	//结束信息
protected:
	BYTE							m_cbChiHuCard;							//吃胡扑克
	tagChiHuResult					m_ChiHuResult[GAME_PLAYER];				//吃胡结果

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数
	ICalculateFrame					* m_pICalculateFrame;				//计算接口
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//属性参数
	CCalculateSink					m_CalculateSink;					//计算回调

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	static const enStartMode		m_GameStartMode;						//开始模式

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
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
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

	//请求事件
public:
	//请求同意
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//请求断线
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem, WORD wOffLineCount) { return wOffLineCount<3; }
	//请求重入
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//请求坐下
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//请求起来
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

	//动作事件
public:
	//用户同意
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户断线
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//游戏事件
protected:
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//用户操作
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);

	//人工智能函数
protected:
	//用户出牌
	bool AI_AutomatismOutCard(WORD wChairID, IServerUserItem *pIServerUserItem);
	//用户操作
	bool AI_AutomatismOperate(WORD wChairID, IServerUserItem *pIServerUserItem,
							BYTE cbActionCard,BYTE cbActionMask,tagGangCardResult& GangCardResult);
	//寻找最匹配牌的机器人
	WORD AI_BestMachChairID(WORD wOutCardChairID, WORD wGameUserChairID, BYTE bCard);
	

	//辅助函数
protected:
	//发送操作
	bool SendOperateNotify();
	//派发扑克
	bool DispatchCardData(WORD wCurrentUser);
	//响应判断
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
public:
	//判断荒局
	bool EstimateNullChiHuCard();
	//获取放炮玩家
	WORD GetProvideUser();
	//获取胡牌玩家
	WORD GetHuPaiUser();
	//抓炮胡牌
	bool IsZhuaPaoChiHu();
	//判断有效投注用户
	bool IsValidBetScoreUser(DWORD dwUserID);
};

//////////////////////////////////////////////////////////////////////////

#endif