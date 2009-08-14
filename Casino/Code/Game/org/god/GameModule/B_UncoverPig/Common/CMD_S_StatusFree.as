package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_StatusFree
	{
		public static  const sizeof_CMD_S_StatusFree:uint=0;

		public function CMD_S_StatusFree()
		{
			
		}
		public static  function readData(data:ByteArray):CMD_S_StatusFree
		{
			var pos:int=data.position;
			var result:CMD_S_StatusFree=new CMD_S_StatusFree  ;
			
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"