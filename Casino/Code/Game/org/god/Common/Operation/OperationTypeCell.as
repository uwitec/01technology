package org.god.Common.Operation
{
	//操作类型表单元
	public class OperationTypeCell
	{
		protected var m_nOPerationType:uint;
		
		public function OperationTypeCell(nOPerationType:uint)
		{
			m_nOPerationType = nOPerationType;
		}
		
		public function getOperationType():uint
		{
			return m_nOPerationType;
		}
		public function toString():String
		{
			return OperationUtil.getOPTypeName(m_nOPerationType);
		}
	}
}
