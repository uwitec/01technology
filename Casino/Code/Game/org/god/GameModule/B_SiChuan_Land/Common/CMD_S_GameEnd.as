package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;
	//游戏结束
	public class CMD_S_GameEnd
	{
		public static  const sizeof_CMD_S_GameEnd:uint = 24 + 3 + 54 + 3 + 2 + 2 + 1 + 3;
		public var fGameScore:Array = new Array(3);//游戏积分
		public var bCardCount:Array = new Array(3);//扑克数目
		public var bCardData:Array = new Array(54);//扑克列表 
		public var bBackCard:Array = new Array(3);//底牌扑克
		public var wLandUser:uint;//坑主玩家
		public var wBombTime:uint;//炸弹倍数
		public var bLandScore:uint;//地主分数
		public var bDoubleTimeScore:Array = new Array(3);//反牌
		public function CMD_S_GameEnd()
		{
		}
		public static function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd;

			for (var i:uint = 0; i < result.fGameScore.length; i ++)
			{
				result.fGameScore[i]=data.readDouble();
			}
			for (i = 0; i <result.bCardCount.length; i ++)
			{
				result.bCardCount[i]=data.readUnsignedByte();
			}
			for (i = 0; i <result.bCardData.length; i ++)
			{
				result.bCardData[i]=data.readUnsignedByte();
			}
			for (i = 0; i <result.bBackCard.length; i ++)
			{
				result.bBackCard[i]=data.readUnsignedByte();
			}
			result.wLandUser=data.readUnsignedShort();
			result.wBombTime=data.readUnsignedShort();
			result.bLandScore=data.readUnsignedByte();
			for (i = 0; i <result.bDoubleTimeScore.length; i ++)
			{
				result.bDoubleTimeScore[i]=data.readUnsignedByte();
			}
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"