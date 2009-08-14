package org.god.GameModule.B_Hoe.Common
{
	import flash.utils.ByteArray;

	//游戏结束
	public class CMD_S_GameEnd
	{
		public static  const sizeof_CMD_S_GameEnd:uint=4 * 8 + 4 + 52;

		public var fGameScore:Array=new Array(4);//游戏积分
		public var bCardCount:Array=new Array(4);//扑克数目
		public var bCardData:Array=new Array(52);//扑克列表 
		public function CMD_S_GameEnd()
		{
		}
		public static  function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd  ;

			for (var i:uint=0; i < result.fGameScore.length; i++)
			{
				result.fGameScore[i]=data.readDouble();
			}
			for (i=0; i < result.bCardCount.length; i++)
			{
				result.bCardCount[i]=data.readUnsignedByte();
			}
			for (i=0; i < result.bCardData.length; i++)
			{
				result.bCardData[i]=data.readUnsignedByte();
			}
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_Hoe.as"
include "../../../Common/Memory.as"