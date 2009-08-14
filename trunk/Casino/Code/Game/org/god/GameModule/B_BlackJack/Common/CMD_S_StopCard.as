package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;

	//用户停牌
	public class CMD_S_StopCard
	{
		public static  const sizeof_CMD_S_StopCard:uint = 2 + 4 ;

		public var wStopUser:uint;//停牌用户
		public var bDoubleBrsut:uint;//双倍爆牌

		public function CMD_S_StopCard()
		{
		}
		public static function readData(data:ByteArray):CMD_S_StopCard
		{
			var pos:int=data.position;
			var result:CMD_S_StopCard=new CMD_S_StopCard;

			result.wStopUser = data.readUnsignedShort();
			result.bDoubleBrsut = data.readUnsignedInt();
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"