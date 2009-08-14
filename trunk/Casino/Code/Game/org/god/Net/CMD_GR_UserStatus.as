package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//用户状态
	public class CMD_GR_UserStatus
	{
		public static const sizeof_CMD_GR_UserStatus = 4 + 2 + 2 + 2 + 1;
		public var dwUserID:uint;//数据库 ID
		public var wTableID:uint;//桌子位置
		public var wChairID:uint;//椅子位置
		public var wNetDelay:uint;//网络延时
		public var cbUserStatus:uint;//用户状态
		public function CMD_GR_UserStatus()
		{
		}
		public static function readData(data:ByteArray):CMD_GR_UserStatus
		{
			var pos:int=data.position;
			var result:CMD_GR_UserStatus=new CMD_GR_UserStatus;
			result.dwUserID = data.readUnsignedInt();
			result.wTableID = data.readUnsignedShort();
			result.wChairID = data.readUnsignedShort();
			result.wNetDelay  = data.readUnsignedShort();
			result.cbUserStatus  = data.readByte();
			data.position = pos;
			return result;
		}
	}
}
include "GLOBALDEF.as";
