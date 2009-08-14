package org.god.Common
{
	import org.god.Common.GlobalCommonMain;
	//用户游戏状态辅助通用
	public class GameUserStatusUtil
	{
		//构造函数
		public function GameUserStatusUtil()
		{
		}
		//获取用户游戏状态名称
		public static function GetUserStatusName(cbUserStatus:uint):String
		{
			switch (cbUserStatus)
			{
				case US_NULL :return "空闲"
				case US_FREE :return "站立"
				case US_SIT : return "坐下"
				case US_READY :return "同意"
				case US_LOOKON :return "旁观"
				case US_PLAY :return "游戏"
				case US_OFFLINE :return "断线"
				default:
				break;
			}
			return "";
		}
		//获取游戏桌名称
		public static function GetTableIDName(wTableID:uint):String
		{
			if(wTableID == INVALID_TABLE)
				return "";
			else
			{
				if(AllowRightView())
					return Number(wTableID).toFixed();
				else
					return "*";
			}
		}
		//判断权限
		protected static function AllowRightView():Boolean
		{
			if(GlobalCommonMain.getGlobalCommonMain().GetUserLevel() <= 1)
			{
				return true;
			}
			else
			return false;
		}
	}
}
include "../Net/GlobalDef.as"