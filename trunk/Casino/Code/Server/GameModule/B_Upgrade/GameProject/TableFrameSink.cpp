#include "StdAfx.h"
#include "TableFrameSink.h"

//��ɫ����
#define CALL_NONE					0x00								//���ƶ���
#define CALL_NT						0x10								//���ƶ���
#define CALL_HEI_TAO				0x01								//���ƶ���
#define CALL_HONG_TAO				0x02								//���ƶ���
#define CALL_MEI_HUA				0x04								//���ƶ���
#define CALL_FANG_KUAI				0x08								//���ƶ���

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
	VarDecFromR8(m_pGameEnd->fScore[wChairID], &decTemp);
	VarDecAdd(&decTemp, &pPartedWinScore[0], &pPartedWinScore[0]);

	//�жϴ���0,���г�ˮ����
	if(VarDecCmpR8(&decTemp, 0) == VARCMP_GT)
	{
		//��ˮ = ���ɲ� * (- ��ˮ��)
		VarDecMul(&decTemp,(DECIMAL*) &m_decAfterTax, &pPartedTaxScore[0]);
		//����Double�ɲ�
		VarDecAdd(&decTemp,&pPartedTaxScore[0], &decTemp);
		VarR8FromDec(&decTemp, &m_pGameEnd->fScore[wChairID]);
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
	//�߼�����
	m_cbPackCount=2;
	m_cbMainValue=0x02;
	m_cbMainColor=COLOR_ERROR;

	//������Ϣ
	m_cbValueOrder[0]=2;
	m_cbValueOrder[1]=2;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	//�߼�����
	m_cbPackCount=2;
	m_cbMainColor=COLOR_ERROR;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	m_pITableFrame->SetGameStatus(GS_UG_CALL);

	//���ñ���
	m_cbMainColor=COLOR_ERROR;

	//��������
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetMainValue(m_cbMainValue);
	m_GameLogic.SetPackCount(m_cbPackCount);

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.cbPackCount=m_cbPackCount;
	GameStart.cbMainValue=m_cbMainValue;
	GameStart.cbValueOrder[0]=m_cbValueOrder[0];
	GameStart.cbValueOrder[1]=m_cbValueOrder[1];

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//�����˿�
	DispatchUserCard();

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
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

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
			ZeroMemory(&GameEnd,sizeof(GameEnd));


			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			
			//�û�����
			GameEnd.wBankerUser=m_wBankerUser;

			//���Ʊ���
			GameEnd.cbCallCard=m_cbCallCard;
			GameEnd.cbCallCount=m_cbCallCount;
			GameEnd.wCallCardUser=m_wCallCardUser;

			//�÷ֱ���
			GameEnd.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(GameEnd.cbScoreCardData,m_cbScoreCardData,sizeof(GameEnd.cbScoreCardData));

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//�۵ױ���
			if ((wChairID!=m_wBankerUser)&&(((wChairID+2)%m_wPlayerCount)!=m_wBankerUser))
			{
				//��������
				BYTE cbMaxCount=1;
				GameEnd.wConcealTime=1;

				//��������
				tagAnalyseResult AnalyseResult;
				m_GameLogic.AnalyseCardData(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID],AnalyseResult);

				//ͬ�Ʒ���
				for (BYTE i=0;i<CountArray(AnalyseResult.SameDataInfo);i++)
				{
					if (AnalyseResult.SameDataInfo[i].cbBlockCount>0) cbMaxCount=__max(i,cbMaxCount);

				}

				//���Ʒ���
				for (BYTE i=0;i<CountArray(AnalyseResult.TractorDataInfo);i++)
				{
					//��ȡ����
					BYTE cbSameCount=i+2;
					BYTE cbMaxLength=AnalyseResult.TractorDataInfo[i].cbTractorMaxLength;

					//���ñ���
					cbMaxCount=__max(cbSameCount*cbMaxLength,cbMaxCount);
				}

				//���ñ���
				cbMaxCount=__min(cbMaxCount,6);
				for (BYTE i=0;i<cbMaxCount;i++) GameEnd.wConcealTime*=2;
			}

			//ͳ�Ʊ���
			DOUBLE fBankerScore=0;
			WORD wWholeScore=GameEnd.wGameScore+GameEnd.wConcealTime*GameEnd.wConcealScore;

			//���ͳ��
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

			//��ҵ÷�
			GameEnd.fScore[m_wBankerUser]=fBankerScore*m_pGameServiceOption->dwCellScore;
			GameEnd.fScore[(m_wBankerUser+2)%m_wPlayerCount]=fBankerScore*m_pGameServiceOption->dwCellScore;
			GameEnd.fScore[(m_wBankerUser+1)%m_wPlayerCount]=-(fBankerScore*m_pGameServiceOption->dwCellScore);
			GameEnd.fScore[(m_wBankerUser+3)%m_wPlayerCount]=-(fBankerScore*m_pGameServiceOption->dwCellScore);

			if(m_CalculateSink.InitCalculateContext(&GameEnd))
			{
				m_pITableFrame->WriteGameEnd(GER_NORMAL,(BYTE*)&GameEnd, sizeof(GameEnd), 
					m_CalculateSink.GetCalculateContext(),&m_CalculateSink);


				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//ׯ���л�
				if (GameEnd.fScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
				else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

				//��������
				m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

				//������Ϸ
				m_pITableFrame->ConcludeGame();

				return true;
			}
			else
			{
				GT_ASSERT(FALSE);
				//ׯ���л�
				if (GameEnd.fScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
				else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

				//��������
				m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

				//������Ϸ
				m_pITableFrame->ConcludeGame();
				return false;
			}

		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		
			//�û�����
			GameEnd.wBankerUser=m_wBankerUser;

			//���Ʊ���
			GameEnd.cbCallCard=m_cbCallCard;
			GameEnd.cbCallCount=m_cbCallCount;
			GameEnd.wCallCardUser=m_wCallCardUser;

			//�÷ֱ���
			GameEnd.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(GameEnd.cbScoreCardData,m_cbScoreCardData,sizeof(GameEnd.cbScoreCardData));

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//��ҵ÷�
			GameEnd.fScore[wChairID]=-(int)(6*m_pGameServiceOption->dwCellScore);
			//ͳ�ƶ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i) continue;
				GameEnd.fScore[i] =m_pGameServiceOption->dwCellScore * 2;
				
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
	case GS_UG_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.fBaseScore=m_pGameServiceOption->dwCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_UG_CALL:		//����״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//��Ϸ����
			StatusCall.wBankerUser=m_wBankerUser;
			StatusCall.cbPackCount=m_cbPackCount;
			StatusCall.cbMainValue=m_cbMainValue;
			StatusCall.cbValueOrder[0]=m_cbValueOrder[0];
			StatusCall.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusCall.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusCall.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusCall.cbHandCardCount[i] = m_cbHandCardCount[i];
			}

			//������Ϣ
			StatusCall.cbCallCard=m_cbCallCard;
			StatusCall.cbCallCount=m_cbCallCount;
			StatusCall.wCallCardUser=m_wCallCardUser;
			StatusCall.cbComplete=(m_bCallCard[wChiarID]==true)?TRUE:FALSE;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_UG_BACK:		//����״̬
		{
			//��������
			CMD_S_StatusBack StatusBack;
			ZeroMemory(&StatusBack,sizeof(StatusBack));

			//��Ϸ����
			StatusBack.cbPackCount=m_cbPackCount;
			StatusBack.cbMainValue=m_cbMainValue;
			StatusBack.cbMainColor=m_cbMainColor;
			StatusBack.cbValueOrder[0]=m_cbValueOrder[0];
			StatusBack.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusBack.cbCallCard=m_cbCallCard;
			StatusBack.cbCallCount=m_cbCallCount;
			StatusBack.wCallCardUser=m_wCallCardUser;

			//�û�����
			StatusBack.wBankerUser=m_wBankerUser;
			StatusBack.wCurrentUser=m_wCurrentUser;

			//�˿˱���
			StatusBack.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusBack.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusBack.cbHandCardCount[i] = m_cbHandCardCount[i];
			}


			//������Ϣ
			if (wChiarID==m_wBankerUser)
			{
				StatusBack.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusBack.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusBack,sizeof(StatusBack));
		}
	case GS_UG_PLAY:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.cbPackCount=m_cbPackCount;
			StatusPlay.cbMainValue=m_cbMainValue;
			StatusPlay.cbMainColor=m_cbMainColor;
			StatusPlay.cbValueOrder[0]=m_cbValueOrder[0];
			StatusPlay.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusPlay.cbCallCard=m_cbCallCard;
			StatusPlay.cbCallCount=m_cbCallCount;
			StatusPlay.wCallCardUser=m_wCallCardUser;

			//�û�����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wFirstOutUser=m_wFirstOutUser;

			//�˿˱���
			StatusPlay.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusPlay.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//������Ϣ
			if (wChiarID==m_wBankerUser)
			{
				StatusPlay.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusPlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���Ʊ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusPlay.cbOutCardCount[i]=m_cbOutCardCount[i];
				CopyMemory(StatusPlay.cbOutCardData[i],m_cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i]);
				StatusPlay.cbHandCardCount[i] = m_cbHandCardCount[i];
			}

			//�÷ֱ���
			StatusPlay.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(StatusPlay.cbScoreCardData,m_cbScoreCardData,sizeof(StatusPlay.cbScoreCardData));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_UG_WAIT:	//�ȴ�״̬
		{
			//��������
			CMD_S_StatusWait StatusWait;
			ZeroMemory(&StatusWait,sizeof(StatusWait));

			//�û�����
			StatusWait.wBankerUser=m_wBankerUser;

			//��Ϸ����
			StatusWait.cbPackCount=m_cbPackCount;
			StatusWait.cbMainValue=m_cbMainValue;
			StatusWait.cbValueOrder[0]=m_cbValueOrder[0];
			StatusWait.cbValueOrder[1]=m_cbValueOrder[1];

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusWait,sizeof(StatusWait));
		}
	}

	return false;
}
//�˹�������Ϸ����
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	switch(cbGameStatus) 
	{
	case GS_UG_CALL:		//����״̬
		{
			WORD wActionUser[GAME_PLAYER];
			BYTE bActionUserCount = 0;
			for(BYTE i = 0; i < GAME_PLAYER; i ++)
			{
				if(m_bCallCard[i] == true)
					continue;
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

			if(bActionUserCount)
			{
				//�Ƿ�AI��ɱ��ͨ���
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
	case GS_UG_BACK:		//����״̬
		{
			//�������ж�
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentUser);
			if(pIServerUserItem == NULL || 
				pIServerUserItem->IsAIUser() == false || 
				pIServerUserItem->GetUserStatus() != US_PLAY)
				return true;
			WORD wChairID = pIServerUserItem->GetChairID();
			if(wChairID == INVALID_CHAIR)
				return true;
			//���
			AI_ConcealCard(wChairID, pIServerUserItem);
		}
		break;
	case GS_UG_PLAY:		//��Ϸ״̬
		{
			//�������ж�
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
//��������
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
//��������
BYTE CTableFrameSink::AI_GetCallCardMask(WORD wCallCardUser)
{
	//��������
	BYTE cbCallMask=CALL_NONE;
	BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

	BYTE cbDispatchCardCount = m_cbHandCardCount[wCallCardUser];
	BYTE cbDispatchCardData[MAX_COUNT];
	memcpy(cbDispatchCardData, m_cbHandCardData[wCallCardUser], sizeof(cbDispatchCardData));

	//��������
	BYTE cbCardCount[6]={0,0,0,0,0,0};
	BYTE cbCallMaskArray[6]={CALL_FANG_KUAI,CALL_MEI_HUA,CALL_HONG_TAO,CALL_HEI_TAO,CALL_NT,CALL_NT};
	BYTE cbCallColorArray[6]={COLOR_FANG_KUAI,COLOR_MEI_HUA,COLOR_HONG_TAO,COLOR_HEI_TAO,COLOR_NT,COLOR_NT};

	//���µ���
	if (m_wCallCardUser!=wCallCardUser)
	{
		//�˿�ͳ��
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			//��ȡ����
			BYTE cbCardColor=m_GameLogic.GetCardColor(cbDispatchCardData[i]);
			BYTE cbCardValue=m_GameLogic.GetCardValue(cbDispatchCardData[i]);

			//����ͳ��
			if (cbCardColor==COLOR_NT) cbCardCount[4+cbCardValue-0x0E]++;
			else if (cbCardValue==m_cbMainValue) cbCardCount[cbCardColor>>4]++;
		}

		//���ƿ���
		for (BYTE i=0;i<CountArray(cbCardCount);i++)
		{
			//�����ж�
			if ((cbCardCount[i]==0)||(cbCardCount[i]<m_cbCallCount)) continue;

			//��Ŀ�ж�
			if ((i<4)&&(cbCardCount[i]>m_cbCallCount))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}

			//�����ж�
			if ((i>=4)&&(cbCardCount[i]>m_cbCallCount)&&(cbCardCount[i]>=2))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}

			//��ɫ�ж�
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
		//�˿�ͳ��
		BYTE cbCallCardCount=0;
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			if (cbDispatchCardData[i]==m_cbCallCard) cbCallCardCount++;
		}

		//���ƿ���
		if (cbCallCardCount>m_cbCallCount) cbCallMask=cbCallMaskArray[cbCallColor>>4];
	}


	return cbCallMask;
}
//����
bool CTableFrameSink::AI_CallCard(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbCallColor)
{
	//��������
	BYTE cbCallCount=0;
	BYTE cbCallCard=0x00;

	BYTE cbDispatchCardCount = m_cbHandCardCount[wChairID];
	BYTE cbDispatchCardData[MAX_COUNT];
	memcpy(cbDispatchCardData, m_cbHandCardData[wChairID], sizeof(cbDispatchCardData));

	//����ͳ��
	if (cbCallColor==COLOR_NT)
	{
		//��Ŀͳ��
		BYTE cbNTCount[2]={0,0};
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			BYTE cbCardData=cbDispatchCardData[i];
			if ((cbCardData==0x4E)||(cbCardData==0x4F)) cbNTCount[cbCardData-0x4E]++;
		}

		//��������
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
		//��Ŀͳ��
		for (BYTE i=0;i<cbDispatchCardCount;i++)
		{
			//��ȡ����
			BYTE cbCardData=cbDispatchCardData[i];
			BYTE cbCardColor=m_GameLogic.GetCardColor(cbCardData);
			BYTE cbCardValue=m_GameLogic.GetCardValue(cbCardData);

			//����ͳ��
			if ((cbCardValue==m_cbMainValue)&&(cbCardColor==cbCallColor)) 
			{
				cbCallCount++;
				cbCallCard=cbCardData;
			}
		}
	}

	if(cbCallCount)
	{
		//��������
		CMD_C_CallCard CallCard;
		CallCard.cbCallCard=cbCallCard;
		CallCard.cbCallCount=cbCallCount;

		//��������
		OnGameMessage(SUB_C_CALL_CARD,&CallCard,sizeof(CallCard), pIServerUserItem);
	}

	return true;
}
//���
bool CTableFrameSink::AI_ConcealCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	BYTE cbDispatchCardCount = m_cbHandCardCount[wChairID];
	BYTE cbDispatchCardData[MAX_COUNT];
	memcpy(cbDispatchCardData, m_cbHandCardData[wChairID], sizeof(cbDispatchCardData));

	BYTE	cbConcealCount = m_cbConcealCount;	//������Ŀ
	BYTE	cbConcealCard[8];					//�����˿�

	//��ȡ����
	BYTE cbResultCard[MAX_COUNT];
	BYTE cbResultCount = m_GameLogic.DistillSingleCard(cbDispatchCardData,
		cbDispatchCardCount, 
		m_cbMainColor, 
		m_cbMainValue, 
		cbResultCard);
	//�жϵ����㹻
	if(cbResultCount >= m_cbConcealCount)
	{
		m_GameLogic.SortCardList(cbResultCard,cbResultCount);
		memcpy(cbConcealCard, cbResultCard + (cbResultCount - cbConcealCount) , cbConcealCount);
	}
	else
	{
		//���浥��
		memcpy(cbConcealCard, cbResultCard, cbResultCount);
		//ɾ����
		m_GameLogic.RemoveCard(cbResultCard,cbResultCount, cbDispatchCardData, cbDispatchCardCount);
		cbDispatchCardCount -= cbResultCount;
		
		//������
		m_GameLogic.SortCardList(cbDispatchCardData,cbDispatchCardCount);
		BYTE cbMoreCount = m_cbConcealCount - cbResultCount;
		memcpy(cbConcealCard + cbResultCount, cbDispatchCardData + (cbDispatchCardCount - cbMoreCount) , cbMoreCount);
	}


	//��������
	CMD_C_ConcealCard ConcealCard;
	ConcealCard.cbConcealCount=cbConcealCount;
	CopyMemory(ConcealCard.cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//��������
	OnGameMessage(SUB_C_CONCEAL_CARD,&ConcealCard,sizeof(ConcealCard), pIServerUserItem);

	return true;
}
//����
bool CTableFrameSink::AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//��������
	BYTE cbHandCardData[MAX_COUNT];
	memcpy(cbHandCardData, m_cbHandCardData[wChairID], sizeof(cbHandCardData));
	BYTE cbHandCardCount = m_cbHandCardCount[wChairID];

	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	bool bSearchOutCard = false;
	BYTE cbTurnCardCount=(wChairID==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser];
	//�Ƿ�AI��ɱ��ͨ���
	if(m_pITableFrame->IsAIKillGameUser())
	{
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
		//�жϻ������Ƿ���ͨ��ҵĶԼ�
		if(wGameUser%2 == wChairID%2)
		{
			
		}
		else
		{
			//�ж��Ƿ��һ����
			if(cbTurnCardCount == 0)
			{
				//Ѱ����ѳ���
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
	//�������
	{
		//��������
		if(bSearchOutCard == false)
			m_GameLogic.SearchOutCard(cbHandCardData,
				cbHandCardCount,
				m_cbOutCardData[m_wFirstOutUser],
				cbTurnCardCount,
				OutCardResult);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.cbCardCount=OutCardResult.cbCardCount;
		CopyMemory(OutCard.cbCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCard.cbCardCount);
		OnGameMessage(SUB_C_OUT_CARD,&OutCard,
			sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE), pIServerUserItem);

	}

	return true;
}
//Ѱ����ѳ���
bool CTableFrameSink::AI_SearchFirstBestOutCard(BYTE cbHandCardData[],
							   BYTE cbHandCardCount,
							   WORD wChairID,
							   tagOutCardResult& OutCardResult)
{
	memset(&OutCardResult, 0, sizeof(OutCardResult));


	return false;
}
//�жϸ�����ƴ���
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
//�жϸ�����ƴ���
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
//�жϸ���ĵ��ƴ���
bool CTableFrameSink::AI_CompareMostCardExist(BYTE cbHandCardData[],
											  BYTE cbHandCardCount,BYTE cbCardData)
{
	for(BYTE i = 0; i < cbHandCardCount; i ++)
	{
		if(m_GameLogic.CompareCardData(cbCardData,cbHandCardData[i])) return true;
	}
	return false;
}
//Ѱ��ƥ�����
bool CTableFrameSink::AI_SearchMatchOutCard(BYTE cbHandCardData[],
						   BYTE cbHandCardCount,
						   BYTE cbMatchCardData[],
						   BYTE cbMatchCardCount,
						   tagOutCardResult& OutCardResult)
{
	

	return false;
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
	case SUB_C_CALL_CARD:		//�û�����
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(CMD_C_CallCard));
			if (wDataSize!=sizeof(CMD_C_CallCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_CallCard * pCallCard=(CMD_C_CallCard *)pDataBuffer;
			return OnUserCallCard(pUserData->UserStatus.wChairID,pCallCard->cbCallCard,pCallCard->cbCallCount);
		}
	case SUB_C_CALL_FINISH:		//�������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserCallFinish(pUserData->UserStatus.wChairID);
		}
	case SUB_C_CONCEAL_CARD:	//�����˿�
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(CMD_C_ConcealCard));
			if (wDataSize!=sizeof(CMD_C_ConcealCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_ConcealCard * pConcealCard=(CMD_C_ConcealCard *)pDataBuffer;
			return OnUserConcealCard(pUserData->UserStatus.wChairID,pConcealCard->cbConcealCard,pConcealCard->cbConcealCount);
		}
	case SUB_C_OUT_CARD:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			GT_ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			GT_ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->UserStatus.wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
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
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//���ñ���
	if (bLookonUser==false)
	{
		m_cbMainValue=0x02;
		m_cbValueOrder[0]=0x02;
		m_cbValueOrder[1]=0x02;
		m_wBankerUser=INVALID_CHAIR;
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount)
{
	//Ч��״̬
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//�û�Ч��
	GT_ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//��Ŀͳ��
	BYTE cbHandCardCount=0;
	for (BYTE i=0;i<m_cbHandCardCount[wChairID];i++)
	{
		if (m_cbHandCardData[wChairID][i]==cbCallCard) cbHandCardCount++;
	}

	//Ч������
	GT_ASSERT((cbHandCardCount>=cbCallCount)&&(cbCallCount>0));
	if ((cbHandCardCount<cbCallCount)||(cbCallCount==0)) return false;

	//�����ж�
	if (m_wCallCardUser==wChairID)
	{
		//Ч������
		GT_ASSERT((cbCallCard==m_cbCallCard)&&(cbCallCount>m_cbCallCount));
		if ((cbCallCard!=m_cbCallCard)||(cbCallCount<=m_cbCallCount)) return true;
	}
	else
	{
		//Ч������
		BYTE cbPackCount=m_GameLogic.GetPackCount();
		BYTE cbCardColor=m_GameLogic.GetCardColor(cbCallCard);
		BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

		//Ч������
		GT_ASSERT((cbCallCount>m_cbCallCount)||((cbCallCount==cbPackCount)&&(cbCardColor>cbCallColor)));
		if ((cbCallCount<=m_cbCallCount)&&((cbCallCount!=cbPackCount)||(cbCardColor<=cbCallColor))) return true;
	}

	//���ñ���
	m_cbCallCard=cbCallCard;
	m_wCallCardUser=wChairID;
	m_cbCallCount=cbCallCount;

	//��������
	CMD_S_CallCard CallCard;
	CallCard.cbCallCard=m_cbCallCard;
	CallCard.cbCallCount=m_cbCallCount;
	CallCard.wCallCardUser=m_wCallCardUser;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));

	return true;
}

