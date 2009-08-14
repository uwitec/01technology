package org.god.Common
{
	//洗码单元
	public class RollbackRateCell
	{
		protected var m_fRollbackRate:Number;//洗码比
		//构造函数
		public function RollbackRateCell(fRollbackRate:Number)
		{
			m_fRollbackRate = fRollbackRate;
		}
		//获取洗码比
		public function getRollbackRate():Number
		{
			return m_fRollbackRate;
		}
		//转换文本
		public function toString():String
		{
			return FuncUtil.fu_GetRollbackRate(m_fRollbackRate);
		}
		//判断相等
		public function equal(rrdata:RollbackRateCell):Boolean
		{
			if(rrdata == null)
				return false;
			var o1:uint = uint((rrdata.getRollbackRate() + 0.000005) * 100000)
			var o2:uint = uint((this.getRollbackRate() + 0.000005) * 100000)
			if(o1 == o2)
				return true;
			
			return false;
		}
	}
}
include "./StringTableDef.as"
