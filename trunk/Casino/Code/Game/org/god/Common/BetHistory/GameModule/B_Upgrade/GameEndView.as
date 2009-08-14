package org.god.Common.BetHistory.GameModule.B_Upgrade
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
	import org.god.GameModule.B_Upgrade.Common.CMD_S_GameEnd;
	//游戏结束视图
	public class GameEndView extends BaseBattleGameEndView
	{
		protected var m_GameEnd:CMD_S_GameEnd;
		protected var m_ConcealCard:CardControl;
		protected var m_CallCard:CardControl;
		protected var m_ScoreCardData:CardControl;
		
		public function GameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super(bh,pIBetHistoryResource);
			m_GameEnd = CMD_S_GameEnd.readData(bh.cbEndData);
			SetWinScore(m_GameEnd.fScore,4);
			
			var nXP:int = 98;
			var nYP:int = 56;
			
			var CardItemArray:Array = new Array(m_GameEnd.cbConcealCount);
			for(var i:int = 0; i < CardItemArray.length; i ++)
			{
				CardItemArray[i] = new tagCardItem;
				CardItemArray[i].bShoot = false;
				CardItemArray[i].cbCardData = m_GameEnd.cbConcealCard[i];
			}
			m_ConcealCard = new CardControl(pIBetHistoryResource.GetResource("IMAGE_CARD_BH"));
			addChild(m_ConcealCard);
			m_ConcealCard.SetPositively(true);
			m_ConcealCard.SetDirection(true);
			m_ConcealCard.SetDisplayFlag(true);
			m_ConcealCard.SetCardItem(CardItemArray, CardItemArray.length);
			m_ConcealCard.SetBenchmarkPos(nXP, nYP, 
												 enXCollocateMode.enXLeft,
												 enYCollocateMode.enYCenter);
			
			nYP += 92;
			CardItemArray = new Array(m_GameEnd.cbCallCount);
			for(i = 0; i < CardItemArray.length; i ++)
			{
				CardItemArray[i] = new tagCardItem;
				CardItemArray[i].bShoot = false;
				CardItemArray[i].cbCardData = m_GameEnd.cbCallCard;
			}
			m_CallCard = new CardControl(pIBetHistoryResource.GetResource("IMAGE_CARD_BH"));
			addChild(m_CallCard);
			m_CallCard.SetPositively(true);
			m_CallCard.SetDirection(true);
			m_CallCard.SetDisplayFlag(true);
			m_CallCard.SetCardItem(CardItemArray, CardItemArray.length);
			m_CallCard.SetBenchmarkPos(nXP, nYP, 
												 enXCollocateMode.enXLeft,
												 enYCollocateMode.enYCenter);
			
			nYP += 92;
			CardItemArray = new Array(m_GameEnd.cbScoreCardCount);
			for(i = 0; i < CardItemArray.length; i ++)
			{
				CardItemArray[i] = new tagCardItem;
				CardItemArray[i].bShoot = false;
				CardItemArray[i].cbCardData = m_GameEnd.cbScoreCardData[i];
			}
			m_ScoreCardData = new CardControl(pIBetHistoryResource.GetResource("IMAGE_CARD_BH"));
			addChild(m_ScoreCardData);
			m_ScoreCardData.SetPositively(true);
			m_ScoreCardData.SetDirection(true);
			m_ScoreCardData.SetDisplayFlag(true);
			m_ScoreCardData.SetCardItem(CardItemArray, CardItemArray.length);
			m_ScoreCardData.SetBenchmarkPos(nXP, nYP, 
												 enXCollocateMode.enXLeft,
												 enYCollocateMode.enYCenter);
		}
		
		//重画函数
		protected override function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);

			if(m_ConcealCard)
			{
				var nXP:int = 10;
				var nYP:int = 52;
				var strText:String;
				
				m_ConcealCard.RectifyControl();
				strText = IDS_CONCEALCARD + ":";
				DrawUtil.TextOut(dc,
					 nXP,
					 nYP ,
					 strText,
				 	 18,
					 0xcc9900);
					
				nYP += 92;
				m_CallCard.RectifyControl();
				strText = IDS_CALLCARD + ":";
				DrawUtil.TextOut(dc,
					 nXP,
					 nYP ,
					 strText,
				 	 18,
					 0xcc9900);
				
				nYP += 92;
				m_ScoreCardData.RectifyControl();
				strText = IDS_SCORECARD + ":";
				DrawUtil.TextOut(dc,
					 nXP,
					 nYP ,
					 strText,
				 	 18,
					 0xcc9900);
				
				nYP += 92;
				strText = IDS_BANKERUSER + ":" + GetChairIDName(m_GameEnd.wBankerUser);
				DrawUtil.TextOut(dc,
					 nXP,
					 nYP ,
					 strText,
				 	 18,
					 0xcc9900);
				nYP += 24;
				strText = IDS_CALLCARDUSER + ":" + GetChairIDName(m_GameEnd.wCallCardUser);
				DrawUtil.TextOut(dc,
					 nXP,
					 nYP ,
					 strText,
				 	 18,
					 0xcc9900);
				nYP += 24;
				var wGameTotalScore:uint = m_GameEnd.wGameScore  + 
				m_GameEnd.wConcealScore * m_GameEnd.wConcealTime;
				strText = IDS_TOTALGAMESCORE + " " + wGameTotalScore + 
				" = " + IDS_GAMESCORE + " " +
				m_GameEnd.wGameScore + " + " + IDS_CONCEALSCORE + " " +
				m_GameEnd.wConcealScore + " X " + IDS_CONCEALTIME + " " +
				m_GameEnd.wConcealTime;
				DrawUtil.TextOut(dc,
					 nXP,
					 nYP ,
					 strText,
				 	 18,
					 0xcc9900);
			}
		}
		public override function Destroy():void
		{
			super.Destroy();
			DestroyCardControl(m_ConcealCard);
			m_ConcealCard = null;
			DestroyCardControl(m_CallCard);
			m_CallCard = null;
			DestroyCardControl(m_ScoreCardData);
			m_ScoreCardData = null;
			m_GameEnd = null;
			
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