package org.god.IGameFrame
{
	//进入游戏事件监听者
	public interface IEnterGameEventListener
	{
		//开始进入游戏
		function BeginEnterGame(param0:*,param1:* = null,param2:* = null,param3:* = null):IGamePaneImpl;
		//取消进入游戏
		function CancelEnterGame(game:IGamePaneImpl):void;
		//完成进入游戏
		function FinishEnterGame(game:IGamePaneImpl):void;
	}
}