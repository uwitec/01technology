package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//游戏站点结构
	public class tagGameStation
	{
		static const sizeof_tagGameStation:int = 2 + 2 + 2 + 2 + 1 + STATION_LEN;
		public var wSortID:uint;//排序号码
		public var wKindID:uint;//名称号码
		public var wJoinID:uint;//挂接号码
		public var wStationID:uint;//站点号码
		public var szStationName:String = new String;//站点名称
		public function tagGameStation()
		{
			//szStationName.length = STATION_LEN;
		}
		public static function readData(data:ByteArray):tagGameStation
		{
			var pos:int=data.position;
			var result:tagGameStation=new tagGameStation;
			result.wSortID = data.readUnsignedShort();
			result.wKindID = data.readUnsignedShort();
			result.wJoinID = data.readUnsignedShort();
			result.wStationID = data.readUnsignedShort();
			result.szStationName = readStringByByteArray(data, STATION_LEN);
			data.position = pos;
			return result;
		}
	}
}
include "Globaldef.as"
include "../Common/Memory.as"
