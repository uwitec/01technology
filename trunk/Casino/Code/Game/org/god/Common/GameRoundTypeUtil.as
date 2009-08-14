package org.god.Common
{
	//游戏局类型
	public class GameRoundTypeUtil
	{
		public static const GameRoundType_Valid = 1;//有效
		public static const GameRoundType_Invalid = 0;//注销
		//获取游戏局类型名称
		public static function GetGameRoundTypeName(cbGameRoundType:uint):String
		{
			switch(cbGameRoundType)
			{
				case GameRoundType_Valid: return "有效";
				case GameRoundType_Invalid: return "注销";
			}
			return "???";
		}
	}
}