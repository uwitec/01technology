package org.god.IGameFrame
{
	//用户回调
	public interface IUserManagerSink
	{
		//用户激活
		function OnUserItemAcitve(pIUserItem:IUserItem):void;
		//用户更新
		function OnUserItemUpdate( pIUserItem:IUserItem):void;
		//用户删除
		function OnUserItemDelete( pIUserItem:IUserItem):void;
	}
}