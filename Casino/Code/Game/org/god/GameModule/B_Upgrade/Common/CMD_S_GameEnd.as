package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//游戏结束
	public class CMD_S_GameEnd
	{
		public static  const sizeof_CMD_S_GameEnd:uint=4 * 8 + 2 + 2 + 2 + 1 + 8 + 
		2 + 1 + 1 + 2 + 1 + 48;

		public var fScore:Array=new Array(4);//用户得分
		public var wGameScore:uint;//游戏得分
		public var wConcealTime:uint;//扣底倍数
		public var wConcealScore:uint;//底牌积分
		public var cbConcealCount:uint;//暗藏数目
		public var cbConcealCard:Array=new Array(8);//暗藏扑克
		//用户变量
		public var wBankerUser:uint;//庄家用户
		
		//叫牌信息
		public var cbCallCard:uint;//叫牌扑克
		public var cbCallCount:uint;//叫牌数目
		public var wCallCardUser:uint;//叫牌用户

		//得分变量
		public var cbScoreCardCount:uint;//扑克数目
		public var cbScoreCardData:Array=new Array(48);//得分扑克

		public function CMD_S_GameEnd()
		{

		}
		public static  function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd  ;

			for (var i:uint=0; i < result.fScore.length; i++)
			{
				result.fScore[i]=data.readDouble();
			}
			result.wGameScore=data.readUnsignedShort();
			result.wConcealTime=data.readUnsignedShort();
			result.wConcealScore=data.readUnsignedShort();
			result.cbConcealCount=data.readUnsignedByte();
			for (i=0; i < result.cbConcealCard.length; i++)
			{
				result.cbConcealCard[i]=data.readUnsignedByte();
			}
			
			result.wBankerUser=data.readUnsignedShort();
		
			result.cbCallCard=data.readUnsignedByte();
			result.cbCallCount=data.readUnsignedByte();
			result.wCallCardUser=data.readUnsignedShort();
		
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