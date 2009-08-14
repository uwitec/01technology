package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//数据包结构信息
	public class CMD_Info
	{
		public static const sizeof_CMD_Info:int=2 + 1 + 1;
		
		public var wDataSize:uint;//数据大小
		public var cbCheckCode:int;//效验字段
		public var cbMessageVer:int;//版本标识
		public function CMD_Info()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeShort(wDataSize);
			result.writeByte(cbCheckCode);
			result.writeByte(cbMessageVer);

			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
