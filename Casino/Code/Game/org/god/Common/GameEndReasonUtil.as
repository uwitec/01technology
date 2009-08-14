package org.god.Common
{
	//游戏结束通用辅助
	public class GameEndReasonUtil
	{
		//结束原因
		public static  const GER_NORMAL:uint=0x00;//常规结束
		public static  const GER_DISMISS:uint=0x01;//游戏解散
		public static  const GER_USER_LEFT:uint=0x02;//用户离开
		//构造函数
		public function GameEndReasonUtil()
		{
		}
		//获取结束原因文本
		public static function GetGameEndReasonText(cbEndReason:uint):String
		{
			switch (cbEndReason)
			{
				case GER_NORMAL :
					return "常规结束";
				case GER_DISMISS :
					return "游戏解散";
				case GER_USER_LEFT :
					return "玩家逃跑";
				default :
					break;
			}
			return "???";
		}
	}
}