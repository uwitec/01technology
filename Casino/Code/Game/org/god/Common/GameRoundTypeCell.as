package org.god.Common
{
	//游戏局类型单元
	public class GameRoundTypeCell
	{
		protected var m_cbGameRoundType:uint;//游戏局类型标记
		//构造函数
		public function GameRoundTypeCell(cbGameRoundType:uint)
		{
			m_cbGameRoundType = cbGameRoundType;
		}
		//获取游戏局类型
		public function getGameRoundType():uint
		{
			return m_cbGameRoundType;
		}
		//转换文本
		public function toString():String
		{
			return GameRoundTypeUtil.GetGameRoundTypeName(m_cbGameRoundType);
		}
	}
}