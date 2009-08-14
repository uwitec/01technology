package org.god.IGameManagement
{
	import flash.events.Event;
	import org.god.Net.tagUserData;
	//用户数据修改事件
	public class UserDataChangeEvent extends Event
	{
		public static const strEventName:String = "UserDataChangeEvent";
		public var m_UserData:tagUserData;
		
		public function UserDataChangeEvent(UserData:tagUserData)
		{
			super(strEventName);
			m_UserData = UserData;
		}
		//获取用户信息
		public function GetUserData():tagUserData
		{
			return m_UserData;
		}
	}
}