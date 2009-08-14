package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;

	public class GameLogic
	{
		//扑克数据
		public static  var m_bCardListData:ByteArray;
		public static  var _nCardMaxCount:int=54;

		public function GameLogic()
		{
			if (m_bCardListData == null)
			{
				m_bCardListData=newLitteEndianByteArray();
				m_bCardListData.writeByte(0x01);
				m_bCardListData.writeByte(0x02);
				m_bCardListData.writeByte(0x03);
				m_bCardListData.writeByte(0x04);
				m_bCardListData.writeByte(0x05);
				m_bCardListData.writeByte(0x06);
				m_bCardListData.writeByte(0x07);
				m_bCardListData.writeByte(0x08);
				m_bCardListData.writeByte(0x09);
				m_bCardListData.writeByte(0x0A);
				m_bCardListData.writeByte(0x0B);
				m_bCardListData.writeByte(0x0C);
				m_bCardListData.writeByte(0x0D);//方块 A - K

				m_bCardListData.writeByte(0x11);
				m_bCardListData.writeByte(0x12);
				m_bCardListData.writeByte(0x13);
				m_bCardListData.writeByte(0x14);
				m_bCardListData.writeByte(0x15);
				m_bCardListData.writeByte(0x16);
				m_bCardListData.writeByte(0x17);
				m_bCardListData.writeByte(0x18);
				m_bCardListData.writeByte(0x19);
				m_bCardListData.writeByte(0x1A);
				m_bCardListData.writeByte(0x1B);
				m_bCardListData.writeByte(0x1C);
				m_bCardListData.writeByte(0x1D);//梅花 A - K

				m_bCardListData.writeByte(0x21);
				m_bCardListData.writeByte(0x22);
				m_bCardListData.writeByte(0x23);
				m_bCardListData.writeByte(0x24);
				m_bCardListData.writeByte(0x25);
				m_bCardListData.writeByte(0x26);
				m_bCardListData.writeByte(0x27);
				m_bCardListData.writeByte(0x28);
				m_bCardListData.writeByte(0x29);
				m_bCardListData.writeByte(0x2A);
				m_bCardListData.writeByte(0x2B);
				m_bCardListData.writeByte(0x2C);
				m_bCardListData.writeByte(0x2D);//红桃 A - K

				m_bCardListData.writeByte(0x31);
				m_bCardListData.writeByte(0x32);
				m_bCardListData.writeByte(0x33);
				m_bCardListData.writeByte(0x34);
				m_bCardListData.writeByte(0x35);
				m_bCardListData.writeByte(0x36);
				m_bCardListData.writeByte(0x37);
				m_bCardListData.writeByte(0x38);
				m_bCardListData.writeByte(0x39);
				m_bCardListData.writeByte(0x3A);
				m_bCardListData.writeByte(0x3B);
				m_bCardListData.writeByte(0x3C);
				m_bCardListData.writeByte(0x3D);//黑桃 A - K

				m_bCardListData.writeByte(0x41);
				m_bCardListData.writeByte(0x42);
			}
		}

		//类型函数
		//获取类型
		public function GetCardType(bCardData:Array,bCardCount:uint):uint
		{

			//开始分析
			switch (bCardCount)
			{
				case 1 ://单牌
					{
						return CT_SINGLE;







					};
				case 2 ://对牌和火箭
					{
						if (bCardData[0] == 0x42 && bCardData[1] == 0x41)
						{
							return CT_MISSILE_CARD;
						}
						return GetCardLogicValue(bCardData[0]) == GetCardLogicValue(bCardData[1])?CT_DOUBLE:CT_INVALID;







					};
				case 3 :
				case 4 ://连牌和炸弹
					{
						var bLogicValue:uint=GetCardLogicValue(bCardData[0]);
						for (var i:uint=1; i < bCardCount; i++)
						{
							if (bLogicValue != GetCardLogicValue(bCardData[i]))
							{
								break;
							}
						}
						if (i == bCardCount)
						{
							return bCardCount == 3?CT_THREE:CT_BOMB_CARD;
						}
						if (bCardCount == 3)
						{
							return CT_INVALID;
						}
						break;







				}
			};

			//其他牌型
			if (bCardCount >= 4)
			{
				//分析扑克
				var AnalyseResult:tagAnalyseResult=new tagAnalyseResult  ;
				AnalysebCardData(bCardData,bCardCount,AnalyseResult);

				//四牌判断
				if (AnalyseResult.bFourCount > 0)
				{
					if (AnalyseResult.bFourCount == 1 && bCardCount == 5)
					{
						//return CT_FOUR_LINE_TAKE_ONE;
					}
					if (AnalyseResult.bFourCount == 1 && AnalyseResult.bDoubleCount == 1 && bCardCount == 6)
					{
						//return CT_FOUR_LINE_TAKE_ONE;
					}
					if (AnalyseResult.bFourCount == 1 && AnalyseResult.bDoubleCount == 2 && bCardCount == 8)
					{
						//return CT_FOUR_LINE_TAKE_DOUBLE;
					}
					if (AnalyseResult.bFourCount == 1 && bCardCount == 6)
					{
						//return CT_FOUR_LINE_TAKE_DOUBLE;
					}

					return CT_INVALID;
				}

				//三牌判断
				if (AnalyseResult.bThreeCount > 0)
				{
					//连牌判断
					if (AnalyseResult.bThreeCount > 1)
					{
						if (AnalyseResult.bThreeLogicVolue[0] == 15)
						{
							return CT_INVALID;
						}
						for (var i:uint=1; i < AnalyseResult.bThreeCount; i++)
						{
							if (AnalyseResult.bThreeLogicVolue[i] != AnalyseResult.bThreeLogicVolue[0] - i)
							{
								return CT_INVALID;
							}
						}
					}

					//牌形判断
					if (AnalyseResult.bThreeCount * 3 == bCardCount)
					{
						return CT_THREE_LINE;
					}
					if (AnalyseResult.bThreeCount == bCardCount - AnalyseResult.bThreeCount * 3)
					{
						return CT_THREE_LINE_TAKE_ONE;
					}
					if (AnalyseResult.bDoubleCount == AnalyseResult.bThreeCount && bCardCount == AnalyseResult.bThreeCount * 3 + AnalyseResult.bDoubleCount * 2)
					{
						//return CT_THREE_LINE_TAKE_DOUBLE;
					}

					return CT_INVALID;
				}

				//两张类型
				if (AnalyseResult.bDoubleCount >= 3)
				{
					//二连判断
					if (AnalyseResult.bDoubleLogicVolue[0] != 15)
					{
						for (var i:uint=1; i < AnalyseResult.bDoubleCount; i++)
						{
							if (AnalyseResult.bDoubleLogicVolue[i] != AnalyseResult.bDoubleLogicVolue[0] - i)
							{
								return CT_INVALID;
							}
						}
						if (AnalyseResult.bDoubleCount * 2 == bCardCount)
						{
							return CT_DOUBLE_LINE;
						}
					}

					return CT_INVALID;
				}
				//单张判断
				if (AnalyseResult.bSignedCount >= 5 && AnalyseResult.bSignedCount == bCardCount)
				{
					var bLogicValue:uint=GetCardLogicValue(bCardData[0]);
					if (bLogicValue >= 15)
					{
						return CT_INVALID;
					}
					for (var i:uint=1; i < AnalyseResult.bSignedCount; i++)
					{
						if (GetCardLogicValue(bCardData[i]) != bLogicValue - i)
						{
							return CT_INVALID;
						}
					}

					return CT_ONE_LINE;
				}
				return CT_INVALID;
			}

			return CT_INVALID;
		}
		//获取数值
		public function GetCardValue(bCardData:uint):uint
		{
			return bCardData & LOGIC_MASK_VALUE;
		}
		//获取花色
		public function GetCardColor(bCardData:uint):uint
		{
			return bCardData & LOGIC_MASK_COLOR;
		}

		//控制函数
		//排列扑克
		public function SortCardList(bCardData:Array,bCardCount:uint):void
		{
			//转换数值
			var bLogicVolue:Array=new Array(20);
			for (var i:uint=0; i < bCardCount; i++)
			{
				bLogicVolue[i]=GetCardLogicValue(bCardData[i]);
			}

			//排序操作
			var bSorted:Boolean=true;
			var bTempData:uint;
			var bLast:uint=bCardCount - 1;
			do
			{
				bSorted=true;
				for (var i:uint=0; i < bLast; i++)
				{
					if (bLogicVolue[i] < bLogicVolue[i + 1] || bLogicVolue[i] == bLogicVolue[i + 1] && bCardData[i] < bCardData[i + 1])
					{
						//交换位置
						bTempData=bCardData[i];
						bCardData[i]=bCardData[i + 1];
						bCardData[i + 1]=bTempData;
						bTempData=bLogicVolue[i];
						bLogicVolue[i]=bLogicVolue[i + 1];
						bLogicVolue[i + 1]=bTempData;
						bSorted=false;
					}
				}
				bLast--;
			} while (bSorted == false);

			return;
		}
		//混乱扑克
		public function RandCardList(bCardBuffer:Array,bBufferCount:uint):void
		{
			//混乱准备
			var bCardData:Array=new Array(_nCardMaxCount);
			
			for(var i:uint = 0; i < _nCardMaxCount; i ++)
			{
				bCardData = m_bCardListData[i];
			}

			//混乱扑克
			var bRandCount:uint=0;
			var bPosition:uint=0;
			do
			{
				bPosition=randUInt() % bBufferCount - bRandCount;
				bCardBuffer[bRandCount++]=bCardData[bPosition];
				bCardData[bPosition]=bCardData[bBufferCount - bRandCount];
			} while (bRandCount < bBufferCount);

			return;
		}
		//删除扑克
		public function RemoveCard(bRemoveCard:Array,bRemoveCount:uint,bCardData:Array,bCardCount:uint):Boolean
		{
			//检验数据
			if (bRemoveCount > bCardCount)
			{
				return false;
			}

			//定义变量
			var bDeleteCount:uint=0;
			var bTempCardData:Array=new Array(20);
			if (bCardCount > bTempCardData.length)
			{
				return false;
			}
			CopyArray(bTempCardData,bCardData,bCardCount);

			//置零扑克
			for (var i:uint=0; i < bRemoveCount; i++)
			{
				for (var j:uint=0; j < bCardCount; j++)
				{
					if (bRemoveCard[i] == bTempCardData[j])
					{
						bDeleteCount++;
						bTempCardData[j]=0;
						break;
					}
				}
			}
			if (bDeleteCount != bRemoveCount)
			{
				return false;
			}

			//清理扑克
			var bCardPos:uint=0;
			for (var i:uint=0; i < bCardCount; i++)
			{
				if (bTempCardData[i] != 0)
				{
					bCardData[bCardPos++]=bTempCardData[i];
				}
			}

			return true;
		}

		//逻辑函数
		//逻辑数值
		public function GetCardLogicValue(bCardData:uint):uint
		{
			//扑克属性
			var bCardColor:uint=GetCardColor(bCardData);
			var bCardValue:uint=GetCardValue(bCardData);

			//转换数值
			if (bCardColor == 0x40)
			{
				return bCardValue + 15;
			}
			return bCardValue <= 2?bCardValue + 13:bCardValue;
		}

		//对比扑克
		public function CompareCard(bFirstList:Array,bNextList:Array,bFirstCount:uint,bNextCount:uint):Boolean
		{
			//获取类型
			var bNextType:uint=GetCardType(bNextList,bNextCount);
			var bFirstType:uint=GetCardType(bFirstList,bFirstCount);

			//类型判断
			if (bFirstType == CT_INVALID)
			{
				return false;
			}
			if (bFirstType == CT_MISSILE_CARD)
			{
				return true;
			}

			//炸弹判断
			if (bFirstType == CT_BOMB_CARD && bNextType != CT_BOMB_CARD)
			{
				return true;
			}
			if (bFirstType != CT_BOMB_CARD && bNextType == CT_BOMB_CARD)
			{
				return false;
			}

			//规则判断
			if (bFirstType != bNextType || bFirstCount != bNextCount)
			{
				return false;
			}

			//开始对比
			switch (bNextType)
			{
				case CT_SINGLE :
				case CT_DOUBLE :
				case CT_THREE :
				case CT_ONE_LINE :
				case CT_DOUBLE_LINE :
				case CT_THREE_LINE :
				case CT_BOMB_CARD :
					{
						var bFirstLogicValue:uint=GetCardLogicValue(bFirstList[0]);
						var bNextLogicValue:uint=GetCardLogicValue(bNextList[0]);
						return bFirstLogicValue > bNextLogicValue;


					};
				case CT_THREE_LINE_TAKE_ONE :
				case CT_THREE_LINE_TAKE_DOUBLE :
					{
						var NextResult:tagAnalyseResult=new tagAnalyseResult  ;
						var FirstResult:tagAnalyseResult=new tagAnalyseResult  ;
						AnalysebCardData(bNextList,bNextCount,NextResult);
						AnalysebCardData(bFirstList,bFirstCount,FirstResult);
						return FirstResult.bThreeLogicVolue[0] > NextResult.bThreeLogicVolue[0];


					};
				case CT_FOUR_LINE_TAKE_ONE :
				case CT_FOUR_LINE_TAKE_DOUBLE :
					{
						var NextResult:tagAnalyseResult=new tagAnalyseResult  ;
						var FirstResult:tagAnalyseResult=new tagAnalyseResult  ;
						AnalysebCardData(bNextList,bNextCount,NextResult);
						AnalysebCardData(bFirstList,bFirstCount,FirstResult);
						return FirstResult.bFourLogicVolue[0] > NextResult.bFourLogicVolue[0];


				}
			};

			return false;
		}

		//内部函数
		//分析扑克
		public function AnalysebCardData(bCardData:Array,bCardCount:uint,AnalyseResult:tagAnalyseResult):void
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
								var src_offset:uint = i - 2 + i == bCardCount - 1 && bCardValueTemp == bLogicValue?1:0;
								CopyArray(AnalyseResult.m_bDCardData,
										   bCardData,2,dst_offset,src_offset);
								AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
								break;

							};
						case 3 ://三张
							{
								var dst_offset:uint = AnalyseResult.bThreeCount * 3;
								var src_offset:uint = i - 3 + i == bCardCount - 1 && bCardValueTemp == bLogicValue?1:0;
						
								CopyArray(AnalyseResult.m_bTCardData,
										   bCardData,3,dst_offset,src_offset);
								AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
								break;

							};
						case 4 ://四张
							{
								var dst_offset:uint = AnalyseResult.bFourCount * 4;
								var src_offset:uint = i - 4 + i == bCardCount - 1 && bCardValueTemp == bLogicValue?1:0;
								
								CopyArray(AnalyseResult.m_bFCardData,
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
							AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i - 1];
						}
						else
						{
							AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i - 1];
							AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
						}
					}
					else
					{
						if (i == bCardCount - 1)
						{
							AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
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
	}
}
include "GameLogicDef.as"
include "../../../Common/Memory.as";
include "../../../Common/MathFunction.as";