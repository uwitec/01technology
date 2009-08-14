package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//用户状态
	public class IPC_UserStatus
	{
		public static  const sizeof_IPC_UserStatus = 4 + 2 + 1;
		public var dwUserID;//用户 I D
		public var wNetDelay;//网络延时
		public var cbUserStatus;//用户状态
		public function IPC_UserStatus()
		{

		}
		public static function readData(data:ByteArray):IPC_UserStatus
		{
			var pos:int=data.position;
			var result:IPC_UserStatus=new IPC_UserStatus;
			result.dwUserID = data.readUnsignedInt();
			result.wNetDelay = data.readUnsignedShort();
			result.cbUserStatus = data.readUnsignedByte();
			data.position = pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
