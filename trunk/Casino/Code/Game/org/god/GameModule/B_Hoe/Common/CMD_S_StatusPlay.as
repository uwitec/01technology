package org.god.GameModule.B_Hoe.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_StatusPlay
	{
		public static  const sizeof_CMD_S_StatusPlay:uint=8 + 2 +
		2 + 2 + 13 + 4 + 1 + 13 + 4 * 8 + 4 * 8;

		public var fBaseScore:Number=0;//基础积分
		public var wBankerUser:uint;//庄家用户
		public var wLastOutUser:uint;//出牌的人
		public var wCurrentUser:uint;//当前玩家
		public var bCardData:Array=new Array(13);//手上扑克
		public var bCardCount:Array=new Array(4);//扑克数目
		public var bTurnCardCount:uint;//基础出牌
		public var bTurnCardData:Array=new Array(13);//出牌列表
		public var fAllTurnScore:Array=new Array(4);//总局得分
		public var fLastTurnScore:Array=new Array(4);//上局得分
		
		public function CMD_S_StatusPlay()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_StatusPlay
		{
			var pos:int=data.position;
			var result:CMD_S_StatusPlay=new CMD_S_StatusPlay;

			result.fBaseScore=data.readDouble();
			result.wBankerUser=data.readUnsignedShort();
			result.wLastOutUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			for(var i:uint = 0; i < result.bCardData.length; i ++)
			{
				result.bCardData[i]=data.readUnsignedByte();
			}
			for(i = 0; i < result.bCardCount.length; i ++)
			{
				result.bCardCount[i]=data.readUnsignedByte();
			}			
			result.bTurnCardCount=data.readUnsignedByte();
			for(i = 0; i < result.bTurnCardData.length; i ++)
			{
				result.bTurnCardData[i]=data.readUnsignedByte();
			}	
			for(i = 0; i < result.fAllTurnScore.length; i ++)
			{
				result.fAllTurnScore[i]=data.readDouble();
			}
			for(i = 0; i < result.fLastTurnScore.length; i ++)
			{
				result.fLastTurnScore[i]=data.readDouble();
			}
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Hoe.as"
include "../../../Common/Memory.as"