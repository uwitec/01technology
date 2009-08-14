package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//游戏开始
	public class CMD_S_GameStart
	{
		public static  const sizeof_CMD_S_GameStart:uint=2 + 1;

		public var wCurrentUser:uint;//当前玩家
		public var bFirstGame:uint;//首局游戏

		public function CMD_S_GameStart()
		{

		}
		public static  function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart  ;

			result.wCurrentUser=data.readUnsignedShort();
			result.bFirstGame=data.readUnsignedByte();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"