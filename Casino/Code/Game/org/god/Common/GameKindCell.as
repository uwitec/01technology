package org.god.Common
{
	//游戏类型单元
	public class GameKindCell
	{
		protected var m_wGameKind:uint;//游戏类型
		//构造函数
		public function GameKindCell(wGameKind:uint)
		{
			m_wGameKind = wGameKind;
		}
		//获取游戏类型
		public function getGameKind():uint
		{
			return m_wGameKind;
		}
		//转换文本
		public function toString():String
		{
			return GameModuleNameFactory.GetGameKindPrixName(m_wGameKind);
		}
	}
}