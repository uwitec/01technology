package org.god.Common
{
	import flash.events.Event;
	//鼠标选择区域事件
	public class CaptureRgnEvent extends Event
	{
		public static const EVENT_NAME:String = "CaptureRgnEvent";
		
		public var m_XPos:int;
		public var m_YPos:int;
		public var m_Width:int;
		public var m_Height:int;
		
		public function CaptureRgnEvent(x:int, y:int, w:int, h:int)
		{
			super(EVENT_NAME);
			m_XPos = x;
			m_YPos = y;
			m_Width= w;
			m_Height=h;
		}
	}
}