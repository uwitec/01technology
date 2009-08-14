package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//网络数据包缓冲
	public class CMD_Buffer
	{
		public var Head:CMD_Head=new CMD_Head;//数据包头
		public var cbBuffer:ByteArray=newLitteEndianByteArray();//数据缓冲
		public function CMD_Buffer()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeBytes(Head.toByteArray());
			result.writeBytes(cbBuffer);

			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
