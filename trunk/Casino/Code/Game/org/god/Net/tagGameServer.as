package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//游戏房间列表结构
	public class tagGameServer
	{
		public static const sizeof_tagGameServer = 2 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 2 + SERVER_LEN;
		public var wSortID:uint;//排序号码
		public var wKindID:uint;//遊戲号码
		public var wTypeID:uint;//類型号码
		public var wServerID:uint;//服務号码
		public var wStationID:uint;//站点号码
		public var wServerPort:uint;//房间端口
		public var dwServerAddr:uint;//房间地址
		public var dwOnLineCount:uint;//在线人数
		public var wMaxConnect:uint;//最大连接
		public var szServerName:String = new String;//房间名称
		public function tagGameServer()
		{
			//szServerName.length = SERVER_LEN;
		}
		public static function readData(data:ByteArray):tagGameServer
		{
			var pos:int=data.position;
			var result:tagGameServer=new tagGameServer;
			result.wSortID = data.readUnsignedShort();
			result.wKindID = data.readUnsignedShort();
			result.wTypeID = data.readUnsignedShort();
			result.wServerID 	= data.readUnsignedShort();
			result.wStationID 	= data.readUnsignedShort();
			result.wServerPort 	= data.readUnsignedShort();
			result.dwServerAddr = data.readUnsignedInt();
			result.dwOnLineCount = data.readUnsignedInt();
			result.wMaxConnect = data.readUnsignedShort();
			result.szServerName = readStringByByteArray(data, SERVER_LEN);
			data.position = pos;
			return result;
		}
	}
}
include "Globaldef.as"
include "../Common/Memory.as"
