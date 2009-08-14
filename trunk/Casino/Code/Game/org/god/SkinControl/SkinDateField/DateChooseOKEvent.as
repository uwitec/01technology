package org.god.SkinControl.SkinDateField
{
	import flash.events.Event;
	
	public class DateChooseOKEvent extends Event
	{
		public static const EVENTNAME_DCOE:String = "EventName_DCOE";
		
		protected var m_Date:Date;
		
		public function DateChooseOKEvent(date:Date)
		{
			super(EVENTNAME_DCOE);
			m_Date = date;
		}
		public function getDate():Date
		{
			return m_Date;
		}
	}
}