package org.god.GameManagement.PlazaWindow.Common
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.utils.ByteArray;
	
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.IPlazaWindow;
	import org.god.Common.DrawUtil;
	import org.god.Net.IClientSocket;
	
	import org.aswing.JPanel;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	import org.aswing.event.AWEvent;
	//基本面板
	public class BasePanel extends JPanel
	{
		protected var m_bmBackground:BitmapData;
		protected var m_IMain:IMain;
		protected var m_IPlazaWindow:IPlazaWindow;
		protected var m_ClientSocket:IClientSocket;
		
		public function BasePanel()
		{
			super();
			SetupPanel();
		}
		protected function SetupPanel():void
		{
			m_bmBackground = null;
		}
		
		protected function DrawPanel(g:Graphics,pDC:BitmapData):void
		{
			
		}
		protected function DrawPanelTitle(pDC:BitmapData,
										  strTitle:String,
										  nXPos:Number = 42,
										  nYPos:Number =10):void
		{
			DrawUtil.TextOut(pDC,nXPos,
								 nYPos,
								 strTitle,
								 26,
								 0xCC9900);
		}
		//发送函数
		protected function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean
		{
			if(m_ClientSocket == null)
				return false;
			return m_ClientSocket.SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
		}
		
		override protected function paint(b:IntRectangle):void
		{
			if(this.graphics && m_bmBackground)
			{
				this.graphics.clear();
			
				var g:Graphics2D = new Graphics2D(this.graphics);
				
				var pDC:BitmapData = m_bmBackground.clone();
				
				DrawPanel(this.graphics, pDC);
				
 				var bBrush:BitmapBrush = new BitmapBrush(pDC, null, false, true);	
 				g.fillRectangle(bBrush,0,0,
								pDC.width,
								pDC.height);
				
				
				bBrush = null;
				g = null;
				pDC    = null;
			
			
				dispatchEvent(new AWEvent(AWEvent.PAINT, false, false));
			}
		}
	}
}