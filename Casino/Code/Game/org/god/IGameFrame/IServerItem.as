package org.god.IGameFrame
{
	//服务项
	public interface IServerItem
	{
		//获取游戏文本
		function toString():String;
		//获取服务项类型
		function getItemType():uint;
		//获取服务项数据
		function getItemData():*;
	}
}