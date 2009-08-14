package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//出牌命令
	public class CMD_C_OutSparrow
	{
		public static const sizeof_CMD_C_OutSparrow:uint = 1  ;
		public var cbSparrowData:uint;//扑克数据

		public function CMD_C_OutSparrow()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();
			result.writeByte(cbSparrowData);
			return result;
		}
	}
}
include "../../../Common/Memory.as"