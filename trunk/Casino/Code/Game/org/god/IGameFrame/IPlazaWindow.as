package org.god.IGameFrame
{
	import flash.utils.ByteArray;
	import flash.display.DisplayObjectContainer;
	import flash.display.DisplayObject;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.IClientSocket;
	//大厅窗口接口
	public interface IPlazaWindow
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
		//退出大厅窗口
		function SendEventExitPlazaWindow():void;
		//显示子显示对象
		function addChildDO(d:DisplayObject):void;
		//隐藏子显示对象
		function removeChildDO(d:DisplayObject):void;
		//活动显示游戏面版
		function activePane(pane:IPaneImpl):int;
		//获取游戏面版数目
		function getGamePaneCount():uint;
		//获取最大游戏面版数目
		function getMaxGamePaneCount():uint;
		//查找游戏面版
		function findPane(i:uint):IPaneImpl;
		//查找游戏面版
		function findEqualPane(param:*):int;
		//查找下一游戏面版
		function findNextActivePane(nBeginPane:int):int;
		//获取面版索引
		function findPaneIndex(pane:IPaneImpl):int;
		//获取空闲游戏面版位置
		function getFreeGamePanePosition():int;
		//进入游戏面版
		function enterGamePane(pane:IPaneImpl):int;
		//退出游戏面版
		function exitGamePane(pane:IPaneImpl):void;
		//销毁游戏面版
		function destroyGamePane(nGamePane:int):void;
		//获取网络处理钩子
		function getSocketSink():IClientSocketSink;
		//处理网络消息
		function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean;
		//完成登陆事件
		function OnEventFinishLogon():void;
		//切换全频事件
		function OnEventToggleFullScreen(b:Boolean):void;
	}
}