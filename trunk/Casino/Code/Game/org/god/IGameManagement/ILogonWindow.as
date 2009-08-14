package org.god.IGameManagement
{
	import flash.display.DisplayObjectContainer;
	import org.god.Net.IClientSocketSink;
	//登陆窗口接口
	public interface ILogonWindow
	{
		function getIMain():IMain;//获取主类接口
		
		function Show():void;//显示
		function Hide():void;//隐藏
		function getActiveDOC():DisplayObjectContainer;//获取显示容器
		function resetAll():void;//复位
		function getSocketSink():IClientSocketSink;//获取网络接口
	}
}