package org.god.Common.Media
{
	import flash.events.Event;
	//静音事件
	public class QuietEvent extends Event
	{
		public static var NAME_QUIETEVENT:String = "NAME_QUIETEVENT";
		
		protected var m_bQuiet:Boolean;
		
		public function QuietEvent(b:Boolean)
		{
			super(NAME_QUIETEVENT);
			m_bQuiet = b;
		}
		public function IsQuiet():Boolean
		{
			return m_bQuiet;
		}
	}
}