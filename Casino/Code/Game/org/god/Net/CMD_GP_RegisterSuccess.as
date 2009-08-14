package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	import org.god.Net.*;
	
	//注册成功
	public class CMD_GP_RegisterSuccess
	{
		public static const sizeof_CMD_GP_RegisterSuccess = tagUserData.sizeof_tagUserData;
		
		public var UserData:tagUserData = new tagUserData;
		
		public function CMD_GP_RegisterSuccess()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_RegisterSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_RegisterSuccess=new CMD_GP_RegisterSuccess;

			result.UserData=tagUserData.readData(data);
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
