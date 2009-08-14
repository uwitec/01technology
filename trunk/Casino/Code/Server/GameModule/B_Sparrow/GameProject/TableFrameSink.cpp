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
	GT_ASSERT(m_pTableFrameSink != NULL);
	bool bFlag = false;
	//判断荒庄
	if(m_pTableFrameSink->EstimateNullChiHuCard())
	{
		bFlag = true;
	}
	else
	{
		/*
		//判断吃胡
		if((m_pTableFrameSink->IsZhuaPaoChiHu()))
		{
			if(m_pTableFrameSink->IsValidBetScoreUser(pItem->GetUserData()->UserData.dwUserID)==false)
				bFlag = true;
		}
		*/
	}
	if(bFlag)
	{
		for(WORD i = 0; i < wBetRegionCount; i ++)
		{
			VarDecSub(&pPartedBetScore[i],&pPartedBetScore[i],&pPartedBetScore[i]);
		}
		VarDecSub(pValidBetScore,pValidBetScore,pValidBetScore);
	}
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
	m_wSiceCount=MAKEWORD(1,1);
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//Run变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

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
	QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
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
	m_wSiceCount=MAKEWORD(1,1);
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//Run变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

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
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//混乱扑克
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//动作分析
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//特殊胡牌
		//m_cbUserAction[i]|=m_GameLogic.EstimateChiHu(m_cbCardIndex[i]);

		//庄家判断
		if (i==m_wBankerUser)
		{
			//杠牌判断
			tagGangCardResult GangCardResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			//胡牌判断
			tagChiHuResult ChiHuResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult);
		}

		//状态设置
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;
			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}
	}

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//设置变量
		GameStart.cbUserAction=m_cbUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//发送数据
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
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			DOUBLE fCellScore=m_pGameServiceOption->dwCellScore;
			BYTE cbBirdValue=(m_cbRepertoryCard[0]&MASK_VALUE);

			//结束信息
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			for (WORD i=0;i<m_wPlayerCount;i++) GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;

			//变量定义
			WORD wChiHuUserCount=0;

			//统计额度
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//自摸类型
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)
				{
					//翻数计算
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);

					//循环累计
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//赢家过滤
						if (i==m_wProvideUser) continue;

						//额度统计
						GameEnd.fGameScore[i] -= cbChiHuOrder * fCellScore;
						GameEnd.fGameScore[m_wProvideUser] += cbChiHuOrder * fCellScore;
					}

					//胡牌数目
					wChiHuUserCount=1;

					//庄家设置
					m_wBankerUser=m_wProvideUser;
				}

				//捉炮类型
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL)
				{
					//变量定义
					WORD wNextBankerUser=INVALID_CHAIR;

					//循环累计
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//输家过滤
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

						//翻数计算
						BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);

						//额度统计
						GameEnd.fGameScore[i] += cbChiHuOrder * fCellScore;
						GameEnd.fGameScore[m_wProvideUser] -= cbChiHuOrder * fCellScore;

						//设置变量
						wNextBankerUser=i;
						wChiHuUserCount++;
					}

					//庄家设置
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
				}

				//杠牌计算
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					for(WORD j=0; j < m_cbWeaveItemCount[i]; j ++)
					{
						if (m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG)//杠牌
						{
							if(m_WeaveItemArray[i][j].wProvideUser == i)//自杠
							{
								if(m_WeaveItemArray[i][j].cbPublicCard == FALSE)
								{
									//暗杠
									for (WORD k=0;k<m_wPlayerCount;k++)
									{
										if(i == k)continue;
										GameEnd.fGameScore[k] -= fCellScore;
										GameEnd.fGameScore[i] += fCellScore;
									}
								}
								else
								{	//点杠
									for (WORD k=0;k<m_wPlayerCount;k++)
									{
										if(i == k)continue;
										GameEnd.fGameScore[k] -= fCellScore/2;
										GameEnd.fGameScore[i] += fCellScore/2;
									}
								}
							}
							else
							{
								//他杠
								GameEnd.fGameScore[m_WeaveItemArray[i][j].wProvideUser] -= fCellScore;
								GameEnd.fGameScore[i] += fCellScore;
							}
						}
					}
				}
			}
			else if ((m_cbOutCardCount==0)&&(m_cbSendCardCount==1))
			{
				//变量定义
				WORD wNextBankerUser=INVALID_CHAIR;

				//特殊胡牌
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//输家过滤
					if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

					//翻数计算
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);

					//循环累计
					if ((m_ChiHuResult[i].wChiHuKind&0x00F0)!=0)
					{
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_ChiHuResult[j].wChiHuKind==CHK_NULL)
							{
								GameEnd.fGameScore[j] -= cbChiHuOrder * fCellScore;
								GameEnd.fGameScore[i] += cbChiHuOrder * fCellScore;
							}
						}
					}
					else
					{
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_ChiHuResult[j].wChiHuKind==CHK_NULL)
							{
								GameEnd.fGameScore[j] -= cbChiHuOrder * fCellScore;
								GameEnd.fGameScore[i] += cbChiHuOrder * fCellScore;
							}
						}
					}

					//设置变量
					wChiHuUserCount++;
					wNextBankerUser=i;
				}

				//庄家设置
				if (wChiHuUserCount==1) m_wBankerUser=wNextBankerUser;

				//杠牌计算
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					for(WORD j=0; j < m_cbWeaveItemCount[i]; j ++)
					{
						if (m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG)//杠牌
						{
							if(m_WeaveItemArray[i][j].wProvideUser == i)//自杠
							{
								if(m_WeaveItemArray[i][j].cbPublicCard == FALSE)
								{
									//暗杠
									for (WORD k=0;k<m_wPlayerCount;k++)
									{
										if(i == k)continue;
										GameEnd.fGameScore[k] -= fCellScore;
										GameEnd.fGameScore[i] += fCellScore;
									}
								}
								else
								{	//点杠
									for (WORD k=0;k<m_wPlayerCount;k++)
									{
										if(i == k)continue;
										GameEnd.fGameScore[k] -= fCellScore/2;
										GameEnd.fGameScore[i] += fCellScore/2;
									}
								}
							}
							else
							{
								//他杠
								GameEnd.fGameScore[m_WeaveItemArray[i][j].wProvideUser] -= fCellScore;
								GameEnd.fGameScore[i] += fCellScore;
							}
						}
					}
				}
			}
			else
			{
				//荒庄处理
				GameEnd.cbChiHuCard=0;
			}

			GameEnd.wSiceCount=m_wSiceCount;
			GameEnd.wBankerUser=m_wBankerUser;
			//组合扑克
			CopyMemory(GameEnd.WeaveItemArray,m_WeaveItemArray,sizeof(tagWeaveItem) * GAME_PLAYER * 4);
			CopyMemory(GameEnd.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			//拷贝扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

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
				GT_ASSERT(FALSE);
				//结束游戏
				m_pITableFrame->ConcludeGame();
				return false;
			}

			return true;
		}
	case GER_USER_LEFT:		//用户强退
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.fGameScore[wChairID]=-(int)(6*m_pGameServiceOption->dwCellScore);
			//统计额度
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i) continue;
				GameEnd.fGameScore[i]=(int)(2*m_pGameServiceOption->dwCellScore);
				
			}
			GameEnd.wSiceCount=m_wSiceCount;
			GameEnd.wBankerUser=m_wBankerUser;
			//组合扑克
			CopyMemory(GameEnd.WeaveItemArray,m_WeaveItemArray,sizeof(tagWeaveItem) * GAME_PLAYER * 4);
			CopyMemory(GameEnd.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			//拷贝扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
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

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.fCellScore=m_pGameServiceOption->dwCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY://游戏状态
		{
		}
	case GS_MJ_HAI_DI:	//海底状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.fCellScore=m_pGameServiceOption->dwCellScore;

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(tagWeaveItem) * GAME_PLAYER * 4);
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],StatusPlay.cbCardData);

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	default:
		break;
	}

	return false;
}
//人工智能游戏动作
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	WORD wCurrentUser = m_wCurrentUser;
	if(m_wCurrentUser == INVALID_CHAIR)
	{
		//随即机器人进行操作
		WORD wActionUser[GAME_PLAYER];
		BYTE bActionUserCount = 0;
		for(BYTE i = 0; i < GAME_PLAYER; i ++)
		{
			BYTE cbActionMask=(m_bResponse[i]==false)?m_cbUserAction[i]:WIK_NULL;
			if(cbActionMask!=WIK_NULL)
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
		if(bActionUserCount)
		{
			wCurrentUser = rand() % bActionUserCount;
			wCurrentUser = wActionUser[wCurrentUser];
		}
		else
			return true;
	}
	//机器人判断
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wCurrentUser);
	if(pIServerUserItem == NULL || 
		pIServerUserItem->IsAIUser() == false || 
		pIServerUserItem->GetUserStatus() != US_PLAY)
		return false;
	WORD wChairID = pIServerUserItem->GetChairID();
	if(wChairID == INVALID_CHAIR)
		return false;

	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	switch(cbGameStatus) 
	{
	case GS_MJ_PLAY://游戏状态
	case GS_MJ_HAI_DI://海底状态
		{
			BYTE cbActionCard=m_cbProvideCard;
			BYTE cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;
			//判断吃碰杠操作
			if(cbActionMask!=WIK_NULL)
			{
				//变量定义
				tagGangCardResult GangCardResult;
				ZeroMemory(&GangCardResult,sizeof(GangCardResult));

				//杠牌判断
				if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
				{
					//桌面杆牌
					if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
					{
						GangCardResult.cbCardCount=1;
						GangCardResult.cbCardData[0]=cbActionCard;
					}

					//自己杆牌
					if ((m_wCurrentUser==wChairID)||(cbActionCard==0))
					{
						m_GameLogic.AnalyseGangCard(m_cbCardIndex[wChairID],
							m_WeaveItemArray[wChairID],
							m_cbWeaveItemCount[wChairID],
							GangCardResult);
					}
				}
				
				//执行吃碰杠操作
				AI_AutomatismOperate(wChairID, pIServerUserItem, cbActionCard,cbActionMask,GangCardResult);
			
			}
			else
			{
				//出牌
				if(m_wCurrentUser != INVALID_CHAIR)
					AI_AutomatismOutCard(wChairID, pIServerUserItem);
			}
		}
		break;
	default:
		break;
	}
	return true;
}
//用户出牌
bool CTableFrameSink::AI_AutomatismOutCard(WORD wChairID, IServerUserItem *pIServerUserItem)
{
	BYTE cbCardData[14];
	BYTE cbCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],cbCardData);
	GT_ASSERT(cbCardCount > 0);
	if(cbCardCount == 0)
		return false;
	//是否AI对杀普通玩家
	if(m_pITableFrame->IsAIKillGameUser())
	{
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();

		//1 选找其他机器人需要的单牌
		BYTE cbSingleCardData[14];
		int  nSingleCardDataCount = 0;
		for(BYTE i = 0; i < cbCardCount; i ++)
		{
			if(m_GameLogic.EstimateEatCard(m_cbCardIndex[wChairID], cbCardData[i]) == WIK_NULL &&
				m_GameLogic.EstimatePengCard(m_cbCardIndex[wChairID], cbCardData[i]) == WIK_NULL &&
				m_GameLogic.EstimateGangCard(m_cbCardIndex[wChairID], cbCardData[i]) == WIK_NULL)
			{
				//记录单牌
				cbSingleCardData[nSingleCardDataCount ++] = cbCardData[i];

				//寻找最匹配牌的机器人
				WORD wBestMachChairID = AI_BestMachChairID(wChairID, wGameUser, cbCardData[i]);
				if(wBestMachChairID != INVALID_CHAIR)
				{
					//发送数据
					CMD_C_OutCard OutCard;
					OutCard.cbCardData=cbCardData[i];
					OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

					return true;

				}
			}

		}
		//2 随机出牌
		if(nSingleCardDataCount > 0)
		{
			for(BYTE i = 0; i < nSingleCardDataCount; i ++)
			{
				//牌型权位
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

				//判断普通玩家杠牌或者胡牌
				tagChiHuResult ChiHuResult;
				if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], cbSingleCardData[i]) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
					m_WeaveItemArray[wGameUser],
					m_cbWeaveItemCount[wGameUser],
					cbSingleCardData[i],
					wChiHuRight,
					ChiHuResult) != WIK_NULL)
				{
					//机器人智商-相当几率给普通玩家杠牌或者胡牌
					if(CAIUtil::DetectionProbability(100 - m_pGameServiceOption->cbAILevel))
					{	
						//发送数据
						CMD_C_OutCard OutCard;
						OutCard.cbCardData=cbSingleCardData[i];
						OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

						return true;
					}
				}
				else
				{
					//发送数据
					CMD_C_OutCard OutCard;
					OutCard.cbCardData=cbSingleCardData[i];
					OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

					return true;
				}
			}
		}
		//3 选找出牌
		memcpy(cbSingleCardData, cbCardData, sizeof(cbCardData));
		nSingleCardDataCount = cbCardCount;
		while(nSingleCardDataCount > 0)
		{
			i = rand() % nSingleCardDataCount;
			//牌型权位
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

			//判断普通玩家杠牌或者胡牌
			tagChiHuResult ChiHuResult;
			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], cbSingleCardData[i]) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
				m_WeaveItemArray[wGameUser],
				m_cbWeaveItemCount[wGameUser],
				cbSingleCardData[i],
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				//机器人智商-相当几率给普通玩家杠牌或者胡牌
				if(CAIUtil::DetectionProbability(100 - m_pGameServiceOption->cbAILevel))
				{	
					//发送数据
					CMD_C_OutCard OutCard;
					OutCard.cbCardData=cbSingleCardData[i];
					OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

					return true;
				}
			}
			else
			{
				//发送数据
				CMD_C_OutCard OutCard;
				OutCard.cbCardData=cbSingleCardData[i];
				OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

				return true;
			}

			nSingleCardDataCount = nSingleCardDataCount - 1;
		}
	}
	
	{
		//随机出牌
		BYTE i = rand() % cbCardCount;

		//发送数据
		CMD_C_OutCard OutCard;
		OutCard.cbCardData=cbCardData[i];
		OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);
	}
	
	return true;
}
//寻找最匹配牌的机器人
WORD CTableFrameSink::AI_BestMachChairID(WORD wOutCardChairID, WORD wGameUserChairID, BYTE bCard)
{
	BYTE i = wOutCardChairID + 1;
	WORD wChairID = i % GAME_PLAYER;
	do {
		if(wChairID != wGameUserChairID)
		{
			tagChiHuResult ChiHuResult;
			//牌型权位
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

			//判断普通玩家杠牌或者胡牌
			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUserChairID], bCard) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUserChairID],
				m_WeaveItemArray[wGameUserChairID],
				m_cbWeaveItemCount[wGameUserChairID],
				bCard,
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				//机器人智商-相当几率给普通玩家杠牌或者胡牌
				if(CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
				{
					wChairID = i++ % GAME_PLAYER;
					continue;
				}
			}

			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wChairID], bCard) != WIK_NULL ||
				m_GameLogic.EstimatePengCard(m_cbCardIndex[wChairID], bCard) != WIK_NULL ||
				m_GameLogic.EstimateEatCard(m_cbCardIndex[wChairID], bCard) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],
				m_WeaveItemArray[wChairID],
				m_cbWeaveItemCount[wChairID],
				bCard,
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				return wChairID;
			}
		}
		wChairID = i++ % GAME_PLAYER;
	} while( wChairID != wOutCardChairID);

	return INVALID_CHAIR;
}
//用户操作
bool CTableFrameSink::AI_AutomatismOperate(WORD wChairID, IServerUserItem *pIServerUserItem,
						  BYTE cbActionCard,BYTE cbActionMask,tagGangCardResult& GangCardResult)
{
	//配置变量
	BYTE			cbCenterCard;		//中心麻将
	BYTE			cbGangCard[5];		//杠牌数据

	//状态变量
	BYTE			cbItemCount;		//子项数目
	BYTE			cbCurrentItem;		//当前子项

	//设置变量
	cbItemCount=0;
	cbCurrentItem=0xFF;
	cbCenterCard=cbActionCard;

	//杠牌信息
	ZeroMemory(cbGangCard,sizeof(cbGangCard));
	for (BYTE i=0;i<GangCardResult.cbCardCount;i++) 
	{
		cbItemCount++;
		cbGangCard[i]=GangCardResult.cbCardData[i];
	}

	//计算数目
	BYTE cbItemKind[4]={WIK_LEFT,WIK_CENTER,WIK_RIGHT,WIK_PENG};
	for (BYTE i=0;i<CountArray(cbItemKind);i++) if ((cbActionMask&cbItemKind[i])!=0) cbItemCount++;

	if(cbActionMask&WIK_CHI_HU)
	{
		//发送命令
		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode=WIK_CHI_HU;
		OperateCard.cbOperateCard=0;
		OnGameMessage(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard), pIServerUserItem);

	}
	else
	{
		if(cbItemCount)
		{
			BYTE bCurrentItem = rand() % cbItemCount;
			if(GangCardResult.cbCardCount)
			{
				//杠牌子项
				 bCurrentItem = rand() % GangCardResult.cbCardCount;

				 //发送命令
				 CMD_C_OperateCard OperateCard;
				 OperateCard.cbOperateCode=WIK_GANG;
				 OperateCard.cbOperateCard=cbGangCard[bCurrentItem];
				 OnGameMessage(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard), pIServerUserItem);

			}
			else
			{
				BYTE cbIndex=0;
				//类型子项
				for (BYTE i=0;i<CountArray(cbItemKind);i++)
				{
					if (((cbActionMask&cbItemKind[i])!=0)&&(bCurrentItem==cbIndex++))
					{
						//发送命令
						CMD_C_OperateCard OperateCard;
						OperateCard.cbOperateCode=cbItemKind[i];
						OperateCard.cbOperateCard=cbCenterCard;
						OnGameMessage(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard), pIServerUserItem);
						break;
					}
				}
				
			}


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
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			GT_ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			GT_ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->UserStatus.wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	{
		m_wBankerUser=INVALID_CHAIR;

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
			{
				m_wBankerUser=i;
				break;
			}
		}
	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	WORD i=0 ;
	//效验状态
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
	GT_ASSERT(wChairID==m_wCurrentUser);
	GT_ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		GT_ASSERT(FALSE);
		return false;
	}

	//设置变量
	m_bSendStatus=true;
	m_bEnjoinChiHu[wChairID]=false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//用户切换
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//响应判断
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//抢杆设置
	if (m_bGangStatus==true)
	{
		for (i=0;i<m_wPlayerCount;i++)
		{
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
		}
		if (i==m_wPlayerCount) m_bGangStatus=false;
	}

	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//效验状态
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//效验用户
	GT_ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return false;

	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		GT_ASSERT(m_bResponse[wChairID]==false);
		GT_ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//效验状态
		if (m_bResponse[wChairID]==true) return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//变量定义
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//设置变量
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//执行判断
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//获取动作
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//优先级别
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//动作判断
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) return true;

		//吃胡等待
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) return true;
			}
		}

		//放弃操作
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//发送扑克
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//变量定义
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//出牌变量
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
			//结束信息
			m_cbChiHuCard=cbTargetCard;
			m_wProvideUser=m_wProvideUser;

			//吃牌权位
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//胡牌判断
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//过滤判断
				//if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;
				if (((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;

				//普通胡牌
				if (m_cbChiHuCard!=0)
				{
					//胡牌判断
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i]);

					//插入扑克
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}

			//结束游戏
			GT_ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//组合扑克
		GT_ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//上牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_CENTER:	//上牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_PENG:		//碰牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_FILL:		//补牌操作
		case WIK_GANG:		//杠牌操作
			{
				//删除扑克
				if ((m_cbSendCardCount==1)&&(m_cbOutCardData==0))
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}
				else
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}

				break;
			}
		}

		//构造结果
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//设置状态
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus=true;
			m_bEnjoinChiPeng[wTargetUser]=true;
		}

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//设置用户
		m_wCurrentUser=wTargetUser;

		//杠牌处理
		if ((cbTargetAction==WIK_GANG)||(cbTargetAction==WIK_FILL))
		{
			//效验动作
			bool bAroseAction=EstimateUserRespond(wTargetUser,cbTargetCard,EstimatKind_GangCard);

			//发送扑克
			if (bAroseAction==false) DispatchCardData(wTargetUser);

			return true;
		}

		//动作判断
		if (m_cbLeftCardCount>1)
		{
			//杠牌判断
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);

			//结果处理
			if (GangCardResult.cbCardCount>0)
			{
				//设置变量
				m_cbUserAction[m_wCurrentUser]|=(WIK_GANG|WIK_FILL);

				//发送动作
				SendOperateNotify();
			}
		}

		return true;
	}

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		//效验操作
		GT_ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//扑克效验
		GT_ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) return false;

		//设置变量
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_FILL:			//补牌操作
		case WIK_GANG:			//杠牌操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//杠牌处理
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
						{
							cbWeaveIndex=i;
							break;
						}
					}

					//效验动作
					GT_ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//组合扑克
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{
					//扑克效验
					GT_ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) return false;

					//设置变量
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}

				//删除扑克
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//设置状态
				if (cbOperateCode==WIK_GANG)
				{
					m_bGangStatus=true;
					m_bEnjoinChiPeng[wChairID]=true;
				}

				//构造结果
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//效验动作
				bool bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				//发送扑克
				if (bAroseAction==false) DispatchCardData(wChairID);

				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//吃牌权位
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

				//普通胡牌
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,wChiHuRight,m_ChiHuResult[wChairID]);

				//结束信息
				m_cbChiHuCard=m_cbProvideCard;

				//结束游戏
				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//状态效验
	GT_ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//荒庄结束
	if (m_cbLeftCardCount==0)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//发牌处理
	if (m_bSendStatus==true)
	{
		//发送扑克
		m_cbSendCardCount++;
		//是否AI对杀普通玩家
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
		if(m_pITableFrame->IsAIKillGameUser() && wGameUser == wCurrentUser && wGameUser != INVALID_CHAIR)
		{
			//牌型权位
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

			//判断普通玩家杠牌或者胡牌
			tagChiHuResult ChiHuResult;
			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], m_cbRepertoryCard[m_cbLeftCardCount-1]) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
				m_WeaveItemArray[wGameUser],
				m_cbWeaveItemCount[wGameUser],
				m_cbRepertoryCard[m_cbLeftCardCount-1],
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				//机器人智商-相当几率给普通玩家杠牌或者胡牌
				if(CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
				{	
					int i = m_cbLeftCardCount-1;
					bool bLoopFlag = true;
					for(;i >= 0 && bLoopFlag; i --)
					{
						//判断普通玩家杠牌或者胡牌
						tagChiHuResult ChiHuResult;
						if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], m_cbRepertoryCard[i]) == WIK_NULL &&
							m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
							m_WeaveItemArray[wGameUser],
							m_cbWeaveItemCount[wGameUser],
							m_cbRepertoryCard[i],
							wChiHuRight,
							ChiHuResult) == WIK_NULL)
						{
							bLoopFlag = false;
							BYTE bTempCard = m_cbRepertoryCard[i];
							m_cbRepertoryCard[i] = m_cbRepertoryCard[m_cbLeftCardCount-1];
							m_cbRepertoryCard[m_cbLeftCardCount-1] = bTempCard;
						}
						
					}
				}				
			}
			
		}
		
		m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//设置变量
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//杠牌判断
		if (m_cbLeftCardCount>1)
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
		}

		//牌型权位
		WORD wChiHuRight=0;
		if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

		//胡牌判断
		tagChiHuResult ChiHuResult;
		//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
		//	m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult);
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
			m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
			0x00,
			wChiHuRight,ChiHuResult);

	}

	//设置变量
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if (EstimatKind==EstimatKind_OutCard)
		{
			//吃碰判断
			if (m_bEnjoinChiPeng[i]==false)
			{
				//碰牌判断
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//吃牌判断
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}

			//杠牌判断
			if (m_cbLeftCardCount>1) 
			{
				m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//胡牌判断
		if (m_bEnjoinChiHu[i]==false)
		{
			//牌型权位
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//吃胡判断
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			//吃胡限制
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) m_bEnjoinChiHu[i]=true;
		}

		//结果判断
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}
//判断荒局
bool CTableFrameSink::EstimateNullChiHuCard()
{
	return (m_cbChiHuCard==0 && m_wProvideUser==INVALID_CHAIR) ? true : false;
}
//获取放炮玩家
WORD  CTableFrameSink::GetProvideUser()
{	
	return m_wProvideUser;
}
//获取胡牌玩家
WORD CTableFrameSink::GetHuPaiUser()
{	
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) return i;
	}
	return INVALID_CHAIR;
}
//抓炮胡牌
bool CTableFrameSink::IsZhuaPaoChiHu()
{
	if(m_wProvideUser != INVALID_CHAIR)
		return m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL ? true : false;
	else
		return false;
}
//判断有效投注用户
bool CTableFrameSink::IsValidBetScoreUser(DWORD dwUserID)
{
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(GetProvideUser());
	if(pIServerUserItem)
	{
		tagUserData UserData = pIServerUserItem->GetUserData()->UserData;
		if(UserData.dwUserID == dwUserID)
			return true;
		if(UserData.dwHighUserID4 == dwUserID)
			return true;
		if(UserData.dwHighUserID3 == dwUserID)
			return true;
		if(UserData.dwHighUserID2 == dwUserID)
			return true;
		if(UserData.dwHighUserID1 == dwUserID)
			return true;
		if(UserData.dwHighUserID0 == dwUserID)
			return true;
	}

	pIServerUserItem = m_pITableFrame->GetServerUserItem(GetHuPaiUser());
	if(pIServerUserItem)
	{
		tagUserData UserData = pIServerUserItem->GetUserData()->UserData;
		if(UserData.dwUserID == dwUserID)
			return true;
		if(UserData.dwHighUserID4 == dwUserID)
			return true;
		if(UserData.dwHighUserID3 == dwUserID)
			return true;
		if(UserData.dwHighUserID2 == dwUserID)
			return true;
		if(UserData.dwHighUserID1 == dwUserID)
			return true;
		if(UserData.dwHighUserID0 == dwUserID)
			return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
