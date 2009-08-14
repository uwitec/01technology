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
	m_wBombTime=1;
	m_bLandScore=0;
	m_wFirstUser=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	memset(m_bDoubleTimeScore,0,sizeof(m_bDoubleTimeScore));

	//Run��Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

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
	m_wBombTime=1;
	m_bLandScore=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	memset(m_bDoubleTimeScore,0,sizeof(m_bDoubleTimeScore));

	//Run��Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

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

	return GameStart();
}
//��Ϸ��ʼ
bool CTableFrameSink::GameStart()
{
	//�����˿�
	BYTE bRandCard[54];
	m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=17;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}
	CopyMemory(m_bBackCard,&bRandCard[51],sizeof(m_bBackCard));
	m_GameLogic.SortCardList(m_bBackCard,3);

	//���ں�÷��5��˳�������׽��û�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{	
		for(WORD j=0;j<17;j++)
		{
			if(m_bHandCardData[i][j]==0x35) m_wCurrentUser=i;			
		}
	}
	if(m_wCurrentUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{	
			for(WORD j=0;j<17;j++)
			{
				if(m_bHandCardData[i][j]==0x25) m_wCurrentUser=i;			
			}
		}
	}
	if(m_wCurrentUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{	
			for(WORD j=0;j<17;j++)
			{
				if(m_bHandCardData[i][j]==0x15) m_wCurrentUser=i;			
			}
		}
	}
	if(m_wCurrentUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{	
			for(WORD j=0;j<17;j++)
			{
				if(m_bHandCardData[i][j]==0x05) m_wCurrentUser=i;			
			}
		}
	}

	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_SCORE);

	WORD wLandUser = FindLandUserBySpecialCard(m_bHandCardData, m_bCardCount, m_wCurrentUser);
	if(wLandUser == INVALID_CHAIR)
	{
		//�����û�
		m_wFirstUser=m_wCurrentUser;
		//�����˿�
		CMD_S_SendCard SendCard;
		SendCard.wCurrentUser=m_wCurrentUser;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			CopyMemory(SendCard.bCardData,m_bHandCardData[i],sizeof(SendCard.bCardData));
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}

	}
	else
	{
		//�����˿�
		CMD_S_SendCard SendCard;
		SendCard.wCurrentUser=INVALID_CHAIR;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			CopyMemory(SendCard.bCardData,m_bHandCardData[i],sizeof(SendCard.bCardData));
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_wCurrentUser = wLandUser;
		//�����û�
		m_wFirstUser=m_wCurrentUser;
		//���е���
		for(int i = 0; i < m_wPlayerCount; i ++)
		{
			if(i == m_wCurrentUser)
				continue;

			IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
			CMD_C_LandScore LandScore;
			LandScore.bLandScore=0;
			OnGameMessage(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore), pIServerUserItem);
		}
		//�е���
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentUser);
		CMD_C_LandScore LandScore;
		LandScore.bLandScore=(BYTE)(m_bLandScore == 0 ? 1 : m_bLandScore + 1);
		OnGameMessage(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore), pIServerUserItem);

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
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			//���ñ���
			GameEnd.wBombTime=m_wBombTime;
			GameEnd.wLandUser=m_wBankerUser;
			GameEnd.bLandScore=m_bLandScore;
			CopyMemory(GameEnd.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(GameEnd.bDoubleTimeScore,m_bDoubleTimeScore,sizeof(m_bDoubleTimeScore));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//��������
			DOUBLE fCellScore=m_pGameServiceOption->dwCellScore;
			bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;

			//�����ж�
			if (wChairID==m_wBankerUser)
			{
				WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
				WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;
				if ((m_bOutCardCount[wUser1]==0)&&(m_bOutCardCount[wUser2]==0)) m_wBombTime*=2;
			}
			else
			{
				if (m_bOutCardCount[m_wBankerUser]==1) m_wBombTime*=2;
			}

			//ͳ�����ö��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==m_wBankerUser)
				{
					//GameEnd.fGameScore[i]=m_wBombTime*m_bLandScore*fCellScore*((bLandWin==true)?2:-2);
				}
				else 
				{
					if(bLandWin)
					{
						if(m_bDoubleTimeScore[i]==2)
							GameEnd.fGameScore[i]=m_wBombTime*m_bLandScore*fCellScore*-2;
						else
							GameEnd.fGameScore[i]=m_wBombTime*m_bLandScore*fCellScore*-1;
					}
					else
					{
						if(m_bDoubleTimeScore[i]==2)
							GameEnd.fGameScore[i]=m_wBombTime*m_bLandScore*fCellScore*2;
						else
							GameEnd.fGameScore[i]=m_wBombTime*m_bLandScore*fCellScore*1;
					}
					
				}
			}
			//ͳ�����ö��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i!=m_wBankerUser)
				{
					GameEnd.fGameScore[m_wBankerUser]+= (-1 * GameEnd.fGameScore[i]);
				}
			}
			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd, sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);
				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//�л��û�
				m_wFirstUser=wChairID;

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
			//Ч�����
			GT_ASSERT(pIServerUserItem!=NULL);
			GT_ASSERT(wChairID<m_wPlayerCount);

			if(m_bLandScore == 0) m_bLandScore = 1;

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			//���ñ���
			GameEnd.wBombTime=m_wBombTime;
			GameEnd.wLandUser=m_wBankerUser;
			GameEnd.bLandScore=m_bLandScore;
			CopyMemory(GameEnd.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			//ͳ�ƶ��
			GameEnd.fGameScore[wChairID]=(m_pGameServiceOption->dwCellScore * m_wBombTime * m_bLandScore);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i) continue;
				GameEnd.fGameScore[i]=GameEnd.fGameScore[wChairID]/2;
				
			}
			GameEnd.fGameScore[wChairID] = - GameEnd.fGameScore[wChairID];

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
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

	GT_ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.fBaseScore=m_pGameServiceOption->dwCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SCORE:		//�з�״̬
		{
			//�������� 
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//���ñ���
			StatusScore.bLandScore=m_bLandScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.fBaseScore=m_pGameServiceOption->dwCellScore;
			CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
			CopyMemory(StatusScore.bCardData,m_bHandCardData[wChairID],m_bCardCount[wChairID]);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBombTime=m_wBombTime;
			StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.bLandScore=m_bLandScore;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.fBaseScore=m_pGameServiceOption->dwCellScore;
			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChairID],sizeof(BYTE)*m_bCardCount[wChairID]);
			CopyMemory(StatusPlay.bDoubleTimeScore,m_bDoubleTimeScore,sizeof(m_bDoubleTimeScore));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_WK_DOUBLETIME:		//����״̬
		{
			//��������
			CMD_S_StatusDoubleTime StatusDoubleTime;
			memset(&StatusDoubleTime,0,sizeof(StatusDoubleTime));

			//���ñ���
			StatusDoubleTime.wBombTime=m_wBombTime;
			StatusDoubleTime.wLandUser=m_wBankerUser;
			StatusDoubleTime.wCurrentUser=m_wCurrentUser;
			StatusDoubleTime.bLandScore=m_bLandScore;
			StatusDoubleTime.fBaseScore=m_pGameServiceOption->dwCellScore;
			CopyMemory(StatusDoubleTime.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusDoubleTime.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusDoubleTime.bCardData,m_bHandCardData[wChairID],sizeof(BYTE)*m_bCardCount[wChairID]);
			CopyMemory(StatusDoubleTime.bDoubleTimeScore,m_bDoubleTimeScore,sizeof(m_bDoubleTimeScore));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusDoubleTime,sizeof(StatusDoubleTime));
		}
	}

	return false;
}
//�˹�������Ϸ����
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	//�������ж�
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentUser);
	if(pIServerUserItem == NULL || 
		pIServerUserItem->IsAIUser() == false || 
		pIServerUserItem->GetUserStatus() != US_PLAY)
	{
		//�ж϶���״̬���
		if(pIServerUserItem->GetUserStatus() != US_OFFLINE)
			return false;
	}
	WORD wChairID = pIServerUserItem->GetChairID();
	if(wChairID == INVALID_CHAIR)
		return false;

	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	switch(cbGameStatus) 
	{
	case GS_WK_SCORE://�з�״̬
		{
			CMD_C_LandScore LandScore;
			//�ж϶���״̬���
			if(pIServerUserItem->GetUserStatus() != US_OFFLINE)
				LandScore.bLandScore=(BYTE)(m_bLandScore == 0 ? 1 : m_bLandScore + 1);
			else
				LandScore.bLandScore=(BYTE)(m_bLandScore == 0 ? 1 : 255);
			OnGameMessage(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore), pIServerUserItem);
		}
		break;
	case GS_WK_DOUBLETIME://����״̬
		{
			CMD_C_DoubleTime DoubleTime;
			//�ж϶���״̬���
			if(pIServerUserItem->GetUserStatus() != US_OFFLINE)
				DoubleTime.bDoubleTime=rand()%2 ? 1 : 2;
			else
				DoubleTime.bDoubleTime=1;
			OnGameMessage(SUB_C_DOUBLETIME,&DoubleTime,sizeof(DoubleTime), pIServerUserItem);
		}
		break;
	case GS_WK_PLAYING://��Ϸ״̬
		{
			AI_AutomatismOutCard(wChairID, pIServerUserItem);
		}
		break;
	default:
		break;
	}


	return true;
}
//�Զ�����
bool CTableFrameSink::AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[wChairID][m_bCardCount[wChairID]-1];
		OnGameMessage(SUB_C_OUT_CART,
			&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE),
			pIServerUserItem);
	}
	else 
	{
		//�ж϶���״̬���
		if(pIServerUserItem->GetUserStatus() != US_OFFLINE)
			AI_AutoOutCard(wChairID, pIServerUserItem, 0);
		else
			AI_PassCard(wChairID,pIServerUserItem);
	}
	return true;
}
//Ѱ�ҳ���
void CTableFrameSink::AI_AutoOutCard(WORD wChairID, IServerUserItem * pIServerUserItem,BYTE bWhichOnsKindCard)
{
	BYTE							bHandCardCount;					//�˿���Ŀ
	BYTE							bHandCardData[20];				//�����˿�

	BYTE							bTurnOutType;					//��������
	BYTE							bTurnCardCount;					//������Ŀ
	BYTE							bTurnCardData[20];				//�����б�

	bHandCardCount  = m_bCardCount[wChairID];
	memcpy(bHandCardData,m_bHandCardData[wChairID], sizeof(bHandCardData));

	bTurnCardCount	= m_bTurnCardCount;
	memcpy(bTurnCardData, m_bTurnCardData, sizeof(bTurnCardData));
	bTurnOutType=m_GameLogic.GetCardType(bTurnCardData,bTurnCardCount);

	int								i=0;
	BYTE							bWhichKindSel=0;
	BYTE							bTempSCardCount=0;				//�˿���Ŀ
	BYTE							bTempSCardData[20];				//�����˿�
	BYTE							bTempDCardCount=0;				//�˿���Ŀ
	BYTE							bTempDCardData[20];				//�����˿�
	BYTE							bTempTCardCount=0;				//�˿���Ŀ
	BYTE							bTempTCardData[20];				//�����˿�
	BYTE							bTempFCardCount=0;				//�˿���Ŀ
	BYTE							bTempFCardData[20];				//�����˿�
	BYTE							bTempGetCardCount=0;				//�˿���Ŀ
	BYTE							bTempGetCardData[20];				//�����˿�
	BYTE							bTempGetCardIndex[20];			//�����˿�
	BYTE 							m_TempCard=0;
	//���û���˳��ƣ�����ʾ
	if(bTurnCardCount==0)
		return;

	//��ȡ�����б�
	for(i=0;i<bHandCardCount;i++)
	{	
		BYTE m_GetCard=m_GameLogic.GetCardLogicValue(bHandCardData[i]);
		if(m_TempCard!=m_GetCard)
		{
			bTempSCardData[bTempSCardCount++]=bHandCardData[i];
			m_TempCard=m_GetCard;
		}
	}
	//��ȡ�����б�
	m_TempCard=0;
	for(i=0;i<bHandCardCount-1;i++)
	{	
		BYTE m_GetCard1=m_GameLogic.GetCardLogicValue(bHandCardData[i]);
		BYTE m_GetCard2=m_GameLogic.GetCardLogicValue(bHandCardData[i+1]);
		if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1<16)
		{
			bTempDCardData[bTempDCardCount++]=bHandCardData[i];
			bTempDCardData[bTempDCardCount++]=bHandCardData[i+1];
			m_TempCard=m_GetCard1;
		}
	}
	//��ȡ�������б�
	m_TempCard=0;
	for(i=0;i<bHandCardCount-2;i++)
	{	
		BYTE m_GetCard1=m_GameLogic.GetCardLogicValue(bHandCardData[i]);
		BYTE m_GetCard2=m_GameLogic.GetCardLogicValue(bHandCardData[i+1]);
		BYTE m_GetCard3=m_GameLogic.GetCardLogicValue(bHandCardData[i+2]);
		if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1==m_GetCard3)
		{
			bTempTCardData[bTempTCardCount++]=bHandCardData[i];
			bTempTCardData[bTempTCardCount++]=bHandCardData[i+1];
			bTempTCardData[bTempTCardCount++]=bHandCardData[i+2];
			m_TempCard=m_GetCard1;
		}
	}
	//��ȡ�������б�
	m_TempCard=0;
	for(i=0;i<bHandCardCount-3;i++)
	{	
		BYTE m_GetCard1=m_GameLogic.GetCardLogicValue(bHandCardData[i]);
		BYTE m_GetCard2=m_GameLogic.GetCardLogicValue(bHandCardData[i+1]);
		BYTE m_GetCard3=m_GameLogic.GetCardLogicValue(bHandCardData[i+2]);
		BYTE m_GetCard4=m_GameLogic.GetCardLogicValue(bHandCardData[i+3]);
		if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1==m_GetCard3&&m_GetCard1==m_GetCard4)
		{
			bTempFCardData[bTempFCardCount++]=bHandCardData[i];
			bTempFCardData[bTempFCardCount++]=bHandCardData[i+1];
			bTempFCardData[bTempFCardCount++]=bHandCardData[i+2];
			bTempFCardData[bTempFCardCount++]=bHandCardData[i+3];
			m_TempCard=m_GetCard1;
		}
	}
	//���������������ж�
	i=0;
	switch(bTurnOutType)
	{
	case CT_SINGLE:
	case CT_ONE_LINE:
		if(bWhichOnsKindCard==1)   //�ж��ǲ��Ǿ���Ψһ��
		{
			for(i=bTempSCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&m_GameLogic.CompareCard(&bTempSCardData[i-bTurnCardCount],bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					if((bWhichKindSel++)>1)
						i=0;
				}
			}
		}
		for(i=bTempSCardCount;i>0;i--)
		{
			if(i-bTurnCardCount>=0&&m_GameLogic.CompareCard(&bTempSCardData[i-bTurnCardCount],bTurnCardData,bTurnCardCount,bTurnCardCount))
			{
				//�ж��ǲ���������
				bool m_bIsHaveCard=false;
				for(int j=0;j<bTempDCardCount;j++)
				{
					for(int n=0;n<bTurnCardCount;n++)
					{
						if(m_GameLogic.GetCardLogicValue(bTempSCardData[i-bTurnCardCount+n])==m_GameLogic.GetCardLogicValue(bTempDCardData[j]))
							m_bIsHaveCard=true;
					}
				}
				//�������������������
				if(bTempGetCardCount==0||!m_bIsHaveCard)
				{
					CopyMemory(bTempGetCardData,&bTempSCardData[i-bTurnCardCount],bTurnCardCount);
					bTempGetCardCount=bTurnCardCount;
				}
				if(!m_bIsHaveCard)
					break;
			}
		}
		break;
	case CT_DOUBLE:
	case CT_DOUBLE_LINE:
		if(bWhichOnsKindCard==1)     //�ж��ǲ��Ǿ���Ψһ��
		{
			for(i=bTempDCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&m_GameLogic.CompareCard(&bTempDCardData[i-bTurnCardCount],bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					if((bWhichKindSel++)>1)
						i=0;
				}
			}
		}
		for(i=bTempDCardCount;i>0;i--)
		{
			if(i-bTurnCardCount>=0&&m_GameLogic.CompareCard(&bTempDCardData[i-bTurnCardCount],bTurnCardData,bTurnCardCount,bTurnCardCount))
			{
				//�ж��ǲ���������
				bool m_bIsHaveCard=false;
				for(int j=0;j<bTempTCardCount;j++)
				{
					for(int n=0;n<bTurnCardCount;n++)
					{
						if(m_GameLogic.GetCardLogicValue(bTempDCardData[i-bTurnCardCount+n])==m_GameLogic.GetCardLogicValue(bTempTCardData[j]))
							m_bIsHaveCard=true;
					}
				}
				if(bTempGetCardCount==0||!m_bIsHaveCard)
				{
					CopyMemory(bTempGetCardData,&bTempDCardData[i-bTurnCardCount],bTurnCardCount);
					bTempGetCardCount=bTurnCardCount;
				}
				if(!m_bIsHaveCard)
					break;
			}
		}
		break;
	case CT_THREE:
	case CT_THREE_LINE:
		if(bWhichOnsKindCard==1)           //�ж��ǲ��Ǿ���Ψһ��
		{
			for(i=bTempTCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&m_GameLogic.CompareCard(&bTempTCardData[i-bTurnCardCount],bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					if((bWhichKindSel++)>1)
						i=0;
				}
			}
		}
		for(i=bTempTCardCount;i>0;i--)
		{
			if(i-bTurnCardCount>=0&&m_GameLogic.CompareCard(&bTempTCardData[i-bTurnCardCount],bTurnCardData,bTurnCardCount,bTurnCardCount))
			{
				//�ж��ǲ���������
				bool m_bIsHaveCard=false;
				for(int j=0;j<bTempFCardCount;j++)
				{
					for(int n=0;n<bTurnCardCount;n++)
					{
						if(m_GameLogic.GetCardLogicValue(bTempTCardData[i-bTurnCardCount+n])==m_GameLogic.GetCardLogicValue(bTempFCardData[j]))
							m_bIsHaveCard=true;
					}
				}
				if(bTempGetCardCount==0||!m_bIsHaveCard)
				{
					CopyMemory(bTempGetCardData,&bTempTCardData[i-bTurnCardCount],bTurnCardCount);
					bTempGetCardCount=bTurnCardCount;
				}
				if(!m_bIsHaveCard&&bTempGetCardCount!=0)
					break;
			}
		}
		break;
	case CT_THREE_LINE_TAKE_ONE:
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			//�����˿�
			tagAnalyseResult AnalyseResult;
			m_GameLogic.AnalysebCardData(bTurnCardData,bTurnCardCount,AnalyseResult);      
			if(bWhichOnsKindCard==1)               //�ж��ǲ��Ǿ���Ψһ��
			{
				for(i=bTempTCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bThreeCount*3>=0&&m_GameLogic.CompareCard(&bTempTCardData[i-AnalyseResult.bThreeCount*3],bTurnCardData,AnalyseResult.bThreeCount*3,AnalyseResult.bThreeCount*3))
					{
						if((bWhichKindSel++)>1)
							i=0;
					}
				}
			}
			for(i=bTempTCardCount;i>0;i--)
			{
				if(i-AnalyseResult.bThreeCount*3>=0&&m_GameLogic.CompareCard(&bTempTCardData[i-AnalyseResult.bThreeCount*3],AnalyseResult.m_bTCardData,AnalyseResult.bThreeCount*3,AnalyseResult.bThreeCount*3))
				{
					//�ж��ǲ���������
					bool m_bIsHaveCard=false;
					for(int j=0;j<bTempFCardCount;j++)
					{
						for(int n=0;n<AnalyseResult.bThreeCount*3;n++)
						{
							if(m_GameLogic.GetCardLogicValue(bTempTCardData[i-AnalyseResult.bThreeCount*3+n])==m_GameLogic.GetCardLogicValue(bTempFCardData[j]))
								m_bIsHaveCard=true;
						}
					}
					if(bTempGetCardCount==0||!m_bIsHaveCard)
					{
						CopyMemory(bTempGetCardData,&bTempTCardData[i-AnalyseResult.bThreeCount*3],AnalyseResult.bThreeCount*3);
						bTempGetCardCount=AnalyseResult.bThreeCount*3;
					}
					if(!m_bIsHaveCard&&bTempGetCardCount!=0)
						i=0;
				}
			}
			if(bTempGetCardCount>0)
			{
				bool bIsHaveSame;
				for(int m=0;m<AnalyseResult.bDoubleCount;m++)
				{
					for(int j=0;j<bTempDCardCount/2;j++)
					{
						//�ж��ǲ���������
						bIsHaveSame=false;
						for(int n=0;n<bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(bTempDCardData[bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(bTempGetCardData[n]))
							{
								bIsHaveSame=true;
							}
						}
						if(!bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<bTempTCardCount;s++)
							{
								for(int n=0;n<bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(bTempDCardData[bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(bTempTCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(bTempGetCardCount==AnalyseResult.bThreeCount*3||!m_bIsHaveCard)
							{
								bTempGetCardData[AnalyseResult.bThreeCount*3+m*2]=bTempDCardData[bTempDCardCount-j*2-1];
								bTempGetCardData[AnalyseResult.bThreeCount*3+m*2+1]=bTempDCardData[bTempDCardCount-j*2-2];
								bTempGetCardCount=AnalyseResult.bThreeCount*3+(m+1)*2;
							}
							if(!m_bIsHaveCard)
							{
								n=bTempGetCardCount;
								j=bTempDCardCount/2;
							}
						}
					}
				}
				for(int m=0;m<AnalyseResult.bSignedCount;m++)
				{
					for(int j=0;j<bTempSCardCount;j++)
					{
						//�ж��ǲ���������
						bIsHaveSame=false;
						for(int n=0;n<bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(bTempSCardData[bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(bTempGetCardData[n]))
							{
								bIsHaveSame=true;
							}
						}
						if(!bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<bTempDCardCount;s++)
							{
								for(n=0;n<bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(bTempSCardData[bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(bTempDCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(bTempGetCardCount==AnalyseResult.bThreeCount*3||!m_bIsHaveCard)
							{
								bTempGetCardData[AnalyseResult.bThreeCount*3+m]=bTempSCardData[bTempSCardCount-j-1];
								bTempGetCardCount=AnalyseResult.bThreeCount*3+m+1;
							}
							if(!m_bIsHaveCard)
							{
								n=bTempGetCardCount;
								j=bTempSCardCount;
							}
						}
					}
				}
			}
		}
		break;
	case CT_FOUR_LINE_TAKE_ONE:
	case CT_FOUR_LINE_TAKE_DOUBLE:
		{
			//�����˿�
			tagAnalyseResult AnalyseResult;
			m_GameLogic.AnalysebCardData(bTurnCardData,bTurnCardCount,AnalyseResult);
			if(bWhichOnsKindCard==1)       //�ж��ǲ��Ǿ���Ψһ��
			{
				for(i=bTempFCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bFourCount*4>=0&&m_GameLogic.CompareCard(&bTempFCardData[i-AnalyseResult.bFourCount*4],bTurnCardData,AnalyseResult.bFourCount*4,AnalyseResult.bFourCount*4))
					{
						if((bWhichKindSel++)>1)
							i=0;
					}
				}
			}
			for(i=bTempFCardCount;i>0;i--)
			{
				if(i-AnalyseResult.bFourCount*4>=0&&m_GameLogic.CompareCard(&bTempFCardData[i-AnalyseResult.bFourCount*4],bTurnCardData,AnalyseResult.bFourCount*4,AnalyseResult.bFourCount*4))
				{
					CopyMemory(bTempGetCardData,&bTempFCardData[i-AnalyseResult.bFourCount*4],AnalyseResult.bFourCount*4);
					bTempGetCardCount=AnalyseResult.bFourCount*4;
					i=0;
				}
			}
			if(bTempGetCardCount>0)
			{
				bool bIsHaveSame;
				for(int m=0;m<AnalyseResult.bDoubleCount;m++)
				{
					for(int j=0;j<bTempDCardCount/2;j++)
					{
						//�ж��ǲ���������
						bIsHaveSame=false;
						for(int n=0;n<bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(bTempDCardData[bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(bTempGetCardData[n]))
							{
								bIsHaveSame=true;
							}
						}
						if(!bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<bTempTCardCount;s++)
							{
								for(int n=0;n<bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(bTempDCardData[bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(bTempTCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(bTempGetCardCount==AnalyseResult.bFourCount*4||!m_bIsHaveCard)
							{
								bTempGetCardData[AnalyseResult.bFourCount*4+m*2]=bTempDCardData[bTempDCardCount-j*2-1];
								bTempGetCardData[AnalyseResult.bFourCount*4+m*2+1]=bTempDCardData[bTempDCardCount-j*2-2];
								bTempGetCardCount=AnalyseResult.bFourCount*4+(m+1)*2;
							}
							if(!m_bIsHaveCard)
							{
								n=bTempGetCardCount;
								j=bTempDCardCount/2;
							}
						}
					}
				}
				for(int m=0;m<AnalyseResult.bSignedCount;m++)
				{
					for(int j=0;j<bTempSCardCount;j++)
					{
						//�ж��ǲ���������
						bIsHaveSame=false;
						for(int n=0;n<bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(bTempSCardData[bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(bTempGetCardData[n]))
							{
								bIsHaveSame=true;
							}
						}
						if(!bIsHaveSame)
						{
							bool m_bIsHaveCard=false;
							for(int s=0;s<bTempDCardCount;s++)
							{
								for(int n=0;n<bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(bTempSCardData[bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(bTempDCardData[j]))
										m_bIsHaveCard=true;
								}
							}
							if(bTempGetCardCount==AnalyseResult.bFourCount*4||!m_bIsHaveCard)
							{
								bTempGetCardData[AnalyseResult.bFourCount*4+m]=bTempSCardData[bTempSCardCount-j-1];
								bTempGetCardCount=AnalyseResult.bFourCount*4+m+1;
							}
							if(!m_bIsHaveCard)
							{
								n=bTempGetCardCount;
								j=bTempSCardCount;
							}
						}
					}
				}
			}
		}
		break;
	}
	if(bTempGetCardCount==0)
	{
		bWhichKindSel=0;
		//�ж�ը���Ŀ�����
		if(bTempFCardCount>3)
		{
			for(i=bTempFCardCount-4;i>=0;i--)
			{
				if(m_GameLogic.CompareCard(&bTempFCardData[i],bTurnCardData,4,bTurnCardCount))
				{
					if((bWhichKindSel++)==0)
					{
						CopyMemory(bTempGetCardData,&bTempFCardData[i],4);
						bTempGetCardCount=4;
					}
				}
			}
		}
		if(bTempGetCardCount==0)
		{
			if(bHandCardCount>1)
			{
				if(m_GameLogic.GetCardLogicValue(bHandCardData[0])>15&&m_GameLogic.GetCardLogicValue(bHandCardData[1])>15)
				{
					CopyMemory(bTempGetCardData,bHandCardData,2);
					bTempGetCardCount=2;
					if(bWhichOnsKindCard==1)
						bWhichKindSel=1;
				}
			}
		}
	}
	BYTE m_GetIndex=0;
	if(bTempGetCardCount==0)
	{
		if(bWhichOnsKindCard!=1)
			AI_PassCard(wChairID,pIServerUserItem);
	}
	else
	{
		for(int j=0;j<bTempGetCardCount;j++)
		{
			for(i=0;i<bHandCardCount;i++)
			{
				if(bHandCardData[i]==bTempGetCardData[j])
				{
					bTempGetCardIndex[m_GetIndex++]=i;
				}
			}
		}

	}
	if(m_GameLogic.CompareCard(bTempGetCardData,bTurnCardData,bTempGetCardCount,bTurnCardCount))
	{
		if(bWhichOnsKindCard==1&&bWhichKindSel==1||bWhichOnsKindCard!=1)
		{
			//��������
			CMD_C_OutCard OutCard;
			OutCard.bCardCount=bTempGetCardCount;
			memcpy(OutCard.bCardData, bTempGetCardData, sizeof(BYTE) * bTempGetCardCount);

			OnGameMessage(SUB_C_OUT_CART,
				&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE),
				pIServerUserItem);

		}
	}
	else
	{
		if(bWhichOnsKindCard!=1)
			AI_PassCard(wChairID,pIServerUserItem);
	}
}

//��������
bool CTableFrameSink::AI_PassCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	OnGameMessage(SUB_C_PASS_CARD, NULL, 0, pIServerUserItem);
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
	case SUB_C_LAND_SCORE:	//�û��з�
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(CMD_C_LandScore));
			if (wDataSize!=sizeof(CMD_C_LandScore)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY&&pUserData->UserStatus.cbUserStatus!=US_OFFLINE) return true;

			//��Ϣ����
			CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pDataBuffer;
			return OnUserLandScore(pUserData->UserStatus.wChairID,pLandScore->bLandScore);
		}
	case SUB_C_OUT_CART:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//Ч������
			GT_ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			GT_ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY&&pUserData->UserStatus.cbUserStatus!=US_OFFLINE) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY&&pUserData->UserStatus.cbUserStatus!=US_OFFLINE) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->UserStatus.wChairID);
		}
	case SUB_C_DOUBLETIME://�û�����
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(CMD_C_DoubleTime));
			if (wDataSize!=sizeof(CMD_C_DoubleTime)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY&&pUserData->UserStatus.cbUserStatus!=US_OFFLINE) return true;

			//��Ϣ����
			CMD_C_DoubleTime * pDoubleTime=(CMD_C_DoubleTime *)pDataBuffer;
			return OnDoubleTime(pUserData->UserStatus.wChairID,pDoubleTime->bDoubleTime);
		}
	}

	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//Ч�����
	if (((bLandScore>3)&&(bLandScore!=255))||(bLandScore<=m_bLandScore)) return false;

	//���ñ���
	if (bLandScore!=255)
	{
		BYTE bBetLandScore = m_bLandScore == 0 ? 1 : m_bLandScore;
		if(bLandScore - bBetLandScore > 0)
		{
			//Ͷע
			tagBetScoreInfo BetScoreInfo;
			BetScoreInfo.dwBetScore = m_pGameServiceOption->dwCellScore * (bLandScore - bBetLandScore) ;
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

		}
		m_bLandScore=bLandScore;
		m_wBankerUser=m_wCurrentUser;

	}
	m_bScoreInfo[wChairID]=bLandScore;

	//��ʼ�ж�

	if (m_bLandScore==1)
	{
		//���ñ���
		if (m_bLandScore==0) m_bLandScore=1;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		//����״̬
		m_pITableFrame->SetGameStatus(GS_WK_DOUBLETIME);

		//���͵���
		m_bCardCount[m_wBankerUser]=20;
		CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser]);

		//������Ϣ
		m_bTurnCardCount=0;
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=(m_wBankerUser+1)%GAME_PLAYER;

		if(m_bScoreInfo[m_wCurrentUser] == 255)
		{
			//�е��������ܷ���
			OnDoubleTime(m_wCurrentUser, 1);
		}
		else
		{
			//û�е��������ͷ���״̬

			//������Ϣ
			CMD_S_DoubleTime DoubleTime;
			DoubleTime.wLandUser=m_wBankerUser;
			DoubleTime.bLandScore=m_bLandScore;
			DoubleTime.wCurrentUser=m_wCurrentUser;
			CopyMemory(DoubleTime.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_DOUBLETIME,&DoubleTime,sizeof(DoubleTime));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_DOUBLETIME,&DoubleTime,sizeof(DoubleTime));
		}

		return true;
	}
	else if((m_wFirstUser==(wChairID+1)%m_wPlayerCount))
	{
		RepositTableFrameSink();
		return GameStart();
	}
	//�����û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//������Ϣ
	CMD_S_LandScore LandScore;
	LandScore.bLandUser=wChairID;
	LandScore.bLandScore=bLandScore;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

	return true;
}
//�û�����
bool CTableFrameSink::OnDoubleTime(WORD wChairID, BYTE bDoubleTime)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_DOUBLETIME) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//Ч�����
	if (bDoubleTime == 0 || bDoubleTime > 2) return false;

	//���ñ���
	m_bDoubleTimeScore[wChairID]=bDoubleTime;

	//�����û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//������Ϣ
	CMD_S_UserDoubleTime UserDoubleTime;
	UserDoubleTime.wDoubleTimeUser=wChairID;
	UserDoubleTime.bDoubleTime=bDoubleTime;
	if(m_bScoreInfo[m_wCurrentUser] == 255 || m_wCurrentUser == m_wBankerUser)
	{
		UserDoubleTime.wCurrentUser=INVALID_CHAIR;
	}
	else
		UserDoubleTime.wCurrentUser=m_wCurrentUser;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USERDOUBLETIME,&UserDoubleTime,sizeof(UserDoubleTime));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USERDOUBLETIME,&UserDoubleTime,sizeof(UserDoubleTime));
	
	if(m_bScoreInfo[m_wCurrentUser] == 255 && m_wCurrentUser != m_wBankerUser)	
	{
		//�е��������ܷ���
		OnDoubleTime(m_wCurrentUser, 1);
		return TRUE;
	}

	//��ʼ�ж�
	if ((m_wBankerUser==(wChairID+1)%m_wPlayerCount))
	{
		//���ñ���
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		//����״̬
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

		//������Ϣ
		m_bTurnCardCount=0;
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//������Ϣ
		CMD_S_GameStart GameStart;
		GameStart.wLandUser=m_wBankerUser;
		GameStart.bLandScore=m_bLandScore;
		GameStart.wCurrentUser=m_wCurrentUser;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

		return true;
	}


	return true;
}
//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_INVALID) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if ((bCardType==CT_BOMB_CARD)||(bCardType==CT_MISSILE_CARD))
	{
		m_wBombTime*=2;

		m_wBombTime = __min(m_wBombTime,5*2);
	}

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bCardType!=CT_MISSILE_CARD) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (bCardType==CT_MISSILE_CARD) m_bTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}
//Ѱ�ҵ���
WORD CTableFrameSink::FindLandUserBySpecialCard(BYTE bHandCardData [3][20], BYTE bCardCount [3], WORD wBeginChair)
{
	for(WORD i = 0; i < 3; i ++)
	{
		WORD wChairID = (wBeginChair + i )%GAME_PLAYER;
		if(m_GameLogic.HaveGuiPairCard(bHandCardData[wChairID], bCardCount[wChairID]) ||
			m_GameLogic.HaveFour2Card(bHandCardData[wChairID], bCardCount[wChairID]))
		{
			return wChairID;
		}
	}
	return INVALID_CHAIR;
}
//////////////////////////////////////////////////////////////////////////
