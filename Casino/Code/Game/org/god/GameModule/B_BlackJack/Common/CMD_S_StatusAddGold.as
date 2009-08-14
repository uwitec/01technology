package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//加注状态
	public class CMD_S_StatusAddGold
	{
		public static  const sizeof_CMD_S_StatusAddGold:uint = 2 + 2 + GAME_PLAYER * 4 + 8 +
		8 + 2*GAME_PLAYER * 8;

		public var wCurrentUser:uint;//当前玩家
		public var wBankerStation:uint;//庄家位置
		public var bAddGoldEnd:Array = new Array(GAME_PLAYER);//加注结束
		public var fBasicGold:Number = 0;//基础金币
		public var fMaxGold:Number = 0;//最大下注
		public var fTableGold:Array =new Array(2*GAME_PLAYER);//桌面金币

		public function CMD_S_StatusAddGold()
		{
		}
		public static function readData(data:ByteArray):CMD_S_StatusAddGold
		{
			var pos:int=data.position;
			var result:CMD_S_StatusAddGold=new CMD_S_StatusAddGold;

			result.wCurrentUser = data.readUnsignedShort();
			result.wBankerStation = data.readUnsignedShort();
			for (var i:uint = 0; i < result.bAddGoldEnd.length; i ++)
			{
				result.bAddGoldEnd[i] = data.readUnsignedInt();
			}
			result.fBasicGold = data.readDouble();
			result.fMaxGold = data.readDouble();
			for (i = 0; i < result.fTableGold.length; i ++)
			{
				result.fTableGold[i] = data.readDouble();
			}
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"