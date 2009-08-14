package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//亮牌数据
	public class CMD_S_ShowCard
	{
		public static  const sizeof_CMD_S_ShowCard:uint=2 + 4 + 1;

		public var wShowCardUser:uint;//亮牌玩家
		public var bShowCard:Array=new Array(4);//亮牌列表
		public var bShowCardCount:uint;//亮牌数目

		public function CMD_S_ShowCard()
		{

		}
		public static  function readData(data:ByteArray):CMD_S_ShowCard
		{
			var pos:int=data.position;
			var result:CMD_S_ShowCard=new CMD_S_ShowCard  ;


			result.wShowCardUser=data.readUnsignedShort();
			for (var i:uint=0; i < result.bShowCard.length; i++)
			{
				result.bShowCard[i]=data.readUnsignedByte();
			}
			result.bShowCardCount=data.readUnsignedByte();

			data.position=pos;
			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"