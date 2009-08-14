package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_StatusShowCard
	{
		public static  const sizeof_CMD_S_StatusShowCard:uint=2 + 2 * GAME_PLAYER + GAME_PLAYER * 4 +
		GAME_PLAYER + HAND_CARD_COUNT;

		public var wCurrentUser:uint;//当前玩家
		public var ShowCardUser:Array=new Array(GAME_PLAYER);//亮牌玩家
		public var ShowCard:Array=new Array(GAME_PLAYER);//玩家亮牌
		public var ShowCardCount:Array=new Array(GAME_PLAYER);//亮牌数目
		public var bCardData:Array=new Array(HAND_CARD_COUNT);//手上扑克

		public function CMD_S_StatusShowCard()
		{
			for(var i:uint = 0; i < ShowCard.length;i ++)
			{
				ShowCard[i] = new Array(4);
			}
		}
		public static  function readData(data:ByteArray):CMD_S_StatusShowCard
		{
			var pos:int=data.position;
			var result:CMD_S_StatusShowCard=new CMD_S_StatusShowCard  ;
			
			result.wCurrentUser = data.readUnsignedShort();
			for (var i:uint=0; i < result.ShowCardUser.length; i++)
			{
				result.ShowCardUser[i]=data.readUnsignedShort();
			}
			for (i=0; i < result.ShowCard.length; i++)
			{
				for(var j:uint = 0; j < result.ShowCard[i].length; j ++)
				{
					result.ShowCard[i][j]=data.readUnsignedByte();
				}
			}
			for (i=0; i < result.ShowCardCount.length; i++)
			{
				result.ShowCardCount[i]=data.readUnsignedByte();
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
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"