package 
{
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagGameProcess;
	import org.god.Net.tagGameStation;
	
	import org.aswing.Icon;
	//游戏图标辅助
	public class GameIconUtil
	{
		//构造函数
		public function GameIconUtil()
		{
		}
		//获取图标
		public static function getFolderExpandedIcon(dwItemType:uint,pItemData:*):Icon
		{
			return getLeafIcon(dwItemType,pItemData);
		}
		//获取图标
		public static function getFolderCollapsedIcon(dwItemType:uint,pItemData:*):Icon
		{
			return getLeafIcon(dwItemType,pItemData);
		}
		//获取图标
		public static function getLeafIcon(dwItemType:uint,pItemData:*):Icon
		{
			switch (dwItemType)
			{
					//类型信息
				case SUB_GP_LIST_TYPE :
					{
						var GameType:tagGameType = pItemData as tagGameType;
						return GameIcon.loadGameIcon(GameType.nbImageID);


					};
					//种类消息
				case SUB_GP_LIST_KIND :
					{
						var GameKind:tagGameKind = pItemData as tagGameKind;
						return GameIcon.loadGameIcon(Convert_ProcessTypeToImageID(GameKind.wProcessType));


					};
					//进程信息
				case SUB_GP_LIST_PROCESS :
					{
						
						return null;


					};
					//站点消息
				case SUB_GP_LIST_STATION :
					{
						
						return null;


					};
					//服务器房间
				case SUB_GP_LIST_SERVER :
					{
						
						return GameIcon.loadGameIcon(254);


					};
				case SUB_GP_LIST_ROOT :
					{
						return GameIcon.loadGameIcon(255);

				}
			};
			return null;
		}
		//获取游戏图标ID
		private static function Convert_ProcessTypeToImageID(wProcessType:uint):uint
		{
			if(wProcessType < 100)
				return wProcessType + 10;
			else if(wProcessType >= 100)
				return wProcessType + 20;
			return 0;
		}
	}
}
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"