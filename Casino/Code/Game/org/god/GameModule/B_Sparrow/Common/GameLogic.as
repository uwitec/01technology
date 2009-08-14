package org.god.GameModule.B_Sparrow.Common
{
	//游戏逻辑类
	public class GameLogic
	{
		public static  var m_cbSparrowDataArray:Array;//麻将数据

		public function GameLogic()
		{
			if (m_cbSparrowDataArray == null)
			{
				m_cbSparrowDataArray=new Array(
				0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//万子
				0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//万子
				0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//万子
				0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//万子
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,//索子
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,//索子
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,//索子
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,//索子
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,//同子
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,//同子
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,//同子
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29);//同子
			}

		}
		
				
		//混乱麻将
		public function RandSparrowData(cbSparrowData:Array,  cbMaxCount:uint):void
		{
			//混乱准备
			var cbSparrowDataTemp:Array = new Array(CountArray(m_cbSparrowDataArray));
			CopyArray(cbSparrowDataTemp,m_cbSparrowDataArray,CountArray(m_cbSparrowDataArray));
		
			//混乱麻将
			var cbRandCount:uint=0;
			var cbPosition:uint=0;
			do
			{
				cbPosition=randUInt()%(cbMaxCount-cbRandCount);
				cbSparrowData[cbRandCount++]=cbSparrowDataTemp[cbPosition];
				cbSparrowDataTemp[cbPosition]=cbSparrowDataTemp[cbMaxCount-cbRandCount];
			} while (cbRandCount<cbMaxCount);
		
			return;
		}
		
		//删除麻将
		public function RemoveSparrow2(cbSparrowIndex:Array/*[MAX_INDEX]*/, cbRemoveSparrow:uint):Boolean
		{
			//效验麻将
		
			//删除麻将
			var cbRemoveIndex:uint=SwitchToSparrowIndex1(cbRemoveSparrow);
			if (cbSparrowIndex[cbRemoveIndex]>0)
			{
				cbSparrowIndex[cbRemoveIndex]--;
				return true;
			}
		
			//失败效验
		
		
			return false;
		}
		
		//删除麻将
		public function RemoveSparrow3(cbSparrowIndex:Array/*[MAX_INDEX]*/,cbRemoveSparrow:Array, cbRemoveCount:uint):Boolean
		{
			//删除麻将
			for (var i:uint=0;i<cbRemoveCount;i++)
			{
				//效验麻将
				ASSERT(IsValidSparrow(cbRemoveSparrow[i]));
				ASSERT(cbSparrowIndex[SwitchToSparrowIndex1(cbRemoveSparrow[i])]>0);
		
				//删除麻将
				var cbRemoveIndex:uint=SwitchToSparrowIndex1(cbRemoveSparrow[i]);
				if (cbSparrowIndex[cbRemoveIndex]==0)
				{
					//错误断言
					ASSERT(false);
		
					//还原删除
					for (var j:uint=0;j<i;j++) 
					{
						ASSERT(IsValidSparrow(cbRemoveSparrow[j]));
						cbSparrowIndex[SwitchToSparrowIndex1(cbRemoveSparrow[j])]++;
					}
		
					return false;
				}
				else 
				{
					//删除麻将
					--cbSparrowIndex[cbRemoveIndex];
				}
			}
		
			return true;
		}
		
		//删除麻将
		public function RemoveSparrow4(cbSparrowData:Array, cbSparrowCount:uint, cbRemoveSparrow:Array, cbRemoveCount:uint):Boolean
		{
			//检验数据
			ASSERT(cbSparrowCount<=14);
			ASSERT(cbRemoveCount<=cbSparrowCount);
		
			//定义变量
			var cbDeleteCount:uint=0;
			var cbTempSparrowData:Array = new Array(14);
			if (cbSparrowCount>CountArray(cbTempSparrowData)) return false;
			CopyArray(cbTempSparrowData,cbSparrowData,cbSparrowCount);
		
			//置零麻将
			for (var i:uint=0;i<cbRemoveCount;i++)
			{
				for (var j:uint=0;j<cbSparrowCount;j++)
				{
					if (cbRemoveSparrow[i]==cbTempSparrowData[j])
					{
						cbDeleteCount++;
						cbTempSparrowData[j]=0;
						break;
					}
				}
			}
		
			//成功判断
			if (cbDeleteCount!=cbRemoveCount) 
			{
				ASSERT(false);
				return false;
			}
		
			//清理麻将
			var cbSparrowPos:uint=0;
			for (var i:uint=0;i<cbSparrowCount;i++)
			{
				if (cbTempSparrowData[i]!=0) cbSparrowData[cbSparrowPos++]=cbTempSparrowData[i];
			}
		
			return true;
		}
		
		//有效判断
		public function IsValidSparrow( cbSparrowData:uint):Boolean
		{
			var cbValue:uint=(cbSparrowData&MASK_VALUE);
			var cbColor:uint=(cbSparrowData&MASK_COLOR)>>4;
			return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
		}
		
		//麻将数目
		public function GetSparrowCount(cbSparrowIndex:Array/*[MAX_INDEX]*/):uint
		{
			//数目统计
			var cbSparrowCount:uint=0;
			for (var i:uint=0;i<MAX_INDEX;i++) cbSparrowCount+=cbSparrowIndex[i];
		
			return cbSparrowCount;
		}
		
		//获取组合
		public function GetWeaveSparrow( cbWeaveKind:uint, cbCenterSparrow:uint, cbSparrowBuffer:Array/*[4]*/):uint
		{
			//组合麻将
			switch (cbWeaveKind)
			{
			case WIK_LEFT:		//上牌操作
				{
					//设置变量
					cbSparrowBuffer[0]=cbCenterSparrow+1;
					cbSparrowBuffer[1]=cbCenterSparrow+2;
					cbSparrowBuffer[2]=cbCenterSparrow;
		
					return 3;
				}
			case WIK_RIGHT:		//上牌操作
				{
					//设置变量
					cbSparrowBuffer[0]=cbCenterSparrow-2;
					cbSparrowBuffer[1]=cbCenterSparrow-1;
					cbSparrowBuffer[2]=cbCenterSparrow;
		
					return 3;
				}
			case WIK_CENTER:	//上牌操作
				{
					//设置变量
					cbSparrowBuffer[0]=cbCenterSparrow-1;
					cbSparrowBuffer[1]=cbCenterSparrow;
					cbSparrowBuffer[2]=cbCenterSparrow+1;
		
					return 3;
				}
			case WIK_PENG:		//碰牌操作
				{
					//设置变量
					cbSparrowBuffer[0]=cbCenterSparrow;
					cbSparrowBuffer[1]=cbCenterSparrow;
					cbSparrowBuffer[2]=cbCenterSparrow;
		
					return 3;
				}
			case WIK_FILL:		//补牌操作
				{
					//设置变量
					cbSparrowBuffer[0]=cbCenterSparrow;
					cbSparrowBuffer[1]=cbCenterSparrow;
					cbSparrowBuffer[2]=cbCenterSparrow;
					cbSparrowBuffer[3]=cbCenterSparrow;
		
					return 4;
				}
			case WIK_GANG:		//杠牌操作
				{
					//设置变量
					cbSparrowBuffer[0]=cbCenterSparrow;
					cbSparrowBuffer[1]=cbCenterSparrow;
					cbSparrowBuffer[2]=cbCenterSparrow;
					cbSparrowBuffer[3]=cbCenterSparrow;
		
					return 4;
				}
			default:
				{
					ASSERT(false);
				}
			}
		
			return 0;
		}
		
		//动作等级
		public function GetUserActionRank(cbUserAction:uint):uint
		{
			//胡牌等级
			if (cbUserAction&WIK_CHI_HU) { return 4; }
		
			//杠牌等级
			if (cbUserAction&(WIK_FILL|WIK_GANG)) { return 3; }
		
			//碰牌等级
			if (cbUserAction&WIK_PENG) { return 2; }
		
			//上牌等级
			if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }
		
			return 0;
		}
		
		//胡牌等级
		public function GetChiHuActionRank(ChiHuResult:tagChiHuResult):uint
		{
			//变量定义
			var cbChiHuOrder:uint=0;
			var wChiHuRight:uint=ChiHuResult.wChiHuRight;
			var wChiHuKind:uint=(ChiHuResult.wChiHuKind&0xFF00)>>4;
		
			//大胡升级
			for (var i:uint=0;i<8;i++)
			{
				wChiHuKind>>=1;
				if ((wChiHuKind&0x0001)!=0) cbChiHuOrder++;
			}
		
			//权位升级
			for ( i=0;i<16;i++)
			{
				wChiHuRight>>=1;
				if ((wChiHuRight&0x0001)!=0) cbChiHuOrder++;
			}
		
			return cbChiHuOrder;
		}
		
		//吃胡判断
		public function EstimateChiHu( cbSparrowIndex:Array/*[MAX_INDEX])/):uint
		{
			//变量定义
			var cbActionMask:uint=WIK_NULL;
		
			//特殊胡牌
			if (IsSiXi(cbSparrowIndex)==true) cbActionMask|=WIK_CHI_HU;
			else if (IsQueYiSe(cbSparrowIndex)==true) cbActionMask|=WIK_CHI_HU;
			else if (IsBanBanHu(cbSparrowIndex)==true) cbActionMask|=WIK_CHI_HU;
			else if (IsLiuLiuShun(cbSparrowIndex)==true) cbActionMask|=WIK_CHI_HU;
		
			return cbActionMask;
		}
		
		//吃牌判断
		public function EstimateEatSparrow( cbSparrowIndex:Array/*[MAX_INDEX]*/, cbCurrentSparrow:uint):uint
		{
			//参数效验
			ASSERT(IsValidSparrow(cbCurrentSparrow));
		
			//过滤判断
			if (cbCurrentSparrow>=0x31) return WIK_NULL;
		
			//变量定义
			var cbExcursion:Array=new Array(0,1,2);
			var cbItemKind:Array=new Array(WIK_LEFT,WIK_CENTER,WIK_RIGHT);
		
			//吃牌判断
			var cbEatKind:uint=0;
			var cbFirstIndex:uint=0;
			var cbCurrentIndex:uint=SwitchToSparrowIndex1(cbCurrentSparrow);
			for (var i:uint=0;i<CountArray(cbItemKind);i++)
			{
				var cbValueIndex:uint=cbCurrentIndex%9;
				if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
				{
					//吃牌判断
					cbFirstIndex=cbCurrentIndex-cbExcursion[i];
					if ((cbCurrentIndex!=cbFirstIndex)&&(cbSparrowIndex[cbFirstIndex]==0)) continue;
					if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbSparrowIndex[cbFirstIndex+1]==0)) continue;
					if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbSparrowIndex[cbFirstIndex+2]==0)) continue;
		
					//设置类型
					cbEatKind|=cbItemKind[i];
				}
			}
		
			return cbEatKind;
		}
		
		//碰牌判断
		public function EstimatePengSparrow( cbSparrowIndex:Array/*[MAX_INDEX]*/, cbCurrentSparrow:uint):uint
		{
			//参数效验
			ASSERT(IsValidSparrow(cbCurrentSparrow));
		
			//碰牌判断
			return (cbSparrowIndex[SwitchToSparrowIndex1(cbCurrentSparrow)]>=2)?WIK_PENG:WIK_NULL;
		}
		
		//杠牌判断
		public function EstimateGangSparrow(cbSparrowIndex:Array/*[MAX_INDEX]*/, cbCurrentSparrow:uint):uint
		{
			//参数效验
			ASSERT(IsValidSparrow(cbCurrentSparrow));
		
			//杠牌判断
			return (cbSparrowIndex[SwitchToSparrowIndex1(cbCurrentSparrow)]==3)?(WIK_GANG|WIK_FILL):WIK_NULL;
		}
		
		//杠牌分析
		public function AnalyseGangSparrow(cbSparrowIndex:Array/*[MAX_INDEX]*/, 
		 WeaveItem:Array, cbWeaveCount:uint, 
		 GangSparrowResult:tagGangSparrowResult):uint
		{
			//设置变量
			var cbActionMask:uint=WIK_NULL;
			//ZeroArray(GangSparrowResult,null);
		
			//手上杠牌
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				if (cbSparrowIndex[i]==4)
				{
					cbActionMask|=(WIK_GANG|WIK_FILL);
					GangSparrowResult.cbSparrowData[GangSparrowResult.cbSparrowCount]=(WIK_GANG|WIK_FILL);
					GangSparrowResult.cbSparrowData[GangSparrowResult.cbSparrowCount++]=SwitchToSparrowData1(i);
				}
			}
		
			//组合杠牌
			for (i=0;i<cbWeaveCount;i++)
			{
				if (WeaveItem[i].cbWeaveKind==WIK_PENG)
				{
					if (cbSparrowIndex[SwitchToSparrowIndex1(WeaveItem[i].cbCenterSparrow)]==1)
					{
						cbActionMask|=(WIK_GANG|WIK_FILL);
						GangSparrowResult.cbSparrowData[GangSparrowResult.cbSparrowCount]=(WIK_GANG|WIK_FILL);
						GangSparrowResult.cbSparrowData[GangSparrowResult.cbSparrowCount++]=WeaveItem[i].cbCenterSparrow;
					}
				}
			}
		
			return cbActionMask;
		}
		
		//吃胡分析
		public function AnalyseChiHuSparrow(cbSparrowIndex:Array/*[MAX_INDEX]*/,
		  WeaveItem:Array, cbWeaveCount:uint, cbCurrentSparrow:uint, wChiHuRight:uint,
		 ChiHuResult:tagChiHuResult):uint
		{
			//变量定义
			var wChiHuKind:uint=CHK_NULL;
			var AnalyseItemArray:Array = new Array;
			
		
			//构造麻将
			var cbSparrowIndexTemp:Array = new Array(MAX_INDEX);
			CopyArray(cbSparrowIndexTemp,cbSparrowIndex,cbSparrowIndexTemp.length);
		
			//插入麻将
			if (cbCurrentSparrow!=0) cbSparrowIndexTemp[SwitchToSparrowIndex1(cbCurrentSparrow)]++;
		
			//权位处理
			if ((cbCurrentSparrow!=0)&&(cbWeaveCount==4)) wChiHuRight|=CHR_QUAN_QIU_REN;
			if (IsQingYiSe(cbSparrowIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHR_QING_YI_SE;
		
			//牌型判断
			if (IsQiXiaoDui(cbSparrowIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_QI_XIAO_DUI;
			if (IsHaoHuaDui(cbSparrowIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_HAO_HUA_DUI;
			if (IsJiangJiangHu(cbSparrowIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_JIANG_JIANG;
		
			//分析麻将
			AnalyseSparrow(cbSparrowIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);
		
			//胡牌分析
			if (AnalyseItemArray.GetCount()>0)
			{
				//眼牌需求
				var bNeedSymbol:Boolean=((wChiHuRight&0xFF00)==0);
		
				//牌型分析
				for (var i:uint=0;i<AnalyseItemArray.length;i++)
				{
					//变量定义
					var bLianSparrow:Boolean=false;
					var bPengSparrow:Boolean=false;
					var pAnalyseItem:tagAnalyseItem=AnalyseItemArray[i];
		
					//牌眼类型
					var cbEyeValue:uint=pAnalyseItem.cbSparrowEye&MASK_VALUE;
					var bSymbolEye:Boolean=true;//((cbEyeValue==2)||(cbEyeValue==5)||(cbEyeValue==8));
		
					//牌型分析
					for (var j:uint=0;j<CountArray(pAnalyseItem.cbWeaveKind);j++)
					{
						var cbWeaveKind:uint=pAnalyseItem.cbWeaveKind[j];
						bPengSparrow=((cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_PENG))!=0)?true:bPengSparrow;
						bLianSparrow=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianSparrow;
					}
		
					//牌型判断
					ASSERT((bLianSparrow==true)||(bPengSparrow==true));
		
					//碰碰牌型
					if ((bLianSparrow==false)&&(bPengSparrow==true)) wChiHuKind|=CHK_PENG_PENG;
					if ((bLianSparrow==true)&&(bPengSparrow==true)&&((bSymbolEye==true)||(bNeedSymbol==false))) wChiHuKind|=CHK_JI_HU;
					if ((bLianSparrow==true)&&(bPengSparrow==false)&&((bSymbolEye==true)||(bNeedSymbol==false))) wChiHuKind|=CHK_PING_HU;
				}
			}
		
			//结果判断
			if (wChiHuKind!=CHK_NULL)
			{
				//设置结果
				ChiHuResult.wChiHuKind=wChiHuKind;
				ChiHuResult.wChiHuRight=wChiHuRight;
		
				return WIK_CHI_HU;
			}
		
			return WIK_NULL;
		}
		
		//四喜胡牌
		public function IsSiXi(cbSparrowIndex:Array/*[MAX_INDEX]*/):Boolean
		{
			//胡牌判断
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				if (cbSparrowIndex[i]==4) return true;
			}
		
			return false;
		}
		
		//缺一色牌
		public function IsQueYiSe(cbSparrowIndex:Array/*[MAX_INDEX]*/):Boolean
		{
			//胡牌判断
			var cbIndex:Array=new Array(0,9,18);
			var i:uint = 0;
			var j:uint = 0;
			for (i=0;i<CountArray(cbIndex);i++)
			{
				for (j=cbIndex[i];j<(cbIndex[i]+9);j++)
				{
					if (cbSparrowIndex[j]!=0) break;
				}
				if (j==(cbIndex[i]+9)) return true;
			}
		
			return false;
		}
		
		//板板胡牌
		public function IsBanBanHu(cbSparrowIndex:Array/*[MAX_INDEX]*/):Boolean
		{
			//胡牌判断
			for (var i:uint=1;i<MAX_INDEX;i+=3) if (cbSparrowIndex[i]!=0) return false;
		
			return true;
		}
		
		//六六顺牌
		public function IsLiuLiuShun(cbSparrowIndex:Array/*[MAX_INDEX]*/):Boolean
		{
			//胡牌判断
			var cbPengCount:uint=0;
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				if ((cbSparrowIndex[i]>=3)&&(++cbPengCount>=2)) return true;
			}
		
			return false;
		}
		
		//清一色牌
		public function IsQingYiSe(cbSparrowIndex:Array/*[MAX_INDEX]*/,  WeaveItem:Array,  cbItemCount:uint):Boolean
		{
			//胡牌判断
			var cbSparrowColor:uint=0xFF;
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				if (cbSparrowIndex[i]!=0)
				{
					//花色判断
					if (cbSparrowColor!=0xFF) return false;
		
					//设置花色
					cbSparrowColor=(SwitchToSparrowData1(i)&MASK_COLOR);
		
					//设置索引
					i=(i/9+1)*9;
				}
			}
		
			//组合判断
			for ( i=0;i<cbItemCount;i++)
			{
				var cbCenterSparrow:uint=WeaveItem[i].cbCenterSparrow;
				if ((cbCenterSparrow&MASK_COLOR)!=cbSparrowColor) return false;
			}
		
			return true;
		}
		
		//七小对牌
		public function IsQiXiaoDui( cbSparrowIndex:Array/*[MAX_INDEX]*/,  WeaveItem:Array,  cbWeaveCount:uint):Boolean
		{
			//组合判断
			if (cbWeaveCount!=0) return false;
		
			//麻将判断
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				var cbSparrowCount:uint=cbSparrowIndex[i];
				if ((cbSparrowCount!=0)&&(cbSparrowCount!=2)&&(cbSparrowCount!=4)) return false;
			}
		
			return true;
		}
		
		//豪华对牌
		public function IsHaoHuaDui( cbSparrowIndex:Array/*[MAX_INDEX]*/,  WeaveItem:Array,  cbItemCount:uint):Boolean
		{
			//变量定义
			var bFourSparrow:Boolean=false;
		
			//组合判断
			for (var i:uint=0;i<cbItemCount;i++)
			{
				//杆补判断
				if (WeaveItem[i].cbWeaveKind!=WIK_FILL) return false;
				if (WeaveItem[i].cbWeaveKind!=WIK_GANG) return false;
		
				//设置变量
				bFourSparrow=true;
			}
		
			//麻将判断
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				//四牌判断
				if (cbSparrowIndex[i]==4)
				{
					bFourSparrow=true;
					continue;
				}
		
				//对牌判断
				if ((cbSparrowIndex[i]!=0)&&(cbSparrowIndex[i]!=2)) return false;
			}
		
			//结果判断
			if (bFourSparrow==false) return false;
		
			return true;
		}
		
		//将将胡牌
		public function IsJiangJiangHu( cbSparrowIndex:Array/*[MAX_INDEX]*/,  WeaveItem:Array,  cbWeaveCount:uint):Boolean
		{
			//组合判断
			for (var i:uint=0;i<cbWeaveCount;i++)
			{
				//类型判断
				var cbWeaveKind:uint=WeaveItem[i].cbWeaveKind;
				if ((cbWeaveKind!=WIK_PENG)&&(cbWeaveKind!=WIK_GANG)&&(cbWeaveKind!=WIK_FILL)) return false;
		
				//数值判断
				var cbCenterValue:uint=(WeaveItem[i].cbCenterSparrow&MASK_VALUE);
				if ((cbCenterValue!=2)&&(cbCenterValue!=5)&&(cbCenterValue!=8)) return false;
			}
		
			//麻将判断
			for ( i=0;i<MAX_INDEX;i++)
			{
				if ((i%3!=1)&&(cbSparrowIndex[i]!=0)) return false;
			}
		
			return true;
		}
		
		//麻将转换
		public function SwitchToSparrowData1(cbSparrowIndex:uint):uint
		{
			ASSERT(cbSparrowIndex<34);
			return ((cbSparrowIndex/9)<<4)|(cbSparrowIndex%9+1);
		}
		
		//麻将转换
		public function SwitchToSparrowIndex1(cbSparrowData:uint):uint
		{
			ASSERT(IsValidSparrow(cbSparrowData));
			return ((cbSparrowData&MASK_COLOR)>>4)*9+(cbSparrowData&MASK_VALUE)-1;
		}
		
		//麻将转换
		public function SwitchToSparrowData2( cbSparrowIndex:Array/*[MAX_INDEX]*/,  cbSparrowData:Array/*[MAX_COUNT]*/):uint
		{
			//转换麻将
			var cbPosition:uint=0;
			for (var i:uint=0;i<MAX_INDEX;i++)
			{
				if (cbSparrowIndex[i]!=0)
				{
					for (var j:uint=0;j<cbSparrowIndex[i];j++)
					{
						ASSERT(cbPosition<MAX_COUNT);
						cbSparrowData[cbPosition++]=SwitchToSparrowData1(i);
					}
				}
			}
		
			return cbPosition;
		}
		
		//麻将转换
		public function SwitchToSparrowIndex3( cbSparrowData:Array,  cbSparrowCount:uint,  cbSparrowIndex:Array/*[MAX_INDEX]*/):uint
		{
			//设置变量
			ZeroArray(cbSparrowIndex);
		
			//转换麻将
			for (var i:uint=0;i<cbSparrowCount;i++)
			{
				ASSERT(IsValidSparrow(cbSparrowData[i]));
				cbSparrowIndex[SwitchToSparrowIndex1(cbSparrowData[i])]++;
			}
		
			return cbSparrowCount;
		}
		
		//分析麻将
		public function AnalyseSparrow( cbSparrowIndex:Array,  WeaveItem:Array,  cbWeaveCount:uint, AnalyseItemArray:Array):Boolean
		{
			//计算数目
			var cbSparrowCount:uint=0;
			for (var i:uint=0;i<MAX_INDEX;i++) cbSparrowCount+=cbSparrowIndex[i];
		
			//效验数目
			ASSERT((cbSparrowCount>=2)&&(cbSparrowCount<=MAX_COUNT)&&((cbSparrowCount-2)%3==0));
			if ((cbSparrowCount<2)||(cbSparrowCount>MAX_COUNT)||((cbSparrowCount-2)%3!=0)) return false;
		
			//变量定义
			var cbKindItemCount:uint=0;
			var KindItem:Array = NewArray(MAX_COUNT-2,tagKindItem);
			
		
			//需求判断
			var cbLessKindItem:uint=(cbSparrowCount-2)/3;
			ASSERT((cbLessKindItem+cbWeaveCount)==4);
		
			//单吊判断
			if (cbLessKindItem==0)
			{
				//效验参数
				ASSERT((cbSparrowCount==2)&&(cbWeaveCount==4));
		
				//牌眼判断
				for (var i:uint=0;i<MAX_INDEX;i++)
				{
					if (cbSparrowIndex[i]==2)
					{
						//变量定义
						var AnalyseItem:tagAnalyseItem = new tagAnalyseItem;
						
						//设置结果
						for (var j:uint=0;j<cbWeaveCount;j++)
						{
							AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
							AnalyseItem.cbCenterSparrow[j]=WeaveItem[j].cbCenterSparrow;
						}
						AnalyseItem.cbSparrowEye=SwitchToSparrowData1(i);
		
						//插入结果
						AnalyseItemArray.push(AnalyseItem);
		
						return true;
					}
				}
		
				return false;
			}
		
			//拆分分析
			if (cbSparrowCount>=3)
			{
				for (var i:uint=0;i<MAX_INDEX;i++)
				{
					//同牌判断
					if (cbSparrowIndex[i]>=3)
					{
						KindItem[cbKindItemCount].cbCenterSparrow=i;
						KindItem[cbKindItemCount].cbSparrowIndex[0]=i;
						KindItem[cbKindItemCount].cbSparrowIndex[1]=i;
						KindItem[cbKindItemCount].cbSparrowIndex[2]=i;
						KindItem[cbKindItemCount++].cbWeaveKind=WIK_PENG;
					}
					
					//连牌判断
					if ((i<(MAX_INDEX-2))&&(cbSparrowIndex[i]>0)&&((i%9)<7))
					{
						for (var j:uint=1;j<=cbSparrowIndex[i];j++)
						{
							if ((cbSparrowIndex[i+1]>=j)&&(cbSparrowIndex[i+2]>=j))
							{
								KindItem[cbKindItemCount].cbCenterSparrow=i;
								KindItem[cbKindItemCount].cbSparrowIndex[0]=i;
								KindItem[cbKindItemCount].cbSparrowIndex[1]=i+1;
								KindItem[cbKindItemCount].cbSparrowIndex[2]=i+2;
								KindItem[cbKindItemCount++].cbWeaveKind=WIK_LEFT;
							}
						}
					}
				}
			}
		
			//组合分析
			if (cbKindItemCount>=cbLessKindItem)
			{
				var i:uint = 0;
				//变量定义
				var cbSparrowIndexTemp:Array = new Array(MAX_INDEX);
				ZeroArray(cbSparrowIndexTemp);
		
				//变量定义
				var cbIndex:Array=new Array(0,1,2,3);
				var pKindItem:Array = new Array(4);
				ZeroArray(pKindItem,null);
		
				//开始组合
				do
				{
					//设置变量
					CopyArray(cbSparrowIndexTemp,cbSparrowIndex,cbSparrowIndexTemp.length);
					for (i=0;i<cbLessKindItem;i++) pKindItem[i]=KindItem[cbIndex[i]];
		
					//数量判断
					var bEnoughSparrow:Boolean=true;
					for (i=0;i<cbLessKindItem*3;i++)
					{
						//存在判断
						cbSparrowIndex=pKindItem[i/3].cbSparrowIndex[i%3];
						if (cbSparrowIndexTemp[cbSparrowIndex]==0)
						{
							bEnoughSparrow=false;
							break;
						}
						else cbSparrowIndexTemp[cbSparrowIndex]--;
					}
		
					//胡牌判断
					if (bEnoughSparrow==true)
					{
						//牌眼判断
						var cbSparrowEye:uint=0;
						for (i=0;i<MAX_INDEX;i++)
						{
							if (cbSparrowIndexTemp[i]==2)
							{
								cbSparrowEye=SwitchToSparrowData1(i);
								break;
							}
						}
		
						//组合类型
						if (cbSparrowEye!=0)
						{
							//变量定义
							var AnalyseItem:tagAnalyseItem = new tagAnalyseItem;
						
							//设置组合
							for (i=0;i<cbWeaveCount;i++)
							{
								AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
								AnalyseItem.cbCenterSparrow[i]=WeaveItem[i].cbCenterSparrow;
							}
		
							//设置牌型
							for (i=0;i<cbLessKindItem;i++) 
							{
								AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i].cbWeaveKind;
								AnalyseItem.cbCenterSparrow[i+cbWeaveCount]=pKindItem[i].cbCenterSparrow;
							}
		
							//设置牌眼
							AnalyseItem.cbSparrowEye=cbSparrowEye;
		
							//插入结果
							AnalyseItemArray.push(AnalyseItem);
						}
					}
		
					//设置索引
					if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
					{
						for (i=cbLessKindItem-1;i>0;i--)
						{
							if ((cbIndex[i-1]+1)!=cbIndex[i])
							{
								var cbNewIndex:uint=cbIndex[i-1];
								for (var j:uint=(i-1);j<cbLessKindItem;j++) cbIndex[j]=cbNewIndex+j-i+2;
								break;
							}
						}
						if (i==0) break;
					}
					else cbIndex[cbLessKindItem-1]++;
					
				} while (true);
		
			}
		
			return (AnalyseItemArray.length()>0);
		}
	}
}

include "./GameLogicDef.as"
include "../../../Common/Memory.as"
include "../../../Common/MathFunction.as"