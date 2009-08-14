package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//初始发牌
	public class CMD_S_InitCard
	{
		public static  const sizeof_CMD_S_InitCard:uint = 1 + 4 + GAME_PLAYER + 1;

		public var cbFundusCard:uint;//底牌扑克
		public var bCanCutCard:uint;//能否分牌
		public var cbCardData:Array = new Array(GAME_PLAYER);//用户扑克
		public var cbCardSum:uint;//扑克总值

		public function CMD_S_InitCard()
		{
		}
		public static function readData(data:ByteArray):CMD_S_InitCard
		{
			var pos:int=data.position;
			var result:CMD_S_InitCard=new CMD_S_InitCard;

			result.cbFundusCard = data.readUnsignedByte();
			result.bCanCutCard = data.readUnsignedInt();
			for (var i:uint = 0; i < result.cbCardData.length; i ++)
			{
				result.cbCardData[i] = data.readUnsignedByte();
			}
			result.cbCardSum = data.readUnsignedByte();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"