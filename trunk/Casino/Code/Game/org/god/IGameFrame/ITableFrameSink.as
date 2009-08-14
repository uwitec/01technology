package org.god.IGameFrame
{
	//框架接口
	public interface ITableFrameSink
	{
		//鼠标左键按下
		function OnLButtonHitTable(wTableID:uint, wChairID:uint, bMirror:Boolean):void;
		//鼠标右键按下
		function OnRButtonHitTable(wTableID:uint, wChairID:uint, bMirror:Boolean):void;
		//鼠标双击
		function OnDoubleHitTable(wTableID:uint, bMirror:Boolean):void;
	}
}