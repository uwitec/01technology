package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//操作提示
	public class CMD_S_OperateNotify
	{
		public static const sizeof_CMD_S_OperateNotify:uint = 2 + 1 + 1 ;
		
		public var wResumeUser:uint;//还原用户
		public var cbActionMask:uint;//动作掩码
		public var cbActionSparrow:uint;//动作扑克

		public function CMD_S_OperateNotify()
		{
		}
		public static function readData(data:ByteArray):CMD_S_OperateNotify
		{
			var pos:int=data.position;
			var result:CMD_S_OperateNotify=new CMD_S_OperateNotify;
			result.wResumeUser 	   = data.readUnsignedShort();
			result.cbActionMask    = data.readUnsignedByte();
			result.cbActionSparrow = data.readUnsignedByte();
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"