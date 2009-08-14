#include "StdAfx.h"
#include "TableFrameSink.h"
#include ".\tableframesink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const enStartMode	CTableFrameSink::m_GameStartMode = enStartMode_AllReady;	//开始模式
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
	m_wCurrentUser   = INVALID_CHAIR;	
	m_wPlayerCount   = 0;		

	//游戏庄家
	m_nBankerStation = -1;
	m_fBankerScore   = 0L;
	m_bBankerWin     = false;
	m_bBankerGetCard = false;

	//标识加注
	m_bAddGoldEnd    = false;	
	memset(m_bAddGold,0,sizeof(m_bAddGold));
	memset(m_fUserAddGold,0,sizeof(m_fUserAddGold));

	//金币信息
	m_fMaxGold       = 0L;
	m_fTurnBasicGold = 1L;
	memset(m_fTableGold,0,sizeof(m_fTableGold));
	
	//分牌相关
	memset(m_cbCutCard,0,sizeof(m_cbCutCard));
	memset(m_cbTableCutCardCount,0,sizeof(m_cbTableCutCardCount));
	memset(m_cbTableCutCardArray,0,sizeof(m_cbTableCutCardArray));

	//初始扑克	
  	memset(m_cbTableCardCount,0,sizeof(m_cbTableCardCount));	
	memset(m_cbTableCardArray,0,sizeof(m_cbTableCardArray));  
    
	//发牌变量
	memset(m_cbRandCardArray,0,sizeof(m_cbRandCardArray)); 
	m_cbSendCardCount = 0;

	//玩家状态		
	memset(m_cbCardStatus,0,sizeof(m_cbCardStatus));
	memset(m_cbCutCardStatus,0,sizeof(m_cbCutCardStatus));	
	
	//组件变量
	m_pITableFrame     = NULL;
	pGameServiceOption = NULL;
	m_pGameServiceAttrib=NULL;
	m_pICalculateFrame=NULL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid,
											   DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);
	
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx != NULL);

	m_pITableFrame = GET_OBJECTPTR_INTERFACE(pIUnknownEx, ITableFrame);
	
	if (m_pITableFrame == NULL)
	{
		return false;
	}

	//获取参数
	pGameServiceOption = m_pITableFrame->GetGameServiceOption();

	GT_ASSERT(pGameServiceOption != NULL);

	m_pITableFrame->SetGameStatus(GS_FREE);

	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	GT_ASSERT(m_pGameServiceAttrib!=NULL);

	m_pICalculateFrame = m_pITableFrame->GetICalculateFrame();
	GT_ASSERT(m_pICalculateFrame != NULL);

	m_fTurnBasicGold = pGameServiceOption->dwCellScore;

	return m_CalculateSink.InitCalculateSink(m_pITableFrame, this);

}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//游戏变量							
	m_wCurrentUser   = INVALID_CHAIR;	
	m_wPlayerCount   = 0;		

	//游戏庄家
	m_fBankerScore   = 0L;
	m_bBankerGetCard = false;

	m_nNotBanker = 0;
	memset(m_fNotBankerScore, 0, sizeof(DOUBLE) * GAME_PLAYER);

	//标识加注
	m_bAddGoldEnd    = false;	
	memset(m_bAddGold,0,sizeof(m_bAddGold));
	memset(m_fUserAddGold,0,sizeof(m_fUserAddGold));	

	//金币信息
	m_fMaxGold       = 0L;
	m_fTurnBasicGold = pGameServiceOption->dwCellScore;
	memset(m_fTableGold,0,sizeof(m_fTableGold));

	//分牌相关
	memset(m_cbCutCard,0,sizeof(m_cbCutCard));
	memset(m_cbTableCutCardCount,0,sizeof(m_cbTableCutCardCount));
	memset(m_cbTableCutCardArray,0,sizeof(m_cbTableCutCardArray));

	//初始扑克	
  	memset(m_cbTableCardCount,0,sizeof(m_cbTableCardCount));	
	memset(m_cbTableCardArray,0,sizeof(m_cbTableCardArray));
	memset(m_cbRandCardArray,0,sizeof(m_cbRandCardArray));  
    
	//发牌变量
	memset(m_cbRandCardArray,0,sizeof(m_cbRandCardArray)); 
	m_cbSendCardCount = 0;

	//玩家状态		
	memset(m_cbCardStatus,0,sizeof(m_cbCardStatus));
	memset(m_cbCutCardStatus,0,sizeof(m_cbCutCardStatus));	

	//设置状态
	m_pITableFrame->SetGameStatus(GS_FREE);

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
	return  true;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, 
											IServerUserItem * pIServerUserItem, 
											BYTE bGameStatus, 
											bool bSendSecret)
{
	bool bWatchUser;

	const tagServerUserData *pUserData = pIServerUserItem->GetUserData();
	
	if (pUserData->UserStatus.cbUserStatus != US_LOOKON)
		bWatchUser = false; 
	
	switch (bGameStatus)
	{
	case GS_FREE:
		{			
			//构造数据
			CMD_S_StatusFree StatusFree;

			memset(&StatusFree, 0, sizeof(StatusFree));

			StatusFree.fBasicGold = pGameServiceOption->dwCellScore;

			//发送场景
			return 	m_pITableFrame->SendGameScene(pIServerUserItem,
			                                      &StatusFree,
												  sizeof(StatusFree));

	
		}
	case GS_ADDGOLDING:
		{
			//构造数据
			CMD_S_StatusAddGold StatusAddGold;
			memset(&StatusAddGold,0,sizeof(StatusAddGold));

			StatusAddGold.fBasicGold    = pGameServiceOption->dwCellScore;
			StatusAddGold.fMaxGold       = m_fMaxGold;		
			StatusAddGold.wBankerStation = m_nBankerStation;
			StatusAddGold.wCurrentUser   = wChiarID;
			CopyMemory(StatusAddGold.bAddGoldEnd,m_bAddGold,sizeof(m_bAddGold));
			CopyMemory(StatusAddGold.fTableGold,m_fTableGold,sizeof(m_fTableGold));
		
			//发送场景
			return 	m_pITableFrame->SendGameScene(pIServerUserItem,
			                                      &StatusAddGold,
												  sizeof(StatusAddGold));
		}

	case GS_PLAYING:
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			StatusPlay.wCurrentUser   = m_wCurrentUser;
			StatusPlay.wBankerStation = m_nBankerStation;
			StatusPlay.bBankerGetCard = m_bBankerGetCard;
			StatusPlay.fMaxGold       = m_fMaxGold;				
			StatusPlay.fBasicGold     = pGameServiceOption->dwCellScore;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbCardStatus,sizeof(m_cbCardStatus));
			CopyMemory(StatusPlay.fTableGold,m_fTableGold,sizeof(m_fTableGold));

			//设置扑克
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbCardStatus[i] != NO_CARD)
				{
					GT_ASSERT(m_pITableFrame->GetServerUserItem(i) != NULL);
					StatusPlay.cbTableCardCount[i] = m_cbTableCardCount[i];
					if ((i == wChiarID) && (bSendSecret == true)) 
					{
						StatusPlay.cbTableCardArray[i][0] = m_cbTableCardArray[i][0];
					}				
				
					CopyMemory(&StatusPlay.cbTableCardArray[i][1],&m_cbTableCardArray[i][1],(m_cbTableCardCount[i]-1)*sizeof(BYTE));
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
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	switch(cbGameStatus) 
	{
	case GS_ADDGOLDING:
		{
			if(m_bAddGoldEnd)
				return true;
			WORD wActionUser[GAME_PLAYER];
			BYTE bActionUserCount = 0;
			for(BYTE i = 0; i < GAME_PLAYER; i ++)
			{
				if (m_cbCardStatus[i] != NO_CARD)
				{
					if( !m_bAddGold[i] && i != m_nBankerStation)
					{
						//机器人判断
						IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
						if(pIServerUserItem == NULL || 
							pIServerUserItem->IsAIUser() == false || 
							pIServerUserItem->GetUserStatus() != US_PLAY)
							continue;
						WORD wChairID = pIServerUserItem->GetChairID();
						if(wChairID == INVALID_CHAIR)
							continue;

						wActionUser[bActionUserCount ++] = i; 
					}
				}
			}

			if(bActionUserCount)
			{
				BYTE bSelAddGoldUser	= rand() % bActionUserCount;
				bSelAddGoldUser = wActionUser[bSelAddGoldUser];
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(bSelAddGoldUser);

				LONG lAddGold = rand() % (LONG)m_fMaxGold;
				if(lAddGold > 10)
					lAddGold = lAddGold / 10 * 10;
				//设置信息
				CMD_C_AddGold AddPoint;
				AddPoint.bDoubleGold = false;
				AddPoint.fGold       = lAddGold;

				OnGameMessage(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint), pIServerUserItem);
			}
		}
		break;
	case GS_PLAYING:
		{
			WORD wActionUser[GAME_PLAYER];
			BYTE bActionUserCount = 0;
			for(BYTE i = 0; i < GAME_PLAYER; i ++)
			{
				if (m_cbCardStatus[i] != NO_CARD &&
					m_cbCardStatus[i] != STOP_CARD &&
					m_cbCardStatus[i] != BRUST_CARD)
				{
					//机器人判断
					IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
					if(pIServerUserItem == NULL || 
						pIServerUserItem->IsAIUser() == false || 
						pIServerUserItem->GetUserStatus() != US_PLAY)
						continue;
					WORD wChairID = pIServerUserItem->GetChairID();
					if(wChairID == INVALID_CHAIR)
						continue;
					if(m_bBankerGetCard)
					{
						if(i == m_nBankerStation)
						{
							wActionUser[bActionUserCount ++] = i; 
							break;
						}
					}
					else
					{
						if(i != m_nBankerStation)
							wActionUser[bActionUserCount ++] = i;

					}

				}
			}
			
			if(bActionUserCount)
			{
				BYTE bSelAddGoldUser	= rand() % bActionUserCount;
				WORD wChairID = wActionUser[bSelAddGoldUser];
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

				//是否AI对杀普通玩家
				if(m_pITableFrame->IsAIKillGameUser())
				{
					DOUBLE fGameGold = 0, fBankerScore = 0;
					bool bCutCard  = false;
					DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();

					if(m_bBankerGetCard && wChairID == m_nBankerStation)
					{
						//机器人为庄并且赢普通玩家停牌，否则继续要牌
						BYTE cbCardStatus = m_cbCardStatus[wGameUser];
						CalcScore(cbCardStatus, wGameUser, fGameGold, fBankerScore, bCutCard);

						if(fBankerScore > 0)
						{
							//发送消息
							CMD_C_StopCard StopCard;
							StopCard.bCutCard = 0;
							OnGameMessage(SUB_C_STOP_CARD,&StopCard,sizeof(StopCard),pIServerUserItem);
						}
						else
						{
							//发送消息
							CMD_C_GetCard GetCard;
							GetCard.bCutCard = 0;

							OnGameMessage(SUB_C_GET_CARD,&GetCard,sizeof(GetCard),pIServerUserItem);
						}
					}
					else
					{
						BYTE cbCardSum = m_GameLogic.GetCardSum(m_cbTableCardArray[wChairID],m_cbTableCardCount[wChairID]);
						//大于16点停牌
						if(cbCardSum > 18)
						{
							//发送消息
							CMD_C_StopCard StopCard;
							StopCard.bCutCard = 0;
							OnGameMessage(SUB_C_STOP_CARD,&StopCard,sizeof(StopCard),pIServerUserItem);
						}
						else
						{
							//发送消息
							CMD_C_GetCard GetCard;
							GetCard.bCutCard = 0;

							OnGameMessage(SUB_C_GET_CARD,&GetCard,sizeof(GetCard),pIServerUserItem);
						}
					}
				}
				else
				{
					BYTE cbCardSum = m_GameLogic.GetCardSum(m_cbTableCardArray[wChairID],m_cbTableCardCount[wChairID]);
					//大于16点停牌
					if(cbCardSum > 16)
					{
						//发送消息
						CMD_C_StopCard StopCard;
						StopCard.bCutCard = 0;
						OnGameMessage(SUB_C_STOP_CARD,&StopCard,sizeof(StopCard),pIServerUserItem);
					}
					else
					{
						//发送消息
						CMD_C_GetCard GetCard;
						GetCard.bCutCard = 0;

						OnGameMessage(SUB_C_GET_CARD,&GetCard,sizeof(GetCard),pIServerUserItem);
					}
				}

			}
		}
		break;
	default:
		break;
	}
	return true;
}
//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, 
											const void * pDataBuffer, 
											WORD wDataSize, 
											IServerUserItem * pIServerUserItem)
{
	bool bWatchUser = false;

	const tagServerUserData *pUserData = pIServerUserItem->GetUserData();

	if(pUserData->UserStatus.cbUserStatus == US_LOOKON)
	{
		bWatchUser = true;
	}

	WORD wChairID = pIServerUserItem->GetChairID();

	switch (wSubCmdID)
	{
	case SUB_C_ADD_GOLD:	//用户加注
		{
			//校验数据
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus != US_PLAY) return true;
			GT_ASSERT(m_cbCardStatus[pUserData->UserStatus.wChairID] == INIT_CARD);
			if (m_cbCardStatus[pUserData->UserStatus.wChairID] != INIT_CARD) return true;

			//获取数据
			CMD_C_AddGold * pAddGold = (CMD_C_AddGold *)pDataBuffer;
		
			//设置信息
			CMD_S_AddGold  AddGoldResult;
			AddGoldResult.wCurrentUser = pUserData->UserStatus.wChairID;

			//校验数据
			GT_ASSERT(pAddGold->fGold <= m_fMaxGold);
			if(pAddGold->fGold > m_fMaxGold) return false;
			//if(pAddGold->fGold < m_fTurnBasicGold)
			//	pAddGold->fGold = m_fTurnBasicGold;

			////记录初始闲家下注
			//if (!pAddGold->bDoubleGold)
			//{
			//	m_fNotBankerScore[wChairID]=pAddGold->fGold;
		//	}

			//双倍处理
			if(pAddGold->bDoubleGold)
			{				
				AddGoldResult.fLastAddGold = 2 * m_fUserAddGold[pUserData->UserStatus.wChairID];
				AddGoldResult.bDoubleGold  = true;

				//投注
				tagBetScoreInfo BetScoreInfo;
				BetScoreInfo.dwBetScore = m_fUserAddGold[pUserData->UserStatus.wChairID];
				BetScoreInfo.wBetRegionIndex = 0;
				BetScoreInfo.pIServerUserItem = pIServerUserItem;
				if(m_pICalculateFrame->BetScore(pIServerUserItem, &BetScoreInfo, 1) == false)
				{
					return false;
				}

				m_fUserAddGold[pUserData->UserStatus.wChairID] = 2 * m_fUserAddGold[pUserData->UserStatus.wChairID];	

			
			}
			else
			{
				AddGoldResult.fLastAddGold = pAddGold->fGold;
				AddGoldResult.bDoubleGold  = false;

				
				//投注
				tagBetScoreInfo BetScoreInfo;
				BetScoreInfo.dwBetScore = pAddGold->fGold;
				BetScoreInfo.wBetRegionIndex = 0;
				BetScoreInfo.pIServerUserItem = pIServerUserItem;
				if(m_pICalculateFrame->BetScore(BetScoreInfo.pIServerUserItem, &BetScoreInfo, 1) == false)//闲家
				{
					return false;
				}
				BetScoreInfo.pIServerUserItem = m_pITableFrame->GetServerUserItem(m_nBankerStation);//庄家
				if(m_pICalculateFrame->BetScore(BetScoreInfo.pIServerUserItem, &BetScoreInfo, 1) == false)
				{
					return false;
				}

				m_fUserAddGold[pUserData->UserStatus.wChairID] = pAddGold->fGold;

			}	

			m_fTableGold[pUserData->UserStatus.wChairID * 2]     = m_fTurnBasicGold;
			//m_fTableGold[pUserData->UserStatus.wChairID * 2 + 1] = m_fUserAddGold[pUserData->UserStatus.wChairID] - m_fTurnBasicGold;
			m_fTableGold[pUserData->UserStatus.wChairID * 2 + 1] = m_fUserAddGold[pUserData->UserStatus.wChairID];

			//标识加注
			m_bAddGold[pUserData->UserStatus.wChairID] = true;
			m_bAddGoldEnd = true;

			//发送消息
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{		
				if (m_cbCardStatus[i] != NO_CARD)
				{
					if( !m_bAddGold[i] && (i != m_nBankerStation) )  m_bAddGoldEnd = false; 
					m_pITableFrame->SendTableData(i,SUB_S_ADD_GOLD,&AddGoldResult,sizeof(AddGoldResult));
				}
				m_pITableFrame->SendLookonData(i,SUB_S_ADD_GOLD,&AddGoldResult,sizeof(AddGoldResult));
			}

			//加注结束
			//开始发牌
			if(m_bAddGoldEnd && !pAddGold->bDoubleGold)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_PLAYING);
			
				//初始发牌
				for (BYTE i = 0; i < GAME_PLAYER; i++) 
				{
					if (m_cbCardStatus[i] == INIT_CARD)
					{
						m_cbTableCardCount[i] = 2;
					}
				}

				//设置信息
				CMD_S_InitCard InitCard;
		
				for (WORD i = 0; i < GAME_PLAYER; i++) 
				{
					m_cbTableCardArray[i][1] = GetRandCard();
					InitCard.cbCardData[i]   = m_cbTableCardArray[i][1];						
				}

				for (WORD i = 0; i < GAME_PLAYER; i++)
				{		
					InitCard.cbFundusCard = 0;

					if (m_cbCardStatus[i] != NO_CARD)
					{					
						m_cbTableCardArray[i][0] = GetRandCard();

						InitCard.cbFundusCard  = m_cbTableCardArray[i][0];									
						InitCard.cbCardSum     = m_GameLogic.GetCardSum(m_cbTableCardArray[i],2);
						InitCard.bCanCutCard   = m_GameLogic.CanCutCard(m_cbTableCardArray[i][0],m_cbTableCardArray[i][1]);

						//发送消息
						m_pITableFrame->SendTableData(i,SUB_S_INIT_CARD,&InitCard,sizeof(InitCard));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_INIT_CARD,&InitCard,sizeof(InitCard));
				}
			}

			return true;
		}
	case SUB_C_STOP_CARD:	//用户停牌
		{	
			//校验数据
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus != US_PLAY) return true;
			
			//设置信息
			CMD_S_StopCard StopCard;
			StopCard.wStopUser = wChairID;	

			//庄家停牌
			if(wChairID == m_nBankerStation)
			{			
				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));

				//游戏结束
				OnEventGameEnd(INVALID_CHAIR,NULL,GO_NORMAL_FINISH);
			}
			if(m_cbCardStatus[wChairID] != BRUST_CARD)
				m_cbCardStatus[wChairID] = STOP_CARD;		

			if(!m_cbCutCard[wChairID])
			{		
				StopCard.bDoubleBrsut = m_GameLogic.IsBurst(m_cbTableCardArray[wChairID],m_cbTableCardCount[wChairID]);
			
				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
			}
			else
			{
				if((m_cbCardStatus[wChairID] == STOP_CARD) || (m_cbCardStatus[wChairID] == BRUST_CARD))
				{
					if(m_cbCutCardStatus[wChairID] == INIT_CARD)
					{
						m_cbCutCardStatus[wChairID] = STOP_CARD;
					}
					if(m_cbCutCardStatus[wChairID] != STOP_CARD)
					{
						m_cbCutCardStatus[wChairID] = INIT_CARD;
						m_pITableFrame->SendTableData(wChairID,SUB_S_CUT_CARD_MARK);
					}
				}

				if(m_cbCutCardStatus[wChairID] == STOP_CARD)
				{
					StopCard.bDoubleBrsut = m_GameLogic.IsBurst(m_cbTableCutCardArray[wChairID],m_cbTableCutCardCount[wChairID]);
				
					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
				}	
			}			
			
			//判断是否轮庄家要牌
			WORD wPlayerCount = 0;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{					
				if(m_cbCutCard[wChairID])
				{
					if ((m_cbCardStatus[i] == BRUST_CARD || m_cbCardStatus[i] == NO_CARD    || m_cbCardStatus[i]    == STOP_CARD) &&
					(m_cbCutCardStatus[i] == BRUST_CARD || m_cbCutCardStatus[i] == NO_CARD || m_cbCutCardStatus[i] == STOP_CARD))
						wPlayerCount++;
				}
				else
				{
					if (m_cbCardStatus[i] == BRUST_CARD || m_cbCardStatus[i] == NO_CARD || m_cbCardStatus[i] == STOP_CARD) 
						wPlayerCount++;
				}
			}
			if ((GAME_PLAYER - wPlayerCount) == 1)
			{
				//庄家发牌
				m_bBankerGetCard = true;
				m_pITableFrame->SendTableData(m_nBankerStation,SUB_S_BANKER_SEND_CARD);		
			}			
				
			return true;
		}

	case SUB_C_GET_CARD:	//用户要牌
		{
			//校验数据			
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus != US_PLAY) return true;	

			//获取数据
			CMD_C_GetCard * pGetCard = (CMD_C_GetCard *)pDataBuffer;

			//设置信息
			CMD_S_SendCard SendCard;
			BYTE cbCardCount,cbCutCardCount;	

			//判断要牌
			if(!pGetCard->bCutCard)
			{
				//左手牌
				cbCardCount = ++m_cbTableCardCount[wChairID];
				//是否AI对杀普通玩家
				if(m_pITableFrame->IsAIKillGameUser())
				{
					DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
					m_cbTableCardArray[wChairID][cbCardCount-1] =  GetRandCard();
				}
				else
				{
					m_cbTableCardArray[wChairID][cbCardCount-1] =  GetRandCard();
				}
				
				
				SendCard.cbUserCard   = m_cbTableCardArray[wChairID][cbCardCount-1];
				SendCard.wCurrentUser = m_wCurrentUser;
				SendCard.wGetCardUser = wChairID;
				SendCard.cbCardSum     = m_GameLogic.GetCardSum(m_cbTableCardArray[wChairID],cbCardCount);
				SendCard.bCutCard     = false;
			}
			else
			{
				//右手牌
				GT_ASSERT(m_cbCutCardStatus[wChairID] != NO_CARD);
				
				cbCutCardCount = ++m_cbTableCutCardCount[wChairID];
				m_cbTableCutCardArray[wChairID][cbCutCardCount-1] =  GetRandCard();

				SendCard.cbUserCard   = m_cbTableCutCardArray[wChairID][cbCutCardCount-1];
				SendCard.wCurrentUser = m_wCurrentUser;
				SendCard.wGetCardUser = wChairID;
				SendCard.cbCardSum    = m_GameLogic.GetCardSum(m_cbTableCutCardArray[wChairID],cbCutCardCount);
				SendCard.bCutCard     = true;				
			}
			
			//发送扑克
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

			//玩家爆牌		
			if(m_cbCutCard[wChairID])
			{
				if(m_GameLogic.IsBurst(m_cbTableCardArray[wChairID],m_cbTableCardCount[wChairID]))
				{
					m_cbCardStatus[wChairID] = BRUST_CARD;
					if((m_cbCutCardStatus[wChairID] == CUT_CARD))
					{
						m_pITableFrame->SendTableData(wChairID,SUB_S_CUT_CARD_MARK);
						m_cbCutCardStatus[wChairID] = INIT_CARD;
					}
				}
					
				if(m_GameLogic.IsBurst(m_cbTableCutCardArray[wChairID],m_cbTableCutCardCount[wChairID]))
				{
					m_cbCutCardStatus[wChairID] = BRUST_CARD;
					
					if(m_cbCardStatus[wChairID] == BRUST_CARD)
					{				
						m_wPlayerCount--;
					}

					CMD_S_BrustCard BrustCard;
					BrustCard.wBrustUser = wChairID;

					//发送爆牌消息
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRUST_CARD,&BrustCard,sizeof(BrustCard)); 
				}					
			}
			else if(m_GameLogic.IsBurst(m_cbTableCardArray[wChairID],m_cbTableCardCount[wChairID]))
			{
				m_cbCardStatus[wChairID] = BRUST_CARD;
				m_wPlayerCount--;

				CMD_S_BrustCard BrustCard;
				BrustCard.wBrustUser = wChairID;

				//发送爆牌消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRUST_CARD,&BrustCard,sizeof(BrustCard));  

				//庄家爆牌
				if(wChairID == m_nBankerStation)
				{		
					//游戏结束		
					OnEventGameEnd(INVALID_CHAIR,NULL,GO_NORMAL_FINISH);
					return true;
				}			
			}

			//仅剩庄家
			if (m_wPlayerCount == 1)
			{
				m_cbCardStatus[m_nBankerStation] = STOP_CARD;
				OnEventGameEnd(INVALID_CHAIR,NULL,GO_NORMAL_FINISH);
				return true;
			}		

			WORD wPlayerCount = 0;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{					
				if(m_cbCutCard[i])
				{
					if (((m_cbCardStatus[i] == BRUST_CARD) || (m_cbCardStatus[i]    == STOP_CARD)) &&
					 ((m_cbCutCardStatus[i] == BRUST_CARD) || (m_cbCutCardStatus[i] == STOP_CARD)))
						wPlayerCount++;
				}
				else
				{
					if ((m_cbCardStatus[i] == BRUST_CARD) || (m_cbCardStatus[i] == NO_CARD) || (m_cbCardStatus[i] == STOP_CARD)) 
						wPlayerCount++;
				}
			}

			if ((GAME_PLAYER - wPlayerCount) == 1)
			{
				//庄家发牌
				m_bBankerGetCard = true;
				m_pITableFrame->SendTableData(m_nBankerStation,SUB_S_BANKER_SEND_CARD);		
			}			
			
			return true;
		}	
	case SUB_C_CUT_CARD:	//用户分牌
		{	
			//校验数据			
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus != US_PLAY) return true;
			WORD wChairID = pUserData->UserStatus.wChairID;
			
			GT_ASSERT(m_GameLogic.CanCutCard(m_cbTableCardArray[wChairID][0],m_cbTableCardArray[wChairID][1]));
			if(!m_GameLogic.CanCutCard(m_cbTableCardArray[wChairID][0],m_cbTableCardArray[wChairID][1])) return false;

			//设置信息
			m_cbTableCutCardArray[wChairID][0] = m_cbTableCardArray[wChairID][1];
			m_cbTableCardArray[wChairID][1]    = 0;

			m_cbTableCardCount[wChairID]    = 1;
			m_cbTableCutCardCount[wChairID] = 1;

			CMD_S_CutCard CutCard;
			CutCard.wCutCardUser    = wChairID;
			CutCard.cbFirstCard     = m_cbTableCardArray[wChairID][0];
			CutCard.cbSecondCard    = m_cbTableCutCardArray[wChairID][0];
			CutCard.cbFirstCardSum  = m_GameLogic.GetCardSum(m_cbTableCardArray[wChairID],1);
			CutCard.cbSecondCardSum = m_GameLogic.GetCardSum(m_cbTableCutCardArray[wChairID],1);

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CUT_CARD,&CutCard,sizeof(CutCard));  

			m_cbCutCardStatus[wChairID] = CUT_CARD;
			m_cbCutCard[wChairID]       = true;
		
			return true;
		}

	}
	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, 
											 const void * pDataBuffer, 
											 WORD wDataSize, 
											 IServerUserItem * pIServerUserItem)
{
	return false;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	m_nNotBanker = 0;
	memset(m_fNotBankerScore, 0, sizeof(DOUBLE) * GAME_PLAYER);

	//变量定义
	const tagUserScore * pUserScore=NULL;
	IServerUserItem * pIServerUserItem=NULL;


	//散列扑克
	m_GameLogic.RandCard(m_cbRandCardArray,52);

	//发牌状态	
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		pIServerUserItem = m_pITableFrame->GetServerUserItem(i);

		if (pIServerUserItem != NULL)
		{		
			m_wPlayerCount++;

			m_cbCardStatus[i] = INIT_CARD;
		}
		else
			m_cbCardStatus[i] = NO_CARD;
	}
	
	//庄家位置
	
	for (BYTE i = 0; i < GAME_PLAYER; i++) 
	{
		m_nBankerStation = (m_nBankerStation + 1) % GAME_PLAYER ;
		if (m_cbCardStatus[m_nBankerStation] == INIT_CARD)
		{
			GT_ASSERT(m_pITableFrame->GetServerUserItem(m_nBankerStation) != NULL);
			break;		
		}
	}
	

	//计算限注
	bool bCalculateGold = false;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if(m_nBankerStation == i) continue;

		pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem != NULL)
		{
			if (bCalculateGold == false)
			{
				bCalculateGold = true;
				pUserScore = pIServerUserItem->GetUserScore();
				GT_ASSERT(pUserScore->fScore > pGameServiceOption->dwCellScore);
				m_fMaxGold = pUserScore->fScore;
			}
			else 
			{
				pUserScore = pIServerUserItem->GetUserScore();
				GT_ASSERT(pUserScore->fScore > 1L);
				m_fMaxGold = __min(m_fMaxGold, pUserScore->fScore);
			}
		}
	}

	pIServerUserItem = m_pITableFrame->GetServerUserItem(m_nBankerStation);
	GT_ASSERT(pIServerUserItem != NULL);
	pUserScore = pIServerUserItem->GetUserScore();
	m_fMaxGold = __min(m_fMaxGold,pUserScore->fScore / m_wPlayerCount);
	m_fMaxGold = __min(m_fMaxGold,pGameServiceOption->dwHighScore);

	//发送开始
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	
	GameStart.wBankerStation  = m_nBankerStation;
	GameStart.wCurrentUser    = m_wCurrentUser;
	GameStart.fMaxGold        = m_fMaxGold;
	GameStart.fTurnBasicGold  = m_fTurnBasicGold;


	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbCardStatus[i] ==INIT_CARD)
		{
			m_fTableGold[2*i] = m_fTurnBasicGold;
			m_pITableFrame->SendTableData(i,
				                          SUB_S_GAME_START,
										  &GameStart,
										  sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i, 
			                           SUB_S_GAME_START, 
									   &GameStart, 
									   sizeof(GameStart));
	}
	//投注
	tagBetScoreInfo BetScoreInfo;
	BetScoreInfo.dwBetScore = m_fTurnBasicGold;
	BetScoreInfo.wBetRegionIndex = 0;
	for(int i = 0; i < m_pGameServiceAttrib->wChairCount; i ++)
	{
		if(m_nBankerStation == i) continue;
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if(pIServerUserItem != NULL && m_cbCardStatus[i] == INIT_CARD)
		{
			BetScoreInfo.pIServerUserItem = pIServerUserItem;
			if(m_pICalculateFrame->BetScore(BetScoreInfo.pIServerUserItem, &BetScoreInfo, 1) == false)//闲
			{
				return false;
			}

			BetScoreInfo.pIServerUserItem = m_pITableFrame->GetServerUserItem(m_nBankerStation);
			if(m_pICalculateFrame->BetScore(BetScoreInfo.pIServerUserItem, &BetScoreInfo, 1) == false)//庄
			{
				return false;
			}
		}
	}

	//设置状态
	m_pITableFrame->SetGameStatus(GS_ADDGOLDING);

	return true;
}


