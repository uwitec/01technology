package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//底牌扑克
	public class CMD_C_ConcealCard
	{
		public static  const sizeof_CMD_C_ConcealCard:uint=1 + 8;

		public var cbConcealCount;//暗藏数目
		public var cbConcealCard:Array=new Array(8);//暗藏扑克

		public function CMD_C_ConcealCard()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			result.writeByte(cbConcealCount);
			for (var i:uint=0; i < cbConcealCard.length; i++)
			{
				result.writeByte(cbConcealCard[i]);
			}
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"