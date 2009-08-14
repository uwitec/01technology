package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//游戏类型结构
	public class tagGameType
	{
		public static const sizeof_tagGameType:int = 2 + 2 + 1 + TYPE_LEN;
		
		public var wSortID:uint;//排序号码
		public var wTypeID:uint;//种类号码
		public var nbImageID:uint;//圖標號碼
		public var szTypeName:String = new String;//种类名字
		public function tagGameType()
		{
			//szTypeName.length = TYPE_LEN;
		}
		public static function readData(data:ByteArray):tagGameType
		{
			var pos:int=data.position;
			var result:tagGameType=new tagGameType;
			result.wSortID = data.readUnsignedShort();
			result.wTypeID = data.readUnsignedShort();
			result.nbImageID = data.readUnsignedByte();
			result.szTypeName = readStringByByteArray(data, TYPE_LEN);
			data.position = pos;
			return result;
		}
	}
}
include "Globaldef.as"
include "../Common/Memory.as"
