package org.god.IGameFrame
{
	//游戏类容器
	public interface IGameClassContainer extends IChannelMessageSink
	{
		//活动显示游戏端
		function ActiveGameClass(game:IGameClass):void;
		//销毁游戏端
		function DestroyGameClass(game:IGameClass, bAutoSitChairAgain:Boolean = false):void;
	}
}