package org.god.Net
{
	import flash.utils.ByteArray;

	//修改基本信息成功
	public class CMD_GP_SetMySelfBasicUserInfoSuccess
	{
		public static  const sizeof_CMD_GP_SetMySelfBasicUserInfoSuccess:uint = 
											 4 + NAME_LEN + PASS_LEN + 1;

		public var dwUserID:uint;//被操作用户ID
		public var szName:String="";//名称
		public var szNewRealityPassWord:String="";//登录密码
		public var cbFaceID:uint;
		
		public function CMD_GP_SetMySelfBasicUserInfoSuccess()
		{

		}
		public static function readData(data:ByteArray):CMD_GP_SetMySelfBasicUserInfoSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetMySelfBasicUserInfoSuccess=new CMD_GP_SetMySelfBasicUserInfoSuccess;

			result.dwUserID= data.readUnsignedInt();
			result.szName = readStringByByteArray(data, NAME_LEN);
			result.szNewRealityPassWord = readStringByByteArray(data, PASS_LEN);
			result.cbFaceID = data.readUnsignedByte();

			data.position=pos;
			return result;
		}
		
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"