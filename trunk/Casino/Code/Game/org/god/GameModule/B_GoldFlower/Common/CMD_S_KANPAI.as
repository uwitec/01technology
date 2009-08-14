package org.god.GameModule.B_GoldFlower.Common
{
	import flash.utils.ByteArray;

	//发牌数据包
	public class CMD_S_KANPAI
	{
		public static  const sizeof_CMD_S_KANPAI:uint=GAME_PLAYER+1;

		public var  bKanPaiFlag:Array = new Array(GAME_PLAYER+1);        //是否看牌状态 ADD
		
		public function CMD_S_KANPAI()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_KANPAI
		{
			var pos:int=data.position;
			var result:CMD_S_KANPAI=new CMD_S_KANPAI  ;

			
			for (var i:uint=0; i < result.bKanPaiFlag.length; i++)
			{
				result.bKanPaiFlag[i]=data.readUnsignedByte();
			}

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_GoldFlower.as"
include "../../../Common/Memory.as"