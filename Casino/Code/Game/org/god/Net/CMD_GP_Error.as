package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class CMD_GP_Error
	{
		public var wErrorCode:uint;//错误代码
		public function CMD_GP_Error()
		{
	
		}
		public static function readData(data:ByteArray):CMD_GP_Error
		{
			var pos:int=data.position;
			var result:CMD_GP_Error=new CMD_GP_Error;

			result.wErrorCode= data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}
}