package org.god.IGameFrame
{
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.tagUserScore;
	import org.god.Net.tagUserStatus;
	
	//用户信息
	public interface IUserItem
	{
		//对象接口
		//访问判断
		function IsActive():Boolean;
		function setActive(b:Boolean):void;

		//属性接口
		//用户 I D
		function GetUserID():uint;
		//用户名字
		function GetUserName():String;
		function GetUserFaceID():uint;
		
		//获取用户
		function GetUserData():tagUserInfoHead;
		function UpdateUserData(UserData:tagUserInfoHead):void;

		//设置接口
		//设置积分
		function SetUserScore(pUserScore:tagUserScore):void;
		//设置状态
		function SetUserStatus(pUserStatus:tagUserStatus):void;
		
		//清理数据
		function CleanData():void;
	}
}