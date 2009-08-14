#include "stdafx.h"
#include ".\calculateframe.h"
#include "DataBaseSink.h"
//Ͷע��Ϣת������
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
//���������ۼ�
inline void DecimalArrayAdd(DECIMAL* pdecLeftArr, DECIMAL* pdecRightArr, DECIMAL* pdecResultArr,WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecAdd(&pdecLeftArr[i], &pdecRightArr[i], &pdecResultArr[i]);
	}
}
//���������ۼ�
inline void DecimalArraySub(DECIMAL* pdecLeftArr, DECIMAL* pdecRightArr, DECIMAL* pdecResultArr,WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecSub(&pdecLeftArr[i], &pdecRightArr[i], &pdecResultArr[i]);
	}
}
//���������۳�
inline void DecimalArrayMulSingleDec(DECIMAL* pdecLeftArr, DECIMAL* pdecRight, DECIMAL* pdecResultArr,WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecMul(&pdecLeftArr[i], pdecRight, &pdecResultArr[i]);
	}
}
//���������
inline void DecimalArraySum(DECIMAL* pdecDecimalArr, DECIMAL* pdecResult, WORD wDecimalCount)
{
	for(int i = 0; i < wDecimalCount; i ++)
	{
		VarDecAdd(&pdecDecimalArr[i], pdecResult, pdecResult);
	}
}

//���캯��
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

//��������
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

//�ӿڲ�ѯ
void * __cdecl CCalculateItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICalculateItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICalculateItem,Guid,dwQueryVer);
	return NULL;
}

//��ܽӿ�
//��������
void __cdecl  CCalculateItem::AddUserRefCount()
{
	m_lUserRefCount++;
}

//��������
void __cdecl  CCalculateItem::ReleaseUserRefCount()
{
	m_lUserRefCount--;
}

//�ж�����
bool __cdecl  CCalculateItem::IsUserRefCountZero()
{
	return m_lUserRefCount <= 0;
}

//�ж��û���ϵ��ȷ
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

//��ȡ��Ͷע
bool __cdecl CCalculateItem::GetTotalBetScore(DECIMAL* pTotalBetScore)
{
	memcpy(pTotalBetScore, &m_TotalBetScore, sizeof(DECIMAL));
	return true;
}

//��ȡ���ɲ�
bool __cdecl CCalculateItem::GetTotalWinScore(DECIMAL* pTotalWinScore)
{
	memcpy(pTotalWinScore, &m_TotalWinScore, sizeof(DECIMAL));
	return true;
}

//��ȡ�ܳ�ˮ
bool __cdecl CCalculateItem::GetTotalTaxScore(DECIMAL* pTotalTaxScore)
{
	memcpy(pTotalTaxScore, &m_TotalTaxScore, sizeof(DECIMAL));
	return true;
}

//��ȡ�ɲ�ռ��
bool __cdecl CCalculateItem::GetWinScoreOccupancy(DECIMAL* pHighScore,
	DECIMAL* pSelfScore,DECIMAL *pLessScore)
{
	memcpy(pHighScore, &m_ParentWinScore[CALVALUEFLAG_HIGH], sizeof(DECIMAL));
	memcpy(pSelfScore, &m_ParentWinScore[CALVALUEFLAG_SELF], sizeof(DECIMAL));
	VarDecSub(&m_TotalWinScore,&m_ParentWinScore[CALVALUEFLAG_LESS],pLessScore); 
	return true;
}

//��ȡ�ɲ�ռ��
bool __cdecl CCalculateItem::GetTaxScoreOccupancy(DECIMAL* pHighScore,
												  DECIMAL* pSelfScore,DECIMAL *pLessScore)
{
	memcpy(pHighScore, &m_ParentTaxScore[CALVALUEFLAG_HIGH], sizeof(DECIMAL));
	memcpy(pSelfScore, &m_ParentTaxScore[CALVALUEFLAG_SELF], sizeof(DECIMAL));
	VarDecSub(&m_TotalTaxScore,&m_ParentTaxScore[CALVALUEFLAG_LESS],pLessScore); 
	return true;
}

