package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//桌子状态结构
	public class  tagTableStatus
	{
		public static const sizeof_tagTableStatus:int = 1 + 1;
		
		public var		bTableLock:uint;//锁定状态
		public var		bPlayStatus:uint;//游戏状态
		public function tagTableStatus()
		{
		}
		public static function readData(data:ByteArray):tagTableStatus
		{
			var pos:int=data.position;
			var result:tagTableStatus=new tagTableStatus;

			result.bTableLock=data.readUnsignedByte();
			result.bPlayStatus=data.readUnsignedByte();
			
			data.position=pos;
			return result;
		}
	}

}
include "GLOBALDEF.as"
