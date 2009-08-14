package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//游戏配置
	public class CMD_GF_Option
	{
		public static const sizeof_CMD_GF_Option:int = 1 + 1;
		
		public var bGameStatus:uint;//游戏状态
		public var bAllowLookon:uint;//允许旁观
		public function CMD_GF_Option()
		{
		}
		public static function readData(data:ByteArray):CMD_GF_Option
		{
			var pos:int=data.position;
			
			var result:CMD_GF_Option=new CMD_GF_Option;
			result.bGameStatus=data.readUnsignedByte();
			result.bAllowLookon=data.readUnsignedByte();
			data.position=pos;
			return result;
		}
	}
}
include "../Common/Memory.as"
