package org.god.GameModule.B_Land.Common
{
	import flash.utils.ByteArray;
	//用户叫分
	public class CMD_C_LandScore
	{
		public static const sizeof_CMD_C_LandScore:uint = 1;
		public var bLandScore:uint;//地主分数
		public function CMD_C_LandScore()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();
			result.writeByte(bLandScore);
			return result;
		}
	}
}
include "../../../Common/Memory.as"