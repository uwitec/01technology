package org.god.GameModule.B_GoldFlower.Common
{
	import flash.utils.ByteArray;

	//用户加注
	public class CMD_C_AddGold
	{
		public static  const sizeof_CMD_C_AddGold:uint=8;

		public var fGold:Number=0;//放弃用户

		public function CMD_C_AddGold()
		{
		}
		public static  function readData(data:ByteArray):CMD_C_AddGold
		{
			var pos:int=data.position;
			var result:CMD_C_AddGold=new CMD_C_AddGold  ;
			result.fGold=data.readDouble();
			data.position=pos;
			return result;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();
			result.writeDouble(fGold);
			return result;
		}
	}
}
include "./CMD_GoldFlower.as"
include "../../../Common/Memory.as"