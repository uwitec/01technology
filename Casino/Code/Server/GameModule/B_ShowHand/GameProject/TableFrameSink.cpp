#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//宏定义
#define IDI_TIME_GIVE_UP						1							//放弃定时器

//静态变量
const BYTE	CGameLogic::m_bCardArray[28]=									//扑克数组
{
	 1, 8, 9,10,11,12,13,
	14,21,22,23,24,25,26,
	27,34,35,36,37,38,39,
	40,47,48,49,50,51,52,
};

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//开始模式

//////////////////////////////////////////////////////////////////////////

//获取扑克花色
BYTE CGameLogic::GetHuaKind(BYTE bCard)
{
	return (bCard-1)/13;
}

//获取扑克数值
BYTE CGameLogic::GetCardValoe(BYTE bCard)
{
	BYTE bVolea=(bCard-1)%13;
	if (bVolea==0) bVolea=13;
	return bVolea;
}

//获取扑克类型
BYTE CGameLogic::GetCardKind(BYTE bCardList[], BYTE bCardCount)
{
	if (IsTongHuaShun(bCardList,bCardCount)) return TONG_HUA_SHUN;
	if (IsTieZhi(bCardList,bCardCount)) return TIE_ZHI;
	if (IsHuLu(bCardList,bCardCount)) return HU_LU;
	if (IsTongHua(bCardList,bCardCount)) return TONG_HUA;
	if (IsShunZhi(bCardList,bCardCount)) return SHUN_ZI;
	if (IsSanTiao(bCardList,bCardCount)) return SAN_TIAO;
	if (IsLiangDui(bCardList,bCardCount)) return LAING_DUI;
	if (IsDuiZhi(bCardList,bCardCount)) return DUI_ZI;
	return SHAN;
}

//查找对牌
BYTE CGameLogic::FindDoubleCard(BYTE bCardList[], BYTE bCardCount, BYTE * bOutDoubleCard)
{
	BYTE bDoubleCount=0;
	if (bCardCount<2) return 0;
	for (BYTE i=0;i<bCardCount-1;i++)
	{
		if (GetCardValoe(bCardList[i])==GetCardValoe(bCardList[i+1]))
		{
			*(bOutDoubleCard+bDoubleCount*2)=bCardList[i];
			*(bOutDoubleCard+bDoubleCount*2+1)=bCardList[i+1];
			bDoubleCount++;
			i++;
		}
	}
	return bDoubleCount;
}

//是否同花顺
bool CGameLogic::IsTongHuaShun(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	BYTE bFirstCardVoloe=GetCardValoe(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
		if (bFirstCardVoloe!=(GetCardValoe(bCardList[i])+i)) return false;
	}
	return true;
}

//是否铁支
bool CGameLogic::IsTieZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<4) return false;
	
	BYTE bBaseVoloe=GetCardValoe(bCardList[1]),bNoFixCount=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardValoe(bCardList[i])!=bBaseVoloe) bNoFixCount++;
		if ((bCardCount-bNoFixCount)<4) return false;
	}
	return true;
}

//是否葫芦
bool CGameLogic::IsHuLu(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;

	BYTE bFirstVoloe=GetCardValoe(bCardList[0]);
	BYTE bMidVoloe=GetCardValoe(bCardList[2]);
	BYTE bLastVoloe=GetCardValoe(bCardList[4]);
	if (GetCardValoe(bCardList[1])!=bFirstVoloe) return false;
	if (GetCardValoe(bCardList[3])!=bLastVoloe) return false;
	if ((bMidVoloe!=bFirstVoloe)&&(bMidVoloe!=bLastVoloe)) return false;
	
	return true;
}

//是否同花
bool CGameLogic::IsTongHua(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
	}
	return true;
}

//是否顺子
bool CGameLogic::IsShunZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	BYTE bFirstCardVoloe=GetCardValoe(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (bFirstCardVoloe!=(GetCardValoe(bCardList[i])+i)) return false;
	}
	return true;
}

//是否三条
bool CGameLogic::IsSanTiao(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<3) return false;
	
	BYTE bBaseVoloe=GetCardValoe(bCardList[2]),bBaseCount=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardValoe(bCardList[i])==bBaseVoloe) bBaseCount++;
	}
	return (bBaseCount==3);
}

