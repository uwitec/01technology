package org.god.GameModule.B_GoldFlower.Common
{
	import flash.utils.ByteArray;

	//游戏结束
	public class CMD_S_GameEnd
	{
		public static  const sizeof_CMD_S_GameEnd:uint=GAME_PLAYER*8 + GAME_PLAYER +
		GAME_PLAYER + 2 + 
		GAME_PLAYER * 3 + 8 + 8 + 8 + 8 + 2 * GAME_PLAYER * 8;

		public var fGameGold:Array=new Array(GAME_PLAYER);//游戏得分
		public var bUserCard:Array=new Array(GAME_PLAYER);//用户扑克
		public var wWinerUser:uint;
		public var bTableCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var bTableCardArray:Array=new Array(GAME_PLAYER);//扑克数组
		public var fMaxGold:Number;//最大下注
		public var fBasicGold:Number;//基础金币
		public var fTurnMaxGold:Number;//最大下注
		public var fTurnBasicGold:Number;//最少下注
		public var fTableGold:Array=new Array(2 * GAME_PLAYER);//桌面金币
		
		public function CMD_S_GameEnd()
		{
			for(var i:uint = 0; i < bTableCardArray.length; i ++)
			{
				bTableCardArray[i] = new Array(3);
			}
		}
		public static  function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd  ;
			
			for (var i:uint=0; i < result.fGameGold.length; i++)
			{
				result.fGameGold[i]=data.readDouble();
			}
			for (i=0; i < result.bUserCard.length; i++)
			{
				result.bUserCard[i]=data.readUnsignedByte();
			}
			result.wWinerUser = data.readUnsignedShort();
			for (i=0; i < result.bTableCardCount.length; i++)
			{
				result.bTableCardCount[i]=data.readUnsignedByte();
			}
			for (i=0; i < result.bTableCardArray.length; i++)
			{
				for(var k:uint = 0; k < result.bTableCardArray[i].length; k ++)
					result.bTableCardArray[i][k]=data.readUnsignedByte();
			}
			result.fMaxGold=data.readDouble();
			result.fBasicGold=data.readDouble();
			result.fTurnMaxGold=data.readDouble();
			result.fTurnBasicGold=data.readDouble();
			for (i=0; i < result.fTableGold.length; i++)
			{
				result.fTableGold[i]=data.readDouble();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_GoldFlower.as"
include "../../../Common/Memory.as"