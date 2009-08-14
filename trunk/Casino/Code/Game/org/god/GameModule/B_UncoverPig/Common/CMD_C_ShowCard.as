package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//亮牌数据包
	public class CMD_C_ShowCard
	{
		public static  const sizeof_CMD_C_ShowCard:uint=4 + 1;

		public var bShowCard:Array=new Array(4);//亮牌列表
		public var bShowCardCount:uint;//亮牌数目

		public function CMD_C_ShowCard()
		{
			ZeroArray(bShowCard);
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();

			for (var i:uint=0; i < bShowCard.length; i++)
			{
				result.writeByte(bShowCard[i]);
			}
			result.writeByte(bShowCardCount);

			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"