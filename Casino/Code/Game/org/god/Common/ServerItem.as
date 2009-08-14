package org.god.Common
{
	import org.god.IGameFrame.IServerItem;
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagGameProcess;
	import org.god.Net.tagGameStation;
	//游戏服务项目
	public class ServerItem implements IServerItem
	{
		private var m_Node:GameTreeNode;//树节点
		//构造函数
		public function ServerItem(ItemData:*, nItemType:uint)
		{
			m_Node = new GameTreeNode(ItemData, nItemType);
		}
		//转换文本
		public function toString():String
		{
			return m_Node.toString();
		}
		//获取项目类型
		public function getItemType():uint
		{
			return m_Node.getItemType();
		}
		//获取项目数据
		public function getItemData():*
		{
			return m_Node.getItemData();
		}
		//判断相等ID
		public function equalItemID(wItemID:uint):Boolean
		{
			return m_Node.equalItemID(wItemID);
		}

	}
}

include "../Net/GLOBALDEF.as"
include "../Net/CMD_Plaza.as"
