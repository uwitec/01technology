#pragma once
#include "Stdafx.h"
#include "GameServiceExport.h"

//计算变量标识
enum CALVALUEFLAG
{
	CALVALUEFLAG_HIGH	= 0,//上线
	CALVALUEFLAG_SELF,		//自身
	CALVALUEFLAG_LESS,		//下线
	CALVALUEFLAG_COUNT,
};

//计算单元
class CCalculateItem : public ICalculateItem
{
	//友元类
	friend class CCalculateFrame;
public:
	//构造函数
	CCalculateItem(void);
	//析构函数
	~CCalculateItem(void);

	//引用接口
public:
	//增加引用
	virtual void __cdecl AddUserRefCount();
	//减少引用
	virtual void __cdecl ReleaseUserRefCount();
	//判断引用
	virtual bool __cdecl IsUserRefCountZero();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCalculateItem))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ICalculateItem框架接口
public:
	//用户 I D
	virtual DWORD __cdecl GetUserID() { return m_ServerUserData.UserData.dwUserID; }
	//父用户 I D
	virtual DWORD __cdecl GetParentUserID(){return m_ServerUserData.UserData.dwParentUserID;}
	//用户等级
	virtual BYTE __cdecl GetUserLevel(){ return m_ServerUserData.UserData.cbLevel;}
	//获取信息
	virtual tagServerUserData * __cdecl GetUserData() { return &m_ServerUserData; };
	//获取用户椅子ID
	virtual WORD	 __cdecl GetChairID(){return m_wChairID;}

	//获取总投注
	virtual bool __cdecl GetTotalBetScore(DECIMAL* pTotalBetScore);
	//获取总派彩
	virtual bool __cdecl GetTotalWinScore(DECIMAL* pTotalWinScore);
	//获取总抽水
	virtual bool __cdecl GetTotalTaxScore(DECIMAL* pTotalTaxScore);

	//获取派彩占成
	virtual bool __cdecl GetWinScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore);
	//获取抽水占成
	virtual bool __cdecl GetTaxScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore);
	//获取洗码与交收额度
	virtual bool __cdecl GetValidBetScoreAndPaidScore(DECIMAL* pTotalScore,
		DECIMAL* pHighScore, DECIMAL *pHighRollbackScore,
		DECIMAL* pLessScore, DECIMAL *pLessRollbackScore,
		DECIMAL* pHighPaidScore, DECIMAL* pLessPaidScore);
	//获取投注详细信息
	virtual bool __cdecl GetDetailBetScore(BYTE *pDetailBetScore,
											WORD& wDetailBetScoreSize);

	//复位计算
	virtual bool __cdecl Reset();

	//配置接口
public:
	//初始化
	virtual bool __cdecl InitCalculateItem(IServerUserItem*	pIServerUserItem,
											CCalculateFrame* pCalculateFrame,
											WORD wBetRegionCount,
											WORD wChairID = INVALID_CHAIR);
	//辅助函数
