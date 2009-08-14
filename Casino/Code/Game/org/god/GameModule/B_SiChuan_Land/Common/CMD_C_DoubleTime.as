package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;
	//用户反牌
	public class CMD_C_DoubleTime
	{
		public static const sizeof_CMD_C_DoubleTime:uint = 1;
		public var bDoubleTime:uint;//反牌 1-不反 2-反牌
		public function CMD_C_DoubleTime()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();
			result.writeByte(bDoubleTime);
			return result;
		}
	}
}
include "../../../Common/Memory.as"