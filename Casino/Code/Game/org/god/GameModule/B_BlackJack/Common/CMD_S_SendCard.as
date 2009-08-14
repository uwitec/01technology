package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	
	//发牌数据包
	public class CMD_S_SendCard
	{
		public static  const sizeof_CMD_S_SendCard:uint = 2 + 1 + 2 + 1 + 4;

		public var wCurrentUser:uint;//当前用户
		public var cbUserCard:uint;//用户扑克
		public var wGetCardUser:uint;//要牌玩家
		public var cbCardSum:uint;//扑克总值
		public var bCutCard:uint;//用户分牌（0:左手牌，1:右手牌）
		
		public function CMD_S_SendCard()
		{
		}
		public static function readData(data:ByteArray):CMD_S_SendCard
		{
			var pos:int=data.position;
			var result:CMD_S_SendCard=new CMD_S_SendCard;

			result.wCurrentUser = data.readUnsignedShort();
			result.cbUserCard = data.readUnsignedByte();
			result.wGetCardUser = data.readUnsignedShort();
			result.cbCardSum = data.readUnsignedByte();
			result.bCutCard = data.readUnsignedInt();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"