public:
	//获取上线
	CCalculateItem*	__cdecl	GetParent(){return m_pParentCalculateItem;}
	//设置上先
	void __cdecl SetParent(CCalculateItem* pItem){m_pParentCalculateItem = pItem;}
	//设置激活
	void __cdecl Active(bool b){m_bActive = b;}

	//用户体系正确
	virtual bool __cdecl IsUserTreeFrameOK();

	//投注
	bool __cdecl BetScore(DECIMAL *pdecBetScore, WORD wCount);
	//投注
	bool __cdecl BetScore(DECIMAL* pdecUserBetScore,
		DECIMAL* pdecLessBetScore,
		CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
		CCalculateItem* pLessCalculateItem);
	//相加自身单项投注
	bool __cdecl AddPartedBetScore(DECIMAL* pdecUserBetScore,
		DECIMAL* pdecHighBetScore,
		DECIMAL* pdecSelfBetScore,
		DECIMAL* pdecLessBetScore);
	//相加上线单项投注
	bool __cdecl AddParentPartedBetScore(DECIMAL* pdecUserBetScore,
		DECIMAL* pdecHighBetScore,
		DECIMAL* pdecSelfBetScore,
		DECIMAL* pdecLessBetScore);

	//派彩
	bool __cdecl WinScore(DECIMAL *pdecWinScore,
		DECIMAL *pdecUserTaxScore, 
		WORD wCount);
	//派彩
	bool __cdecl WinScore(DECIMAL* pdecUserWinScore,
		DECIMAL* pdecLessWinScore,
		DECIMAL* pdecUserTaxScore,
		DECIMAL *pdecLessTaxScore, 
		CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
		CCalculateItem* pLessCalculateItem);

	//相加自身单项派彩
	bool __cdecl AddPartedWinScore(DECIMAL* pdecUserWinScore,
		DECIMAL* pdecHighWinScore,
		DECIMAL* pdecSelfWinScore,
		DECIMAL* pdecLessWinScore);
	//相加上线单项派彩
	bool __cdecl AddParentPartedWinScore(DECIMAL* pdecUserWinScore,
		DECIMAL* pdecHighWinScore,
		DECIMAL* pdecSelfWinScore,
		DECIMAL* pdecLessWinScore);

	//相加自身单项抽水
	bool __cdecl AddPartedTaxScore(DECIMAL* pdecUserTaxScore,
		DECIMAL* pdecHighTaxScore,
		DECIMAL* pdecSelfTaxScore,
		DECIMAL* pdecLessTaxScore);
	//相加上线单项抽水
	bool __cdecl AddParentPartedTaxScore(DECIMAL* pdecUserTaxScore,
		DECIMAL* pdecHighTaxScore,
		DECIMAL* pdecSelfTaxScore,
		DECIMAL* pdecLessTaxScore);

	//计算派彩
	bool __cdecl CalculateWinScore(void* pCalculateContext,
		ICalculateSink* pCalculateSink);
	//计算总结果
	bool __cdecl CalculateResult(void* pCalculateContext,
		ICalculateSink* pCalculateSink);

	//变量定义
protected:
	bool				m_bActive;	
	LONG				m_lUserRefCount;				//下线引用计数
	CCalculateItem*		m_pParentCalculateItem;			//父亲

	CCalculateFrame*	m_pCalculateFrame;				//计算框架
	tagServerUserData	m_ServerUserData;				//用户信息
	WORD				m_wChairID;						//桌子位置
	
	WORD				m_wBetRegionCount;				//投注区域数目
	//计算信息
protected:
	DECIMAL				m_TotalBetScore;				//总投注

	//单项投注信息
protected:
	DECIMAL*			m_TotalPartedBetScore;			//单项投注区域总投注信息
	DECIMAL*			m_PartedBetScore[CALVALUEFLAG_COUNT];//自身单项投注区域投注信息
	DECIMAL*			m_ParentPartedBetScore[CALVALUEFLAG_COUNT];//上线单项投注区域投注信息

	//单项派彩信息
protected:
	DECIMAL*			m_TotalPartedWinScore;			//单项投注区域总派彩信息
	DECIMAL*			m_PartedWinScore[CALVALUEFLAG_COUNT];//自身单项投注区域派彩信息
	DECIMAL*			m_ParentPartedWinScore[CALVALUEFLAG_COUNT];//上线单项投注区域派彩信息

	//单项抽水信息
protected:
	DECIMAL*			m_TotalPartedTaxScore;			//单项投注区域总抽水信息
	DECIMAL*			m_PartedTaxScore[CALVALUEFLAG_COUNT];//自身单项投注区域抽水信息
	DECIMAL*			m_ParentPartedTaxScore[CALVALUEFLAG_COUNT];//上线单项投注区域抽水信息

	//合项投注信息
