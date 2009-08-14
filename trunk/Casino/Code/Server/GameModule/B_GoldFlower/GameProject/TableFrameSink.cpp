#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define IDI_TIME_GIVE_UP						1							//������ʱ��

//��̬����
const BYTE	CGameLogic::m_bCardArray[52]=									//�˿�����
{
	 1,2,3,4,5,6,7,8,9,10,11,12,13,									        //����A-K
	14,15,16,17,18,19,20,21,22,23,24,25,26,
	27,28,29,30,31,32,33,34,35,36,37,38,39,
	40,41,42,43,44,45,46,47,48,49,50,51,52,
};

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ
//static BYTE                            nWiner;                                     //Ӯ��    //ADD

//////////////////////////////////////////////////////////////////////////////
////��ȡ�˿˻�ɫ
BYTE CGameLogic::GetHuaKind(BYTE bCard)
{
	return (bCard-1)/13;
}

//��ȡ�˿���ֵ
BYTE CGameLogic::GetCardValoe(BYTE bCard)
{
	BYTE bVolea=(bCard-1)%13;
	if (bVolea==0) bVolea=13;
	return bVolea;
}

//��ȡ�˿�����
BYTE CGameLogic::GetCardKind(BYTE bCardList[], BYTE bCardCount)
{
	if (IsSanTiao(bCardList,bCardCount)) return SAN_TIAO;
	if (IsTongHuaShun(bCardList,bCardCount)) return TONG_HUA_SHUN;
	//if (IsTieZhi(bCardList,bCardCount)) return TIE_ZHI;
	//if (IsHuLu(bCardList,bCardCount)) return HU_LU;
	if (IsTongHua(bCardList,bCardCount)) return TONG_HUA;
	if (IsShunZhi(bCardList,bCardCount)) return SHUN_ZI;
	//if (IsSanTiao(bCardList,bCardCount)) return SAN_TIAO;
	//if (IsLiangDui(bCardList,bCardCount)) return LAING_DUI;
	if (IsDuiZhi(bCardList,bCardCount)) return DUI_ZI;
	if (IsTeShu(bCardList,bCardCount)) return TeShu;
	return SHAN;
}


//���Ҷ���
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

//��������
bool CGameLogic::IsTeShu(BYTE bCardList[], BYTE bCardCount)
{
    if(bCardList[0]%13==5&&bCardList[1]%13==3&&bCardList[2]%13==2) return true;

	return false;
}

//�Ƿ�ͬ��˳
bool CGameLogic::IsTongHuaShun(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	BYTE bFirstCardVoloe=GetCardValoe(bCardList[0]);
	/*for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
		if (bFirstCardVoloe!=(GetCardValoe(bCardList[i])+i)) return false;
    }*/
	for (BYTE i=1;i<bCardCount;i++)
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
	if(!IsShunZhi(bCardList,bCardCount)) return false;
	return true;
}

//�Ƿ���֧
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

//�Ƿ��«
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

//�Ƿ�ͬ��
bool CGameLogic::IsTongHua(BYTE bCardList[], BYTE bCardCount)
{
	//if (bCardCount!=5) return false;
	
	BYTE bBaseHua=GetHuaKind(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetHuaKind(bCardList[i])!=bBaseHua) return false;
	}
	return true;
}

//�Ƿ�˳��
bool CGameLogic::IsShunZhi(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount!=3) return false;
	
	//BYTE bBaseHua=GetHuaKind(bCardList[0]);
	if((GetCardValoe(bCardList[0])==13) && (GetCardValoe(bCardList[1])==2) && (GetCardValoe(bCardList[2])==1))  return true;
	BYTE bFirstCardVoloe=GetCardValoe(bCardList[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (bFirstCardVoloe!=(GetCardValoe(bCardList[i])+i)) return false;
	}
	return true;
}

//�Ƿ�����
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

//�Ƿ�����
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

//�Ƿ����
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

//�Աȵ�ֻ�˿�
bool CGameLogic::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstVolae=GetCardValoe(bFirstCard);
	BYTE bLastVolae=GetCardValoe(bLastCard);
	if (bFirstVolae==bLastVolae) return bFirstCard>bLastCard;
	return bFirstVolae>bLastVolae;
}

