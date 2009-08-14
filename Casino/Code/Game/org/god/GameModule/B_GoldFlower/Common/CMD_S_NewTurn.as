package org.god.GameModule.B_GoldFlower.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_NewTurn
	{
		public static  const sizeof_CMD_S_NewTurn:uint=8 + 8 + 8 + 8 + 2 * GAME_PLAYER * 8;

		public var fMaxGold:Number;//最大下注
		public var fBasicGold:Number;//基础金币
		public var fTurnMaxGold:Number;//最大下注
		public var fTurnBasicGold:Number;//最少下注
		public var fTableGold:Array=new Array(2 * GAME_PLAYER);//桌面金币
		
		public function CMD_S_NewTurn()
		{
			
		}
		public static  function readData(data:ByteArray):CMD_S_NewTurn
		{
			var pos:int=data.position;
			var result:CMD_S_NewTurn=new CMD_S_NewTurn  ;

			result.fMaxGold=data.readDouble();
			result.fBasicGold=data.readDouble();
			result.fTurnMaxGold=data.readDouble();
			result.fTurnBasicGold=data.readDouble();
			for(var i:int = 0; i < result.fTableGold.length; i ++)
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