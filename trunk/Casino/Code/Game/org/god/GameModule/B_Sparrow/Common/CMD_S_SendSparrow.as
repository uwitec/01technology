package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//发送扑克
	public class CMD_S_SendSparrow
	{
		public static const sizeof_CMD_S_SendSparrow:uint = 1 + 1 + 2;
		
		public var cbSparrowData:uint;//扑克数据
		public var cbActionMask:uint;//动作掩码
		public var wCurrentUser:uint;//当前用户

		public function CMD_S_SendSparrow()
		{
		}
		public static function readData(data:ByteArray):CMD_S_SendSparrow
		{
			var pos:int=data.position;
			var result:CMD_S_SendSparrow=new CMD_S_SendSparrow;
			result.cbSparrowData = data.readUnsignedByte();
			result.cbActionMask = data.readUnsignedByte();
			result.wCurrentUser = data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"