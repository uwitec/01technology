package org.god.GameModule.B_Upgrade.Common
{
	import flash.utils.ByteArray;

	//用户叫牌
	public class CMD_S_CallCard
	{
		public static  const sizeof_CMD_S_CallCard:uint=1 + 1 + 2;

		public var cbCallCard:uint;//叫牌扑克
		public var cbCallCount:uint;//叫牌数目
		public var wCallCardUser:uint;//叫牌用户

		public function CMD_S_CallCard()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_CallCard
		{
			var pos:int=data.position;
			var result:CMD_S_CallCard=new CMD_S_CallCard  ;

			result.cbCallCard=data.readUnsignedByte();
			result.cbCallCount=data.readUnsignedByte();
			result.wCallCardUser=data.readUnsignedShort();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Upgrade.as"
include "../../../Common/Memory.as"