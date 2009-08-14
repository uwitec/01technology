package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;
	//用户反牌
	public class CMD_S_UserDoubleTime
	{
		public static  const sizeof_CMD_S_UserDoubleTime:uint = 2+1+2;

		public var wDoubleTimeUser:uint;//反牌玩家
		public var bDoubleTime:uint;//反牌 1-不反 2-反牌
		public var wCurrentUser:uint;//当前玩家
		
		public function CMD_S_UserDoubleTime()
		{
		}
		public static function readData(data:ByteArray):CMD_S_UserDoubleTime
		{
			var pos:int=data.position;
			var result:CMD_S_UserDoubleTime=new CMD_S_UserDoubleTime;
			
			result.wDoubleTimeUser = data.readUnsignedShort();
			result.bDoubleTime = data.readUnsignedByte();
			result.wCurrentUser = data.readUnsignedShort();
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"