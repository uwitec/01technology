package org.god.Net
{
	import flash.utils.ByteArray;

	//修改基本信息成功
	public class CMD_GP_SetBasicSubUserInfoSuccess
	{
		public static  const sizeof_CMD_GP_SetBasicSubUserInfoSuccess:uint = 
		 4 + 4 + NAME_LEN;

		public var dwUserID:uint;//被操作用户ID
		public var dwParentUserID;//被操作用户父亲ID
		public var szName:String="";//名称


		public function CMD_GP_SetBasicSubUserInfoSuccess()
		{

		}
		public static function readData(data:ByteArray):CMD_GP_SetBasicSubUserInfoSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetBasicSubUserInfoSuccess=new CMD_GP_SetBasicSubUserInfoSuccess;

			result.dwUserID= data.readUnsignedInt();
			result.dwParentUserID= data.readUnsignedInt();
			result.szName = readStringByByteArray(data, NAME_LEN);
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"