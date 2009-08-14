package org.god.Common
{
	//排序单元
	public class SortTypeCell
	{
		protected var m_cbSortType:uint;//排序类型
		//构造函数
		public function SortTypeCell(cbSortType:uint)
		{
			m_cbSortType = cbSortType;
		}
		//获取排序类型
		public function getSortType():uint
		{
			return m_cbSortType;
		}
		//转换文本
		public function toString():String
		{
			return SortTypeUtil.GetSortTypeName(m_cbSortType);
		}
	}
}