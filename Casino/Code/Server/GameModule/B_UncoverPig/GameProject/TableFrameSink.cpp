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
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pICalculateFrame=NULL;

	//游戏变量
    m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR; 
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	m_bShowCardTime = 0;
	ZeroMemory(m_bScoreCard , sizeof(m_bScoreCard)) ;
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;
	m_bTotalShowCardCount = 0 ;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;
	memset(m_bShowCardFlag, 0, sizeof(m_bShowCardFlag));

	//Run信息
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;
	m_wGetGpadeUser = INVALID_CHAIR;

	//扑克信息
	memset(m_bCardCount , 0 , GAME_PLAYER);	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
    
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
	//游戏变量
	m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR;  
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	m_bShowCardTime = 0;
	ZeroMemory(m_bScoreCard , sizeof(m_bScoreCard)) ;
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;
	m_bTotalShowCardCount = 0 ;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;
	memset(m_bShowCardFlag, 0, sizeof(m_bShowCardFlag));


	//Run信息
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;

	//扑克信息
	memset(m_bCardCount , 0 , GAME_PLAYER);	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;


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
	m_pITableFrame->SetGameStatus(GS_WK_SHOWCARD);

	//游戏变量
	m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR;  
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;
	memset(m_bShowCardFlag, 0, sizeof(m_bShowCardFlag));

	//Run信息
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;

	//混乱扑克
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=HAND_CARD_COUNT;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}
	//设置用户	

	//发送扑克
	CMD_S_SendCard SendCard;
	memset(&SendCard , 0 , sizeof(SendCard)) ;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		SendCard.wCurrentUser=i;
		CopyMemory(SendCard.bCardData,m_bHandCardData[i],sizeof(SendCard.bCardData));
        m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	//首先出牌者
	if(INVALID_CHAIR==m_wGetGpadeUser)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
			for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j)
			{
				BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
				bCardColor = bCardColor>>4 ;
				BYTE bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
				if(1==bCardColor && 2==bCardValue) 
				{
					m_wCurrentUser = i ;
					break ;
				}
			}
	}
	else  
		m_wCurrentUser = m_wGetGpadeUser ;

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
			m_wGetGpadeUser = INVALID_CHAIR;

			//效验参数
			GT_ASSERT(pIServerUserItem!=NULL);
			GT_ASSERT(wChairID<m_wPlayerCount);

			//构造数据
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

		
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

			CopyMemory(GameEnd.ScoreCard , m_bScoreCard , sizeof(m_bScoreCard)) ;
			CopyMemory(GameEnd.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(GameEnd.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(GameEnd.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(GameEnd.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*sizeof(GameEnd.bTurnCardData));
			for(WORD wChairID = 0; wChairID < 4; wChairID++)
				CopyMemory(&GameEnd.bCardData[4],m_bHandCardData[wChairID],sizeof(BYTE)*m_bCardCount[wChairID]);
			CopyMemory(GameEnd.bShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;
			CopyMemory(GameEnd.bScoreCardCount , m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;

			//计算额度
			bool    bShowSpade   = false ,    //亮黑桃Q
				    bShowHearts  = false ,    //亮红桃A
					bShowClubs   = false ,    //亮梅花10
					bShowDiamonds= false ;    //亮方块J
			BYTE    bCardColor ;
			BYTE    bCardValue ;

			//判断亮牌
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				for(BYTE j=0 ; j<4 ; ++j)
				{
					if(0!=m_bShowCard[i][j])
					{
						bCardColor = m_GameLogic.GetCardColor(m_bShowCard[i][j]) ;
						bCardColor = bCardColor>>4 ;
						bCardValue = m_GameLogic.GetCardValue(m_bShowCard[i][j]) ;
						//判断扑克
                        
						//黑桃Q
						if(3==bCardColor && 12==bCardValue)  bShowSpade         = true ;
						//红桃A
						else if(2==bCardColor && 1==bCardValue)   bShowHearts   = true ;
						//方块J
						else if(0==bCardColor && 11==bCardValue)  bShowDiamonds = true ;
                        //梅花10
						else if(1==bCardColor && 10==bCardValue)  bShowClubs    = true ; 
					}
				}
			}

			//扑克分数
			int  bScore[16]={0,0,0,-10,-10,-10,-10,-10,-10,-20,-30,-40,-50,100,-100,50} ;
			BYTE bIndex ;

			//红桃翻倍
			if(true==bShowHearts) for(BYTE i=0 ; i<13 ; ++i) bScore[i] *=2 ;
			//猪牌翻倍
			if(true==bShowSpade)    bScore[14] *= 2 ;
			//羊牌翻倍
			if(true==bShowDiamonds) bScore[13] *= 2 ;

			bool  bGetClubs    = false  ,     //得变压器
				  bGetHearts   = false  ,     //得红桃A
				  bGetSpade    = false ,      //得黑桃Q
				  bGetDiamonds = false ;      //得方块J

			DOUBLE  fCellScore=m_pGameServiceOption->dwCellScore;

			//计算额度
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				bGetClubs    = false ;
				bGetHearts   = false ;
				bGetSpade    = false ;
				bGetDiamonds = false ;      

				//没有分牌
				if(0==m_bScoreCardCount[i]) continue ;

				for(BYTE j=0 ; j<m_bScoreCardCount[i] ; ++j)
				{
					//数据检验
					GT_ASSERT(0!=m_bScoreCard[i][j]) ;
					if(0==m_bScoreCard[i][j]) return false ;
					bCardColor = m_GameLogic.GetCardColor(m_bScoreCard[i][j]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bScoreCard[i][j]) ;
					//数据检验
					GT_ASSERT(2==bCardColor || (0==bCardColor && 11==bCardValue) || (1==bCardColor && 10==bCardValue) || (3==bCardColor && 12==bCardValue));   

					bIndex = bCardValue-2 ;
					//黑桃Q
					if(3==bCardColor && 12==bCardValue) 
					{
						bGetSpade   = true ;
						bIndex      = 14 ; 
					}
					//红桃A
					if(2==bCardColor && 1==bCardValue) 
					{
						bGetHearts   = true ;
						bIndex       = 12 ; 
					}
					//方块J
					if(0==bCardColor && 11==bCardValue)
					{
						bGetDiamonds = true ;
						bIndex       = 13 ; 
					}
					//梅花10
					if(1==bCardColor && 10==bCardValue)
					{
						bGetClubs    = true ; 
						bIndex       = 15 ; 
					}
					//非梅花10
					if(1!=bCardColor) GameEnd.fGameScore[i]  += bScore[bIndex] ;
				}	

				//调整得分

				//处理变压
				if(1==m_bScoreCardCount[i]      && true==bShowClubs  && true==bGetClubs)  GameEnd.fGameScore[i] = 100 ;
				else if(1==m_bScoreCardCount[i] && false==bShowClubs && true==bGetClubs)  GameEnd.fGameScore[i] = 50  ;
				else if(m_bScoreCardCount[i]>1  && false==bShowClubs && true==bGetClubs)  GameEnd.fGameScore[i] *= 2  ;
				else if(m_bScoreCardCount[i]>1  && true==bShowClubs  && true==bGetClubs)  GameEnd.fGameScore[i] *= 4  ;

				//满红得分
				if((13==m_bScoreCardCount[i]) && (false==bGetClubs) && (false==bGetDiamonds) && (false==bGetSpade))
				{
					if(true==bShowHearts) GameEnd.fGameScore[i] = 400 ;
					else                  GameEnd.fGameScore[i] = 200 ;
				}
				//满贯得分
				if(16==m_bScoreCardCount[i])
				{
					//玩家亮牌
					if(false!=bShowSpade || false!=bShowHearts || false!=bShowDiamonds || false!=bShowClubs) 
					{
						GameEnd.fGameScore[i] = 0 ;
						for(BYTE k=0 ; k<15 ; ++k) GameEnd.fGameScore[i] += abs(bScore[k]) ;
						//变压器
						if(true==bShowClubs) GameEnd.fGameScore[i] *= 4 ;
						else                 GameEnd.fGameScore[i] *= 2 ;
					}
					else//没有亮牌 
						GameEnd.fGameScore[i] = 800 ;
					
				}
			}

			//平衡额度
			DOUBLE fTempScore[GAME_PLAYER] ;
			memset(fTempScore , 0 , sizeof(fTempScore)) ;
			fTempScore[0] = GameEnd.fGameScore[0]-(GameEnd.fGameScore[1]+GameEnd.fGameScore[2]+GameEnd.fGameScore[3])/3 ;
			fTempScore[1] = GameEnd.fGameScore[1]-(GameEnd.fGameScore[0]+GameEnd.fGameScore[2]+GameEnd.fGameScore[3])/3 ;
			fTempScore[2] = GameEnd.fGameScore[2]-(GameEnd.fGameScore[0]+GameEnd.fGameScore[1]+GameEnd.fGameScore[3])/3 ;
			fTempScore[3] = GameEnd.fGameScore[3]-(GameEnd.fGameScore[0]+GameEnd.fGameScore[1]+GameEnd.fGameScore[2])/3 ;
			
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				GameEnd.fGameScore[i] = fTempScore[i] ;
				GameEnd.fGameScore[i] *= (fCellScore / 100.0);	
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
			m_wGetGpadeUser = INVALID_CHAIR;

			//定义变量
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			CopyMemory(GameEnd.ScoreCard , m_bScoreCard , sizeof(m_bScoreCard)) ;
			CopyMemory(GameEnd.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(GameEnd.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(GameEnd.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(GameEnd.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*sizeof(GameEnd.bTurnCardData));
			for(WORD i = 0; i < 4; i++)
				CopyMemory(&GameEnd.bCardData[4],m_bHandCardData[i],sizeof(BYTE)*m_bCardCount[i]);
			CopyMemory(GameEnd.bShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;
			CopyMemory(GameEnd.bScoreCardCount , m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;


			//统计额度
			GameEnd.fGameScore[wChairID]=6*m_pGameServiceOption->dwCellScore;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i) continue;
				GameEnd.fGameScore[i]=GameEnd.fGameScore[wChairID]/3;

			}
			GameEnd.fGameScore[wChairID] = - GameEnd.fGameScore[wChairID];

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


			return true;
		}
	}

	GT_ASSERT(FALSE);

	return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_WK_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SHOWCARD:	//玩家亮牌
		{
			CMD_S_StatusShowCard StatusShowCard ;
			memset(&StatusShowCard , 0 , sizeof(StatusShowCard)) ;
			StatusShowCard.wCurrentUser = wChairID ;
			CopyMemory(StatusShowCard.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(StatusShowCard.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(StatusShowCard.bCardData , m_bHandCardData[wChairID] , sizeof(m_bHandCardData[wChairID])) ;
			CopyMemory(StatusShowCard.ShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusShowCard,sizeof(StatusShowCard));
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//设置变量
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wFirstOutUser = m_wFirstOutUser ;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;

			CopyMemory(StatusPlay.ScoreCard , m_bScoreCard , sizeof(m_bScoreCard)) ;
			CopyMemory(StatusPlay.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(StatusPlay.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*sizeof(StatusPlay.bTurnCardData));
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChairID],sizeof(BYTE)*m_bCardCount[wChairID]);
			CopyMemory(StatusPlay.bShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;
			CopyMemory(StatusPlay.bScoreCardCount , m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}
//人工智能游戏动作
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	switch(cbGameStatus) 
	{
	case GS_WK_SHOWCARD://玩家亮牌
		{
			for(BYTE i = 0; i < GAME_PLAYER; i ++)
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
				if(m_bShowCardFlag[i])
					continue;

				BYTE bShowCardData[4];
				BYTE dwCardCount=AI_CanShowCard(wChairID,pIServerUserItem,bShowCardData); 
				if(dwCardCount == 0)
				{
					CMD_C_ShowCard  ShowCard ;
					memset(&ShowCard , 0 , sizeof(ShowCard)) ;

					OnGameMessage(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard), pIServerUserItem) ;
				}
				else
				{
					//50%机会亮牌
					if((rand() % 2) == 0)
					{
						CMD_C_ShowCard  ShowCard ;
						memset(&ShowCard , 0 , sizeof(ShowCard)) ;

						OnGameMessage(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard), pIServerUserItem) ;

					}
					else
					{
						CMD_C_ShowCard  ShowCard ;
						memset(&ShowCard , 0 , sizeof(ShowCard)) ;

						CopyMemory(ShowCard.bShowCard , bShowCardData , dwCardCount) ;
						ShowCard.bShowCardCount = dwCardCount ;

						OnGameMessage(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard), pIServerUserItem) ;

					}
				}
			}
		}
		break;
	case GS_WK_PLAYING://游戏状态
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
			AI_AutomatismOutCard(wChairID,pIServerUserItem);
		}
		break;
	default:
		break;
	}
	return true;
}
//判断亮牌
BYTE  CTableFrameSink::AI_CanShowCard(WORD wChairID, IServerUserItem * pIServerUserItem,BYTE bShowCardData[4])
{
	if(m_bShowCardCount[wChairID] != 0)
		return 0;
	else
	{
		BYTE	bHandCardData[HAND_CARD_COUNT];	//手上扑克
		memcpy(bHandCardData, m_bHandCardData[wChairID], sizeof(bHandCardData));

		bool bCanShowCard=false ;
		BYTE bCardColor ;
		BYTE bCardValue ;
		BYTE dwCardCount=0 ; 
		for(BYTE i=0 ; i<HAND_CARD_COUNT ; ++i)
		{
			bCardColor = m_GameLogic.GetCardColor(bHandCardData[i]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = m_GameLogic.GetCardValue(bHandCardData[i]) ;
			//判断扑克
			if((3==bCardColor && 12==bCardValue) || //黑桃Q
				(2==bCardColor && 1==bCardValue)  || //红桃A
				(0==bCardColor && 11==bCardValue) || //方块J
				(1==bCardColor && 10==bCardValue))   //梅花10
			{

				bCanShowCard = true ;

				//保存亮牌
				bShowCardData[dwCardCount++] = bHandCardData[i] ;

			}
		}

		return bCanShowCard ? dwCardCount : 0;
	}
}
//自动出牌
bool  CTableFrameSink::AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//设置扑克
	BYTE bHandCardCount = m_bCardCount[wChairID];//扑克数目
	BYTE bHandCardData[HAND_CARD_COUNT];	//手上扑克
	memcpy(bHandCardData, m_bHandCardData[wChairID], sizeof(bHandCardData));

	for(BYTE i=bHandCardCount-1 ; i!=255 ; --i)
	{
		//数据验证
		ASSERT(i>=0 && i<=bHandCardCount) ;
		if(i==255 || i>bHandCardCount) return false ;
	
		BYTE bCardData[HAND_CARD_COUNT];
		bCardData[0] = bHandCardData[i];

		//有效判断
		if(true==AI_VerdictOutCard(wChairID,pIServerUserItem,
			true,
			bCardData,1,
			bHandCardData,bHandCardCount)) 
		{
			//发送数据
			CMD_C_OutCard OutCard;
			memset(&OutCard , 0 , sizeof(OutCard)) ;
			OutCard.bCardCount=1;
			OutCard.bCardData=bCardData[0];

			OnGameMessage(SUB_C_OUT_CART,&OutCard,
				sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE),pIServerUserItem);

			return true ;
		}
		
	}
	return false ;
}
//出牌判断
bool CTableFrameSink::AI_VerdictOutCard(WORD wChairID, IServerUserItem * pIServerUserItem,
										bool bAutoOutCard,
										BYTE bCardData[HAND_CARD_COUNT],BYTE bShootCount,
										BYTE bHandCardData[HAND_CARD_COUNT],BYTE bHandCardCount)
{
	bool bFirstOutSpade = true;
	bool bFirstHearts   = true ;  
	bool bFirstClubs    = true ;   
	bool bFirstDiamonds = true ;
	bool bFirstOutCard  = false ;
	bool bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;

	//出牌判断
	if (bShootCount==1)
	{
		if(true==bFirstGame)
		{
			//首牌判断
			bool bFisrCard=true ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
				if(HAND_CARD_COUNT!=m_bCardCount[i]) bFisrCard = false ;

			if(true==bFisrCard)
			{
				BYTE bCardColor = m_GameLogic.GetCardColor(bCardData[0]) ;
				bCardColor = bCardColor>>4 ;
				BYTE bCardValue = m_GameLogic.GetCardValue(bCardData[0]) ;
				if(1==bCardColor && 2==bCardValue) 
				{
					bFirstClubs = false ;
					return true ;
				}
				
				return false ;
			}
		}

		if(INVALID_CHAIR==m_wFirstOutUser) 
		{
			BYTE bCardColor = m_GameLogic.GetCardColor(bCardData[0]) ;
			bCardColor      = bCardColor>>4 ; 
			return AI_VerdictFirstGameColor(bCardColor , bCardData[0] , true,bHandCardData,bHandCardCount) ;
		}
		BYTE bFirstOutColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
		bFirstOutColor = bFirstOutColor>>4 ;
		BYTE bCurrentColor = m_GameLogic.GetCardColor(bCardData[0]) ;
		bCurrentColor = bCurrentColor>>4 ;

		//是否垫牌
		if(bFirstOutColor!=bCurrentColor) 
		{
			for(BYTE i=0 ; i<bHandCardCount ; ++i)
			{
				bCurrentColor = m_GameLogic.GetCardColor(bHandCardData[i]) ;
				bCurrentColor = bCurrentColor >> 4 ;
				if(bFirstOutColor==bCurrentColor)
				{
					return false ;
				}
			}
			return true ;
		}
		else
			return AI_VerdictFirstGameColor(bFirstOutColor , bCardData[0] ,true,bHandCardData,bHandCardCount) ;


	}

	return false;
}
//第一轮花色
bool  CTableFrameSink::AI_VerdictFirstGameColor(BYTE bFirstOutColor , 
											 BYTE bCardData , 
											 bool bAutoOutCard,
											 BYTE bHandCardData[HAND_CARD_COUNT],BYTE bHandCardCount)
{
	BYTE bCurrentColor = m_GameLogic.GetCardColor(bCardData) ;
	bCurrentColor = bCurrentColor>>4 ;

	bool bFirstOutSpade = true;
	bool bFirstHearts   = true ;  
	bool bFirstClubs    = true ;   
	bool bFirstDiamonds = true ;
	bool bFirstOutCard  = false ;


	//第一轮花色
	switch(bFirstOutColor)
	{
	case 0:      //方块
		{
			//恢复变量
			if(true==bFirstOutCard) bFirstDiamonds = true ;

			//第一轮判断
			if(true==bFirstDiamonds)
			{
				bFirstDiamonds = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_bShowCardCount[2] ; ++i)
					if(bCardData==m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<bHandCardCount ; ++i)
						{
							if(bCardData==bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								//恢复变量
								bFirstDiamonds = true ;
								return false ;
							}
						}

						return true ;
					}
					else
					{
						//恢复变量
						bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 1:     //梅花
		{
			//恢复变量
			if(true==bFirstOutCard) bFirstClubs = true ;

			//第一轮判断
			if(true==bFirstClubs)
			{
				bFirstClubs = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_bShowCardCount[2] ; ++i)
					if(bCardData==m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<bHandCardCount ; ++i)
						{
							if(bCardData==bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								//恢复变量
								bFirstClubs = true ;
								return false ;
							}
						}

						return true ;
					}
					else
					{
						//恢复变量
						bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 2:     //红桃
		{
			//恢复变量
			if(true==bFirstOutCard) bFirstHearts = true ;

			if(true==bFirstHearts)
			{
				bFirstHearts = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_bShowCardCount[2] ; ++i)
					if(bCardData==m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<bHandCardCount ; ++i)
						{
							if(bCardData==bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								//恢复变量
								bFirstHearts = true ;
								return false ;
							}
						}
						return true ;
					}
					else
					{
						//恢复变量
						bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 3:     //黑桃
		{
			//恢复变量
			if(true==bFirstOutCard) bFirstHearts = true ;

			if(true==bFirstOutSpade)
			{
				bFirstOutSpade = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_bShowCardCount[2] ; ++i)
					if(bCardData==m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<bHandCardCount ; ++i)
						{
							if(bCardData==bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								//恢复变量
								bFirstOutSpade = true ;
								return false ;
							}
						}
						return true ;
					}
					else 
					{
						//恢复变量
						bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	default:    //错误
		return false ;
	}
	return true ;
}


bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	BYTE k = 0;
	switch (wSubCmdID)
	{
	case SUB_C_SHOW_CART:       //玩家亮牌
		{
			CMD_C_ShowCard *pShowCard = (CMD_C_ShowCard*)pDataBuffer ;
			
			//数据校验
			GT_ASSERT(wDataSize==sizeof(CMD_C_ShowCard)) ;
			if(wDataSize!=sizeof(CMD_C_ShowCard)) return false ;
            
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();		
			m_bShowCardFlag[pUserData->UserStatus.wChairID] = 1;
			++m_bShowCardTime;

			//没有亮牌
			if(0==pShowCard->bShowCardCount) 
			{
				//游戏开始 
				if(GAME_PLAYER==m_bShowCardTime)
				{
					//亮第四张
					if(3==m_bTotalShowCardCount)
					{
						BYTE bCardColor ;
						BYTE bCardValue ;
						bool bHaveShow=false ;

						for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
							for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j) 
							{
								bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
								bCardColor = bCardColor>>4 ;
								bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
								//判断扑克
								if((3==bCardColor && 12==bCardValue)  || //黑桃Q
									(2==bCardColor && 1==bCardValue)  || //红桃A
									(0==bCardColor && 11==bCardValue) || //方块J
									(1==bCardColor && 10==bCardValue))   //梅花10
								{
									bHaveShow = false ;
									for(BYTE p=0 ; p<GAME_PLAYER ; ++p)
										for(BYTE c=0 ; c<4 ; ++c)
										{
											if(m_bShowCard[p][c]==m_bHandCardData[i][j]) bHaveShow=true ;
										}
										if(false==bHaveShow)
										{
											m_wShowCardUser[i] = i ; 
											for(k=0 ; k<4 ; ++k)  if(0==m_bShowCard[i][k]) break ;
											m_bShowCard[i][k] = m_bHandCardData[i][j] ;

											//亮牌数据
											CMD_S_ShowCard ShowCard ;
											memset(&ShowCard , 0 , sizeof(ShowCard)) ;
											ShowCard.wShowCardUser = i ;
											ShowCard.bShowCardCount = k+1 ;
											CopyMemory(ShowCard.bShowCard , m_bShowCard[i] , ShowCard.bShowCardCount) ;

											m_bShowCardCount[i] = ShowCard.bShowCardCount ;

											m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
											m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
											break ;
										}
								}
							}
					}

					CMD_S_GameStart GameStart ;
					memset(&GameStart , 0 , sizeof(GameStart)) ;
					GameStart.wCurrentUser = m_wCurrentUser ;
					GameStart.bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
					m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
				}
				return true ;
			}
			
			//记录亮牌
			m_wShowCardUser[m_bShowCardUserCount++] = pUserData->UserStatus.wChairID ;
			CopyMemory(m_bShowCard[pUserData->UserStatus.wChairID] , pShowCard->bShowCard , pShowCard->bShowCardCount*sizeof(BYTE)) ;

			return OnUserShowCard(pUserData->UserStatus.wChairID , pShowCard->bShowCard , pShowCard->bShowCardCount) ;
		}
	case SUB_C_OUT_CART:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//效验数据
			GT_ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) 
			{
				return false;
			}

			GT_ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData)));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData))) 
			{
				return false;
			}


			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
		
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->bCardData,1);
			//////////////////////////////////////////////////////////////////////////

		}
	}

	return true;
}

