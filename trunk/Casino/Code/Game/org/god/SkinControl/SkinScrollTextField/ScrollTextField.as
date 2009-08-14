package org.god.SkinControl.SkinScrollTextField
{
	import flash.display.BitmapData;
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	
	import org.aswing.Container;
	import org.aswing.JLabel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;

	public class ScrollTextField extends Container
	{
		public static const eDirect_Top:int  = ScrollTextPane.eDirect_Top;
		public static const eDirect_Left:int = ScrollTextPane.eDirect_Left;
		
		protected var m_ScrollTextPane:ScrollTextPane;
		protected var m_bmdBackGround:BitmapData;
		protected var m_bmBackGround:Bitmap;
		protected var m_bmdBackGroundMC:MovieClip;
		
		public function ScrollTextField(bmdBackGround:BitmapData,
										bmdBackGroundMC:MovieClip,
										bgW:int,
										bgH:int,
										eDirect:int = 0,
										bMultiLine:Boolean = false,
										nTextAreaX:int = 20,
										nTextAreaY:int = 20,
										nTextAreaW:int = 200,
										nTextAreaH:int = 200,
										nScrollDelay:int = 500)
		{
			super();
			if(bmdBackGround)
			{
				m_bmdBackGround = bmdBackGround;
				m_bmBackGround = new Bitmap(m_bmdBackGround,  "auto", true);
				addChild(m_bmBackGround);
			}
			if(bmdBackGroundMC)
			{
				m_bmdBackGroundMC = bmdBackGroundMC;
				addChild(m_bmdBackGroundMC);
			}
			setSizeWH(bgW, bgH);
			
			m_ScrollTextPane = new ScrollTextPane(eDirect,
										bMultiLine,
										nScrollDelay);
			addChild(m_ScrollTextPane);
			m_ScrollTextPane.setLocationXY(nTextAreaX, nTextAreaY);
			m_ScrollTextPane.setSizeWH(nTextAreaW, nTextAreaH);

		}
		public function UpdateText(strText:String):void
		{
			m_ScrollTextPane.UpdateText(strText);
		}
		public override function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_ScrollTextPane)
				m_ScrollTextPane.setFont(newFont);
		}
		public override function setForeground(c:ASColor):void
		{
			super.setForeground(c);
			if(m_ScrollTextPane)
				m_ScrollTextPane.setForeground(c);
		}
		public function Start():void
		{
			m_ScrollTextPane.Start();
		}
		public function Stop():void
		{
			m_ScrollTextPane.Stop();
		}
		public function Destroy():void
		{
			if(m_bmdBackGround)
			{
				removeChild(m_bmBackGround);
				m_bmBackGround = null;
				m_bmdBackGround.dispose();
				m_bmdBackGround = null;
			}
			if(m_bmdBackGroundMC)
			{
				removeChild(m_bmdBackGroundMC);
				m_bmdBackGroundMC = null;
			}
			m_ScrollTextPane.Destroy();
			removeChild(m_ScrollTextPane);
			m_ScrollTextPane = null;
		}
		
	}
}