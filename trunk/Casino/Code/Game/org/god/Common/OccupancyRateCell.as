package org.god.Common
{
	//占成比单元
	public class OccupancyRateCell
	{
		protected var m_fOccupancyRate:Number;//占成比
		protected var m_cbOccupancyRate_NoFlag:uint;//不占成成标记
		//构造函数
		public function OccupancyRateCell(fOccupancyRate:Number,
										  cbOccupancyRate_NoFlag:uint = 0)
		{
			m_fOccupancyRate = fOccupancyRate;
			m_cbOccupancyRate_NoFlag = cbOccupancyRate_NoFlag;
		}
		//转换文本
		public function toString():String
		{
			return FuncUtil.fu_GetOccupancyRateName(m_fOccupancyRate,m_cbOccupancyRate_NoFlag);
		}
		//获取占成比
		public function getOccupancyRate():Number
		{
			return m_fOccupancyRate;
		}
		//获取不占成标记
		public function getOccupancyRate_NoFlag():uint
		{
			return m_cbOccupancyRate_NoFlag;
		}
		//判断相等
		public function equal(d:OccupancyRateCell):Boolean
		{
			if(d == null)
				return false;
			if(d.getOccupancyRate_NoFlag() == this.getOccupancyRate_NoFlag())
			{
				var o1:uint = uint((d.getOccupancyRate() + 0.00005) * 10000)
				var o2:uint = uint((this.getOccupancyRate() + 0.00005) * 10000)
				if(o1 == o2)
					return true;
			}
			return false;
		}
	}
}
include "./StringTableDef.as"