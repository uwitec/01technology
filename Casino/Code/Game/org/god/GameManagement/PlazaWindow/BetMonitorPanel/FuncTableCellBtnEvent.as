package 
{
	import flash.events.Event;
	//表功能按钮时间
	public class FuncTableCellBtnEvent extends Event
	{
		public static const FTCBTNEVENTNAME:String = "FuncUserTableCellBtnEvent_Name";
		
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