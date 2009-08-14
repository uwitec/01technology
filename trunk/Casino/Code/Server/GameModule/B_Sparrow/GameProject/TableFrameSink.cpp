#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////
CCalculateSink::CCalculateSink()
{
	m_pGameEnd = NULL;
}
CCalculateSink::~CCalculateSink()
{

}
//�ӿڲ�ѯ
void * __cdecl CCalculateSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICalculateSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICalculateSink,Guid,dwQueryVer);
	return NULL;
}
//ICalculateSink�ӿ�

//�����ɲ�
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

	//ת��Double�ɲ�ΪDecimal�ɲ�
	DECIMAL decTemp;
	memset(&decTemp, 0, sizeof(DECIMAL));
	VarDecFromR8(m_pGameEnd->fGameScore[wChairID], &decTemp);
	VarDecAdd(&decTemp, &pPartedWinScore[0], &pPartedWinScore[0]);

	//�жϴ���0,���г�ˮ����
	if(VarDecCmpR8(&decTemp, 0) == VARCMP_GT)
	{
		//��ˮ = ���ɲ� * (- ��ˮ��)
		VarDecMul(&decTemp,(DECIMAL*) &m_decAfterTax, &pPartedTaxScore[0]);
		//����Double�ɲ�
		VarDecAdd(&decTemp,&pPartedTaxScore[0], &decTemp);
		VarR8FromDec(&decTemp, &m_pGameEnd->fGameScore[wChairID]);
	}

	return true;
}
//����ϴ��
bool __cdecl CCalculateSink::RectifyValidBetScore(void* pCalculateContext,
												  ICalculateItem* pItem,
												  DECIMAL* pPartedBetScore,
												  WORD	 wBetRegionCount,
												  DECIMAL* pValidBetScore)
{
	GT_ASSERT(m_pTableFrameSink != NULL);
	bool bFlag = false;
	//�жϻ�ׯ
	if(m_pTableFrameSink->EstimateNullChiHuCard())
	{
		bFlag = true;
	}
	else
	{
		/*
		//�жϳԺ�
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

	//��ȡ����
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
//��ʼ������������
bool __cdecl CCalculateSink::InitCalculateContext(CMD_S_GameEnd* pGameEnd)
{
	GT_ASSERT(pGameEnd != NULL);
	m_pGameEnd = pGameEnd;

	return true;
}
//��ȡ����������
void* __cdecl CCalculateSink::GetCalculateContext()
{
	return m_pGameEnd;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//Run����
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pICalculateFrame=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	GT_ASSERT(m_pGameServiceOption!=NULL);

	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	GT_ASSERT(m_pGameServiceAttrib!=NULL);

	m_pICalculateFrame = m_pITableFrame->GetICalculateFrame();
	GT_ASSERT(m_pICalculateFrame != NULL);

	return m_CalculateSink.InitCalculateSink(m_pITableFrame, this);

}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//Run����
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//�����˿�
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//��������
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�������
		//m_cbUserAction[i]|=m_GameLogic.EstimateChiHu(m_cbCardIndex[i]);

		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);

			//�����ж�
			tagChiHuResult ChiHuResult;
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult);
		}

		//״̬����
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;
			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	//Ͷע
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

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			DOUBLE fCellScore=m_pGameServiceOption->dwCellScore;
			BYTE cbBirdValue=(m_cbRepertoryCard[0]&MASK_VALUE);

			//������Ϣ
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			for (WORD i=0;i<m_wPlayerCount;i++) GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;

			//��������
			WORD wChiHuUserCount=0;

			//ͳ�ƶ��
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//��������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)
				{
					//��������
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (i==m_wProvideUser) continue;

						//���ͳ��
						GameEnd.fGameScore[i] -= cbChiHuOrder * fCellScore;
						GameEnd.fGameScore[m_wProvideUser] += cbChiHuOrder * fCellScore;
					}

					//������Ŀ
					wChiHuUserCount=1;

					//ׯ������
					m_wBankerUser=m_wProvideUser;
				}

				//׽������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL)
				{
					//��������
					WORD wNextBankerUser=INVALID_CHAIR;

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//��ҹ���
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

						//��������
						BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);

						//���ͳ��
						GameEnd.fGameScore[i] += cbChiHuOrder * fCellScore;
						GameEnd.fGameScore[m_wProvideUser] -= cbChiHuOrder * fCellScore;

						//���ñ���
						wNextBankerUser=i;
						wChiHuUserCount++;
					}

					//ׯ������
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
				}

				//���Ƽ���
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					for(WORD j=0; j < m_cbWeaveItemCount[i]; j ++)
					{
						if (m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG)//����
						{
							if(m_WeaveItemArray[i][j].wProvideUser == i)//�Ը�
							{
								if(m_WeaveItemArray[i][j].cbPublicCard == FALSE)
								{
									//����
									for (WORD k=0;k<m_wPlayerCount;k++)
									{
										if(i == k)continue;
										GameEnd.fGameScore[k] -= fCellScore;
										GameEnd.fGameScore[i] += fCellScore;
									}
								}
								else
								{	//���
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
								//����
								GameEnd.fGameScore[m_WeaveItemArray[i][j].wProvideUser] -= fCellScore;
								GameEnd.fGameScore[i] += fCellScore;
							}
						}
					}
				}
			}
			else if ((m_cbOutCardCount==0)&&(m_cbSendCardCount==1))
			{
				//��������
				WORD wNextBankerUser=INVALID_CHAIR;

				//�������
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��ҹ���
					if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

					//��������
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);

					//ѭ���ۼ�
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

					//���ñ���
					wChiHuUserCount++;
					wNextBankerUser=i;
				}

				//ׯ������
				if (wChiHuUserCount==1) m_wBankerUser=wNextBankerUser;

				//���Ƽ���
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					for(WORD j=0; j < m_cbWeaveItemCount[i]; j ++)
					{
						if (m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG)//����
						{
							if(m_WeaveItemArray[i][j].wProvideUser == i)//�Ը�
							{
								if(m_WeaveItemArray[i][j].cbPublicCard == FALSE)
								{
									//����
									for (WORD k=0;k<m_wPlayerCount;k++)
									{
										if(i == k)continue;
										GameEnd.fGameScore[k] -= fCellScore;
										GameEnd.fGameScore[i] += fCellScore;
									}
								}
								else
								{	//���
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
								//����
								GameEnd.fGameScore[m_WeaveItemArray[i][j].wProvideUser] -= fCellScore;
								GameEnd.fGameScore[i] += fCellScore;
							}
						}
					}
				}
			}
			else
			{
				//��ׯ����
				GameEnd.cbChiHuCard=0;
			}

			GameEnd.wSiceCount=m_wSiceCount;
			GameEnd.wBankerUser=m_wBankerUser;
			//����˿�
			CopyMemory(GameEnd.WeaveItemArray,m_WeaveItemArray,sizeof(tagWeaveItem) * GAME_PLAYER * 4);
			CopyMemory(GameEnd.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd, sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);



				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//������Ϸ
				m_pITableFrame->ConcludeGame();

				return true;
			}
			else
			{
				GT_ASSERT(FALSE);
				//������Ϸ
				m_pITableFrame->ConcludeGame();
				return false;
			}

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.fGameScore[wChairID]=-(int)(6*m_pGameServiceOption->dwCellScore);
			//ͳ�ƶ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i) continue;
				GameEnd.fGameScore[i]=(int)(2*m_pGameServiceOption->dwCellScore);
				
			}
			GameEnd.wSiceCount=m_wSiceCount;
			GameEnd.wBankerUser=m_wBankerUser;
			//����˿�
			CopyMemory(GameEnd.WeaveItemArray,m_WeaveItemArray,sizeof(tagWeaveItem) * GAME_PLAYER * 4);
			CopyMemory(GameEnd.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_USER_LEFT,(BYTE*)&GameEnd, sizeof(GameEnd),
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


				//������Ϸ
				m_pITableFrame->ConcludeGame();

				return true;

			}
			else
			{
				GT_ASSERT(FALSE);
				//������Ϸ
				m_pITableFrame->ConcludeGame();
				return false;
			}


		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.fCellScore=m_pGameServiceOption->dwCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY://��Ϸ״̬
		{
		}
	case GS_MJ_HAI_DI:	//����״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.fCellScore=m_pGameServiceOption->dwCellScore;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(tagWeaveItem) * GAME_PLAYER * 4);
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],StatusPlay.cbCardData);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	default:
		break;
	}

	return false;
}
//�˹�������Ϸ����
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	WORD wCurrentUser = m_wCurrentUser;
	if(m_wCurrentUser == INVALID_CHAIR)
	{
		//�漴�����˽��в���
		WORD wActionUser[GAME_PLAYER];
		BYTE bActionUserCount = 0;
		for(BYTE i = 0; i < GAME_PLAYER; i ++)
		{
			BYTE cbActionMask=(m_bResponse[i]==false)?m_cbUserAction[i]:WIK_NULL;
			if(cbActionMask!=WIK_NULL)
			{
				//�������ж�
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
	//�������ж�
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
	case GS_MJ_PLAY://��Ϸ״̬
	case GS_MJ_HAI_DI://����״̬
		{
			BYTE cbActionCard=m_cbProvideCard;
			BYTE cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;
			//�жϳ����ܲ���
			if(cbActionMask!=WIK_NULL)
			{
				//��������
				tagGangCardResult GangCardResult;
				ZeroMemory(&GangCardResult,sizeof(GangCardResult));

				//�����ж�
				if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
				{
					//�������
					if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
					{
						GangCardResult.cbCardCount=1;
						GangCardResult.cbCardData[0]=cbActionCard;
					}

					//�Լ�����
					if ((m_wCurrentUser==wChairID)||(cbActionCard==0))
					{
						m_GameLogic.AnalyseGangCard(m_cbCardIndex[wChairID],
							m_WeaveItemArray[wChairID],
							m_cbWeaveItemCount[wChairID],
							GangCardResult);
					}
				}
				
				//ִ�г����ܲ���
				AI_AutomatismOperate(wChairID, pIServerUserItem, cbActionCard,cbActionMask,GangCardResult);
			
			}
			else
			{
				//����
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
//�û�����
bool CTableFrameSink::AI_AutomatismOutCard(WORD wChairID, IServerUserItem *pIServerUserItem)
{
	BYTE cbCardData[14];
	BYTE cbCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],cbCardData);
	GT_ASSERT(cbCardCount > 0);
	if(cbCardCount == 0)
		return false;
	//�Ƿ�AI��ɱ��ͨ���
	if(m_pITableFrame->IsAIKillGameUser())
	{
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();

		//1 ѡ��������������Ҫ�ĵ���
		BYTE cbSingleCardData[14];
		int  nSingleCardDataCount = 0;
		for(BYTE i = 0; i < cbCardCount; i ++)
		{
			if(m_GameLogic.EstimateEatCard(m_cbCardIndex[wChairID], cbCardData[i]) == WIK_NULL &&
				m_GameLogic.EstimatePengCard(m_cbCardIndex[wChairID], cbCardData[i]) == WIK_NULL &&
				m_GameLogic.EstimateGangCard(m_cbCardIndex[wChairID], cbCardData[i]) == WIK_NULL)
			{
				//��¼����
				cbSingleCardData[nSingleCardDataCount ++] = cbCardData[i];

				//Ѱ����ƥ���ƵĻ�����
				WORD wBestMachChairID = AI_BestMachChairID(wChairID, wGameUser, cbCardData[i]);
				if(wBestMachChairID != INVALID_CHAIR)
				{
					//��������
					CMD_C_OutCard OutCard;
					OutCard.cbCardData=cbCardData[i];
					OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

					return true;

				}
			}

		}
		//2 �������
		if(nSingleCardDataCount > 0)
		{
			for(BYTE i = 0; i < nSingleCardDataCount; i ++)
			{
				//����Ȩλ
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

				//�ж���ͨ��Ҹ��ƻ��ߺ���
				tagChiHuResult ChiHuResult;
				if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], cbSingleCardData[i]) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
					m_WeaveItemArray[wGameUser],
					m_cbWeaveItemCount[wGameUser],
					cbSingleCardData[i],
					wChiHuRight,
					ChiHuResult) != WIK_NULL)
				{
					//����������-�൱���ʸ���ͨ��Ҹ��ƻ��ߺ���
					if(CAIUtil::DetectionProbability(100 - m_pGameServiceOption->cbAILevel))
					{	
						//��������
						CMD_C_OutCard OutCard;
						OutCard.cbCardData=cbSingleCardData[i];
						OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

						return true;
					}
				}
				else
				{
					//��������
					CMD_C_OutCard OutCard;
					OutCard.cbCardData=cbSingleCardData[i];
					OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

					return true;
				}
			}
		}
		//3 ѡ�ҳ���
		memcpy(cbSingleCardData, cbCardData, sizeof(cbCardData));
		nSingleCardDataCount = cbCardCount;
		while(nSingleCardDataCount > 0)
		{
			i = rand() % nSingleCardDataCount;
			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

			//�ж���ͨ��Ҹ��ƻ��ߺ���
			tagChiHuResult ChiHuResult;
			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], cbSingleCardData[i]) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
				m_WeaveItemArray[wGameUser],
				m_cbWeaveItemCount[wGameUser],
				cbSingleCardData[i],
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				//����������-�൱���ʸ���ͨ��Ҹ��ƻ��ߺ���
				if(CAIUtil::DetectionProbability(100 - m_pGameServiceOption->cbAILevel))
				{	
					//��������
					CMD_C_OutCard OutCard;
					OutCard.cbCardData=cbSingleCardData[i];
					OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

					return true;
				}
			}
			else
			{
				//��������
				CMD_C_OutCard OutCard;
				OutCard.cbCardData=cbSingleCardData[i];
				OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);

				return true;
			}

			nSingleCardDataCount = nSingleCardDataCount - 1;
		}
	}
	
	{
		//�������
		BYTE i = rand() % cbCardCount;

		//��������
		CMD_C_OutCard OutCard;
		OutCard.cbCardData=cbCardData[i];
		OnGameMessage(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard), pIServerUserItem);
	}
	
	return true;
}
//Ѱ����ƥ���ƵĻ�����
WORD CTableFrameSink::AI_BestMachChairID(WORD wOutCardChairID, WORD wGameUserChairID, BYTE bCard)
{
	BYTE i = wOutCardChairID + 1;
	WORD wChairID = i % GAME_PLAYER;
	do {
		if(wChairID != wGameUserChairID)
		{
			tagChiHuResult ChiHuResult;
			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

			//�ж���ͨ��Ҹ��ƻ��ߺ���
			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUserChairID], bCard) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUserChairID],
				m_WeaveItemArray[wGameUserChairID],
				m_cbWeaveItemCount[wGameUserChairID],
				bCard,
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				//����������-�൱���ʸ���ͨ��Ҹ��ƻ��ߺ���
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
//�û�����
bool CTableFrameSink::AI_AutomatismOperate(WORD wChairID, IServerUserItem *pIServerUserItem,
						  BYTE cbActionCard,BYTE cbActionMask,tagGangCardResult& GangCardResult)
{
	//���ñ���
	BYTE			cbCenterCard;		//�����齫
	BYTE			cbGangCard[5];		//��������

	//״̬����
	BYTE			cbItemCount;		//������Ŀ
	BYTE			cbCurrentItem;		//��ǰ����

	//���ñ���
	cbItemCount=0;
	cbCurrentItem=0xFF;
	cbCenterCard=cbActionCard;

	//������Ϣ
	ZeroMemory(cbGangCard,sizeof(cbGangCard));
	for (BYTE i=0;i<GangCardResult.cbCardCount;i++) 
	{
		cbItemCount++;
		cbGangCard[i]=GangCardResult.cbCardData[i];
	}

	//������Ŀ
	BYTE cbItemKind[4]={WIK_LEFT,WIK_CENTER,WIK_RIGHT,WIK_PENG};
	for (BYTE i=0;i<CountArray(cbItemKind);i++) if ((cbActionMask&cbItemKind[i])!=0) cbItemCount++;

	if(cbActionMask&WIK_CHI_HU)
	{
		//��������
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
				//��������
				 bCurrentItem = rand() % GangCardResult.cbCardCount;

				 //��������
				 CMD_C_OperateCard OperateCard;
				 OperateCard.cbOperateCode=WIK_GANG;
				 OperateCard.cbOperateCard=cbGangCard[bCurrentItem];
				 OnGameMessage(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard), pIServerUserItem);

			}
			else
			{
				BYTE cbIndex=0;
				//��������
				for (BYTE i=0;i<CountArray(cbItemKind);i++)
				{
					if (((cbActionMask&cbItemKind[i])!=0)&&(bCurrentItem==cbIndex++))
					{
						//��������
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

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			GT_ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			GT_ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->UserStatus.wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
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

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	WORD i=0 ;
	//Ч��״̬
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	GT_ASSERT(wChairID==m_wCurrentUser);
	GT_ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		GT_ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;
	m_bEnjoinChiHu[wChairID]=false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//�û��л�
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//��������
	if (m_bGangStatus==true)
	{
		for (i=0;i<m_wPlayerCount;i++)
		{
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
		}
		if (i==m_wPlayerCount) m_bGangStatus=false;
	}

	//�ɷ��˿�
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч���û�
	GT_ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return false;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		GT_ASSERT(m_bResponse[wChairID]==false);
		GT_ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true) return false;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) return true;
			}
		}

		//��������
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;
			m_wProvideUser=m_wProvideUser;

			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				//if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;
				if (((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;

				//��ͨ����
				if (m_cbChiHuCard!=0)
				{
					//�����ж�
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i]);

					//�����˿�
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}

			//������Ϸ
			GT_ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����˿�
		GT_ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_FILL:		//���Ʋ���
		case WIK_GANG:		//���Ʋ���
			{
				//ɾ���˿�
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

		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//����״̬
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus=true;
			m_bEnjoinChiPeng[wTargetUser]=true;
		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if ((cbTargetAction==WIK_GANG)||(cbTargetAction==WIK_FILL))
		{
			//Ч�鶯��
			bool bAroseAction=EstimateUserRespond(wTargetUser,cbTargetCard,EstimatKind_GangCard);

			//�����˿�
			if (bAroseAction==false) DispatchCardData(wTargetUser);

			return true;
		}

		//�����ж�
		if (m_cbLeftCardCount>1)
		{
			//�����ж�
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);

			//�������
			if (GangCardResult.cbCardCount>0)
			{
				//���ñ���
				m_cbUserAction[m_wCurrentUser]|=(WIK_GANG|WIK_FILL);

				//���Ͷ���
				SendOperateNotify();
			}
		}

		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		GT_ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//�˿�Ч��
		GT_ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_FILL:			//���Ʋ���
		case WIK_GANG:			//���Ʋ���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ�����
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

					//Ч�鶯��
					GT_ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{
					//�˿�Ч��
					GT_ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) return false;

					//���ñ���
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//����״̬
				if (cbOperateCode==WIK_GANG)
				{
					m_bGangStatus=true;
					m_bEnjoinChiPeng[wChairID]=true;
				}

				//������
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);

				//�����˿�
				if (bAroseAction==false) DispatchCardData(wChairID);

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				WORD wChiHuRight=0;
				if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,wChiHuRight,m_ChiHuResult[wChairID]);

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//������Ϸ
				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//״̬Ч��
	GT_ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) return false;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��ׯ����
	if (m_cbLeftCardCount==0)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//���ƴ���
	if (m_bSendStatus==true)
	{
		//�����˿�
		m_cbSendCardCount++;
		//�Ƿ�AI��ɱ��ͨ���
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
		if(m_pITableFrame->IsAIKillGameUser() && wGameUser == wCurrentUser && wGameUser != INVALID_CHAIR)
		{
			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

			//�ж���ͨ��Ҹ��ƻ��ߺ���
			tagChiHuResult ChiHuResult;
			if(m_GameLogic.EstimateGangCard(m_cbCardIndex[wGameUser], m_cbRepertoryCard[m_cbLeftCardCount-1]) != WIK_NULL ||
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wGameUser],
				m_WeaveItemArray[wGameUser],
				m_cbWeaveItemCount[wGameUser],
				m_cbRepertoryCard[m_cbLeftCardCount-1],
				wChiHuRight,
				ChiHuResult) != WIK_NULL)
			{
				//����������-�൱���ʸ���ͨ��Ҹ��ƻ��ߺ���
				if(CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
				{	
					int i = m_cbLeftCardCount-1;
					bool bLoopFlag = true;
					for(;i >= 0 && bLoopFlag; i --)
					{
						//�ж���ͨ��Ҹ��ƻ��ߺ���
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

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//�����ж�
		if (m_cbLeftCardCount>1)
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
		}

		//����Ȩλ
		WORD wChiHuRight=0;
		if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

		//�����ж�
		tagChiHuResult ChiHuResult;
		//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
		//	m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult);
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
			m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
			0x00,
			wChiHuRight,ChiHuResult);

	}

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}

			//�����ж�
			if (m_cbLeftCardCount>1) 
			{
				m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;
			if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//�Ժ��ж�
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			//�Ժ�����
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) m_bEnjoinChiHu[i]=true;
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}
//�жϻľ�
bool CTableFrameSink::EstimateNullChiHuCard()
{
	return (m_cbChiHuCard==0 && m_wProvideUser==INVALID_CHAIR) ? true : false;
}
//��ȡ�������
WORD  CTableFrameSink::GetProvideUser()
{	
	return m_wProvideUser;
}
//��ȡ�������
WORD CTableFrameSink::GetHuPaiUser()
{	
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) return i;
	}
	return INVALID_CHAIR;
}
//ץ�ں���
bool CTableFrameSink::IsZhuaPaoChiHu()
{
	if(m_wProvideUser != INVALID_CHAIR)
		return m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL ? true : false;
	else
		return false;
}
//�ж���ЧͶע�û�
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
