package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//房间帐号登录
	public class CMD_GR_LogonByAccount
	{
		public static const sizeof_CMD_GR_LogonByAccount:int = 4 + 4 + NAME_LEN + PASS_LEN;
		public var dwPlazaVersion:uint;//广场版本
		public var dwProcessVersion:uint;//进程版本
		public var szAccounts:String=new String  ;//登录帐号
		public var szPassWord:String=new String  ;//登录密码
		public function CMD_GR_LogonByAccount()
		{
			//szAccounts.length = NAME_LEN;
			//szPassWord.length = PASS_LEN;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwPlazaVersion);
			result.writeUnsignedInt(dwProcessVersion);
			writeStringToByteArray(result, szAccounts, NAME_LEN);
			writeStringToByteArray(result, szPassWord, PASS_LEN);
			
			return result;
		}
	}
}
include "GLOBALDEF.as";
include "../Common/Memory.as"