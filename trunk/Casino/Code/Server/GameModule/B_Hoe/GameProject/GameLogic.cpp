#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount)
{
	//�����ж�
	if (IsTongHuaShun(bCardData,bCardCount)) return CT_TONG_HUA_SHUN;
	if (IsTieZhi(bCardData,bCardCount)) return CT_TIE_ZHI;
	if (IsHuLu(bCardData,bCardCount)) return CT_HU_LU;
	if (IsTongHua(bCardData,bCardCount)) return CT_TONG_HUA;
	if (IsShunZhi(bCardData,bCardCount)) return CT_SHUN_ZI;
	if (IsSanTiao(bCardData,bCardCount)) return CT_THREE_TIAO;
	if (IsDouble(bCardData,bCardCount)) return CT_DOUBLE;
	if (bCardCount==1) return CT_SINGLE;

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicValue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicValue[i]=GetCardLogicValue(bCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicValue[i]<bLogicValue[i+1])||
				((bLogicValue[i]==bLogicValue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//����λ��
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicValue[i];
				bLogicValue[i]=bLogicValue[i+1];
				bLogicValue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//��������
	GT_ASSERT(bRemoveCount<=bCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[MAX_COUNT];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//�����˿�
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//�����˿�
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//��������
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//��Ч�ж�
	if ((cbColor<=0x30)&&(cbValue>=0x01)&&(cbValue<=0x0D)) return true;

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//ת����ֵ
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//�Ƿ����
bool CGameLogic::IsDouble(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=2) return false;
	
	//��������
	BYTE bNextValue=GetCardLogicValue(bCardData[1]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	return (bNextValue==bFirstValue);
}

//�Ƿ���֧
bool CGameLogic::IsTieZhi(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=5) return false;
	
	//��������
	BYTE bNoFixCount=0;
	BYTE bCenterValue=GetCardLogicValue(bCardData[1]);

	//��������
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardLogicValue(bCardData[i])!=bCenterValue)
		{
			bNoFixCount++;
			if (bNoFixCount>1) return false;
		}
	}

	return (bNoFixCount==1);
}

//�Ƿ��«
bool CGameLogic::IsHuLu(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=5) return false;

	//��������
	BYTE bMidValue=GetCardLogicValue(bCardData[2]);
	BYTE bLastValue=GetCardLogicValue(bCardData[4]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	if (GetCardLogicValue(bCardData[3])!=bLastValue) return false;
	if (GetCardLogicValue(bCardData[1])!=bFirstValue) return false;
	if ((bMidValue!=bFirstValue)&&(bMidValue!=bLastValue)) return false;
	
	return true;
}

//�Ƿ�ͬ��
bool CGameLogic::IsTongHua(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=5) return false;
	
	//��������
	BYTE bFirstColor=GetCardColor(bCardData[0]);

	//��������
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardColor(bCardData[i])!=bFirstColor) return false;
	}

	return true;
}

//�Ƿ�˳��
bool CGameLogic::IsShunZhi(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=5) return false;
	
	//��������
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	//���⴦��
	if (bFirstValue==15)
	{
		for (BYTE i=1;i<bCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(bCardData[i]);
			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//��������
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
	}

	return true;
}

//�Ƿ�����
bool CGameLogic::IsSanTiao(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=3) return false;
	
	//��������
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardLogicValue(bCardData[i])!=bFirstValue) return false;
	}

	return true;
}

