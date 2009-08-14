package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//人数信息
	public class  tagGameServerOnLineCountInfo
	{
		public static const sizeof_tagGameServerOnLineCountInfo:int = 2 + 4;

		public var		wServerID:uint;//类型标识
		public var		dwOnLineCount:uint;//在线人数
		public function tagGameServerOnLineCountInfo()
		{
		}
		public static function readData(data:ByteArray):tagGameServerOnLineCountInfo
		{
			var pos:int=data.position;
			var result:tagGameServerOnLineCountInfo=new tagGameServerOnLineCountInfo;
			
			result.wServerID = data.readUnsignedShort();
			result.dwOnLineCount = data.readUnsignedInt();

			data.position = pos;
			return result;
		}
	}
}

