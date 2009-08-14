package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//登陆成功
	public class CMD_GR_LookUserIPResult
	{
		public static  const sizeof_CMD_GR_LookUserIPResult:uint=4 + NAME_LEN + NAME_LEN + 4 + 256;

		public var dwTargetUserID:uint;//目标用户
		public var szAccount:String="";//帐号
		public var szName:String="";//名称
		public var dwUserIP:uint;//IP
		public var szIPDescribe:String="";//IP描叙

		public function CMD_GR_LookUserIPResult()
		{
		}
		public static function readData(data:ByteArray):CMD_GR_LookUserIPResult
		{
			var pos:int=data.position;
			var result:CMD_GR_LookUserIPResult=new CMD_GR_LookUserIPResult  ;

			result.dwTargetUserID=data.readUnsignedInt();
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			result.szName = readStringByByteArray(data, NAME_LEN);
			result.dwUserIP=data.readUnsignedInt();
			result.szIPDescribe = readStringByByteArray(data, 256);
			
			data.position=pos;
			
			return result;
		}
	}
}
include "GLOBALDEF.as";
include "../Common/Memory.as"