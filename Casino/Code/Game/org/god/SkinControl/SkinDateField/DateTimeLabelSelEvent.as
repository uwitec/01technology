package org.god.SkinControl.SkinDateField
{
	import flash.events.Event;
	
	public class DateTimeLabelSelEvent extends Event
	{
		public static const EVENTNAME_DTLS:String = "EventName_DTLS";
		
		protected var m_nNumValue:uint;
		
		public function DateTimeLabelSelEvent(nNumValue:uint)
		{
			super(EVENTNAME_DTLS);
			m_nNumValue = nNumValue;
		}
		public function getNumValue():uint
		{
			return m_nNumValue;
		}
	}
}