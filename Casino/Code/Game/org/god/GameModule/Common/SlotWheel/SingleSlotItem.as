package org.god.GameModule.Common.SlotWheel
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	
	public class SingleSlotItem extends MovieClip
	{
		protected var m_bLastRequstSlotItemType:Boolean;
		protected var m_cbSlotItemType:uint;
		protected var m_bmBackground:Bitmap;
		protected var m_ScrollDeltaValue:*;
		
		public function SingleSlotItem(type:uint,
									   deltaValue:*,
									   bg:BitmapData,
									   xp:int = 0,
									   yp:int = 0)
		{
			super();
			m_cbSlotItemType = type;
			m_ScrollDeltaValue= deltaValue;
			m_bmBackground = new Bitmap(bg,"auto",true);
			addChild(m_bmBackground);
			m_bmBackground.x  = xp;
			m_bmBackground.y  = yp;
			m_bLastRequstSlotItemType = false;
		}
		public function Destroy():void
		{
			removeChild(m_bmBackground);
			m_bmBackground.bitmapData.dispose();
			m_bmBackground = null;
		}
		public function IsLastRequstSlotItemType():Boolean
		{
			return m_bLastRequstSlotItemType;
		}
		public function SetLastRequstSlotItemType(b:Boolean):void
		{
			m_bLastRequstSlotItemType = b;
		}
		public function GetSlotItemType():uint
		{
			return m_cbSlotItemType;
		}
		public function GetScrollDeltaValue():*
		{
			return m_ScrollDeltaValue;
		}
		public function SetScrollDeltaValue(deltaValue:*):void
		{
			m_ScrollDeltaValue = deltaValue;
		}
	}
}