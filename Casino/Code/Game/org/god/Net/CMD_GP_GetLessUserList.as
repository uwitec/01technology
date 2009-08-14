package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//帐号获取
	public class CMD_GP_GetLessUserList
	{
		public static const sizeof_CMD_GP_GetLessUserList:int = 4;

		public var dwParentUserID:uint;	//上线帐号ID

		public function CMD_GP_GetLessUserList()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwParentUserID);
			
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"