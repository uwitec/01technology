package org.god.Net
{
	import flash.utils.ByteArray;

	//注册子帐号
	public class CMD_GP_RegisterSubUser
	{
		public static  const sizeof_CMD_GP_RegisterSubUser:uint = 
											4 + 4 + NAME_LEN + NAME_LEN + PASS_LEN;

		public var dwOperationUserID:uint;//操作用户ID
		public var dwParentUserID:uint;//父亲帐号ID
		public var szAccount:String="";//名称
		public var szName:String="";//名称
		public var szPassWord:String = "";//登录密码

		public function CMD_GP_RegisterSubUser()
		{

		}
		
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeUnsignedInt(dwParentUserID);
			writeStringToByteArray(result, szAccount, NAME_LEN);
			writeStringToByteArray(result, szName, NAME_LEN);
			writeStringToByteArray(result, szPassWord, PASS_LEN);

			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"