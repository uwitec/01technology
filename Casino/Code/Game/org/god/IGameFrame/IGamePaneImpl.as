package org.god.IGameFrame
{
	import flash.display.DisplayObjectContainer;
	import flash.utils.ByteArray;
	import flash.display.MovieClip;
	
	import org.god.Net.ClientSocket;
	//游戏面板
	public interface IGamePaneImpl extends IPaneImpl
	{
		//获取网络接口
		function getClientSocket():ClientSocket;
		//执行进入游戏任务
		function DoEnterGameWork():void;
		//判断登陆成功
		function isLogonOK():Boolean;
		//显示状态消息对话框
		function ShowStatusMessage(strMsg:String):void;
		//隐藏状态消息对话框
		function HideStatusMessage():void;
		//显示消息对话框
		function ShowMessageBox(strMsg:String):void;

	}
}
include "../Net/InterfaceModuleIDef.as"
include "../Net/NetModuleIDef.as"
include "../Net/GLOBALDEF.as"
include "../Net/CMD_Plaza.as"
