package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//踢出用户
	public class CMD_GR_KillUser
	{
		public static const sizeof_CMD_GR_KillUser:uint = 4;
		public var dwTargetUserID:uint;//目标用户
		public function CMD_GR_KillUser()
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