//�Ա�ͬ��˳
bool CGameLogic::CompareTongHuaShun(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//�Ա���֧
bool CGameLogic::CompareTeiZhi(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//�ԱȺ�«
bool CGameLogic::CompareHuLu(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//�Ա�ͬ��
bool CGameLogic::CompareTongHua(BYTE bFirstList[], BYTE bLastList[])
{
	BYTE bFirst=0,bLast=0;
	for (BYTE i=0;i<3;i++)
	{
		bFirst=GetCardValoe(bFirstList[i]);
		bLast=GetCardValoe(bLastList[i]);
		if (bFirst!=bLast) return (bFirst>bLast);
	}
	return GetHuaKind(bFirstList[0])>GetHuaKind(bLastList[0]);
}

//�Ա�˳��
bool CGameLogic::CompareShunZhi(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//�Ա�����
bool CGameLogic::CompareSanTiao(BYTE bFirstList[], BYTE bLastList[])
{
	return CompareOnlyOne(bFirstList[2],bLastList[2]);
}

//�Ա�����
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

//�Աȶ���
bool CGameLogic::CompareDuiZhi(BYTE bFirstList[], BYTE bLastList[],BYTE bCardCount)
{
	if(GetCardValoe(bFirstList[1])!=GetCardValoe(bLastList[1])) return GetCardValoe(bFirstList[1])>GetCardValoe(bLastList[1]);
	if(GetCardValoe(bFirstList[0])!=GetCardValoe(bLastList[0])) return GetCardValoe(bFirstList[0])>GetCardValoe(bLastList[0]);
	if(GetCardValoe(bFirstList[2])!=GetCardValoe(bLastList[2])) return GetCardValoe(bFirstList[2])>GetCardValoe(bLastList[2]);
	/*if (FindDoubleCard(bFirstList,bCardCount,bDoubleCard[0])!=1) return false;
	if (FindDoubleCard(bLastList,bCardCount,bDoubleCard[1])!=1) return true;
	BYTE bFirstVoloe=GetCardValoe(bDoubleCard[0][0]);
	BYTE bLastVoloe=GetCardValoe(bDoubleCard[1][0]);
	if (bFirstVoloe==bLastVoloe) return (GetHuaKind(bDoubleCard[0][0])==HEI_TAO);
	return bFirstVoloe>bLastVoloe;*/
	return false;
}

//�Ա�ɢ��
bool CGameLogic::CompareSanCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount)
{
	if (GetCardValoe(bFirstList[0])!=GetCardValoe(bLastList[0]))  return (GetCardValoe(bFirstList[0])>GetCardValoe(bLastList[0]));	
	if (GetCardValoe(bFirstList[1])!=GetCardValoe(bLastList[1])) return (GetCardValoe(bFirstList[1])>GetCardValoe(bLastList[1]));
	if (GetCardValoe(bFirstList[2])!=GetCardValoe(bLastList[2])) return (GetCardValoe(bFirstList[2])>GetCardValoe(bLastList[2]));
	return false;
	//return CompareOnlyOne(bFirstList[0],bLastList[0]);
}

//�����˿�
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

//�����˿�
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

//�Ա��˿ˣ�ǰ�ߴ��ں��� TRUE������ FALSE
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bLastList[],BYTE bCardCount)
{
	//�����˿�
	SortCard(bFirstList,bCardCount);
	SortCard(bLastList,bCardCount);
	
	//��ȡ����
	BYTE bFirstKind=GetCardKind(bFirstList,bCardCount);
	BYTE bLastKind=GetCardKind(bLastList,bCardCount);
	
	//���Ͳ���ͬ���Ա�����
	if (bFirstKind!=bLastKind)
	{
		if((bFirstKind==TeShu) && (bLastKind==SAN_TIAO)) return true;
		if((bFirstKind==SAN_TIAO) && (bLastKind==TeShu)) return false;
		return bFirstKind>bLastKind;
	}
	
	//������ͬ
	switch(bFirstKind)
	{
		//ɢ��
	case SHAN: return CompareSanCard(bFirstList,bLastList,bCardCount);
		//����
	case DUI_ZI: return CompareDuiZhi(bFirstList,bLastList,bCardCount);
		//����
	//case LAING_DUI: return CompareLiangDui(bFirstList,bLastList,bCardCount);
		//����
	case SAN_TIAO: return CompareSanTiao(bFirstList,bLastList);
		//˳��
	case SHUN_ZI: return CompareShunZhi(bFirstList,bLastList);
		//ͬ��
	case TONG_HUA: return CompareTongHua(bFirstList,bLastList);
		//��«
	//case HU_LU: return CompareHuLu(bFirstList,bLastList);
		//��֧
	//case TIE_ZHI: return CompareTeiZhi(bFirstList,bLastList);
		//ͬ��˳
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
	VarDecFromR8(m_pGameEnd->fGameGold[wChairID], &decTemp);
	VarDecAdd(&decTemp, &pPartedWinScore[0], &pPartedWinScore[0]);

	//�жϴ���0,���г�ˮ����
	if(VarDecCmpR8(&decTemp, 0) == VARCMP_GT)
	{
		//��ˮ = ���ɲ� * (- ��ˮ��)
		VarDecMul(&decTemp,(DECIMAL*) &m_decAfterTax, &pPartedTaxScore[0]);
		//����Double�ɲ�
		VarDecAdd(&decTemp,&pPartedTaxScore[0], &decTemp);
		VarR8FromDec(&decTemp, &m_pGameEnd->fGameGold[wChairID]);
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
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	m_wBeginTurnUser=INVALID_CHAIR;
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));
	memset(m_bKanPaiFlag, 0, sizeof(m_bKanPaiFlag));

	//�����Ϣ
	m_fMaxGold=0;
	m_fTurnMaxGold=0;
	m_fTurnBasicGold=0;
	memset(m_fTableGold,0,sizeof(m_fTableGold));

	//��������
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bKanPaiFlag,0,sizeof(m_bKanPaiFlag));             //add

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
	//״̬����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	m_wBeginTurnUser=INVALID_CHAIR;

	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bKanPaiFlag,0,sizeof(m_bKanPaiFlag));          //add
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//��ұ���
	m_fMaxGold=0L;
	m_fTurnBasicGold=0L;
	memset(m_fTableGold,0,sizeof(m_fTableGold));

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
	GT_ASSERT(wChairID<m_wPlayerCount);
	return m_bPlayStatus[wChairID]?true:false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	WORD i=0;
	//״̬����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bKanPaiFlag,0,sizeof(m_bKanPaiFlag));         //add
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//��ұ���
	m_fMaxGold=0L;
	m_fTurnBasicGold=0L;
	memset(m_fTableGold,0,sizeof(m_fTableGold));
	
	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//��������
	const tagUserScore * pUserScore=NULL;
	IServerUserItem * pIServerUserItem=NULL;

	//������ע
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
	m_fTurnMaxGold = m_fMaxGold/4;

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
				GT_ASSERT(FALSE);
				return false;
			}
		}
	}
	//�ַ��˿�
	m_bSendCardCount=3; 
	m_GameLogic.RandCard(m_bTableCardArray[0],sizeof(m_bTableCardArray)/sizeof(m_bTableCardArray[0][0]));  //��������ĸ���
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			m_bTableCardCount[i]=m_bSendCardCount;
			
		}
	}
	//�Ƿ�AI��ɱ��ͨ���
	if(m_pITableFrame->IsAIKillGameUser())
	{
		//����������-�൱���ʽ�������Ƹ���ͨ���
		if(CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
		{
			DWORD wLoser = DeduceLoser();
			DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
			if(wGameUser != wLoser)
			{
				BYTE bTableCardArray[3];
				memcpy(bTableCardArray, m_bTableCardArray[wLoser], sizeof(bTableCardArray));
				memcpy(m_bTableCardArray[wLoser], m_bTableCardArray[wGameUser], sizeof(bTableCardArray));
				memcpy(m_bTableCardArray[wGameUser], bTableCardArray, sizeof(bTableCardArray));
			}
		}
	}

	//��ȡ��ǰ��� 
	if(m_wCurrentWiner == INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++) 
	    {
		    if (m_bPlayStatus[i]==TRUE)
			{
				m_wCurrentUser=i;
			    break;
			}
		}
	}
	else
	{
		for (WORD i=0;i<m_wPlayerCount-1;i++)														  
		{
			m_wCurrentUser=(m_wCurrentWiner+i+1)%m_wPlayerCount;		  					  
			if (m_bPlayStatus[m_wCurrentUser]==TRUE) break;         
		}    
	}
	m_wBeginTurnUser=m_wCurrentUser;

  	//���Ϳ�ʼ
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.fMaxGold=m_fMaxGold;
	GameStart.fTurnMaxGold=m_fTurnMaxGold;
	GameStart.fTurnBasicGold=m_fTurnBasicGold;

   	for (i=0;i<m_wPlayerCount;i++)
	{
		GameStart.bCardData[i][0]=m_bTableCardArray[i][0];
		GameStart.bCardData[i][1]=m_bTableCardArray[i][1];
		GameStart.bCardData[i][2]=m_bTableCardArray[i][2];
		m_GameLogic.SortCard(GameStart.bCardData[i],3);            
		
	}		 
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;
}
								
