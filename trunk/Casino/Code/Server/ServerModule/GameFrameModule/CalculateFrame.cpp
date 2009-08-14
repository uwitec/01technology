#include "stdafx.h"
#include ".\calculateframe.h"
#include "DataBaseSink.h"
//投注信息转换大数
inline void  Convert_BetScoreInfoToBetDecimal(tagBetScoreInfo *pBetScoreInfo, WORD wBetScoreInfoCount,
											 DECIMAL* pBetDecimal, WORD wBetDecimalCount)
{
	DECIMAL decTemp;
	for(int i = 0; i < wBetScoreInfoCount; i ++)
	{
		if(pBetScoreInfo[i].wBetRegionIndex < wBetDecimalCount)
		{
			VarDecFromUI4(pBetScoreInfo[i].dwBetScore,&decTemp);
			VarDecAdd(&decTemp, &pBetDecimal[pBetScoreInfo[i].wBetRegionIndex],
				&pBetDecimal[pBetScoreInfo[i].wBetRegionIndex]);
		}
	}
}
//大数数组累加
inline void DecimalArrayAdd(DECIMAL* pdecLeftArr, DECIMAL* pdecRightArr, DECIMAL* pdecResultArr,WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecAdd(&pdecLeftArr[i], &pdecRightArr[i], &pdecResultArr[i]);
	}
}
//大数数组累减
inline void DecimalArraySub(DECIMAL* pdecLeftArr, DECIMAL* pdecRightArr, DECIMAL* pdecResultArr,WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecSub(&pdecLeftArr[i], &pdecRightArr[i], &pdecResultArr[i]);
	}
}
//大数数组累乘
inline void DecimalArrayMulSingleDec(DECIMAL* pdecLeftArr, DECIMAL* pdecRight, DECIMAL* pdecResultArr,WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecMul(&pdecLeftArr[i], pdecRight, &pdecResultArr[i]);
	}
}
//大数数组和
inline void DecimalArraySum(DECIMAL* pdecDecimalArr, DECIMAL* pdecResult, WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecAdd(&pdecDecimalArr[i], pdecResult, pdecResult);
	}
}

//构造函数
CCalculateItem::CCalculateItem(void)
{
	m_lUserRefCount	   = 0;
	m_pParentCalculateItem = NULL;
	m_pCalculateFrame	   = NULL;
	m_wChairID			   = INVALID_CHAIR;
	m_wBetRegionCount	   = 0;
	m_TotalPartedBetScore  = NULL;
	m_TotalPartedWinScore  = NULL;
	m_TotalPartedTaxScore  = NULL;
	for(int i = 0; i < CALVALUEFLAG_COUNT; i ++)
	{
		m_PartedBetScore[i] = NULL;
		m_ParentPartedBetScore[i] = NULL;
		m_PartedWinScore[i] = NULL;
		m_ParentPartedWinScore[i] = NULL;
		m_PartedTaxScore[i] = NULL;
		m_ParentPartedTaxScore[i] = NULL;
	}
}

//析构函数
CCalculateItem::~CCalculateItem(void)
{
	SafeDeleteArray(m_TotalPartedBetScore);
	SafeDeleteArray(m_TotalPartedWinScore);
	SafeDeleteArray(m_TotalPartedTaxScore);

	for(int i = 0; i < CALVALUEFLAG_COUNT; i ++)
	{
		SafeDeleteArray(m_PartedBetScore[i]);
		SafeDeleteArray(m_ParentPartedBetScore[i]);
		SafeDeleteArray(m_PartedWinScore[i]);
		SafeDeleteArray(m_ParentPartedWinScore[i]);
		SafeDeleteArray(m_PartedTaxScore[i]);
		SafeDeleteArray(m_ParentPartedTaxScore[i]);
	}
}

//接口查询
void * __cdecl CCalculateItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICalculateItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICalculateItem,Guid,dwQueryVer);
	return NULL;
}

//框架接口
//增加引用
void __cdecl  CCalculateItem::AddUserRefCount()
{
	m_lUserRefCount++;
}

//减少引用
void __cdecl  CCalculateItem::ReleaseUserRefCount()
{
	m_lUserRefCount--;
}

//判断引用
bool __cdecl  CCalculateItem::IsUserRefCountZero()
{
	return m_lUserRefCount <= 0;
}

//判断用户体系正确
bool __cdecl CCalculateItem::IsUserTreeFrameOK()
{
	if(this->GetUserData()->UserData.cbLevel == 0)
		return true;
	else
	{
		//GT_ASSERT(GetParent());
		if(GetParent())
			return GetParent()->IsUserTreeFrameOK();
		else
			return false;
	}
}

//获取总投注
bool __cdecl CCalculateItem::GetTotalBetScore(DECIMAL* pTotalBetScore)
{
	memcpy(pTotalBetScore, &m_TotalBetScore, sizeof(DECIMAL));
	return true;
}

//获取总派彩
bool __cdecl CCalculateItem::GetTotalWinScore(DECIMAL* pTotalWinScore)
{
	memcpy(pTotalWinScore, &m_TotalWinScore, sizeof(DECIMAL));
	return true;
}

//获取总抽水
bool __cdecl CCalculateItem::GetTotalTaxScore(DECIMAL* pTotalTaxScore)
{
	memcpy(pTotalTaxScore, &m_TotalTaxScore, sizeof(DECIMAL));
	return true;
}

//获取派彩占成
bool __cdecl CCalculateItem::GetWinScoreOccupancy(DECIMAL* pHighScore,
	DECIMAL* pSelfScore,DECIMAL *pLessScore)
{
	memcpy(pHighScore, &m_ParentWinScore[CALVALUEFLAG_HIGH], sizeof(DECIMAL));
	memcpy(pSelfScore, &m_ParentWinScore[CALVALUEFLAG_SELF], sizeof(DECIMAL));
	VarDecSub(&m_TotalWinScore,&m_ParentWinScore[CALVALUEFLAG_LESS],pLessScore); 
	return true;
}

//获取派彩占成
bool __cdecl CCalculateItem::GetTaxScoreOccupancy(DECIMAL* pHighScore,
												  DECIMAL* pSelfScore,DECIMAL *pLessScore)
{
	memcpy(pHighScore, &m_ParentTaxScore[CALVALUEFLAG_HIGH], sizeof(DECIMAL));
	memcpy(pSelfScore, &m_ParentTaxScore[CALVALUEFLAG_SELF], sizeof(DECIMAL));
	VarDecSub(&m_TotalTaxScore,&m_ParentTaxScore[CALVALUEFLAG_LESS],pLessScore); 
	return true;
}

