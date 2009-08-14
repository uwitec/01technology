package org.god.GameModule.B_Hoe.Common
{
	public class GameLogic
	{
		public static  var m_bCardListData:Array;//扑克数据
		
		public function GameLogic()
		{
			if(m_bCardListData == null)
			{
				m_bCardListData = new Array(
				0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
				0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//黑桃 A - K
				);
			}
		}
		
			//获取数值
		public function GetCardValue( cbCardData:uint):uint { return cbCardData&LOGIC_MASK_VALUE; }
		//获取花色
		public function GetCardColor( cbCardData:uint):uint { return cbCardData&LOGIC_MASK_COLOR; }

		//获取类型
		public function GetCardType(cbCardData:Array, cbCardCount:uint):uint
		{
			//类型判断
			if (IsTongHuaShun(cbCardData,cbCardCount)) return CT_TONG_HUA_SHUN;
			if (IsTieZhi(cbCardData,cbCardCount)) return CT_TIE_ZHI;
			if (IsHuLu(cbCardData,cbCardCount)) return CT_HU_LU;
			if (IsTongHua(cbCardData,cbCardCount)) return CT_TONG_HUA;
			if (IsShunZhi(cbCardData,cbCardCount)) return CT_SHUN_ZI;
			if (IsSanTiao(cbCardData,cbCardCount)) return CT_THREE_TIAO;
			if (IsDouble(cbCardData,cbCardCount)) return CT_DOUBLE;
			if (cbCardCount==1) return CT_SINGLE;
		
			return CT_ERROR;
		}
		
		//排列扑克
		public function SortCardList(cbCardData:Array, cbCardCount:uint):void
		{
			//转换数值
			var cbLogicValue:Array = new Array(MAX_COUNT);
			for (var i:uint=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	
		
			//排序操作
			var bSorted:Boolean=true;
			var cbTempData:uint;
			var bLast:uint=cbCardCount-1;
			do
			{
				bSorted=true;
				for (var i:uint=0;i<bLast;i++)
				{
					if ((cbLogicValue[i]<cbLogicValue[i+1])||
						((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
					{
						//交换位置
						cbTempData=cbCardData[i];
						cbCardData[i]=cbCardData[i+1];
						cbCardData[i+1]=cbTempData;
						cbTempData=cbLogicValue[i];
						cbLogicValue[i]=cbLogicValue[i+1];
						cbLogicValue[i+1]=cbTempData;
						bSorted=false;
					}	
				}
				bLast--;
			} while(bSorted==false);
		
			return;
		}
		
		//混乱扑克
		public function RandCardList(cbCardBuffer:Array,  cbBufferCount:uint):void
		{
			//混乱准备
			var cbCardData:Array = new Array(m_bCardListData.length);
			CopyArray(cbCardData,m_bCardListData,m_bCardListData.length);
		
			//混乱扑克
			var bRandCount:uint=0;
			var bPosition:uint=0;
			do
			{
				bPosition=randUInt()%(cbBufferCount-bRandCount);
				cbCardBuffer[bRandCount++]=cbCardData[bPosition];
				cbCardData[bPosition]=cbCardData[cbBufferCount-bRandCount];
			} while (bRandCount<cbBufferCount);
		
			return;
		}
		
		//删除扑克
		public function RemoveCard(bRemoveCard:Array,  bRemoveCount:uint, cbCardData:Array, cbCardCount:uint):Boolean
		{
			//检验数据
			ASSERT(bRemoveCount<=cbCardCount);
		
			//定义变量
			var bDeleteCount:uint=0;
			var bTempCardData:Array = new Array(MAX_COUNT);
			if (cbCardCount>CountArray(bTempCardData)) return false;
			CopyArray(bTempCardData,cbCardData,cbCardCount);
		
			//置零扑克
			for (var i:uint=0;i<bRemoveCount;i++)
			{
				for (var j:uint=0;j<cbCardCount;j++)
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
			var bCardPos:uint=0;
			for (var i:uint=0;i<cbCardCount;i++)
			{
				if (bTempCardData[i]!=0) cbCardData[bCardPos++]=bTempCardData[i];
			}
		
			return true;
		}
		
		//有效判断
		public function IsValidCard( cbCardData:uint):Boolean
		{
			//变量定义
			var cbColor:uint=GetCardColor(cbCardData);
			var cbValue:uint=GetCardValue(cbCardData);
		
			//有效判断
			if ((cbColor<=0x30)&&(cbValue>=0x01)&&(cbValue<=0x0D)) return true;
		
			return false;
		}
		
		//逻辑数值
		 public function GetCardLogicValue( cbCardData:uint):uint
		{
			//扑克属性
			var bCardColor:uint=GetCardColor(cbCardData);
			var bCardValue:uint=GetCardValue(cbCardData);
		
			//转换数值
			return (bCardValue<=2)?(bCardValue+13):bCardValue;
		}
		
		//是否对子
		 public function IsDouble(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=2) return false;
			
			//分析处理
			var bNextValue:uint=GetCardLogicValue(cbCardData[1]);
			var bFirstValue:uint=GetCardLogicValue(cbCardData[0]);
		
			return (bNextValue==bFirstValue);
		}
		
		//是否铁支
		 public function IsTieZhi(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=5) return false;
			
			//变量定义
			var bNoFixCount:uint=0;
			var bCenterValue:uint=GetCardLogicValue(cbCardData[1]);
		
			//分析处理
			for (var i:uint=0;i<cbCardCount;i++)
			{
				if (GetCardLogicValue(cbCardData[i])!=bCenterValue)
				{
					bNoFixCount++;
					if (bNoFixCount>1) return false;
				}
			}
		
			return (bNoFixCount==1);
		}
		
		//是否葫芦
		 public function IsHuLu(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=5) return false;
		
			//分析处理
			var bMidValue:uint=GetCardLogicValue(cbCardData[2]);
			var bLastValue:uint=GetCardLogicValue(cbCardData[4]);
			var bFirstValue:uint=GetCardLogicValue(cbCardData[0]);
			if (GetCardLogicValue(cbCardData[3])!=bLastValue) return false;
			if (GetCardLogicValue(cbCardData[1])!=bFirstValue) return false;
			if ((bMidValue!=bFirstValue)&&(bMidValue!=bLastValue)) return false;
			
			return true;
		}
		
		//是否同花
		 public function IsTongHua(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=5) return false;
			
			//变量定义
			var bFirstColor:uint=GetCardColor(cbCardData[0]);
		
			//分析处理
			for (var i:uint=1;i<cbCardCount;i++)
			{
				if (GetCardColor(cbCardData[i])!=bFirstColor) return false;
			}
		
			return true;
		}
		
		//是否顺子
		 public function IsShunZhi(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=5) return false;
			
			//变量定义
			var bFirstValue:uint=GetCardLogicValue(cbCardData[0]);
		
			//特殊处理
			if (bFirstValue==15)
			{
				for (var i:uint=1;i<cbCardCount;i++)
				{
					var cbLogicValue:uint=GetCardLogicValue(cbCardData[i]);
					if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
				}
		
				return true;
			}
		
			//分析处理
			for (var i:uint=1;i<cbCardCount;i++)
			{
				if (bFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) return false;
			}
		
			return true;
		}
		
		//是否三条
		 public function IsSanTiao(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=3) return false;
			
			//分析处理
			var bFirstValue:uint=GetCardLogicValue(cbCardData[0]);
			for (var i:uint=1;i<cbCardCount;i++)
			{
				if (GetCardLogicValue(cbCardData[i])!=bFirstValue) return false;
			}
		
			return true;
		}
		
		//是否同花顺
		 public function IsTongHuaShun(cbCardData:Array, cbCardCount:uint):Boolean
		{
			//数目判断
			if (cbCardCount!=5) return false;
			
			//变量定义
			var bFirstColor:uint=GetCardColor(cbCardData[0]);
			var bFirstValue:uint=GetCardLogicValue(cbCardData[0]);
		
			//特殊处理
			if (bFirstValue==15)
			{
				for (var i:uint=1;i<cbCardCount;i++)
				{
					var cbLogicValue:uint=GetCardLogicValue(cbCardData[i]);
					if (GetCardColor(cbCardData[i])!=bFirstColor) return false;
					if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
				}
		
				return true;
			}
		
			//分析处理
			for (var i:uint=1;i<cbCardCount;i++)
			{
				if (GetCardColor(cbCardData[i])!=bFirstColor) return false;
				if (bFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) return false;
			}
		
			return true;
		}
		
		//对比扑克
		 public function CompareCard2( cbFirstCardData:uint, 
									  cbNextCardData:uint):Boolean
		{
			//数值判断
			var cbNextValue:uint=GetCardLogicValue(cbNextCardData);
			var cbFirstValue:uint=GetCardLogicValue(cbFirstCardData);
			if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);
		
			//花色判断
			var cbNextColor:uint=GetCardColor(cbNextCardData);
			var cbFirstColor:uint=GetCardColor(cbFirstCardData);
		
			return (cbFirstColor>cbNextColor);
		}
		
		//对比扑克
		 public function CompareCard4( bFirstList:Array,  
									  bNextList:Array, 
									  cbFirstCount:uint,  
									  cbNextCount:uint):Boolean
		{
			//数目判断
			if (cbFirstCount!=cbNextCount) return false;
		
			//获取类型
			var bNextType:uint=GetCardType(bNextList,cbNextCount);
			var bFirstType:uint=GetCardType(bFirstList,cbFirstCount);
		
			//类型过滤
			ASSERT(bFirstType!=CT_ERROR);
			if ((bFirstType==CT_ERROR)||(bNextType==CT_ERROR)) return false;
			
			//简单类型
			switch(bFirstType)
			{
			case CT_SINGLE:			//单牌
			case CT_DOUBLE:			//对子
			case CT_THREE_TIAO:		//三条
				{
					if (bNextType!=bFirstType) return false;
					return CompareCard2(bFirstList[0],bNextList[0]);
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
					return CompareCard2(bFirstList[0],bNextList[0]);
				}
			case CT_HU_LU:			//葫芦
				{
					return CompareCard2(bFirstList[2],bNextList[2]);
				}
			case CT_TIE_ZHI:		//铁支
				{
					return CompareCard2(bFirstList[1],bNextList[1]);
				}
			}
			
			return false;
		}
		
		//出牌判断
		 public function SearchOutCard(bHandCardData:Array,
		   bHandCardCount:uint, 
		   bTurnCardData:Array, 
		   bTurnCardCount:uint, 
		   bTurnOutType:uint, 
		   bTempGetCardIndex:Array):uint
		{
			//分析过滤
			if (bHandCardCount<bTurnCardCount) return 0;
			//如果没有人出牌，不提示
			if(bTurnCardCount==0)
				return 0;
			if(bTurnOutType==CT_ERROR)
				return 0;
			var i:int=0;
			var j:int=0;
			var n:int=0;
			var s:int=0;
			var bWhichKindSel:uint=0;
			var	bTempSCardCount:uint=0;				//扑克数目
			var	bTempSCardData:Array = new Array(13);				//手上扑克
			var bTempDCardCount=0;				//扑克数目
			var	bTempDCardData:Array = new Array(13);				//手上扑克
			var	bTempTCardCount:uint=0;				//扑克数目
			var	bTempTCardData:Array = new Array(13);				//手上扑克
			var	bTempFCardCount:uint=0;				//扑克数目
			var	bTempFCardData:Array = new Array(13);				//手上扑克
			var	bTempGetCardCount:uint=0;				//扑克数目
			var	bTempGetCardData:Array = new Array(13);				//手上扑克
			var TempCard:uint=0;
			
			//获取单牌列表
			for(i=0;i<bHandCardCount;i++)
			{	
				var GetCard:uint=GetCardLogicValue(bHandCardData[i]);
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
				var GetCard1:uint=GetCardLogicValue(bHandCardData[i]);
				var GetCard2:uint=GetCardLogicValue(bHandCardData[i+1]);
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
				var GetCard1:uint=GetCardLogicValue(bHandCardData[i]);
				var GetCard2:uint=GetCardLogicValue(bHandCardData[i+1]);
				var GetCard3:uint=GetCardLogicValue(bHandCardData[i+2]);
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
				var GetCard1:uint=GetCardLogicValue(bHandCardData[i]);
				var GetCard2:uint=GetCardLogicValue(bHandCardData[i+1]);
				var GetCard3:uint=GetCardLogicValue(bHandCardData[i+2]);
				var GetCard4:uint=GetCardLogicValue(bHandCardData[i+3]);
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
			var bIsGetCard:Boolean=false;
			
			switch(bTurnOutType)
			{
				case CT_SINGLE:	//单牌类型
				{					
					for(i=bTempSCardCount;i>0;i--)
					{
						if(i-bTurnCardCount>=0&&CompareCard4(CloneArray(bTempSCardData,i-bTurnCardCount),
																	bTurnCardData,
																	bTurnCardCount,
																	bTurnCardCount))
						{
							//判断是不是最合理的
							var bIsHaveCard:Boolean=false;
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
								CopyArray(bTempGetCardData,CloneArray(bTempSCardData,i-bTurnCardCount),bTurnCardCount);
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
						if(i-bTurnCardCount>=0&&CompareCard4(CloneArray(bTempDCardData,i-bTurnCardCount),
														bTurnCardData,bTurnCardCount,bTurnCardCount))
						{
							//判断是不是最合理的
							var bIsHaveCard:Boolean=false;
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
								CopyArray(bTempGetCardData,CloneArray(bTempDCardData,i-bTurnCardCount),bTurnCardCount);
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
						if(i-bTurnCardCount>=0&&CompareCard4(CloneArray(bTempTCardData,i-bTurnCardCount),
														bTurnCardData,bTurnCardCount,bTurnCardCount))
						{
							//判断是不是最合理的
							var bIsHaveCard:Boolean=false;
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
								CopyArray(bTempGetCardData,CloneArray(bTempTCardData,i-bTurnCardCount),bTurnCardCount);
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
						var AnalyseResultArray_ShunZhi:Array;
						AnalyseResultArray_ShunZhi = AnalyseCardData_ShunZhi(bTempSCardData,
															   bTempSCardCount);
						if(AnalyseResultArray_ShunZhi.length > 0)
						{
							for(i = AnalyseResultArray_ShunZhi.length -1; i >= 0; i--)
							{
								if(CompareCard4(AnalyseResultArray_ShunZhi[i],
															bTurnCardData,
															bTurnCardCount,
															bTurnCardCount))
								{
									CopyArray(bTempGetCardData,
										  AnalyseResultArray_ShunZhi[i],
										  bTurnCardCount);
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
							var AnalyseResultArray_TongHua:Array;
							AnalyseResultArray_TongHua = AnalyseCardData_TongHua(bHandCardData,
															   bHandCardCount);
							if(AnalyseResultArray_TongHua.length > 0)
							{
								for(i = AnalyseResultArray_TongHua.length -1; i >= 0; i--)
								{
									if(CompareCard4(AnalyseResultArray_TongHua[i],
															bTurnCardData,
															bTurnCardCount,
															bTurnCardCount))
									{
										CopyArray(bTempGetCardData,
										  AnalyseResultArray_TongHua[i],
										  bTurnCardCount);
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
							var AnalyseResultArray_HuLu:Array;
							AnalyseResultArray_HuLu = AnalyseCardData_HuLu(bTempTCardData,
															   bTempTCardCount,
															   bTempDCardData,
															   bTempDCardCount);
							if(AnalyseResultArray_HuLu.length > 0)
							{
								for(i = AnalyseResultArray_HuLu.length -1; i >= 0; i--)
								{
									if(CompareCard4(AnalyseResultArray_HuLu[i],
															bTurnCardData,
															bTurnCardCount,
															bTurnCardCount))
									{
										CopyArray(bTempGetCardData,
										  AnalyseResultArray_HuLu[i],
										  bTurnCardCount);
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
							var AnalyseResultArray_TieZhi:Array;
							AnalyseResultArray_TieZhi = AnalyseCardData_TieZhi(bTempFCardData,
															   bTempFCardCount,
															   bTempSCardData,
															   bTempSCardCount);
							if(AnalyseResultArray_TieZhi.length > 0)
							{
								for(i = AnalyseResultArray_TieZhi.length -1; i >= 0; i--)
								{
									if(CompareCard4(AnalyseResultArray_TieZhi[i],
															bTurnCardData,
															bTurnCardCount,
															bTurnCardCount))
									{
										CopyArray(bTempGetCardData,
										  AnalyseResultArray_TieZhi[i],
										  bTurnCardCount);
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
						var AnalyseResultArray_TongHuaShun:Array;
						AnalyseResultArray_TongHuaShun = AnalyseCardData_TongHuaShun(bTempSCardData,
															   bTempSCardCount);
						if(AnalyseResultArray_TongHuaShun.length > 0)
						{
							for(i = AnalyseResultArray_TongHuaShun.length -1; i >= 0; i--)
							{
								if(CompareCard4(AnalyseResultArray_TongHuaShun[i],
														bTurnCardData,
														bTurnCardCount,
														bTurnCardCount))
								{
									CopyArray(bTempGetCardData,
									  AnalyseResultArray_TongHuaShun[i],
									  bTurnCardCount);
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
			var GetIndex:uint=0;
			for(j=0;j<bTempGetCardCount;j++)
			{
				for(i=0;i<bHandCardCount;i++)
				{
					if(bHandCardData[i]==bTempGetCardData[j])
					{
						bTempGetCardIndex[GetIndex++]=i;
					}
				}
			}
			return bTempGetCardCount;
		}
		private function AnalyseCardData_ShunZhi(CardData:Array,
												CardCount:uint):Array
		{
			var AnalyseResult:Array = new Array;
			var n:uint = 0;
			for(var i:uint = 0; i < CardCount - 5; i ++)
			{
				var tmpArray:Array = CloneArray(CardData,i);
				if(IsShunZhi(tmpArray, 5))
				{
					AnalyseResult[n++] = tmpArray;
				}
			}
			
			return AnalyseResult;
		}
		private function AnalyseCardData_TongHua(CardData:Array,
												CardCount:uint):Array
		{
			var AnalyseResult:Array = new Array;
			var n:uint = 0;
			for(var h:uint = 0; h < 4; h ++)
			{
				var TongHuaArray:Array = GetTongHuaCardData(h,
															CardData,
															CardCount);
				if(TongHuaArray.length >= 5)
				{
					var k:uint = TongHuaArray.length;
					var tmpArray:Array = new Array(5);
					tmpArray[4] = TongHuaArray[k-1];
					tmpArray[3] = TongHuaArray[k-2];
					tmpArray[2] = TongHuaArray[k-3];
					tmpArray[1] = TongHuaArray[k-4];
					for(var l:uint = 0; l < k - 4; l ++)
					{
						tmpArray[0] = TongHuaArray[l]; 
						if(IsTongHua(tmpArray, 5))
						{
							AnalyseResult[n++] = tmpArray;
						}
					}
				}
			}
			
			return AnalyseResult;
		}
		private function GetTongHuaCardData(HuaSe:uint,
												CardData:Array,
												CardCount:uint):Array
		{
			var AnalyseResult:Array = new Array;
			var n:uint = 0;
			for(var i:uint = 0; i < CardCount; i ++)
			{
				if((GetCardColor(CardData[i]) >> 4) == HuaSe)
				{
					AnalyseResult[n++] = CardData[i];
				}
			}
			return AnalyseResult;
		}
		private function AnalyseCardData_HuLu(TCardData:Array,
												TCardCount:uint,
												DCardData:Array,
												DCardCount:uint):Array
		{
			var AnalyseResult:Array = new Array;
			if(TCardCount < 3 || DCardCount < 2)
				return AnalyseResult;
			var n:uint = 0;
			for(var i:uint = 0; i < TCardCount -2; i+=3)
			{
				var tmpArray:Array = new Array(5);
				tmpArray[0] = TCardData[i];
				tmpArray[1] = TCardData[i+1];
				tmpArray[2] = TCardData[i+2];
					
				for(var k:uint = 0; k < DCardCount - 1; k +=2)
				{
					if(GetCardLogicValue(tmpArray[0])!=GetCardLogicValue(DCardData[k]))
					{
						tmpArray[3] = DCardData[k];
						tmpArray[4] = DCardData[k+1];
						if(IsHuLu(tmpArray, 5))
						{
							AnalyseResult[n++] = tmpArray;
						}
					}
				}
				
			}
			
			return AnalyseResult;
		}
		private function AnalyseCardData_TieZhi(FCardData:Array,
												FCardCount:uint,
												SCardData:Array,
												SCardCount:uint):Array
		{
			var AnalyseResult:Array = new Array;
			if(FCardCount < 4 || SCardCount < 1)
				return AnalyseResult;
			var n:uint = 0;
			for(var i:uint = 0; i < FCardCount -3; i+=4)
			{
				var tmpArray:Array = new Array(5);
				tmpArray[0] = FCardData[i];
				tmpArray[1] = FCardData[i+1];
				tmpArray[2] = FCardData[i+2];
				tmpArray[3] = FCardData[i+3];
				for(var k:uint = 0; k < SCardCount; k +=1)
				{
					if(GetCardLogicValue(tmpArray[0])!=GetCardLogicValue(SCardData[k]))
					{
						tmpArray[4] = SCardData[k];
						if(IsTieZhi(tmpArray, 5))
						{
							AnalyseResult[n++] = tmpArray;
						}
					}
				}
				
			}
			
			return AnalyseResult;
		}
		private function AnalyseCardData_TongHuaShun(CardData:Array,
												CardCount:uint):Array
		{
			var AnalyseResult:Array = new Array;
			var n:uint = 0;
			var AnalyseResultArray_ShunZhi:Array;
			AnalyseResultArray_ShunZhi = AnalyseCardData_ShunZhi(CardData,
															   CardCount);
						
			for(var i:uint = 0; i < AnalyseResultArray_ShunZhi.length; i ++)
			{
				var tmpArray:Array = CloneArray(AnalyseResultArray_ShunZhi[i],0);
				if(IsTongHuaShun(tmpArray, 5))
				{
					AnalyseResult[n++] = tmpArray;
				}
			}
			
			return AnalyseResult;
		}
		public function AnalysebCardData(bCardData:Array,
										 bCardCount:uint,
										 AnalyseResult:tagAnalyseResult):void
		{
			//变量定义
			var bSameCount:uint=1;
			var bCardValueTemp:uint=0;
			var bLogicValue:uint=GetCardLogicValue(bCardData[0]);

			//设置结果
			AnalyseResult.init();
			
			//扑克分析
			for (var i:uint=1; i < bCardCount; i++)
			{
				//获取扑克
				bCardValueTemp=GetCardLogicValue(bCardData[i]);
				if (bCardValueTemp == bLogicValue)
				{
					bSameCount++;
				}

				//保存结果
				if (bCardValueTemp != bLogicValue || i == bCardCount - 1)
				{
					switch (bSameCount)
					{
						case 2 ://两张
							{
								var dst_offset:uint = AnalyseResult.bDoubleCount * 2;
								var src_offset:uint = i-2+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0);
								CopyArray(AnalyseResult.bDCardData,
										   bCardData,2,dst_offset,src_offset);
								AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
								break;

							};
						case 3 ://三张
							{
								var dst_offset:uint = AnalyseResult.bThreeCount * 3;
								var src_offset:uint = i-3+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0);
						
								CopyArray(AnalyseResult.bTCardData,
										   bCardData,3,dst_offset,src_offset);
								AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
								break;

							};
						case 4 ://四张
							{
								var dst_offset:uint = AnalyseResult.bFourCount * 4;
								var src_offset:uint =i-4+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0);
								
								CopyArray(AnalyseResult.bFCardData,
									bCardData,4,dst_offset,src_offset);
								AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount++]=bLogicValue;
								break;

						}
					}
				};

				//设置变量
				if (bCardValueTemp != bLogicValue)
				{
					if (bSameCount == 1)
					{
						if (i != bCardCount - 1)
						{
							AnalyseResult.bSCardData[AnalyseResult.bSignedCount++]=bCardData[i - 1];
						}
						else
						{
							AnalyseResult.bSCardData[AnalyseResult.bSignedCount++]=bCardData[i - 1];
							AnalyseResult.bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
						}
					}
					else
					{
						if (i == bCardCount - 1)
						{
							AnalyseResult.bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
						}
					}
					bSameCount=1;
					bLogicValue=bCardValueTemp;

				}
			}

			//单牌数目
			var bOtherCount:uint=AnalyseResult.bDoubleCount * 2 + AnalyseResult.bThreeCount * 3 + AnalyseResult.bFourCount * 4;
			return;
		}
		//////////////////////////////////////////////////////////////////////////

	}
}
include "./CMD_Hoe.as"
include "./GameLogicDef.as"
include "../../../Common/Memory.as"
include "../../../Common/MathFunction.as"