//�Ƿ�ͬ��˳
bool CGameLogic::IsTongHuaShun(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=5) return false;
	
	//��������
	BYTE bFirstColor=GetCardColor(bCardData[0]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	//���⴦��
	if (bFirstValue==15)
	{
		for (BYTE i=1;i<bCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(bCardData[i]);
			if (GetCardColor(bCardData[i])!=bFirstColor) return false;
			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//��������
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardColor(bCardData[i])!=bFirstColor) return false;
		if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
	}

	return true;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstCardData, BYTE bNextCardData)
{
	//��ֵ�ж�
	BYTE cbNextValue=GetCardLogicValue(bNextCardData);
	BYTE cbFirstValue=GetCardLogicValue(bFirstCardData);
	if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);

	//��ɫ�ж�
	BYTE cbNextColor=GetCardColor(bNextCardData);
	BYTE cbFirstColor=GetCardColor(bFirstCardData);

	return (cbFirstColor>cbNextColor);
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��Ŀ�ж�
	if (bFirstCount!=bNextCount) return false;

	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//���͹���
	//GT_ASSERT(bFirstType!=CT_ERROR);
	if ((bFirstType==CT_ERROR)||(bNextType==CT_ERROR)) return false;
	
	//������
	switch(bFirstType)
	{
	case CT_SINGLE:			//����
	case CT_DOUBLE:			//����
	case CT_THREE_TIAO:		//����
		{
			if (bNextType!=bFirstType) return false;
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	}

	//�����ж�
	if (bFirstType!=bNextType) return (bFirstType>bNextType);

	//��������
	switch(bFirstType)
	{
	case CT_SHUN_ZI:		//˳��
	case CT_TONG_HUA:		//ͬ��
	case CT_TONG_HUA_SHUN:	//ͬ��˳
		{
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	case CT_HU_LU:			//��«
		{
			return CompareCard(bFirstList[2],bNextList[2]);
		}
	case CT_TIE_ZHI:		//��֧
		{
			return CompareCard(bFirstList[1],bNextList[1]);
		}
	}
	
	return false;
}

//�����ж�
BYTE CGameLogic::SearchOutCard(BYTE bHandCardData[], BYTE bHandCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT])
{
	//��������
	if (bHandCardCount<bTurnCardCount) return 0;
	//���û���˳��ƣ�����ʾ
	if(bTurnCardCount==0)
		return 0;
	if(bTurnOutType==CT_ERROR)
		return 0;
	int i=0;
	int j=0;
	int n=0;
	int s=0;
	BYTE	bWhichKindSel=0;
	BYTE	bTempSCardCount=0;				//�˿���Ŀ
	BYTE	bTempSCardData[13];				//�����˿�
	BYTE	bTempDCardCount=0;				//�˿���Ŀ
	BYTE	bTempDCardData[13];				//�����˿�
	BYTE	bTempTCardCount=0;				//�˿���Ŀ
	BYTE	bTempTCardData[13];				//�����˿�
	BYTE	bTempFCardCount=0;				//�˿���Ŀ
	BYTE	bTempFCardData[13];				//�����˿�
	BYTE	bTempGetCardCount = 0;				//�˿���Ŀ
	BYTE	bTempGetCardData[13];				//�����˿�
	BYTE	TempCard=0;

	//��ȡ�����б�
	for(i=0;i<bHandCardCount;i++)
	{	
		BYTE GetCard=GetCardLogicValue(bHandCardData[i]);
		if(TempCard!=GetCard)
		{
			bTempSCardData[bTempSCardCount++]=bHandCardData[i];
			TempCard=GetCard;
		}
	}
	//��ȡ�����б�
	TempCard=0;
	for(i=0;i<bHandCardCount-1;i++)
	{	
		BYTE GetCard1=GetCardLogicValue(bHandCardData[i]);
		BYTE GetCard2=GetCardLogicValue(bHandCardData[i+1]);
		if(TempCard!=GetCard1&&GetCard1==GetCard2&&GetCard1<16)
		{
			bTempDCardData[bTempDCardCount++]=bHandCardData[i];
			bTempDCardData[bTempDCardCount++]=bHandCardData[i+1];
			TempCard=GetCard1;
		}
	}
	//��ȡ�������б�
	TempCard=0;
	for(i=0;i<bHandCardCount-2;i++)
	{	
		BYTE GetCard1=GetCardLogicValue(bHandCardData[i]);
		BYTE GetCard2=GetCardLogicValue(bHandCardData[i+1]);
		BYTE GetCard3=GetCardLogicValue(bHandCardData[i+2]);
		if(TempCard!=GetCard1&&GetCard1==GetCard2&&GetCard1==GetCard3)
		{
			bTempTCardData[bTempTCardCount++]=bHandCardData[i];
			bTempTCardData[bTempTCardCount++]=bHandCardData[i+1];
			bTempTCardData[bTempTCardCount++]=bHandCardData[i+2];
			TempCard=GetCard1;
		}
	}
	//��ȡ�������б�
	TempCard=0;
	for(i=0;i<bHandCardCount-3;i++)
	{	
		BYTE GetCard1=GetCardLogicValue(bHandCardData[i]);
		BYTE GetCard2=GetCardLogicValue(bHandCardData[i+1]);
		BYTE GetCard3=GetCardLogicValue(bHandCardData[i+2]);
		BYTE GetCard4=GetCardLogicValue(bHandCardData[i+3]);
		if(TempCard!=GetCard1&&GetCard1==GetCard2&&GetCard1==GetCard3&&GetCard1==GetCard4)
		{
			bTempFCardData[bTempFCardCount++]=bHandCardData[i];
			bTempFCardData[bTempFCardCount++]=bHandCardData[i+1];
			bTempFCardData[bTempFCardCount++]=bHandCardData[i+2];
			bTempFCardData[bTempFCardCount++]=bHandCardData[i+3];
			TempCard=GetCard1;
		}
	}
	//���������������ж�
	i=0;
	bool bIsGetCard=false;

	switch(bTurnOutType)
	{
	case CT_SINGLE:	//��������
		{					
			for(i=bTempSCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&CompareCard(&bTempSCardData[i-bTurnCardCount],
					bTurnCardData,
					bTurnCardCount,
					bTurnCardCount))
				{
					//�ж��ǲ���������
					bool bIsHaveCard=false;
					for(j=0;j<bTempDCardCount;j++)
					{
						for(n=0;n<bTurnCardCount;n++)
						{
							if(GetCardLogicValue(bTempSCardData[i-bTurnCardCount+n])==GetCardLogicValue(bTempDCardData[j]))
								bIsHaveCard=true;
						}
					}
					//�������������������
					if(bTempGetCardCount==0||!bIsHaveCard)
					{
						memcpy(bTempGetCardData, &bTempSCardData[i-bTurnCardCount], bTurnCardCount);
						bTempGetCardCount=bTurnCardCount;
					}
					if(!bIsHaveCard)
						break;
				}
			}
		}break;
	case CT_DOUBLE://��������
		{
			for(i=bTempDCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&CompareCard(&bTempDCardData[i-bTurnCardCount],
					bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					//�ж��ǲ���������
					bool bIsHaveCard=false;
					for(j=0;j<bTempTCardCount;j++)
					{
						for(n=0;n<bTurnCardCount;n++)
						{
							if(GetCardLogicValue(bTempDCardData[i-bTurnCardCount+n])==GetCardLogicValue(bTempTCardData[j]))
								bIsHaveCard=true;
						}
					}
					if(bTempGetCardCount==0||!bIsHaveCard)
					{
						memcpy(bTempGetCardData, &bTempDCardData[i-bTurnCardCount], bTurnCardCount);
						bTempGetCardCount=bTurnCardCount;
					}
					if(!bIsHaveCard)
						break;
				}
			}
		}break;
	case CT_THREE_TIAO://��������
		{
			for(i=bTempTCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&CompareCard(&bTempTCardData[i-bTurnCardCount],
					bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					//�ж��ǲ���������
					bool bIsHaveCard=false;
					for(j=0;j<bTempFCardCount;j++)
					{
						for(n=0;n<bTurnCardCount;n++)
						{
							if(GetCardLogicValue(bTempTCardData[i-bTurnCardCount+n])==GetCardLogicValue(bTempFCardData[j]))
								bIsHaveCard=true;
						}
					}
					if(bTempGetCardCount==0||!bIsHaveCard)
					{
						memcpy(bTempGetCardData,&bTempTCardData[i-bTurnCardCount],bTurnCardCount);
						bTempGetCardCount=bTurnCardCount;
					}
					if(!bIsHaveCard&&bTempGetCardCount!=0)
						break;
				}
			}
		}break;
	case CT_SHUN_ZI://˳������
	case CT_TONG_HUA://ͬ������
	case CT_HU_LU://��«����
	case CT_TIE_ZHI://��֧����
	case CT_TONG_HUA_SHUN://ͬ��˳��
		{
			if(bTurnOutType == CT_SHUN_ZI)
			{
				//Ѱ��˳������
				BYTE AnalyseResultArray_ShunZhi[13][5];
				int an = AnalyseCardData_ShunZhi(bTempSCardData,
					bTempSCardCount,AnalyseResultArray_ShunZhi);
				if(an > 0)
				{
					for(i = an -1; i >= 0; i--)
					{
						if(CompareCard(AnalyseResultArray_ShunZhi[i],
							bTurnCardData,
							bTurnCardCount,
							bTurnCardCount))
						{
							memcpy(bTempGetCardData, AnalyseResultArray_ShunZhi[i], bTurnCardCount);

							bTempGetCardCount=bTurnCardCount;
							bIsGetCard = true;
							break;
						}
					}
				}
			}

			if(bTurnOutType == CT_SHUN_ZI ||
				bTurnOutType == CT_TONG_HUA)
			{
				//Ѱ��ͬ������
				if(bIsGetCard == false)
				{
					BYTE AnalyseResultArray_TongHua[13][5];
					int an = AnalyseCardData_TongHua(bHandCardData,
						bHandCardCount,AnalyseResultArray_TongHua);
					if(an > 0)
					{
						for(i = an -1; i >= 0; i--)
						{
							if(CompareCard(AnalyseResultArray_TongHua[i],
								bTurnCardData,
								bTurnCardCount,
								bTurnCardCount))
							{
								memcpy(bTempGetCardData, AnalyseResultArray_TongHua[i], bTurnCardCount);

								bTempGetCardCount=bTurnCardCount;
								bIsGetCard = true;
								break;
							}
						}
					}
				}
			}

			if(bTurnOutType == CT_SHUN_ZI ||
				bTurnOutType == CT_TONG_HUA ||
				bTurnOutType == CT_HU_LU)
			{
				//Ѱ�Һ�«����
				if(bIsGetCard == false)
				{
					BYTE AnalyseResultArray_HuLu[13][5];
					int an = AnalyseCardData_HuLu(bTempTCardData,
						bTempTCardCount,
						bTempDCardData,
						bTempDCardCount,
						AnalyseResultArray_HuLu);
					if(an > 0)
					{
						for(i = an -1; i >= 0; i--)
						{
							if(CompareCard(AnalyseResultArray_HuLu[i],
								bTurnCardData,
								bTurnCardCount,
								bTurnCardCount))
							{
								memcpy(bTempGetCardData, AnalyseResultArray_HuLu[i], bTurnCardCount);
								bTempGetCardCount=bTurnCardCount;
								bIsGetCard = true;
								break;
							}
						}
					}
				}
			}
			if(bTurnOutType == CT_SHUN_ZI ||
				bTurnOutType == CT_TONG_HUA ||
				bTurnOutType == CT_HU_LU ||
				bTurnOutType == CT_TIE_ZHI)
			{
				//Ѱ����֧����
				if(bIsGetCard == false)
				{
					BYTE AnalyseResultArray_TieZhi[13][5];
					int an = AnalyseCardData_TieZhi(bTempFCardData,
						bTempFCardCount,
						bTempSCardData,
						bTempSCardCount,
						AnalyseResultArray_TieZhi);
					if(an> 0)
					{
						for(i = an -1; i >= 0; i--)
						{
							if(CompareCard(AnalyseResultArray_TieZhi[i],
								bTurnCardData,
								bTurnCardCount,
								bTurnCardCount))
							{
								memcpy(bTempGetCardData, AnalyseResultArray_TieZhi[i], bTurnCardCount);

								bTempGetCardCount=bTurnCardCount;
								bIsGetCard = true;
								break;
							}
						}
					}
				}
			}

			//Ѱ��ͬ��˳��
			if(bIsGetCard == false)
			{
				BYTE AnalyseResultArray_TongHuaShun[13][5];
				int an = AnalyseCardData_TongHuaShun(bTempSCardData,
					bTempSCardCount,AnalyseResultArray_TongHuaShun);
				if(an > 0)
				{
					for(i = an -1; i >= 0; i--)
					{
						if(CompareCard(AnalyseResultArray_TongHuaShun[i],
							bTurnCardData,
							bTurnCardCount,
							bTurnCardCount))
						{
							memcpy(bTempGetCardData, AnalyseResultArray_TongHuaShun[i], bTurnCardCount);

							bTempGetCardCount=bTurnCardCount;
							bIsGetCard = true;
							break;
						}
					}
				}
			}
		}break;
	default:
		break;
	}
	BYTE GetIndex=0;
	for(j=0;j<bTempGetCardCount;j++)
	{
		cbOutCard[GetIndex++] = bTempGetCardData[j];
		
	}
	return bTempGetCardCount;
}
//����˳������
int CGameLogic::AnalyseCardData_ShunZhi(const BYTE CardData[],
BYTE CardCount,BYTE AnalyseResult[][5])
{
	int n = 0;
	for(int i = 0; i < CardCount - 5; i ++)
	{
		if(IsShunZhi((BYTE*)&CardData[i], 5))
		{
			memcpy(AnalyseResult[n++], &CardData[i], 5);
		}
	}

	return n;
}
//����ͬ������
int CGameLogic::AnalyseCardData_TongHua(const BYTE CardData[],
									BYTE CardCount,BYTE AnalyseResult[][5])
{
	int n = 0;
	for(int h = 0; h < 4; h ++)
	{
		BYTE TongHuaArray[13];
		int an = GetTongHuaCardData(h,
			CardData,
			CardCount,
			TongHuaArray);
		if(an >= 5)
		{
			int k = an;
			BYTE tmpArray[5];
			tmpArray[4] = TongHuaArray[k-1];
			tmpArray[3] = TongHuaArray[k-2];
			tmpArray[2] = TongHuaArray[k-3];
			tmpArray[1] = TongHuaArray[k-4];
			for(int l = 0; l < k - 4; l ++)
			{
				tmpArray[0] = TongHuaArray[l]; 
				if(IsTongHua(tmpArray, 5))
				{
					memcpy(AnalyseResult[n++] ,tmpArray,5);
				}
			}
		}
	}

	return n;
}
//����ͬ������
int CGameLogic::GetTongHuaCardData(BYTE HuaSe,
								   const BYTE CardData[],
								   BYTE CardCount,BYTE AnalyseResult[])
{
	int n = 0;
	for(int i = 0; i < CardCount; i ++)
	{
		if((GetCardColor(CardData[i]) >> 4) == HuaSe)
		{
			AnalyseResult[n++] = CardData[i];
		}
	}
	return n;
}
//������«����
int CGameLogic::AnalyseCardData_HuLu(const BYTE TCardData[],
int TCardCount,
const BYTE DCardData[],
int DCardCount,
		   BYTE AnalyseResult[][5])
{
	int n = 0;
	if(TCardCount < 3 || DCardCount < 2)
		return n;
	for(int i = 0; i < TCardCount -2; i+=3)
	{
		BYTE tmpArray[5];
		tmpArray[0] = TCardData[i];
		tmpArray[1] = TCardData[i+1];
		tmpArray[2] = TCardData[i+2];

		for(int k = 0; k < DCardCount - 1; k +=2)
		{
			if(GetCardLogicValue(tmpArray[0])!=GetCardLogicValue(DCardData[k]))
			{
				tmpArray[3] = DCardData[k];
				tmpArray[4] = DCardData[k+1];
				if(IsHuLu(tmpArray, 5))
				{
					memcpy(AnalyseResult[n++],tmpArray,5);
				}
			}
		}

	}

	return n;
}
//������֧����
int CGameLogic::AnalyseCardData_TieZhi(const BYTE FCardData[],
int FCardCount,
const BYTE SCardData[],
int SCardCount,
BYTE AnalyseResult[][5])
{
	int n = 0;
	if(FCardCount < 4 || SCardCount < 1)
		return n;
	for(int i = 0; i < FCardCount -3; i+=4)
	{
		BYTE tmpArray[5];
		tmpArray[0] = FCardData[i];
		tmpArray[1] = FCardData[i+1];
		tmpArray[2] = FCardData[i+2];
		tmpArray[3] = FCardData[i+3];
		for(int k = 0; k < SCardCount; k +=1)
		{
			if(GetCardLogicValue(tmpArray[0])!=GetCardLogicValue(SCardData[k]))
			{
				tmpArray[4] = SCardData[k];
				if(IsTieZhi(tmpArray, 5))
				{
					memcpy(AnalyseResult[n++],tmpArray,5);
				}
			}
		}

	}

	return n;
}
//ͬ��˳��
int CGameLogic::AnalyseCardData_TongHuaShun(const BYTE CardData[],
											int CardCount,
											BYTE AnalyseResult[][5])
{
	int n = 0;
	BYTE AnalyseResultArray_ShunZhi[13][5];
	int an = AnalyseCardData_ShunZhi(CardData,
		CardCount, AnalyseResultArray_ShunZhi);

	for(int i = 0; i < an; i ++)
	{
		if(IsTongHuaShun(AnalyseResultArray_ShunZhi[i], 5))
		{
			memcpy(AnalyseResult[n++], AnalyseResultArray_ShunZhi[i], 5);
		}
	}

	return n;
}
//�����˿�
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//��������
	BYTE bSameCount=1,bCardValueTemp=0;
	BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

	//���ý��
	memset(&AnalyseResult,0,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=1;i<bCardCount;i++)
	{
		//��ȡ�˿�
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//������
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 2:		//����
				{
					CopyMemory(&AnalyseResult.m_bDCardData[AnalyseResult.bDoubleCount*2],&bCardData[i-2+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],2);
					AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
					break;
				}
			case 3:		//����
				{
					CopyMemory(&AnalyseResult.m_bTCardData[AnalyseResult.bThreeCount*3],&bCardData[i-3+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],3);
					AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
					break;
				}
			case 4:		//����
				{
					CopyMemory(&AnalyseResult.m_bFCardData[AnalyseResult.bFourCount*4],&bCardData[i-4+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],4);
					AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount++]=bLogicValue;
					break;
				}
			}
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i-1];
				else
				{
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i-1];
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
				}
			}
			else
			{
				if(i==bCardCount-1)
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;

		}
	}

	//������Ŀ
	BYTE bOtherCount=AnalyseResult.bDoubleCount*2+AnalyseResult.bThreeCount*3+AnalyseResult.bFourCount*4;	return;
}
//////////////////////////////////////////////////////////////////////////
