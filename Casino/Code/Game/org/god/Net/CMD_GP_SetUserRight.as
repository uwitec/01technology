package org.god.Net
{
	import flash.utils.ByteArray;

	//帐号权限
	public class CMD_GP_SetUserRight
	{
		public static  const sizeof_CMD_GP_SetUserRight:uint = 4 + 4 + 4 + 4;

		public var dwOperationUserID:uint;//操作用户ID
		public var dwUserID:uint;//被操作用户ID
		public var dwUserRight:uint;//游戏权限
		public var dwMasterRight:uint;//管理权限
		
		public function CMD_GP_SetUserRight()
		{

		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeUnsignedInt(dwUserID);
			result.writeUnsignedInt(dwUserRight);
			result.writeUnsignedInt(dwMasterRight);

			return result;
		}
	}
}
include "../Common/Memory.as"