//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, 
											 IServerUserItem * pIServerUserItem, 
											 BYTE cbReason)
{	



	switch(cbReason)
	{
	case GER_USER_LEFT:			//用户强退
		{	
			//强退处理
			if(wChairID == m_nBankerStation)
			{			
				 return OnUserLeft(m_bAddGoldEnd);			
			}
			else
			{	
				return OnUserLeft(wChairID,m_bAddGoldEnd);					
			}			
			return true;
		}				
	case GO_NORMAL_FINISH: 
		{	
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));	

			DOUBLE	fBankerScore = 0;

			GameEnd.wBankerStation = m_nBankerStation;
			CopyMemory(GameEnd.cbPlayStatus,m_cbCardStatus,sizeof(m_cbCardStatus));
			CopyMemory(GameEnd.fTableGold,m_fTableGold,sizeof(m_fTableGold));

			//设置扑克
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if(m_pITableFrame->GetServerUserItem(i) != NULL)
				{
					GameEnd.cbTableCardCount[i] = m_cbTableCardCount[i];
					GameEnd.cbTableCardArray[i][0] = m_cbTableCardArray[i][0];
					CopyMemory(&GameEnd.cbTableCardArray[i][1],&m_cbTableCardArray[i][1],(m_cbTableCardCount[i]-1)*sizeof(BYTE));
				}
			}

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//底牌信息
				if ((m_cbCardStatus[i] == NO_CARD)) GameEnd.cbUserCard[i]=0;
				else GameEnd.cbUserCard[i] = m_cbTableCardArray[i][0];	
				if(!m_cbCutCard)GameEnd.cbCutCard[i] = 0;
				else GameEnd.cbCutCard[i] = m_cbTableCutCardArray[i][0];

				if(m_nBankerStation == i) continue;

				//统计成绩
				if(m_cbCardStatus[i] != NO_CARD)
				{
					IServerUserItem * pIUserItem = m_pITableFrame->GetServerUserItem(i);
					GT_ASSERT(pIUserItem != NULL);	

					DOUBLE fGameGold = 0, fBankerScore = 0;
					bool bCutCard  = false;

					BYTE cbCardStatus = m_cbCardStatus[i];
					CalcScore(cbCardStatus, i, fGameGold, fBankerScore, bCutCard);

					GameEnd.fGameGold[i]  = fGameGold;
					m_fBankerScore       += fBankerScore;

					if(m_cbCutCard[i])
					{						
						fGameGold    = 0.0; 
						fBankerScore = 0.0;
						bCutCard     = true;

						cbCardStatus = m_cbCutCardStatus[i];
						CalcScore(cbCardStatus, i, fGameGold, fBankerScore, bCutCard);

						GameEnd.fGameGold[i] += fGameGold;
						m_fBankerScore       += fBankerScore;	
					}									
				}	
				else
				{
					GameEnd.fGameGold[i] = -m_fNotBankerScore[i];
				}
			}	

			//额度类型
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardSum[i]    = m_GameLogic.GetCardSum(m_cbTableCardArray[i],m_cbTableCardCount[i]);
				GameEnd.cbCutCardSum[i] = m_GameLogic.GetCardSum(m_cbTableCutCardArray[i],m_cbTableCutCardCount[i]);

			}	

			
			//庄家额度
			GameEnd.fGameGold[m_nBankerStation]   = m_fBankerScore;
			//辅助变量
			m_bBankerWin = GameEnd.fGameGold[m_nBankerStation] > 0 ? true : false;

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				//修改额度
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd,sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);

				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//结束游戏
				m_pITableFrame->ConcludeGame();
			}
			else
			{
				GT_ASSERT(FALSE);
				//结束游戏
				m_pITableFrame->ConcludeGame();
				return false;
			}

		
			break;
		}	
	}
	return true;
}

