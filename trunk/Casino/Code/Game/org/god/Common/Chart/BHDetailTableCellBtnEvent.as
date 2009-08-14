package org.god.Common.Chart
{
	import flash.events.Event;
	//游戏局记录详细信息表单元事件
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