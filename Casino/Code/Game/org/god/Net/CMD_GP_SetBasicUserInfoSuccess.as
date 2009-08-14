package org.god.Net
{
	import flash.utils.ByteArray;

	//修改基本信息成功
	public class CMD_GP_SetBasicUserInfoSuccess
	{
		public static  const sizeof_CMD_GP_SetBasicUserInfoSuccess:uint = 
											 4 + NAME_LEN + 1;

		public var dwUserID:uint;//被操作用户ID
		public var szName:String="";//名称
		public var cbFaceID:uint;
		
		public function CMD_GP_SetBasicUserInfoSuccess()
		{

		}
		public static function readData(data:ByteArray):CMD_GP_SetBasicUserInfoSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetBasicUserInfoSuccess=new CMD_GP_SetBasicUserInfoSuccess;

			result.dwUserID= data.readUnsignedInt();
			result.szName = readStringByByteArray(data, NAME_LEN);
			result.cbFaceID = data.readUnsignedByte();

			data.position=pos;
			return result;
		}
		
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"