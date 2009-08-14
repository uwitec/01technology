package org.god.Net
{
	import flash.utils.ByteArray;
	//申请帐号
	public class CMD_GP_RegisterUser
	{
		public static  const sizeof_CMD_GP_RegisterUser:uint = 4 + 
		tagUserData.sizeof_tagUserData + PASS_LEN;

		public var dwOperationUserID:uint;//操作用户ID
		public var UserData:tagUserData = new tagUserData;//用户信息
		public var szPassWord:String;//登录密码

		public function CMD_GP_RegisterUser()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			UserData.writeByteArray(result);
			writeStringToByteArray(result, szPassWord, PASS_LEN);
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"