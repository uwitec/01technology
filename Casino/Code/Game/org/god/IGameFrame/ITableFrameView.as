package org.god.IGameFrame
{
	import org.aswing.geom.IntPoint;

	//框架接口
	public interface ITableFrameView
	{
		//配置函数
		function InitTableFrame( wTableCount:uint,  
								wPlayerCount:uint, 
								pGameServer:IServerItem,
								pGameKind:IServerItem,
								pIUnknownEx:ITableFrameSink):Boolean;
		//销毁函数
		function DestroyTableFrame():void;
		//更新桌子
		function UpdateTableView( wTableID:uint,  bMustUpdate:Boolean):void;
		//获取游戏桌指针
		function GetTableArrayPtr( wTableID:uint):ITableView;
		//设置显示
		function ShowUserInfo( bShowUser:Boolean):Boolean;
		//设置视图桌子
		function EnsureVisibleTable( wTableID:uint):Boolean;
		//闪动椅子
		function FlashTable( wTableID:uint):Boolean;
		//闪动椅子
		function FlashChair( wTableID:uint,  wChairID:uint):Boolean;
		//获取桌子数目
		function GetTableCount():uint;
		//获取椅子数目
		function GetChairCount():uint;
		//设置用户信息
		function SetUserInfo( wTableID:uint,  wChairID:uint,  pIUserItem:IUserItem):Boolean;
		//设置游戏标志 
		function SetPlayFlag( wTableID:uint,  bPlaying:Boolean):void;
		//设置密码标志
		function SetPassFlag( wTableID:uint,  bPass:Boolean):void;
		//获取用户信息
		function GetUserInfo( wTableID:uint,  wChairID:uint):IUserItem;
		//获取空椅子数
		function GetNullChairCount( wTableID:uint,  wNullChairID:uint):IntPoint;
		//查询游戏标志
		function QueryPlayFlag( wTableID:uint):Boolean;
		//查询密码标志
		function QueryPassFlag( wTableID:uint):Boolean;
		//获取桌子属性
		function GetTableAttrib( wTableID:uint):tagTableState;
	}
}