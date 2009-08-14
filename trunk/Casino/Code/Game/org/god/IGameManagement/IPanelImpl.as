package org.god.IGameManagement
{
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	
	import org.god.Net.IClientSocket;
	
	import org.aswing.geom.IntRectangle;
	//管理面板接口
	public interface IPanelImpl
	{
		function Create(param0:IMain,param1:IPlazaWindow):int;//创建
		function Destroy():void;//销毁
		function getNavCmd():int;//获取面板类型
		function getTitle():String;//获取面版标题
		function Active(data:*):int;//活动显示
		function Deactive():int;//取消活动显示
		
		function getActiveDC():Sprite;//获取当前活动显示对象
		function moveDC(cb:IntRectangle):void;//移动
		function validateDC():void;//刷新
		function setVisibleDC(b:Boolean):void;//设置有效
		
		function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean;//处理网络消息
	}
}
include "../Net/InterfaceModuleIDef.as"
include "../Net/NetModuleIDef.as"
include "../Net/GLOBALDEF.as"
include "../Net/CMD_Plaza.as"
