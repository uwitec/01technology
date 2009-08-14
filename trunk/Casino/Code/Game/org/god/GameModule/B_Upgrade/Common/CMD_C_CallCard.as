package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//用户叫牌
	public class CMD_C_CallCard
	{
		public static  const sizeof_CMD_C_CallCard:uint=1 + 1;

		public var cbCallCard:uint;//叫牌扑克
		public var cbCallCount:uint;//叫牌数目

		public function CMD_C_CallCard()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			result.writeByte(cbCallCard);
			result.writeByte(cbCallCount);
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"