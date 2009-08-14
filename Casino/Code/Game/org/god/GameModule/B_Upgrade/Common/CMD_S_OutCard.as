package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//用户出牌
	public class CMD_S_OutCard
	{
		public static  const sizeof_CMD_S_OutCard:uint=1 + 2 + 2 + 52;
		public static  const sizeof_cbCardData:uint=52;

		public var cbCardCount:uint;//扑克数目
		public var wOutCardUser:uint;//出牌玩家
		public var wCurrentUser:uint;//当前玩家
		public var cbCardData:Array=new Array(52);//扑克列表

		public function CMD_S_OutCard()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_OutCard
		{
			var pos:int=data.position;
			var result:CMD_S_OutCard=new CMD_S_OutCard  ;

			result.cbCardCount=data.readUnsignedByte();
			result.wOutCardUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			
			result.cbCardData.length = result.cbCardCount;
			for (var i:uint=0; i < result.cbCardCount; i++)
			{
				result.cbCardData[i]=data.readUnsignedByte();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"