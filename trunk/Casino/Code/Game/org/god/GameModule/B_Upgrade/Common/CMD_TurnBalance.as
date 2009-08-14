package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//一轮统计
	public class CMD_TurnBalance
	{
		public static  const sizeof_CMD_TurnBalance:uint=2 + 2;

		public var wTurnWinner:uint;//一轮胜者
		public var wCurrentUser:uint;//当前玩家

		public function CMD_TurnBalance()
		{
		}
		public static  function readData(data:ByteArray):CMD_TurnBalance
		{
			var pos:int=data.position;
			var result:CMD_TurnBalance=new CMD_TurnBalance  ;

			result.wTurnWinner=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"