//��ȡϴ���뽻�ն��
bool __cdecl CCalculateItem::GetValidBetScoreAndPaidScore(DECIMAL* pTotalScore,
	DECIMAL* pHighScore, DECIMAL *pHighRollbackScore,
	DECIMAL* pLessScore, DECIMAL *pLessRollbackScore,
	DECIMAL* pHighPaidScore, DECIMAL* pLessPaidScore)
{
	DECIMAL decTemp;
	memset(&decTemp, 0, sizeof(DECIMAL));
	//��ȡϴ��
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
	//��ȡ���ն��
	VarDecAdd(&m_ParentWinScore[CALVALUEFLAG_HIGH], pHighRollbackScore, pHighPaidScore);
	VarDecFromR8(-1.0,&decTemp);
	VarDecMul(&m_ParentWinScore[CALVALUEFLAG_LESS], &decTemp, pLessPaidScore);
	VarDecAdd(pLessPaidScore, pLessRollbackScore, pLessPaidScore);

	return true;
}

//��ȡͶע��ϸ��Ϣ
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

//��λ����
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

//��ʼ��
bool __cdecl CCalculateItem::InitCalculateItem(IServerUserItem*	pIServerUserItem,
							   CCalculateFrame* pCalculateFrame,
							   WORD wBetRegionCount,
							   WORD wChairID)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem && pCalculateFrame && wBetRegionCount >= 1);
	if(!pIServerUserItem || !pCalculateFrame || wBetRegionCount < 1)
		return false;
	//���ñ���
	memcpy(&m_ServerUserData, pIServerUserItem->GetUserData(), sizeof(tagServerUserData));
	m_pCalculateFrame  = pCalculateFrame;
	m_wBetRegionCount  = wBetRegionCount;
	m_wChairID		   = wChairID;

	//�����ڴ�
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

	//��ʼ��
	Reset();

	return true;
}

//Ͷע
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

//Ͷע
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

//���������Ͷע
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

//������ߵ���Ͷע
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

