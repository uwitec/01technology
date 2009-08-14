package org.god.GameModule.B_Land.Common
{
	import flash.utils.ByteArray;
	//出牌数据包
	public class CMD_C_OutCard
	{
		public static const sizeof_CMD_C_OutCard:uint = 1 + 20;
		public var bCardCount:uint;//出牌数目
		public var bCardData:Array = new Array(20);//扑克列表
		public function CMD_C_OutCard()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();
			result.writeByte(bCardCount);
			for(var i:int = 0; i < bCardCount;i ++)
			{
				result.writeByte(bCardData[i]);
			}
			return result;
		}
	}
}
include "../../../Common/Memory.as"