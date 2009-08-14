package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;
	//下注结果
	public class CMD_S_AddGold
	{
		public static  const sizeof_CMD_S_AddGold:uint = 4 + 2 + 8;

		public var bDoubleGold:uint;//是否双倍
		public var wCurrentUser:uint;//当前用户
		public var fLastAddGold:Number = 0;//下注数目
		
		public function CMD_S_AddGold()
		{
		}
		public static function readData(data:ByteArray):CMD_S_AddGold
		{
			var pos:int=data.position;
			var result:CMD_S_AddGold=new CMD_S_AddGold;

			result.bDoubleGold = data.readUnsignedInt();
			result.wCurrentUser = data.readUnsignedShort();
			result.fLastAddGold = data.readDouble();
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"