//获取洗码与交收额度
bool __cdecl CCalculateItem::GetValidBetScoreAndPaidScore(DECIMAL* pTotalScore,
	DECIMAL* pHighScore, DECIMAL *pHighRollbackScore,
	DECIMAL* pLessScore, DECIMAL *pLessRollbackScore,
	DECIMAL* pHighPaidScore, DECIMAL* pLessPaidScore)
{
	DECIMAL decTemp;
	memset(&decTemp, 0, sizeof(DECIMAL));
	//获取洗码
	memcpy(pTotalScore, &m_TotalValidBetScore, sizeof(DECIMAL));
	memcpy(pHighScore, &m_ParentValidBetScore[CALVALUEFLAG_HIGH], sizeof(DECIMAL));
	memcpy(pLessScore, &m_ParentValidBetScore[CALVALUEFLAG_LESS], sizeof(DECIMAL));
	if(this->GetParent())
	{
		memcpy(&decTemp, &this->GetParent()->GetUserData()->decRollbackRate, sizeof(DECIMAL));
		VarDecMul(pHighScore, &decTemp, pHighRollbackScore);
	}
	else
	{
		memset(pHighRollbackScore, 0, sizeof(DECIMAL));
	}
	VarDecFromR8(-1.0,&decTemp);
	VarDecMul(&this->GetUserData()->decRollbackRate, &decTemp, &decTemp);
	VarDecMul(pLessScore, &decTemp, pLessRollbackScore);
	//获取交收额度
	VarDecAdd(&m_ParentWinScore[CALVALUEFLAG_HIGH], pHighRollbackScore, pHighPaidScore);
	VarDecFromR8(-1.0,&decTemp);
	VarDecMul(&m_ParentWinScore[CALVALUEFLAG_LESS], &decTemp, pLessPaidScore);
	VarDecAdd(pLessPaidScore, pLessRollbackScore, pLessPaidScore);

	return true;
}

//获取投注详细信息
bool __cdecl CCalculateItem::GetDetailBetScore(BYTE *pDetailBetScore,
							   WORD& wDetailBetScoreSize)
{
	wDetailBetScoreSize = sizeof(DOUBLE) * m_wBetRegionCount;
	DOUBLE	*pDouble = (DOUBLE*)pDetailBetScore;
	DECIMAL *pDecimal= m_PartedBetScore[CALVALUEFLAG_SELF];
	for(int i = 0; i < m_wBetRegionCount; i ++)
	{
		VarR8FromDec(pDecimal, pDouble);
		pDouble  ++;
		pDecimal ++;
	}
	return true;
}