//�������
bool CTableFrameSink::OnUserCallFinish(WORD wChairID)
{
	WORD i = 0;
	//Ч��״̬
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//�û�Ч��
	GT_ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//��������
	m_bCallCard[wChairID]=true;

	//����ж�
	for (i=0;i<m_wPlayerCount;i++)
	{
		if (m_bCallCard[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;

	//ׯ������
	if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wCallCardUser;

	//���·���
	if (m_wBankerUser==INVALID_CHAIR)
	{
		//����״̬
		ZeroMemory(m_bCallCard,sizeof(m_bCallCard));

		//�����˿�
		DispatchUserCard();

		return true;
	}

	//��ɫ����
	if (m_wCallCardUser==INVALID_CHAIR) m_cbMainColor=COLOR_NT;
	else m_cbMainColor=m_GameLogic.GetCardColor(m_cbCallCard);

	//���ñ���
	m_wCurrentUser=m_wBankerUser;
	m_wFirstOutUser=m_wBankerUser;
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_pITableFrame->SetGameStatus(GS_UG_BACK);

	//���õ���
	BYTE cbHandCardCount=m_cbHandCardCount[m_wCurrentUser];
	CopyMemory(&m_cbHandCardData[m_wCurrentUser][cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
	m_cbHandCardCount[m_wCurrentUser]+=m_cbConcealCount;

	//��������
	CMD_S_SendConceal SendConceal;
	SendConceal.cbMainColor=m_cbMainColor;
	SendConceal.wBankerUser=m_wBankerUser;
	SendConceal.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�������
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

		//������Ϣ
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
	}

	return true;
}

//�����˿�
bool CTableFrameSink::OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount)
{
	//Ч��״̬
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_BACK);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_BACK) return true;

	//Ч������
	GT_ASSERT((wChairID==m_wCurrentUser)&&(m_cbConcealCount==cbConcealCount));
	if ((wChairID!=m_wCurrentUser)||(m_cbConcealCount!=cbConcealCount)) return false;

	//ɾ������
	if (m_GameLogic.RemoveCard(cbConcealCard,cbConcealCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		GT_ASSERT(FALSE);
		return false;
	}

	//�����˿�
	m_cbHandCardCount[wChairID]-=cbConcealCount;
	CopyMemory(m_cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//�˿�����
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		GT_ASSERT(m_cbHandCardCount[i]==m_GameLogic.GetDispatchCount());
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
	}

	//����״̬
	m_pITableFrame->SetGameStatus(GS_UG_PLAY);

	//��������
	CMD_S_GamePlay GamePlay;
	GamePlay.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�������
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

		//������Ϣ
		m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч��״̬
	GT_ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_PLAY) return true;

	//Ч���û�
	GT_ASSERT((wChairID==m_wCurrentUser)&&(cbCardCount<=MAX_COUNT));
	if ((wChairID!=m_wCurrentUser)||(cbCardCount>MAX_COUNT)) return false;

	//�׳�����
	if (m_wFirstOutUser==wChairID)
	{
		//�����ж�
		BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
		if (cbCardType==CT_ERROR) return false;

		//˦�ƴ���
		if (cbCardType==CT_THROW_CARD)
		{
			//˦���ж�
			tagOutCardResult OutCardResult;
			ZeroMemory(&OutCardResult,sizeof(OutCardResult));
			bool bThrowSuccess=m_GameLogic.EfficacyThrowCard(cbCardData,cbCardCount,wChairID,m_cbHandCardData,m_cbHandCardCount[wChairID],OutCardResult);

			//�������
			if (bThrowSuccess==false)
			{
				//ɾ���˿�
				if (m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
				m_cbHandCardCount[wChairID]-=OutCardResult.cbCardCount;

				//���Ƽ�¼
				m_cbOutCardCount[wChairID]=OutCardResult.cbCardCount;
				CopyMemory(m_cbOutCardData[wChairID],OutCardResult.cbResultCard,OutCardResult.cbCardCount);

				//�л��û�
				m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

				//��������
				CMD_S_ThrowResult ThrowResult;
				ThrowResult.wOutCardUser=wChairID;
				ThrowResult.wCurrentUser=m_wCurrentUser;
				ThrowResult.cbThrowCardCount=cbCardCount;
				ThrowResult.cbResultCardCount=OutCardResult.cbCardCount;
				CopyMemory(&ThrowResult.cbCardDataArray[0],cbCardData,sizeof(BYTE)*cbCardCount);
				CopyMemory(&ThrowResult.cbCardDataArray[cbCardCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);

				//���ͼ���
				BYTE cbCardDataCount=ThrowResult.cbThrowCardCount+ThrowResult.cbResultCardCount;
				WORD wSendSize=sizeof(ThrowResult)-sizeof(ThrowResult.cbCardDataArray)+cbCardDataCount*sizeof(BYTE);

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);

				return true;
			}
		}
	}
	else
	{
		//��Ŀ�ж�
		GT_ASSERT(cbCardCount==m_cbOutCardCount[m_wFirstOutUser]);
		if (cbCardCount!=m_cbOutCardCount[m_wFirstOutUser]) return false;

		//����Ч��
		if (m_GameLogic.EfficacyOutCard(cbCardData,cbCardCount,m_cbOutCardData[m_wFirstOutUser],
			m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
		{
			GT_ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
	m_cbHandCardCount[wChairID]-=cbCardCount;

	//���Ƽ�¼
	m_cbOutCardCount[wChairID]=cbCardCount;
	CopyMemory(m_cbOutCardData[wChairID],cbCardData,cbCardCount);

	//�л��û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wFirstOutUser) m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//һ�ֽ���
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//�ж�ʤ��
		m_wTurnWinner=m_GameLogic.CompareCardArray(m_cbOutCardData,m_cbOutCardCount[0],m_wFirstOutUser);

		//����÷�
		if ((m_wTurnWinner!=m_wBankerUser)&&((m_wTurnWinner+2)%m_wPlayerCount!=m_wBankerUser))
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
				BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
				m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i],m_cbOutCardCount[i],pcbScoreCard,cbMaxCount);
			}
		}

		//���ñ���
		if (m_cbHandCardCount[m_wTurnWinner]!=0)
		{
			m_wFirstOutUser=m_wTurnWinner;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
		}

		//�û��л�
		m_wCurrentUser=(m_cbHandCardCount[m_wTurnWinner]==0)?INVALID_CHAIR:m_wTurnWinner;

		//��������
		CMD_TurnBalance TurnBalance;
		TurnBalance.wTurnWinner=m_wTurnWinner;
		TurnBalance.wCurrentUser=m_wCurrentUser;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
	}

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(m_wTurnWinner,NULL,GER_NORMAL);

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchUserCard()
{
	//�����˿�
	BYTE cbRandCard[CELL_PACK*2];
	m_GameLogic.RandCardList(cbRandCard,CELL_PACK*m_cbPackCount);

	//�û��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbHandCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//�����˿�
	m_cbConcealCount=CELL_PACK*m_cbPackCount-m_wPlayerCount*m_GameLogic.GetDispatchCount();
	CopyMemory(m_cbConcealCard,&cbRandCard[CELL_PACK*m_cbPackCount-m_cbConcealCount],sizeof(BYTE)*m_cbConcealCount);

	//���Ϳ�ʼ
	CMD_S_SendCard SendCard;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//Ч��״̬
		GT_ASSERT(m_cbHandCardCount[i]<=CountArray(SendCard.cbCardData));
		if (m_cbHandCardCount[i]>CountArray(SendCard.cbCardData)) return false;

		//�����˿�
		CopyMemory(SendCard.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
		SendCard.cbCardCount=m_cbHandCardCount[i];
		CopyMemory(SendCard.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//����ͳ��
		WORD wHeadSize=sizeof(SendCard)-sizeof(SendCard.cbCardData);
		WORD wSendSize=wHeadSize+sizeof(SendCard.cbCardData[0])*SendCard.cbCardCount;

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