//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:	                                    	//�������
	case GER_NO_PLAYER:		                                   //û�����
		{
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			GameEnd.fMaxGold=m_fMaxGold;
			GameEnd.fTurnMaxGold=m_fTurnMaxGold;
			GameEnd.fTurnBasicGold=m_fTurnBasicGold;
			GameEnd.fBasicGold=m_pGameServiceOption->dwCellScore;
			CopyMemory(GameEnd.fTableGold,m_fTableGold,sizeof(m_fTableGold));

			//�����˿�
			memcpy(GameEnd.bTableCardCount, m_bTableCardCount, sizeof(m_bTableCardCount));
			memcpy(GameEnd.bTableCardArray, m_bTableCardArray, sizeof(m_bTableCardArray));

			//������ע
			DOUBLE fAllGold=0L;
			for (WORD i=0;i<CountArray(m_fTableGold);i++) fAllGold+=m_fTableGold[i];

			//ͳ����Ϣ	
			WORD wWinerUser=DeduceWiner();														                 
			m_wCurrentWiner = wWinerUser;
		
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//���ö����Ϣ
				if (i==wWinerUser)
				{
					GameEnd.fGameGold[i]=fAllGold-m_fTableGold[i*2]-m_fTableGold[i*2+1];
				}
				else 
					GameEnd.fGameGold[i]=-m_fTableGold[i*2]-m_fTableGold[i*2+1];			    
			}
			GameEnd.wWinerUser = wWinerUser;
	
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
			//Ч�����
			GT_ASSERT(pIServerUserItem!=NULL);
			GT_ASSERT(wChairID<m_wPlayerCount);

			//ǿ�˴���
			OnUserGiveUp(wChairID);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			StatusFree.fBasicGold=m_pGameServiceOption->dwCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			StatusPlay.wCurrentUser=m_wCurrentUser;

			CopyMemory(StatusPlay.bPlayStatus,m_bPlayStatus,sizeof(m_bPlayStatus));
			CopyMemory(StatusPlay.fTableGold,m_fTableGold,sizeof(m_fTableGold));

			StatusPlay.fMaxGold=m_fMaxGold;
			StatusPlay.fTurnMaxGold=m_fTurnMaxGold;
			StatusPlay.fTurnBasicGold=m_fTurnBasicGold;
			StatusPlay.fBasicGold=m_pGameServiceOption->dwCellScore;

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					GT_ASSERT(m_pITableFrame->GetServerUserItem(i)!=NULL);
					StatusPlay.bTableCardCount[i]=m_bTableCardCount[i];
					if ((i==wChiarID)&&(bSendSecret==true)) StatusPlay.bTableCardArray[i][0]=m_bTableCardArray[i][0];
					CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_bTableCardArray[i][1],(m_bTableCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//Ч����
	GT_ASSERT(FALSE);

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
	WORD wChairID = pIServerUserItem->GetChairID();
	if(wChairID == INVALID_CHAIR)
		return false;

	WORD wCurrentWiner=DeduceWiner();
	//�жϵ�ǰӮ��
	if(wCurrentWiner == wChairID)
	{
	}
	else
	{
		DWORD wGameUser = m_pITableFrame->GetAIKillGameUserChairID();
		//����������-�൱���ʷ���
		if(wGameUser == wCurrentWiner && CAIUtil::DetectionProbability(m_pGameServiceOption->cbAILevel))
		{
			//������Ϣ
			OnGameMessage(SUB_C_GIVE_UP, NULL, 0, pIServerUserItem);

			return true;
		}
		else
		{

		}
	}
	//��ȡ���
	LONG lGold;
	LONG lTurnMaxGold = m_fMaxGold / 4 - m_fTurnBasicGold;
	if(lTurnMaxGold <= 0)
	{
		lGold = 0;
	}
	else
	{
		lTurnMaxGold += 1;
		LONG lAddGold = rand() % lTurnMaxGold ;
		lAddGold = __min(lAddGold, m_fTurnMaxGold);
		if(lAddGold>10)
			lAddGold = lAddGold / 10 * 10;
		lGold = lAddGold;
	}
	lGold = m_fTurnBasicGold + lGold;

	//��������
	CMD_C_AddGold AddPoint;
	AddPoint.fGold=lGold;
	OnGameMessage(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint), pIServerUserItem);

	return true;
}
//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}
//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//���ñ���
	if (bLookonUser==false)
	{
		m_wCurrentWiner=INVALID_CHAIR;
	}

	return true;
}
//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_GOLD:	//�û���ע
		{
			//Ч������
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;
			if (m_bPlayStatus[pUserData->UserStatus.wChairID]==FALSE) return true;

			//��Ϣ����
			CMD_C_AddGold * pAddGold=(CMD_C_AddGold *)pDataBuffer;
			OnUserAddGold(pUserData->UserStatus.wChairID,pAddGold->fGold,false);	
			return true;
		}
	case SUB_C_GIVE_UP:		//�û�����
		{
			//Ч������
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_PLAY) return true;
			if (m_bPlayStatus[pUserData->UserStatus.wChairID]==FALSE) return true;

			//��Ϣ����
			OnUserGiveUp(pUserData->UserStatus.wChairID);

			return true;
		}
	case SUB_C_KAIPAI:      //�û�����                                                             //add
		{
			if(m_pITableFrame->GetGameStatus() == GS_PLAYING)
			{
				//��Ϣ����
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);
			}
			return true;
		}
	case SUB_C_KANPAI:      //�û�����
		{ 
			 OnUserKanPai(TRUE);    
			 return true;			 
		}
	case SUB_C_NOKANPAI:      //�û�����
		{ 
			OnUserKanPai(FALSE);    
			return true;			 
		}

	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//״̬�ж�
	GT_ASSERT(m_bPlayStatus[wChairID]==TRUE);
	if (m_bPlayStatus[wChairID]==FALSE) return false;

	//��������
	m_bPlayStatus[wChairID]=FALSE;

	//�жϿ�ʼ�������
	if(wChairID == m_wBeginTurnUser)
	{
		//˳ʱ�����¼�	
		WORD wNextUser=INVALID_CHAIR;
		for (WORD i=0;i<m_wPlayerCount-1;i++)														  
		{
			wNextUser=(m_wBeginTurnUser+i+1)%m_wPlayerCount;		  					  
			if (m_bPlayStatus[wNextUser]==TRUE) break;         
		}    
		m_wBeginTurnUser = wNextUser;
	}

	//���ͷ�����Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wUserChairID=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//�жϽ���
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

