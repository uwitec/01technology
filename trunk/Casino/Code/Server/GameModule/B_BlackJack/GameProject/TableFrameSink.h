#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\CommandDefinition\CMD_BlackJack.h"
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
	WORD							m_wPlayerCount;							//玩家人数	
	WORD							m_wCurrentUser;							//当前用户
	
	//分牌相关
protected:	
	BYTE							m_cbTableCutCardCount[GAME_PLAYER];		//分牌扑克
	BYTE							m_cbTableCutCardArray[GAME_PLAYER][13];	//分牌扑克  
	bool                            m_cbCutCard[GAME_PLAYER];               //是否分牌

	//初始扑克
protected:
	BYTE							m_cbTableCardCount[GAME_PLAYER];		//扑克数目
	BYTE							m_cbTableCardArray[GAME_PLAYER][13];	//桌面扑克  
    
	//发牌变量
protected:
	BYTE			            	m_cbRandCardArray[52];					//散乱扑克
	BYTE			            	m_cbSendCardCount;				        //已发扑克

	//游戏庄家
protected:	
	int 						    m_nBankerStation;						//庄家位置
	BOOL                            m_bBankerWin;                           //庄家获胜
	BOOL							m_bBankerGetCard;                       //庄家要牌  
	DOUBLE							m_fBankerScore;							//庄家额度

	//游戏闲家
protected:
	int								m_nNotBanker;
	DOUBLE							m_fNotBankerScore[GAME_PLAYER];			//闲家额度

	//标识加注
protected:
	BOOL							m_bAddGold[GAME_PLAYER];				//是否加注	
	bool                            m_bAddGoldEnd;                          //加注结束	

	//金币信息
protected:
	DOUBLE							m_fMaxGold;								//最大下注
	DOUBLE							m_fTurnBasicGold;						//最少下注
	DOUBLE							m_fTableGold[2*GAME_PLAYER];			//下注金币
	DOUBLE                          m_fUserAddGold[GAME_PLAYER];            //用户下注
	
	//玩家状态
protected:
	BYTE							m_cbCardStatus[GAME_PLAYER];		   //初始发牌
	BYTE							m_cbCutCardStatus[GAME_PLAYER];		   //分牌用户

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* pGameServiceOption;					//配置参数
	ICalculateFrame					* m_pICalculateFrame;				//计算接口
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//属性参数
	CCalculateSink					m_CalculateSink;					//计算回调

	//属性变量
protected:
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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);
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

	//游戏函数
protected:
	//获取扑克
	BYTE GetRandCard();
	//用户强退
	bool OnUserLeft(bool bAddGoldEnd);
    //用户强退
	bool OnUserLeft(WORD wChairID, bool bAddGoldEnd);
	//计算成绩
	bool CalcScore(BYTE cbCardStatus, WORD wChairID, DOUBLE & fGameGold, DOUBLE & fBankerScore, bool bCutCard);
	//最后投注
	DOUBLE GetLastGold(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif