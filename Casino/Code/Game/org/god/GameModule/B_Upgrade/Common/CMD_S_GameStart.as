package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//游戏开始
	public class CMD_S_GameStart
	{
		public static  const sizeof_CMD_S_GameStart:uint=2 + 1 + 1 + 2;

		public var wBankerUser:uint;//庄家用户
		public var cbPackCount:uint;//副数数目
		public var cbMainValue:uint;//主牌数值
		public var cbValueOrder:Array=new Array(2);//主牌数值

		public function CMD_S_GameStart()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_GameStart
		{
			var pos:int=data.position;
			var result:CMD_S_GameStart=new CMD_S_GameStart  ;
			result.wBankerUser=data.readUnsignedShort();
			result.cbPackCount=data.readUnsignedByte();
			result.cbMainValue=data.readUnsignedByte();
			for (var i:uint=0; i < result.cbValueOrder.length; i++)
			{
				result.cbValueOrder[i]=data.readUnsignedByte();
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"