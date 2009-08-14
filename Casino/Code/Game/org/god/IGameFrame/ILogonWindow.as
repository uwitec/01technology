package org.god.IGameFrame
{
	import flash.display.DisplayObjectContainer;
	import org.god.Net.IClientSocketSink;
	//登陆窗口接口
	public interface ILogonWindow
	{
		//获取主类
		function getIMain():IMain;
		//显示
		function Show():void;
		//隐藏
		function Hide():void;
		//获取活动显示对象容器
		function getActiveDOC():DisplayObjectContainer;
		//复位
		function resetAll():void;
		//获取网络处理钩子接口
		function getSocketSink():IClientSocketSink;
	}
}