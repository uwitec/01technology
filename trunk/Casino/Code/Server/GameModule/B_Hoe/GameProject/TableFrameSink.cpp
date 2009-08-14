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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��ʷ���
	memset(m_fAllTurnScore,0,sizeof(m_fAllTurnScore));
	memset(m_fLastTurnScore,0,sizeof(m_fLastTurnScore));

	//Run��Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pICalculateFrame=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//Run��Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
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
	//����״̬
	m_pITableFrame->SetGameStatus(GS_RF_PLAYING);

	//�����˿�
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=13;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//�����û�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=0;j<m_bCardCount[i];j++)
		{
			if (m_bHandCardData[i][j]==0x03) 
			{
				//�����û�
				m_wBankerUser=i;
				m_wCurrentUser=i;

				goto SET_CURRENT_USER;
			}
		}
	}

	//�����ж�
	GT_ASSERT(FALSE);
	SET_CURRENT_USER:

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(GameStart.cbCardData));
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
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			GT_ASSERT(pIServerUserItem!=NULL);
			GT_ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ���Ʒ�
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

			//ͳ�ƶ��
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

				//���ͳ��
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					m_fAllTurnScore[i]+=GameEnd.fGameScore[i];
					m_fLastTurnScore[i]=GameEnd.fGameScore[i];
				}
			
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
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ�ƶ��
			DOUBLE fCardScore[GAME_PLAYER];
			memset(fCardScore, 0, sizeof(DOUBLE)*GAME_PLAYER);
			fCardScore[wChairID]=4.0 * 13 *m_pGameServiceOption->dwCellScore;//һ�Ұ����ң�ʮ��̫��

			//ͳ�ƶ��
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
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_RF_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.fBaseScore=m_pGameServiceOption->dwCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.fBaseScore=m_pGameServiceOption->dwCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);

			//��ʷ���
			CopyMemory(StatusPlay.fAllTurnScore,m_fAllTurnScore,sizeof(m_fAllTurnScore));
			CopyMemory(StatusPlay.fLastTurnScore,m_fLastTurnScore,sizeof(m_fLastTurnScore));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
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
		return false;
	WORD wChiarID = pIServerUserItem->GetChairID();
	if(wChiarID == INVALID_CHAIR)
		return false;
	
	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//�׳��ж�
		BYTE cbOutCard=m_bHandCardData[wChiarID][m_bCardCount[wChiarID]-1];
		if ((m_wBankerUser==wChiarID)&&(m_bCardCount[m_wBankerUser]==13)) cbOutCard=0x03;

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=cbOutCard;
		OnGameMessage(SUB_C_OUT_CART,
			&OutCard,
			sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE), pIServerUserItem);

	}
	else
	{
		BYTE cbCardCount = 0;//�˿���Ŀ
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
			//��������
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
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->UserStatus.wChairID);
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
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	BYTE i = 0;
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;


	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�׳��ж�
	if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==13))
	{
		for ( i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x03) break;
		}
		if (i==bCardCount) return false;
	}

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//�����ж�
	bool bMostest=false;
	if ((bCardCount<=3)&&(bCardData[0]==0x32)) bMostest=true;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bMostest==false) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
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
	if (bMostest==true) m_bTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
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

//////////////////////////////////////////////////////////////////////////
