package org.god.Net
{

	import flash.utils.ByteArray;
	import flash.errors.EOFError;

	public interface IClientSocket
	{
		//设置接口
		function SetSocketSink(pIUnknownEx:IClientSocketSink):Boolean;
		//获取接口
		function GetSocketSink(Guid:String,dwQueryVer:uint):IClientSocketSink;
		//添加接口
		function AddSocketRecvSink(pIUnknownEx:IClientSocketRecvSink):Boolean;
		//添加接口
		function RemoveSocketRecvSink(pIUnknownEx:IClientSocketRecvSink):Boolean;
		//获取发送间隔
		function GetLastSendTick():uint;
		//获取接收间隔
		function GetLastRecvTick():uint;
		//获取发送数目
		function GetSendPacketCount():uint;
		//获取接收数目
		function GetRecvPacketCount():uint;
		//获取状态
		function GetConnectState():int;
		//连接服务器
		function Connect(szServerIP:String,wPort:int):Boolean;
		//发送函数
		function SendCmd(wMainCmdID:int,wSubCmdID:int):Boolean;
		//发送函数
		function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean;
		//关闭连接
		function CloseSocket(bNotify:Boolean):Boolean;
	}
}
