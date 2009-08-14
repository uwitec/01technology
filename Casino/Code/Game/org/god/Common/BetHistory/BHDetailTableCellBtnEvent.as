package org.god.Common.BetHistory
{
	import flash.events.Event;
	//游戏记录表事件
	public class BHDetailTableCellBtnEvent extends Event
	{
		public static const FTCBTNEVENTNAME:String = "BHDetailTableCellBtnEvent_Name";
		
		protected var m_strActionBtnName:String;
		
		public function BHDetailTableCellBtnEvent(strActionBtnName:String)
		{
			super(FTCBTNEVENTNAME, true, true);
			m_strActionBtnName = strActionBtnName;
		}
		public function getActionBtnName():String
		{
			return m_strActionBtnName;
		}
	}
}