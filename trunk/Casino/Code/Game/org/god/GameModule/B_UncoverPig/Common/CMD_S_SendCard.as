package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//发送扑克
	public class CMD_S_SendCard
	{
		public static  const sizeof_CMD_S_SendCard:uint=2 + HAND_CARD_COUNT;


		public var wCurrentUser:uint;//当前玩家
		public var bCardData:Array=new Array(HAND_CARD_COUNT);//手上扑克

		public function CMD_S_SendCard()
		{

		}
		public static  function readData(data:ByteArray):CMD_S_SendCard
		{
			var pos:int=data.position;
			var result:CMD_S_SendCard=new CMD_S_SendCard  ;

			result.wCurrentUser=data.readUnsignedShort();
			for (var i:uint=0; i < result.bCardData.length; i++)
			{
				result.bCardData[i]=data.readUnsignedByte();
			}
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"