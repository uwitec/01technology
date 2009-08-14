package org.god.GameModule.B_Upgrade.Common
{
	public class GameLogic
	{
		//属性变量
		protected var m_cbPackCount:uint;//牌副数目
		protected var m_cbDispatchCount:uint;//派发数目

		//逻辑变量
		protected var m_cbMainColor:uint;//主牌花色
		protected var m_cbMainValue:uint;//主牌数值

		//辅助变量
		protected var m_cbSortRight:Array = new Array(5);//排序权位

		//变量定义
		public static  var m_cbNTValue:uint;//常主数值
		public static  var m_cbCardData:Array;//扑克数据
		
		public function GameLogic()
		{
			if(m_cbCardData == null)
			{
				m_cbNTValue=VALUE_ERROR;
				m_cbCardData=new Array
				(
					0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
					0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
					0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
					0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
					0x4E,0x4F
				);
			}
			
			//属性变量
			m_cbPackCount=0;
			m_cbDispatchCount=0;

			//逻辑变量
			m_cbMainValue=VALUE_ERROR;
			m_cbMainColor=COLOR_ERROR;
		
			//辅助变量
			for (var i:uint=0;i<CountArray(m_cbSortRight);i++) 
				m_cbSortRight[i]=i*COLOR_RIGHT;
		}
				
		//设置副数
		public function SetPackCount(cbPackCount:uint):Boolean
		{
			//效验参数
			ASSERT((cbPackCount>0)&&(cbPackCount<=2));
			if ((cbPackCount==0)||(cbPackCount>2)) return false;
		
			//副数数目
			m_cbPackCount=cbPackCount;
		
			//派发数目
			var cbConcealCount:uint=((m_cbPackCount%2)==0)?8:6;
			m_cbDispatchCount=((m_cbPackCount*CELL_PACK)-cbConcealCount)/4;
		
			return false;
		}
		
		//主牌花色
		public function SetMainColor(cbMainColor:uint):Boolean 
		{
			//效验参数
			ASSERT((cbMainColor==COLOR_ERROR)||(cbMainColor<=COLOR_NT));
			if ((cbMainColor!=COLOR_ERROR)&&(cbMainColor>COLOR_NT)) return false;
		
			//设置变量
			m_cbMainColor=cbMainColor;
		
			//更新权位
			UpdateSortRight();
		
			return true;
		}
		
		//主牌数值
		public function SetMainValue(cbMainValue:uint):Boolean
		{
			//效验参数
			ASSERT((cbMainValue==VALUE_ERROR)||(cbMainValue<=0x0D));
			if ((cbMainValue!=VALUE_ERROR)&&(cbMainValue>0x0D)) return false;
		
			//设置变量
			m_cbMainValue=cbMainValue;
		
			//更新权位
			UpdateSortRight();
		
			return true;
		}
		
		//获取类型
		public function GetCardType(cbCardData:Array, cbCardCount:uint):uint
		{
			var i:uint = 0;
			var j:uint = 0;
			//状态效验
			ASSERT(m_cbMainColor!=COLOR_ERROR);
			ASSERT(m_cbMainValue!=VALUE_ERROR);
		
			//数目判断
			ASSERT((cbCardCount>0)&&(cbCardCount<=MAX_COUNT));
			if ((cbCardCount==0)||(cbCardCount>MAX_COUNT)) return CT_ERROR;
		
			//同花判断
			if (GetCardLogicColor2(cbCardData,cbCardCount)==COLOR_ERROR) return CT_ERROR;
		
			//同牌判断
			var cbSameCount:uint=1;
			for (i=1;i<cbCardCount;i++) 
			{
				if (cbCardData[i]==cbCardData[0]) cbSameCount++;
				else break;
			}
			if (cbSameCount==cbCardCount) return CT_SINGLE+cbSameCount-1;
		
			//拖牌判断
			if ((cbSameCount>=2)&&(cbCardCount%cbSameCount)==0)
			{
				//变量定义
				var cbBlockCount:uint=cbCardCount/cbSameCount;
		
				//单元判断
				for (i=1;i<cbBlockCount;i++)
				{
					//变量定义
					var cbIndex:uint=i*cbSameCount;
					var cbFirstValue:uint=GetCardValue(cbCardData[0]);
		
					//相同分析
					for (j=0;j<cbSameCount;j++)
					{
						if (cbCardData[cbIndex+j]!=cbCardData[cbIndex]) break;
					}
					if (j!=cbSameCount) break;
		
					//相连分析
					if (IsLineValue(cbCardData[cbIndex-cbSameCount],cbCardData[cbIndex])==false) break;
				}
		
				//成功判断
				if (i==cbBlockCount) return CT_TRACKOR_2+cbSameCount-2;
			}
		
			return CT_THROW_CARD;
		}
		
		//排列扑克
		public function SortCardList(cbCardData:Array, cbCardCount:uint):void
		{
			//转换数值
			var cbSortValue:Array = new Array(MAX_COUNT);
			for (var i:uint=0;i<cbCardCount;i++) cbSortValue[i]=GetCardSortOrder(cbCardData[i]);	
		
			//排序操作
			var bSorted:Boolean=true;
			var bTempData:uint;
			var bLast:uint=cbCardCount-1;
			do
			{
				bSorted=true;
				for (var i:uint=0;i<bLast;i++)
				{
					if ((cbSortValue[i]<cbSortValue[i+1])||
						((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
					{
						//交换位置
						bTempData=cbCardData[i];
						cbCardData[i]=cbCardData[i+1];
						cbCardData[i+1]=bTempData;
						bTempData=cbSortValue[i];
						cbSortValue[i]=cbSortValue[i+1];
						cbSortValue[i+1]=bTempData;
						bSorted=false;
					}	
				}
				bLast--;
			} while(bSorted==false);
		
			return;
		}
		//获取数值
		public function GetCardValue(cbCardData:uint):uint { return cbCardData&LOGIC_MASK_VALUE; }
		//获取花色
		public function GetCardColor(cbCardData:uint):uint { return cbCardData&LOGIC_MASK_COLOR; }
		//获取副数
		public function GetPackCount():uint { return m_cbPackCount; }
		//派发数目
		public function GetDispatchCount():uint { return m_cbDispatchCount; }
		//主牌花色
		public function GetMainColor():uint { return m_cbMainColor; }
		//主牌数值
		public function GetMainValue():uint { return m_cbMainValue; }
		//混乱扑克
		public function RandCardList(cbCardBuffer:Array,  cbBufferCount:uint):void
		{
			//混乱准备
			var cbCardData:Array = new Array(CELL_PACK*2);
			for (var i:uint=0;i<2;i++) 
			{
					CopyArray(cbCardData,m_cbCardData,m_cbCardData.length,CELL_PACK*i,0);
			}
		
			//混乱扑克
			var cbRandCount:uint=0;
			var cbPosition:uint=0;
			do
			{
				cbPosition=randUInt()%(cbBufferCount-cbRandCount);
				cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
				cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
			} while (cbRandCount<cbBufferCount);
		
			return;
		}
		
		//删除扑克
		public function RemoveCard(cbRemoveCard:Array,
								   cbRemoveCount:uint,
								   cbCardData:Array, 
								   cbCardCount:uint,
								   cbRemoveCardIndex:uint = 0):Boolean
		{
			//检验数据
			ASSERT(cbRemoveCount<=cbCardCount);
		
			//定义变量
			var cbDeleteCount:uint=0;
			var cbTempCardData:Array = new Array(MAX_COUNT);
			if (cbCardCount>CountArray(cbTempCardData)) return false;
			CopyArray(cbTempCardData,cbCardData,cbCardCount);
		
			//置零扑克
			for (var i:uint=0;i<cbRemoveCount;i++)
			{
				for (var j:uint=0;j<cbCardCount;j++)
				{
					if (cbRemoveCard[i + cbRemoveCardIndex]==cbTempCardData[j])
					{
						cbDeleteCount++;
						cbTempCardData[j]=0;
						break;
					}
				}
			}
			if (cbDeleteCount!=cbRemoveCount) return false;
		
			//清理扑克
			var cbCardPos:uint=0;
			for (var i:uint=0;i<cbCardCount;i++)
			{
				if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
			}
		
			return true;
		}
		
		//获取积分
		public function GetCardScore(cbCardData:Array, cbCardCount:uint):uint
		{
			//变量定义
			var wCardScore:uint=0;
		
			//扑克累计
			for (var i:uint=0;i<cbCardCount;i++)
			{
				//获取数值
				var cbCardValue:uint=GetCardValue(cbCardData[i]);
		
				//累计积分
				if (cbCardValue==5) 
				{
					wCardScore+=5;
					continue;
				}
		
				//累计积分
				if ((cbCardValue==10)||(cbCardValue==13)) 
				{
					wCardScore+=10;
					continue;
				}
			}
		
			return wCardScore;
		}
		
		//获取积分
		public function GetScoreCard(cbCardData:Array, cbCardCount:uint,cbScoreCard:Array, cbMaxCount:uint):uint
		{
			//变量定义
			var cbPostion:uint=0;
		
			//扑克累计
			for (var i:uint=0;i<cbCardCount;i++)
			{
				//获取数值
				var cbCardValue:uint=GetCardValue(cbCardData[i]);
		
				//累计扑克
				if ((cbCardValue==5)||(cbCardValue==10)||(cbCardValue==13))
				{
					ASSERT(cbPostion<cbMaxCount);
					cbScoreCard[cbPostion++]=cbCardData[i];
				}
			}
		
			return cbPostion;
		}
		
		//效验甩牌
		public function EfficacyThrowCard(cbOutCardData:Array, 
		cbOutCardCount:uint, wOutCardIndex:uint, 
		cbHandCardData:Array, 
		cbHandCardCount:uint, 
		OutCardResult:tagOutCardResult):Boolean
		{
			//分析扑克
			var AnalyseResult:tagAnalyseResult;
			AnalyseCardData(cbOutCardData,cbOutCardCount,AnalyseResult);
		
			//循环比较
			for (var i:uint=0;i<GAME_PLAYER-1;i++)
			{
				//获取数据
				var wUserIndex:uint=(wOutCardIndex+i+1)%GAME_PLAYER;
		
				//提取扑克
				var cbDistillCardHand:Array = new Array(MAX_COUNT);
				var cbDistillCountHand:uint=DistillCardByColor(cbHandCardData[wUserIndex],
				cbHandCardCount,
				AnalyseResult.cbCardColor,
				cbDistillCardHand);
		
				//判断过滤
				if (cbDistillCountHand==0) continue;
		
				//牌型判断
				for (var j:uint=2;j>=2;j--)
				{
					//变量定义
					var pSameDataInfo:tagSameDataInfo=AnalyseResult.SameDataInfo[j-1];
					var pTractorDataInfo:tagTractorDataInfo=AnalyseResult.TractorDataInfo[j-2];
		
					//拖牌判断
					if (pTractorDataInfo.cbTractorCount>0)
					{
						//变量定义
						var cbOutTractorIndex:uint=0;
						var cbUserTractorIndex:uint=0;
		
						//提取拖牌
						var TractorDataInfo:tagTractorDataInfo = new tagTractorDataInfo;
						DistillTrackorByCount(cbDistillCardHand,cbDistillCountHand,j,TractorDataInfo);
		
						//循环对比
						for (var cbUserRound:uint=0;cbUserRound<TractorDataInfo.cbTractorCount;cbUserRound++)
						{
							//获取属性
							var cbLineCountUser:uint=TractorDataInfo.cbTractorData[cbUserTractorIndex];
							var cbSameCountUser:uint=TractorDataInfo.cbTractorData[cbUserTractorIndex+1];
		
							//对比拖牌
							for (var cbOutRound:uint=0;cbOutRound<pTractorDataInfo.cbTractorCount;cbOutRound++)
							{
								//获取属性
								var cbLineCountOut:uint=pTractorDataInfo.cbTractorData[cbOutTractorIndex];
								var cbSameCountOut:uint=pTractorDataInfo.cbTractorData[cbOutTractorIndex+1];
		
								//拖牌对比
								if (cbLineCountUser>=cbLineCountOut)
								{
									//扑克定义
									var cbNextCardData:uint=TractorDataInfo.cbTractorData[cbUserTractorIndex+2];
									var cbFisrtCardData:uint=pTractorDataInfo.cbTractorData[cbOutTractorIndex+2];
		
									//对比大小
									if (CompareCardData(cbFisrtCardData,cbNextCardData)==true)
									{
										OutCardResult.cbCardCount=cbLineCountOut*cbSameCountOut;
										CopyArray(OutCardResult.cbResultCard,
										pTractorDataInfo.cbTractorData,
										OutCardResult.cbCardCount,
										0,
										cbOutTractorIndex+2);
		
										return false;
									}
								}
		
								//设置索引
								cbOutTractorIndex+=(2+cbLineCountOut*cbSameCountOut);
							}
		
							//设置索引
							cbUserTractorIndex+=(2+cbLineCountUser*cbSameCountUser);
						}
					}
		
					//同牌判断
					if (pSameDataInfo.cbBlockCount>0)
					{
						//提取同牌
						var SameDataInfo:tagSameDataInfo = new tagSameDataInfo;
						DistillCardByCount(cbDistillCardHand,cbDistillCountHand,j,SameDataInfo);
		
						//同牌判断
						if (SameDataInfo.cbBlockCount>0)
						{
							//扑克定义
							var cbLessSameIndex:uint=pSameDataInfo.cbCardCount-j;
							var cbFisrtCardData:uint=pSameDataInfo.cbSameData[cbLessSameIndex];
		
							//对比大小
							if (CompareCardData(cbFisrtCardData,SameDataInfo.cbSameData[0])==true)
							{
								OutCardResult.cbCardCount=j;
								CopyArray(OutCardResult.cbResultCard,
								pSameDataInfo.cbSameData,
								OutCardResult.cbCardCount,
								0,cbLessSameIndex);
		
								return false;
							}
						}
					}
				}
		
				//单牌判断
				if (AnalyseResult.SameDataInfo[0].cbCardCount!=0)
				{
					//变量定义
					var cbCardCount:uint=AnalyseResult.SameDataInfo[0].cbCardCount;
					var cbLessCardData:uint=AnalyseResult.SameDataInfo[0].cbSameData[cbCardCount-1];
		
					//对比扑克
					if (CompareCardData(cbLessCardData,cbDistillCardHand[0])==true)
					{
						OutCardResult.cbCardCount=1;
						OutCardResult.cbResultCard[0]=cbLessCardData;
		
						return false;
					}
				}
			}
		
			return true;
		}
		
		//对比扑克
		public function CompareCardData(cbFirstCardData:uint, cbNextCardData:uint):Boolean
		{
			//获取花色
			var cbLogicColorNext:uint=GetCardLogicColor1(cbNextCardData);
			var cbLogicColorFirst:uint=GetCardLogicColor1(cbFirstCardData);
		
			//对比大小
			if ((cbLogicColorNext==COLOR_NT)||(cbLogicColorNext==cbLogicColorFirst))
			{
				//获取大小
				var cbLogicValueNext:uint=GetCardLogicValue(cbNextCardData);
				var cbLogicValueFirst:uint=GetCardLogicValue(cbFirstCardData);
		
				//大小判断
				if (cbLogicValueNext>cbLogicValueFirst) return true;
			}
		
			return false;
		}
		
		//对比扑克
		public function CompareCardResult(WinnerResult:tagAnalyseResult, UserResult:tagAnalyseResult):Boolean
		{
			//对比拖牌
			for (var i:uint=2;i>=2;i--)
			{
				//变量定义
				var pTractorDataInfoUser:tagTractorDataInfo=UserResult.TractorDataInfo[i-2];
				var pTractorDataInfoWinner:tagTractorDataInfo=WinnerResult.TractorDataInfo[i-2];
		
				//对比拖牌
				if ((pTractorDataInfoWinner.cbTractorCount>0)&&(pTractorDataInfoUser.cbTractorCount>0))
				{
					//获取大小
					var cbNextCardData:uint=pTractorDataInfoUser.cbTractorData[2];
					var cbFisrtCardData:uint=pTractorDataInfoWinner.cbTractorData[2];
		
					//对比大小
					if (CompareCardData(cbFisrtCardData,cbNextCardData)==true) return true;
				}
			}
		
			//对比同牌
			for (var i:uint=2;i>=1;i--)
			{
				//变量定义
				var pSameDataInfoUser:tagSameDataInfo=UserResult.SameDataInfo[i-1];
				var pSameDataInfoWinner:tagSameDataInfo=WinnerResult.SameDataInfo[i-1];
		
				//对比同牌
				if ((pSameDataInfoWinner.cbCardCount>0)&&(pSameDataInfoUser.cbCardCount>0))
				{
					//获取大小
					var cbNextCardData:uint=pSameDataInfoUser.cbSameData[0];
					var cbFisrtCardData:uint=pSameDataInfoWinner.cbSameData[0];
		
					//对比大小
					if (CompareCardData(cbFisrtCardData,cbNextCardData)==true) return true;
				}
			}
		
			return false;
		}
		
		//对比扑克
		public function CompareCardArray(cbOutCardData:Array, cbCardCount:uint, wFirstIndex:uint):uint
		{
			//变量定义
			var wWinnerIndex:uint=wFirstIndex;
			var cbCardType:uint=GetCardType(cbOutCardData[wFirstIndex],cbCardCount);
		
			//对比扑克
			switch (cbCardType)
			{
			case CT_SINGLE:		//单牌类型
			case CT_SAME_2:		//对牌类型
			case CT_SAME_3:		//三牌类型
			case CT_SAME_4:		//四牌类型
			case CT_TRACKOR_2:	//拖拉机型
			case CT_TRACKOR_3:	//拖拉机型
			case CT_TRACKOR_4:	//拖拉机型
				{
					//循环比较
					for (var i:uint=0;i<GAME_PLAYER-1;i++)
					{
						//用户索引
						var wUserIndex:uint=(wFirstIndex+i+1)%GAME_PLAYER;
		
						//牌型判断
						if (GetCardType(cbOutCardData[wUserIndex],cbCardCount)!=cbCardType) continue;
		
						//对比大小
						if (CompareCardData(cbOutCardData[wWinnerIndex][0],cbOutCardData[wUserIndex][0])==true) 
						{
							wWinnerIndex=wUserIndex;
							continue;
						}
					}
		
					return wWinnerIndex;
				}
			case CT_THROW_CARD:	//甩牌类型
				{
					//花色判断
					if (GetCardLogicColor1(cbOutCardData[wWinnerIndex][0])==COLOR_NT) return wFirstIndex;
		
					//分析牌型
					var WinnerResult:tagAnalyseResult = new tagAnalyseResult;
					AnalyseCardData(cbOutCardData[wFirstIndex],cbCardCount,WinnerResult);
		
					//循环比较
					for (var i:uint=0;i<GAME_PLAYER-1;i++)
					{
						//规则判断
						var wUserIndex:uint=(wFirstIndex+i+1)%GAME_PLAYER;
						if (GetCardLogicColor2(cbOutCardData[wUserIndex],cbCardCount)!=COLOR_NT) continue;
		
						//构造牌型
						var UserResult:tagAnalyseResult = new tagAnalyseResult;
						if (RectifyCardWeave(cbOutCardData[wUserIndex],cbCardCount,WinnerResult,UserResult)==false) continue;
		
						//对比结果
						if (CompareCardResult(WinnerResult,UserResult)==true)
						{
							wWinnerIndex=wUserIndex;
							tagAnalyseResult.CopyData(WinnerResult,UserResult);
						}
					}
		
					return wWinnerIndex;
				}
			}
		
			//错误断言
			ASSERT(false);
		
			return wFirstIndex;
		}
		
		//分析扑克
		 public function AnalyseCardData(cbCardData:Array,  cbCardCount:uint,AnalyseResult:tagAnalyseResult):Boolean
		{
			//设置结果
			tagAnalyseResult.ZeroData(AnalyseResult);
		
			//花色判断
			AnalyseResult.cbCardColor=GetCardLogicColor2(cbCardData,cbCardCount);
			if (AnalyseResult.cbCardColor==COLOR_ERROR) return false;
		
			//变量定义
			var cbAnalyseCard:Array = new Array(MAX_COUNT);
			var cbAnalyseCount=cbCardCount;
			CopyArray(cbAnalyseCard,cbCardData,cbCardCount);
		
			//提取扑克
			for (var i:uint=2;i>=2;i--)
			{
				//提取拖牌
				var pTractorDataInfo:tagTractorDataInfo=AnalyseResult.TractorDataInfo[i-2];
				DistillTrackorByCount(cbAnalyseCard,cbAnalyseCount,i,AnalyseResult.TractorDataInfo[i-2]);
		
				//拖牌处理
				if (pTractorDataInfo.cbTractorCount>0)
				{
					var cbIndex:uint=0;
					var cbRemoveCount:uint=0;
					for (var j:uint=0;j<pTractorDataInfo.cbTractorCount;j++)
					{
						//变量计算
						cbRemoveCount=pTractorDataInfo.cbTractorData[cbIndex]*i;
		
						//删除扑克
						cbAnalyseCount-=cbRemoveCount;
						RemoveCard(pTractorDataInfo.cbTractorData,
						cbRemoveCount,
						cbAnalyseCard,
						cbAnalyseCount+cbRemoveCount,
						cbIndex+2);
		
						//设置变量
						cbIndex+=cbRemoveCount+2;
					}
				}
		
				//提取同牌
				var pSameDataInfo:tagSameDataInfo=AnalyseResult.SameDataInfo[i-1];
				DistillCardByCount(cbAnalyseCard,cbAnalyseCount,i,AnalyseResult.SameDataInfo[i-1]);
		
				//删除扑克
				if (pSameDataInfo.cbBlockCount>0)
				{
					cbAnalyseCount-=pSameDataInfo.cbCardCount;
					RemoveCard(pSameDataInfo.cbSameData,
					pSameDataInfo.cbCardCount,
					cbAnalyseCard,
					cbAnalyseCount+pSameDataInfo.cbCardCount);
				}
			}
		
			//提取单牌
			AnalyseResult.SameDataInfo[0].cbCardCount=cbAnalyseCount;
			AnalyseResult.SameDataInfo[0].cbBlockCount=cbAnalyseCount;
			CopyArray(AnalyseResult.SameDataInfo[0].cbSameData,cbAnalyseCard,cbAnalyseCount);
		
			return true;
		}

		//出牌判断
		public function SearchOutCard(cbHandCardData:Array,
		 cbHandCardCount:uint,cbTurnCardData:Array, 
		  cbTurnCardCount:uint,
		  OutCardResult:tagOutCardResult):Boolean
		{
			//设置变量
			tagOutCardResult.ZeroData(OutCardResult);
			
			//首出用户
			if (cbTurnCardCount==0)
			{
				for (var i:uint=2;i>=2;i--)
				{
					//提取拖牌
					var TractorDataInfo:tagTractorDataInfo = new tagTractorDataInfo;
					DistillTrackorByCount(cbHandCardData,cbHandCardCount,i,TractorDataInfo);
		
					//拖牌判断
					if (TractorDataInfo.cbTractorCount>0)
					{
						//获取属性
						var cbLineCount:uint=TractorDataInfo.cbTractorData[0];
						var cbSameCount:uint=TractorDataInfo.cbTractorData[1];
						
						//构造结果
						OutCardResult.cbCardCount=cbLineCount*cbSameCount;
						CopyArray(OutCardResult.cbResultCard,
						TractorDataInfo.cbTractorData,
						OutCardResult.cbCardCount,0,2);
		
						return true;
					}
		
					//提取同牌
					var SameDataInfo:tagSameDataInfo = new tagSameDataInfo;
					DistillCardByCount(cbHandCardData,cbHandCardCount,i,SameDataInfo);
		
					//同牌判断
					if (SameDataInfo.cbCardCount>0)
					{
						//构造结果
						OutCardResult.cbCardCount=i;
						CopyArray(OutCardResult.cbResultCard,SameDataInfo.cbSameData,i,0,0);
		
						return true;
					}
				}
		
				//提取单牌
				OutCardResult.cbCardCount=1;
				OutCardResult.cbResultCard[0]=cbHandCardData[0];
		
				return true;
			}
		
			//分析扑克
			var AnalyseResult:tagAnalyseResult = new tagAnalyseResult;
			AnalyseCardData(cbTurnCardData,cbTurnCardCount,AnalyseResult);
		
			//提取扑克
			var cbAnalyseCard:Array = new Array(MAX_COUNT);
			var cbAnalyseCardCount:uint=DistillCardByColor(cbHandCardData,
			cbHandCardCount,
			GetCardLogicColor1(cbTurnCardData[0]),
			cbAnalyseCard);
		
			//数目判断
			if (cbTurnCardCount>=cbAnalyseCardCount)
			{
				//构造扑克
				OutCardResult.cbCardCount=cbAnalyseCardCount;
				CopyArray(OutCardResult.cbResultCard,cbAnalyseCard,cbAnalyseCardCount);
		
				//剩余扑克
				if (cbTurnCardCount>cbAnalyseCardCount)
				{
					//变量定义
					var cbSourceCount:uint=OutCardResult.cbCardCount;
					var cbLeaveCount:uint=(cbTurnCardCount-cbAnalyseCardCount);
		
					//构造扑克
					var cbCardDataTemp:Array = new Array(MAX_COUNT);
					var cbCardCountTemp:uint=cbHandCardCount;
					CopyArray(cbCardDataTemp,cbHandCardData,cbHandCardCount);
		
					//删除扑克
					cbCardCountTemp-=cbAnalyseCardCount;
					RemoveCard(cbAnalyseCard,cbAnalyseCardCount,cbCardDataTemp,cbCardCountTemp+cbAnalyseCardCount);
		
					//构造结果
					OutCardResult.cbCardCount+=cbLeaveCount;
					CopyArray(OutCardResult.cbResultCard,
					cbCardDataTemp,
					cbLeaveCount,
					cbSourceCount,
					cbCardCountTemp-cbLeaveCount);
				}
		
				return true;
			}
		
			//需求变量
			var DemandInfo:tagDemandInfo= new tagDemandInfo;
			tagDemandInfo.ZeroData(DemandInfo);
		
			//需求分析
			for (var i:uint=2;i>=2;i--)
			{
				//变量定义
				var pSameDataInfo:tagSameDataInfo=AnalyseResult.SameDataInfo[i-1];
				var pTractorDataInfo:tagTractorDataInfo=AnalyseResult.TractorDataInfo[i-2];
		
				//拖牌需求
				if (pTractorDataInfo.cbTractorCount>0)
				{
					DemandInfo.cbTractorCardCount[i-2]=pTractorDataInfo.cbCardCount;
					DemandInfo.cbMaxTractor[i-2]=pTractorDataInfo.cbTractorMaxLength;
				}
		
				//同牌需求
				if (pSameDataInfo.cbBlockCount>0) DemandInfo.cbSameCardCount[i-1]=pSameDataInfo.cbCardCount;
			}
		
			//单牌需求
			if (AnalyseResult.SameDataInfo[0].cbCardCount>0)
			 DemandInfo.cbSameCardCount[0]=AnalyseResult.SameDataInfo[0].cbCardCount;
		
			//出牌搜索
			for (var i:uint=2;i>=2;i--)
			{
				//拖牌判断
				if (DemandInfo.cbTractorCardCount[i-2]>0)
				{
					//提取拖牌
					var TractorDataInfo:tagTractorDataInfo = new tagTractorDataInfo;
					DistillTrackorByCount(cbAnalyseCard,cbAnalyseCardCount,i,TractorDataInfo);
		
					//变量定义
					var cbIndex:uint=0;
					var cbLeaveCount:uint=DemandInfo.cbTractorCardCount[i-2];
		
					//提取扑克
					for (var j:uint=0;j<TractorDataInfo.cbTractorCount;j++)
					{
						//调整索引
						if (j==0) cbIndex=TractorDataInfo.cbTractorMaxIndex;
						if ((j==1)&&(TractorDataInfo.cbTractorMaxIndex!=0)) cbIndex=0;
		
						//调整索引
						if ((j!=0)&&(cbIndex==TractorDataInfo.cbTractorMaxIndex))
						{
							//获取属性
							var cbLineCount:uint=TractorDataInfo.cbTractorData[cbIndex];
							var cbSameCount:uint=TractorDataInfo.cbTractorData[cbIndex+1];
		
							//设置索引
							cbIndex+=cbLineCount*cbSameCount+2;
						}
		
						//获取属性
						var cbLineCount:uint=TractorDataInfo.cbTractorData[cbIndex];
						var cbSameCount:uint=TractorDataInfo.cbTractorData[cbIndex+1];
		
						//提取变量
						var cbTractorCardCount:uint=cbLineCount*cbSameCount;
						var cbTakeCardCount:uint=__min(cbTractorCardCount,cbLeaveCount);
		
						//提取扑克
						OutCardResult.cbCardCount+=cbTakeCardCount;
						CopyArray(OutCardResult.cbResultCard,
							TractorDataInfo.cbTractorData,cbTakeCardCount,
							OutCardResult.cbCardCount-cbTakeCardCount, cbIndex+2+cbTractorCardCount-cbTakeCardCount);
		
						//删除扑克
						cbLeaveCount-=cbTakeCardCount;
						cbAnalyseCardCount-=cbTakeCardCount;
						RemoveCard(TractorDataInfo.cbTractorData,
							cbTakeCardCount,
							cbAnalyseCard,cbAnalyseCardCount+cbTakeCardCount,
							cbIndex+2+cbTractorCardCount-cbTakeCardCount);
		
						//设置索引
						cbIndex+=cbLineCount*cbSameCount+2;
					}
		
					//设置需求
					DemandInfo.cbSameCardCount[i-1]+=cbLeaveCount;
				}
		
				//同牌判断
				if (DemandInfo.cbSameCardCount[i-1]>0)
				{
					//提取同牌
					var SameDataInfo:tagSameDataInfo= new tagSameDataInfo;
					DistillCardByCount(cbAnalyseCard,cbAnalyseCardCount,i,SameDataInfo);
		
					//提取分析
					var cbTakeCardCount:uint=__min(SameDataInfo.cbCardCount,DemandInfo.cbSameCardCount[i-1]);
					cbTakeCardCount=cbTakeCardCount-cbTakeCardCount%i;
		
					//构造结果
					OutCardResult.cbCardCount+=cbTakeCardCount;
					CopyArray(OutCardResult.cbResultCard,
						SameDataInfo.cbSameData,cbTakeCardCount,
						OutCardResult.cbCardCount-cbTakeCardCount, SameDataInfo.cbCardCount-cbTakeCardCount);
		
					//删除扑克
					cbAnalyseCardCount-=cbTakeCardCount;
					RemoveCard(SameDataInfo.cbSameData,
					cbTakeCardCount,cbAnalyseCard,cbAnalyseCardCount+cbTakeCardCount,
					SameDataInfo.cbCardCount-cbTakeCardCount);
		
					//设置需求
					DemandInfo.cbSameCardCount[i-2]+=(DemandInfo.cbSameCardCount[i-1]-cbTakeCardCount);
				}
			}
		
			//提取单牌
			if (DemandInfo.cbSameCardCount[0]>0)
			{
				//变量定义
				var cbSourceCount:uint=OutCardResult.cbCardCount;
				var cbLeaveCount:uint=DemandInfo.cbSameCardCount[0];
		
				//构造结果
				OutCardResult.cbCardCount+=cbLeaveCount;
				CopyArray(OutCardResult.cbResultCard,
				cbAnalyseCard,
				cbLeaveCount,
				cbSourceCount,
				cbAnalyseCardCount-cbLeaveCount);
			}
		
			return true;
		}
		
		//效验出牌
		public function EfficacyOutCard(cbOutCardData:Array,
		 cbOutCardCount:uint, 
		 cbFirstCardData:Array, 
		 cbFirstCardCount:uint,
		  cbHandCardData:Array,
		   cbHandCardCount:uint):Boolean
		{
			//状态效验
			ASSERT(m_cbMainColor!=COLOR_ERROR);
			ASSERT(m_cbMainValue!=VALUE_ERROR);
		
			//数目判断
			if (cbOutCardCount!=cbFirstCardCount) return false;
		
			//扑克类型
			var cbOutCardType:uint=GetCardType(cbOutCardData,cbOutCardCount);
			var cbFirstCardType:uint=GetCardType(cbFirstCardData,cbFirstCardCount);
		
			//逻辑花色
			var cbOutLogicColor:uint=GetCardLogicColor2(cbOutCardData,cbOutCardCount);
			var cbFirstLogicColor:uint=GetCardLogicColor2(cbFirstCardData,cbFirstCardCount);
		
			//同型判断
			ASSERT(cbFirstCardType!=CT_ERROR);
			if ((cbFirstCardType!=CT_THROW_CARD)&&(cbOutLogicColor==cbFirstLogicColor)&&(cbOutCardType==cbFirstCardType)) return true;
		
			//提取扑克
			var cbDistillCardOut:Array = new Array(MAX_COUNT);
			var cbDistillCardHand:Array = new Array(MAX_COUNT);
			var cbDistillCountOut:uint=DistillCardByColor(cbOutCardData,cbOutCardCount,cbFirstLogicColor,cbDistillCardOut);
			var cbDistillCountHand:uint=DistillCardByColor(cbHandCardData,cbHandCardCount,cbFirstLogicColor,cbDistillCardHand);
		
			//花色判断
			if (cbFirstLogicColor!=cbOutLogicColor) return (cbDistillCountOut==cbDistillCountHand);
		
			//变量定义
			var SameDataInfo:Array = new Array(3);
			for(var i:uint=0; i<SameDataInfo.length; i ++)
				SameDataInfo[i] = new tagSameDataInfo;
			var TractorDataInfo:Array = new Array(3);
			for(var i:uint=0; i<TractorDataInfo.length; i ++)
				TractorDataInfo[i] = new tagTractorDataInfo;
		
			//扑克分析
			for (var i:uint=2;i>=2;i--)
			{
				//提取拖牌
				DistillTrackorByCount(cbFirstCardData,cbFirstCardCount,i,TractorDataInfo[INDEX_FIRST]);
		
				//拖牌判断
				if (TractorDataInfo[INDEX_FIRST].cbTractorCount>0)
				{
					//提取拖牌
					DistillTrackorByCount(cbDistillCardOut,cbDistillCountOut,i,TractorDataInfo[INDEX_OUT]);
					DistillTrackorByCount(cbDistillCardHand,cbDistillCountHand,i,TractorDataInfo[INDEX_HAND]);
		
					//出牌判断
					if (TractorDataInfo[INDEX_HAND].cbCardCount>TractorDataInfo[INDEX_OUT].cbCardCount)
					{
						//数目判断
						if (TractorDataInfo[INDEX_FIRST].cbCardCount==(TractorDataInfo[INDEX_OUT].cbCardCount+i))
						{
							//提取同牌
							var SameDataInfoOut:tagSameDataInfo = new tagSameDataInfo;
							DistillCardByCount(cbDistillCardOut,cbDistillCountOut,i,SameDataInfoOut);
		
							//提取拖牌
							var cbHandTractorData:Array = new Array(MAX_COUNT);
							var cbHandTractorCardCount:uint=0;
							var cbIndex:uint=0;
							for (var j:uint=0;j<TractorDataInfo[INDEX_HAND].cbTractorCount;j++)
							{
								//获取属性
								var cbLineCount:uint=TractorDataInfo[INDEX_HAND].cbTractorData[cbIndex];
								var cbSameCount:uint=TractorDataInfo[INDEX_HAND].cbTractorData[cbIndex+1];
		
								//设置扑克
								var cbSourceCount:uint=cbHandTractorCardCount;
								cbHandTractorCardCount+=cbLineCount*cbSameCount;
								CopyArray(cbHandTractorData,
								TractorDataInfo[INDEX_HAND].cbTractorData,
								cbLineCount*cbSameCount,
								cbSourceCount,cbIndex+2);
		
								//设置索引
								cbIndex+=(2+cbLineCount*cbSameCount);
							}
		
							//交集判断
							var cbIntersectionCount:uint=GetIntersectionCount(SameDataInfoOut.cbSameData,SameDataInfoOut.cbCardCount,cbHandTractorData,cbHandTractorCardCount);
							if (cbIntersectionCount<TractorDataInfo[INDEX_FIRST].cbCardCount) return false;
						}
		
						//数目判断
						if (TractorDataInfo[INDEX_FIRST].cbCardCount>(TractorDataInfo[INDEX_OUT].cbCardCount+i)) return false;
					}
				}
		
				//提取同牌
				DistillCardByCount(cbFirstCardData,cbFirstCardCount,i,SameDataInfo[INDEX_FIRST]);
		
				//同牌判断
				if (SameDataInfo[INDEX_FIRST].cbBlockCount>0)
				{
					//提取同牌
					DistillCardByCount(cbDistillCardOut,cbDistillCountOut,i,SameDataInfo[INDEX_OUT]);
					DistillCardByCount(cbDistillCardHand,cbDistillCountHand,i,SameDataInfo[INDEX_HAND]);
		
					//数目判断
					var cbBlockCountOut:uint=SameDataInfo[INDEX_OUT].cbBlockCount;
					var cbBlockCountHand:uint=SameDataInfo[INDEX_HAND].cbBlockCount;
					var cbBlockCountFirst:uint=SameDataInfo[INDEX_FIRST].cbBlockCount;
					if ((cbBlockCountFirst>cbBlockCountOut)&&(cbBlockCountHand>cbBlockCountOut)) return false;
				}
			}
		
			return true;
		}
		
		//有效判断
		public function IsValidCard( cbCardData:uint):Boolean
		{
			//获取属性
			var cbCardColor:uint=GetCardColor(cbCardData);
			var cbCardValue:uint=GetCardValue(cbCardData);
		
			//有效判断
			if ((cbCardData==0x4E)||(cbCardData==0x4F)) return true;
			if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) return true;
		
			return false;
		}
		
		//排序等级
		 public function GetCardSortOrder(cbCardData:uint):uint
		{
			//逻辑数值
			var cbCardColor:uint=GetCardColor(cbCardData);
			var cbCardValue:uint=GetCardValue(cbCardData);
		
			//鬼牌过滤
			if ((cbCardData==0x4E)||(cbCardData==0x4F))
			{
				var cbSortValue:uint=cbCardValue+11;
				return cbSortValue+m_cbSortRight[4];
			}
		
			//常主过滤
			if (cbCardValue==m_cbNTValue)
			{
				var cbSortValue:uint=(cbCardColor>>4)+15;
				if (cbCardColor==m_cbMainColor) cbSortValue=19;
				return cbSortValue+m_cbSortRight[4];
			}
		
			//花色变换
			var cbSortColor:uint=cbCardColor;
			if ((cbCardColor==m_cbMainColor)||(cbCardValue==m_cbMainValue)) cbSortColor=0x40;
		
			//数值变换
			var cbSortValue:uint=cbCardValue;
			if (cbCardValue==1) cbSortValue+=13;
		
			//主牌变换
			if (cbCardValue==m_cbMainValue)
			{
				if (cbCardColor==m_cbMainColor) cbSortValue=24;
				else cbSortValue=(cbCardColor>>4)+20;
			}
		
			return cbSortValue+m_cbSortRight[cbSortColor>>4];
		}
		
		//逻辑数值
		public function GetCardLogicValue(cbCardData:uint):uint
		{
			//逻辑数值
			var cbCardColor:uint=GetCardColor(cbCardData);
			var cbCardValue:uint=GetCardValue(cbCardData);
			var cbNTRight:uint=((m_cbNTValue==VALUE_ERROR)||(m_cbNTValue==m_cbMainValue))?0:2;
		
			//鬼牌过滤
			if ((cbCardData==0x4E)||(cbCardData==0x4F))
			{
				var cbLogicValue:uint=cbCardValue+3+cbNTRight;
				var cbDecreaseOrder:uint=(m_cbMainColor==COLOR_NT)?1:0;
				return cbLogicValue+m_cbSortRight[4]-cbDecreaseOrder;
			}
		
			//常主过滤
			if (cbCardValue==m_cbNTValue)
			{
				var cbLogicValue:uint=(cbCardColor==m_cbMainColor)?16:15;
				return cbLogicValue+m_cbSortRight[4];
			}
		
			//花色变换
			var cbLogicColor:uint=cbCardColor;
			if ((cbCardColor==m_cbMainColor)||(cbCardValue==m_cbMainValue)) cbLogicColor=0x40;
		
			//转换变换
			var cbLogicValue:uint=cbCardValue;
			if (cbCardValue==1) cbLogicValue+=13;
		
			//主牌变换
			if (cbCardValue==m_cbMainValue)
			{
				if (cbCardColor==m_cbMainColor) cbLogicValue=16+cbNTRight;
				else cbLogicValue=15+cbNTRight;
			}
		
			//紧凑处理
			if ((cbCardValue!=1)&&(cbCardValue!=m_cbMainValue)&&(cbCardValue!=m_cbNTValue))
			{
				if (cbCardValue<m_cbMainValue) cbLogicValue++;
				if ((m_cbNTValue!=VALUE_ERROR)&&(m_cbNTValue!=m_cbMainValue)&&(cbCardValue<m_cbNTValue)) cbLogicValue++;
			}
		
			return cbLogicValue+m_cbSortRight[cbLogicColor>>4];
		}
		
		//逻辑花色
		public function GetCardLogicColor1(cbCardData:uint):uint
		{
			//逻辑数值
			var cbCardColor:uint=GetCardColor(cbCardData);
			var cbCardValue:uint=GetCardValue(cbCardData);
		
			//花色判断
			if (cbCardColor==COLOR_NT) return COLOR_NT;
			if (cbCardColor==m_cbMainColor) return COLOR_NT;
		
			//数值判断
			if (cbCardValue==m_cbNTValue) return COLOR_NT;
			if (cbCardValue==m_cbMainValue) return COLOR_NT;
		
			return cbCardColor;
		}
		
		//牌列花色
		public function GetCardLogicColor2(cbCardData:Array, cbCardCount:uint):uint
		{
			//获取花色
			var cbFirstColor:uint=GetCardLogicColor1(cbCardData[0]);
			var cbSecondColor:uint=GetCardLogicColor1(cbCardData[cbCardCount-1]);
		
			return (cbFirstColor==cbSecondColor)?cbFirstColor:COLOR_ERROR;
		}
		
		//是否相连
		public function IsLineValue(cbFirstCard:uint, cbSecondCard:uint):Boolean
		{
			//获取数值
			var cbFirstLogicValue:uint=GetCardLogicValue(cbFirstCard);
			var cbSecondLogicValue:uint=GetCardLogicValue(cbSecondCard);
		
			//相连判断
			return (cbFirstLogicValue==(cbSecondLogicValue+1));
		}
		
		//是否同花
		public function IsSameColor(cbFirstCard:uint, cbSecondCard:uint):Boolean
		{
			//获取花色
			var cbFirstLogicColor:uint=GetCardLogicColor1(cbFirstCard);
			var cbSecondLogicColor:uint=GetCardLogicColor1(cbSecondCard);
		
			//同花判断
			return (cbFirstLogicColor==cbSecondLogicColor);
		}
		
		//提取扑克
		public function DistillCardByColor(cbCardData:Array,
		 cbCardCount:uint, cbCardColor:uint,  cbResultCard:Array):uint
		{
			//变量定义
			var cbResultCount:uint=0;
		
			//提取扑克
			for (var i:uint=0;i<cbCardCount;i++)
			{
				//花色判断
				if (GetCardLogicColor1(cbCardData[i])==cbCardColor) 
				{
					var tmp:uint = cbResultCount;
					cbResultCard[tmp]=cbCardData[i];
					cbResultCount++;
					continue;
				}
		
				//终止判断
				if (cbResultCount>0) break;
			}
		
			return cbResultCount;
		}
		
		//提取扑克
		public function DistillCardByCount(cbCardData:Array, 
		cbCardCount:uint, cbSameCount:uint,
		SameDataInfo:tagSameDataInfo):uint
		{
			var j:uint = 0;
			//参数效验
			ASSERT((cbSameCount>=2)&&(cbSameCount<=2));
			if ((cbSameCount<2)||(cbSameCount>2))
				return 0;
		
			//设置变量
			tagSameDataInfo.ZeroData(SameDataInfo);
		
			//提取判断
			if (cbCardCount<cbSameCount) return 0;
		
			//提取扑克
			for (var i:uint=0;i<cbCardCount-cbSameCount+1;i++)
			{
				//数目判断
				for (j=1;j<cbSameCount;j++)
				{
					if (cbCardData[i]!=cbCardData[i+j]) break;
				}
		
				//成功处理
				if (j==cbSameCount)
				{
					//同牌属性
					SameDataInfo.cbBlockCount++;
		
					//扑克数据
					for (var k:uint=0;k<cbSameCount;k++)
					{
						SameDataInfo.cbSameData[SameDataInfo.cbCardCount++]=cbCardData[i+k];
					}
				}
		
				//递增处理
				i+=j-1;
			}
		
			return SameDataInfo.cbBlockCount;
		}
		
		//提取扑克
		public function DistillTrackorByCount(cbCardData:Array,
		cbCardCount:uint,cbSameCount:uint, TractorDataInfo:tagTractorDataInfo):uint
		{
			//参数效验
			ASSERT((cbSameCount>=2)&&(cbSameCount<=2));
			if ((cbSameCount<2)||(cbSameCount>2)) return 0;
		
			//设置变量
			tagTractorDataInfo.ZeroData(TractorDataInfo);
		
			//提取同牌
			var SameDataInfo:tagSameDataInfo = new tagSameDataInfo;
			DistillCardByCount(cbCardData,cbCardCount,cbSameCount,SameDataInfo);
		
			//牌段判断
			if (SameDataInfo.cbBlockCount<2) return 0;
		
			//分析处理
			var cbResultIndex:uint=0;
			for (var i:uint=0;i<SameDataInfo.cbBlockCount-1;i++)
			{
				//分析过滤
				if (SameDataInfo.cbSameData[i*cbSameCount]==0) continue;
		
				//变量定义
				var cbLineCount:uint=1;
				var cbTrackorIndex:Array = new Array(MAX_TRACKOR);
				ZeroArray(cbTrackorIndex,i*cbSameCount);
		
				//连牌判断
				for (var j:uint=i;j<SameDataInfo.cbBlockCount-1;j++)
				{
					//分析过滤
					if (SameDataInfo.cbSameData[(j+1)*cbSameCount]==0) continue;
		
					//变量定义
					var cbSecondIndex:uint=(j+1)*cbSameCount;
					var cbFirstIndex:uint=cbTrackorIndex[cbLineCount-1];
					var cbFirstCardData:uint=SameDataInfo.cbSameData[cbFirstIndex];
					var cbSecondCardData:uint=SameDataInfo.cbSameData[cbSecondIndex];
		
					//连牌判断
					if (IsLineValue(cbFirstCardData,cbSecondCardData)==true)
					{
						ASSERT(cbLineCount<MAX_TRACKOR);
						var tmp:uint = cbLineCount;
						cbTrackorIndex[tmp]=cbSecondIndex;
						cbLineCount++;
						continue;
					}
		
					//副主间隔
					if (cbFirstCardData==cbSecondCardData) continue;
					if (GetCardValue(cbSecondCardData)==m_cbNTValue) continue;
					if (GetCardValue(cbSecondCardData)==m_cbMainValue) continue;
		
					break;
				}
		
				//成功判断
				if (cbLineCount>=2)
				{
					//最大长度
					if (cbLineCount>TractorDataInfo.cbTractorMaxLength)
					{
						TractorDataInfo.cbTractorMaxLength=cbLineCount;
						TractorDataInfo.cbTractorMaxIndex=cbResultIndex;
					}
		
					//设置结果
					TractorDataInfo.cbTractorCount++;
					TractorDataInfo.cbCardCount+=cbLineCount*cbSameCount;
					TractorDataInfo.cbTractorData[cbResultIndex++]=cbLineCount;
					TractorDataInfo.cbTractorData[cbResultIndex++]=cbSameCount;
		
					//设置数据
					for (var k:uint=0;k<cbLineCount;k++)
					{
						var cbIndex:uint=cbTrackorIndex[k];
						for (var l:uint=0;l<cbSameCount;l++)
						{
							TractorDataInfo.cbTractorData[cbResultIndex++]=SameDataInfo.cbSameData[cbIndex+l];
							SameDataInfo.cbSameData[cbIndex+l]=0;
						}
					}
				}
			}
		
			return TractorDataInfo.cbTractorCount;
		}
		
		//更新权位
		public function UpdateSortRight():void
		{
			//设置权位
			for (var i:uint=0;i<CountArray(m_cbSortRight);i++) m_cbSortRight[i]=i*COLOR_RIGHT;
		
			//主牌权位
			if ((m_cbMainColor!=COLOR_ERROR)&&(m_cbMainColor!=COLOR_NT)) m_cbSortRight[m_cbMainColor>>4]=4*COLOR_RIGHT;
		
			//调整权位
			switch (m_cbMainColor)
			{
			case COLOR_MEI_HUA:		//梅花
				{
					m_cbSortRight[COLOR_HEI_TAO>>4]=COLOR_RIGHT;
					break;
				}
			case COLOR_HONG_TAO:	//红桃
				{
					m_cbSortRight[COLOR_FANG_KUAI>>4]=2*COLOR_RIGHT;
					break;
				}
			}
		
			return;
		}
		
		//调整扑克
		public function RectifyCardWeave(cbCardData:Array,
		cbCardCount:uint, 
		TargetResult:tagAnalyseResult, 
		RectifyResult:tagAnalyseResult):Boolean
		{
			//设置变量
			tagAnalyseResult.ZeroData(RectifyResult);
		
			//花色判断
			RectifyResult.cbCardColor=GetCardLogicColor2(cbCardData,cbCardCount);
			if (RectifyResult.cbCardColor==COLOR_ERROR) return false;
		
			//构造扑克
			var cbTempCardData:Array = new Array(MAX_COUNT);
			var cbTempCardCount:uint=cbCardCount;
			CopyArray(cbTempCardData,cbCardData,cbCardCount);
		
			//循环调整
			for (var i:uint=2;i>=2;i--)
			{
				//调整变量
				var pSameDataInfoRectify:tagSameDataInfo=RectifyResult.SameDataInfo[i-1];
				var pTractorDataInfoRectify:tagTractorDataInfo=RectifyResult.TractorDataInfo[i-2];
		
				//目标变量
				var pSameDataInfoTarget:tagSameDataInfo=TargetResult.SameDataInfo[i-1];
				var pTractorDataInfoTarget:tagTractorDataInfo=TargetResult.TractorDataInfo[i-2];
		
				//调整拖牌
				if (pTractorDataInfoTarget.cbTractorCount>0)
				{
					//提取拖牌
					var TractorDataInfo:tagTractorDataInfo = new tagTractorDataInfo;
					DistillTrackorByCount(cbTempCardData,cbTempCardCount,i,TractorDataInfo);
		
					//数据判断
					if (pTractorDataInfoTarget.cbCardCount>TractorDataInfo.cbCardCount) return false;
					if (pTractorDataInfoTarget.cbTractorMaxLength>TractorDataInfo.cbTractorMaxLength) return false;
		
					//变量定义
					var cbCollectCount:uint=0;
					var cbCollectCardData:Array = new Array(MAX_COUNT);
		
					//设置属性
					pTractorDataInfoRectify.cbTractorMaxIndex=255;
					pTractorDataInfoRectify.cbTractorMaxLength=pTractorDataInfoTarget.cbTractorMaxLength;
		
					//设置结果
					var cbIndex:uint=0;
					for (var j:uint=0;j<TractorDataInfo.cbTractorCount;j++)
					{
						//变量定义
						var cbRectifyLineCount:uint=TractorDataInfo.cbTractorData[cbIndex];
						var cbResidualLineCount:uint=(pTractorDataInfoTarget.cbCardCount-cbCollectCount)/i;
		
						//属性定义
						var cbSameCount:uint=i;
						var cbLineCount:uint=__min(cbRectifyLineCount,cbResidualLineCount);
		
						//设置属性
						pTractorDataInfoRectify.cbTractorCount++;
						pTractorDataInfoRectify.cbCardCount+=cbLineCount*cbSameCount;
		
						//设置属性
						pTractorDataInfoRectify.cbTractorData[cbIndex]=cbLineCount;
						pTractorDataInfoRectify.cbTractorData[cbIndex+1]=cbSameCount;
		
						//设置扑克
						var cbInsertCount:uint=cbLineCount*cbSameCount;
						CopyArray(cbCollectCardData,
						TractorDataInfo.cbTractorData,
						cbInsertCount,cbCollectCount,cbIndex+2);
						
						CopyArray(pTractorDataInfoRectify.cbTractorData,
						TractorDataInfo.cbTractorData,cbInsertCount,
						cbIndex+2,
						cbIndex+2);
		
						//设置变量
						cbIndex+=(cbInsertCount+2);
						cbCollectCount+=cbInsertCount;
		
						//完成判断
						if (cbCollectCount>=pTractorDataInfoTarget.cbCardCount) break;
					}
		
					//删除扑克
					cbTempCardCount-=cbCollectCount;
					RemoveCard(cbCollectCardData,cbCollectCount,cbTempCardData,cbTempCardCount+cbCollectCount);
				}
		
				//调整同牌
				if (pSameDataInfoTarget.cbCardCount>0)
				{
					//提取同牌
					var SameDataInfo:tagSameDataInfo = new tagSameDataInfo;
					DistillCardByCount(cbTempCardData,cbTempCardCount,i,SameDataInfo);
		
					//数据判断
					if (pSameDataInfoTarget.cbCardCount>SameDataInfo.cbCardCount) return false;
					if (pSameDataInfoTarget.cbBlockCount>SameDataInfo.cbBlockCount) return false;
		
					//设置扑克
					pSameDataInfoRectify.cbCardCount=pSameDataInfoTarget.cbCardCount;
					pSameDataInfoRectify.cbBlockCount=pSameDataInfoTarget.cbBlockCount;
					CopyArray(pSameDataInfoRectify.cbSameData,SameDataInfo.cbSameData,pSameDataInfoRectify.cbCardCount);
		
					//变量定义
					var cbRemoveCount:uint=pSameDataInfoRectify.cbCardCount;
					var pcbRemoveCard:Array=pSameDataInfoRectify.cbSameData[0];
		
					//删除扑克
					cbTempCardCount-=cbRemoveCount;
					RemoveCard(pcbRemoveCard,
							   cbRemoveCount,
							   cbTempCardData,
							   cbTempCardCount+cbRemoveCount);
				}
			}
		
			//设置单牌
			if (cbTempCardCount>0)
			{
				RectifyResult.SameDataInfo[0].cbCardCount=cbTempCardCount;
				RectifyResult.SameDataInfo[0].cbBlockCount=cbTempCardCount;
				CopyArray(RectifyResult.SameDataInfo[0].cbSameData,
													   cbTempCardData,
													   cbTempCardCount);
			}
		
			return true;
		}
		
		//交集数目
		public function GetIntersectionCount(cbCardData1:Array, cbCardCount1:uint, cbCardData2:Array, cbCardCount2:uint):uint
		{
			//变量定义
			var cbAnalyseCard:Array = new Array(MAX_COUNT);
			CopyArray(cbAnalyseCard,cbCardData2,cbCardCount2);
		
			//分析判断
			var cbIntersectionCount:uint=0;
			for (var i:uint=0;i<cbCardCount1;i++)
			{
				for (var j:uint=0;j<cbCardCount2;j++)
				{
					if (cbCardData1[i]==cbAnalyseCard[j])
					{
						//设置变量
						cbAnalyseCard[j]=0;
						cbIntersectionCount++;
		
						break;
					}
				}
			}
		
			return cbIntersectionCount;
		}

		
	}
}
include "./CMD_Upgrade.as"
include "./GameLogicDef.as"
include "../../../Common/MathFunction.as"
include "../../../Common/Memory.as"