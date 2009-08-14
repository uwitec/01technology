package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;


	public class GameLogic
	{
		//扑克类型
		public static  const CT_INVALID:uint=0;//错误类型

		//数值掩码
		public static  const LOGIC_MASK_COLOR:uint=0xF0;//花色掩码
		public static  const LOGIC_MASK_VALUE:uint=0x0F;//数值掩码

		public static var m_bCardListData:Array;
		
		public function GameLogic()
		{
			if(m_bCardListData == null)
			{
				m_bCardListData = new Array(0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
				0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//黑桃 A - K
				);
			}
		}
		//类型函数
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
				
		//排列扑克
		public function SortCardList( bCardData:Array,  bCardCount:uint):void
		{
			//转换数值
			var bLogicVolue:Array = new Array(16);
			for (var i:uint=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);
		
			//排序操作
			var bSorted:Boolean=true;
			var bTempData:uint;
			var bLast:uint=bCardCount-1;
			var m_bCardCount:uint=1;
			do
			{
				bSorted=true;
				for (var i:uint=0;i<bLast;i++)
				{
					if ((bLogicVolue[i]<bLogicVolue[i+1])||
						((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
					{
						//交换位置
						bTempData=bCardData[i];
						bCardData[i]=bCardData[i+1];
						bCardData[i+1]=bTempData;
						bTempData=bLogicVolue[i];
						bLogicVolue[i]=bLogicVolue[i+1];
						bLogicVolue[i+1]=bTempData;
						bSorted=false;
					}	
				}
				bLast--;
			} while(bSorted==false);
		
			return;
		}
		
		//混乱扑克
		public function RandCardList(bCardBuffer:Array, bBufferCount:uint):void
		{
			//混乱准备
			var bCardData:Array = new Array(m_bCardListData.length);
			CopyArray(bCardData,m_bCardListData);
		
		
			//混乱扑克
			var bRandCount:uint=0;
			var bPosition:uint=0;
			do
			{
				
				bPosition=randUInt()%(bBufferCount-bRandCount);
				bCardBuffer[bRandCount++]=bCardData[bPosition];
				bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
			} while (bRandCount<bBufferCount);
		
			return;
		}
		
		//删除扑克
		public function RemoveCard(bRemoveCard:Array, bRemoveCount:uint,bCardData:Array, bCardCount:uint):Boolean
		{
			//检验数据
			ASSERT(bRemoveCount<=bCardCount);
		
			//定义变量
			var bDeleteCount:uint=0;
			var bTempCardData:Array = new Array(HAND_CARD_COUNT);
			if (bCardCount>CountArray(bTempCardData)) return false;
			CopyArray(bTempCardData,bCardData,bCardCount);
		
			//置零扑克
			for (var i:uint=0;i<bRemoveCount;i++)
			{
				for (var j:uint=0;j<bCardCount;j++)
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
			for (var i:uint=0;i<bCardCount;i++)
			{
				if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
			}
		
			return true;
		}
		//逻辑数值 
		 public function GetCardLogicValue(bCardData:uint):uint
		{
			//扑克属性
			var bCardValue:uint=GetCardValue(bCardData);  
			var bCardColor:uint=GetCardColor(bCardData) ;
			var bTempValue:uint=((1==bCardValue)?(bCardValue+13):bCardValue);
		
			//转换数值
			switch ((bCardColor>>4))
			{
			case 0: { return bTempValue; }
			case 1: { return bTempValue+14; }
			case 2: { return bTempValue+2*14; }
			case 3: { return bTempValue+3*14; }
			}
		
			//错误断言
			ASSERT(false);
		
			return 255;
		}
		
		//获取分数
		public function GetScoreCard(bCardData:Array , bScoreCardData:Array , bCardDataCount:uint , bScoreCount:uint):uint 
		{
			var  bCardColor:uint ;
			var  bCardValue:uint ;
		
			bScoreCount = 0;
		
			for(var i:uint=0 ; i<bCardDataCount ; ++i)
			{
				bCardColor = GetCardColor(bCardData[i]) ;
				bCardColor = bCardColor>>4 ;
				bCardValue = GetCardValue(bCardData[i]) ;
		        
				if(2==bCardColor || (0==bCardColor && 11==bCardValue) || (1==bCardColor && 10==bCardValue) || (3==bCardColor && 12==bCardValue)) bScoreCardData[bScoreCount++] = bCardData[i] ;
			}
		
			return bScoreCount;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"
include "../../../Common/MathFunction.as"