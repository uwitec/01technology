package org.god.GameModule.B_ShowHand.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_StatusPlay
	{
		public static  const sizeof_CMD_S_StatusPlay:uint=2 + GAME_PLAYER + 
		GAME_PLAYER + GAME_PLAYER*5 + 8 + 8 + 8 + 8 + 2 * GAME_PLAYER * 8;

		public var wCurrentUser:uint;//当前玩家
		public var bPlayStatus:Array=new Array(GAME_PLAYER);//游戏状态
		public var bTableCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var bTableCardArray:Array=new Array(GAME_PLAYER);//扑克数组
		public var fMaxGold:Number;//最大下注
		public var fBasicGold:Number;//基础金币
		public var fTurnMaxGold:Number;//最大下注
		public var fTurnBasicGold:Number;//最少下注
		public var fTableGold:Array=new Array(2 * GAME_PLAYER);//桌面金币
		
		public function CMD_S_StatusPlay()
		{
			for(var i:uint = 0; i < bTableCardArray.length; i ++)
			{
				bTableCardArray[i] = new Array(5);
			}
		}
		public static  function readData(data:ByteArray):CMD_S_StatusPlay
		{
			var pos:int=data.position;
			var result:CMD_S_StatusPlay=new CMD_S_StatusPlay  ;

			result.wCurrentUser=data.readUnsignedShort();
			for(var i:uint = 0; i < result.bPlayStatus.length; i ++)
			{
				result.bPlayStatus[i]=data.readUnsignedByte();
			}
			for(i = 0; i < result.bTableCardCount.length; i ++)
			{
				result.bTableCardCount[i]=data.readUnsignedByte();
			}
			for(i = 0; i < result.bTableCardArray.length; i ++)
			{
				for(var k:uint = 0; k < result.bTableCardArray[i].length; k ++)
					result.bTableCardArray[i][k]=data.readUnsignedByte();
			}
			
			result.fMaxGold=data.readDouble();
			result.fBasicGold=data.readDouble();
			result.fTurnMaxGold=data.readDouble();
			result.fTurnBasicGold=data.readDouble();
			for(i = 0; i < result.fTableGold.length; i ++)
			{
				result.fTableGold[i]=data.readDouble();
			}
			

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_ShowHand.as"
include "../../../Common/Memory.as"