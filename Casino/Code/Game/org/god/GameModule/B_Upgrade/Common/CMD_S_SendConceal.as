package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//底牌扑克
	public class CMD_S_SendConceal
	{
		public static  const sizeof_CMD_S_SendConceal:uint=1 + 2 + 2 + 1 + 8;

		public var cbMainColor:uint;//主牌数值
		public var wBankerUser:uint;//庄家用户
		public var wCurrentUser:uint;//当前用户
		public var cbConcealCount:uint;//暗藏数目
		public var cbConcealCard:Array=new Array(8);//暗藏扑克

		public function CMD_S_SendConceal()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_SendConceal
		{
			var pos:int=data.position;
			var result:CMD_S_SendConceal=new CMD_S_SendConceal  ;

			result.cbMainColor=data.readUnsignedByte();
			result.wBankerUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			result.cbConcealCount=data.readUnsignedByte();
			for(var i:uint = 0; i < result.cbConcealCard.length; i ++)
			{
				result.cbConcealCard[i] = data.readUnsignedByte();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"