#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式

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
	VarDecFromR8(m_pGameEnd->fGameScore[wChairID], &decTemp);
	VarDecAdd(&decTemp, &pPartedWinScore[0], &pPartedWinScore[0]);

	//判断大于0,进行抽水结算
	if(VarDecCmpR8(&decTemp, 0) == VARCMP_GT)
	{
		//抽水 = 总派彩 * (- 抽水率)
		VarDecMul(&decTemp,(DECIMAL*) &m_decAfterTax, &pPartedTaxScore[0]);
		//更新Double派彩
		VarDecAdd(&decTemp,&pPartedTaxScore[0], &decTemp);
		VarR8FromDec(&decTemp, &m_pGameEnd->fGameScore[wChairID]);
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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//历史额度
	memset(m_fAllTurnScore,0,sizeof(m_fAllTurnScore));
	memset(m_fLastTurnScore,0,sizeof(m_fLastTurnScore));

	//Run信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//扑克信息
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pICalculateFrame=NULL;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
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
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//Run信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//扑克信息
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

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
	return true;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_RF_PLAYING);

	//混乱扑克
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=13;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//设置用户
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=0;j<m_bCardCount[i];j++)
		{
			if (m_bHandCardData[i][j]==0x03) 
			{
				//设置用户
				m_wBankerUser=i;
				m_wCurrentUser=i;

				goto SET_CURRENT_USER;
			}
		}
	}

	//错误判断
	GT_ASSERT(FALSE);
	SET_CURRENT_USER:

	//发送扑克
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(GameStart.cbCardData));
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
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

	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			//效验参数
			GT_ASSERT(pIServerUserItem!=NULL);
			GT_ASSERT(wChairID<m_wPlayerCount);

			//构造数据
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//常规结束
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//统计牌分
			DOUBLE fCardScore[GAME_PLAYER];
			DOUBLE fCellScore=m_pGameServiceOption->dwCellScore;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				DOUBLE fScoreTimes=1.0;
				if (m_bCardCount[i]==13) fScoreTimes=4.0;
				else if (m_bCardCount[i]>=10) fScoreTimes=3.0;
				else if (m_bCardCount[i]>=8) fScoreTimes=2.0;
				fCardScore[i]=fScoreTimes*m_bCardCount[i]*fCellScore;
			}

			//统计额度
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				for (WORD j=0;j<m_wPlayerCount;j++)
				{
					if (j==i) continue;
					GameEnd.fGameScore[i]+=fCardScore[j]-fCardScore[i];
				}
			}

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd, sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);

				//额度统计
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					m_fAllTurnScore[i]+=GameEnd.fGameScore[i];
					m_fLastTurnScore[i]=GameEnd.fGameScore[i];
				}
			
				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//结束游戏
				m_pITableFrame->ConcludeGame();

				return true;
			}
			else
			{
				GT_ASSERT(FALSE);
				//结束游戏
				m_pITableFrame->ConcludeGame();
				return false;
			}
		}
	case GER_USER_LEFT:		//用户强退
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//剩余扑克
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//统计额度
			DOUBLE fCardScore[GAME_PLAYER];
			memset(fCardScore, 0, sizeof(DOUBLE)*GAME_PLAYER);
			fCardScore[wChairID]=4.0 * 13 *m_pGameServiceOption->dwCellScore;//一家包三家，十三太保

			//统计额度
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				for (WORD j=0;j<m_wPlayerCount;j++)
				{
					if (j==i) continue;
					GameEnd.fGameScore[i]+=fCardScore[j]-fCardScore[i];
				}
			}

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_USER_LEFT,(BYTE*)&GameEnd, sizeof(GameEnd),
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
				GT_ASSERT(FALSE);
				//结束游戏
				m_pITableFrame->ConcludeGame();
				return false;
			}
		}
	}

	GT_ASSERT(FALSE);

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_RF_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//设置变量
			StatusFree.fBaseScore=m_pGameServiceOption->dwCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//设置变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.fBaseScore=m_pGameServiceOption->dwCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);

			//历史额度
			CopyMemory(StatusPlay.fAllTurnScore,m_fAllTurnScore,sizeof(m_fAllTurnScore));
			CopyMemory(StatusPlay.fLastTurnScore,m_fLastTurnScore,sizeof(m_fLastTurnScore));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

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
	WORD wChiarID = pIServerUserItem->GetChairID();
	if(wChiarID == INVALID_CHAIR)
		return false;
	
	//先出牌者
	if (m_bTurnCardCount==0)
	{
		//首出判断
		BYTE cbOutCard=m_bHandCardData[wChiarID][m_bCardCount[wChiarID]-1];
		if ((m_wBankerUser==wChiarID)&&(m_bCardCount[m_wBankerUser]==13)) cbOutCard=0x03;

		//发送数据
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=cbOutCard;
		OnGameMessage(SUB_C_OUT_CART,
			&OutCard,
			sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE), pIServerUserItem);

	}
	else
	{
		BYTE cbCardCount = 0;//扑克数目
		BYTE cbResultCard[13];
		memset(cbResultCard, 0, sizeof(BYTE)*13);

		BYTE bTurnOutType=m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount);

		cbCardCount = m_GameLogic.SearchOutCard(m_bHandCardData[wChiarID],
			m_bCardCount[wChiarID],
			m_bTurnCardData,
			m_bTurnCardCount,
			bTurnOutType,
			cbResultCard);

		if(cbCardCount > 0)
		{
			//发送数据
			CMD_C_OutCard OutCard;
			OutCard.bCardCount=cbCardCount;
			memcpy(OutCard.bCardData, cbResultCard, cbCardCount);

			OnGameMessage(SUB_C_OUT_CART,
				&OutCard,
				sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE), pIServerUserItem);

		}
		else
		{
			OnGameMessage(SUB_C_PASS_CARD,
				NULL,
				0, pIServerUserItem);

		}
	}
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
	case SUB_C_OUT_CART:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//效验数据
			GT_ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			GT_ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserPassCard(pUserData->UserStatus.wChairID);
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	BYTE i = 0;
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;


	//类型判断
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//首出判断
	if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==13))
	{
		for ( i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x03) break;
		}
		if (i==bCardCount) return false;
	}

	//更随出牌
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//大牌判断
	bool bMostest=false;
	if ((bCardCount<=3)&&(bCardData[0]==0x32)) bMostest=true;

	//出牌记录
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//切换用户
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bMostest==false) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//发送数据
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//出牌最大
	if (bMostest==true) m_bTurnCardCount=0;

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//设置变量
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//发送数据
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//////////////////////////////////////////////////////////////////////////
