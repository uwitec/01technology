package org.god.GameModule.B_ShowHand.Common
{
	import flash.utils.ByteArray;

	//用户放弃
	public class CMD_S_GiveUp
	{
		public static  const sizeof_CMD_S_GiveUp:uint=2;

		public var wUserChairID:uint = 0;//放弃用户

		public function CMD_S_GiveUp()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_GiveUp
		{
			var pos:int=data.position;
			var result:CMD_S_GiveUp=new CMD_S_GiveUp  ;

			result.wUserChairID=data.readUnsignedShort();


			data.position=pos;
			return result;
		}
	}
}
include "./CMD_ShowHand.as"
include "../../../Common/Memory.as"