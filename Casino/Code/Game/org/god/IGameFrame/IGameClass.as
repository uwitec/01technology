package org.god.IGameFrame
{
	import flash.display.MovieClip;
	//游戏类接口
	public interface IGameClass extends IChannelMessageSink 
	{
		//创建游戏端
		function CreateGameClient(clientContainer:IGameClassContainer):Boolean;
		//销毁游戏端
		function DestroyGameClient():void;
		//获取影片
		function GetMovieClip():MovieClip;
		//获取游戏视图
		function GetGameView():IGameView;
		//退出游戏
		function SendEventExitGameClient(bAutoSitChairAgain:Boolean = false):void;
	}
}