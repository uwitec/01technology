package org.god.Common
{
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagGameProcess;
	import org.god.Net.tagGameStation;
	//游戏树节点
	public class GameTreeNode
	{
		private var m_ItemData:*;//节点数据
		private var m_nItemType:uint;//节点类型
		//构造函数
		public function GameTreeNode(ItemData:*, nItemType:uint)
		{
			m_ItemData = ItemData;
			m_nItemType = nItemType;
		}
		//转换文本
		public function toString():String
		{
			switch (m_nItemType)
			{
					//类型信息
				case SUB_GP_LIST_TYPE :
					{
						var GameType:tagGameType = m_ItemData as tagGameType;
						return GameType.szTypeName;


					};
					//种类消息
				case SUB_GP_LIST_KIND :
					{
						var GameKind:tagGameKind = m_ItemData as tagGameKind;
						return GameKind.szKindName;


					};
					//进程信息
				case SUB_GP_LIST_PROCESS :
					{
						var GameProcess:tagGameProcess = m_ItemData as tagGameProcess;
						return GameProcess.szKindName;


					};
					//站点消息
				case SUB_GP_LIST_STATION :
					{
						var GameStation:tagGameStation = m_ItemData as tagGameStation;
						return GameStation.szStationName;


					};
					//服务器房间
				case SUB_GP_LIST_SERVER :
					{
						var GameServer:tagGameServer = m_ItemData as tagGameServer;
						return GameServer.szServerName;


					};
				case SUB_GP_LIST_ROOT :
					{
						return strGameProduceName;
				}
			};
			return "";
		}
		
		public function getItemType():uint
		{
			return m_nItemType;
		}
		public function getItemData():*
		{
			return m_ItemData;
		}
		public function getItemID():uint
		{
			switch (m_nItemType)
			{
					//类型信息
				case SUB_GP_LIST_TYPE :
					{
						var GameType:tagGameType = m_ItemData as tagGameType;
						return GameType.wTypeID;


					};
					//种类消息
				case SUB_GP_LIST_KIND :
					{
						var GameKind:tagGameKind = m_ItemData as tagGameKind;
						return GameKind.wKindID;


					};
					//进程信息
				case SUB_GP_LIST_PROCESS :
					{
						var GameProcess:tagGameProcess = m_ItemData as tagGameProcess;
						return GameProcess.wKindID;


					};
					//站点消息
				case SUB_GP_LIST_STATION :
					{
						var GameStation:tagGameStation = m_ItemData as tagGameStation;
						return GameStation.wStationID;


					};
					//服务器房间
				case SUB_GP_LIST_SERVER :
					{
						var GameServer:tagGameServer = m_ItemData as tagGameServer;
						return GameServer.wServerID;


					};
				case SUB_GP_LIST_ROOT :
					{
						return 0;

				}
			};
			return 0;
		}
		public function equalItemID(wItemID:uint):Boolean
		{
			switch (m_nItemType)
			{
					//类型信息
				case SUB_GP_LIST_TYPE :
					{
						var GameType:tagGameType = m_ItemData as tagGameType;
						return GameType.wTypeID == wItemID;


					};
					//种类消息
				case SUB_GP_LIST_KIND :
					{
						var GameKind:tagGameKind = m_ItemData as tagGameKind;
						return GameKind.wKindID == wItemID;


					};
					//进程信息
				case SUB_GP_LIST_PROCESS :
					{
						var GameProcess:tagGameProcess = m_ItemData as tagGameProcess;
						return GameProcess.wKindID == wItemID;


					};
					//站点消息
				case SUB_GP_LIST_STATION :
					{
						var GameStation:tagGameStation = m_ItemData as tagGameStation;
						return GameStation.wStationID == wItemID;


					};
					//服务器房间
				case SUB_GP_LIST_SERVER :
					{
						var GameServer:tagGameServer = m_ItemData as tagGameServer;
						return GameServer.wServerID == wItemID;


					};
				case SUB_GP_LIST_ROOT :
					{
						return false;

				}
			};
			return false;
		}

	}
}
include "../GlobalConst.as"
include "../Net/GLOBALDEF.as"
include "../Net/CMD_Plaza.as"