package org.god.Common
{
	import org.god.Common.FuncUtil;
	//用户级别单元
	public class UserLevelCell
	{
		protected var m_cbLevel:uint;//用户等级
		//构造函数
		public function UserLevelCell(cbLevel:uint)
		{
			m_cbLevel = cbLevel;
		}
		//获取用户等级
		public function getLevel():uint
		{
			return m_cbLevel;
		}
		//用户等级名称
		public function toString():String
		{
			return FuncUtil.fu_GetPeerLevelName(m_cbLevel);
		}
	}
}