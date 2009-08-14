#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//静态变量

//扑克数据
const BYTE	CGameLogic::m_bCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//获取类型
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount)
{
	//类型判断
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

//排列扑克
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//转换数值
	BYTE bLogicValue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicValue[i]=GetCardLogicValue(bCardData[i]);	

	//排序操作
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
				//交换位置
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

//混乱扑克
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//混乱准备
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//检验数据
	GT_ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	BYTE bDeleteCount=0,bTempCardData[MAX_COUNT];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//置零扑克
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

	//清理扑克
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//变量定义
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//有效判断
	if ((cbColor<=0x30)&&(cbValue>=0x01)&&(cbValue<=0x0D)) return true;

	return false;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//转换数值
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//是否对子
bool CGameLogic::IsDouble(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=2) return false;
	
	//分析处理
	BYTE bNextValue=GetCardLogicValue(bCardData[1]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	return (bNextValue==bFirstValue);
}

//是否铁支
bool CGameLogic::IsTieZhi(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=5) return false;
	
	//变量定义
	BYTE bNoFixCount=0;
	BYTE bCenterValue=GetCardLogicValue(bCardData[1]);

	//分析处理
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

//是否葫芦
bool CGameLogic::IsHuLu(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=5) return false;

	//分析处理
	BYTE bMidValue=GetCardLogicValue(bCardData[2]);
	BYTE bLastValue=GetCardLogicValue(bCardData[4]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	if (GetCardLogicValue(bCardData[3])!=bLastValue) return false;
	if (GetCardLogicValue(bCardData[1])!=bFirstValue) return false;
	if ((bMidValue!=bFirstValue)&&(bMidValue!=bLastValue)) return false;
	
	return true;
}

//是否同花
bool CGameLogic::IsTongHua(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=5) return false;
	
	//变量定义
	BYTE bFirstColor=GetCardColor(bCardData[0]);

	//分析处理
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardColor(bCardData[i])!=bFirstColor) return false;
	}

	return true;
}

//是否顺子
bool CGameLogic::IsShunZhi(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=5) return false;
	
	//变量定义
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	//特殊处理
	if (bFirstValue==15)
	{
		for (BYTE i=1;i<bCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(bCardData[i]);
			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//分析处理
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
	}

	return true;
}

//是否三条
bool CGameLogic::IsSanTiao(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=3) return false;
	
	//分析处理
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardLogicValue(bCardData[i])!=bFirstValue) return false;
	}

	return true;
}

//是否同花顺
bool CGameLogic::IsTongHuaShun(BYTE bCardData[], BYTE bCardCount)
{
	//数目判断
	if (bCardCount!=5) return false;
	
	//变量定义
	BYTE bFirstColor=GetCardColor(bCardData[0]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	//特殊处理
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

	//分析处理
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardColor(bCardData[i])!=bFirstColor) return false;
		if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
	}

	return true;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE bFirstCardData, BYTE bNextCardData)
{
	//数值判断
	BYTE cbNextValue=GetCardLogicValue(bNextCardData);
	BYTE cbFirstValue=GetCardLogicValue(bFirstCardData);
	if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);

	//花色判断
	BYTE cbNextColor=GetCardColor(bNextCardData);
	BYTE cbFirstColor=GetCardColor(bFirstCardData);

	return (cbFirstColor>cbNextColor);
}

