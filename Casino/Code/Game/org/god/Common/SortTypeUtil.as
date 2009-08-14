package org.god.Common
{
	//排序通用
	public class SortTypeUtil
	{
		public static  const SORTTYPE_TimeDesc:int = 1;//时间降序
		public static  const SORTTYPE_TimeAsc:int = SORTTYPE_TimeDesc + 1;//时间升序
		public static  const SORTTYPE_BetScoreDesc:int = SORTTYPE_TimeAsc + 1;//投注降序
		public static  const SORTTYPE_BetScoreAsc:int  = SORTTYPE_BetScoreDesc + 1;//投注升序
		public static  const SORTTYPE_WinScoreDesc :int = SORTTYPE_BetScoreAsc + 1;//派彩降序
		public static  const SORTTYPE_WinScoreAsc :int = SORTTYPE_WinScoreDesc + 1;//派彩升序
		public static  const SORTTYPE_ValidScoreDesc :int = SORTTYPE_WinScoreAsc + 1;//洗码降序
		public static  const SORTTYPE_ValidScoreAsc:int  = SORTTYPE_ValidScoreDesc + 1;//洗码升序
		public static  const SORTTYPE_RollbackDesc :int = SORTTYPE_ValidScoreAsc + 1;//洗码降序
		public static  const SORTTYPE_RollbackAsc :int = SORTTYPE_RollbackDesc + 1;//洗码升序
		public static  const SORTTYPE_RoundCountDesc :int = SORTTYPE_RollbackAsc + 1;//游戏局数目降序
		public static  const SORTTYPE_RoundCountAsc :int = SORTTYPE_RoundCountDesc + 1;//游戏局数目升序
		//获取排序类型文本
		public static function GetSortTypeName(cbSortType:uint):String
		{
			switch (cbSortType)
			{
				case SORTTYPE_TimeDesc :
					return "时间降序";
				case SORTTYPE_TimeAsc :
					return "时间升序";
				case SORTTYPE_BetScoreDesc :
					return "投注降序";
				case SORTTYPE_BetScoreAsc :
					return "投注升序";
				case SORTTYPE_WinScoreDesc :
					return "派彩降序";
				case SORTTYPE_WinScoreAsc :
					return "派彩升序";
				case SORTTYPE_ValidScoreDesc :
					return "实货量降序";
				case SORTTYPE_ValidScoreAsc :
					return "实货量升序";
				case SORTTYPE_RollbackDesc :
					return "洗码降序";
				case SORTTYPE_RollbackAsc :
					return "洗码升序";
				case SORTTYPE_RoundCountDesc :
					return "局数目降序";
				case SORTTYPE_RoundCountAsc :
					return "局数目升序";

			}
			return "???";
		}
	}
}