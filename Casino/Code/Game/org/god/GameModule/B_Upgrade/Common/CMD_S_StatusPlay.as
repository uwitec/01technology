package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_StatusPlay
	{
		public static  const sizeof_CMD_S_StatusPlay:uint=1 + 1 + 1 + 2 +
		2 + 2 + 2 + 1 + 1 + 2 + 1 + 52 + 4 + 1 + 8 + 4 + 4 * 52 + 1 + 48;

		//游戏变量
		public var cbPackCount:uint;//副数数目
		public var cbMainValue:uint;//主牌数值
		public var cbMainColor:uint;//主牌数值
		public var cbValueOrder:Array=new Array(2);//主牌数值

		//用户变量
		public var wBankerUser:uint;//庄家用户
		public var wCurrentUser:uint;//当前用户
		public var wFirstOutUser:uint;//出牌用户

		//叫牌信息
		public var cbCallCard:uint;//叫牌扑克
		public var cbCallCount:uint;//叫牌数目
		public var wCallCardUser:uint;//叫牌用户

		//扑克变量
		public var cbCardCount:uint;//扑克数目
		public var cbCardData:Array=new Array(52);//扑克列表
		public var cbHandCardCount:Array=new Array(4);//扑克数目

		//底牌信息
		public var cbConcealCount:uint;//暗藏数目
		public var cbConcealCard:Array=new Array(8);//暗藏扑克

		//出牌变量
		public var cbOutCardCount:Array=new Array(4);//出牌数目
		public var cbOutCardData:Array=new Array(4);//出牌列表

		//得分变量
		public var cbScoreCardCount:uint;//扑克数目
		public var cbScoreCardData:Array=new Array(48);//得分扑克


		public function CMD_S_StatusPlay()
		{
			for (var i:uint=0; i < cbOutCardData.length; i++)
			{
				cbOutCardData[i]=new Array(52);
			}
		}
		public static  function readData(data:ByteArray):CMD_S_StatusPlay
		{
			var pos:int=data.position;
			var result:CMD_S_StatusPlay=new CMD_S_StatusPlay  ;
			
			result.cbPackCount=data.readUnsignedByte();
			result.cbMainValue=data.readUnsignedByte();
			result.cbMainColor=data.readUnsignedByte();
			for (var i:uint=0; i < result.cbValueOrder.length; i++)
			{
				result.cbValueOrder[i]=data.readUnsignedByte();
			}
			result.wBankerUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			result.wFirstOutUser=data.readUnsignedShort();

			result.cbCallCard=data.readUnsignedByte();
			result.cbCallCount=data.readUnsignedByte();
			result.wCallCardUser=data.readUnsignedShort();

			result.cbCardCount=data.readUnsignedByte();
			for (i=0; i < result.cbCardData.length; i++)
			{
				result.cbCardData[i]=data.readUnsignedByte();
			}
			for(i = 0; i < result.cbHandCardCount.length; i ++)
			{
				result.cbHandCardCount[i] = data.readUnsignedByte();
			}
			result.cbConcealCount=data.readUnsignedByte();
			for (i=0; i < result.cbConcealCard.length; i++)
			{
				result.cbConcealCard[i]=data.readUnsignedByte();
			}
			
			for (i=0; i < result.cbOutCardCount.length; i++)
			{
				result.cbOutCardCount[i]=data.readUnsignedByte();
			}
			for (i=0; i < result.cbOutCardData.length; i++)
			{
				for(var j:uint = 0; j < result.cbOutCardData[i].length; j ++)
					result.cbOutCardData[i][j]=data.readUnsignedByte();
			}
			result.cbScoreCardCount=data.readUnsignedByte();
			for (i=0; i < result.cbScoreCardData.length; i++)
			{
				result.cbScoreCardData[i]=data.readUnsignedByte();
			}
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"