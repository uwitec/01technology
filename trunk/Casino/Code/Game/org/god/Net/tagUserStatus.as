package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//用户状态信息
	public class tagUserStatus
	{
		public static const sizeof_tagUserStatus = 2 + 2 + 2 + 1;
		public var wTableID:uint;//桌子号码
		public var wChairID:uint;//椅子位置
		public var wNetDelay:uint;//网络延时
		public var cbUserStatus:uint;//用户状态
		public function tagUserStatus()
		{

		}
		public static function readData(data:ByteArray):tagUserStatus
		{
			var pos:int=data.position;
			var result:tagUserStatus=new tagUserStatus;
			result.wTableID = data.readUnsignedShort();
			result.wChairID= data.readUnsignedShort();
			result.wNetDelay= data.readUnsignedShort();
			result.cbUserStatus  = data.readByte();
			data.position = pos;
			return result;
		}
		public function clone():tagUserStatus
		{
			var result:tagUserStatus=new tagUserStatus;
			result.wTableID = this.wTableID;
			result.wChairID = this.wChairID;
			result.wNetDelay= this.wNetDelay;
			result.cbUserStatus  = this.cbUserStatus;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