protected:
	DECIMAL				m_TotalWinScore;				//总派彩
	DECIMAL				m_WinScore[CALVALUEFLAG_COUNT]; //自身派彩信息
	DECIMAL				m_ParentWinScore[CALVALUEFLAG_COUNT]; //上线派彩信息

	DECIMAL				m_TotalValidBetScore;			//总洗码
	DECIMAL				m_ValidBetScore[CALVALUEFLAG_COUNT]; //自身洗码信息
	DECIMAL				m_ParentValidBetScore[CALVALUEFLAG_COUNT]; //上线洗码信息

	DECIMAL				m_TotalTaxScore;				//总抽水
	DECIMAL				m_TaxScore[CALVALUEFLAG_COUNT]; //自身抽水信息
	DECIMAL				m_ParentTaxScore[CALVALUEFLAG_COUNT]; //上线抽水信息
};
//////////////////////////////////////////////////////////////////////////
//类说明
typedef CArrayTemplate<CCalculateItem *> CCalculateItemArray;
typedef CArrayTemplate<DWORD>			 CCalculateItemPosArray;

//计算框架
class CCalculateFrame : public ICalculateFrame
{
	//友元类
	friend class CCalculateItem;

public:
	//构造函数
	CCalculateFrame(void);
	//析构函数
	~CCalculateFrame(void);

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCalculateFrame))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ICalculateFrame框架接口
public:
	//枚举计算单元
	virtual ICalculateItem* __cdecl EnumCalculateItem(WORD wEnumIndex);
	//获取计算单元数目
	virtual DWORD	__cdecl GetCalculateItemCount();

	//获取总投注
	virtual bool __cdecl GetFrameTotalBetScore(DECIMAL* pTotalBetScore);
	//获取总派彩
	virtual bool __cdecl GetFrameTotalWinScore(DECIMAL* pTotalWinScore);

	//用户坐下
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//复位计算
	virtual bool __cdecl Reset();
	//构建计算信息
	virtual bool __cdecl StructCalculateItem();

	//投注
	virtual bool __cdecl BetScore(IServerUserItem * pIServerUserItem,
									tagBetScoreInfo *pBetScoreInfo, WORD wCount);
	//检查投注
	virtual BYTE __cdecl CheckBetScore(IServerUserItem * pIServerUserItem,
									tagBetScoreInfo *pBetScoreInfo, WORD wCount);

	//计算代理对会员方式
	virtual bool __cdecl CalculateCTP(void* pCalculateContext,
									ICalculateSink* pCalculateSink);
	//计算会员对会员方式
	virtual bool __cdecl CalculatePTP(tagBetScoreInfo *pBetScoreInfo, WORD wCount);

	//配置接口
public:
	//初始化
	virtual bool __cdecl InitCalculateFrame(WORD wTableID, 
		ITableFrameManager*	pITableFrameManager,
		tagTableFrameParameter * pTableFrameParameter);

	//辅助函数
protected:
	//添加计算单元
	CCalculateItem* AddAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem);
	//删除计算单元
	bool		    DelAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem);
	//查找计算单元
	CCalculateItem* SearchCalculateItem(DWORD dwUserID);
	//增加计算单元
	void			AddUserCalculateItem(CCalculateItem* pItem);
	//结算回调
	void			BetScoreSink(ICalculateItem * pICalculateItem, DECIMAL* pdecTotalBetScore);

	//配置信息
protected:
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//服务属性
	const tagGameServiceOption		* m_pGameServiceOption;				//服务配置
	IGameServiceFrame				* m_pIGameServiceFrame;				//功能接口
	IServerUserManager				* m_pIServerUserManager;			//用户接口
	ITableFrameManager				* m_pITableFrameManager;			//桌子接口
	WORD							m_wTableID;							//桌子ID

	//计算定义
protected:
	CCalculateItemArray				m_CalculateItemArray;				//计算单元数组
	CCalculateItem*					m_pCompancyCalculateItem;			//公司结算单元

	//组件变量
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//数据库通知
};