//�ɲ�
bool __cdecl CCalculateItem::WinScore(DECIMAL *pdecWinScore,
									  DECIMAL* pdecUserTaxScore,
									  WORD wCount)
{
	GT_ASSERT(m_wBetRegionCount == wCount);

	//������Ϣ
	DECIMAL* decUserWinScore = new DECIMAL[m_wBetRegionCount];					//�����ɲ�
	memcpy(decUserWinScore, pdecWinScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decLessWinScore = new DECIMAL[m_wBetRegionCount];					//�����ɲ�
	memcpy(decLessWinScore, decUserWinScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decUserTaxScore = new DECIMAL[m_wBetRegionCount];					//�����ˮ
	memcpy(decUserTaxScore, pdecUserTaxScore, sizeof(DECIMAL) * m_wBetRegionCount);

	DECIMAL* decLessTaxScore = new DECIMAL[m_wBetRegionCount];					//���߳�ˮ
	memcpy(decLessTaxScore, decUserTaxScore, sizeof(DECIMAL) * m_wBetRegionCount);

	//����������
	CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1];
	memset(pLessCalculateItemChain, 0 , sizeof(CCalculateItem*) * (USERLEVEL_GAMEUSER+1));
	pLessCalculateItemChain[this->GetUserData()->UserData.cbLevel] = this;

	//��������
	if(GetParent()->WinScore(decUserWinScore, 
		decLessWinScore, 
		decUserTaxScore, 
		decLessTaxScore,
		pLessCalculateItemChain, this))
	{
		//�жϻ�Ա,�����ɲ�
		if(this->GetUserData()->UserData.cbLevel == USERLEVEL_GAMEUSER)
		{
			//�ۼ��ɲ�
			DecimalArrayAdd(decUserWinScore, m_TotalPartedWinScore, m_TotalPartedWinScore, m_wBetRegionCount);
			DecimalArrayAdd(decUserWinScore, m_PartedWinScore[CALVALUEFLAG_SELF], m_PartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
			
			//�ۼӳ�ˮ
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

//�ɲ�
bool __cdecl  CCalculateItem::WinScore(DECIMAL* pdecUserWinScore,
									   DECIMAL* pdecLessWinScore,
									   DECIMAL* pdecUserTaxScore,
									   DECIMAL *pdecLessTaxScore, 
									   CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
									   CCalculateItem* pLessCalculateItem)
{
	//�����Լ����ͽڵ�
	pLessCalculateItemChain[this->GetUserData()->UserData.cbLevel]	= this;

	//////////////////////////////////////////////////////////////////////////
	//�����ˮ

	//������㻺������
	DECIMAL* decHighTaxScore = new DECIMAL[m_wBetRegionCount];				//����
	memset(decHighTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decSelfTaxScore = new DECIMAL[m_wBetRegionCount];				//����
	memset(decSelfTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decLessTaxScore = new DECIMAL[m_wBetRegionCount];				//����
	memcpy(decLessTaxScore, pdecLessTaxScore, sizeof(DECIMAL) * m_wBetRegionCount);//�������߳�ˮ��Ϣ

	//�������߳�ˮռ�ɱ�
	DECIMAL decLessTaxOccupancyRate;
	memset(&decLessTaxOccupancyRate, 0, sizeof(DECIMAL));

	//Ѱ�����߳�ˮռ�ɱ�
	for(int i = this->GetUserData()->UserData.cbLevel + 1; i < USERLEVEL_GAMEUSER; i ++)
	{
		if(pLessCalculateItemChain[i])
		{
			memcpy(&decLessTaxOccupancyRate, &pLessCalculateItemChain[i]->GetUserData()->decTaxOccupancyRate,
				sizeof(DECIMAL));
			break;
		}
	}

	//��ȡ�����ˮռ�ɱ�
	DECIMAL decMyTaxOccupancyRate;
	memcpy(&decMyTaxOccupancyRate, &this->GetUserData()->decTaxOccupancyRate, sizeof(DECIMAL));

	//�ж����߲��ܴ��������ˮռ�ɱ�
	GT_ASSERT(VarDecCmp(&decLessTaxOccupancyRate, &decMyTaxOccupancyRate) == VARCMP_EQ ||
		VarDecCmp(&decLessTaxOccupancyRate, &decMyTaxOccupancyRate) == VARCMP_LT);
	if(VarDecCmp(&decLessTaxOccupancyRate, &decMyTaxOccupancyRate) == VARCMP_GT)
		decLessTaxOccupancyRate = decMyTaxOccupancyRate;

	//����ʵ�ʳ�ˮռ����=�����ˮռ�ɱ�-���߳�ˮռ�ɱ�
	DECIMAL decMarginTaxOccupancyRate;//����ʵ�ʳ�ˮռ����
	VarDecSub(&decMyTaxOccupancyRate,
		&decLessTaxOccupancyRate, &decMarginTaxOccupancyRate);

	//�����ˮ=����ʵ�ʳ�ˮռ����*�ܳ�ˮ
	DecimalArrayMulSingleDec(pdecUserTaxScore, &decMarginTaxOccupancyRate, decSelfTaxScore, m_wBetRegionCount);

	//���߳�ˮ=���߳�ˮ-�����ˮ
	if(this->GetUserData()->UserData.cbLevel != 0)
	{
		DecimalArraySub(decLessTaxScore, decSelfTaxScore, decHighTaxScore, m_wBetRegionCount);
	}

	//�ۼӵ�ǰ�����ˮ��Ϣ
	AddPartedTaxScore(pdecUserTaxScore,decHighTaxScore,decSelfTaxScore,decLessTaxScore);

	//֪ͨ�����ۼӵ�ǰ�����ˮ��Ϣ
	pLessCalculateItem->AddParentPartedTaxScore(pdecUserTaxScore,decHighTaxScore,decSelfTaxScore,decLessTaxScore);


	//////////////////////////////////////////////////////////////////////////
	//�����ɲ�

	//������㻺������
	DECIMAL* decHighWinScore = new DECIMAL[m_wBetRegionCount];				//����
	memset(decHighWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decSelfWinScore = new DECIMAL[m_wBetRegionCount];				//����
	memset(decSelfWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decLessWinScore = new DECIMAL[m_wBetRegionCount];				//����
	memcpy(decLessWinScore, pdecLessWinScore, sizeof(DECIMAL) * m_wBetRegionCount);//���������ɲ���Ϣ
	
	//�жϲ�ռ��
	if(this->GetUserData()->UserData.cbOccupancyRate_NoFlag)
	{

	}
	else
	{
		//��������ռ�ɱ�
		DECIMAL decLessOccupancyRate;
		memset(&decLessOccupancyRate, 0, sizeof(DECIMAL));

		//Ѱ������ռ�ɱ�,�������Ϊ��ռ��,��������Ѱ��
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

		//��ȡ����ռ�ɱ�
		DECIMAL decMyOccupancyRate;
		memcpy(&decMyOccupancyRate, &this->GetUserData()->decOccupancyRate, sizeof(DECIMAL));

		//�ж����߲��ܴ�������ռ�ɱ�
		GT_ASSERT(VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_EQ ||
			VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_LT);
		if(VarDecCmp(&decLessOccupancyRate, &decMyOccupancyRate) == VARCMP_GT)
			decLessOccupancyRate = decMyOccupancyRate;

		//����ʵ��ռ����=����ռ�ɱ�-����ռ�ɱ�
		DECIMAL decMarginOccupancyRate;//����ʵ��ռ����
		VarDecSub(&decMyOccupancyRate,
			&decLessOccupancyRate, &decMarginOccupancyRate);

		//�����ɲ�=����ʵ��ռ����*���ɲ�
		DecimalArrayMulSingleDec(pdecUserWinScore, &decMarginOccupancyRate, decSelfWinScore, m_wBetRegionCount);

	}

	//�����ɲ�=�����ɲ�-�����ɲ�
	if(this->GetUserData()->UserData.cbLevel != 0)
	{
		DecimalArraySub(decLessWinScore, decSelfWinScore, decHighWinScore, m_wBetRegionCount);
	}

	//�ۼӵ�ǰ�����ɲ���Ϣ
	AddPartedWinScore(pdecUserWinScore,decHighWinScore,decSelfWinScore,decLessWinScore);

	//֪ͨ�����ۼӵ�ǰ�����ɲ���Ϣ
	pLessCalculateItem->AddParentPartedWinScore(pdecUserWinScore,decHighWinScore,decSelfWinScore,decLessWinScore);

	//�жϼ������,��ǰΪ��˾�ʺŽ���
	bool bResult = false;
	if(this->GetUserData()->UserData.cbLevel == 0)
		bResult = true;
	else
	{
		//��������
		bResult = GetParent()->WinScore(pdecUserWinScore, 
			decHighWinScore, 
			pdecUserTaxScore,
			decHighTaxScore,
			pLessCalculateItemChain, 
			this);
	}
	
	//�ͷ��ڴ�
	SafeDeleteArray(decHighTaxScore);
	SafeDeleteArray(decSelfTaxScore);
	SafeDeleteArray(decLessTaxScore);

	SafeDeleteArray(decHighWinScore);
	SafeDeleteArray(decSelfWinScore);
	SafeDeleteArray(decLessWinScore);

	return bResult;
}

//����������ɲ�
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

//������ߵ����ɲ�
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

//����������ˮ
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

//������ߵ����ˮ
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

//�����ɲ�
bool __cdecl CCalculateItem::CalculateWinScore(void* pCalculateContext,
											 ICalculateSink* pCalculateSink)
{

	//��Ա��Ҽ���
	if(this->GetUserData()->UserData.cbLevel != USERLEVEL_GAMEUSER)
		return true;

	//�ж���ͶעΪ0
	if(VarDecCmpR8(&m_TotalBetScore, 0) == VARCMP_EQ)
		return true;

	GT_ASSERT(pCalculateSink);
	GT_ASSERT(m_wChairID != INVALID_CHAIR);

	DECIMAL* decUserWinScore = new DECIMAL[m_wBetRegionCount];				//�ɲ�
	memset(decUserWinScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);
	DECIMAL* decUserTaxScore = new DECIMAL[m_wBetRegionCount];				//��ˮ
	memset(decUserTaxScore, 0, sizeof(DECIMAL) * m_wBetRegionCount);

	//�����ɲ�
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

//�����ܽ��
bool __cdecl CCalculateItem::CalculateResult(void* pCalculateContext,
					 ICalculateSink* pCalculateSink)
{
	GT_ASSERT(pCalculateSink);

	if(VarDecCmpR8(&m_TotalBetScore, 0) == VARCMP_EQ)
		return true;

	//////////////////////////////////////////////////////////////////////////
	//ʵ���ɲ� = �ɲ� + ��ˮ����

	DecimalArrayAdd(m_TotalPartedWinScore,m_TotalPartedTaxScore,m_TotalPartedWinScore, m_wBetRegionCount);
	DecimalArrayAdd(m_PartedWinScore[CALVALUEFLAG_HIGH],m_PartedTaxScore[CALVALUEFLAG_HIGH],m_PartedWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(m_PartedWinScore[CALVALUEFLAG_SELF],m_PartedTaxScore[CALVALUEFLAG_SELF],m_PartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(m_PartedWinScore[CALVALUEFLAG_LESS],m_PartedTaxScore[CALVALUEFLAG_LESS],m_PartedWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	DecimalArrayAdd(m_ParentPartedWinScore[CALVALUEFLAG_HIGH],m_ParentPartedTaxScore[CALVALUEFLAG_HIGH],m_ParentPartedWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArrayAdd(m_ParentPartedWinScore[CALVALUEFLAG_SELF],m_ParentPartedTaxScore[CALVALUEFLAG_SELF],m_ParentPartedWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArrayAdd(m_ParentPartedWinScore[CALVALUEFLAG_LESS],m_ParentPartedTaxScore[CALVALUEFLAG_LESS],m_ParentPartedWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	//////////////////////////////////////////////////////////////////////////
	//�����ɲ�

	DECIMAL		decTotalWinScore;				//���ɲ�
	DECIMAL		decWinScore[CALVALUEFLAG_COUNT]; //�����ɲ���Ϣ
	DECIMAL		decParentWinScore[CALVALUEFLAG_COUNT]; //�����ɲ���Ϣ
	memset(&decTotalWinScore, 0, sizeof(DECIMAL));
	memset(decWinScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(decParentWinScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	//����ɲ�
	DecimalArraySum(m_TotalPartedWinScore, &decTotalWinScore, m_wBetRegionCount);
	DecimalArraySum(m_PartedWinScore[CALVALUEFLAG_HIGH], &decWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_PartedWinScore[CALVALUEFLAG_SELF], &decWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_PartedWinScore[CALVALUEFLAG_LESS], &decWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedWinScore[CALVALUEFLAG_HIGH], &decParentWinScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedWinScore[CALVALUEFLAG_SELF], &decParentWinScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedWinScore[CALVALUEFLAG_LESS], &decParentWinScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	//�����ɲʽ��
	VarDecAdd(&decTotalWinScore, &m_TotalWinScore, &m_TotalWinScore);
	VarDecAdd(&decWinScore[CALVALUEFLAG_HIGH], &m_WinScore[CALVALUEFLAG_HIGH], &m_WinScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decWinScore[CALVALUEFLAG_SELF], &m_WinScore[CALVALUEFLAG_SELF], &m_WinScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decWinScore[CALVALUEFLAG_LESS], &m_WinScore[CALVALUEFLAG_LESS], &m_WinScore[CALVALUEFLAG_LESS]);
	VarDecAdd(&decParentWinScore[CALVALUEFLAG_HIGH], &m_ParentWinScore[CALVALUEFLAG_HIGH], &m_ParentWinScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decParentWinScore[CALVALUEFLAG_SELF], &m_ParentWinScore[CALVALUEFLAG_SELF], &m_ParentWinScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decParentWinScore[CALVALUEFLAG_LESS], &m_ParentWinScore[CALVALUEFLAG_LESS], &m_ParentWinScore[CALVALUEFLAG_LESS]);

	//////////////////////////////////////////////////////////////////////////
	//�����ˮ

	DECIMAL		decTotalTaxScore;				//�ܳ�ˮ
	DECIMAL		decTaxScore[CALVALUEFLAG_COUNT]; //�����ˮ��Ϣ
	DECIMAL		decParentTaxScore[CALVALUEFLAG_COUNT]; //���߳�ˮ��Ϣ
	memset(&decTotalTaxScore, 0, sizeof(DECIMAL));
	memset(decTaxScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(decParentTaxScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	//��ӳ�ˮ
	DecimalArraySum(m_TotalPartedTaxScore, &decTotalTaxScore, m_wBetRegionCount);
	DecimalArraySum(m_PartedTaxScore[CALVALUEFLAG_HIGH], &decTaxScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_PartedTaxScore[CALVALUEFLAG_SELF], &decTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_PartedTaxScore[CALVALUEFLAG_LESS], &decTaxScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedTaxScore[CALVALUEFLAG_HIGH], &decParentTaxScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedTaxScore[CALVALUEFLAG_SELF], &decParentTaxScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedTaxScore[CALVALUEFLAG_LESS], &decParentTaxScore[CALVALUEFLAG_LESS], m_wBetRegionCount);

	//�����ˮ���
	VarDecAdd(&decTotalTaxScore, &m_TotalTaxScore, &m_TotalTaxScore);
	VarDecAdd(&decTaxScore[CALVALUEFLAG_HIGH], &m_TaxScore[CALVALUEFLAG_HIGH], &m_TaxScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decTaxScore[CALVALUEFLAG_SELF], &m_TaxScore[CALVALUEFLAG_SELF], &m_TaxScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decTaxScore[CALVALUEFLAG_LESS], &m_TaxScore[CALVALUEFLAG_LESS], &m_TaxScore[CALVALUEFLAG_LESS]);
	VarDecAdd(&decParentTaxScore[CALVALUEFLAG_HIGH], &m_ParentTaxScore[CALVALUEFLAG_HIGH], &m_ParentTaxScore[CALVALUEFLAG_HIGH]);
	VarDecAdd(&decParentTaxScore[CALVALUEFLAG_SELF], &m_ParentTaxScore[CALVALUEFLAG_SELF], &m_ParentTaxScore[CALVALUEFLAG_SELF]);
	VarDecAdd(&decParentTaxScore[CALVALUEFLAG_LESS], &m_ParentTaxScore[CALVALUEFLAG_LESS], &m_ParentTaxScore[CALVALUEFLAG_LESS]);

	//////////////////////////////////////////////////////////////////////////
	//����ϴ��

	DECIMAL		decTotalValidBetScore;			//��ϴ��
	DECIMAL		decValidBetScore[CALVALUEFLAG_COUNT]; //����ϴ����Ϣ
	DECIMAL		decParentValidBetScore[CALVALUEFLAG_COUNT]; //����ϴ����Ϣ
	memset(&decTotalValidBetScore, 0, sizeof(DECIMAL));
	memset(decValidBetScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);
	memset(decParentValidBetScore, 0, sizeof(DECIMAL) * CALVALUEFLAG_COUNT);

	//���ϴ�����
	DecimalArraySum(m_TotalPartedBetScore, &decTotalValidBetScore, m_wBetRegionCount);
	DecimalArraySum(m_PartedBetScore[CALVALUEFLAG_HIGH], &decValidBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_PartedBetScore[CALVALUEFLAG_SELF], &decValidBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_PartedBetScore[CALVALUEFLAG_LESS], &decValidBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedBetScore[CALVALUEFLAG_HIGH], &decParentValidBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedBetScore[CALVALUEFLAG_SELF], &decParentValidBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount);
	DecimalArraySum(m_ParentPartedBetScore[CALVALUEFLAG_LESS], &decParentValidBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount);
	//����ϴ�����
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_TotalPartedBetScore, m_wBetRegionCount, &decTotalValidBetScore);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_PartedBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount, &decValidBetScore[CALVALUEFLAG_HIGH]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_PartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount, &decValidBetScore[CALVALUEFLAG_SELF]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_PartedBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount, &decValidBetScore[CALVALUEFLAG_LESS]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_ParentPartedBetScore[CALVALUEFLAG_HIGH], m_wBetRegionCount, &decParentValidBetScore[CALVALUEFLAG_HIGH]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_ParentPartedBetScore[CALVALUEFLAG_SELF], m_wBetRegionCount, &decParentValidBetScore[CALVALUEFLAG_SELF]);
	pCalculateSink->RectifyValidBetScore(pCalculateContext, this, m_ParentPartedBetScore[CALVALUEFLAG_LESS], m_wBetRegionCount, &decParentValidBetScore[CALVALUEFLAG_LESS]);
	//����ϴ�����
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
//���캯��
CCalculateFrame::CCalculateFrame(void)
{
	m_pGameServiceAttrib = NULL;
	m_pGameServiceOption = NULL;
	m_pCompancyCalculateItem = NULL;
	m_pIGameServiceFrame = NULL;
	m_pIServerUserManager= NULL;
	m_pITableFrameManager = NULL;
}

//��������
CCalculateFrame::~CCalculateFrame(void)
{
	for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{
		SafeDelete(m_CalculateItemArray[i]);
	}
	m_CalculateItemArray.RemoveAll();
}

//�ӿڲ�ѯ
void * __cdecl CCalculateFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICalculateFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICalculateFrame,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CCalculateFrame::InitCalculateFrame(WORD wTableID,
												 ITableFrameManager*	pITableFrameManager,
												 tagTableFrameParameter * pTableFrameParameter)
{
	//Ч�����
	GT_ASSERT(pTableFrameParameter!=NULL);
	GT_ASSERT(pITableFrameManager!=NULL);

	//��ѯ�ӿ�
	m_pIGameServiceFrame=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIGameServiceFrame,IGameServiceFrame);
	GT_ASSERT(m_pIGameServiceFrame!=NULL);
	if (m_pIGameServiceFrame==NULL) return false;
	m_pIServerUserManager=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIServerUserManager,IServerUserManager);
	GT_ASSERT(m_pIServerUserManager!=NULL);
	if (m_pIServerUserManager==NULL) return false;

	//���ñ���
	m_pITableFrameManager = pITableFrameManager;
	m_wTableID=wTableID;
	m_pGameServiceAttrib=pTableFrameParameter->pGameServiceAttrib;
	m_pGameServiceOption=pTableFrameParameter->pGameServiceOption;

	//���ݿ�����
	IDataBaseEngine* pIDataBaseEngine=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIDataBaseEngine,IDataBaseEngine);
	if (pIDataBaseEngine==NULL)
	{
		TRACE(TEXT("��ȡ���ݿ��������ʧ��"));
		return false;
	}
	//���ݿⴥ��
	IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
	if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
	{
		return false;
	}
	
	Reset();
	return true;
}
//��ܽӿ�

//ö�ټ��㵥Ԫ
ICalculateItem* __cdecl CCalculateFrame::EnumCalculateItem(WORD wEnumIndex)
{
	GT_ASSERT((wEnumIndex>=0)&&(wEnumIndex<m_CalculateItemArray.GetCount()));
	if(0 > wEnumIndex || wEnumIndex >= m_CalculateItemArray.GetCount())
		return NULL;
	return m_CalculateItemArray[wEnumIndex];
}

//��ȡ���㵥Ԫ��Ŀ
DWORD	__cdecl CCalculateFrame::GetCalculateItemCount()
{
	return m_CalculateItemArray.GetCount();
}

//��ȡ��Ͷע
bool __cdecl CCalculateFrame::GetFrameTotalBetScore(DECIMAL* pTotalBetScore)
{
	if(m_pCompancyCalculateItem == NULL)
		return false;
	return true;
}

//��ȡ���ɲ�
bool __cdecl CCalculateFrame::GetFrameTotalWinScore(DECIMAL* pTotalWinScore)
{
	if(m_pCompancyCalculateItem == NULL)
		return false;
	
	return true;
}



//�û�����
bool __cdecl CCalculateFrame::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(bLookonUser)
		return true;
	GT_ASSERT(pIServerUserItem);
	//Ч�����
	if(pIServerUserItem == NULL)
		return false;

	if(pIServerUserItem->GetUserLevel() != USERLEVEL_GAMEUSER)
		return true;

	if(m_pITableFrameManager->IsGameStarted() == false)
		return true;

	return AddAndCheckUserCalculateItem(wChairID, pIServerUserItem) == NULL ? false : true;
}

//�û�����
bool __cdecl CCalculateFrame::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(bLookonUser)
		return true;
	GT_ASSERT(pIServerUserItem);
	//Ч�����
	if(pIServerUserItem == NULL)
		return false;

	if(pIServerUserItem->GetUserLevel() != USERLEVEL_GAMEUSER)
		return true;

	return DelAndCheckUserCalculateItem(wChairID, pIServerUserItem);
}

//��λ����
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
	//�������
	DBR_GR_ResetBetScore ResetBetScore;
	ResetBetScore.wServerID = m_pGameServiceOption->wServerID;
	ResetBetScore.wTableID	= m_wTableID;

	//Ͷ�����ݿ�
	m_DataBaseEvent.PostDataBaseEvent(DBR_GR_RESET_BETSCORE,0,
		0,&ResetBetScore,sizeof(ResetBetScore));*/

	return true;
}

//�������㵥Ԫ
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

//Ͷע
bool __cdecl CCalculateFrame::BetScore(IServerUserItem * pIServerUserItem,
	tagBetScoreInfo *pBetScoreInfo, WORD wCount)
{
	//Ч�����
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

//���Ͷע
BYTE __cdecl CCalculateFrame::CheckBetScore(IServerUserItem * pIServerUserItem,
	tagBetScoreInfo *pBetScoreInfo, WORD wCount)
{
	//Ч�����
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

//����
bool __cdecl CCalculateFrame::CalculateCTP(void* pCalculateContext,
	ICalculateSink* pCalculateSink)
{
	//Ч�����
	GT_ASSERT(pCalculateSink);
	if(pCalculateSink == NULL)
		return false;
	//�����ɲ�
	for(int i = 0; i < m_CalculateItemArray.GetCount(); i ++)
	{
		if(m_CalculateItemArray[i] &&
			m_CalculateItemArray[i]->CalculateWinScore(pCalculateContext, pCalculateSink) == false)
		{
			return false;
		}
	}
	//������
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
//�������Ի�Ա��ʽ
bool __cdecl CCalculateFrame::CalculatePTP(tagBetScoreInfo *pBetScoreInfo, WORD wCount)
{
	return false;
}

//��������
//��Ӽ��㵥Ԫ
CCalculateItem*  CCalculateFrame::AddAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//��������
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
	
	//Ѱ������
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
//ɾ�����㵥Ԫ
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
//���Ӽ��㵥Ԫ
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
//���Ӽ��㵥Ԫ
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
//����ص�
void	CCalculateFrame::BetScoreSink(ICalculateItem * pICalculateItem, DECIMAL* pdecTotalBetScore)
{
	//�������
	DBR_GR_InsertBetScore InsertBetScore;
	InsertBetScore.dwUserID = pICalculateItem->GetUserData()->UserData.dwUserID;
	InsertBetScore.wServerID = m_pGameServiceOption->wServerID;
	InsertBetScore.wTableID	= m_wTableID;
	memcpy(&InsertBetScore.decTotalBetScore, pdecTotalBetScore, sizeof(DECIMAL));

	//Ͷ�����ݿ�
	m_DataBaseEvent.PostDataBaseEvent(DBR_GR_INSERT_BETSCORE,0,
		0,&InsertBetScore,sizeof(InsertBetScore));
}