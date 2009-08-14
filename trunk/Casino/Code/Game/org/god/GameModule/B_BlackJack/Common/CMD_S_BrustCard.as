package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;

	//用户爆牌
	public class CMD_S_BrustCard
	{
		public static  const sizeof_CMD_S_BrustCard:uint = 2;

		public var wBrustUser:uint;//爆牌用户
		
		public function CMD_S_BrustCard()
		{
		}
		public static function readData(data:ByteArray):CMD_S_BrustCard
		{
			var pos:int=data.position;
			var result:CMD_S_BrustCard=new CMD_S_BrustCard;

			result.wBrustUser = data.readUnsignedShort();
			

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"