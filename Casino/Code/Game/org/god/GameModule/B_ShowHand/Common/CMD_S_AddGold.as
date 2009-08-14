package org.god.GameModule.B_ShowHand.Common
{
	import flash.utils.ByteArray;

	//加注结果
	public class CMD_S_AddGold
	{
		public static  const sizeof_CMD_S_AddGold:uint=2 + 2 + 8 + 8;

		public var wLastChairID:uint;//上一用户
		public var wCurrentUser:uint;//当前用户
		public var fLastAddGold:Number;//加注数目
		public var fCurrentLessGold:Number;//最少加注

		public function CMD_S_AddGold()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_AddGold
		{
			var pos:int=data.position;
			var result:CMD_S_AddGold=new CMD_S_AddGold  ;

			result.wLastChairID=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			result.fLastAddGold=data.readDouble();
			result.fCurrentLessGold=data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_ShowHand.as"
include "../../../Common/Memory.as"