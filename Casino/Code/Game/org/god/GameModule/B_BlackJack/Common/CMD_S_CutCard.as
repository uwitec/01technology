package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;

	//用户分牌
	public class CMD_S_CutCard
	{
		public static  const sizeof_CMD_S_CutCard:uint = 2 + 1 + 1 + 1 + 1;

		public var wCutCardUser:uint;//分牌用户
		public var cbFirstCard:uint;//首张扑克
		public var cbSecondCard:uint;//次张扑克
		public var cbFirstCardSum:uint;//首张扑克
		public var cbSecondCardSum:uint;//次张扑克
		
		public function CMD_S_CutCard()
		{
		}
		public static function readData(data:ByteArray):CMD_S_CutCard
		{
			var pos:int=data.position;
			var result:CMD_S_CutCard=new CMD_S_CutCard;

			result.wCutCardUser = data.readUnsignedShort();
			result.cbFirstCard = data.readUnsignedByte();
			result.cbSecondCard = data.readUnsignedByte();
			result.cbFirstCardSum = data.readUnsignedByte();
			result.cbSecondCardSum = data.readUnsignedByte();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"