package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//帐号登录
	public class CMD_GP_LogonByAccount
	{
		public static const sizeof_CMD_GP_LogonByAccount:int = 4 + NAME_LEN + 
		PASS_LEN + PASS_LEN + 1;
		
		public var dwPlazaVersion:uint;//广场版本
		public var szAccounts:String = "";//登录帐号
		public var szPassWord:String = "";//登录密码
		public var szRealityPass:String = "";//真实密码
		public var cbClientType:uint;//客户端类型
		
		public function CMD_GP_LogonByAccount()
		{
			//szAccounts.length = NAME_LEN;
			//szPassWord.length = PASS_LEN;
			//szRealityPass.length = PASS_LEN;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwPlazaVersion);
			writeStringToByteArray(result, szAccounts, NAME_LEN);
			writeStringToByteArray(result, szPassWord, PASS_LEN);
			writeStringToByteArray(result, szRealityPass, PASS_LEN);
			result.writeByte(cbClientType);
			
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"