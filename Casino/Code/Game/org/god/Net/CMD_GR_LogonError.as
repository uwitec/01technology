package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//登陆失败
	public class CMD_GR_LogonError
	{
		public var wErrorCode:uint;//错误代码
		public function CMD_GR_LogonError()
		{
		}
		public static function readData(data:ByteArray):CMD_GR_LogonError
		{
			var pos:int=data.position;
			var result:CMD_GR_LogonError=new CMD_GR_LogonError;

			result.wErrorCode= data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}

}
include "GLOBALDEF.as";
