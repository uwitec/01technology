package 
{
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.tagUserScore;
	import org.god.Net.tagUserStatus;
	import org.god.IGameFrame.IUserItem;
	//用户信息
	public class UserItem implements IUserItem
	{
		private var m_UserData:tagUserInfoHead;
		private var m_bActive;
		
		public function UserItem()
		{
			m_UserData = new tagUserInfoHead;
		}
		//访问判断
		public function IsActive():Boolean
		{
			return m_bActive;
		}
		public function setActive(b:Boolean):void
		{
			m_bActive = b;
		}

		//属性接口
		//用户 I D
		public function GetUserID():uint
		{
			return m_UserData.dwUserID;
		}
		//用户名字
		public function GetUserName():String
		{
			return m_UserData.szName;
		}
		public function GetUserFaceID():uint
		{
			return m_UserData.wFaceID;
		}		
		//获取用户
		public function GetUserData():tagUserInfoHead
		{
			return m_UserData;
		}
		public function UpdateUserData(UserData:tagUserInfoHead):void
		{
			m_UserData.wFaceID = UserData.wFaceID;
			m_UserData.dwUserID = UserData.dwUserID;
			m_UserData.szName = String(UserData.szName);
			
			m_UserData.UserStatus.wTableID=UserData.UserStatus.wTableID;
			m_UserData.UserStatus.wChairID=UserData.UserStatus.wChairID;
			m_UserData.UserStatus.wNetDelay=UserData.UserStatus.wNetDelay;
			m_UserData.UserStatus.cbUserStatus=UserData.UserStatus.cbUserStatus;
			
		}

		//设置接口
		//设置积分
		public function SetUserScore(pUserScore:tagUserScore):void
		{
		}
		//设置状态
		public function SetUserStatus(pUserStatus:tagUserStatus):void
		{
			//设置变量
			m_UserData.UserStatus.wTableID=pUserStatus.wTableID;
			m_UserData.UserStatus.wChairID=pUserStatus.wChairID;
			m_UserData.UserStatus.wNetDelay=pUserStatus.wNetDelay;
			m_UserData.UserStatus.cbUserStatus=pUserStatus.cbUserStatus;
		}
		
		public function CleanData():void
		{
			m_bActive=false;
			m_UserData = new tagUserInfoHead;
			m_UserData.UserStatus.wTableID=INVALID_TABLE;
			m_UserData.UserStatus.wChairID=INVALID_CHAIR;
		}
	}
}
include "../../../Net/GLOBALDEF.as"
