package org.god.GameModule.B_Hoe.Common
{
	import flash.utils.ByteArray;

	//用户出牌
	public class CMD_S_OutCard
	{
		public static  const sizeof_CMD_S_OutCard:uint=1 + 2 + 2 + 13;
		public static  const sizeof_bCardData = 13;
		
		public var bCardCount:uint;//扑克数目
		public var wCurrentUser:uint;//当前玩家
		public var wOutCardUser:uint;//出牌玩家
		public var bCardData:Array=new Array(13);//扑克列表

		public function CMD_S_OutCard()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_OutCard
		{
			var pos:int=data.position;
			var result:CMD_S_OutCard=new CMD_S_OutCard  ;
			
			result.bCardCount = data.readUnsignedByte();
			result.wCurrentUser=data.readUnsignedShort();
			result.wOutCardUser=data.readUnsignedShort();
			result.bCardData.length = result.bCardCount;
			for (var i:uint=0; i < result.bCardCount; i++)
			{
				result.bCardData[i]=data.readUnsignedByte();
			}
			

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Hoe.as"
include "../../../Common/Memory.as"