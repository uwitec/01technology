package org.god.Net
{
	import flash.utils.ByteArray;

	//游戏信息
	public class CMD_GP_GameInfo
	{
		public static  const sizeof_CMD_GP_GameInfo:uint=128 + 64 + 64;

		public var szMainWebPageAddress:String="";//游戏首页地址
		public var szFieldLiveVideoAddress:String="";//现场地址
		public var szVideoTapeAddress:String="";//视频录象地址

		public function CMD_GP_GameInfo()
		{

		}
		public static  function readData(data:ByteArray):CMD_GP_GameInfo
		{
			var pos:int=data.position;
			var result:CMD_GP_GameInfo=new CMD_GP_GameInfo  ;

			result.szMainWebPageAddress=readStringByByteArray(data,128);
			result.szFieldLiveVideoAddress=readStringByByteArray(data,64);
			result.szVideoTapeAddress=readStringByByteArray(data,64);

			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"