//计算成绩
bool CTableFrameSink::CalcScore(BYTE cbCardStatus,WORD wChairID,
								DOUBLE &fGameGold,  DOUBLE &fBankerScore,
								bool bCutCard)
{
	//爆牌玩家
	if ((cbCardStatus == BRUST_CARD)) 
	{
		fGameGold    =  -GetLastGold(wChairID);
		fBankerScore = GetLastGold(wChairID); 
		return true;
	}

	GT_ASSERT((cbCardStatus == STOP_CARD) || (cbCardStatus == DOUBLE_GOLD));

	//庄家爆牌
	if(m_cbCardStatus[m_nBankerStation] == BRUST_CARD)
	{						
		fGameGold     = GetLastGold(wChairID);
		fBankerScore -= GetLastGold(wChairID); 
		return true;
	}
	//庄家停牌
	else
	{	
		BYTE bWin = 0;
		//推断胜者
		if(bCutCard)
		{
			bWin = m_GameLogic.DeduceWiner(m_cbTableCardArray[m_nBankerStation],
				m_cbTableCardCount[m_nBankerStation],
				m_cbTableCutCardArray[wChairID],
				m_cbTableCutCardCount[wChairID]);
		}
		else
		{
			bWin = m_GameLogic.DeduceWiner(m_cbTableCardArray[m_nBankerStation],
				m_cbTableCardCount[m_nBankerStation],
				m_cbTableCardArray[wChairID],
				m_cbTableCardCount[wChairID]);
		}		
		switch (bWin)
		{
			//平牌
		case 0:
			{
				fGameGold     = -GetLastGold(wChairID);
				fBankerScore  = GetLastGold(wChairID);	

				break;
			}
			//庄家胜
		case 1:
			{
				fGameGold     = -GetLastGold(wChairID);
				fBankerScore  = GetLastGold(wChairID);							

				break;
			}
			//闲家胜
		case 2:
			{
				fGameGold     = GetLastGold(wChairID);					
				fBankerScore  = -GetLastGold(wChairID);

				break;
			}
		}
	}	
	return true;
}

