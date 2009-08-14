package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//叫牌状态
	public class CMD_S_StatusCall
	{
		public static  const sizeof_CMD_S_StatusCall:uint=2 + 1 + 1 + 2 + 1 + 52 + 4 + 1 + 1 + 1 + 2;

		//游戏变量
		public var wBankerUser:uint;//庄家用户
		public var cbPackCount:uint;//副数数目
		public var cbMainValue:uint;//主牌数值
		public var cbValueOrder:Array=new Array(2);//主牌数值

		//发牌变量
		public var cbCardCount:uint;//扑克数目
		public var cbCardData:Array=new Array(52);//扑克列表
		public var cbHandCardCount:Array=new Array(4);//扑克数目

		//叫牌信息
		public var cbComplete:uint;//完成标志
		public var cbCallCard:uint;//叫牌扑克
		public var cbCallCount:uint;//叫牌数目
		public var wCallCardUser:uint;//叫牌用户

		public function CMD_S_StatusCall()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_StatusCall
		{
			var pos:int=data.position;
			var result:CMD_S_StatusCall=new CMD_S_StatusCall  ;
			result.wBankerUser=data.readUnsignedShort();
			result.cbPackCount=data.readUnsignedByte();
			result.cbMainValue=data.readUnsignedByte();
			for(var i:uint = 0; i < result.cbValueOrder.length; i ++)
			{
				result.cbValueOrder[i] = data.readUnsignedByte();
			}
			result.cbCardCount=data.readUnsignedByte();
			for(i = 0; i < result.cbCardData.length; i ++)
			{
				result.cbCardData[i] = data.readUnsignedByte();
			}
			for(i = 0; i < result.cbHandCardCount.length; i ++)
			{
				result.cbHandCardCount[i] = data.readUnsignedByte();
			}
			result.cbComplete=data.readUnsignedByte();
			result.cbCallCard=data.readUnsignedByte();
			result.cbCallCount=data.readUnsignedByte();
			result.wCallCardUser=data.readUnsignedShort();
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"