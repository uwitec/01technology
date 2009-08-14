package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//用户出牌
	public class CMD_C_OutCard
	{
		public static  const sizeof_CMD_C_OutCard:uint=1 + 52;
		public static  const sizeof_cbCardData:uint = 52;
		public var cbCardCount:uint;//出牌数目
		public var cbCardData:Array=new Array(52);//扑克列表

		public function CMD_C_OutCard()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			result.writeByte(cbCardCount);
			for (var i:uint=0; i < cbCardCount; i++)
			{
				result.writeByte(cbCardData[i]);
			}
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"