//��ע�¼�
bool CTableFrameSink::OnUserAddGold(WORD wChairID, DOUBLE fGold, bool bCancel)
{
	//��������
	if (bCancel==false)
	{
		//Ч������
		GT_ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//����ʣ�µĽ��
		DOUBLE fLeaveGold=m_fMaxGold-m_fTableGold[wChairID*2+1];
		fGold=__min(fLeaveGold,__max(fGold,m_fTurnBasicGold));

		//Ͷע
		tagBetScoreInfo BetScoreInfo;
		BetScoreInfo.dwBetScore = fGold - m_fTableGold[wChairID*2];
		BetScoreInfo.wBetRegionIndex = 0;

		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if(pIServerUserItem != NULL)
		{
			BetScoreInfo.pIServerUserItem = pIServerUserItem;

			if(m_pICalculateFrame->BetScore(pIServerUserItem, &BetScoreInfo, 1) == false)
			{
				GT_ASSERT(FALSE);
				return false;
			}
		}
		m_fTableGold[wChairID*2]  = fGold;
		
	}
	BYTE i = 0;

	//��������
	bool bFinishTurn=false;
	WORD wNextUser=INVALID_CHAIR;
	for (i=0;i<m_wPlayerCount-1;i++)
	{
		m_wOperaCount++;
		wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
		if (m_bPlayStatus[wNextUser]==TRUE) break;
	}

	//�ж���ע
	if (m_wOperaCount>=m_wPlayerCount)
	{
		DOUBLE fTableGold=m_fMaxGold/4;
		i = 0;
		while (i<m_wPlayerCount)
		{
			if ((m_bPlayStatus[i]==TRUE)&&(m_fTableGold[i*2]!=fTableGold)) break;
			i++;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//���ñ���
	m_wCurrentUser=wNextUser;

	//һ���ж�
	if (bFinishTurn==true)
	{
		m_fTurnBasicGold = 0;

		//��������
		CMD_S_AddGold GoldResult;
		GoldResult.wLastChairID=wChairID;
		GoldResult.wCurrentUser=m_wCurrentUser;
		GoldResult.fCurrentLessGold=m_fTurnBasicGold;
		GoldResult.fLastAddGold=m_fTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));

		//�ۼƽ��
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_fTableGold[i*2+1]+=m_fTableGold[i*2];
			m_fTableGold[i*2]=0L;
		}

		CMD_S_NewTurn NewTurn;
		memset(&NewTurn,0,sizeof(NewTurn));

		NewTurn.fMaxGold=m_fMaxGold;
		NewTurn.fBasicGold=m_pGameServiceOption->dwCellScore;
		NewTurn.fTurnMaxGold=m_fTurnMaxGold;
		NewTurn.fTurnBasicGold=m_fTurnBasicGold;
		CopyMemory(NewTurn.fTableGold,m_fTableGold,sizeof(m_fTableGold));

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_NEWTURN,&NewTurn,sizeof(NewTurn));
	}
	else
	{
		m_fTurnBasicGold=__max(m_fTableGold[wChairID*2],m_fTurnBasicGold);

		//��������
		CMD_S_AddGold AddGold;
		AddGold.wLastChairID=wChairID;
		AddGold.wCurrentUser=m_wCurrentUser;   
		AddGold.fCurrentLessGold=m_fTurnBasicGold;
		AddGold.fLastAddGold=m_fTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
	}

	//�ⶥ
	bool bAllBetMaxGold = true;
	for (WORD i=0;i<m_wPlayerCount;i++)														  
	{  					  
		if (m_bPlayStatus[i]==TRUE)
		{
			if(m_fTableGold[i*2] + m_fTableGold[i*2 + 1] < m_fMaxGold)
			{
				bAllBetMaxGold = false;
			}
		}
	}    
	if(bAllBetMaxGold)
	{
		OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);
	}
	
	return true;
}
	
