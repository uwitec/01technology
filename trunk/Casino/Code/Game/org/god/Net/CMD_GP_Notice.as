package org.god.Net
{
	import flash.utils.ByteArray;

	//公告
	public class CMD_GP_Notice
	{
		public static  const sizeof_CMD_GP_Notice:uint=512;


		public var szNotice:String="";//公告内容



		public function CMD_GP_Notice()
		{

		}
		public static  function readData(data:ByteArray):CMD_GP_Notice
		{
			var pos:int=data.position;
			var result:CMD_GP_Notice=new CMD_GP_Notice  ;

			result.szNotice=readStringByByteArray(data,512);
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"