//闲家强退
bool CTableFrameSink::OnUserLeft(WORD wChairID,bool bAddGoldEnd)
{
	//定义变量
	CMD_S_GameEnd GameEnd;
	memset(&GameEnd,0,sizeof(GameEnd));	
	GameEnd.wBankerStation = m_nBankerStation;
	CopyMemory(GameEnd.cbPlayStatus,m_cbCardStatus,sizeof(m_cbCardStatus));
	CopyMemory(GameEnd.fTableGold,m_fTableGold,sizeof(m_fTableGold));

	//设置扑克
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if(m_pITableFrame->GetServerUserItem(i) != NULL)
		{
			GameEnd.cbTableCardCount[i] = m_cbTableCardCount[i];
			GameEnd.cbTableCardArray[i][0] = m_cbTableCardArray[i][0];
			if(m_cbTableCardCount[i] > 0)
				CopyMemory(&GameEnd.cbTableCardArray[i][1],&m_cbTableCardArray[i][1],(m_cbTableCardCount[i]-1)*sizeof(BYTE));
		}
	}

	m_cbCardStatus[wChairID] = NO_CARD;
	m_wPlayerCount--;

	//判断结束
	WORD wPlayerCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);

		if (pIServerUserItem != NULL && m_cbCardStatus[i] == INIT_CARD)
		{		
			wPlayerCount++;
		}
	}

	//修改额度
	if(!m_bAddGoldEnd)	
	{
		m_nNotBanker ++;
		m_fNotBankerScore[wChairID] = m_fTurnBasicGold;
		m_fBankerScore      += m_fTurnBasicGold;


	}
	else
	{
		m_nNotBanker ++;
		//修改额度	
		m_fNotBankerScore[wChairID] = GetLastGold(wChairID);
		m_fBankerScore      += GetLastGold(wChairID);

	}
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//底牌信息
		if ((m_cbCardStatus[i] == NO_CARD)) GameEnd.cbUserCard[i]=0;
		else GameEnd.cbUserCard[i] = m_cbTableCardArray[i][0];	
		if(!m_cbCutCard)GameEnd.cbCutCard[i] = 0;
		else GameEnd.cbCutCard[i] = m_cbTableCutCardArray[i][0];		
		GameEnd.cbCardSum[i]    = m_GameLogic.GetCardSum(m_cbTableCardArray[i],m_cbTableCardCount[i]);
		GameEnd.cbCutCardSum[i] = m_GameLogic.GetCardSum(m_cbTableCutCardArray[i],m_cbTableCutCardCount[i]);
	}	



	if(wPlayerCount >= 2)
	{
		return true;
	}
	//仅剩庄家
	else 
	{

		//庄家额度		
		for(int i = 0; i < GAME_PLAYER; i++)
		{
			if(!m_bAddGoldEnd) 
			{
				GameEnd.fGameGold[i] = -m_fTurnBasicGold;

			}
			else 
			{
/*				if(i != m_nBankerStation)
				{
					IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);

					//投注
					tagBetScoreInfo BetScoreInfo;
					BetScoreInfo.dwBetScore = m_fNotBankerScore[i] - m_fTurnBasicGold;
					BetScoreInfo.wBetRegionIndex = 0;
					BetScoreInfo.pIServerUserItem = pIServerUserItem;
					if(m_pICalculateFrame->BetScore(BetScoreInfo.pIServerUserItem, &BetScoreInfo, 1) == false)//闲家
					{
						GT_ASSERT(FALSE);
					}
					BetScoreInfo.pIServerUserItem = m_pITableFrame->GetServerUserItem(m_nBankerStation);//庄家
					if(m_pICalculateFrame->BetScore(BetScoreInfo.pIServerUserItem, &BetScoreInfo, 1) == false)
					{
						GT_ASSERT(FALSE);
					}
				}*/

				GameEnd.fGameGold[i] = -m_fNotBankerScore[i];
			}
		}

		GameEnd.fGameGold[m_nBankerStation] = m_fBankerScore;

		if(m_CalculateSink.InitCalculateContext(&GameEnd))
		{
			//修改额度
			m_pITableFrame->WriteGameEnd(GER_USER_LEFT,(BYTE*)&GameEnd,sizeof(GameEnd), 
				m_CalculateSink.GetCalculateContext(),&m_CalculateSink);

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame();
		}
		else
		{
			GT_ASSERT(FALSE);
			//结束游戏
			m_pITableFrame->ConcludeGame();
			return false;
		}
	
	}
	return true;
}

