package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//用户出牌
	public class CMD_S_OutCard
	{
		public static  const sizeof_CMD_S_OutCard:uint=1 + 2 + 2 + 1;
		public static  const sizeof_bCardData:uint = 1;
		
		public var bCardCount:uint;//出牌数目
		public var wCurrentUser:uint;//当前玩家
		public var wOutCardUser:uint;//出牌玩家
		public var bCardData:uint;//出牌扑克

		public function CMD_S_OutCard()
		{

		}
		public static  function readData(data:ByteArray):CMD_S_OutCard
		{
			var pos:int=data.position;
			var result:CMD_S_OutCard=new CMD_S_OutCard  ;

			result.bCardCount=data.readUnsignedByte();
			result.wCurrentUser=data.readUnsignedShort();
			result.wOutCardUser=data.readUnsignedShort();
			result.bCardData=data.readUnsignedByte();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"