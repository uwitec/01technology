package org.god.Common
{
	//游戏模块名称库
	public class GameModuleNameFactory
	{
		//构造函数
		public function GameModuleNameFactory()
		{
		}
		//获取游戏模块名称
		public static  function GetGameModuleName(wProcessType:uint):String
		{
			return GetGameModulePrixName(wProcessType) + "/" + GetGameModulePrixName(wProcessType) + ".swf";
		}
		//获取游戏简图名称
		public static  function GetGameMiniUI(wProcessType:uint):String
		{
			return GetGameModulePrixName(wProcessType) + "/MUI_" + GetGameModulePrixName(wProcessType) + ".jpg";
		}
		//获取游戏帮助名称
		public static  function GetGameHelpContext(wProcessType:uint):String
		{
			return GetGameModulePrixName(wProcessType) + "/Help_" + GetGameModulePrixName(wProcessType) + ".jpg";
		}
		//获取游戏模块前缀
		public static  function GetGameModulePrixName(wProcessType:uint):String
		{
			switch (wProcessType)
			{
				case ID_B_HOE :
					return "B_Hoe";
				case ID_B_LAND :
					return "B_Land";
				case ID_B_UNCOVERPIG :
					return "B_Uncoverpig";
				case ID_B_BACK_JACK :
					return "B_Blackjack";
				case ID_B_UPGRADE :
					return "B_Upgrade";
				case ID_B_SPARROW :
					return "B_Sparrow";
				case ID_B_SHOWHAND :
					return "B_ShowHand";
				case ID_B_THREELEAF :
					return "B_GoldFlower";
				case ID_B_SICHUAN_LAND :
					return "B_SiChuan_Land";

				case ID_V_DRAGONTIGER :
					return "V_Dragontiger";
				case ID_V_BACCARAT :
					return "V_Baccarat";
				case ID_V_SUPER6 :
					return "V_Super6";
				case ID_V_SICBO :
					return "V_Sicbo";
				case ID_V_ROULETTE :
					return "V_Roulette";

				case ID_P_SLOT_COOLFRUIT :
					return "P_Slot_CoolFruit";
				case ID_P_SLOT_POKER :
					return "P_Slot_Poker";
				case ID_P_SLOT_MAHJONGBALL :
					return "P_Slot_MahjongBall";
				case ID_P_SLOT_SOCCER :
					return "P_Slot_Soccer";
				case ID_P_SLOT_STAR97 :
					return "P_Slot_Star97";
				case ID_P_VIDEOPOKER_7PK :
					return "P_VideoPoker_7PK";
				case ID_P_VIDEOPOKER_5PK :
					return "P_VideoPoker_5PK";
				case ID_P_VIDEOPOKER_5PK1 :
					return "P_VideoPoker_5PK1";
				case ID_P_VIDEOPOKER_5PK2 :
					return "P_VideoPoker_5PK2";
				case ID_P_BIGGAMEMACHINE_FRUIT :
					return "P_BigGameMachine_Fruit";
				case ID_P_BIGGAMEMACHINE_LUCKYNUMBER :
					return "P_BigGameMachine_LuckyNumber";
				case ID_P_BIGGAMEMACHINE_GUESSTRAIN :
					return "P_BigGameMachine_GuessTrain";
				case ID_P_BIGGAMEMACHINE_MONSTERLEGEND :
					return "P_BigGameMachine_MonsterLegend";
				case ID_P_BIGGAMEMACHINE_HORSERACE :
					return "P_BigGameMachine_HorseRace";
				case ID_P_TABLEGAME_BLACKJACK :
					return "P_TableGame_BlackJack";
				case ID_P_TABLEGAME_WAR :
					return "P_TableGame_War";
				case ID_P_TABLEGAME_REDDOG :
					return "P_TableGame_RedDog";
				case ID_P_TABLEGAME_ROULETTE :
					return "P_TableGame_Roulette";
				case ID_P_TABLEGAME_DRAGONTIGER :
					return "P_TableGame_DragonTiger";
				case ID_P_TABLEGAME_BACCARAT :
					return "P_TableGame_Baccarat";
				case ID_P_TABLEGAME_SICBO :
					return "P_TableGame_Sicbo";
			}
			return "";
		}
		//游戏类型前缀
		public static  function GetGameTypePrixName(wGameType:uint):String
		{
			switch (wGameType)
			{
				case dwGameTypeMark_All :
					return "全部";
				case dwGameTypeMark_Battle :
					return "对战类";
				case dwGameTypeMark_Video :
					return "视频类";
				case dwGameTypeMark_Pokie :
					return "电子类";
				case dwGameTypeMark_Lottery :
					return "博彩类";
			}
			return "";
		}
		//获取游戏种类标记
		public static  function GetGameTypeMark(wProcessType:uint):uint
		{
			if (IsBattleGameProcessType(wProcessType))
			{
				return dwGameTypeMark_Battle;
			}
			else if (IsVideoGameProcessType(wProcessType))
			{
				return dwGameTypeMark_Video;
			}
			else if (IsPokieGameProcessType(wProcessType))
			{
				return dwGameTypeMark_Pokie;
			}
			else if (IsLotteryGameProcessType(wProcessType))
			{
				return dwGameTypeMark_Lottery;
			}
			else
			{
				return dwGameTypeMark_All;
			}
		}//获取游戏类型名
		public static  function GetGameKindPrixName(wGameKind:uint):String
		{
			switch (wGameKind)
			{
				case ID_PLAZA :
					return "全部";

				case ID_B_HOE :
					return "对战-锄大地";
				case ID_B_LAND :
					return "对战-斗地主";
				case ID_B_UNCOVERPIG :
					return "对战-拱猪";
				case ID_B_BACK_JACK :
					return "对战-黑杰克";
				case ID_B_UPGRADE :
					return "对战-升级";
				case ID_B_SPARROW :
					return "对战-麻将";
				case ID_B_SHOWHAND :
					return "对战-梭哈";
				case ID_B_THREELEAF :
					return "对战-诈金花";
				case ID_B_SICHUAN_LAND :
					return "对战-四川斗地主";

				case ID_V_DRAGONTIGER :
					return "视频-龙虎";
				case ID_V_BACCARAT :
					return "视频-百家乐";
				case ID_V_SUPER6 :
					return "视频-Super6";
				case ID_V_SICBO :
					return "视频-骰宝";
				case ID_V_ROULETTE :
					return "视频-轮盘";

				case ID_P_SLOT_COOLFRUIT :
					return "电子-水果拉霸机";
				case ID_P_SLOT_POKER :
					return "电子-扑克拉霸机";
				case ID_P_SLOT_MAHJONGBALL :
					return "电子-筒子拉霸机";
				case ID_P_SLOT_SOCCER :
					return "电子-足球拉霸机";
				case ID_P_SLOT_STAR97 :
					return "电子-明星97拉霸机";

				case ID_P_VIDEOPOKER_7PK :
					return "电子-7PK电动扑克";
				case ID_P_VIDEOPOKER_5PK :
					return "电子-5PK电动扑克";
				case ID_P_VIDEOPOKER_5PK1 :
					return "电子-百搭二王电动扑克";
				case ID_P_VIDEOPOKER_5PK2 :
					return "电子-花牌电动扑克";

				case ID_P_BIGGAMEMACHINE_FRUIT :
					return "电子-水果大转轮";
				case ID_P_BIGGAMEMACHINE_LUCKYNUMBER :
					return "电子-数字大转轮";
				case ID_P_BIGGAMEMACHINE_GUESSTRAIN :
					return "电子-钻石列车";
				case ID_P_BIGGAMEMACHINE_MONSTERLEGEND :
					return "电子-圣兽传说";
				case ID_P_BIGGAMEMACHINE_HORSERACE :
					return "电子-跑马";

				case ID_P_TABLEGAME_BLACKJACK :
					return "电子-21点";
				case ID_P_TABLEGAME_WAR :
					return "电子-斗大";
				case ID_P_TABLEGAME_REDDOG :
					return "电子-红狗";
				case ID_P_TABLEGAME_ROULETTE :
					return "电子-轮盘";
				case ID_P_TABLEGAME_DRAGONTIGER :
					return "电子-龙虎";
				case ID_P_TABLEGAME_BACCARAT :
					return "电子-百家乐";
				case ID_P_TABLEGAME_SICBO :
					return "电子-骰宝";

				case ID_L_MARKSIX :
					return "彩票-六合彩";
			}
			return "";
		}
	}
}
include "../Net/GLOBALDEF.as";