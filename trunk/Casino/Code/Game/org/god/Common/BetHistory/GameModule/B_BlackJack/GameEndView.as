package org.god.Common.BetHistory.GameModule.B_BlackJack
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
	import org.god.GameModule.B_BlackJack.Common.CMD_S_GameEnd;
	//游戏结束视图
	public class GameEndView extends BaseBattleGameEndView
	{
		protected var m_GameEnd:CMD_S_GameEnd;
		protected var m_CardControl:Array;
		
		public function GameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super(bh,pIBetHistoryResource);
			m_GameEnd = CMD_S_GameEnd.readData(bh.cbEndData);
			SetWinScore(m_GameEnd.fGameGold,4);
			m_CardControl = new Array(4);
			var nXP:int = 120;
			var nYP:int = 56;

			for(var i:int = 0; i < m_CardControl.length; i ++)
			{
				var CardItemArray:Array = new Array(m_GameEnd.cbTableCardCount[i]);
				for(var k:int = 0; k < m_GameEnd.cbTableCardCount[i]; k ++)
				{
					CardItemArray[k] = m_GameEnd.cbTableCardArray[i][k];
				}

				m_CardControl[i] = new CardControl(pIBetHistoryResource.GetResource("IMAGE_CARD_BH"));
				addChild(m_CardControl[i]);
				m_CardControl[i].SetCardData(CardItemArray, CardItemArray.length);
				m_CardControl[i].SetBasicStation(new Point(nXP, nYP + i * 92), 
												 enXCollocateMode.enXLeft,
												 enYCollocateMode.enYCenter);
				m_CardControl[i].ShowFirstCard(true);
			}
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
					
					strText = GetChairIDName(i);
					if(i == m_GameEnd.wBankerStation)
						strText = strText + "(" + IDS_BR_B_BANK + ")";
					strText = strText  + ": " + m_GameEnd.cbCardSum[i] + " " + IDS_POINT;
					DrawUtil.TextOut(dc,
					 nXP,
					 nYP + i * 92,
					 strText,
				 	 18,
					 0xcc9900);
				}
			}
		}
		public override function Destroy():void
		{
			super.Destroy();
			m_GameEnd = null;
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