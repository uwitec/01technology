package org.god.IGameFrame
{

	import org.aswing.geom.IntPoint;
	
	//桌子接口
	public interface ITableView
	{
		//初始化函数
		function InitTableView(pGameServer:IServerItem,
							   pGameKind:IServerItem,
							   wTableID:uint, 
							   wChairCount:uint,
							   pITableFrameView:ITableFrameView):void;
		function DestroyTableView():void;
		
		//设置用户信息
		function SetUserInfo( wChairID:uint, pIUserItem:IUserItem):Boolean;
		//设置鼠标按下
		function SetMouseDown( bMouseDown:Boolean):void;
		//设置焦点框架
		function SetFocusFrame( bFocusFrame:Boolean):void;
		//设置盘旋位置
		function SetHoverChairID( wHoverChairID:uint):void;
		//设置游戏标志 
		function SetPlayFlag( bPlaying:Boolean):void;
		//设置密码标志
		function SetPassFlag( bPass:Boolean):void;
		//设置备注
		function SetTableNote(pszNote:String):void;
		//获取用户信息
		function GetUserInfo(wChairID:uint):IUserItem;
		//获取空椅子数
		function GetNullChairCount(wNullChairID:uint):IntPoint;
		//查询焦点框架
		function QueryFocusFrame():Boolean;
		//查询盘旋位置
		function QueryHoverChairID():uint;
		//查询游戏标志
		function QueryPlayFlag():Boolean;
		//查询密码标志
		function QueryPassFlag():Boolean;
		//获取桌子属性
		function GetTableAttrib():tagTableState;

	}
}