//是否两对
bool CGameLogic::IsLiangDui(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<4) return FALSE;
	
	BYTE bDoubleCount=0;
	for (BYTE i=0;i<bCardCount-1;i++)
	{
		if (GetCardValoe(bCardList[i])==GetCardValoe(bCardList[i+1]))
		{
			i++;
			bDoubleCount++;
			if (bDoubleCount==2) return true;
		}
	}
	return false;
}

//是否对子
bool CGameLogic::IsDuiZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount<2) return false;
	
	BYTE bDoubleCount=0;
	for (BYTE i=0;i<bCardCount-1;i++)
	{
		if (GetCardValoe(bCardList[i])==GetCardValoe(bCardList[i+1])) return true;
	}
	return false;
}

//对比单只扑克
bool CGameLogic::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstVolae=GetCardValoe(bFirstCard);
	BYTE bLastVolae=GetCardValoe(bLastCard);
	if (bFirstVolae==bLastVolae) return bFirstCard>bLastCard;
	return bFirstVolae>bLastVolae;
}

//对比同花顺
bool CGameLogic::CompareTongHuaShun(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//对比铁支
bool CGameLogic::CompareTeiZhi(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//对比葫芦
bool CGameLogic::CompareHuLu(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//对比同花
bool CGameLogic::CompareTongHua(BYTE bFirstList[], BYTE bLastList[])
{
	BYTE bFirst=0,bLast=0;
	for (BYTE i=0;i<5;i++)
	{
		bFirst=GetCardValoe(bFirstList[i]);
		bLast=GetCardValoe(bLastList[i]);
		if (bFirst!=bLast) return (bFirst>bLast);
	}
	return GetHuaKind(bFirstList[0])>GetHuaKind(bLastList[0]);
}

//对比顺子
bool CGameLogic::CompareShunZhi(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//对比三条
bool CGameLogic::CompareSanTiao(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//对比两对
bool CGameLogic::CompareLiangDui(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	BYTE bDoubleCard[2][4];
	if (FindDoubleCard(bFirstList,bCardCount,bDoubleCard[0])!=2) return false;
	if (FindDoubleCard(bLastList,bCardCount,bDoubleCard[1])!=2) return true;
	BYTE bFirstVoloe=GetCardValoe(bDoubleCard[0][0]);
	BYTE bLastVoloe=GetCardValoe(bDoubleCard[1][0]);
	if (bFirstVoloe==bLastVoloe) 
	{
		bFirstVoloe=GetCardValoe(bDoubleCard[0][2]);
		bLastVoloe=GetCardValoe(bDoubleCard[1][2]);
		if (bFirstVoloe!=bLastVoloe) return bFirstVoloe>bLastVoloe;
		return (GetHuaKind(bDoubleCard[0][0])==HEI_TAO);
	}
	return bFirstVoloe>bLastVoloe;
}

//对比对子
bool CGameLogic::CompareDuiZhi(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	BYTE bDoubleCard[2][4];
	if (FindDoubleCard(bFirstList,bCardCount,bDoubleCard[0])!=1) return false;
	if (FindDoubleCard(bLastList,bCardCount,bDoubleCard[1])!=1) return true;
	BYTE bFirstVoloe=GetCardValoe(bDoubleCard[0][0]);
	BYTE bLastVoloe=GetCardValoe(bDoubleCard[1][0]);
	if (bFirstVoloe==bLastVoloe) return (GetHuaKind(bDoubleCard[0][0])==HEI_TAO);
	return bFirstVoloe>bLastVoloe;
}

//对比散牌
bool CGameLogic::CompareSanCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//混乱扑克
void CGameLogic::RandCard(BYTE bCardBuffer[], BYTE bBufferCount)
{
	BYTE bSend=0,bStation=0,bCardList[CountArray(m_bCardArray)];
	CopyMemory(bCardList,m_bCardArray,sizeof(m_bCardArray));
	static long int dwRandCount=0L;
	srand((unsigned)time(NULL)+dwRandCount++);
	do
	{
		bStation=rand()%(CountArray(m_bCardArray)-bSend);
		bCardBuffer[bSend++]=bCardList[bStation];
		bCardList[bStation]=bCardList[CountArray(m_bCardArray)-bSend];
	} while (bSend<bBufferCount);

	return;
}

//排列扑克
void CGameLogic::SortCard(BYTE bCardList[], BYTE bCardCount)
{
	bool bSorted=true;
	BYTE bTemp,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++) 
		{
			if (CompareOnlyOne(bCardList[i],bCardList[i+1])==FALSE)
			{	
				bTemp=bCardList[i];
				bCardList[i]=bCardList[i+1];
				bCardList[i+1]=bTemp;
				bSorted=false;
			}	
		}
		bLast--;
	} while (bSorted==false);

	return;
}

//对比扑克，前者大于后者 TRUE，否则 FALSE
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	//排列扑克
	SortCard(bFirstList,bCardCount);
	SortCard(bLastList,bCardCount);
	
	//获取类型
	BYTE bFirstKind=GetCardKind(bFirstList,bCardCount);
	BYTE bLastKind=GetCardKind(bLastList,bCardCount);
	
	//类型不相同，对比类型
	if (bFirstKind!=bLastKind) return bFirstKind>bLastKind;
	
	//类型相同
	switch(bFirstKind)
	{
		//散牌
	case SHAN: return CompareSanCard(bFirstList,bLastList,bCardCount);
		//对子
	case DUI_ZI: return CompareDuiZhi(bFirstList,bLastList,bCardCount);
		//两对
	case LAING_DUI: return CompareLiangDui(bFirstList,bLastList,bCardCount);
		//三条
	case SAN_TIAO: return CompareSanTiao(bFirstList,bLastList);
		//顺子
	case SHUN_ZI: return CompareShunZhi(bFirstList,bLastList);
		//同花
	case TONG_HUA: return CompareTongHua(bFirstList,bLastList);
		//葫芦
	case HU_LU: return CompareHuLu(bFirstList,bLastList);
		//铁支
	case TIE_ZHI: return CompareTeiZhi(bFirstList,bLastList);
		//同花顺
	case TONG_HUA_SHUN: return CompareTongHuaShun(bFirstList,bLastList);
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////
CCalculateSink::CCalculateSink()
{
	m_pGameEnd = NULL;
}
CCalculateSink::~CCalculateSink()
{

}
//接口查询
void * __cdecl CCalculateSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICalculateSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICalculateSink,Guid,dwQueryVer);
	return NULL;
}
//ICalculateSink接口

//计算派彩
bool __cdecl CCalculateSink::CalculateResult(void* pCalculateContext,
											 WORD wChairID,
											 ICalculateItem* pItem,
											 DECIMAL* pPartedBetScore,
											 DECIMAL* pPartedWinScore,
											 DECIMAL* pPartedTaxScore,
											 WORD	 wBetRegionCount)
{
	GT_ASSERT(m_pGameEnd);
	GT_ASSERT(m_pGameEnd == pCalculateContext);
	GT_ASSERT(m_pGameServiceAttrib);
	GT_ASSERT(wChairID < m_pGameServiceAttrib->wChairCount);
	GT_ASSERT(wBetRegionCount == m_pGameServiceAttrib->wBetRegionCount);

	if(m_pGameEnd == NULL ||
		wChairID >= m_pGameServiceAttrib->wChairCount)
	{
		return false;
	}

	//转换Double派彩为Decimal派彩
	DECIMAL decTemp;
	memset(&decTemp, 0, sizeof(DECIMAL));
	VarDecFromR8(m_pGameEnd->fGameGold[wChairID], &decTemp);
	VarDecAdd(&decTemp, &pPartedWinScore[0], &pPartedWinScore[0]);

	//判断大于0,进行抽水结算
	if(VarDecCmpR8(&decTemp, 0) == VARCMP_GT)
	{
		//抽水 = 总派彩 * (- 抽水率)
		VarDecMul(&decTemp,(DECIMAL*) &m_decAfterTax, &pPartedTaxScore[0]);
		//更新Double派彩
		VarDecAdd(&decTemp,&pPartedTaxScore[0], &decTemp);
		VarR8FromDec(&decTemp, &m_pGameEnd->fGameGold[wChairID]);
	}

	return true;
}
//调整洗码
bool __cdecl CCalculateSink::RectifyValidBetScore(void* pCalculateContext,
												  ICalculateItem* pItem,
												  DECIMAL* pPartedBetScore,
												  WORD	 wBetRegionCount,
												  DECIMAL* pValidBetScore)
{
	return true;
}
bool __cdecl CCalculateSink::InitCalculateSink(ITableFrame	*pITableFrame,
											   CTableFrameSink *pTableFrameSink)
{
	m_pITableFrame=pITableFrame;
	if (m_pITableFrame==NULL) return false;

	m_pTableFrameSink = pTableFrameSink;
	GT_ASSERT(m_pTableFrameSink != NULL);

	//获取参数
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	GT_ASSERT(m_pGameServiceAttrib!=NULL);

	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	GT_ASSERT(m_pGameServiceOption!=NULL);

	m_pICalculateFrame = m_pITableFrame->GetICalculateFrame();
	GT_ASSERT(m_pICalculateFrame != NULL);

	memset(&m_decAfterTax, 0, sizeof(DECIMAL));
	DECIMAL decTemp;
	memset(&decTemp, 0, sizeof(DECIMAL));
	VarDecFromI4(-1, &decTemp);
	VarDecMul(&decTemp,(DECIMAL*) &m_pGameServiceOption->decTaxRate, &m_decAfterTax);

	return true;
}
//初始化计算上下文
bool __cdecl CCalculateSink::InitCalculateContext(CMD_S_GameEnd* pGameEnd)
{
	GT_ASSERT(pGameEnd != NULL);
	m_pGameEnd = pGameEnd;

	return true;
}
//获取计算上下文
void* __cdecl CCalculateSink::GetCalculateContext()
{
	return m_pGameEnd;
}

//////////////////////////////////////////////////////////////////////////
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));

	//金币信息
	m_fMaxGold=0;
	m_fTurnBasicGold=0;
	memset(m_fTableGold,0,sizeof(m_fTableGold));

	//辅助变量
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pICalculateFrame=NULL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	
	GT_ASSERT(m_pGameServiceOption!=NULL);

	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	GT_ASSERT(m_pGameServiceAttrib!=NULL);

	m_pICalculateFrame = m_pITableFrame->GetICalculateFrame();
	GT_ASSERT(m_pICalculateFrame != NULL);

	return m_CalculateSink.InitCalculateSink(m_pITableFrame, this);

}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//状态变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//金币变量
	m_fMaxGold=0L;
	m_fTurnBasicGold=0L;
	memset(m_fTableGold,0,sizeof(m_fTableGold));

	return;
}

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	GT_ASSERT(wChairID<m_wPlayerCount);
	return m_bPlayStatus[wChairID]?true:false;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//状态变量
	m_wOperaCount=0;
	m_bSendCardCount=0;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//金币变量
	m_fMaxGold=0L;
	m_fTurnBasicGold=0L;
	memset(m_fTableGold,0,sizeof(m_fTableGold));

	//设置状态
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//变量定义
	const tagUserScore * pUserScore=NULL;
	IServerUserItem * pIServerUserItem=NULL;

	//计算下注
	bool bCalculateGold=false;
	

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			m_bPlayStatus[i]=true;
			m_fTableGold[i*2+1]=m_pGameServiceOption->dwCellScore;
			if (bCalculateGold==false)
			{
				bCalculateGold=true;
				pUserScore=pIServerUserItem->GetUserScore();
				GT_ASSERT(pUserScore->fScore>m_pGameServiceOption->dwCellScore);
				m_fMaxGold=pUserScore->fScore;
			}
			else 
			{
				pUserScore=pIServerUserItem->GetUserScore();
				GT_ASSERT(pUserScore->fScore>1L);
				m_fMaxGold=__min(m_fMaxGold,pUserScore->fScore);
			}
		}
	}
	if(m_fMaxGold > m_pGameServiceOption->dwHighScore)
		m_fMaxGold = m_pGameServiceOption->dwHighScore;

	//投注
	tagBetScoreInfo BetScoreInfo;
	BetScoreInfo.dwBetScore = m_pGameServiceOption->dwCellScore;
	BetScoreInfo.wBetRegionIndex = 0;
	for(int i = 0; i < m_pGameServiceAttrib->wChairCount; i ++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if(pIServerUserItem != NULL)
		{
			BetScoreInfo.pIServerUserItem = pIServerUserItem;

			if(m_pICalculateFrame->BetScore(pIServerUserItem, &BetScoreInfo, 1) == false)
			{
				return false;
			}
		}
	}

	//分发扑克
	m_bSendCardCount=2;
	m_GameLogic.RandCard(m_bTableCardArray[0],sizeof(m_bTableCardArray)/sizeof(m_bTableCardArray[0][0]));
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			m_bTableCardCount[i]=m_bSendCardCount;
		}
	}
	m_wCurrentUser=DeduceWiner(1,1);
	//是否AI对杀普通玩家
	if(m_pITableFrame->IsAIKillGameUser())
	{
		//机器人智商-相当几率交换最差牌给普通玩家
		if(CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
		{
			DWORD wLoser = DeduceLoser(0,4);
			DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
			if(wGameUser != wLoser)
			{
				BYTE bTableCardArray[5];
				memcpy(bTableCardArray, m_bTableCardArray[wLoser], sizeof(bTableCardArray));
				memcpy(m_bTableCardArray[wLoser], m_bTableCardArray[wGameUser], sizeof(bTableCardArray));
				memcpy(m_bTableCardArray[wGameUser], bTableCardArray, sizeof(bTableCardArray));
			}
		}
	}

	//发送开始
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.fMaxGold=m_fMaxGold;
	GameStart.fTurnMaxGold=m_fMaxGold/4L;
	if(GameStart.fTurnMaxGold <= m_pGameServiceOption->dwCellScore)
	{
		GameStart.fTurnMaxGold = m_pGameServiceOption->dwCellScore;
	}
	GameStart.fTurnBasicGold=m_fTurnBasicGold;
	for (WORD i=0;i<m_wPlayerCount;i++) GameStart.bCardData[i]=m_bTableCardArray[i][1];
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		GameStart.bFundusCard=0;
		if (m_bPlayStatus[i]==TRUE)
		{
			GameStart.bFundusCard=m_bTableCardArray[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//设置定时器
	//m_pITableFrame->SetGameTimer(IDI_TIME_GIVE_UP,35000L,1,m_wCurrentUser);

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
	case GER_NO_PLAYER:		//没有玩家
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			CopyMemory(GameEnd.bPlayStatus,m_bPlayStatus,sizeof(m_bPlayStatus));
			GameEnd.fMaxGold=m_fMaxGold;
			GameEnd.fTurnBasicGold=m_fTurnBasicGold;
			GameEnd.fBasicGold=m_pGameServiceOption->dwCellScore;
			CopyMemory(GameEnd.fTableGold,m_fTableGold,sizeof(m_fTableGold));

			//设置下注
			if (m_bSendCardCount<=2)
			{
				GameEnd.fTurnMaxGold=m_fMaxGold/4L;
				if(GameEnd.fTurnMaxGold <= m_pGameServiceOption->dwCellScore)
				{
					GameEnd.fTurnMaxGold = m_pGameServiceOption->dwCellScore;
				}
			}
			else if (m_bSendCardCount==3) GameEnd.fTurnMaxGold=m_fMaxGold/2L;
			else GameEnd.fTurnMaxGold=m_fMaxGold;

			//设置扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bTableCardCount[i]=m_bTableCardCount[i];
				GameEnd.bTableCardArray[i][0]=m_bTableCardArray[i][0];
				if(m_bTableCardCount[i] >= 1)
				{
					CopyMemory(&GameEnd.bTableCardArray[i][1],&m_bTableCardArray[i][1],(m_bTableCardCount[i]-1)*sizeof(BYTE));	
				}
			}


			//计算总注
			DOUBLE fAlfGold=0L;
			for (WORD i=0;i<CountArray(m_fTableGold);i++) fAlfGold+=m_fTableGold[i];

			//统计信息
			WORD wWinerUser=DeduceWiner(0,4);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//底牌信息
				if ((cbReason==GER_NO_PLAYER)||(m_bPlayStatus[i]==FALSE)) GameEnd.bUserCard[i]=0;
				else GameEnd.bUserCard[i]=m_bTableCardArray[i][0];

				//信用额度信息
				if (i==wWinerUser)
				{
					GameEnd.fGameGold[i]=fAlfGold-m_fTableGold[i*2]-m_fTableGold[i*2+1];
				}
				else GameEnd.fGameGold[i]=-m_fTableGold[i*2]-m_fTableGold[i*2+1];
			}

			//修改用户额度
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					GameEnd.fGameGold[i]=GameEnd.fGameGold[i];
				}
				else
				{
					GameEnd.fGameGold[i]=-m_fTableGold[i*2]-m_fTableGold[i*2+1];
				}
			}
			GameEnd.bSendCardCount = m_bSendCardCount;
			GameEnd.wWinerUser = wWinerUser;

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd, sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);
				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				//结束游戏
				m_pITableFrame->ConcludeGame();
				return true;

			}
			else
			{
				//删除定时器
				/*if (m_bTimeStation!=255)
				{
					KillTimer(m_bTimeStation);
					m_bTimeStation=255;
				}*/				
				GT_ASSERT(FALSE);
				//结束游戏
				m_pITableFrame->ConcludeGame();
				return false;
			}


		
		}
	case GER_USER_LEFT:		//用户强退
		{
			//效验参数
			GT_ASSERT(pIServerUserItem!=NULL);
			GT_ASSERT(wChairID<m_wPlayerCount);

			//强退处理
			OnUserGiveUp(wChairID);

			return true;
		}
	}

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			StatusFree.fBasicGold=m_pGameServiceOption->dwCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.fMaxGold=m_fMaxGold;
			StatusPlay.fTurnBasicGold=m_fTurnBasicGold;
			StatusPlay.fBasicGold=m_pGameServiceOption->dwCellScore;
			CopyMemory(StatusPlay.bPlayStatus,m_bPlayStatus,sizeof(m_bPlayStatus));
			CopyMemory(StatusPlay.fTableGold,m_fTableGold,sizeof(m_fTableGold));

			//设置下注
			if (m_bSendCardCount<=2)
			{
				StatusPlay.fTurnMaxGold=m_fMaxGold/4L;
				if(StatusPlay.fTurnMaxGold <= m_pGameServiceOption->dwCellScore)
				{
					StatusPlay.fTurnMaxGold = m_pGameServiceOption->dwCellScore;
				}
			}
			else if (m_bSendCardCount==3) StatusPlay.fTurnMaxGold=m_fMaxGold/2L;
			else StatusPlay.fTurnMaxGold=m_fMaxGold;

			//设置扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					GT_ASSERT(m_pITableFrame->GetServerUserItem(i)!=NULL);
					StatusPlay.bTableCardCount[i]=m_bTableCardCount[i];
					if ((i==wChiarID)&&(bSendSecret==true)) StatusPlay.bTableCardArray[i][0]=m_bTableCardArray[i][0];
					if(m_bTableCardCount[i] >= 1)
						CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_bTableCardArray[i][1],(m_bTableCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//效验结果
	GT_ASSERT(FALSE);

	return false;
}
//人工智能游戏动作
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	//机器人判断
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentUser);
	if(pIServerUserItem == NULL || 
		pIServerUserItem->IsAIUser() == false || 
		pIServerUserItem->GetUserStatus() != US_PLAY)
		return false;
	WORD wChairID = pIServerUserItem->GetChairID();
	if(wChairID == INVALID_CHAIR)
		return false;

	WORD wCurrentWiner=DeduceWiner(0,4);
	//判断当前赢家
	if(wCurrentWiner == wChairID)
	{
	}
	else
	{
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
		//机器人智商-相当几率放弃
		if(wGameUser == wCurrentWiner && CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
		{
			//发送消息
			OnGameMessage(SUB_C_GIVE_UP, NULL, 0, pIServerUserItem);

			return true;
		}
		else
		{

		}
	}
	//获取金币	
	LONG lTurnMaxGold = 1;
	if (m_bSendCardCount<=2)
	{
		lTurnMaxGold=m_fMaxGold/4L;
		if(lTurnMaxGold <= m_pGameServiceOption->dwCellScore)
		{
			lTurnMaxGold = m_pGameServiceOption->dwCellScore;
		}
	}
	else if (m_bSendCardCount==3) lTurnMaxGold=m_fMaxGold/2L;
	else lTurnMaxGold=m_fMaxGold;

	lTurnMaxGold=lTurnMaxGold-m_fTableGold[wChairID*2+1];
	lTurnMaxGold = __max(lTurnMaxGold,m_fTurnBasicGold);

	lTurnMaxGold = lTurnMaxGold - m_fTurnBasicGold;
	lTurnMaxGold += 1;
	LONG lGold=0;
	if(lTurnMaxGold!=0)
	{
		lGold= (wCurrentWiner == wChairID) ? (((rand() % (LONG)lTurnMaxGold)/2)  + lTurnMaxGold/2): (rand() % ((LONG)lTurnMaxGold)/4);
	}
	else
	{
		//lGold = 1;
	}
	lGold=lGold + m_fTurnBasicGold;
	lGold=__max(lGold,m_fTurnBasicGold);


	//发送数据
	CMD_C_AddGold AddPoint;
	AddPoint.fGold=lGold;
	OnGameMessage(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint), pIServerUserItem);

	return true;
}
//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_GOLD:	//用户加注
		{
			//效验数据
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;
			if (m_bPlayStatus[pUserData->UserStatus.wChairID]==FALSE) return true;

			//消息处理
			CMD_C_AddGold * pAddGold=(CMD_C_AddGold *)pDataBuffer;
			OnUserAddGold(pUserData->UserStatus.wChairID,pAddGold->fGold,false);

			return true;
		}
	case SUB_C_GIVE_UP:		//用户放弃
		{
			//效验数据
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;
			if (m_bPlayStatus[pUserData->UserStatus.wChairID]==FALSE) return true;

			//消息处理
			OnUserGiveUp(pUserData->UserStatus.wChairID);

			return true;
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//放弃事件
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//状态判断
	GT_ASSERT(m_bPlayStatus[wChairID]==TRUE);
	if (m_bPlayStatus[wChairID]==FALSE) return false;

	//设置数据
	m_bPlayStatus[wChairID]=FALSE;
	//m_bTableCardCount[wChairID]=0;

	//发送放弃消息
	CMD_S_GiveUp GiveUp;
	GiveUp.wUserChairID=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));


	//判断结束
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==TRUE) wPlayerCount++;
	}
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddGold(wChairID,0L,true);
	}
	else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//加注事件
