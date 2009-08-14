package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//消息数据包
	public class CMD_GF_Message
	{
		public static const sizeof_CMD_GF_Message:uint = 2 + 2 + 2;
		
		public var wMessageType:uint;//消息类型
		public var wErrorCode:uint;//错误消息代码
		public var wMessageLength:uint;//消息长度
		public var szContent:String="";//消息内容
		public function CMD_GF_Message()
		{
			//szContent.length = [1024];
		}
		public static  function readData(data:ByteArray):CMD_GF_Message
		{
			var pos:int=data.position;
			var result:CMD_GF_Message=new CMD_GF_Message  ;
			result.wMessageType=data.readUnsignedShort();
			result.wErrorCode=data.readUnsignedShort();
			result.wMessageLength=data.readUnsignedShort();
			if(result.wMessageLength > 0)
			{
				result.szContent = readStringByByteArray(data, 1024);
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "../Common/Memory.as"
