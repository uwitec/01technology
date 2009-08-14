package org.god.GameModule.B_ShowHand.Common
{
	import flash.utils.ByteArray;

	//发牌数据包
	public class CMD_R_SendCard
	{
		public static  const sizeof_CMD_R_SendCard:uint=8 + 2 + 4;

		public var fMaxGold:Number;//最大下注
		public var wCurrentUser:uint;//当前用户
		public var bUserCard:Array=new Array(4);//用户扑克

		public function CMD_R_SendCard()
		{
		}
		public static  function readData(data:ByteArray):CMD_R_SendCard
		{
			var pos:int=data.position;
			var result:CMD_R_SendCard=new CMD_R_SendCard  ;

			result.fMaxGold=data.readDouble();
			result.wCurrentUser=data.readUnsignedShort();
			for (var i:uint=0; i < result.bUserCard.length; i++)
			{
				result.bUserCard[i]=data.readUnsignedByte();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_ShowHand.as"
include "../../../Common/Memory.as"