//�ƶ�ʤ��
WORD CTableFrameSink::DeduceWiner(BYTE bBeginPos, BYTE bEndPos)
{
	BYTE k=0;
	//�����˿�
	BYTE bTableCardArray[m_wPlayerCount][3];													  
	CopyMemory(bTableCardArray,m_bTableCardArray,sizeof(m_bTableCardArray));
	
	BYTE bWiner=0;
	//Ѱ�����
	for (k=0;k<m_wPlayerCount;k++)
	{
		if (m_bPlayStatus[k]==TRUE){ bWiner=k;break;}
	}

	//�Ա����
	for (BYTE i=bWiner+1;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==false) continue;
		if(m_GameLogic.CompareCard(bTableCardArray[i],bTableCardArray[bWiner],3))
		{
			bWiner=i;
		}
		
	}	
	return bWiner;
}
//�ƶ�����
WORD CTableFrameSink::DeduceLoser(BYTE bBeginPos, BYTE bEndPos)
{
	BYTE k=0;
	//�����˿�
	BYTE bTableCardArray[m_wPlayerCount][3];													  
	CopyMemory(bTableCardArray,m_bTableCardArray,sizeof(m_bTableCardArray));

	BYTE bLoser=0;
	//Ѱ�����
	for (k=0;k<m_wPlayerCount;k++)
	{
		if (m_bPlayStatus[k]==TRUE){ bLoser=k;break;}
	}

	//�Ա����
	for (BYTE i=bLoser+1;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==false) continue;
		if(m_GameLogic.CompareCard(bTableCardArray[bLoser],bTableCardArray[i],3))
		{
			bLoser=i;
		}

	}	
	return bLoser;
}
bool CTableFrameSink::OnUserKanPai(BOOL bKanPai) 
{
	m_bKanPaiFlag[GAME_PLAYER]=FALSE;
	m_bKanPaiFlag[m_wCurrentUser]=bKanPai;
	for(BYTE i = 0; i < GAME_PLAYER + 1; i ++)
	{
		if(m_bKanPaiFlag[i])
		{
			m_bKanPaiFlag[GAME_PLAYER] = TRUE;
			break;
		}
	}

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_KANPAI,m_bKanPaiFlag,sizeof(m_bKanPaiFlag));
	return true;
}
