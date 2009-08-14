package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;
	//游戏状态
	public class CMD_S_StatusScore
	{
		public static const sizeof_CMD_S_StatusScore:uint = 1 + 8 + 2 + 3 + 17;
		public var bLandScore:uint;//地主分数
		public var fBaseScore:Number = new Number;//基础积分
		public var wCurrentUser:uint;//当前玩家
		public var bScoreInfo:Array = new Array(3);//叫分信息
		public var bCardData:Array = new Array(17);//手上扑克
		public function CMD_S_StatusScore()
		{
		}
		public static function readData(data:ByteArray):CMD_S_StatusScore
		{
			var pos:int=data.position;
			var result:CMD_S_StatusScore=new CMD_S_StatusScore;
			result.bLandScore = data.readUnsignedByte();
			result.fBaseScore = data.readDouble();
			result.wCurrentUser = data.readUnsignedShort();
			
			for(var i:uint = 0; i < result.bScoreInfo.length; i ++)
			{
				result.bScoreInfo[i] = data.readUnsignedByte();
			}
			for( i = 0; i < result.bCardData.length; i ++)
			{
				result.bCardData[i] = data.readUnsignedByte();
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"