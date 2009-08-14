package org.god.GameModule.Common
{
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.SkinControl.SkinImage;
	
	import org.aswing.graphics.Graphics2D;
	import org.aswing.graphics.BitmapBrush;
	
	public class BaseScoreView extends Sprite
	{
		protected var m_bmBackGround:SkinImage;
		protected var m_nMeChairID:uint;
		protected var m_fScore:Array;
		protected var m_ptChairID:Array;
		protected var m_ptScore:Array;
		protected var m_nWidth:Number;
		protected var m_nHeight:Number;
		protected var m_nFontSize:uint;
		
		public function BaseScoreView(bg:SkinImage, nChairNum:uint = 4, nFontSize:uint=24)
		{
			super();
			m_nMeChairID   = 0xffff;
			m_nFontSize	   = nFontSize;
			m_bmBackGround = bg;
			m_fScore = new Array(nChairNum);
			m_ptChairID = new Array(nChairNum);
			m_ptScore = new Array(nChairNum);
			for(var i:uint = 0; i < nChairNum; i ++)
			{
				m_fScore[i] = Number(0);
				m_ptChairID[i] = new Point;
				m_ptScore[i] = new Point;
			}
			m_nWidth = m_bmBackGround.GetWidth();
			m_nHeight = m_bmBackGround.GetHeight();
			RectifyScoreView(m_bmBackGround.GetWidth(),
							m_bmBackGround.GetHeight());
		}
		public function Destroy():void
		{
			m_bmBackGround.Unload();
			m_bmBackGround = null;
			
			m_fScore = null;
			ZeroArray(m_ptChairID,null);
			m_ptChairID = null;
			ZeroArray(m_ptScore,null);
			m_ptScore = null;
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
				//绘画视图
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
		public function SetMeChairID(nID:uint):void
		{
			m_nMeChairID = nID;
		}
		//重置积分
		public function  ResetScore():void
		{
			//设置数据
			ZeroArray(m_fScore, Number(0));
		}
		//设置积分
		public function  SetGameScore(wChairID:uint, fScore:Number):void
		{
			if(m_fScore.length > wChairID)
			{
				m_fScore[wChairID] = fScore;
			}
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
			var strChairID:Array = new Array("①","②","③","④","⑤","⑥","⑦","⑧","⑨");
			var strStringBuffer:String;
			var nFontColor:uint;
			
			for(var i:uint = 0; i < m_fScore.length; i ++)
			{
				nFontColor = 0xffffff;
				strStringBuffer = strChairID[i];
				if(i == m_nMeChairID)
				{
					strStringBuffer = strStringBuffer + " " + IDS_MYSELF;
					nFontColor = 0xffff00;
				}
				DrawUtil.TextOut(dc,
					 m_ptChairID[i].x,
					 m_ptChairID[i].y,
					 strStringBuffer,
				 	 m_nFontSize,
					 nFontColor,0,0, "center");
				
				nFontColor = 0xffffff;
				if(m_fScore[i] < 0)
					nFontColor = 0xff2020;
				strStringBuffer = m_fScore[i].toFixed(2);
				
				DrawUtil.TextOut(dc,
					 m_ptScore[i].x,
					 m_ptScore[i].y,
					 strStringBuffer,
				 	 m_nFontSize,
					 nFontColor,0,0, "center");
			}
			
		}
		//调整控件
		protected function RectifyScoreView(nWidth:int,nHeight:int):void
		{
		}
	}
}
include "../../StringTableDef.as"
include "../../Common/Memory.as"