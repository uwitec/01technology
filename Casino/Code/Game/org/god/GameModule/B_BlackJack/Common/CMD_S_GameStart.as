package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//游戏开始
	public class CMD_S_GameStart
	{
		public static  const sizeof_CMD_S_GameStart:uint = 2 + 1 + 2 + 8 + 8 + GAME_PLAYER;

		public var wBankerStation:uint;//庄家位置
		public var cbFundusCard:uint;//底牌扑克
		public var wCurrentUser:uint;//当前玩家
		public var fMaxGold:Number;//最大下注
		public var fTurnBasicGold:Number;//最少下注
		public var cbCardData:Array = new Array(GAME_PLAYER);//用户扑克

		public function CMD_S_GameStart()
		{
		}
		public static function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart;

			result.wBankerStation = data.readUnsignedShort();
			result.cbFundusCard = data.readUnsignedByte();
			result.wCurrentUser = data.readUnsignedShort();
			result.fMaxGold = data.readDouble();
			result.fTurnBasicGold = data.readDouble();
			for(var i:uint = 0; i < result.cbCardData.length; i ++)
			{
					result.cbCardData[i] = data.readUnsignedByte();
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"