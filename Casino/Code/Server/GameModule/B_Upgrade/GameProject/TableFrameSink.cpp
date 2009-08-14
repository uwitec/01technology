#include "StdAfx.h"
#include "TableFrameSink.h"

//花色定义
#define CALL_NONE					0x00								//叫牌定义
#define CALL_NT						0x10								//叫牌定义
#define CALL_HEI_TAO				0x01								//叫牌定义
#define CALL_HONG_TAO				0x02								//叫牌定义
#define CALL_MEI_HUA				0x04								//叫牌定义
#define CALL_FANG_KUAI				0x08								//叫牌定义

WORD Convert_CardMask_To_CardColor(BYTE cbCardMask)
{
	if(CALL_FANG_KUAI&cbCardMask)
		return COLOR_FANG_KUAI;
	if(CALL_MEI_HUA&cbCardMask)
		return COLOR_MEI_HUA;
	if(CALL_HONG_TAO&cbCardMask)
		return COLOR_HONG_TAO;
	if(CALL_HEI_TAO&cbCardMask)
		return COLOR_HEI_TAO;
	if(CALL_NT&cbCardMask)
		return COLOR_NT;
	return CALL_NONE;
}

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
	VarDecFromR8(m_pGameEnd->fScore[wChairID], &decTemp);
	VarDecAdd(&decTemp, &pPartedWinScore[0], &pPartedWinScore[0]);

	//判断大于0,进行抽水结算
	if(VarDecCmpR8(&decTemp, 0) == VARCMP_GT)
	{
		//抽水 = 总派彩 * (- 抽水率)
		VarDecMul(&decTemp,(DECIMAL*) &m_decAfterTax, &pPartedTaxScore[0]);
		//更新Double派彩
		VarDecAdd(&decTemp,&pPartedTaxScore[0], &decTemp);
		VarR8FromDec(&decTemp, &m_pGameEnd->fScore[wChairID]);
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
	//逻辑变量
	m_cbPackCount=2;
	m_cbMainValue=0x02;
	m_cbMainColor=COLOR_ERROR;

	//连局信息
	m_cbValueOrder[0]=2;
	m_cbValueOrder[1]=2;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//叫牌信息
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//状态变量
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//出牌变量
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//底牌扑克
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//用户扑克
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	//逻辑变量
	m_cbPackCount=2;
	m_cbMainColor=COLOR_ERROR;

	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;

	//叫牌信息
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//状态变量
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//出牌变量
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//底牌扑克
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//用户扑克
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	m_pITableFrame->SetGameStatus(GS_UG_CALL);

	//设置变量
	m_cbMainColor=COLOR_ERROR;

	//设置属性
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetMainValue(m_cbMainValue);
	m_GameLogic.SetPackCount(m_cbPackCount);

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.cbPackCount=m_cbPackCount;
	GameStart.cbMainValue=m_cbMainValue;
	GameStart.cbValueOrder[0]=m_cbValueOrder[0];
	GameStart.cbValueOrder[1]=m_cbValueOrder[1];

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//发送扑克
	DispatchUserCard();

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
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//底牌信息
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//游戏得分
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

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
			ZeroMemory(&GameEnd,sizeof(GameEnd));


			//底牌信息
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			
			//用户变量
			GameEnd.wBankerUser=m_wBankerUser;

			//叫牌变量
			GameEnd.cbCallCard=m_cbCallCard;
			GameEnd.cbCallCount=m_cbCallCount;
			GameEnd.wCallCardUser=m_wCallCardUser;

			//得分变量
			GameEnd.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(GameEnd.cbScoreCardData,m_cbScoreCardData,sizeof(GameEnd.cbScoreCardData));

			//游戏得分
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//扣底倍数
			if ((wChairID!=m_wBankerUser)&&(((wChairID+2)%m_wPlayerCount)!=m_wBankerUser))
			{
				//变量定义
				BYTE cbMaxCount=1;
				GameEnd.wConcealTime=1;

				//分析牌型
				tagAnalyseResult AnalyseResult;
				m_GameLogic.AnalyseCardData(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID],AnalyseResult);

				//同牌分析
				for (BYTE i=0;i<CountArray(AnalyseResult.SameDataInfo);i++)
				{
					if (AnalyseResult.SameDataInfo[i].cbBlockCount>0) cbMaxCount=__max(i,cbMaxCount);

				}

				//拖牌分析
				for (BYTE i=0;i<CountArray(AnalyseResult.TractorDataInfo);i++)
				{
					//获取属性
					BYTE cbSameCount=i+2;
					BYTE cbMaxLength=AnalyseResult.TractorDataInfo[i].cbTractorMaxLength;

					//设置变量
					cbMaxCount=__max(cbSameCount*cbMaxLength,cbMaxCount);
				}

				//设置倍数
				cbMaxCount=__min(cbMaxCount,6);
				for (BYTE i=0;i<cbMaxCount;i++) GameEnd.wConcealTime*=2;
			}

			//统计变量
			DOUBLE fBankerScore=0;
			WORD wWholeScore=GameEnd.wGameScore+GameEnd.wConcealTime*GameEnd.wConcealScore;

			//额度统计
			if (wWholeScore==0) 
			{
				fBankerScore=3.0;
				m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+2)%13+1;
			}
			else if (wWholeScore<20*m_cbPackCount) 
			{
				fBankerScore=2.0;
				m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+1)%13+1;
			}
			else if (wWholeScore<40*m_cbPackCount)
			{
				fBankerScore=1.0;
				m_cbValueOrder[m_wBankerUser%2]=m_cbValueOrder[m_wBankerUser%2]%13+1;
			}
			else
			{
				fBankerScore=-(wWholeScore-40*m_cbPackCount)/(10*m_cbPackCount)-1;
				char tv = fBankerScore;
				m_cbValueOrder[(m_wBankerUser+1)%2]=(BYTE)((m_cbValueOrder[(m_wBankerUser+1)%2]-tv-2)%13+1);
			}

			//玩家得分
			GameEnd.fScore[m_wBankerUser]=fBankerScore*m_pGameServiceOption->dwCellScore;
			GameEnd.fScore[(m_wBankerUser+2)%m_wPlayerCount]=fBankerScore*m_pGameServiceOption->dwCellScore;
			GameEnd.fScore[(m_wBankerUser+1)%m_wPlayerCount]=-(fBankerScore*m_pGameServiceOption->dwCellScore);
			GameEnd.fScore[(m_wBankerUser+3)%m_wPlayerCount]=-(fBankerScore*m_pGameServiceOption->dwCellScore);

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd, sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);


				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//庄家切换
				if (GameEnd.fScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
				else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

				//主牌设置
				m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

				//结束游戏
				m_pITableFrame->ConcludeGame();

				return true;
			}
			else
			{
				GT_ASSERT(FALSE);
				//庄家切换
				if (GameEnd.fScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
				else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

				//主牌设置
				m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

				//结束游戏
				m_pITableFrame->ConcludeGame();
				return false;
			}

		}
	case GER_USER_LEFT:		//用户强退
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//底牌信息
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		
			//用户变量
			GameEnd.wBankerUser=m_wBankerUser;

			//叫牌变量
			GameEnd.cbCallCard=m_cbCallCard;
			GameEnd.cbCallCount=m_cbCallCount;
			GameEnd.wCallCardUser=m_wCallCardUser;

			//得分变量
			GameEnd.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(GameEnd.cbScoreCardData,m_cbScoreCardData,sizeof(GameEnd.cbScoreCardData));

			//游戏得分
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//玩家得分
			GameEnd.fScore[wChairID]=-(int)(6*m_pGameServiceOption->dwCellScore);
			//统计额度
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i) continue;
				GameEnd.fScore[i] =m_pGameServiceOption->dwCellScore * 2;
				
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
	case GS_UG_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.fBaseScore=m_pGameServiceOption->dwCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_UG_CALL:		//叫牌状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//游戏变量
			StatusCall.wBankerUser=m_wBankerUser;
			StatusCall.cbPackCount=m_cbPackCount;
			StatusCall.cbMainValue=m_cbMainValue;
			StatusCall.cbValueOrder[0]=m_cbValueOrder[0];
			StatusCall.cbValueOrder[1]=m_cbValueOrder[1];

			//发牌变量
			StatusCall.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusCall.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusCall.cbHandCardCount[i] = m_cbHandCardCount[i];
			}

			//叫牌信息
			StatusCall.cbCallCard=m_cbCallCard;
			StatusCall.cbCallCount=m_cbCallCount;
			StatusCall.wCallCardUser=m_wCallCardUser;
			StatusCall.cbComplete=(m_bCallCard[wChiarID]==true)?TRUE:FALSE;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_UG_BACK:		//底牌状态
		{
			//构造数据
			CMD_S_StatusBack StatusBack;
			ZeroMemory(&StatusBack,sizeof(StatusBack));

			//游戏变量
			StatusBack.cbPackCount=m_cbPackCount;
			StatusBack.cbMainValue=m_cbMainValue;
			StatusBack.cbMainColor=m_cbMainColor;
			StatusBack.cbValueOrder[0]=m_cbValueOrder[0];
			StatusBack.cbValueOrder[1]=m_cbValueOrder[1];

			//叫牌变量
			StatusBack.cbCallCard=m_cbCallCard;
			StatusBack.cbCallCount=m_cbCallCount;
			StatusBack.wCallCardUser=m_wCallCardUser;

			//用户变量
			StatusBack.wBankerUser=m_wBankerUser;
			StatusBack.wCurrentUser=m_wCurrentUser;

			//扑克变量
			StatusBack.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusBack.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusBack.cbHandCardCount[i] = m_cbHandCardCount[i];
			}


			//底牌信息
			if (wChiarID==m_wBankerUser)
			{
				StatusBack.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusBack.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusBack,sizeof(StatusBack));
		}
	case GS_UG_PLAY:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.cbPackCount=m_cbPackCount;
			StatusPlay.cbMainValue=m_cbMainValue;
			StatusPlay.cbMainColor=m_cbMainColor;
			StatusPlay.cbValueOrder[0]=m_cbValueOrder[0];
			StatusPlay.cbValueOrder[1]=m_cbValueOrder[1];

			//叫牌变量
			StatusPlay.cbCallCard=m_cbCallCard;
			StatusPlay.cbCallCount=m_cbCallCount;
			StatusPlay.wCallCardUser=m_wCallCardUser;

			//用户变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wFirstOutUser=m_wFirstOutUser;

			//扑克变量
			StatusPlay.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusPlay.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//底牌信息
			if (wChiarID==m_wBankerUser)
			{
				StatusPlay.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusPlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//出牌变量
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusPlay.cbOutCardCount[i]=m_cbOutCardCount[i];
				CopyMemory(StatusPlay.cbOutCardData[i],m_cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i]);
				StatusPlay.cbHandCardCount[i] = m_cbHandCardCount[i];
			}

			//得分变量
			StatusPlay.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(StatusPlay.cbScoreCardData,m_cbScoreCardData,sizeof(StatusPlay.cbScoreCardData));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_UG_WAIT:	//等待状态
		{
			//构造数据
			CMD_S_StatusWait StatusWait;
			ZeroMemory(&StatusWait,sizeof(StatusWait));

			//用户变量
			StatusWait.wBankerUser=m_wBankerUser;

			//游戏变量
			StatusWait.cbPackCount=m_cbPackCount;
			StatusWait.cbMainValue=m_cbMainValue;
			StatusWait.cbValueOrder[0]=m_cbValueOrder[0];
			StatusWait.cbValueOrder[1]=m_cbValueOrder[1];

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusWait,sizeof(StatusWait));
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
	case GS_UG_CALL:		//叫牌状态
		{
			WORD wActionUser[GAME_PLAYER];
			BYTE bActionUserCount = 0;
			for(BYTE i = 0; i < GAME_PLAYER; i ++)
			{
				if(m_bCallCard[i] == true)
					continue;
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

			if(bActionUserCount)
			{
				//是否AI对杀普通玩家
				if(m_pITableFrame->IsAIKillGameUser())
				{
					if(CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
					{
						DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();

						BYTE cbWantCallMask = AI_GetCallCardMaskByLessCardCount(wGameUser);					
						for(BYTE i = 0; i < bActionUserCount; i ++)
						{
							WORD wChairID = wActionUser[i];

							BYTE cbCallMask = AI_GetCallCardMask(wChairID);
							IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
							if(cbWantCallMask & cbCallMask)
								AI_CallCard(wChairID,pIServerUserItem,Convert_CardMask_To_CardColor(cbWantCallMask));
							else
								OnGameMessage(SUB_C_CALL_FINISH, NULL, 0, pIServerUserItem);
						}
					}
					else
					{
						BYTE bSelAddGoldUser	= rand() % bActionUserCount;
						WORD wChairID = wActionUser[bSelAddGoldUser];
						IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

						BYTE cbCallMask = AI_GetCallCardMask(wChairID);
						if(cbCallMask == CALL_NONE)
						{
							OnGameMessage(SUB_C_CALL_FINISH, NULL, 0, pIServerUserItem);
						}
						else 
						{
							AI_CallCard(wChairID,pIServerUserItem,Convert_CardMask_To_CardColor(cbCallMask));
						}
					}
				}
				else
				{
					for(BYTE i = 0; i < bActionUserCount; i ++)
					{
						WORD wChairID = wActionUser[i];
						IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

						BYTE cbCallMask = AI_GetCallCardMask(wChairID);
						if(cbCallMask == CALL_NONE)
						{
							OnGameMessage(SUB_C_CALL_FINISH, NULL, 0, pIServerUserItem);
						}
						else 
						{
							AI_CallCard(wChairID,pIServerUserItem,Convert_CardMask_To_CardColor(cbCallMask));
						}
					}
				}
				

			}
		}
		break;
	case GS_UG_BACK:		//底牌状态
		{
			//机器人判断
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentUser);
			if(pIServerUserItem == NULL || 
				pIServerUserItem->IsAIUser() == false || 
				pIServerUserItem->GetUserStatus() != US_PLAY)
				return true;
			WORD wChairID = pIServerUserItem->GetChairID();
			if(wChairID == INVALID_CHAIR)
				return true;
			//埋底
			AI_ConcealCard(wChairID, pIServerUserItem);
		}
		break;
	case GS_UG_PLAY:		//游戏状态
		{
			//机器人判断
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentUser);
			if(pIServerUserItem == NULL || 
				pIServerUserItem->IsAIUser() == false || 
				pIServerUserItem->GetUserStatus() != US_PLAY)
				return true;
			WORD wChairID = pIServerUserItem->GetChairID();
			if(wChairID == INVALID_CHAIR)
				return true;
			AI_AutomatismOutCard(wChairID, pIServerUserItem);
		}
		break;
	default:
		break;
	}

	return true;
}
//叫牌掩码
BYTE CTableFrameSink::AI_GetCallCardMaskByLessCardCount(WORD wUser)
{
	BYTE cbColorDataCount[4];
	m_GameLogic.GetCardColorCount(m_cbHandCardData[wUser],
		m_cbHandCardCount[wUser],
		cbColorDataCount);
	BYTE nMin = 0;
	for(BYTE i = 1; i < 4; i ++)
	{
		if(cbColorDataCount[nMin] > cbColorDataCount[i])
			nMin = i;
	}

	BYTE CallMask[] = {CALL_FANG_KUAI,CALL_MEI_HUA,CALL_HONG_TAO,CALL_HEI_TAO};

	return CallMask[nMin];
}
//叫牌掩码
BYTE CTableFrameSink::AI_GetCallCardMask(WORD wCallCardUser)
{
	//变量定义
	BYTE cbCallMask=CALL_NONE;
	BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

	BYTE cbDispatchCardCount = m_cbHandCardCount[wCallCardUser];
	BYTE cbDispatchCardData[MAX_COUNT];
	memcpy(cbDispatchCardData, m_cbHandCardData[wCallCardUser], sizeof(cbDispatchCardData));

	//辅助变量
	BYTE cbCardCount[6]={0,0,0,0,0,0};
	BYTE cbCallMaskArray[6]={CALL_FANG_KUAI,CALL_MEI_HUA,CALL_HONG_TAO,CALL_HEI_TAO,CALL_NT,CALL_NT};
	BYTE cbCallColorArray[6]={COLOR_FANG_KUAI,COLOR_MEI_HUA,COLOR_HONG_TAO,COLOR_HEI_TAO,COLOR_NT,COLOR_NT};

	//更新调整
	if (m_wCallCardUser!=wCallCardUser)
	{
		//扑克统计
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			//获取属性
			BYTE cbCardColor=m_GameLogic.GetCardColor(cbDispatchCardData[i]);
			BYTE cbCardValue=m_GameLogic.GetCardValue(cbDispatchCardData[i]);

			//数据统计
			if (cbCardColor==COLOR_NT) cbCardCount[4+cbCardValue-0x0E]++;
			else if (cbCardValue==m_cbMainValue) cbCardCount[cbCardColor>>4]++;
		}

		//主牌控制
		for (BYTE i=0;i<CountArray(cbCardCount);i++)
		{
			//规则判断
			if ((cbCardCount[i]==0)||(cbCardCount[i]<m_cbCallCount)) continue;

			//数目判断
			if ((i<4)&&(cbCardCount[i]>m_cbCallCount))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}

			//无主判断
			if ((i>=4)&&(cbCardCount[i]>m_cbCallCount)&&(cbCardCount[i]>=2))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}

			//花色判断
			BYTE cbPackCount=m_GameLogic.GetPackCount();
			if ((cbCardCount[i]==cbPackCount)&&(cbCallColorArray[i]>cbCallColor))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}
		}
	}
	else
	{
		//扑克统计
		BYTE cbCallCardCount=0;
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			if (cbDispatchCardData[i]==m_cbCallCard) cbCallCardCount++;
		}

		//叫牌控制
		if (cbCallCardCount>m_cbCallCount) cbCallMask=cbCallMaskArray[cbCallColor>>4];
	}


	return cbCallMask;
}
//叫牌
bool CTableFrameSink::AI_CallCard(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbCallColor)
{
	//变量定义
	BYTE cbCallCount=0;
	BYTE cbCallCard=0x00;

	BYTE cbDispatchCardCount = m_cbHandCardCount[wChairID];
	BYTE cbDispatchCardData[MAX_COUNT];
	memcpy(cbDispatchCardData, m_cbHandCardData[wChairID], sizeof(cbDispatchCardData));

	//数据统计
	if (cbCallColor==COLOR_NT)
	{
		//数目统计
		BYTE cbNTCount[2]={0,0};
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			BYTE cbCardData=cbDispatchCardData[i];
			if ((cbCardData==0x4E)||(cbCardData==0x4F)) cbNTCount[cbCardData-0x4E]++;
		}

		//叫牌数据
		if (cbNTCount[0]>=cbNTCount[1])
		{
			cbCallCard=0x4E;
			cbCallCount=cbNTCount[0];
		}
		else
		{
			cbCallCard=0x4F;
			cbCallCount=cbNTCount[1];
		}
	}
	else
	{
		//数目统计
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			//获取属性
			BYTE cbCardData=cbDispatchCardData[i];
			BYTE cbCardColor=m_GameLogic.GetCardColor(cbCardData);
			BYTE cbCardValue=m_GameLogic.GetCardValue(cbCardData);

			//数据统计
			if ((cbCardValue==m_cbMainValue)&&(cbCardColor==cbCallColor)) 
			{
				cbCallCount++;
				cbCallCard=cbCardData;
			}
		}
	}

	if(cbCallCount)
	{
		//构造数据
		CMD_C_CallCard CallCard;
		CallCard.cbCallCard=cbCallCard;
		CallCard.cbCallCount=cbCallCount;

		//发送数据
		OnGameMessage(SUB_C_CALL_CARD,&CallCard,sizeof(CallCard), pIServerUserItem);
	}

	return true;
}
//埋底
bool CTableFrameSink::AI_ConcealCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	BYTE cbDispatchCardCount = m_cbHandCardCount[wChairID];
	BYTE cbDispatchCardData[MAX_COUNT];
	memcpy(cbDispatchCardData, m_cbHandCardData[wChairID], sizeof(cbDispatchCardData));

	BYTE	cbConcealCount = m_cbConcealCount;	//暗藏数目
	BYTE	cbConcealCard[8];					//暗藏扑克

	//提取单牌
	BYTE cbResultCard[MAX_COUNT];
	BYTE cbResultCount = m_GameLogic.DistillSingleCard(cbDispatchCardData,
		cbDispatchCardCount, 
		m_cbMainColor, 
		m_cbMainValue, 
		cbResultCard);
	//判断单牌足够
	if(cbResultCount >= m_cbConcealCount)
	{
		m_GameLogic.SortCardList(cbResultCard,cbResultCount);
		memcpy(cbConcealCard, cbResultCard + (cbResultCount - cbConcealCount) , cbConcealCount);
	}
	else
	{
		//保存单牌
		memcpy(cbConcealCard, cbResultCard, cbResultCount);
		//删除牌
		m_GameLogic.RemoveCard(cbResultCard,cbResultCount, cbDispatchCardData, cbDispatchCardCount);
		cbDispatchCardCount -= cbResultCount;
		
		//补充牌
		m_GameLogic.SortCardList(cbDispatchCardData,cbDispatchCardCount);
		BYTE cbMoreCount = m_cbConcealCount - cbResultCount;
		memcpy(cbConcealCard + cbResultCount, cbDispatchCardData + (cbDispatchCardCount - cbMoreCount) , cbMoreCount);
	}


	//构造数据
	CMD_C_ConcealCard ConcealCard;
	ConcealCard.cbConcealCount=cbConcealCount;
	CopyMemory(ConcealCard.cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//发送数据
	OnGameMessage(SUB_C_CONCEAL_CARD,&ConcealCard,sizeof(ConcealCard), pIServerUserItem);

	return true;
}
//出牌
bool CTableFrameSink::AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//变量定义
	BYTE cbHandCardData[MAX_COUNT];
	memcpy(cbHandCardData, m_cbHandCardData[wChairID], sizeof(cbHandCardData));
	BYTE cbHandCardCount = m_cbHandCardCount[wChairID];

	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	bool bSearchOutCard = false;
	BYTE cbTurnCardCount=(wChairID==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser];
	//是否AI对杀普通玩家
	if(m_pITableFrame->IsAIKillGameUser())
	{
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
		//判断机器人是否普通玩家的对家
		if(wGameUser%2 == wChairID%2)
		{
			
		}
		else
		{
			//判断是否第一出牌
			if(cbTurnCardCount == 0)
			{
				//寻找最佳出牌
				if(AI_SearchFirstBestOutCard(cbHandCardData,
					cbHandCardCount,
					wChairID,
					OutCardResult))
				{
					bSearchOutCard = true;
				}
				
			}
			else
			{
				
			}
		}
	}
	//随机出牌
	{
		//搜索出牌
		if(bSearchOutCard == false)
			m_GameLogic.SearchOutCard(cbHandCardData,
				cbHandCardCount,
				m_cbOutCardData[m_wFirstOutUser],
				cbTurnCardCount,
				OutCardResult);

		//发送数据
		CMD_C_OutCard OutCard;
		OutCard.cbCardCount=OutCardResult.cbCardCount;
		CopyMemory(OutCard.cbCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCard.cbCardCount);
		OnGameMessage(SUB_C_OUT_CARD,&OutCard,
			sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE), pIServerUserItem);

	}

	return true;
}
//寻找最佳出牌
bool CTableFrameSink::AI_SearchFirstBestOutCard(BYTE cbHandCardData[],
							   BYTE cbHandCardCount,
							   WORD wChairID,
							   tagOutCardResult& OutCardResult)
{
	memset(&OutCardResult, 0, sizeof(OutCardResult));


	return false;
}
//判断更大的牌存在
bool CTableFrameSink::AI_CompareMostCardExist(BYTE cbHandCardData[],
							 BYTE cbHandCardCount,tagTractorDataInfo TractorDataInfo)
{
	tagAnalyseMostResult AnalyseResult;
	m_GameLogic.DistillMostCard(cbHandCardData, cbHandCardCount, AnalyseResult);
	BYTE cbCardColor = m_GameLogic.GetCardColor(TractorDataInfo.cbTractorData[2]);
	BYTE cbAnalyseMostResultIndex = m_GameLogic.GetAnalyseMostResultIndexByCardColor(cbCardColor);

	for(BYTE i = 0; i < AnalyseResult.cbTractorDataInfoCount[cbAnalyseMostResultIndex]; i ++)
	{
		if(m_GameLogic.CompareCardResult(TractorDataInfo, AnalyseResult.TractorDataInfo[cbAnalyseMostResultIndex][i])) return true;
	}
	return false;
}
//判断更大的牌存在
bool CTableFrameSink::AI_CompareMostCardExist(BYTE cbHandCardData[],
											  BYTE cbHandCardCount,tagSameDataInfo SameDataInfo)
{
	tagAnalyseMostResult AnalyseResult;
	m_GameLogic.DistillMostCard(cbHandCardData, cbHandCardCount, AnalyseResult);
	BYTE cbCardColor = m_GameLogic.GetCardColor(SameDataInfo.cbSameData[0]);
	BYTE cbAnalyseMostResultIndex = m_GameLogic.GetAnalyseMostResultIndexByCardColor(cbCardColor);

	for(BYTE i = 0; i < AnalyseResult.cbSameDataInfoInfoCount[cbAnalyseMostResultIndex]; i ++)
	{
		if(m_GameLogic.CompareCardResult(SameDataInfo,AnalyseResult.SameDataInfo[cbAnalyseMostResultIndex][i])) return true;
	}
	return false;
}
//判断更大的单牌存在
bool CTableFrameSink::AI_CompareMostCardExist(BYTE cbHandCardData[],
											  BYTE cbHandCardCount,BYTE cbCardData)
{
	for(BYTE i = 0; i < cbHandCardCount; i ++)
	{
		if(m_GameLogic.CompareCardData(cbCardData,cbHandCardData[i])) return true;
	}
	return false;
}
//寻找匹配出牌
bool CTableFrameSink::AI_SearchMatchOutCard(BYTE cbHandCardData[],
						   BYTE cbHandCardCount,
						   BYTE cbMatchCardData[],
						   BYTE cbMatchCardCount,
						   tagOutCardResult& OutCardResult)
{
	

	return false;
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
	case SUB_C_CALL_CARD:		//用户叫牌
		{
			//效验数据
			GT_ASSERT(wDataSize==sizeof(CMD_C_CallCard));
			if (wDataSize!=sizeof(CMD_C_CallCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_CallCard * pCallCard=(CMD_C_CallCard *)pDataBuffer;
			return OnUserCallCard(pUserData->UserStatus.wChairID,pCallCard->cbCallCard,pCallCard->cbCallCount);
		}
	case SUB_C_CALL_FINISH:		//叫牌完成
		{
			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserCallFinish(pUserData->UserStatus.wChairID);
		}
	case SUB_C_CONCEAL_CARD:	//底牌扑克
		{
			//效验数据
			GT_ASSERT(wDataSize==sizeof(CMD_C_ConcealCard));
			if (wDataSize!=sizeof(CMD_C_ConcealCard)) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			CMD_C_ConcealCard * pConcealCard=(CMD_C_ConcealCard *)pDataBuffer;
			return OnUserConcealCard(pUserData->UserStatus.wChairID,pConcealCard->cbConcealCard,pConcealCard->cbConcealCount);
		}
	case SUB_C_OUT_CARD:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//效验数据
			GT_ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			GT_ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//消息处理
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	}

	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户起来
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置变量
	if (bLookonUser==false)
	{
		m_cbMainValue=0x02;
		m_cbValueOrder[0]=0x02;
		m_cbValueOrder[1]=0x02;
		m_wBankerUser=INVALID_CHAIR;
	}

	return true;
}

//用户叫牌
bool CTableFrameSink::OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount)
{
	//效验状态
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//用户效验
	GT_ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//数目统计
	BYTE cbHandCardCount=0;
	for (BYTE i=0;i<m_cbHandCardCount[wChairID];i++)
	{
		if (m_cbHandCardData[wChairID][i]==cbCallCard) cbHandCardCount++;
	}

	//效验数据
	GT_ASSERT((cbHandCardCount>=cbCallCount)&&(cbCallCount>0));
	if ((cbHandCardCount<cbCallCount)||(cbCallCount==0)) return false;

	//叫牌判断
	if (m_wCallCardUser==wChairID)
	{
		//效验数据
		GT_ASSERT((cbCallCard==m_cbCallCard)&&(cbCallCount>m_cbCallCount));
		if ((cbCallCard!=m_cbCallCard)||(cbCallCount<=m_cbCallCount)) return true;
	}
	else
	{
		//效验数据
		BYTE cbPackCount=m_GameLogic.GetPackCount();
		BYTE cbCardColor=m_GameLogic.GetCardColor(cbCallCard);
		BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

		//效验数据
		GT_ASSERT((cbCallCount>m_cbCallCount)||((cbCallCount==cbPackCount)&&(cbCardColor>cbCallColor)));
		if ((cbCallCount<=m_cbCallCount)&&((cbCallCount!=cbPackCount)||(cbCardColor<=cbCallColor))) return true;
	}

	//设置变量
	m_cbCallCard=cbCallCard;
	m_wCallCardUser=wChairID;
	m_cbCallCount=cbCallCount;

	//构造数据
	CMD_S_CallCard CallCard;
	CallCard.cbCallCard=m_cbCallCard;
	CallCard.cbCallCount=m_cbCallCount;
	CallCard.wCallCardUser=m_wCallCardUser;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));

	return true;
}

