package org.god.Common.BetHistory.GameModule.Common
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.Common.BetHistory.IBetHistoryResource;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.BetHistory.tagBetHistoryData;
	//基本对战游戏结束视图
	public class BaseBattleGameEndView extends BaseGameEndView
	{
		protected var m_imageWinScoreView:SkinImage;
		protected var m_fScore:Array;
		
		public function BaseBattleGameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super(bh,pIBetHistoryResource);
			m_imageWinScoreView = pIBetHistoryResource.GetResource("IMAGE_CGEV_CELL_WINSCORE");;
		}
		public override function Destroy():void
		{
			super.Destroy();
			m_fScore = null;
			m_imageWinScoreView = null;
		}
		public function SetWinScore(fScore:Array,nChairNum:uint):void
		{
			if(nChairNum > 0)
			{
				m_fScore = new Array(nChairNum);
				for(var i:int = 0; i < nChairNum; i ++)
				{
					m_fScore[i] = fScore[i];
				}
			}
		}
		//重画函数
		protected override function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);
			if(m_imageWinScoreView)
			{
				var nXP:int = 7;
				var nYP:int = 476;
				m_imageWinScoreView.DrawBitmapData(dc,nXP,nYP);
				if(m_fScore)
				{
					var strStringBuffer:String;
					var nFontColor:uint;
					
					nYP += 20;
					for(var i:int = 0; i < m_fScore.length; i ++)
					{
						nFontColor = 0xCC9900;
						strStringBuffer = GetChairIDName(i);
				
						DrawUtil.TextOut(dc,
						 nXP + 55,
						 nYP + 12 + i * 22,
						 strStringBuffer,
				 		 18,nFontColor,
					 	 0,0, "center");
				
						if(m_fScore[i] < 0)
							nFontColor = 0xff2020;
						strStringBuffer = m_fScore[i].toFixed(2);
				
						DrawUtil.TextOut(dc,
						 nXP + 188,
						 nYP + 12 + i * 21,
						 strStringBuffer,
				 		 18,
						 nFontColor,0,0, "center");
					}
				}
			}
		}
	}
}