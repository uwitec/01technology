package org.god.GameModule.B_BlackJack.Common
{
	public class GameLogic
	{
		private static var m_bCardArray:Array;					//扑克定义

		public function GameLogic()
		{
			if(m_bCardArray == null)
			{
				m_bCardArray = new Array( 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
	14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52);
			}
		}
				//获取扑克数值
		public function GetCardValue( bCard:uint):uint
		{
			var bValue:uint = bCard%13;
			if (bValue == 0 || bValue == 11 || bValue == 12) bValue=10;
			if (bValue == 1) bValue = 11;
			return bValue;
		}
		
		//获取扑克总值
		public function GetCardSum(bCardBuffer:Array,bCardCount:uint):uint
		{
			var cbCardSum:uint = 0;
			
			for(var i:uint = 0; i < bCardCount; i++)
			{
				cbCardSum += GetCardValue(bCardBuffer[i]);
			}
			
			if(cbCardSum > 21)
			{
				for(var i:uint = 0; i < bCardCount; i++)
				{
					//是否有A
					if(GetCardValue(bCardBuffer[i]) == 11)
					{
						//按1计算
						cbCardSum -= 10;
					}
				}
			}
		
			return cbCardSum;
		}
		
		//是否爆牌
		public function IsBurst(bCardBuffer:Array, bCardCount:uint):Boolean
		{
			var wCardSum:uint = 0;
			
			for(var i:uint = 0; i < bCardCount; i++)
			{
				wCardSum += GetCardValue(bCardBuffer[i]);
			}
		
			if(wCardSum > 21)
			{
				for(var i:uint = 0; i < bCardCount; i++)
				{
					//是否有A
					if(GetCardValue(bCardBuffer[i]) == 11)
					{
						//按1计算
						wCardSum -= 10;
					}
				}
			}
		
			if(wCardSum > 21) return true;
			
			return false;
		}
		
		//能否分牌
		public function CanCutCard(bFirstCard:uint, bLastCard:uint):Boolean
		{
			var bFirstValue:uint = bFirstCard % 13;
			var bLastValue:uint  = bLastCard  % 13;
			if (bFirstValue == bLastValue) return true;
			return false;
		}
		
		//是否BlackJack
		public function IsBlackJack(bCardBuffer:Array):Boolean
		{
			var bCardSum:uint=0;
			if((GetCardValue(bCardBuffer[0]) == 1) && (GetCardValue(bCardBuffer[1]) == 10)) return true;
			if((GetCardValue(bCardBuffer[1]) == 1) && (GetCardValue(bCardBuffer[0]) == 10)) return true;
			return false;
		}
		
		//对比单只扑克
		public function CompareOnlyOne(bFirstCard:uint, bLastCard:uint):Boolean
		{
			var bFirstValue:uint = GetCardValue(bFirstCard);
			var bLastValue:uint  = GetCardValue(bLastCard);
			if (bFirstValue == bLastValue) return bFirstValue > bLastValue;
			return bFirstValue > bLastValue;
		}
		
		//推断胜者
		public function DeduceWiner(bBankerCard:Array,bBankerCardCount:uint,
									 bIdleCard:Array, bIdleCardCount:uint):uint
		{
			//平
			if(IsBlackJack(bBankerCard) && IsBlackJack(bIdleCard)) return 0;
		
			//庄家赢
			if(IsBlackJack(bBankerCard) && !IsBlackJack(bIdleCard)) return 1;
		
			//闲家赢
			if(!IsBlackJack(bBankerCard) && IsBlackJack(bIdleCard)) return 2;
		
			//比较大小
			var bBankerSum:uint = 0;
			var bIdleSum:uint   = 0;
			
			for(var i:uint=0;i<bBankerCardCount;i++)
			{
				bBankerSum += GetCardValue(bBankerCard[i]);
			}
		
			for(var i:uint=0;i<bIdleCardCount;i++)
			{
				bIdleSum += GetCardValue(bIdleCard[i]);
			}
		
			if(bBankerSum > bIdleSum) 
				return 1;
			else if(bBankerSum < bIdleSum)
				return 2;
			else
				return 0;
		}
		
		//混乱扑克
		public function RandCard(bCardBuffer:Array, bBufferCount:uint):void
		{
			var bSend:uint = 0;
			var bStation:uint = 0;
			var bCardList:Array = new Array(CountArray(m_bCardArray));
		
			CopyArray(bCardList, m_bCardArray, CountArray(m_bCardArray));
			
		
			do
			{
				bStation = randUInt() % (CountArray(m_bCardArray) - bSend);
		
				bCardBuffer[bSend++] = bCardList[bStation];
				bCardList[bStation]  = bCardList[CountArray(m_bCardArray)-bSend];
			
			} while (bSend < bBufferCount);	
		}
		
		//排列扑克
		public function SortCard(bCardList:Array, bCardCount:uint):void
		{
			var bSorted:Boolean     = true;
			var bTemp:uint = 0;
			var bLast:uint = bCardCount - 1;
			do
			{
				bSorted = true;
				for (var i:uint = 0; i < bLast; i++) 
				{
					if (CompareOnlyOne(bCardList[i],bCardList[i+1]) == false)
					{	
						bTemp = bCardList[i];
						bCardList[i]   = bCardList[i+1];
						bCardList[i+1] = bTemp;
						bSorted = false;
					}	
				}
				bLast--;
			} while (bSorted == false);
		
			return;
		}
	}
}
include "../../../Common/Memory.as"
include "../../../Common/MathFunction.as"