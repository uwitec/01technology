package org.god.IGameFrame
{
	import flash.display.MovieClip;
	//游戏视图
	public interface IGameView
	{
		//初始游戏视图
		function InitGameView():Boolean;
		//销毁游戏视图
		function DestroyGameView():void;
		//获取游戏视图影片
		function GetGameViewMC():MovieClip;
	}
}