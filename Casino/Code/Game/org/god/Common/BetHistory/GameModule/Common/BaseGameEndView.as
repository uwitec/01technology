package org.god.Common.BetHistory.GameModule.Common
{
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.Common.BetHistory.IBetHistoryResource;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.BetHistory.tagBetHistoryData;
	
	import org.aswing.Container;
	import org.aswing.graphics.Graphics2D;
	import org.aswing.graphics.BitmapBrush;
	//基本游戏结束视图
	public class BaseGameEndView extends Sprite
	{
		protected var strChairID:Array = ["①","②","③","④","⑤","⑥","⑦","⑧","⑨"];

		protected var m_IBetHistoryResource:IBetHistoryResource;
		protected var m_bh:tagBetHistoryData;
		
		protected var m_bmBackGround:SkinImage;
		protected var m_nWidth:int;
		protected var m_nHeight:int;

		public function BaseGameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super();
			m_bh = bh;
			m_IBetHistoryResource = pIBetHistoryResource;
			m_bmBackGround = m_IBetHistoryResource.GetGameEndViewImage(m_bh.wProcessType);
			m_nWidth = m_bmBackGround.GetWidth();
			m_nHeight = m_bmBackGround.GetHeight();
			RectifyGameEndView(m_bmBackGround.GetWidth(),
							m_bmBackGround.GetHeight());
		}
		public function Destroy():void
		{
			m_bh = null;
			m_IBetHistoryResource = null;
			m_bmBackGround = null;
		}
		public function MoveWindow(xp:Number, yp:Number):void
		{
			this.x = xp;
			this.y = yp;
		}
		public function ShowWindow(bShow:Boolean):void
		{
			if(bShow)
			{
				Invalidate();
			}
			this.visible = bShow;
		}
		public function Width():Number
		{
			return m_nWidth;
		}
		public function Height():Number
		{
			return m_nHeight;
		}
		protected function GetChairIDName(wChairID:uint):String
		{
			if(wChairID < strChairID.length)
				return strChairID[wChairID];
			else
				return "*";
		}
		private function Invalidate():void
		{
			if(this.graphics)
			{
				this.graphics.clear();
				if(m_nWidth == 0 || m_nHeight == 0)
					return;
				var g:Graphics2D = new Graphics2D(this.graphics);
				
				var rect:Rectangle = new Rectangle(0, 0, m_nWidth, m_nHeight);
				var pDC:BitmapData = new BitmapData(m_nWidth, m_nHeight, true, 0x00ffffff);
				
				OnPaint(pDC,rect);
				
 				var bBrush:BitmapBrush = new BitmapBrush(pDC, 
														 null,
														 false,
														 true);	
 				g.fillRectangle(bBrush,0,0,
								pDC.width,
								pDC.height);
				
				
				bBrush = null;
				g 	  = null;
				pDC    = null;
			}
		}
		//重画函数
		protected function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			m_bmBackGround.DrawBitmapData(dc,0,0);

		}
		//调整控件
		protected function RectifyGameEndView(nWidth:int,nHeight:int):void
		{
		}
		
	}
}