//玩家亮牌
bool CTableFrameSink::OnUserShowCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	BYTE k=0;
	//亮牌数据
	CMD_S_ShowCard ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;
	ShowCard.wShowCardUser = wChairID ;
	ShowCard.bShowCardCount = bCardCount ;
	CopyMemory(ShowCard.bShowCard , bCardData , bCardCount) ;

	//保留亮牌
	m_wShowCardUser[wChairID] = wChairID ;      
	CopyMemory(m_bShowCard[wChairID] , bCardData , bCardCount) ;
	m_bTotalShowCardCount += bCardCount ;

	m_bShowCardCount[wChairID] = bCardCount ;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
    

	//游戏开始 
	if(GAME_PLAYER==m_bShowCardTime)
	{
		//亮第四张
	    if(3==m_bTotalShowCardCount)
		{
			BYTE bCardColor ;
			BYTE bCardValue ;
			bool bHaveShow=false ;

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
				for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j) 
				{
                    bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
					//判断扑克
					if((3==bCardColor && 12==bCardValue)  || //黑桃Q
						(2==bCardColor && 1==bCardValue)  || //红桃A
						(0==bCardColor && 11==bCardValue) || //方块J
						(1==bCardColor && 10==bCardValue))   //梅花10
					{
						bHaveShow = false ;
						for(BYTE p=0 ; p<GAME_PLAYER ; ++p)
							for(BYTE c=0 ; c<4 ; ++c)
							{
								if(m_bShowCard[p][c]==m_bHandCardData[i][j]) bHaveShow=true ;
							}
						if(false==bHaveShow)
						{
							m_wShowCardUser[i] = i ; 
							for(k=0 ; k<4 ; ++k)  if(0==m_bShowCard[i][k]) break ;
                            m_bShowCard[i][k] = m_bHandCardData[i][j] ;

							//亮牌数据
							CMD_S_ShowCard ShowCard ;
							memset(&ShowCard , 0 , sizeof(ShowCard)) ;
							ShowCard.wShowCardUser = i ;
							ShowCard.bShowCardCount = k+1 ;
							CopyMemory(ShowCard.bShowCard , m_bShowCard[i] , ShowCard.bShowCardCount) ;

							m_bShowCardCount[i] = ShowCard.bShowCardCount ;

							m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
							m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));

							break ;
						}
					}
				}				
		}
		CMD_S_GameStart GameStart ;
		memset(&GameStart , 0 , sizeof(GameStart)) ;
		GameStart.wCurrentUser = m_wCurrentUser ;
		GameStart.bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
	}
    return true ;
}


