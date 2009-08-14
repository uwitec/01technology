package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//登陆成功
	public class CMD_GP_LogonSuccess
	{
		public var UserData:tagUserData = new tagUserData;
		public var cbUserType:uint;
		public var dwRealUserID:uint;
		
		public function CMD_GP_LogonSuccess()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_LogonSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_LogonSuccess=new CMD_GP_LogonSuccess;

			result.UserData=tagUserData.readData(data);
			data.position = pos + tagUserData.sizeof_tagUserData;
			result.cbUserType=data.readByte();
			result.dwRealUserID=data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
