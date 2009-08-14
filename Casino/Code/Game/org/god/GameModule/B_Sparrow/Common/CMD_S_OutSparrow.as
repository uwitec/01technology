package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//出牌命令
	public class CMD_S_OutSparrow
	{
		public static const sizeof_CMD_S_OutSparrow:uint = 2 + 1 ;
		
		public var wOutSparrowUser:uint;//出牌用户
		public var cbOutSparrowData:uint;//出牌扑克

		public function CMD_S_OutSparrow()
		{
		}
		public static function readData(data:ByteArray):CMD_S_OutSparrow
		{
			var pos:int=data.position;
			var result:CMD_S_OutSparrow=new CMD_S_OutSparrow;
			result.wOutSparrowUser = data.readUnsignedShort();
			result.cbOutSparrowData = data.readUnsignedByte();
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"