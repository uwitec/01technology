package org.god.GameModule.Common.SlotWheel
{
	public class DefaultWeaveSlotItem extends BaseWeaveSlotItem
	{
		protected var m_nSlotItemBeginYPos:int;
		protected var m_nSlotItemEndYPos:int;
		protected var m_nSlotItemMidYPos:int;
		protected var m_nSlotItemHalfH:int;
		
		public function DefaultWeaveSlotItem(view:BaseSlotWheelView)
		{
			super(view);
		}
		public override function Init():void
		{
			m_nSlotItemBeginYPos = m_BaseSlotWheelConfig.m_rectWeaveSlotItemClipMask.height/2
									- (m_BaseSlotWheelConfig.m_cbWeaveSlotItemPoolSize/2 *
									 m_BaseSlotWheelConfig.m_SlotItemSizeH);
			m_nSlotItemEndYPos = m_nSlotItemBeginYPos + 
									(m_BaseSlotWheelConfig.m_cbWeaveSlotItemPoolSize *
									 m_BaseSlotWheelConfig.m_SlotItemSizeH);
			m_nSlotItemMidYPos = (m_nSlotItemBeginYPos + m_nSlotItemEndYPos)/2;
			m_nSlotItemHalfH   = m_BaseSlotWheelConfig.m_SlotItemSizeH/2;
			super.Init();
		}
		/////
		protected override function GetLastScrollDeltaVaule():*
		{
			for(var i:uint = 0; i < m_SingleSlotItems.length; i ++)
			{
				if(m_SingleSlotItems[i] != null)
				{
					return m_SingleSlotItems[i].GetScrollDeltaValue() - m_BaseSlotWheelConfig.m_SlotItemSizeH;
				}
			}
			
			return m_nSlotItemEndYPos - m_BaseSlotWheelConfig.m_SlotItemSizeH;
		}
		protected override function AdjustSlotDeltaVaule(lastItem:SingleSlotItem,
														 p:Number = 0):Number
		{
			if((lastItem.y + m_nSlotItemHalfH) < m_nSlotItemMidYPos)
			{
				var nDistance:int  = m_nSlotItemMidYPos - (lastItem.y + m_nSlotItemHalfH);
				if(Math.abs(nDistance) > Math.abs(p))
				{
					p = p;
				}
				else
				{
					if(Math.abs(nDistance) > m_BaseSlotWheelConfig.m_fScrollAdjustValue)
					{
						p = m_BaseSlotWheelConfig.m_fScrollAdjustValue;
					}
					else
					{
						p = nDistance;
					}
				}
			}
			else if((lastItem.y + m_nSlotItemHalfH) == m_nSlotItemMidYPos)
				p = 0;
			else
			{
				var nDistance:int  = (lastItem.y + m_nSlotItemHalfH) - m_nSlotItemMidYPos;
				if(Math.abs(nDistance) > Math.abs(p))
				{
					p = p;
				}
				else
				{
					if(Math.abs(nDistance) > m_BaseSlotWheelConfig.m_fScrollAdjustValue)
					{
						p = m_BaseSlotWheelConfig.m_fScrollAdjustValue;
					}
					else
					{
						p = nDistance;
					}
				}
				
				p = -p;
			}
			
			return p;
		}
		protected override function IsBestSlotDeltaVaule(item:SingleSlotItem):Boolean
		{
			if(int(item.y + m_nSlotItemHalfH) == m_nSlotItemMidYPos)
				return true;
			else
				return false;
		}
		protected override function UpdateSingleSlotItem(item:SingleSlotItem,deltaValue:*):void
		{
			super.UpdateSingleSlotItem(item,deltaValue);
			item.y = int(item.GetScrollDeltaValue());
		}
		protected override function CanRemoveSlotItem(item:SingleSlotItem):Boolean
		{
			if(item.y > m_nSlotItemEndYPos)
				return true;
			else
				return false;
		}
		
	}
}