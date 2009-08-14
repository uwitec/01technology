package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.SkinControl.SkinImage;
	import org.god.GameModule.Common.IUserInfoView;
	import org.god.Common.GlobalCommonMain;
	import org.god.IGameFrame.UserDataChangeEvent;
	
	import org.god.Net.tagUserData;
	
	import org.aswing.graphics.Graphics2D;
	import org.aswing.graphics.BitmapBrush;
	
	public class BaseUserInfoView extends MovieClip implements IUserInfoView
	{
		protected var m_bmBackGround:SkinImage;
		protected var m_nWidth:Number;
		protected var m_nHeight:Number;
		protected var m_nFontSize:uint;
		protected var m_nFontColor:uint;
		protected var m_ptUserName:Point = new Point;
		protected var m_ptScore:Point = new Point;
		protected var m_strName:String;
		protected var m_fScore:Number = 0;
		
		public function BaseUserInfoView(bg:SkinImage,
										 nFontSize:uint=18,
										 nFontColor:uint=0xffffffff)
		{
			super();
			
			m_bmBackGround = bg;
			m_nFontSize	   = nFontSize;
			m_nFontColor   = nFontColor;

			m_nWidth = m_bmBackGround.GetWidth();
			m_nHeight = m_bmBackGround.GetHeight();
			RectifyUserInfoView(m_bmBackGround.GetWidth(),
							m_bmBackGround.GetHeight());
		}
		public function createUserInfoView():Boolean
		{
			var main:* = GlobalCommonMain.getGlobalCommonMain().GetIMain();
			if(main)
			{
				
				main.addUserDataChangeEventListener(OnEventUserDataChange);
				
				var userData:tagUserData = main.getUserData();
				m_strName = String(userData.szName);
				m_fScore  = Number(userData.UserScore.fScore);
				Invalidate();
				
				return true;
			}
			return false;
		}
		public function destroy():void
		{
			var main:* = GlobalCommonMain.getGlobalCommonMain().GetIMain();
			if(main)
			{
				main.removeUserDataChangeEventListener(OnEventUserDataChange);
			}
			m_bmBackGround.Unload();
			m_bmBackGround = null;
			m_ptUserName   = null;
			m_ptScore	   = null;
		}
		public function getMovieClip():MovieClip
		{
			return this;
		}
		public function moveMovieClip(xp:Number, yp:Number):void
		{
			this.x = xp;
			this.y = yp;
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
			
			DrawUtil.TextOut(dc,
					 m_ptUserName.x,
					 m_ptUserName.y,
					 m_strName,
				 	 m_nFontSize,
					 m_nFontColor,0,0, "center");
			
			DrawUtil.TextOut(dc,
					 m_ptScore.x,
					 m_ptScore.y,
					 m_fScore.toFixed(2),
				 	 m_nFontSize,
					 m_nFontColor,0,0, "center");
		}
		//调整控件
		protected function RectifyUserInfoView(nWidth:int,nHeight:int):void
		{
		}
		protected function OnEventUserDataChange(e:UserDataChangeEvent):void
		{
			var userData:tagUserData = e.GetUserData();
			m_strName = String(userData.szName);
			m_fScore  = Number(userData.UserScore.fScore);
			Invalidate();
		}
	}
}
include "../../StringTableDef.as"
include "../../Common/Memory.as"