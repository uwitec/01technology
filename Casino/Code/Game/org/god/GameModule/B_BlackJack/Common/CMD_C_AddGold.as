package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//用户下注
	public class CMD_C_AddGold
	{
		public static  const sizeof_CMD_C_AddGold:uint=4 + 8;

		public var bDoubleGold:uint;//是否双倍
		public var fGold:Number;//下注数目

		public function CMD_C_AddGold()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();
			result.writeUnsignedInt(bDoubleGold);
			result.writeDouble(fGold);
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"