//复位计算
bool __cdecl CCalculateItem::Reset()
{
	memset(&m_TotalBetScore, 0, sizeof(DECIMAL));
	memset(m_TotalPartedBetScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	memset(m_TotalPartedWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	memset(m_TotalPartedTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);

	for(int i = 0; i < CALVALUEFLAG_COUNT; i ++)
	{
		memset(m_PartedBetScore[i], 0, sizeof(DECIMAL) * m_wBetRegionCount);
		memset(m_ParentPartedBetScore[i], 0, sizeof(DECIMAL) * m_wBetRegionCount);
		memset(m_PartedWinScore[i], 0, sizeof(DECIMAL) * m_wBetRegionCount);
		memset(m_ParentPartedWinScore[i], 0, sizeof(DECIMAL) * m_wBetRegionCount);
		memset(m_PartedTaxScore[i], 0, sizeof(DECIMAL) * m_wBetRegionCount);
		memset(m_ParentPartedTaxScore[i], 0, sizeof(DECIMAL) * m_wBetRegionCount);
	}

	memset(&m_TotalWinScore, 0, sizeof(DECIMAL));
	memset(m_WinScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(m_ParentWinScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	memset(&m_TotalValidBetScore, 0, sizeof(DECIMAL));
	memset(m_ValidBetScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(m_ParentValidBetScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	memset(&m_TotalTaxScore, 0, sizeof(DECIMAL));
	memset(m_TaxScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(m_ParentTaxScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	return true;
}

//初始化
bool __cdecl CCalculateItem::InitCalculateItem(IServerUserItem*	pIServerUserItem,
							   CCalculateFrame* pCalculateFrame,
							   WORD wBetRegionCount,
							   WORD wChairID)
{
	//效验参数
	GT_ASSERT(pIServerUserItem && pCalculateFrame && wBetRegionCount >= 1);
	if(!pIServerUserItem || !pCalculateFrame || wBetRegionCount < 1)
		return false;
	//设置变量
	memcpy(&m_ServerUserData, pIServerUserItem->GetUserData(), sizeof(tagServerUserData));
	m_pCalculateFrame  = pCalculateFrame;
	m_wBetRegionCount  = wBetRegionCount;
	m_wChairID		   = wChairID;

	//分配内存
	m_TotalPartedBetScore = new DECIMAL[m_wBetRegionCount];
	m_TotalPartedWinScore = new DECIMAL[m_wBetRegionCount];
	m_TotalPartedTaxScore = new DECIMAL[m_wBetRegionCount];
	for(int i = 0; i < CALVALUEFLAG_COUNT; i ++)
	{
		m_PartedBetScore[i] = new DECIMAL[m_wBetRegionCount];
		m_ParentPartedBetScore[i] = new DECIMAL[m_wBetRegionCount];
		m_PartedWinScore[i] = new DECIMAL[m_wBetRegionCount];
		m_ParentPartedWinScore[i] = new DECIMAL[m_wBetRegionCount];
		m_PartedTaxScore[i] = new DECIMAL[m_wBetRegionCount];
		m_ParentPartedTaxScore[i] = new DECIMAL[m_wBetRegionCount];
	}

	//初始化
	Reset();

	return true;
}

//投注
bool __cdecl CCalculateItem::BetScore(DECIMAL *pdecBetScore, WORD wCount)
{
	GT_ASSERT(m_wBetRegionCount == wCount);

	DECIMAL* decUserBetScore = new DECIMAL[m_wBetRegionCount];
	memcpy(decUserBetScore, pdecBetScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decLessBetScore = new DECIMAL[m_wBetRegionCount];
	memcpy(decLessBetScore, decUserBetScore, sizeof(DECIMAL) * m_wBetRegionCount);
	
	CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1];
	memset(pLessCalculateItemChain, 0 , sizeof(CCalculateItem*) * (USERLEVEL_GAMEUSER+1));
	pLessCalculateItemChain[this->GetUserData()->UserData.cbLevel] = this;

	if(GetParent()->BetScore(decUserBetScore, decLessBetScore, pLessCalculateItemChain, this))
	{
		if(this->GetUserData()->UserData.cbLevel == USERLEVEL_GAMEUSER)
		{
			DecimalArrayAdd(decUserBetScore, m_TotalPartedBetScore, m_TotalPartedBetScore, m_wBetRegionCount);
			DecimalArrayAdd(decUserBetScore, m_PartedBetScore[CALVALUEFLAG_SELF], m_PartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
			
			DECIMAL decTotalBetScore;
			memset(&decTotalBetScore, 0, sizeof(DECIMAL));
			DecimalArraySum(decUserBetScore, &decTotalBetScore, m_wBetRegionCount);
			VarDecAdd(&decTotalBetScore, &m_TotalBetScore, &m_TotalBetScore);

			m_pCalculateFrame->BetScoreSink(this,&decTotalBetScore);
		}
		SafeDeleteArray(decLessBetScore);
		SafeDeleteArray(decUserBetScore);
		return true;
	}
	SafeDeleteArray(decLessBetScore);
	SafeDeleteArray(decUserBetScore);
	return false;
}

//投注
bool __cdecl  CCalculateItem::BetScore(DECIMAL* pdecUserBetScore,
					  DECIMAL* pdecLessBetScore,
					  CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
					  CCalculateItem* pLessCalculateItem)
{
	pLessCalculateItemChain[this->GetUserData()->UserData.cbLevel]	= this;

	DECIMAL* decHighBetScore = new DECIMAL[m_wBetRegionCount];
	memset(decHighBetScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decSelfBetScore = new DECIMAL[m_wBetRegionCount];
	memset(decSelfBetScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decLessBetScore = new DECIMAL[m_wBetRegionCount];
	memcpy(decLessBetScore, pdecLessBetScore, sizeof(DECIMAL) * m_wBetRegionCount);

	if(this->GetUserData()->UserData.cbOccupancyRate_NoFlag)
	{

	}
	else
	{
		DECIMAL decLessOccupancyRate;
		memset(&decLessOccupancyRate, 0, sizeof(DECIMAL));

		for(int i = this->GetUserData()->UserData.cbLevel + 1; i < USERLEVEL_GAMEUSER; i ++)
		{
			if(pLessCalculateItemChain[i] && 
				pLessCalculateItemChain[i]->GetUserData()->UserData.cbOccupancyRate_NoFlag == 0)
			{
				memcpy(&decLessOccupancyRate, &pLessCalculateItemChain[i]->GetUserData()->decOccupancyRate,
					sizeof(DECIMAL));
				break;
			}
		}
		DECIMAL decMyOccupancyRate;
		memset(&decMyOccupancyRate, 0, sizeof(DECIMAL));
		memcpy(&decMyOccupancyRate, &this->GetUserData()->decOccupancyRate, sizeof(DECIMAL));

		GT_ASSERT(VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_EQ ||
			VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_LT);
		if(VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_GT)
			decLessOccupancyRate = decMyOccupancyRate;

		DECIMAL decMarginOccupancyRate;
		memset(&decMarginOccupancyRate, 0, sizeof(DECIMAL));
		VarDecSub(&decMyOccupancyRate,
			&decLessOccupancyRate, &decMarginOccupancyRate);

		DecimalArrayMulSingleDec(pdecUserBetScore, &decMarginOccupancyRate, decSelfBetScore, m_wBetRegionCount);

	}

	if(this->GetUserData()->UserData.cbLevel != 0)
	{
		DecimalArraySub(decLessBetScore, decSelfBetScore, decHighBetScore, m_wBetRegionCount);
	}


	AddPartedBetScore(pdecUserBetScore,decHighBetScore,decSelfBetScore,decLessBetScore);
	pLessCalculateItem->AddParentPartedBetScore(pdecUserBetScore,decHighBetScore,decSelfBetScore,decLessBetScore);
	
	bool bResult = false;
	if(this->GetUserData()->UserData.cbLevel == 0)
	{
		bResult = true;
	}
	else
	{
		bResult = GetParent()->BetScore(pdecUserBetScore, decHighBetScore, pLessCalculateItemChain, this);
	}
	SafeDeleteArray(decHighBetScore);
	SafeDeleteArray(decSelfBetScore);
	SafeDeleteArray(decLessBetScore);

	return bResult;
}

//相加自身单项投注
bool __cdecl CCalculateItem::AddPartedBetScore(DECIMAL* pdecUserBetScore,
							   DECIMAL* pdecHighBetScore,
							   DECIMAL* pdecSelfBetScore,
							   DECIMAL* pdecLessBetScore)
{
	DecimalArrayAdd(pdecUserBetScore,m_TotalPartedBetScore,m_TotalPartedBetScore, m_wBetRegionCount);
	DecimalArrayAdd(pdecHighBetScore,m_PartedBetScore[CALVALUEFLAG_HIGH],m_PartedBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(pdecSelfBetScore,m_PartedBetScore[CALVALUEFLAG_SELF],m_PartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(pdecLessBetScore,m_PartedBetScore[CALVALUEFLAG_LESS],m_PartedBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(pdecUserBetScore, &m_TotalBetScore, m_wBetRegionCount);

	return true;
}

//相加上线单项投注
bool __cdecl CCalculateItem::AddParentPartedBetScore(DECIMAL* pdecUserBetScore,
									 DECIMAL* pdecHighBetScore,
									 DECIMAL* pdecSelfBetScore,
									 DECIMAL* pdecLessBetScore)
{
	DecimalArrayAdd(pdecHighBetScore,m_ParentPartedBetScore[CALVALUEFLAG_HIGH],m_ParentPartedBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(pdecSelfBetScore,m_ParentPartedBetScore[CALVALUEFLAG_SELF],m_ParentPartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(pdecLessBetScore,m_ParentPartedBetScore[CALVALUEFLAG_LESS],m_ParentPartedBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	return true;
}

//派彩
bool __cdecl CCalculateItem::WinScore(DECIMAL *pdecWinScore,
									  DECIMAL* pdecUserTaxScore,
									  WORD wCount)
{
	GT_ASSERT(m_wBetRegionCount == wCount);

	//拷贝信息
	DECIMAL* decUserWinScore = new DECIMAL[m_wBetRegionCount];					//自身派彩
	memcpy(decUserWinScore, pdecWinScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decLessWinScore = new DECIMAL[m_wBetRegionCount];					//下线派彩
	memcpy(decLessWinScore, decUserWinScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decUserTaxScore = new DECIMAL[m_wBetRegionCount];					//自身抽水
	memcpy(decUserTaxScore, pdecUserTaxScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decLessTaxScore = new DECIMAL[m_wBetRegionCount];					//下线抽水
	memcpy(decLessTaxScore, decUserTaxScore, sizeof(DECIMAL) * m_wBetRegionCount);

	//构建树型链
	CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1];
	memset(pLessCalculateItemChain, 0 , sizeof(CCalculateItem*) * (USERLEVEL_GAMEUSER+1));
	pLessCalculateItemChain[this->GetUserData()->UserData.cbLevel] = this;

	//计算上线
	if(GetParent()->WinScore(decUserWinScore, 
		decLessWinScore, 
		decUserTaxScore, 
		decLessTaxScore,
		pLessCalculateItemChain, this))
	{
		//判断会员,计算派彩
		if(this->GetUserData()->UserData.cbLevel == USERLEVEL_GAMEUSER)
		{
			//累加派彩
			DecimalArrayAdd(decUserWinScore, m_TotalPartedWinScore, m_TotalPartedWinScore, m_wBetRegionCount);
			DecimalArrayAdd(decUserWinScore, m_PartedWinScore[CALVALUEFLAG_SELF], m_PartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
			
			//累加抽水
			DecimalArrayAdd(decUserTaxScore, m_TotalPartedTaxScore, m_TotalPartedTaxScore, m_wBetRegionCount);
			DecimalArrayAdd(decUserTaxScore, m_PartedTaxScore[CALVALUEFLAG_SELF], m_PartedTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
		}
		SafeDeleteArray(decLessWinScore);
		SafeDeleteArray(decUserWinScore);
		SafeDeleteArray(decLessTaxScore);
		SafeDeleteArray(decUserTaxScore);
		return true;
	}
	SafeDeleteArray(decLessWinScore);
	SafeDeleteArray(decUserWinScore);
	SafeDeleteArray(decLessTaxScore);
	SafeDeleteArray(decUserTaxScore);
	return false;
}

//派彩
bool __cdecl  CCalculateItem::WinScore(DECIMAL* pdecUserWinScore,
									   DECIMAL* pdecLessWinScore,
									   DECIMAL* pdecUserTaxScore,
									   DECIMAL *pdecLessTaxScore, 
									   CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
									   CCalculateItem* pLessCalculateItem)
{
	//设置自己树型节点
	pLessCalculateItemChain[this->GetUserData()->UserData.cbLevel]	= this;

	//////////////////////////////////////////////////////////////////////////
	//计算抽水

	//申请计算缓冲数据
	DECIMAL* decHighTaxScore = new DECIMAL[m_wBetRegionCount];				//上线
	memset(decHighTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decSelfTaxScore = new DECIMAL[m_wBetRegionCount];				//自身
	memset(decSelfTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decLessTaxScore = new DECIMAL[m_wBetRegionCount];				//下线
	memcpy(decLessTaxScore, pdecLessTaxScore, sizeof(DECIMAL) * m_wBetRegionCount);//拷贝下线抽水信息

	//计算下线抽水占成比
	DECIMAL decLessTaxOccupancyRate;
	memset(&decLessTaxOccupancyRate, 0, sizeof(DECIMAL));

	//寻找下线抽水占成比
	for(int i = this->GetUserData()->UserData.cbLevel + 1; i < USERLEVEL_GAMEUSER; i ++)
	{
		if(pLessCalculateItemChain[i])
		{
			memcpy(&decLessTaxOccupancyRate, &pLessCalculateItemChain[i]->GetUserData()->decTaxOccupancyRate,
				sizeof(DECIMAL));
			break;
		}
	}

	//获取自身抽水占成比
	DECIMAL decMyTaxOccupancyRate;
	memcpy(&decMyTaxOccupancyRate, &this->GetUserData()->decTaxOccupancyRate, sizeof(DECIMAL));

	//判断下线不能大于自身抽水占成比
	GT_ASSERT(VarDecCmp(&decLessTaxOccupancyRate, &decMyTaxOccupancyRate) == VARCMP_EQ ||
		VarDecCmp(&decLessTaxOccupancyRate, &decMyTaxOccupancyRate) == VARCMP_LT);
	if(VarDecCmp(&decLessTaxOccupancyRate, &decMyTaxOccupancyRate) == VARCMP_GT)
		decLessTaxOccupancyRate = decMyTaxOccupancyRate;

	//自身实际抽水占成数=自身抽水占成比-下线抽水占成比
	DECIMAL decMarginTaxOccupancyRate;//自身实际抽水占成数
	VarDecSub(&decMyTaxOccupancyRate,
		&decLessTaxOccupancyRate, &decMarginTaxOccupancyRate);

	//自身抽水=自身实际抽水占成数*总抽水
	DecimalArrayMulSingleDec(pdecUserTaxScore, &decMarginTaxOccupancyRate, decSelfTaxScore, m_wBetRegionCount);

	//上线抽水=下线抽水-自身抽水
	if(this->GetUserData()->UserData.cbLevel != 0)
	{
		DecimalArraySub(decLessTaxScore, decSelfTaxScore, decHighTaxScore, m_wBetRegionCount);
	}

	//累加当前自身抽水信息
	AddPartedTaxScore(pdecUserTaxScore,decHighTaxScore,decSelfTaxScore,decLessTaxScore);

	//通知下线累加当前自身抽水信息
	pLessCalculateItem->AddParentPartedTaxScore(pdecUserTaxScore,decHighTaxScore,decSelfTaxScore,decLessTaxScore);


	//////////////////////////////////////////////////////////////////////////
	//计算派彩

	//申请计算缓冲数据
	DECIMAL* decHighWinScore = new DECIMAL[m_wBetRegionCount];				//上线
	memset(decHighWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decSelfWinScore = new DECIMAL[m_wBetRegionCount];				//自身
	memset(decSelfWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decLessWinScore = new DECIMAL[m_wBetRegionCount];				//下线
	memcpy(decLessWinScore, pdecLessWinScore, sizeof(DECIMAL) * m_wBetRegionCount);//拷贝下线派彩信息
	
	//判断不占成
	if(this->GetUserData()->UserData.cbOccupancyRate_NoFlag)
	{

	}
	else
	{
		//计算下线占成比
		DECIMAL decLessOccupancyRate;
		memset(&decLessOccupancyRate, 0, sizeof(DECIMAL));

		//寻找下线占成比,如果下线为不占成,则再往下寻找
		for(int i = this->GetUserData()->UserData.cbLevel + 1; i < USERLEVEL_GAMEUSER; i ++)
		{
			if(pLessCalculateItemChain[i] && 
				pLessCalculateItemChain[i]->GetUserData()->UserData.cbOccupancyRate_NoFlag == 0)
			{
				memcpy(&decLessOccupancyRate, &pLessCalculateItemChain[i]->GetUserData()->decOccupancyRate,
					sizeof(DECIMAL));
				break;
			}
		}

		//获取自身占成比
		DECIMAL decMyOccupancyRate;
		memcpy(&decMyOccupancyRate, &this->GetUserData()->decOccupancyRate, sizeof(DECIMAL));

		//判断下线不能大于自身占成比
		GT_ASSERT(VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_EQ ||
			VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_LT);
		if(VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_GT)
			decLessOccupancyRate = decMyOccupancyRate;

		//自身实际占成数=自身占成比-下线占成比
		DECIMAL decMarginOccupancyRate;//自身实际占成数
		VarDecSub(&decMyOccupancyRate,
			&decLessOccupancyRate, &decMarginOccupancyRate);

		//自身派彩=自身实际占成数*总派彩
		DecimalArrayMulSingleDec(pdecUserWinScore, &decMarginOccupancyRate, decSelfWinScore, m_wBetRegionCount);

	}

	//上线派彩=下线派彩-自身派彩
	if(this->GetUserData()->UserData.cbLevel != 0)
	{
		DecimalArraySub(decLessWinScore, decSelfWinScore, decHighWinScore, m_wBetRegionCount);
	}

	//累加当前自身派彩信息
	AddPartedWinScore(pdecUserWinScore,decHighWinScore,decSelfWinScore,decLessWinScore);

	//通知下线累加当前自身派彩信息
	pLessCalculateItem->AddParentPartedWinScore(pdecUserWinScore,decHighWinScore,decSelfWinScore,decLessWinScore);

	//判断计算结束,当前为公司帐号结束
	bool bResult = false;
	if(this->GetUserData()->UserData.cbLevel == 0)
		bResult = true;
	else
	{
		//计算上线
		bResult = GetParent()->WinScore(pdecUserWinScore, 
			decHighWinScore, 
			pdecUserTaxScore,
			decHighTaxScore,
			pLessCalculateItemChain, 
			this);
	}
	
	//释放内存
	SafeDeleteArray(decHighTaxScore);
	SafeDeleteArray(decSelfTaxScore);
	SafeDeleteArray(decLessTaxScore);

	SafeDeleteArray(decHighWinScore);
	SafeDeleteArray(decSelfWinScore);
	SafeDeleteArray(decLessWinScore);

	return bResult;
}

//相加自身单项派彩
bool __cdecl CCalculateItem::AddPartedWinScore(DECIMAL* pdecUserWinScore,
											   DECIMAL* pdecHighWinScore,
											   DECIMAL* pdecSelfWinScore,
											   DECIMAL* pdecLessWinScore)
{
	DecimalArrayAdd(pdecUserWinScore,m_TotalPartedWinScore,m_TotalPartedWinScore, m_wBetRegionCount);
	
	DecimalArrayAdd(pdecHighWinScore,m_PartedWinScore[CALVALUEFLAG_HIGH],m_PartedWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(pdecSelfWinScore,m_PartedWinScore[CALVALUEFLAG_SELF],m_PartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(pdecLessWinScore,m_PartedWinScore[CALVALUEFLAG_LESS],m_PartedWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	return true;
}

//相加上线单项派彩
bool __cdecl CCalculateItem::AddParentPartedWinScore(DECIMAL* pdecUserWinScore,
													 DECIMAL* pdecHighWinScore,
													 DECIMAL* pdecSelfWinScore,
													 DECIMAL* pdecLessWinScore)
{
	DecimalArrayAdd(pdecHighWinScore,m_ParentPartedWinScore[CALVALUEFLAG_HIGH],m_ParentPartedWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(pdecSelfWinScore,m_ParentPartedWinScore[CALVALUEFLAG_SELF],m_ParentPartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(pdecLessWinScore,m_ParentPartedWinScore[CALVALUEFLAG_LESS],m_ParentPartedWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	return true;
}

//相加自身单项抽水
bool __cdecl CCalculateItem::AddPartedTaxScore(DECIMAL* pdecUserTaxScore,
											   DECIMAL* pdecHighTaxScore,
											   DECIMAL* pdecSelfTaxScore,
											   DECIMAL* pdecLessTaxScore)
{
	DecimalArrayAdd(pdecUserTaxScore,m_TotalPartedTaxScore,m_TotalPartedTaxScore, m_wBetRegionCount);

	DecimalArrayAdd(pdecHighTaxScore,m_PartedTaxScore[CALVALUEFLAG_HIGH],m_PartedTaxScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(pdecSelfTaxScore,m_PartedTaxScore[CALVALUEFLAG_SELF],m_PartedTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(pdecLessTaxScore,m_PartedTaxScore[CALVALUEFLAG_LESS],m_PartedTaxScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	return true;
}

//相加上线单项抽水
bool __cdecl CCalculateItem::AddParentPartedTaxScore(DECIMAL* pdecUserTaxScore,
													 DECIMAL* pdecHighTaxScore,
													 DECIMAL* pdecSelfTaxScore,
													 DECIMAL* pdecLessTaxScore)
{
	DecimalArrayAdd(pdecHighTaxScore,m_ParentPartedTaxScore[CALVALUEFLAG_HIGH],m_ParentPartedTaxScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(pdecSelfTaxScore,m_ParentPartedTaxScore[CALVALUEFLAG_SELF],m_ParentPartedTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(pdecLessTaxScore,m_ParentPartedTaxScore[CALVALUEFLAG_LESS],m_ParentPartedTaxScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	return true;
}

//计算派彩
bool __cdecl CCalculateItem::CalculateWinScore(void* pCalculateContext,
											 ICalculateSink* pCalculateSink)
{

	//会员玩家计算
	if(this->GetUserData()->UserData.cbLevel != USERLEVEL_GAMEUSER)
		return true;

	//判断总投注为0
	if(VarDecCmpR8(&m_TotalBetScore, 0) == VARCMP_EQ)
		return true;

	GT_ASSERT(pCalculateSink);
	GT_ASSERT(m_wChairID != INVALID_CHAIR);

	DECIMAL* decUserWinScore = new DECIMAL[m_wBetRegionCount];				//派彩
	memset(decUserWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decUserTaxScore = new DECIMAL[m_wBetRegionCount];				//抽水
	memset(decUserTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);

	//计算派彩
	pCalculateSink->CalculateResult(pCalculateContext,
		m_wChairID,
		this,
		m_TotalPartedBetScore, 
		decUserWinScore,
		decUserTaxScore,
		m_wBetRegionCount);

	if(WinScore(decUserWinScore, decUserTaxScore, m_wBetRegionCount))
	{
		SafeDeleteArray(decUserWinScore);
		SafeDeleteArray(decUserTaxScore);
		return true;
	}
	else
	{
		SafeDeleteArray(decUserTaxScore);
		SafeDeleteArray(decUserWinScore);
		return false;
	}
}

//计算总结果
bool __cdecl CCalculateItem::CalculateResult(void* pCalculateContext,
					 ICalculateSink* pCalculateSink)
{
	GT_ASSERT(pCalculateSink);

	if(VarDecCmpR8(&m_TotalBetScore, 0) == VARCMP_EQ)
		return true;

	//////////////////////////////////////////////////////////////////////////
	//实际派彩 = 派彩 + 抽水所得

	DecimalArrayAdd(m_TotalPartedWinScore,m_TotalPartedTaxScore,m_TotalPartedWinScore, m_wBetRegionCount);
	DecimalArrayAdd(m_PartedWinScore[CALVALUEFLAG_HIGH],m_PartedTaxScore[CALVALUEFLAG_HIGH],m_PartedWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(m_PartedWinScore[CALVALUEFLAG_SELF],m_PartedTaxScore[CALVALUEFLAG_SELF],m_PartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(m_PartedWinScore[CALVALUEFLAG_LESS],m_PartedTaxScore[CALVALUEFLAG_LESS],m_PartedWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	DecimalArrayAdd(m_ParentPartedWinScore[CALVALUEFLAG_HIGH],m_ParentPartedTaxScore[CALVALUEFLAG_HIGH],m_ParentPartedWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(m_ParentPartedWinScore[CALVALUEFLAG_SELF],m_ParentPartedTaxScore[CALVALUEFLAG_SELF],m_ParentPartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(m_ParentPartedWinScore[CALVALUEFLAG_LESS],m_ParentPartedTaxScore[CALVALUEFLAG_LESS],m_ParentPartedWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	//////////////////////////////////////////////////////////////////////////
	//计算派彩

	DECIMAL		decTotalWinScore;				//总派彩
	DECIMAL		decWinScore[CALVALUEFLAG_COUNT]; //自身派彩信息
	DECIMAL		decParentWinScore[CALVALUEFLAG_COUNT]; //上线派彩信息
	memset(&decTotalWinScore, 0, sizeof(DECIMAL));
	memset(decWinScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(decParentWinScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	//相加派彩
	DecimalArraySum(m_TotalPartedWinScore, &decTotalWinScore, m_wBetRegionCount);
	DecimalArraySum(m_PartedWinScore[CALVALUEFLAG_HIGH], &decWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_PartedWinScore[CALVALUEFLAG_SELF], &decWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_PartedWinScore[CALVALUEFLAG_LESS], &decWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedWinScore[CALVALUEFLAG_HIGH], &decParentWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedWinScore[CALVALUEFLAG_SELF], &decParentWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedWinScore[CALVALUEFLAG_LESS], &decParentWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	//保存派彩结果
	VarDecAdd(&decTotalWinScore, &m_TotalWinScore, &m_TotalWinScore);
	VarDecAdd(&decWinScore[CALVALUEFLAG_HIGH], &m_WinScore[CALVALUEFLAG_HIGH], &m_WinScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decWinScore[CALVALUEFLAG_SELF], &m_WinScore[CALVALUEFLAG_SELF], &m_WinScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decWinScore[CALVALUEFLAG_LESS], &m_WinScore[CALVALUEFLAG_LESS], &m_WinScore[CALVALUEFLAG_LESS]);
	VarDecAdd(&decParentWinScore[CALVALUEFLAG_HIGH], &m_ParentWinScore[CALVALUEFLAG_HIGH], &m_ParentWinScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decParentWinScore[CALVALUEFLAG_SELF], &m_ParentWinScore[CALVALUEFLAG_SELF], &m_ParentWinScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decParentWinScore[CALVALUEFLAG_LESS], &m_ParentWinScore[CALVALUEFLAG_LESS], &m_ParentWinScore[CALVALUEFLAG_LESS]);

	//////////////////////////////////////////////////////////////////////////
	//计算抽水

	DECIMAL		decTotalTaxScore;				//总抽水
	DECIMAL		decTaxScore[CALVALUEFLAG_COUNT]; //自身抽水信息
	DECIMAL		decParentTaxScore[CALVALUEFLAG_COUNT]; //上线抽水信息
	memset(&decTotalTaxScore, 0, sizeof(DECIMAL));
	memset(decTaxScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(decParentTaxScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	//相加抽水
	DecimalArraySum(m_TotalPartedTaxScore, &decTotalTaxScore, m_wBetRegionCount);
	DecimalArraySum(m_PartedTaxScore[CALVALUEFLAG_HIGH], &decTaxScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_PartedTaxScore[CALVALUEFLAG_SELF], &decTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_PartedTaxScore[CALVALUEFLAG_LESS], &decTaxScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedTaxScore[CALVALUEFLAG_HIGH], &decParentTaxScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedTaxScore[CALVALUEFLAG_SELF], &decParentTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedTaxScore[CALVALUEFLAG_LESS], &decParentTaxScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	//保存抽水结果
	VarDecAdd(&decTotalTaxScore, &m_TotalTaxScore, &m_TotalTaxScore);
	VarDecAdd(&decTaxScore[CALVALUEFLAG_HIGH], &m_TaxScore[CALVALUEFLAG_HIGH], &m_TaxScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decTaxScore[CALVALUEFLAG_SELF], &m_TaxScore[CALVALUEFLAG_SELF], &m_TaxScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decTaxScore[CALVALUEFLAG_LESS], &m_TaxScore[CALVALUEFLAG_LESS], &m_TaxScore[CALVALUEFLAG_LESS]);
	VarDecAdd(&decParentTaxScore[CALVALUEFLAG_HIGH], &m_ParentTaxScore[CALVALUEFLAG_HIGH], &m_ParentTaxScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decParentTaxScore[CALVALUEFLAG_SELF], &m_ParentTaxScore[CALVALUEFLAG_SELF], &m_ParentTaxScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decParentTaxScore[CALVALUEFLAG_LESS], &m_ParentTaxScore[CALVALUEFLAG_LESS], &m_ParentTaxScore[CALVALUEFLAG_LESS]);

	//////////////////////////////////////////////////////////////////////////
	//计算洗码

	DECIMAL		decTotalValidBetScore;			//总洗码
	DECIMAL		decValidBetScore[CALVALUEFLAG_COUNT]; //自身洗码信息
	DECIMAL		decParentValidBetScore[CALVALUEFLAG_COUNT]; //上线洗码信息
	memset(&decTotalValidBetScore, 0, sizeof(DECIMAL));
	memset(decValidBetScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(decParentValidBetScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	//相加洗码货量
	DecimalArraySum(m_TotalPartedBetScore, &decTotalValidBetScore, m_wBetRegionCount);
	DecimalArraySum(m_PartedBetScore[CALVALUEFLAG_HIGH], &decValidBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_PartedBetScore[CALVALUEFLAG_SELF], &decValidBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_PartedBetScore[CALVALUEFLAG_LESS], &decValidBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedBetScore[CALVALUEFLAG_HIGH], &decParentValidBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedBetScore[CALVALUEFLAG_SELF], &decParentValidBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedBetScore[CALVALUEFLAG_LESS], &decParentValidBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	//调整洗码货量
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_TotalPartedBetScore, m_wBetRegionCount, &decTotalValidBetScore);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_PartedBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount, &decValidBetScore[CALVALUEFLAG_HIGH]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_PartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount, &decValidBetScore[CALVALUEFLAG_SELF]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_PartedBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount, &decValidBetScore[CALVALUEFLAG_LESS]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_ParentPartedBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount, &decParentValidBetScore[CALVALUEFLAG_HIGH]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_ParentPartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount, &decParentValidBetScore[CALVALUEFLAG_SELF]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_ParentPartedBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount, &decParentValidBetScore[CALVALUEFLAG_LESS]);
	//保存洗码货量
	VarDecAdd(&decTotalValidBetScore, &m_TotalValidBetScore, &m_TotalValidBetScore);
	VarDecAdd(&decValidBetScore[CALVALUEFLAG_HIGH], &m_ValidBetScore[CALVALUEFLAG_HIGH], &m_ValidBetScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decValidBetScore[CALVALUEFLAG_SELF], &m_ValidBetScore[CALVALUEFLAG_SELF], &m_ValidBetScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decValidBetScore[CALVALUEFLAG_LESS], &m_ValidBetScore[CALVALUEFLAG_LESS], &m_ValidBetScore[CALVALUEFLAG_LESS]);
	VarDecAdd(&decParentValidBetScore[CALVALUEFLAG_HIGH], &m_ParentValidBetScore[CALVALUEFLAG_HIGH], &m_ParentValidBetScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decParentValidBetScore[CALVALUEFLAG_SELF], &m_ParentValidBetScore[CALVALUEFLAG_SELF], &m_ParentValidBetScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decParentValidBetScore[CALVALUEFLAG_LESS], &m_ParentValidBetScore[CALVALUEFLAG_LESS], &m_ParentValidBetScore[CALVALUEFLAG_LESS]);


	return true;
}
//////////////////////////////////////////////////////////////////////////
//构造函数
CCalculateFrame::CCalculateFrame(void)
{
	m_pGameServiceAttrib = NULL;
	m_pGameServiceOption = NULL;
	m_pCompancyCalculateItem = NULL;
	m_pIGameServiceFrame = NULL;
	m_pIServerUserManager= NULL;
	m_pITableFrameManager = NULL;
}

//析构函数
CCalculateFrame::~CCalculateFrame(void)
{
	for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{
		SafeDelete(m_CalculateItemArray[i]);
	}
	m_CalculateItemArray.RemoveAll();
}

//接口查询
void * __cdecl CCalculateFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICalculateFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICalculateFrame,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CCalculateFrame::InitCalculateFrame(WORD wTableID,
												 ITableFrameManager*	pITableFrameManager,
												 tagTableFrameParameter * pTableFrameParameter)
{
	//效验参数
	GT_ASSERT(pTableFrameParameter!=NULL);
	GT_ASSERT(pITableFrameManager!=NULL);

	//查询接口
	m_pIGameServiceFrame=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIGameServiceFrame,IGameServiceFrame);
	GT_ASSERT(m_pIGameServiceFrame!=NULL);
	if (m_pIGameServiceFrame==NULL) return false;
	m_pIServerUserManager=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIServerUserManager,IServerUserManager);
	GT_ASSERT(m_pIServerUserManager!=NULL);
	if (m_pIServerUserManager==NULL) return false;

	//设置变量
	m_pITableFrameManager = pITableFrameManager;
	m_wTableID=wTableID;
	m_pGameServiceAttrib=pTableFrameParameter->pGameServiceAttrib;
	m_pGameServiceOption=pTableFrameParameter->pGameServiceOption;

	//数据库引擎
	IDataBaseEngine* pIDataBaseEngine=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIDataBaseEngine,IDataBaseEngine);
	if (pIDataBaseEngine==NULL)
	{
		TRACE(TEXT("获取数据库引擎服务失败"));
		return false;
	}
	//数据库触发
	IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
	if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
	{
		return false;
	}
	
	Reset();
	return true;
}
//框架接口

//枚举计算单元
ICalculateItem* __cdecl CCalculateFrame::EnumCalculateItem(WORD wEnumIndex)
{
	GT_ASSERT((wEnumIndex>=0)&&(wEnumIndex<m_CalculateItemArray.GetCount()));
	if(0 > wEnumIndex || wEnumIndex >= m_CalculateItemArray.GetCount())
		return NULL;
	return m_CalculateItemArray[wEnumIndex];
}

//获取计算单元数目
DWORD	__cdecl CCalculateFrame::GetCalculateItemCount()
{
	return m_CalculateItemArray.GetCount();
}

//获取总投注
bool __cdecl CCalculateFrame::GetFrameTotalBetScore(DECIMAL* pTotalBetScore)
{
	if(m_pCompancyCalculateItem == NULL)
		return false;
	return true;
}

//获取总派彩
bool __cdecl CCalculateFrame::GetFrameTotalWinScore(DECIMAL* pTotalWinScore)
{
	if(m_pCompancyCalculateItem == NULL)
		return false;
	
	return true;
}



//用户坐下
bool __cdecl CCalculateFrame::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(bLookonUser)
		return true;
	GT_ASSERT(pIServerUserItem);
	//效验参数
	if(pIServerUserItem == NULL)
		return false;

	if(pIServerUserItem->GetUserLevel() != USERLEVEL_GAMEUSER)
		return true;

	if(m_pITableFrameManager->IsGameStarted() == false)
		return true;

	return AddAndCheckUserCalculateItem(wChairID, pIServerUserItem) == NULL ? false : true;
}

//用户起来
bool __cdecl CCalculateFrame::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(bLookonUser)
		return true;
	GT_ASSERT(pIServerUserItem);
	//效验参数
	if(pIServerUserItem == NULL)
		return false;

	if(pIServerUserItem->GetUserLevel() != USERLEVEL_GAMEUSER)
		return true;

	return DelAndCheckUserCalculateItem(wChairID, pIServerUserItem);
}

//复位计算
bool __cdecl CCalculateFrame::Reset()
{	
#ifdef _DEBUG
	{
		for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
		{	
			if(m_CalculateItemArray[i] != NULL)
			{
				for(int j = 0; j < m_CalculateItemArray.GetCount(); j ++)
				{	
					if(i != j && m_CalculateItemArray[j] == m_CalculateItemArray[i] )
					{
						GT_ASSERT(FALSE);
					}
				}
			}
		}
	}
#endif
	for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{	
		if(m_CalculateItemArray[i] != NULL)
		{
			SafeDelete(m_CalculateItemArray[i]);
		}
	}
	m_CalculateItemArray.RemoveAll();
/*
	//定义变量
	DBR_GR_ResetBetScore ResetBetScore;
	ResetBetScore.wServerID = m_pGameServiceOption->wServerID;
	ResetBetScore.wTableID	= m_wTableID;

	//投递数据库
	m_DataBaseEvent.PostDataBaseEvent(DBR_GR_RESET_BETSCORE,0,
		0,&ResetBetScore,sizeof(ResetBetScore));*/

	return true;
}

//构建计算单元
bool __cdecl CCalculateFrame::StructCalculateItem()
{
	IServerUserItem* pIUserItem = NULL;

	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		pIUserItem = m_pITableFrameManager->GetServerUserItem(i);
		if (pIUserItem!=NULL &&
			pIUserItem->GetUserLevel() == USERLEVEL_GAMEUSER) 
		{
			CCalculateItem* pCalculateItem = AddAndCheckUserCalculateItem(i, pIUserItem);
			GT_ASSERT(pCalculateItem);
		}
	}
	return true;
}

//投注
bool __cdecl CCalculateFrame::BetScore(IServerUserItem * pIServerUserItem,
	tagBetScoreInfo *pBetScoreInfo, WORD wCount)
{
	//效验参数
	GT_ASSERT(pIServerUserItem != NULL);
	GT_ASSERT(pBetScoreInfo && wCount > 0);
	if(pIServerUserItem == NULL || pBetScoreInfo == NULL || wCount <= 0)
		return false;
	CCalculateItem *pItem = SearchCalculateItem(pIServerUserItem->GetUserID());
	if(pItem == NULL)
		return false;
	
	if(pItem->GetUserData()->UserData.cbLevel == USERLEVEL_GAMEUSER)
	{
		DECIMAL* decUserBetScore = NULL;
		_BEGIN_TRY
		{
			decUserBetScore = new DECIMAL[m_pGameServiceAttrib->wBetRegionCount];
			memset(decUserBetScore, 0, sizeof(DECIMAL) * m_pGameServiceAttrib->wBetRegionCount);
			Convert_BetScoreInfoToBetDecimal(pBetScoreInfo, wCount, decUserBetScore, m_pGameServiceAttrib->wBetRegionCount);

			if(pItem->BetScore(decUserBetScore, m_pGameServiceAttrib->wBetRegionCount))
			{
				SafeDeleteArray(decUserBetScore);
				return true;
			}
		}
		CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH
		SafeDeleteArray(decUserBetScore);

	}
	
	return false;
}

//检查投注
BYTE __cdecl CCalculateFrame::CheckBetScore(IServerUserItem * pIServerUserItem,
	tagBetScoreInfo *pBetScoreInfo, WORD wCount)
{
	//效验参数
	GT_ASSERT(pIServerUserItem != NULL);
	GT_ASSERT(pBetScoreInfo && wCount > 0);
	if(pIServerUserItem == NULL || pBetScoreInfo == NULL || wCount <= 0)
		return -1;
	if(pIServerUserItem->GetUserData()->UserData.cbLevel != USERLEVEL_GAMEUSER)
		return -1;
	CCalculateItem *pItem = SearchCalculateItem(pIServerUserItem->GetUserID());
	if(pItem == NULL)
		return -1;

	for(int i = 0; i < wCount; i ++)
	{
		GT_ASSERT(pBetScoreInfo[i].wBetRegionIndex < m_pGameServiceAttrib->wBetRegionCount);
		if(pBetScoreInfo[i].wBetRegionIndex >= m_pGameServiceAttrib->wBetRegionCount)
			return -1;
		if(m_pGameServiceOption->dwHighScore != 0)
		{
			if(pBetScoreInfo[i].dwBetScore > m_pGameServiceOption->dwHighScore)
				return -1;
		}
		if(m_pGameServiceOption->dwCellScore != 0)
		{
			if(pBetScoreInfo[i].dwBetScore > m_pGameServiceOption->dwCellScore)
				return -1;
		}
	}


	return 0;
}

//计算
bool __cdecl CCalculateFrame::CalculateCTP(void* pCalculateContext,
	ICalculateSink* pCalculateSink)
{
	//效验参数
	GT_ASSERT(pCalculateSink);
	if(pCalculateSink == NULL)
		return false;
	//计算派彩
	for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{
		if(m_CalculateItemArray[i] &&
			m_CalculateItemArray[i]->CalculateWinScore(pCalculateContext, pCalculateSink) == false)
		{
			return false;
		}
	}
	//计算结果
	for(i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{
		if(m_CalculateItemArray[i] &&
			m_CalculateItemArray[i]->CalculateResult(pCalculateContext, pCalculateSink) == false)
		{
			return false;
		}
	}
	return true;
}
//计算代理对会员方式
bool __cdecl CCalculateFrame::CalculatePTP(tagBetScoreInfo *pBetScoreInfo, WORD wCount)
{
	return false;
}

//辅助函数
//添加计算单元
CCalculateItem*  CCalculateFrame::AddAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//变量定义
	bool bNewFlag = false;
	CCalculateItem *pCalculateItem = NULL;
	pCalculateItem = SearchCalculateItem(pIServerUserItem->GetUserID());
	if(pCalculateItem != NULL)
	{
		pCalculateItem->Active(true);
	}
	else
	{
		pCalculateItem = new CCalculateItem();
		if(pCalculateItem->InitCalculateItem(pIServerUserItem,
			this,
			m_pGameServiceAttrib->wBetRegionCount,
			wChairID)==false)
		{
			SafeDelete(pCalculateItem);
			return NULL;
		}
		bNewFlag = true;
	}

	CCalculateItem* pSearchCalculateItem = pCalculateItem;
	
	//寻找上线
	while(pSearchCalculateItem->GetUserLevel()!=0)
	{
		if(pSearchCalculateItem->GetParent() == NULL)
		{
			CCalculateItem* pParent=SearchCalculateItem(pSearchCalculateItem->GetParentUserID());
			if(pParent == NULL)
			{
				IServerUserItem * pIServerUserItemParent = m_pIServerUserManager->SearchUser(pSearchCalculateItem->GetParentUserID());
				GT_ASSERT(pIServerUserItemParent);
				
				pParent = AddAndCheckUserCalculateItem(wChairID,pIServerUserItemParent);
				GT_ASSERT(pParent);
				if(pParent == NULL)
					return NULL;
			}
			
			pSearchCalculateItem->SetParent(pParent);
			pSearchCalculateItem = pSearchCalculateItem->GetParent();
			
		}
		else
		{
			pSearchCalculateItem = pSearchCalculateItem->GetParent();
		}
	}
	if(pCalculateItem->IsUserTreeFrameOK())
	{
		if(bNewFlag)
			AddUserCalculateItem(pCalculateItem);

		return pCalculateItem;
	}
	else
	{
		if(bNewFlag)
			SafeDelete(pCalculateItem);
		return NULL;
	}
}
//删除计算单元
bool  CCalculateFrame::DelAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	CCalculateItem *pCalculateItem = NULL;
	pCalculateItem = SearchCalculateItem(pIServerUserItem->GetUserID());
	if(pCalculateItem != NULL)
	{
		pCalculateItem->Active(false);
		return true;
	}
	else
		return false;
}
//增加计算单元
void		CCalculateFrame::AddUserCalculateItem(CCalculateItem* pItem)
{
	if(m_pCompancyCalculateItem == NULL)
	{
		if(pItem && pItem->GetUserLevel() == 0)
		{
			m_pCompancyCalculateItem = pItem;
		}
	}
	m_CalculateItemArray.Add(pItem);
}
//增加计算单元
CCalculateItem* CCalculateFrame::SearchCalculateItem(DWORD dwUserID)
{
	for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{
		if(m_CalculateItemArray[i] &&
			m_CalculateItemArray[i]->GetUserID() == dwUserID)
		{
			return m_CalculateItemArray[i];
		}
	}
	return NULL;
}
//结算回调
void	CCalculateFrame::BetScoreSink(ICalculateItem * pICalculateItem, DECIMAL* pdecTotalBetScore)
{
	//定义变量
	DBR_GR_InsertBetScore InsertBetScore;
	InsertBetScore.dwUserID = pICalculateItem->GetUserData()->UserData.dwUserID;
	InsertBetScore.wServerID = m_pGameServiceOption->wServerID;
	InsertBetScore.wTableID	= m_wTableID;
	memcpy(&InsertBetScore.decTotalBetScore, pdecTotalBetScore, sizeof(DECIMAL));

	//投递数据库
	m_DataBaseEvent.PostDataBaseEvent(DBR_GR_INSERT_BETSCORE,0,
		0,&InsertBetScore,sizeof(InsertBetScore));
}