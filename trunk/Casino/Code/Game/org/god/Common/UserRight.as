package org.god.Common
{
	//用户权限辅助
	public class UserRight
	{
		public static const UR_CAN_PLAYBATTLEGAME:uint 	= dwGameTypeMark_Battle;//是否允许玩对战游戏
		public static const UR_CAN_PLAYVIDEOGAME:uint 	= dwGameTypeMark_Video;//是否允许玩真人视频游戏
		public static const UR_CAN_PLAYPOKIEGAME:uint 	= dwGameTypeMark_Pokie;//是否允许玩电子游戏
		public static const UR_CAN_PLAYLOTTERYGAME:uint = dwGameTypeMark_Lottery;//是否允许玩彩票游戏
		//构造函数
		public function UserRight()
		{
		}
		public static function CanPlayBattleGame(dwUserRight:uint):Boolean//是否允许玩对战游戏
		{
			return (dwUserRight & UR_CAN_PLAYBATTLEGAME)!=0 ? false : true;
		}
		public static function CanPlayVideoGame(dwUserRight:uint):Boolean//是否允许玩真人视频游戏
		{
			return (dwUserRight & UR_CAN_PLAYVIDEOGAME)!=0 ? false : true;
		}
		public static function CanPlayPokieGame(dwUserRight:uint):Boolean//是否允许玩电子游戏
		{
			return (dwUserRight & UR_CAN_PLAYPOKIEGAME)!=0 ? false : true;
		}
		public static function CanPlayLotteryGameGame(dwUserRight:uint):Boolean//是否允许玩彩票游戏
		{
			return (dwUserRight & UR_CAN_PLAYLOTTERYGAME)!=0 ? false : true;
		}
		////////////////////////////////////////////////////////
		public static function LimitPlayBattleGame(dwUserRight:uint):uint//允许玩对战游戏
		{
			return (dwUserRight | UR_CAN_PLAYBATTLEGAME);
		}
		public static function LimitPlayVideoGame(dwUserRight:uint):uint//允许玩真人视频游戏
		{
			return (dwUserRight | UR_CAN_PLAYVIDEOGAME);
		}
		public static function LimitPlayPokieGame(dwUserRight:uint):uint//允许玩电子游戏
		{
			return (dwUserRight | UR_CAN_PLAYPOKIEGAME);
		}
		public static function LimitPlayLotteryGameGame(dwUserRight:uint):uint//允许玩彩票游戏
		{
			return (dwUserRight | UR_CAN_PLAYLOTTERYGAME);
		}
	}
}
include "../Net/GlobalDef.as"