package 
{
	import flash.events.Event;
	//用户表状态单元事件
	public class UserStateTableCellEvent extends Event
	{
		public static const USTCEVENTNAME:String = "UserStateTableCellEvent_Name";
		
		protected var m_strNewState:String;
		
		public function UserStateTableCellEvent(strNewState:String)
		{
			super(USTCEVENTNAME, true, true);
			m_strNewState = strNewState;
		}
		public function getNewState():String
		{
			return m_strNewState;
		}
	}
}