bool CTableFrameSink::OnUserAddGold(WORD wChairID, DOUBLE fGold, bool bCancel)
{
	WORD i = 0;
	//设置数据
	if (bCancel==false)
	{
		//效验数据
		GT_ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//计算限注
		DOUBLE fMaxGold=0L;
		if (m_bSendCardCount<=2)
		{
			fMaxGold=m_fMaxGold/4L;
			if(fMaxGold <= m_pGameServiceOption->dwCellScore)
			{
				fMaxGold = m_pGameServiceOption->dwCellScore;
			}
		}
		else if (m_bSendCardCount==3) fMaxGold=m_fMaxGold/2L;
		else fMaxGold=m_fMaxGold;
		
		//计算剩下的金币
		DOUBLE fLeaveGold=fMaxGold-m_fTableGold[wChairID*2+1];
		m_fTableGold[wChairID*2]=__min(fLeaveGold,__max(fGold,m_fTurnBasicGold));
	}

	//操作次数
	bool bFinishTurn=false;
	WORD wNextUser=INVALID_CHAIR;
	for (i=0;i<m_wPlayerCount-1;i++)
	{
		m_wOperaCount++;
		wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
		if (m_bPlayStatus[wNextUser]==TRUE) break;
	}
	
	//判断下注
	if (m_wOperaCount>=m_wPlayerCount)
	{
		DOUBLE fTableGold=0L;
		for (i=0;i<m_wPlayerCount;i++)
		{
			if (m_bPlayStatus[i]==TRUE)
			{
				fTableGold=m_fTableGold[i*2];
				break;
			}
		}
		while (i<m_wPlayerCount)
		{
			if ((m_bPlayStatus[i]==TRUE)&&(m_fTableGold[i*2]!=fTableGold)) break;
			i++;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//一轮判断
	if (bFinishTurn==true)
	{
		//发送数据
		CMD_S_AddGold GoldResult;
		GoldResult.fCurrentLessGold=0L;
		GoldResult.wLastChairID=wChairID;
		GoldResult.wCurrentUser=INVALID_CHAIR;
		GoldResult.fLastAddGold=m_fTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));

		//投注
		tagBetScoreInfo BetScoreInfo;
		BetScoreInfo.wBetRegionIndex = 0;
		for(int i = 0; i < m_pGameServiceAttrib->wChairCount; i ++)
		{
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pIServerUserItem != NULL)
			{
				BetScoreInfo.dwBetScore = m_fTableGold[i*2];
				BetScoreInfo.pIServerUserItem = pIServerUserItem;

				if(m_pICalculateFrame->BetScore(pIServerUserItem, &BetScoreInfo, 1) == false)
				{
					GT_ASSERT(FALSE);
					return false;
				}
			}
		}
		//累计金币
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_fTableGold[i*2+1]+=m_fTableGold[i*2];
			m_fTableGold[i*2]=0L;
		}

		//开始一轮
		if (m_bSendCardCount<5) 
		{
			//设置变量
			m_wOperaCount=0;
			m_bSendCardCount++;
			m_fTurnBasicGold=0L;
			m_wCurrentUser=DeduceWiner(1,m_bSendCardCount-1);

			//发送扑克
			CMD_R_SendCard SendCard;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if (m_bPlayStatus[i]==TRUE) m_bTableCardCount[i]=m_bSendCardCount;
				SendCard.bUserCard[i]=m_bTableCardArray[i][m_bSendCardCount-1];
			}

			//删除旧定时器
			/*if (m_bTimeStation!=255)
			{
			KillTimer(m_bTimeStation);
			m_bTimeStation=255;
			}*/

			//设置定时器
			/*m_bTimeStation=TIME_PASS+m_wCurrentUser;
			SetTimer(m_bTimeStation,TIME_COUNT);*/

			//发送消息
			SendCard.wCurrentUser=m_wCurrentUser;
			SendCard.fMaxGold=(m_bSendCardCount!=3)?m_fMaxGold:m_fMaxGold/2L;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
	}
	else
	{
		//设置变量
		m_wCurrentUser=wNextUser;
		m_fTurnBasicGold=__max(m_fTableGold[wChairID*2],m_fTurnBasicGold);

		//发送数据
		CMD_S_AddGold AddGold;
		AddGold.wLastChairID=wChairID;
		AddGold.wCurrentUser=m_wCurrentUser;
		AddGold.fCurrentLessGold=m_fTurnBasicGold;
		AddGold.fLastAddGold=m_fTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));

		//删除旧定时器
		/*if (m_bTimeStation!=255)
		{
			KillTimer(m_bTimeStation);
			m_bTimeStation=255;
		}

		//设置定时器
		m_bTimeStation=TIME_PASS+m_wCurrentUser;
		SetTimer(m_bTimeStation,TIME_COUNT);*/
	}

	return true;
}

