package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//留底状态
	public class CMD_S_StatusBack
	{
		public static  const sizeof_CMD_S_StatusBack:uint=1 + 1 + 1 + 2 + 
		1 + 1 + 2 + 2 + 2 + 1 + 60 + 4 + 1 + 8;

		//游戏变量
		public var cbPackCount:uint;//副数数目
		public var cbMainValue:uint;//主牌数值
		public var cbMainColor:uint;//主牌数值
		public var cbValueOrder:Array=new Array(2);//主牌数值

		//叫牌信息
		public var cbCallCard:uint;//叫牌扑克
		public var cbCallCount:uint;//叫牌数目
		public var wCallCardUser:uint;//叫牌用户

		//用户变量
		public var wBankerUser:uint;//庄家用户
		public var wCurrentUser:uint;//当前用户

		//扑克变量
		public var cbCardCount:uint;//扑克数目
		public var cbCardData:Array=new Array(60);//扑克列表
		public var cbHandCardCount:Array=new Array(4);//扑克数目

		//底牌信息
		public var cbConcealCount:uint;//暗藏数目
		public var cbConcealCard:Array=new Array(8);//暗藏扑克


		public function CMD_S_StatusBack()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_StatusBack
		{
			var pos:int=data.position;
			var result:CMD_S_StatusBack=new CMD_S_StatusBack  ;
			result.cbPackCount=data.readUnsignedByte();
			result.cbMainValue=data.readUnsignedByte();
			result.cbMainColor=data.readUnsignedByte();
			for (var i:uint=0; i < result.cbValueOrder.length; i++)
			{
				result.cbValueOrder[i]=data.readUnsignedByte();
			}
			result.cbCallCard=data.readUnsignedByte();
			result.cbCallCount=data.readUnsignedByte();
			result.wCallCardUser=data.readUnsignedShort();
			
			result.wBankerUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			
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
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"