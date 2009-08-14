package org.god.Common.BetHistory.GameModule.B_SiChuan_Land
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.Common.BetHistory.IBetHistoryResource;
	import org.god.Common.BetHistory.GameModule.Common.BaseBattleGameEndView;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.BetHistory.tagBetHistoryData;
	import org.god.GameModule.Common.enXCollocateMode;
	import org.god.GameModule.Common.enYCollocateMode;
	import org.god.GameModule.B_SiChuan_Land.Common.CMD_S_GameEnd;
	//游戏结束视图
	public class GameEndView extends BaseBattleGameEndView
	{
		protected var m_GameEnd:CMD_S_GameEnd;
		protected var m_CardControl:Array;
		protected var m_BackCardControl:CardControl;
		
		public function GameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super(bh,pIBetHistoryResource);
			m_GameEnd = CMD_S_GameEnd.readData(bh.cbEndData);
			SetWinScore(m_GameEnd.fGameScore,3);
			m_CardControl = new Array(3);
			var nXP:int = 88;
			var nYP:int = 56;
			var nCardDataIndex:int = 0;
			for(var i:int = 0; i < m_CardControl.length; i ++)
			{
				var CardItemArray:Array = new Array(m_GameEnd.bCardCount[i]);
				for(var k:int = 0; k < m_GameEnd.bCardCount[i]; k ++)
				{
					CardItemArray[k] = new tagCardItem;
					CardItemArray[k].bShoot = false;
					CardItemArray[k].bCardData = m_GameEnd.bCardData[k + nCardDataIndex];
				}
				nCardDataIndex += CardItemArray.length;
				
				m_CardControl[i] = new CardControl(pIBetHistoryResource.GetResource("IMAGE_CARD_BH"));
				addChild(m_CardControl[i]);
				m_CardControl[i].SetPositively(true);
				m_CardControl[i].SetDirection(true);
				m_CardControl[i].SetDisplayFlag(true);
				m_CardControl[i].SetCardItem(CardItemArray, CardItemArray.length);
				m_CardControl[i].SetBenchmarkPos(nXP, nYP + i * 92, 
												 enXCollocateMode.enXLeft,
												 enYCollocateMode.enYCenter);
			}
			var CardItemArray:Array = new Array(3);
			for(var k:int = 0; k < 3; k ++)
			{
				CardItemArray[k] = new tagCardItem;
				CardItemArray[k].bShoot = false;
				CardItemArray[k].bCardData = m_GameEnd.bBackCard[k];
			}	
			m_BackCardControl = new CardControl(pIBetHistoryResource.GetResource("IMAGE_CARD_BH"));
			addChild(m_BackCardControl);
			m_BackCardControl.SetPositively(true);
			m_BackCardControl.SetDirection(true);
			m_BackCardControl.SetDisplayFlag(true);
			m_BackCardControl.SetCardItem(CardItemArray, CardItemArray.length);
			m_BackCardControl.SetBenchmarkPos(nXP, nYP + i * 92, 
											 enXCollocateMode.enXLeft,
											 enYCollocateMode.enYCenter);
	
		}
		
		//重画函数
		protected override function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);

			if(m_CardControl)
			{
				var nXP:int = 10;
				var nYP:int = 52;
				
				var strText:String;
				for(var i:int = 0; i < m_CardControl.length; i ++)
				{
					m_CardControl[i].RectifyControl();
					
					if(m_GameEnd.wLandUser == i)
					{
						
						strText = GetChairIDName(i) + "(" + IDS_LAND + "):";
						if(m_GameEnd.bCardCount[i] == 0)
							strText = strText + IDS_WINNER;
					}
					else
					{
						strText = GetChairIDName(i) + ":";
						if(m_GameEnd.bCardCount[i] == 0)
							strText = strText + IDS_WINNER;
					}
					DrawUtil.TextOut(dc,
						 nXP,
					 	 nYP + i * 92,
						 strText,
				 		 18,
					 	 0xcc9900);
				}
				
				DrawUtil.TextOut(dc,
						 nXP,
					 	 nYP + i * 92,
						 IDS_BACKCARD + ":",
				 		 18,
					 	 0xcc9900);
				
				nYP-=16;
				DrawUtil.TextOut(dc,
						 nXP,
					 	 nYP + (i + 1)* 92,
						 IDS_CELLSCORETIME + ": X " + m_GameEnd.bLandScore,
				 		 18,
					 	 0xcc9900);
				DrawUtil.TextOut(dc,
						 nXP,
					 	 nYP + (i + 1)* 92 + 20,
						 IDS_BOMBTIME + ": X " + m_GameEnd.wBombTime,
				 		 18,
					 	 0xcc9900);
				DrawUtil.TextOut(dc,
						 nXP,
					 	 nYP + (i + 1)* 92 + 40,
						 IDS_TOTALTIME + ": X " + m_GameEnd.wBombTime * m_GameEnd.bLandScore,
				 		 18,
					 	 0xcc9900);
			}
		}
		public override function Destroy():void
		{
			super.Destroy();
			m_GameEnd = null;
			removeChild(m_BackCardControl);
			m_BackCardControl = null;
			ZeroArray(m_CardControl,null,DestroyCardControl);
			m_CardControl = null;
		}
		protected function DestroyCardControl(c:CardControl):void
		{
			c.Destroy();
			removeChild(c);
		}
	}
}
include "../../StringTableDef.as"
include "../../../Memory.as"