//推断胜者
WORD CTableFrameSink::DeduceWiner(BYTE bBeginPos, BYTE bEndPos)
{
	BYTE bWiner=0;
	//保存扑克
	BYTE bTableCardArray[m_wPlayerCount][5];
	CopyMemory(bTableCardArray,m_bTableCardArray,sizeof(m_bTableCardArray));

	//寻找玩家
	for (bWiner=0;bWiner<m_wPlayerCount;bWiner++)
	{
		if (m_bPlayStatus[bWiner]==TRUE) break;
	}

	//对比玩家
	for (BYTE i=(bWiner+1);i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==false) continue;
		if (m_GameLogic.CompareCard(bTableCardArray[i]+bBeginPos,
			bTableCardArray[bWiner]+bBeginPos,
			bEndPos-bBeginPos+1)==true)
			bWiner=i;
	}

	return bWiner;
}
//推断输者
WORD CTableFrameSink::DeduceLoser(BYTE bBeginPos, BYTE bEndPos)
{
	BYTE bLoser=0;
	//保存扑克
	BYTE bTableCardArray[m_wPlayerCount][5];
	CopyMemory(bTableCardArray,m_bTableCardArray,sizeof(m_bTableCardArray));

	//寻找玩家
	for (bLoser=0;bLoser<m_wPlayerCount;bLoser++)
	{
		if (m_bPlayStatus[bLoser]==TRUE) break;
	}

	//对比玩家
	for (BYTE i=(bLoser+1);i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==false) continue;
		if (m_GameLogic.CompareCard(bTableCardArray[bLoser]+bBeginPos,
			bTableCardArray[i]+bBeginPos,
			bEndPos-bBeginPos+1)==true)
			bLoser=i;
	}

	return bLoser;
}
//////////////////////////////////////////////////////////////////////////