//叫牌完成
bool CTableFrameSink::OnUserCallFinish(WORD wChairID)
{
	WORD i = 0;
	//效验状态
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//用户效验
	GT_ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//变量设置
	m_bCallCard[wChairID]=true;

	//完成判断
	for (i=0;i<m_wPlayerCount;i++)
	{
		if (m_bCallCard[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;

	//庄家设置
	if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wCallCardUser;

	//重新发牌
	if (m_wBankerUser==INVALID_CHAIR)
	{
		//设置状态
		ZeroMemory(m_bCallCard,sizeof(m_bCallCard));

		//发送扑克
		DispatchUserCard();

		return true;
	}

	//花色调整
	if (m_wCallCardUser==INVALID_CHAIR) m_cbMainColor=COLOR_NT;
	else m_cbMainColor=m_GameLogic.GetCardColor(m_cbCallCard);

	//设置变量
	m_wCurrentUser=m_wBankerUser;
	m_wFirstOutUser=m_wBankerUser;
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_pITableFrame->SetGameStatus(GS_UG_BACK);

	//设置底牌
	BYTE cbHandCardCount=m_cbHandCardCount[m_wCurrentUser];
	CopyMemory(&m_cbHandCardData[m_wCurrentUser][cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
	m_cbHandCardCount[m_wCurrentUser]+=m_cbConcealCount;

	//构造数据
	CMD_S_SendConceal SendConceal;
	SendConceal.cbMainColor=m_cbMainColor;
	SendConceal.wBankerUser=m_wBankerUser;
	SendConceal.wCurrentUser=m_wCurrentUser;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//构造底牌
		if (i==m_wCurrentUser)
		{
			SendConceal.cbConcealCount=m_cbConcealCount;
			CopyMemory(SendConceal.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		}
		else 
		{
			SendConceal.cbConcealCount=0;
			ZeroMemory(SendConceal.cbConcealCard,sizeof(SendConceal.cbConcealCard));
		}

		//发送消息
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
	}

	return true;
}

//底牌扑克
bool CTableFrameSink::OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount)
{
	//效验状态
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_BACK);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_BACK) return true;

	//效验数据
	GT_ASSERT((wChairID==m_wCurrentUser)&&(m_cbConcealCount==cbConcealCount));
	if ((wChairID!=m_wCurrentUser)||(m_cbConcealCount!=cbConcealCount)) return false;

	//删除底牌
	if (m_GameLogic.RemoveCard(cbConcealCard,cbConcealCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		GT_ASSERT(FALSE);
		return false;
	}

	//设置扑克
	m_cbHandCardCount[wChairID]-=cbConcealCount;
	CopyMemory(m_cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//扑克排列
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		GT_ASSERT(m_cbHandCardCount[i]==m_GameLogic.GetDispatchCount());
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
	}

	//设置状态
	m_pITableFrame->SetGameStatus(GS_UG_PLAY);

	//构造数据
	CMD_S_GamePlay GamePlay;
	GamePlay.wCurrentUser=m_wCurrentUser;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//构造底牌
		if (i==m_wCurrentUser)
		{
			GamePlay.cbConcealCount=m_cbConcealCount;
			CopyMemory(GamePlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		}
		else 
		{
			GamePlay.cbConcealCount=0;
			ZeroMemory(GamePlay.cbConcealCard,sizeof(GamePlay.cbConcealCard));
		}

		//发送消息
		m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//效验状态
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_PLAY) return true;

	//效验用户
	GT_ASSERT((wChairID==m_wCurrentUser)&&(cbCardCount<=MAX_COUNT));
	if ((wChairID!=m_wCurrentUser)||(cbCardCount>MAX_COUNT)) return false;

	//首出牌者
	if (m_wFirstOutUser==wChairID)
	{
		//类型判断
		BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
		if (cbCardType==CT_ERROR) return false;

		//甩牌处理
		if (cbCardType==CT_THROW_CARD)
		{
			//甩牌判断
			tagOutCardResult OutCardResult;
			ZeroMemory(&OutCardResult,sizeof(OutCardResult));
			bool bThrowSuccess=m_GameLogic.EfficacyThrowCard(cbCardData,cbCardCount,wChairID,m_cbHandCardData,m_cbHandCardCount[wChairID],OutCardResult);

			//结果处理
			if (bThrowSuccess==false)
			{
				//删除扑克
				if (m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
				m_cbHandCardCount[wChairID]-=OutCardResult.cbCardCount;

				//出牌记录
				m_cbOutCardCount[wChairID]=OutCardResult.cbCardCount;
				CopyMemory(m_cbOutCardData[wChairID],OutCardResult.cbResultCard,OutCardResult.cbCardCount);

				//切换用户
				m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

				//构造数据
				CMD_S_ThrowResult ThrowResult;
				ThrowResult.wOutCardUser=wChairID;
				ThrowResult.wCurrentUser=m_wCurrentUser;
				ThrowResult.cbThrowCardCount=cbCardCount;
				ThrowResult.cbResultCardCount=OutCardResult.cbCardCount;
				CopyMemory(&ThrowResult.cbCardDataArray[0],cbCardData,sizeof(BYTE)*cbCardCount);
				CopyMemory(&ThrowResult.cbCardDataArray[cbCardCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);

				//发送计算
				BYTE cbCardDataCount=ThrowResult.cbThrowCardCount+ThrowResult.cbResultCardCount;
				WORD wSendSize=sizeof(ThrowResult)-sizeof(ThrowResult.cbCardDataArray)+cbCardDataCount*sizeof(BYTE);

				//发送数据
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);

				return true;
			}
		}
	}
	else
	{
		//数目判断
		GT_ASSERT(cbCardCount==m_cbOutCardCount[m_wFirstOutUser]);
		if (cbCardCount!=m_cbOutCardCount[m_wFirstOutUser]) return false;

		//出牌效验
		if (m_GameLogic.EfficacyOutCard(cbCardData,cbCardCount,m_cbOutCardData[m_wFirstOutUser],
			m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
		{
			GT_ASSERT(FALSE);
			return false;
		}
	}

	//删除扑克
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
	m_cbHandCardCount[wChairID]-=cbCardCount;

	//出牌记录
	m_cbOutCardCount[wChairID]=cbCardCount;
	CopyMemory(m_cbOutCardData[wChairID],cbCardData,cbCardCount);

	//切换用户
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wFirstOutUser) m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));

	//发送数据
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//一轮结算
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//判断胜者
		m_wTurnWinner=m_GameLogic.CompareCardArray(m_cbOutCardData,m_cbOutCardCount[0],m_wFirstOutUser);

		//计算得分
		if ((m_wTurnWinner!=m_wBankerUser)&&((m_wTurnWinner+2)%m_wPlayerCount!=m_wBankerUser))
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
				BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
				m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i],m_cbOutCardCount[i],pcbScoreCard,cbMaxCount);
			}
		}

		//设置变量
		if (m_cbHandCardCount[m_wTurnWinner]!=0)
		{
			m_wFirstOutUser=m_wTurnWinner;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
		}

		//用户切换
		m_wCurrentUser=(m_cbHandCardCount[m_wTurnWinner]==0)?INVALID_CHAIR:m_wTurnWinner;

		//构造数据
		CMD_TurnBalance TurnBalance;
		TurnBalance.wTurnWinner=m_wTurnWinner;
		TurnBalance.wCurrentUser=m_wCurrentUser;

		//发送数据
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
	}

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(m_wTurnWinner,NULL,GER_NORMAL);

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchUserCard()
{
	//混乱扑克
	BYTE cbRandCard[CELL_PACK*2];
	m_GameLogic.RandCardList(cbRandCard,CELL_PACK*m_cbPackCount);

	//用户扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbHandCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//底牌扑克
	m_cbConcealCount=CELL_PACK*m_cbPackCount-m_wPlayerCount*m_GameLogic.GetDispatchCount();
	CopyMemory(m_cbConcealCard,&cbRandCard[CELL_PACK*m_cbPackCount-m_cbConcealCount],sizeof(BYTE)*m_cbConcealCount);

	//发送开始
	CMD_S_SendCard SendCard;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//效验状态
		GT_ASSERT(m_cbHandCardCount[i]<=CountArray(SendCard.cbCardData));
		if (m_cbHandCardCount[i]>CountArray(SendCard.cbCardData)) return false;

		//拷贝扑克
		CopyMemory(SendCard.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
		SendCard.cbCardCount=m_cbHandCardCount[i];
		CopyMemory(SendCard.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//发送统计
		WORD wHeadSize=sizeof(SendCard)-sizeof(SendCard.cbCardData);
		WORD wSendSize=wHeadSize+sizeof(SendCard.cbCardData[0])*SendCard.cbCardCount;

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
