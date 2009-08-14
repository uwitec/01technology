package org.god.GameModule.B_GoldFlower.Common
{
	import flash.utils.ByteArray;

	//游戏开始
	public class CMD_S_GameStart
	{
		public static  const sizeof_CMD_S_GameStart:uint=2 + 8 + 8 + 8 + GAME_PLAYER * 3;

		public var wCurrentUser:uint;//当前玩家
		public var fMaxGold:Number;//最大下注
		public var fTurnMaxGold:Number;//最大下注
		public var fTurnBasicGold:Number;//最少下注
		public var bCardData:Array=new Array(GAME_PLAYER);//用户扑克

		public function CMD_S_GameStart()
		{
			for(var i:uint = 0; i < bCardData.length; i ++)
			{
				bCardData[i] = new Array(3);
			}
		}
		public static  function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart  ;

			result.wCurrentUser=data.readUnsignedShort();
			result.fMaxGold=data.readDouble();
			result.fTurnMaxGold=data.readDouble();
			result.fTurnBasicGold=data.readDouble();
			for (var i:uint=0; i < result.bCardData.length; i++)
			{
				for(var k:uint = 0; k < result.bCardData[i].length; k ++)
				{
					result.bCardData[i][k]=data.readUnsignedByte();
				}
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_GoldFlower.as"
include "../../../Common/Memory.as"