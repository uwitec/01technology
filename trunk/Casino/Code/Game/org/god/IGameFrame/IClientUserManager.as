package org.god.IGameFrame
{
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.tagUserScore;
	import org.god.Net.tagUserStatus;
	//用户管理
	public interface IClientUserManager
	{
		//管理接口
		//设置回调接口
		function Destroy():void;
		//设置管理接口
		function SetUserManagerSink(pIUnknownEx:*):Boolean;
		//增加用户
		function ActiveUserItem(UserData:tagUserInfoHead):IUserItem;
		//删除用户
		function DeleteUserItem(pIUserItem:IUserItem):Boolean;
		//更新积分
		function UpdateUserItemScore(pIUserItem:IUserItem,pUserScore:tagUserScore):Boolean;
		//更新状态
		function UpdateUserItemStatus(pIUserItem:IUserItem,pUserStatus:tagUserStatus):Boolean;

		//信息接口
		//获取人数
		function GetOnLineCount():uint;

		//查找接口
		//枚举用户
		function EnumUserItem(wEnumIndex:uint):IUserItem;
		//查找用户
		function SearchUserByID(dwUserID:uint):IUserItem;
	}
}