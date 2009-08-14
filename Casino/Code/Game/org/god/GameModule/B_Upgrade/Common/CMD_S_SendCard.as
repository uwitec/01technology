package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//发送扑克
	public class CMD_S_SendCard
	{
		public static  const sizeof_CMD_S_SendCard:uint=4 + 1 +52;
		public static  const sizeof_cbCardData:uint=52;

		public var cbHandCardCount:Array=new Array(4);//扑克数目
		public var cbCardCount:uint;//扑克数目
		public var cbCardData:Array=new Array(52);//扑克列表

		public function CMD_S_SendCard()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_SendCard
		{
			var pos:int=data.position;
			var result:CMD_S_SendCard=new CMD_S_SendCard  ;

			for (var i:uint=0; i < result.cbHandCardCount.length; i++)
			{
				result.cbHandCardCount[i]=data.readUnsignedByte();
			}			
			result.cbCardCount=data.readUnsignedByte();
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