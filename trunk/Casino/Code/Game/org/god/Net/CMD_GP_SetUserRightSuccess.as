package org.god.Net
{
	import flash.utils.ByteArray;

	//帐号权限
	public class CMD_GP_SetUserRightSuccess
	{
		public static  const sizeof_CMD_GP_SetUserRightSuccess:uint = 4 + 4 + 4;

		public var dwUserID:uint;//被操作用户ID
		public var dwUserRight:uint;//游戏权限
		public var dwMasterRight:uint;//管理权限

		public function CMD_GP_SetUserRightSuccess()
		{

		}
		public static function readData(data:ByteArray):CMD_GP_SetUserRightSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetUserRightSuccess=new CMD_GP_SetUserRightSuccess;
			result.dwUserID=data.readUnsignedInt();
			result.dwUserRight=data.readUnsignedInt();
			result.dwMasterRight=data.readUnsignedInt();

			data.position=pos;
			return result;
		}
	}
}