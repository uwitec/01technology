package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//检测结构信息
	public class CMD_KN_DetectSocket
	{
		public var dwSendTickCount:uint;//发送时间
		public var dwRecvTickCount:uint;//接收时间
		public static function readData(data:ByteArray):CMD_KN_DetectSocket
		{
			var pos:int=data.position;
			var result:CMD_KN_DetectSocket=new CMD_KN_DetectSocket;

			result.dwSendTickCount=data.readUnsignedInt();
			result.dwRecvTickCount=data.readUnsignedInt();
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
