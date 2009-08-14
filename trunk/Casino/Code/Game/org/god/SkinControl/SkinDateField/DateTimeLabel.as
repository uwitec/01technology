package org.god.SkinControl.SkinDateField
{
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	import org.aswing.JLabel;
	import org.aswing.ASColor;
	import org.aswing.graphics.SolidBrush;
	import org.aswing.graphics.Graphics2D;
	
	public class DateTimeLabel extends JLabel
	{
		protected var m_bSelected:Boolean;
		protected var m_bCanSelect:Boolean;
		protected var m_SelectedMC:Sprite;
		protected var m_RollOverMC:Sprite;
		protected var m_nNumValue:uint;
		protected var m_nMaxNumValue:uint;
		protected var m_strSuffix:String = "";
		protected var m_nWidthBackgroundOffset:uint = 0;
		
		public function DateTimeLabel(nMaxNumValue:uint = 0,
									   strSuffix:String = "",
									   bCanSelect:Boolean = true,
									   nWidthBackgroundOffset:uint = 5)
		{
			super("",null,JLabel.LEFT);
			
			m_bCanSelect = bCanSelect;
			m_nMaxNumValue = nMaxNumValue;
			m_strSuffix = strSuffix;
			m_nWidthBackgroundOffset = nWidthBackgroundOffset;

			m_SelectedMC = new Sprite;
			m_SelectedMC.visible = false;
			m_SelectedMC.tabEnabled = false;
			addChild(m_SelectedMC);
			
			m_RollOverMC = new Sprite;
			m_RollOverMC.visible = false;
			m_RollOverMC.tabEnabled = false;
			addChild(m_RollOverMC);

			addEventListener(MouseEvent.CLICK, OnEventClick);
			addEventListener(MouseEvent.ROLL_OVER, OnEventRollOver);
			addEventListener(MouseEvent.ROLL_OUT, OnEventRollOut);
		}
		override public function setComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);
			if(m_SelectedMC)
			{
				m_SelectedMC.graphics.clear();
    			var SelColor:ASColor = new ASColor(0x0000FF, 0.45);
				var g:Graphics2D = new Graphics2D(m_SelectedMC.graphics);
				g.fillRectangle(new SolidBrush(SelColor), 0, 2, w-m_nWidthBackgroundOffset, h-2);
		
				m_RollOverMC.graphics.clear();
    			SelColor = new ASColor(0x0000FF, 0.2);
				g = new Graphics2D(m_RollOverMC.graphics);
				g.fillRectangle(new SolidBrush(SelColor), 0, 2, w-m_nWidthBackgroundOffset, h-2);
			}
		}
		public function getSelected():Boolean
		{
			return m_bSelected;
		}
		public function setSelected(b:Boolean):void
		{
			if(m_bCanSelect && m_SelectedMC)
			{
				m_bSelected = b;
				m_SelectedMC.visible = b;
			}
		}
		public function setNumValue(n:uint):void
		{
			if(n > m_nMaxNumValue)
				n = 0;
			if(n < 0)
				n = 0;
			m_nNumValue = n;
			
			setText(getNumString());
		}
		public function getNumValue():uint
		{
			return m_nNumValue;
		}
		public function getNumString():String
		{
			if(m_nNumValue < 10)
				return "0" + m_nNumValue + m_strSuffix;
			else
				return m_nNumValue + m_strSuffix;
		}
		public function IncreaseOne():void
		{
			setNumValue(m_nNumValue +1);
		}
		public function DecreaseOne():void
		{
			var nTmp:int = int(m_nNumValue) - 1;
			if(nTmp < 0)
				nTmp = m_nMaxNumValue;
			setNumValue(nTmp);
		}
		private function OnEventClick(e:MouseEvent):void
		{
			dispatchEvent(new DateTimeLabelSelEvent(getNumValue()));
		}
		private function OnEventRollOver(e:MouseEvent):void
		{
			if(m_bCanSelect && m_RollOverMC)
			{
				m_RollOverMC.visible = true;
			}
		}
		private function OnEventRollOut(e:MouseEvent):void
		{
			if(m_bCanSelect && m_RollOverMC)
			{
				m_RollOverMC.visible = false;
			}
		}
	}
}