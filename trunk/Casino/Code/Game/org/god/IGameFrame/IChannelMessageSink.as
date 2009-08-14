package org.god.IGameFrame
{
	import flash.utils.ByteArray;
	//信道数据处理
	public interface IChannelMessageSink
	{
		//处理信道消息
		function OnIPCChannelMessage(wMainCmdID:uint,
								  wSubCmdID:uint,
								  pIPCBuffer:*, 
								  wDataSize:uint):Boolean;
		//处理网络消息
		function OnIPCSocketMessage(wMainCmdID:uint,
								wSubCmdID:uint,
								pBuffer:ByteArray,
								wDataSize:int):Boolean;
	}
}