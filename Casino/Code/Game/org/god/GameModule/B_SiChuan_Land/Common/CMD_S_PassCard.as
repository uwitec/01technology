package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;
	//放弃出牌
	public class CMD_S_PassCard
	{
		public static const sizeof_CMD_S_PassCard:uint = 1 + 2 + 2;
		public var bNewTurn:uint;//一轮开始
		public var wPassUser:uint;//放弃玩家
		public var wCurrentUser:uint;//当前玩家
		public function CMD_S_PassCard()
		{
		}
		public static function readData(data:ByteArray):CMD_S_PassCard
		{
			var pos:int=data.position;
			var result:CMD_S_PassCard=new CMD_S_PassCard;
			result.bNewTurn = data.readUnsignedByte();
			result.wPassUser = data.readUnsignedShort();
			result.wCurrentUser = data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"