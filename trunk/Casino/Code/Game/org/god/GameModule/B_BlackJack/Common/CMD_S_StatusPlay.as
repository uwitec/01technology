package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//游戏状态
	public class CMD_S_StatusPlay
	{
		public static  const sizeof_CMD_S_StatusPlay:uint = 2 + 2 + GAME_PLAYER + GAME_PLAYER + 
		GAME_PLAYER * 13 + 8 + 8 + 2 * GAME_PLAYER * 8 + 4;

		public var wCurrentUser:uint;//当前玩家
		public var wBankerStation:uint;//庄家位置
		public var cbPlayStatus:Array = new Array(GAME_PLAYER);//游戏状态
		public var cbTableCardCount:Array = new Array(GAME_PLAYER);//扑克数目
		public var cbTableCardArray:Array = new Array(GAME_PLAYER);//扑克数组
		public var fMaxGold:Number;//最大下注
		public var fBasicGold:Number;//基础金币
		public var fTableGold:Array = new Array(2*GAME_PLAYER);//桌面金币
		public var bBankerGetCard:uint;//庄家要牌

		public function CMD_S_StatusPlay()
		{
			for (var i:uint = 0; i < cbTableCardArray.length; i ++)
			{
				cbTableCardArray[i] = new Array(13);
			}
		}
		public static function readData(data:ByteArray):CMD_S_StatusPlay
		{
			var pos:int=data.position;
			var result:CMD_S_StatusPlay=new CMD_S_StatusPlay;

			result.wCurrentUser = data.readUnsignedShort();
			result.wBankerStation = data.readUnsignedShort();
			for (var i:uint = 0; i < result.cbPlayStatus.length; i ++)
			{
				result.cbPlayStatus[i] = data.readUnsignedByte();
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
			result.fMaxGold = data.readDouble();
			result.fBasicGold = data.readDouble();
			for (i = 0; i < result.fTableGold.length; i ++)
			{
				result.fTableGold[i] = data.readDouble();
			}
			result.bBankerGetCard = data.readUnsignedInt();
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"