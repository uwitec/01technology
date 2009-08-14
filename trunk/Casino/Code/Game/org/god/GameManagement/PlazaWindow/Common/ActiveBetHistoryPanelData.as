package org.god.GameManagement.PlazaWindow.Common
{	
	//游戏记录激活数据
	public class ActiveBetHistoryPanelData
	{
		public var szAccount:String="";//帐号
		public var fBeginTime:Number=0;//开始时间
		public var fEndTime:Number=0;//结束时间
		public var wGameType:uint;//游戏类型
		public var wGameKind:uint;//游戏种类
		public var cbGameRoundType:uint;//结算有效性
		
		public function ActiveBetHistoryPanelData()
		{
			
		}
	}
}