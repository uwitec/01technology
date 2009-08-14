package org.god.Net
{
	import flash.utils.ByteArray;

	//状态设定成功
	public class CMD_GP_SetUserCongealStateSuccess
	{
		public static  const sizeof_CMD_GP_SetUserCongealStateSuccess:uint = 4 + 1 + 4;

		public var dwUserID:uint;//被操作用户ID
		public var cbState:uint;//0-启用 1-停用
		public var dwStateMark:uint;//掩码

		public function CMD_GP_SetUserCongealStateSuccess()
		{

		}
		public static function readData(data:ByteArray):CMD_GP_SetUserCongealStateSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetUserCongealStateSuccess=new CMD_GP_SetUserCongealStateSuccess;

			result.dwUserID=data.readUnsignedInt();
			result.cbState=data.readUnsignedByte();
			result.dwStateMark=data.readUnsignedInt();
			data.position=pos;
			return result;
		}
	}
}