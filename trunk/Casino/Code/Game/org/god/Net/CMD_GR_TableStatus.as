package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//桌子状态信息
	public class CMD_GR_TableStatus
	{
		public static const sizeof_CMD_GR_TableStatus:int = 2 + 1;
		public var wTableID:uint;//桌子号码
		public var bPlayStatus:uint;//游戏状态
		public function CMD_GR_TableStatus()
		{
		}
		public static function readData(data:ByteArray):CMD_GR_TableStatus
		{
			var pos:int=data.position;
			var result:CMD_GR_TableStatus=new CMD_GR_TableStatus;

			result.wTableID=data.readUnsignedShort();
			result.bPlayStatus=data.readUnsignedByte();
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as";
