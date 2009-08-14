package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//空闲状态
	public class CMD_S_StatusFree
	{
		public static  const sizeof_CMD_S_StatusFree:uint = 8;

		public var fBasicGold:Number = 0;//基础金币

		public function CMD_S_StatusFree()
		{
		}
		public static function readData(data:ByteArray):CMD_S_StatusFree
		{
			var pos:int=data.position;
			var result:CMD_S_StatusFree=new CMD_S_StatusFree;

			result.fBasicGold = data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"