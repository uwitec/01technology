package org.god.GameModule.B_UncoverPig.Common
{
	import flash.utils.ByteArray;

	//游戏状态
	public class CMD_S_GameEnd
	{
		public static  const sizeof_CMD_S_GameEnd:uint=4 * 8 + GAME_PLAYER * 2 + GAME_PLAYER * 4 +
		GAME_PLAYER * HAND_CARD_COUNT + GAME_PLAYER +GAME_PLAYER + GAME_PLAYER * 16 + GAME_PLAYER + GAME_PLAYER;

		public var fGameScore:Array=new Array(4);//游戏积分
		public var ShowCardUser:Array=new Array(GAME_PLAYER);//亮牌玩家
		public var ShowCard:Array=new Array(GAME_PLAYER);//玩家亮牌
		public var bCardData:Array=new Array(GAME_PLAYER);//手上扑克
		public var bCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var bTurnCardData:Array=new Array(GAME_PLAYER);//桌面出牌
		public var ScoreCard:Array=new Array(GAME_PLAYER);//分数扑克
		public var bScoreCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var bShowCardCount:Array=new Array(GAME_PLAYER);//亮牌数目

		public function CMD_S_GameEnd()
		{
			for (var i:uint=0; i < ShowCard.length; i++)
			{
				ShowCard[i]=new Array(4);
			}
			for (var i:uint=0; i < bCardData.length; i++)
			{
				bCardData[i]=new Array(HAND_CARD_COUNT);
			}
			for (var i:uint=0; i < ScoreCard.length; i++)
			{
				ScoreCard[i]=new Array(16);
			}
		}
		public static  function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd  ;
			
			for (var i:uint=0; i < result.fGameScore.length; i++)
			{
				result.fGameScore[i]=data.readDouble();
			}
			for (i=0; i < result.ShowCardUser.length; i++)
			{
				result.ShowCardUser[i]=data.readUnsignedShort();
			}
			for (i=0; i < result.ShowCard.length; i++)
			{
				for (var j:uint=0; j < result.ShowCard[i].length; j++)
				{
					result.ShowCard[i][j]=data.readUnsignedByte();
				}
			}
			for (i=0; i < result.bCardData.length; i++)
			{
				for (var j:uint=0; j < result.bCardData[i].length; j++)
				{
					result.bCardData[i][j]=data.readUnsignedByte();
				}
			}
			
			for (i=0; i < result.bCardCount.length; i++)
			{
				result.bCardCount[i]=data.readUnsignedByte();
			}
			for (i=0; i < result.bTurnCardData.length; i++)
			{
				result.bTurnCardData[i]=data.readUnsignedByte();
			}
			for (i=0; i < result.ScoreCard.length; i++)
			{
				for (var j:uint=0; j < result.ScoreCard[i].length; j++)
				{
					result.ScoreCard[i][j]=data.readUnsignedByte();
				}
			}
			
			for (i=0; i < result.bScoreCardCount.length; i++)
			{
				result.bScoreCardCount[i]=data.readUnsignedByte();
			}
			
			for (i=0; i < result.bShowCardCount.length; i++)
			{
				result.bShowCardCount[i]=data.readUnsignedByte();
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "./CMD_UncoverPig.as"
include "../../../Common/Memory.as"