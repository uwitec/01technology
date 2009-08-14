package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//用户要牌
	public class CMD_C_GetCard
	{
		public static  const sizeof_CMD_C_GetCard:uint=4;

		public var bCutCard:uint;//用户分牌（0:左手牌，1:右手牌）

		public function CMD_C_GetCard()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			result.writeUnsignedInt(bCutCard);
			
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"