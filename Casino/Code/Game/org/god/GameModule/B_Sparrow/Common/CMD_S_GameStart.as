package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//游戏开始
	public class CMD_S_GameStart
	{
		public static const sizeof_CMD_S_GameStart:uint = 2 + 2 + 2 + 1 + 14;
		
		public var wSiceCount:uint;//骰子点数
		public var wBankerUser:uint;//庄家用户
		public var wCurrentUser:uint;//当前用户
		public var cbUserAction:uint;//用户动作
		public var cbSparrowData:Array = new Array(14);//扑克列表


		public function CMD_S_GameStart()
		{
		}
		public static function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart;
			result.wSiceCount = data.readUnsignedShort();
			result.wBankerUser = data.readUnsignedShort();
			result.wCurrentUser = data.readUnsignedShort();
			result.cbUserAction = data.readUnsignedByte();
			for(var i:uint = 0; i < result.cbSparrowData.length; i ++)
			{
				result.cbSparrowData[i] = data.readUnsignedByte();
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"