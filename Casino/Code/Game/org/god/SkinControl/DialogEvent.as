package org.god.SkinControl
{
	import flash.events.Event;
	
	public class DialogEvent extends Event
	{
		public static const DIALOGEVENTNAME = "dialog_event";
		
		public static const CMD_OK:uint = 0;
		public static const CMD_CANCEL:uint = CMD_OK + 1;
		public static const CMD_YES:uint = 0;
		public static const CMD_NO:uint = CMD_YES + 1;
		
		public var m_nCmd:uint;
		public var m_pUserData:*;
		public var m_nWParam:*;
		public var m_nLParam:*;
		
		public function DialogEvent(nCmd:uint, 
									pUserData:* = null,
									nWParam:* = null,
									nLParam:* = null)
		{
			super(DIALOGEVENTNAME);
			m_nCmd = nCmd;
			m_pUserData = pUserData;
			m_nWParam 	= nWParam;
			m_nLParam	= nLParam;
		}
		public function GetCmd():uint 
		{
			return m_nCmd;
		}
		public function GetUserObject():* 
		{
			return m_pUserData;
		}
		public function GetWParam():*  
		{
			return m_nWParam;
		}
		public function GetLParam():*  
		{
			return m_nLParam;
		}
		
	}
}