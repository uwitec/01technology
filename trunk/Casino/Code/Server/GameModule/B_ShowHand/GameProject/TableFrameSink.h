#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//梭哈扑克类型
#define SHAN								0
#define DUI_ZI								1
#define LAING_DUI							2
#define SAN_TIAO							3
#define SHUN_ZI								4
#define	TONG_HUA							5
#define HU_LU								6
#define TIE_ZHI								7
#define TONG_HUA_SHUN						8

//扑克花色类型
#define FANG_KUAI							0
#define MEI_HUA								1
#define HONG_TAO							2
#define HEI_TAO								3

//结束原因
#define GER_NO_PLAYER						0x10						//没有玩家

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_bCardArray[28];					//扑克定义

	//基础函数
private:
	//获取扑克花色
	BYTE GetHuaKind(BYTE bCard);
	//获取扑克数值
	BYTE GetCardValoe(BYTE bCard);
	//获取扑克类型
	BYTE GetCardKind(BYTE bCardList[], BYTE bCardCount);
	//查找对牌
	BYTE FindDoubleCard(BYTE bCardList[], BYTE bCardCount, BYTE * bOutDoubleCard);

	//类型函数
public:
	//是否同花顺
	bool IsTongHuaShun(BYTE bCardList[], BYTE bCardCount);
	//是否铁支
	bool IsTieZhi(BYTE bCardList[], BYTE bCardCount);
	//是否葫芦
	bool IsHuLu(BYTE bCardList[], BYTE bCardCount);
	//是否同花
	bool IsTongHua(BYTE bCardList[], BYTE bCardCount);
	//是否顺子
	bool IsShunZhi(BYTE bCardList[], BYTE bCardCount);
	//是否三条
	bool IsSanTiao(BYTE bCardList[], BYTE bCardCount);
	//是否两对
	bool IsLiangDui(BYTE bCardList[], BYTE bCardCount);
	//是否对子
	bool IsDuiZhi(BYTE bCardList[], BYTE bCardCount);

	//对比函数
public:
	//对比单只扑克
	bool CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard);
	//对比同花顺
	bool CompareTongHuaShun(BYTE bFirstList[], BYTE bLastList[]);
	//对比铁支
	bool CompareTeiZhi(BYTE bFirstList[], BYTE bLastList[]);
	//对比葫芦
	bool CompareHuLu(BYTE bFirstList[], BYTE bLastList[]);
	//对比同花
	bool CompareTongHua(BYTE bFirstList[], BYTE bLastList[]);
	//对比顺子
	bool CompareShunZhi(BYTE bFirstList[], BYTE bLastList[]);
	//对比三条
	bool CompareSanTiao(BYTE bFirstList[], BYTE bLastList[]);
	//对比两对
	bool CompareLiangDui(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);
	//对比对子
	bool CompareDuiZhi(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);
	//对比散牌
	bool CompareSanCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);

	//功能函数
public:
	//混乱扑克
	void RandCard(BYTE bCardBuffer[], BYTE bBufferCount);
	//排列扑克
	void SortCard(BYTE bCardList[], BYTE bCardCount);
	//对比扑克
	bool CompareCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);
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
class CTableFrameSink : public ITableFrameSink
{
	//游戏变量
protected:
	WORD							m_wOperaCount;							//操作次数
	WORD							m_wCurrentUser;							//当前用户
	BYTE							m_bSendCardCount;						//发牌数目
	BYTE							m_bTableCardCount[GAME_PLAYER];			//扑克数目
	BYTE							m_bTableCardArray[GAME_PLAYER][5];		//桌面扑克

	//金币信息
protected:
	DOUBLE							m_fMaxGold;								//最大下注
	DOUBLE							m_fTurnBasicGold;						//最少下注
	DOUBLE							m_fTableGold[2*GAME_PLAYER];			//下注金币

	//辅助变量
protected:
	BYTE							m_bPlayStatus[GAME_PLAYER];				//游戏状态

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

	//游戏事件
protected:
	//放弃事件
	bool OnUserGiveUp(WORD wChairID);
	//加注事件
	bool OnUserAddGold(WORD wChairID, DOUBLE fGold, bool bCancel);

	//游戏函数
protected:
	//推断胜者
	WORD DeduceWiner(BYTE bBeginPos, BYTE bEndPos);
	//推断输者
	WORD DeduceLoser(BYTE bBeginPos, BYTE bEndPos);
};

//////////////////////////////////////////////////////////////////////////

#endif