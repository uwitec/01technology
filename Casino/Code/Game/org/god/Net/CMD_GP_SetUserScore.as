package org.god.Net
{
	import flash.utils.ByteArray;

	//存取额度
	public class CMD_GP_SetUserScore
	{
		public static const sizeof_CMD_GP_SetUserScore:uint = 4 + 4 + 4 + 8;
		
		public var dwOperationUserID:uint;//操作用户ID
		public var dwParentUserID:uint;//被存取用户上线ID
		public var dwUserID:uint;//被存取用户ID
		public var fScore:Number;//额度
		public function CMD_GP_SetUserScore()
		{

		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeUnsignedInt(dwParentUserID);
			result.writeUnsignedInt(dwUserID);
			result.writeDouble(fScore);
			
			return result;
		}
	}
}
include "../Common/Memory.as"