//庄家强退
bool CTableFrameSink::OnUserLeft(bool bAddGoldEnd)
{
	//定义变量
	CMD_S_GameEnd GameEnd;
	memset(&GameEnd,0,sizeof(GameEnd));	

	GameEnd.wBankerStation = m_nBankerStation;
	CopyMemory(GameEnd.cbPlayStatus,m_cbCardStatus,sizeof(m_cbCardStatus));
	CopyMemory(GameEnd.fTableGold,m_fTableGold,sizeof(m_fTableGold));

	//设置扑克
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if(m_pITableFrame->GetServerUserItem(i) != NULL)
		{
			GameEnd.cbTableCardCount[i] = m_cbTableCardCount[i];
			GameEnd.cbTableCardArray[i][0] = m_cbTableCardArray[i][0];
			if(m_cbTableCardCount[i] > 0)
				CopyMemory(&GameEnd.cbTableCardArray[i][1],&m_cbTableCardArray[i][1],(m_cbTableCardCount[i]-1)*sizeof(BYTE));
		}
	}

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//底牌信息
		if ((m_cbCardStatus[i] == NO_CARD)) GameEnd.cbUserCard[i]=0;
		else GameEnd.cbUserCard[i] = m_cbTableCardArray[i][0];	
		if(!m_cbCutCard)GameEnd.cbCutCard[i] = 0;
		else GameEnd.cbCutCard[i] = m_cbTableCutCardArray[i][0];		
		GameEnd.cbCardSum[i]    = m_GameLogic.GetCardSum(m_cbTableCardArray[i],m_cbTableCardCount[i]);
		GameEnd.cbCutCardSum[i] = m_GameLogic.GetCardSum(m_cbTableCutCardArray[i],m_cbTableCutCardCount[i]);
	}	
	//发送消息
	//	CMD_S_UserLeft UserLeft;
	//	UserLeft.wUserChairID = m_nBankerStation;

	//	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_LEFT,&UserLeft,sizeof(UserLeft));
	//	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_LEFT,&UserLeft,sizeof(UserLeft));

	//修改额度
	if(!m_bAddGoldEnd)
	{	
	
		int nCurUserCount = 0;
		GameEnd.fGameGold[m_nBankerStation] = -m_fMaxGold;	
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_nBankerStation == i) continue;
			IServerUserItem * pIUserItem=m_pITableFrame->GetServerUserItem(i);
			if(pIUserItem!=NULL)
			{
				nCurUserCount ++;
			}
		}	
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_nBankerStation == i) continue;
			IServerUserItem * pIUserItem=m_pITableFrame->GetServerUserItem(i);
			if(pIUserItem!=NULL)
			{
				GameEnd.fGameGold[i] = - GameEnd.fGameGold[m_nBankerStation]/nCurUserCount;
			}
		}	

		if(m_CalculateSink.InitCalculateContext(&GameEnd))
		{
			//修改额度
			m_pITableFrame->WriteGameEnd(GER_USER_LEFT,(BYTE*)&GameEnd,sizeof(GameEnd), 
				m_CalculateSink.GetCalculateContext(),&m_CalculateSink);

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_bBankerWin = false;

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
		else
		{
			m_bBankerWin = false;
			GT_ASSERT(FALSE);
			//结束游戏
			m_pITableFrame->ConcludeGame();
			return false;
		}
	}
	else
	{
		DOUBLE fBankerScore = 0;
		int nCurUserCount = 0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//底牌信息
			if ((m_cbCardStatus[i] == NO_CARD)) GameEnd.cbUserCard[i]=0;
			else GameEnd.cbUserCard[i] = m_cbTableCardArray[i][0];

			if(m_nBankerStation == i) continue;

			//统计成绩
			if(m_cbCardStatus[i] != NO_CARD)
			{
				IServerUserItem * pIUserItem=m_pITableFrame->GetServerUserItem(i);
				GT_ASSERT(pIUserItem!=NULL);					

				GameEnd.fGameGold[i] = GetLastGold(i);		

				fBankerScore += -GetLastGold(i)	;

				nCurUserCount ++;
			}	
			else
			{
				GameEnd.fGameGold[i] = -m_fNotBankerScore[i];
				fBankerScore += m_fNotBankerScore[i]	;
			}
		}	

		GameEnd.fGameGold[m_nBankerStation]   = fBankerScore;		

		if(m_CalculateSink.InitCalculateContext(&GameEnd))
		{
			//修改额度
			m_pITableFrame->WriteGameEnd(GER_USER_LEFT,(BYTE*)&GameEnd,sizeof(GameEnd), 
				m_CalculateSink.GetCalculateContext(),&m_CalculateSink);

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_bBankerWin = false;

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
		else
		{
			m_bBankerWin = false;
			GT_ASSERT(FALSE);
			//结束游戏
			m_pITableFrame->ConcludeGame();
			return false;
		}


	}

	return true;
}

//获取扑克
BYTE CTableFrameSink::GetRandCard()
{
	m_cbSendCardCount++;
	return 	m_cbRandCardArray[m_cbSendCardCount];
}
DOUBLE CTableFrameSink::GetLastGold(WORD wChairID)
{
	return m_fTableGold[wChairID * 2] + m_fTableGold[wChairID * 2 + 1];
}
//////////////////////////////////////////////////////////////////////////