//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData, BYTE bCardCount)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(&bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;	

	//更改出牌
	m_bTurnCardCount++ ;
	if(1==m_bTurnCardCount) m_wFirstOutUser = wChairID ;
	m_bTurnCardData[wChairID] = bCardData ;
	if(GAME_PLAYER==m_bTurnCardCount) 
	{
		m_bTurnCardCount = 0 ;

		//最大出牌
		WORD wMostUser = m_wFirstOutUser ;
		BYTE bFirstCardColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
        bFirstCardColor = bFirstCardColor >> 4 ;
		BYTE bMostCardValue  = m_GameLogic.GetCardValue(m_bTurnCardData[m_wFirstOutUser]) ;
		bool bFoudGpade = false ;

		if(1==bMostCardValue) bMostCardValue += 13 ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bCurrentCarColor  = m_GameLogic.GetCardColor(m_bTurnCardData[i]) ;
            bCurrentCarColor = bCurrentCarColor >> 4 ;
			BYTE bCurrentCardValue = m_GameLogic.GetCardValue(m_bTurnCardData[i]) ;
			if(1==bCurrentCardValue) bCurrentCardValue += 13 ;
			if(bCurrentCarColor == bFirstCardColor)
				if(bMostCardValue < bCurrentCardValue)
				{
					bMostCardValue = bCurrentCardValue ;
					wMostUser = i ;
				}
			//得到黑桃Q
			if(3==bCurrentCarColor && 12==bCurrentCardValue) bFoudGpade=true ;
		}
		//得到黑桃Q
		if(true==bFoudGpade) m_wGetGpadeUser = wMostUser ;

		//获取分数
		BYTE bScoreCard[GAME_PLAYER] ;
		BYTE bScoreCount=0 ;
        if(true==m_GameLogic.GetScoreCard(m_bTurnCardData , bScoreCard , GAME_PLAYER , bScoreCount))
		{
			CopyMemory(&m_bScoreCard[wMostUser][m_bScoreCardCount[wMostUser]] , bScoreCard , bScoreCount) ;
            m_bScoreCardCount[wMostUser] += bScoreCount ;
		}

        m_wTurnWiner = wMostUser ;
		if (m_bCardCount[wChairID]!=0)
		{
			m_wCurrentUser = wMostUser ;
		}
		else m_wCurrentUser=INVALID_CHAIR;	
		m_wFirstOutUser = wMostUser;	
		memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
	}	
	else
	{
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		if(0==m_bCardCount[m_wCurrentUser])	m_wCurrentUser=INVALID_CHAIR;
	}
	//切换用户
	
	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	WORD wSendSize;
	CopyMemory(&OutCard.bCardData,&bCardData,bCardCount*sizeof(BYTE));
	wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);

	//发送数据

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//判断结束游戏
	
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	}

	return true;
}
//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理


//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
