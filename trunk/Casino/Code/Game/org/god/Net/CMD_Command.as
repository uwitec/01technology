package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//数据包命令信息
	public class CMD_Command
	{
		public static const sizeof_CMD_Command:int=2 + 2;
		
		public var wMainCmdID:uint;//主命令码
		public var wSubCmdID:uint;//子命令码
		public function CMD_Command()
		{
		}
		public static function readData(data:ByteArray):CMD_Command
		{
			var pos:int=data.position;
			
			var result:CMD_Command=new CMD_Command;
			result.wMainCmdID=data.readUnsignedShort();
			result.wSubCmdID=data.readUnsignedShort();
			data.position=pos;
			return result;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray =newLitteEndianByteArray();
			
			result.writeShort(wMainCmdID);
			result.writeShort(wSubCmdID);

			return result;
		}
	}

}
include "GLOBALDEF.as"
include "../Common/Memory.as"
