package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//注册子帐号成功
	public class CMD_GP_RegisterSubUserSuccess
	{
		public static const sizeof_CMD_GP_RegisterSubUserSuccess = tagSubUserData.sizeof_tagSubUserData;
		
		public var UserData:tagSubUserData = new tagSubUserData;
		
		public function CMD_GP_RegisterSubUserSuccess()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_RegisterSubUserSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_RegisterSubUserSuccess=new CMD_GP_RegisterSubUserSuccess;

			result.UserData=tagSubUserData.readData(data);
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
