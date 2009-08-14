package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//游戏状态
	public class CMD_S_StatusFree
	{
		public static const sizeof_CMD_S_StatusFree:uint = 8 + 2;
		
		public var fCellScore:Number = 0;//基础金币
		public var wBankerUser:uint;//庄家用户

		public function CMD_S_StatusFree()
		{
		}
		public static function readData(data:ByteArray):CMD_S_StatusFree
		{
			var pos:int=data.position;
			var result:CMD_S_StatusFree=new CMD_S_StatusFree;
			result.fCellScore = data.readDouble();
			result.wBankerUser = data.readUnsignedShort();
			
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"