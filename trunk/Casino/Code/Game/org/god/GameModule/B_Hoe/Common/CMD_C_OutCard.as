package org.god.GameModule.B_Hoe.Common
{
	import flash.utils.ByteArray;

	//出牌数据包
	public class CMD_C_OutCard
	{
		public static  const sizeof_CMD_C_OutCard:uint=1 + 13;
		public static  const sizeof_bCardData:uint = 13;

		public var bCardCount:uint;//出牌数目
		public var bCardData:Array=new Array(13);//扑克列表
		
		public function CMD_C_OutCard()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			result.writeByte(bCardCount);
			for (var i:uint=0; i < bCardCount; i++)
			{
				result.writeByte(bCardData[i]);
			}
			return result;
		}
	}
}
include "./CMD_Hoe.as"
include "../../../Common/Memory.as"