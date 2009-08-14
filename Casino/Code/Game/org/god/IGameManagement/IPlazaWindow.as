package org.god.IGameManagement
{
	import flash.utils.ByteArray;
	import flash.display.DisplayObjectContainer;
	import flash.display.DisplayObject;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.IClientSocket;
	//大厅窗口接口
	public interface IPlazaWindow
	{
		function getIMain():IMain;//获取主类接口
				
		function Show():void;//显示
		function Hide():void;//隐藏
		function getActiveDOC():DisplayObjectContainer;//获取显示对象容器
		function resetAll():void;//复位
		
		function activePanel(nNavCmd:uint,pParam0:*=null, pParam1:*=null):uint;//活动显示管理面版

		function getSocketSink():IClientSocketSink;//获取网络接口
		function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean;//处理网络消息
	
		function OnEventFinishLogon():void;//登陆完成事件
		function OnEventToggleFullScreen(b:Boolean):void;//切换全频事件
	}
}