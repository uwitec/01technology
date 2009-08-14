package org.god.Common
{
	import org.god.Common.GameModuleNameFactory;
	//游戏类型辅助
	public class GameTypeCell
	{
		protected var m_wGameType:uint;//游戏类型
		//构造函数
		public function GameTypeCell(wGameType:uint)
		{
			m_wGameType = wGameType;
		}
		//获取游戏类型
		public function getGameType():uint
		{
			return m_wGameType;
		}
		//转换文本
		public function toString():String
		{
			return GameModuleNameFactory.GetGameTypePrixName(m_wGameType);
		}
	}
}