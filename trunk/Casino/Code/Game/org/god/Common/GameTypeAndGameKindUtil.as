package org.god.Common
{
	//游戏种类与游戏类型通用辅助
	public class GameTypeAndGameKindUtil
	{
		//获取游戏种类数组
		public static function GetGameTypeArray():Array
		{
			var result:Array = new Array;
			result.push(dwGameTypeMark_All);
			result.push(dwGameTypeMark_Battle);
			result.push(dwGameTypeMark_Video);
			result.push(dwGameTypeMark_Pokie);
			result.push(dwGameTypeMark_Lottery);
			return result;
		}
		//获取游戏类型数组
		public static function GetGameKindArray(wGameType:uint=0):Array
		{
			var result:Array = new Array;
	
			result.push(ID_PLAZA);
				
			if(wGameType == dwGameTypeMark_Battle)
			{
				result.push(ID_B_HOE);
				result.push(ID_B_LAND);
				result.push(ID_B_UNCOVERPIG);
				result.push(ID_B_BACK_JACK);
				result.push(ID_B_UPGRADE);
				result.push(ID_B_SPARROW);
				result.push(ID_B_SHOWHAND);
				result.push(ID_B_THREELEAF);
				result.push(ID_B_SICHUAN_LAND);
			}
			if(wGameType == dwGameTypeMark_Video)
			{
				result.push(ID_V_DRAGONTIGER);
				result.push(ID_V_BACCARAT);
				result.push(ID_V_SUPER6);
				result.push(ID_V_SICBO);
				result.push(ID_V_ROULETTE);
			}
			if(wGameType == dwGameTypeMark_Pokie)
			{
				result.push(ID_P_SLOT_COOLFRUIT); 
				result.push(ID_P_SLOT_POKER);
				result.push(ID_P_SLOT_MAHJONGBALL); 
				result.push(ID_P_SLOT_SOCCER);
				result.push(ID_P_SLOT_STAR97);
				
				result.push(ID_P_VIDEOPOKER_7PK);
				result.push(ID_P_VIDEOPOKER_5PK);
				result.push(ID_P_VIDEOPOKER_5PK1);
				result.push(ID_P_VIDEOPOKER_5PK2);
				
				result.push(ID_P_BIGGAMEMACHINE_FRUIT);
				result.push(ID_P_BIGGAMEMACHINE_LUCKYNUMBER);
				result.push(ID_P_BIGGAMEMACHINE_GUESSTRAIN);
				result.push(ID_P_BIGGAMEMACHINE_MONSTERLEGEND);
				result.push(ID_P_BIGGAMEMACHINE_HORSERACE);
				
				result.push(ID_P_TABLEGAME_BLACKJACK);
				result.push(ID_P_TABLEGAME_WAR);
				result.push(ID_P_TABLEGAME_REDDOG);
				result.push(ID_P_TABLEGAME_ROULETTE);
				result.push(ID_P_TABLEGAME_DRAGONTIGER);
				result.push(ID_P_TABLEGAME_BACCARAT);
				result.push(ID_P_TABLEGAME_SICBO);
			}
			if(wGameType == dwGameTypeMark_Lottery)
			{
				result.push(ID_L_MARKSIX);
			}
			return result;
		}
		//获取全部游戏类型数组
		public static function GetAllGameKindArray():Array
		{
			var result:Array = new Array;

			result.push(ID_B_HOE);
			result.push(ID_B_LAND);
			result.push(ID_B_UNCOVERPIG);
			result.push(ID_B_BACK_JACK);
			result.push(ID_B_UPGRADE);
			result.push(ID_B_SPARROW);
			result.push(ID_B_SHOWHAND);
			result.push(ID_B_THREELEAF);
			result.push(ID_B_SICHUAN_LAND);
			
			result.push(ID_V_DRAGONTIGER);
			result.push(ID_V_BACCARAT);
			result.push(ID_V_SUPER6);
			result.push(ID_V_SICBO);
			result.push(ID_V_ROULETTE);
				
			result.push(ID_P_SLOT_COOLFRUIT); 
			result.push(ID_P_SLOT_POKER);
			result.push(ID_P_SLOT_MAHJONGBALL); 
			result.push(ID_P_SLOT_SOCCER);
			result.push(ID_P_SLOT_STAR97);
				
			result.push(ID_P_VIDEOPOKER_7PK);
			result.push(ID_P_VIDEOPOKER_5PK);
			result.push(ID_P_VIDEOPOKER_5PK1);
			result.push(ID_P_VIDEOPOKER_5PK2);
				
			result.push(ID_P_BIGGAMEMACHINE_FRUIT);
			result.push(ID_P_BIGGAMEMACHINE_LUCKYNUMBER);
			result.push(ID_P_BIGGAMEMACHINE_GUESSTRAIN);
			result.push(ID_P_BIGGAMEMACHINE_MONSTERLEGEND);
			result.push(ID_P_BIGGAMEMACHINE_HORSERACE);
				
			result.push(ID_P_TABLEGAME_BLACKJACK);
			result.push(ID_P_TABLEGAME_WAR);
			result.push(ID_P_TABLEGAME_REDDOG);
			result.push(ID_P_TABLEGAME_ROULETTE);
			result.push(ID_P_TABLEGAME_DRAGONTIGER);
			result.push(ID_P_TABLEGAME_BACCARAT);
			result.push(ID_P_TABLEGAME_SICBO);

			result.push(ID_L_MARKSIX);
			
			return result;
		}
		//获取游戏种类
		public static function GetGameTypeByGameKind(wGameKind:uint):uint
		{
			if(IsBattleGameProcessType(wGameKind))
				return dwGameTypeMark_Battle;
			if(IsVideoGameProcessType(wGameKind))
				return dwGameTypeMark_Video;
			if(IsPokieGameProcessType(wGameKind))
				return dwGameTypeMark_Pokie;
			if(IsLotteryGameProcessType(wGameKind))
				return dwGameTypeMark_Lottery;
			return 0;
		}
	}
}
include "../Net/GLOBALDEF.as"