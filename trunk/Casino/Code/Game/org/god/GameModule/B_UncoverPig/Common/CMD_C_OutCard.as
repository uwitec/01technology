package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//出牌数据包
	public class CMD_C_OutCard
	{
		public static  const sizeof_CMD_C_OutCard:uint=1 + 1;
		public static  const sizeof_bCardData:uint = 1;

		public var bCardCount:uint;//出牌数目
		public var bCardData:uint;//扑克列表

		public function CMD_C_OutCard()
		{
			bCardCount= 0;
			bCardData= 0;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			
			result.writeByte(bCardCount);
			result.writeByte(bCardData);

			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"