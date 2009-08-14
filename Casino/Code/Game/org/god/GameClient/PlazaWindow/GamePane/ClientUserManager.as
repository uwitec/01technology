package 
{
	import org.aswing.util.LinkedList;
	
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.tagUserScore;
	import org.god.Net.tagUserStatus;
	import org.god.IGameFrame.IClientUserManager;
	import org.god.IGameFrame.IUserManagerSink;
	import org.god.IGameFrame.IUserItem;
	//客户端用户管理
	public class ClientUserManager implements IClientUserManager
	{
		private var m_UserItemActive:LinkedList;//活动数组
		
		private var m_pIUserManagerSink:IUserManagerSink;//回调接口

		public function ClientUserManager()
		{
			m_UserItemActive  = new LinkedList;
		}
		//管理接口
		//设置回调接口
		public function Destroy():void
		{
			if(m_UserItemActive)
			{
				var pUserItemActive:UserItem=null;
				for (var i:uint=0; i<m_UserItemActive.size(); i++)
				{
					pUserItemActive=m_UserItemActive.get(i);
					if (pUserItemActive!=null)
					{
						pUserItemActive.CleanData();
					}
				}
				m_UserItemActive.clear();
				m_UserItemActive = null;
			}
			
			m_pIUserManagerSink = null;
		}
		public function SetUserManagerSink(pIUnknownEx:*):Boolean
		{
			m_pIUserManagerSink = pIUnknownEx as IUserManagerSink;
			return m_pIUserManagerSink != null?true:false;
		}
		//增加用户
		public function ActiveUserItem(UserData:tagUserInfoHead):IUserItem
		{
			var pUserItem:UserItem=null;
			pUserItem=new UserItem;
			if (pUserItem==null)
			{
				return null;
			}

			//拷贝数据
			pUserItem.UpdateUserData(UserData);
			pUserItem.setActive(true);
			m_UserItemActive.append(pUserItem);

			//更新信息
			if (m_pIUserManagerSink)
			{
				m_pIUserManagerSink.OnUserItemAcitve(pUserItem);
			}
			return pUserItem;
		}
		//删除用户
		public function DeleteUserItem(pIUserItem:IUserItem):Boolean
		{
			//查找用户
			var pUserItemActive:UserItem=null;
			for (var i:uint=0; i<m_UserItemActive.size(); i++)
			{
				pUserItemActive=m_UserItemActive.get(i);
				if (pIUserItem==pUserItemActive)
				{
					//删除用户
					m_UserItemActive.removeAt(i);
					
					if (m_pIUserManagerSink)
					{
						m_pIUserManagerSink.OnUserItemDelete(pUserItemActive);
					}
					pUserItemActive.CleanData();
					return true;
				}
			}
			return false;
		}
		//更新积分
		public function UpdateUserItemScore(pIUserItem:IUserItem,pUserScore:tagUserScore):Boolean
		{
			if (pUserScore==null)
			{
				return false;
			}

			//设置数据
			pIUserItem.SetUserScore(pUserScore);

			//通知更新
			if (m_pIUserManagerSink)
			{
				m_pIUserManagerSink.OnUserItemUpdate(pIUserItem);
			}
			return true;
		}
		//更新状态
		public function UpdateUserItemStatus(pIUserItem:IUserItem,pUserStatus:tagUserStatus):Boolean
		{
			//设置数据
			pIUserItem.SetUserStatus(pUserStatus);

			//通知更新
			if (m_pIUserManagerSink)
			{
				m_pIUserManagerSink.OnUserItemUpdate(pIUserItem);
			}
			return true;

		}

		//信息接口
		//获取人数
		public function GetOnLineCount():uint
		{
			return m_UserItemActive.size();
		}

		//查找接口
		//枚举用户
		public function EnumUserItem(wEnumIndex:uint):IUserItem
		{
			if (wEnumIndex<m_UserItemActive.size())
			{
				var pUserItem:UserItem=m_UserItemActive.get(wEnumIndex);
				if (pUserItem.IsActive()==true)
				{
					return pUserItem;
				}
				else
				{
					return null;
				}
			}
			return null;
		}
		//查找用户
		public function SearchUserByID(dwUserID:uint):IUserItem
		{
			var pUserItem:UserItem=null;
			for (var i:int=0; i<m_UserItemActive.size(); i++)
			{
				pUserItem=m_UserItemActive.get(i);

				if (pUserItem.IsActive()==true && 
				pUserItem.GetUserData().dwUserID==dwUserID)
				{
					return pUserItem;
				}
			}
			return null;
		}
	}
}