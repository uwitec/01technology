package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//版本信息
	public class CMD_GF_Info
	{
		public static const sizeof_CMD_GF_Info:int = 1;
		
		public var bAllowLookon:uint;//旁观标志
		public function CMD_GF_Info()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeByte(bAllowLookon);

			return result;
		}
	}
}
include "../Common/Memory.as"
