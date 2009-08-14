package
{
	import org.god.Net.tagUserData;
	import org.god.Net.tagSubUserData;
	//用户数据树节点数据
	public class UserDataTreeNodeData
	{
		public var m_UserData:tagUserData;
		public var m_SubUserDataArray:Array = new Array(0);
		public var m_bLessUserDataLoaded:Boolean;
		
		public function UserDataTreeNodeData(UserData:tagUserData,
									 bLessUserDataLoaded:Boolean = false)
		{
			m_UserData = UserData;
			m_bLessUserDataLoaded = bLessUserDataLoaded;
		}
		public function getAccount():String
		{
			if(m_UserData)
				return m_UserData.szAccount;
			else
				return "??????";
		}
		public function getUserData():tagUserData
		{
			return m_UserData;
		}
		public function getLessUserDataLoaded():Boolean
		{
			return m_bLessUserDataLoaded;
		}
		public function setLessUserDataLoaded(bLessUserDataLoaded:Boolean = true):void
		{
		  m_bLessUserDataLoaded = bLessUserDataLoaded;
		}
		public function appendSubUserData(SubUserData:tagSubUserData):void
		{
			m_SubUserDataArray.push(SubUserData);
		}
		public function getSubUserDatas():Array
		{
			return m_SubUserDataArray;
		}
	}
}