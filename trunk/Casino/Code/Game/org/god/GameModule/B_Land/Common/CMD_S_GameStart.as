package org.god.GameModule.B_Land.Common
{
	import flash.utils.ByteArray;
	//游戏开始
	public class CMD_S_GameStart
	{
		public static const sizeof_CMD_S_GameStart:uint = 2 + 1 + 2 + 3;
		public var wLandUser:uint;//地主玩家
		public var bLandScore:uint;//地主分数
		public var wCurrentUser:uint;//当前玩家
		public var bBackCard:Array = new Array(3);//底牌扑克
		public function CMD_S_GameStart()
		{
		}
		public static function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart;
			result.wLandUser = data.readUnsignedShort();
			result.bLandScore = data.readUnsignedByte();
			result.wCurrentUser = data.readUnsignedShort();
			for(var i:uint = 0; i < result.bBackCard.length; i ++)
			{
				result.bBackCard[i] = data.readUnsignedByte();
			}
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"