package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;

	//游戏结束
	public class CMD_S_GameEnd
	{
		public static  const sizeof_CMD_S_GameEnd:uint = GAME_PLAYER * 8 + GAME_PLAYER +
		GAME_PLAYER + GAME_PLAYER + GAME_PLAYER + 2 + GAME_PLAYER + 2*GAME_PLAYER*8 + 
		GAME_PLAYER + GAME_PLAYER * 13;

		public var fGameGold:Array = new Array(GAME_PLAYER);//游戏得分
		public var cbUserCard:Array = new Array(GAME_PLAYER);//用户扑克
		public var cbCutCard:Array = new Array(GAME_PLAYER);//分牌扑克
		public var cbCardSum:Array = new Array(GAME_PLAYER);//扑克总值
		public var cbCutCardSum:Array = new Array(GAME_PLAYER);//扑克总值
		public var wBankerStation:uint;//庄家位置
		public var cbPlayStatus:Array = new Array(GAME_PLAYER);//游戏状态
		public var fTableGold:Array = new Array(2*GAME_PLAYER);//桌面金币
		public var cbTableCardCount:Array = new Array(GAME_PLAYER);//扑克数目
		public var cbTableCardArray:Array = new Array(GAME_PLAYER);//扑克数组
		
		public function CMD_S_GameEnd()
		{
			for (var i:uint = 0; i < cbTableCardArray.length; i ++)
			{
				cbTableCardArray[i] = new Array(13);
			}
		}
		public static function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd;

			for(var i:uint = 0; i < result.fGameGold.length; i ++)
			{
				 result.fGameGold[i] = data.readDouble();
			}
			
			for(i = 0; i < result.cbUserCard.length; i ++)
			{
				 result.cbUserCard[i] = data.readUnsignedByte();
			}
			for(i = 0; i < result.cbCutCard.length; i ++)
			{
				 result.cbCutCard[i] = data.readUnsignedByte();
			}
			for(i = 0; i < result.cbCardSum.length; i ++)
			{
				 result.cbCardSum[i] = data.readUnsignedByte();
			}
			for(i = 0; i < result.cbCutCardSum.length; i ++)
			{
				 result.cbCutCardSum[i] = data.readUnsignedByte();
			}
			result.wBankerStation = data.readUnsignedShort();
			for (var i:uint = 0; i < result.cbPlayStatus.length; i ++)
			{
				result.cbPlayStatus[i] = data.readUnsignedByte();
			}
			for (i = 0; i < result.fTableGold.length; i ++)
			{
				result.fTableGold[i] = data.readDouble();
			}
			for (i = 0; i < result.cbTableCardCount.length; i ++)
			{
				result.cbTableCardCount[i] = data.readUnsignedByte();
			}
			for (i = 0; i < result.cbTableCardArray.length; i ++)
			{
				for (var k:uint = 0; k < result.cbTableCardArray[i].length; k ++)
				{
					result.cbTableCardArray[i][k] = data.readUnsignedByte();
				}
			}
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"