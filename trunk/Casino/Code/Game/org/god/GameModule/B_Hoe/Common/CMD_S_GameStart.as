package org.god.GameModule.B_Hoe.Common
{
	import flash.utils.ByteArray;

	//发送扑克
	public class CMD_S_GameStart
	{
		public static  const sizeof_CMD_S_GameStart:uint=2 +2 + 13;

		public var wBankerUser:uint;//庄家用户
		public var wCurrentUser:uint;//当前玩家
		public var cbCardData:Array=new Array(13);//扑克列表

		public function CMD_S_GameStart()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart  ;

			result.wBankerUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			for(var i:uint = 0; i <result.cbCardData.length; i ++)
			{
				result.cbCardData[i]=data.readUnsignedByte();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Hoe.as"
include "../../../Common/Memory.as"