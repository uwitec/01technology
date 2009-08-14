package org.god.GameModule.Common.SlotWheel
{
	import flash.geom.Rectangle;
	
	public class DefaultSlotWheelConfig extends BaseSlotWheelConfig
	{
		public function DefaultSlotWheelConfig()
		{
			super();
		}
		public override function Init():void
		{
			super.Init();
			m_cbSlotWheelRow = 1;
			m_cbSlotWheelCol = 3;
			m_cbSlotItemCount= 8;
			m_cbSlotItemType = new Array(m_cbSlotItemCount);
			for(var i:int = 0; i < m_cbSlotItemType.length; i ++)
			{
				m_cbSlotItemType[i] = i + 1;
			}
			m_cbWeaveSlotItemPoolSize = 5;
			m_fSpeedupDuration	= 1;
			m_fSpeeddownDuration= 1;
			m_fScrollMaxValue	= 30;
			m_fScrollAdjustValue= 5;
			
			m_rectWeaveSlotItemClipMask = new Rectangle(0,0,80,140);
			m_ViewBasicStationX = 360;
			m_ViewBasicStationY = 291;
			m_ViewBasicSizeW	= 80;
			m_ViewBasicSizeH	= 138;
			m_SlotItemSizeW		= 80;
			m_SlotItemSizeH		= 79;
			m_SlotItemBitmapX	= 0;
			m_SlotItemBitmapY	= 0;
		}
	}
}