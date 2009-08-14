package org.god.Common
{
	import org.god.IGameFrame.IMain;
	import org.god.IGameManagement.IMain;
	import org.god.Net.tagUserData;
	//主类接口
	public interface ICommonMain
	{
		//设置游戏全局主类
		function SetIGameFrameMain(main:org.god.IGameFrame.IMain):void;
		//设置管理全局主类
		function SetIGameManagementMain(main:org.god.IGameManagement.IMain):void;
		//获取游戏全局主类
		function GetIGameFrameMain():org.god.IGameFrame.IMain;
		//获取管理全局主类
		function GetIGameManagementMain():org.god.IGameManagement.IMain;
		//获取全局主类
		function GetIMain():*;
		//获取用户信息
		function GetUserData():tagUserData;
		//获取用户等级
		function GetUserLevel():uint;
		//获取用户游戏权限
		function GetUserRight():uint;
		//获取用户管理权限
		function GetMasterRight():uint;
	}
}