package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//等待状态
	public class CMD_S_StatusWait
	{
		public static  const sizeof_CMD_S_StatusWait:uint=1 + 1 + 2 + 2;

		//游戏变量
		public var cbPackCount:uint;//副数数目
		public var cbMainValue:uint;//主牌数值
		public var cbValueOrder:Array=new Array(2);//主牌数值

		//用户变量
		public var wBankerUser:uint;//庄家用户

		public function CMD_S_StatusWait()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_StatusWait
		{
			var pos:int=data.position;
			var result:CMD_S_StatusWait=new CMD_S_StatusWait  ;
			result.cbPackCount=data.readUnsignedByte();
			result.cbMainValue=data.readUnsignedByte();
			for (var i:uint=0; i < result.cbValueOrder.length; i++)
			{
				result.cbValueOrder[i]=data.readUnsignedByte();
			}
			result.wBankerUser=data.readUnsignedShort();
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"