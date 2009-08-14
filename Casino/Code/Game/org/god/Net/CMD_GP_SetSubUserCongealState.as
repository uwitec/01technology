package org.god.Net
{
	import flash.utils.ByteArray;

	//子帐号停用启用状态
	public class CMD_GP_SetSubUserCongealState
	{
		public static  const sizeof_CMD_GP_SetSubUserCongealState:uint = 4 + 4 +  1;

		public var dwOperationUserID:uint;//操作用户ID
		public var dwUserID:uint;//被操作用户ID
		public var cbState:uint;//0-启用 1-停用
		
		public function CMD_GP_SetSubUserCongealState()
		{

		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeUnsignedInt(dwUserID);
			result.writeByte(cbState);

			return result;
		}
	}
}
include "../Common/Memory.as"