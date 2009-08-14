package org.god.IGameFrame
{
	import flash.display.DisplayObjectContainer;
	//用户协议窗口接口
	public interface IRuleWindow
	{
		//获取主类接口
		function getIMain():IMain;
		//显示
		function Show():void;
		//隐藏
		function Hide():void;
		//获取活动显示对象容器
		function getActiveDOC():DisplayObjectContainer;
		//复位
		function resetAll():void;
	}
}