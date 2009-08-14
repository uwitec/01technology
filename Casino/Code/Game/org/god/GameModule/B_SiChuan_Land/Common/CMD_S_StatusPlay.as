package org.god.GameModule.B_SiChuan_Land.Common
{
	import flash.utils.ByteArray;
	//游戏状态
	public class CMD_S_StatusPlay
	{
		public static const sizeof_CMD_S_StatusPlay:uint = 2 + 2 + 8 +
		1 + 2 + 2 + 3 + 20 + 3 + 1 + 20 + 3;
		public var wLandUser:uint;//坑主玩家
		public var wBombTime:uint;//炸弹倍数
		public var fBaseScore:Number = new Number;//基础积分
		public var bLandScore:uint;//地主分数
		public var wLastOutUser:uint;//出牌的人
		public var wCurrentUser:uint;//当前玩家
		public var bBackCard:Array = new Array(3);//底牌扑克
		public var bCardData:Array = new Array(20);//手上扑克
		public var bCardCount:Array = new Array(3);//扑克数目
		public var bTurnCardCount:uint;//基础出牌
		public var bTurnCardData:Array = new Array(20);//出牌列表
		public var bDoubleTimeScore:Array = new Array(3);//反牌
		public function CMD_S_StatusPlay()
		{
		}
		public static function readData(data:ByteArray):CMD_S_StatusPlay
		{
			var pos:int=data.position;
			var result:CMD_S_StatusPlay=new CMD_S_StatusPlay;
			result.wLandUser = data.readUnsignedShort();
			result.wBombTime = data.readUnsignedShort();
			result.fBaseScore = data.readDouble();
			result.bLandScore = data.readUnsignedByte();
			result.wLastOutUser = data.readUnsignedShort();
			result.wCurrentUser = data.readUnsignedShort();
			for(var i:uint = 0; i < result.bBackCard.length; i ++)
			{
				result.bBackCard[i] = data.readUnsignedByte();
			}
			for( i = 0; i < result.bCardData.length; i ++)
			{
				result.bCardData[i] = data.readUnsignedByte();
			}
			for( i = 0; i < result.bCardCount.length; i ++)
			{
				result.bCardCount[i] = data.readUnsignedByte();
			}
			result.bTurnCardCount = data.readUnsignedByte();
			for( i = 0; i < result.bTurnCardData.length; i ++)
			{
				result.bTurnCardData[i] = data.readUnsignedByte();
			}
			for( i = 0; i < result.bDoubleTimeScore.length; i ++)
			{
				result.bDoubleTimeScore[i] = data.readUnsignedByte();
			}
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"