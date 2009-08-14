package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//游戏房间信息
	public class CMD_GR_ServerInfo
	{
		public var wKindID:uint;//类型 I D
		public var wGameGenre:uint;//游戏类型
		public var wTableCount:uint;//桌子数目
		public var wChairCount:uint;//椅子数目
		public function CMD_GR_ServerInfo()
		{
			
		}
		public static function readData(data:ByteArray):CMD_GR_ServerInfo
		{
			var pos:int=data.position;
			var result:CMD_GR_ServerInfo=new CMD_GR_ServerInfo;
			result.wKindID=data.readUnsignedShort();
			result.wGameGenre=data.readUnsignedShort();
			result.wTableCount=data.readUnsignedShort();
			result.wChairCount=data.readUnsignedShort();
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as";
