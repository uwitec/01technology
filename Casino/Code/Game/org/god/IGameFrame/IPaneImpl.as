package org.god.IGameFrame
{
	import flash.display.DisplayObjectContainer;
	import flash.utils.ByteArray;
	import flash.display.MovieClip;
	
	import org.god.Net.IClientSocket;
	//游戏面板接口
	public interface IPaneImpl
	{
		//创建
		function Create(param0:*,param1:* = null,param2:* = null,param3:* = null):int;
		//销毁
		function Destroy():void;
		//获取面版类型
		function getPaneType():int;
		//获取面版名称
		function getPaneName():String;
		//显示面版
		function showPane(mcParent:MovieClip,bShow:Boolean):void;
		//活动
		function Active(data:*):int;
		//取消活动
		function Deactive():int;
		//闪烁
		function Flash():void;
		//判断同类型游戏
		function EqualPane(param:*):Boolean;
		//获取面版参数
		function getPaneParam():*;
		//获取面版游戏类型数据
		function getGameKind():*;
		//获取活动显示对象容器
		function getActiveDOC():DisplayObjectContainer;
		//处理网络消息
		function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean;
	}
}
include "../Net/InterfaceModuleIDef.as"
include "../Net/NetModuleIDef.as"
include "../Net/GLOBALDEF.as"
include "../Net/CMD_Plaza.as"
