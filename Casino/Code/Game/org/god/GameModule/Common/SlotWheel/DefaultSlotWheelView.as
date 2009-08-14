package org.god.GameModule.Common.SlotWheel
{
	public class DefaultSlotWheelView extends BaseSlotWheelView
	{
		public function DefaultSlotWheelView(res:SlotWheelResource,
									  config:BaseSlotWheelConfig)
		{
			super(res,config);
		}
		//////////
		//调整位置
		public override function RectifyWeaveSlotItem():void
		{
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c ++)
				{
					m_BaseWeaveSlotItems[r][c].MoveWindow(c * (m_SlotWheelConfig.m_SlotItemSizeW + 10),
														  r * m_SlotWheelConfig.m_SlotItemSizeH);
					if(m_BaseWeaveSlotItemClipMasks[r][c])
						m_BaseWeaveSlotItemClipMasks[r][c].MoveWindow(c * (m_SlotWheelConfig.m_SlotItemSizeW + 10),
														  r * m_SlotWheelConfig.m_SlotItemSizeH);
				}
			}
		}
		public override function CreateWeaveSlotItem():BaseWeaveSlotItem
		{
			return new DefaultWeaveSlotItem(this);
		}
	}
}