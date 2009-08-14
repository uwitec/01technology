package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//查看地址
	public class CMD_GR_LookUserIP
	{
		public static const sizeof_CMD_GR_LookUserIP:uint = 4;

		public var dwTargetUserID:uint;//目标用户
		
		public function CMD_GR_LookUserIP()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwTargetUserID);
			
			return result;
		}
	}
}

include "../Common/Memory.as"
