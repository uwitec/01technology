package org.god.Net
{
	import flash.utils.ByteArray;

	public interface IClientSocketRecvSink
	{
		//网络读取消息
		 function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean;
	}
}
