package org.god.SkinControl.SkinPageBar
{
	import flash.events.Event;
	
	public class GotoPageIndexEvent extends Event
	{
		public static const EVENTNAME:String = "GotoPageIndexEvent";
		
		protected var m_wPageIndex:uint;
		
		public function GotoPageIndexEvent(wPageIndex:uint)
		{
			super(EVENTNAME);
			m_wPageIndex = wPageIndex;
		}
		public function getPageIndex():uint
		{
			return m_wPageIndex;
		}
		
	}
}