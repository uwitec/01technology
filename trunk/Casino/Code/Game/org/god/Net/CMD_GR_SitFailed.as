package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//坐下失败
	public class CMD_GR_SitFailed
	{
		public static const sizeof_CMD_GR_SitFailed:int = 2;
		public var wErrorCode:uint;//错误描述
		public function CMD_GR_SitFailed()
		{
		}
		public static function readData(data:ByteArray):CMD_GR_SitFailed
		{
			var pos:int=data.position;
			var result:CMD_GR_SitFailed=new CMD_GR_SitFailed;

			result.wErrorCode= data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"