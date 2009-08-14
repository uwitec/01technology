#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对子类型
#define CT_THREE_TIAO				3									//三条类型
#define	CT_SHUN_ZI					4									//顺子类型
#define CT_TONG_HUA					5									//同花类型
#define CT_HU_LU					6									//葫芦类型
#define CT_TIE_ZHI					7									//铁支类型
#define CT_TONG_HUA_SHUN			8									//同花顺型

//宏定义
#define MAX_COUNT					13									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
class tagAnalyseResult
{
public:
	BYTE bFourCount;//四张数目
	BYTE bThreeCount;//三张数目
	BYTE bDoubleCount;//两张数目
	BYTE bSignedCount;//单张数目
	BYTE bFourLogicVolue[3];//四张列表
	BYTE bThreeLogicVolue[4];//三张列表
	BYTE bDoubleLogicVolue[6];//两张列表
	BYTE m_bSCardData[13];//手上扑克
	BYTE m_bDCardData[13];//手上扑克
	BYTE m_bTCardData[13];//手上扑克
	BYTE m_bFCardData[13];//手上扑克

	tagAnalyseResult()
	{
	}

	void init()
	{
		bFourCount=0;//四张数目
		bThreeCount=0;//三张数目
		bDoubleCount=0;//两张数目
		bSignedCount=0;//单张数目
		memset(bFourLogicVolue, 0, sizeof(bFourLogicVolue));
		memset(bThreeLogicVolue, 0, sizeof(bThreeLogicVolue));
		memset(bDoubleLogicVolue, 0, sizeof(bDoubleLogicVolue));
		memset(m_bSCardData, 0, sizeof(m_bSCardData));
		memset(m_bDCardData, 0, sizeof(m_bDCardData));
		memset(m_bTCardData, 0, sizeof(m_bTCardData));
		memset(m_bFCardData, 0, sizeof(m_bFCardData));
		
	}
};
//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[52];				//扑克数据

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//获取数值
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//混乱扑克
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//逻辑函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);


	//类型函数
public:
	//是否对子
	bool IsDouble(BYTE bCardData[], BYTE bCardCount);
	//是否铁支
	bool IsTieZhi(BYTE bCardData[], BYTE bCardCount);
	//是否葫芦
	bool IsHuLu(BYTE bCardData[], BYTE bCardCount);
	//是否同花
	bool IsTongHua(BYTE bCardData[], BYTE bCardCount);
	//是否顺子
	bool IsShunZhi(BYTE bCardData[], BYTE bCardCount);
	//是否三条
	bool IsSanTiao(BYTE bCardData[], BYTE bCardCount);
	//是否同花顺
	bool IsTongHuaShun(BYTE bCardData[], BYTE bCardCount);

	//分析函数
public:
	//分析顺子类型
	int AnalyseCardData_ShunZhi(const BYTE CardData[],
		BYTE CardCount,BYTE AnalyseResult[][5]);
	//分析同花类型
	int AnalyseCardData_TongHua(const BYTE CardData[],
		BYTE CardCount,BYTE AnalyseResult[][5]);
	//分析同花类型
	int GetTongHuaCardData(BYTE HuaSe,
		const BYTE CardData[],
		BYTE CardCount,BYTE AnalyseResult[]);
	//分析葫芦类型
	int AnalyseCardData_HuLu(const BYTE TCardData[],
		int TCardCount,
		const BYTE DCardData[],
		int DCardCount,
		BYTE AnalyseResult[][5]);
	//分析铁支类型
	int AnalyseCardData_TieZhi(const BYTE FCardData[],
		int FCardCount,
		const BYTE SCardData[],
		int SCardCount,
		BYTE AnalyseResult[][5]);
	//同花顺型
	int AnalyseCardData_TongHuaShun(const BYTE CardData[],
		int CardCount,
		BYTE AnalyseResult[][5]);
	//分析扑克
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);

	//功能函数
public:
	//对比扑克
	bool CompareCard(BYTE bFirstCardData, BYTE bNextCardData);
	//对比扑克
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//出牌判断
	BYTE SearchOutCard(BYTE bHandCardData[], BYTE bHandCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT]);
};

//////////////////////////////////////////////////////////////////////////

#endif