//对比扑克
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//数目判断
	if (bFirstCount!=bNextCount) return false;

	//获取类型
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//类型过滤
	//GT_ASSERT(bFirstType!=CT_ERROR);
	if ((bFirstType==CT_ERROR)||(bNextType==CT_ERROR)) return false;
	
	//简单类型
	switch(bFirstType)
	{
	case CT_SINGLE:			//单牌
	case CT_DOUBLE:			//对子
	case CT_THREE_TIAO:		//三条
		{
			if (bNextType!=bFirstType) return false;
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	}

	//类型判断
	if (bFirstType!=bNextType) return (bFirstType>bNextType);

	//五条类型
	switch(bFirstType)
	{
	case CT_SHUN_ZI:		//顺子
	case CT_TONG_HUA:		//同花
	case CT_TONG_HUA_SHUN:	//同花顺
		{
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	case CT_HU_LU:			//葫芦
		{
			return CompareCard(bFirstList[2],bNextList[2]);
		}
	case CT_TIE_ZHI:		//铁支
		{
			return CompareCard(bFirstList[1],bNextList[1]);
		}
	}
	
	return false;
}

//出牌判断
BYTE CGameLogic::SearchOutCard(BYTE bHandCardData[], BYTE bHandCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT])
{
	//分析过滤
	if (bHandCardCount<bTurnCardCount) return 0;
	//如果没有人出牌，不提示
	if(bTurnCardCount==0)
		return 0;
	if(bTurnOutType==CT_ERROR)
		return 0;
	int i=0;
	int j=0;
	int n=0;
	int s=0;
	BYTE	bWhichKindSel=0;
	BYTE	bTempSCardCount=0;				//扑克数目
	BYTE	bTempSCardData[13];				//手上扑克
	BYTE	bTempDCardCount=0;				//扑克数目
	BYTE	bTempDCardData[13];				//手上扑克
	BYTE	bTempTCardCount=0;				//扑克数目
	BYTE	bTempTCardData[13];				//手上扑克
	BYTE	bTempFCardCount=0;				//扑克数目
	BYTE	bTempFCardData[13];				//手上扑克
	BYTE	bTempGetCardCount = 0;				//扑克数目
	BYTE	bTempGetCardData[13];				//手上扑克
	BYTE	TempCard=0;

	//获取单牌列表
	for(i=0;i<bHandCardCount;i++)
	{	
		BYTE GetCard=GetCardLogicValue(bHandCardData[i]);
		if(TempCard!=GetCard)
		{
			bTempSCardData[bTempSCardCount++]=bHandCardData[i];
			TempCard=GetCard;
		}
	}
	//获取对牌列表
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
	//获取三张牌列表
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
	//获取四张牌列表
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
	//根据所出牌类型判断
	i=0;
	bool bIsGetCard=false;

	switch(bTurnOutType)
	{
	case CT_SINGLE:	//单牌类型
		{					
			for(i=bTempSCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&CompareCard(&bTempSCardData[i-bTurnCardCount],
					bTurnCardData,
					bTurnCardCount,
					bTurnCardCount))
				{
					//判断是不是最合理的
					bool bIsHaveCard=false;
					for(j=0;j<bTempDCardCount;j++)
					{
						for(n=0;n<bTurnCardCount;n++)
						{
							if(GetCardLogicValue(bTempSCardData[i-bTurnCardCount+n])==GetCardLogicValue(bTempDCardData[j]))
								bIsHaveCard=true;
						}
					}
					//把最合理的情况保存起来
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
	case CT_DOUBLE://对子类型
		{
			for(i=bTempDCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&CompareCard(&bTempDCardData[i-bTurnCardCount],
					bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					//判断是不是最合理的
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
	case CT_THREE_TIAO://三条类型
		{
			for(i=bTempTCardCount;i>0;i--)
			{
				if(i-bTurnCardCount>=0&&CompareCard(&bTempTCardData[i-bTurnCardCount],
					bTurnCardData,bTurnCardCount,bTurnCardCount))
				{
					//判断是不是最合理的
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
	case CT_SHUN_ZI://顺子类型
	case CT_TONG_HUA://同花类型
	case CT_HU_LU://葫芦类型
	case CT_TIE_ZHI://铁支类型
	case CT_TONG_HUA_SHUN://同花顺型
		{
			if(bTurnOutType == CT_SHUN_ZI)
			{
				//寻找顺子类型
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
				//寻找同花类型
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
				//寻找葫芦类型
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
				//寻找铁支类型
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

			//寻找同花顺型
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
//分析顺子类型
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
//分析同花类型
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
//分析同花类型
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
//分析葫芦类型
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
//分析铁支类型
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
//同花顺型
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
//分析扑克
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//变量定义
	BYTE bSameCount=1,bCardValueTemp=0;
	BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

	//设置结果
	memset(&AnalyseResult,0,sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i=1;i<bCardCount;i++)
	{
		//获取扑克
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//保存结果
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 2:		//两张
				{
					CopyMemory(&AnalyseResult.m_bDCardData[AnalyseResult.bDoubleCount*2],&bCardData[i-2+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],2);
					AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
					break;
				}
			case 3:		//三张
				{
					CopyMemory(&AnalyseResult.m_bTCardData[AnalyseResult.bThreeCount*3],&bCardData[i-3+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],3);
					AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
					break;
				}
			case 4:		//四张
				{
					CopyMemory(&AnalyseResult.m_bFCardData[AnalyseResult.bFourCount*4],&bCardData[i-4+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],4);
					AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount++]=bLogicValue;
					break;
				}
			}
		}

		//设置变量
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

	//单牌数目
	BYTE bOtherCount=AnalyseResult.bDoubleCount*2+AnalyseResult.bThreeCount*3+AnalyseResult.bFourCount*4;	return;
}
//////////////////////////////////////////////////////////////////////////
