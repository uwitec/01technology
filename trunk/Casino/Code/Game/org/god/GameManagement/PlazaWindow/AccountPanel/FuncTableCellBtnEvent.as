package 
{
	import flash.events.Event;
	//帐号表事件
	public class FuncTableCellBtnEvent extends Event
	{
		public static const FTCBTNEVENTNAME:String = "FuncTableCellBtnEvent_Name";
		
		protected var m_strActionBtnName:String;
		
		public function FuncTableCellBtnEvent(strActionBtnName:String)
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