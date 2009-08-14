package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//甩牌结果
	public class CMD_S_ThrowResult
	{
		public static  const sizeof_CMD_S_ThrowResult:uint=2 + 2 + 1 + 1 + 104;
		public static  const sizeof_cbCardDataArray:uint =104;

		public var wOutCardUser:uint;//出牌玩家
		public var wCurrentUser:uint;//当前玩家
		public var cbThrowCardCount:uint;//扑克数目
		public var cbResultCardCount:uint;//扑克数目
		public var cbCardDataArray:Array=new Array(104);//扑克数组
		
		public function CMD_S_ThrowResult()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_ThrowResult
		{
			var pos:int=data.position;
			var result:CMD_S_ThrowResult=new CMD_S_ThrowResult  ;

			result.wOutCardUser=data.readUnsignedShort();
			result.wCurrentUser=data.readUnsignedShort();
			result.cbThrowCardCount=data.readUnsignedByte();
			result.cbResultCardCount=data.readUnsignedByte();
			
			result.cbCardDataArray.length = result.cbThrowCardCount + result.cbResultCardCount;
			for (var i:uint=0; i < result.cbCardDataArray.length; i++)
			{
				result.cbCardDataArray[i]=data.readUnsignedByte();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"