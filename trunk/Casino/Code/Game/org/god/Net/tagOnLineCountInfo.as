package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//人数信息
	public class  tagOnLineCountInfo
	{
		public static const sizeof_tagOnLineCountInfo:int = 2 + 2 + 4;
		public var 		wTypeID:uint;//种类标识
		public var		wKindID:uint;//类型标识
		public var		dwOnLineCount:uint;//在线人数
		public function tagOnLineCountInfo()
		{
		}
		public static function readData(data:ByteArray):tagOnLineCountInfo
		{
			var pos:int=data.position;
			var result:tagOnLineCountInfo=new tagOnLineCountInfo;
			
			result.wTypeID = data.readUnsignedShort();
			result.wKindID = data.readUnsignedShort();
			result.dwOnLineCount = data.readUnsignedInt();

			data.position = pos;
			return result;
		}
	}
}

