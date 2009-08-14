package org.god.GameModule.B_BlackJack.Common
{
	import flash.utils.ByteArray;

	//用户强退
	public class CMD_S_UserLeft
	{
		public static  const sizeof_CMD_S_UserLeft:uint = 2 + 4;

		public var wUserChairID:uint;//强退用户
		public var bAddGoldEnd:uint;//加注结束

		public function CMD_S_UserLeft()
		{
		}
		public static function readData(data:ByteArray):CMD_S_UserLeft
		{
			var pos:int=data.position;
			var result:CMD_S_UserLeft=new CMD_S_UserLeft;

			result.wUserChairID = data.readUnsignedShort();
			result.bAddGoldEnd = data.readUnsignedInt();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_BlackJack.as"
include "../../../Common/Memory.as"