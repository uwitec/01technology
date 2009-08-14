package org.god.GameModule.Common.SlotWheel
{
	import flash.display.MovieClip;
	
	public class BaseWeaveSlotItem extends MovieClip
	{
		protected static const CompareAdjustSlotDeltaVauleResult_LT:int = 0;
		protected static const CompareAdjustSlotDeltaVauleResult_EQ:int = 1;
		protected static const CompareAdjustSlotDeltaVauleResult_GT:int = 2;
		
		protected var m_BaseSlotWheelView:BaseSlotWheelView;
		protected var m_SlotWheelResource:SlotWheelResource;
		protected var m_BaseSlotWheelConfig:BaseSlotWheelConfig;
		
		protected var m_SingleSlotItems:Array;
		protected var m_SlotWheelScrollMotion:SlotWheelScrollMotion;
		protected var m_cbRequestLastSlotType:uint;
		protected var m_bAddLastSlotItem:Boolean;
		
		public function BaseWeaveSlotItem(view:BaseSlotWheelView)
		{
			super();
			m_BaseSlotWheelView 	= view;
			m_SlotWheelResource		= m_BaseSlotWheelView.GetResource();
			m_BaseSlotWheelConfig 	= m_BaseSlotWheelView.GetConfig();
			
			Init();
		}
		public function Init():void
		{
			m_SingleSlotItems = new Array(m_BaseSlotWheelConfig.m_cbWeaveSlotItemPoolSize);
			for(var i:int = m_SingleSlotItems.length-1; i >= 0 ; i --)
			{
				m_SingleSlotItems[i] = m_SlotWheelResource.CreateRandomSingleSlotItem(GetLastScrollDeltaVaule());
				addChild(m_SingleSlotItems[i]);
				UpdateSingleSlotItem(m_SingleSlotItems[i],0);
			}
			
			m_SlotWheelScrollMotion = new SlotWheelScrollMotion(this,
																m_BaseSlotWheelConfig.m_fSpeedupDuration,
																m_BaseSlotWheelConfig.m_fSpeeddownDuration,
																m_BaseSlotWheelConfig.m_fScrollMaxValue,
																m_BaseSlotWheelConfig.m_fScrollAdjustValue);
		}
		public function Destroy():void
		{
			if(m_SlotWheelScrollMotion)
			{
				m_SlotWheelScrollMotion.Destroy();
				m_SlotWheelScrollMotion = null;
			}
			for(var i:uint = 0; i < m_SingleSlotItems.length; i ++)
			{
				m_SingleSlotItems[i].Destroy();
				removeChild(m_SingleSlotItems[i]);
				m_SingleSlotItems[i] = null;
			}
			this.mask			  = null;
			m_SingleSlotItems 	  = null;
			m_SlotWheelResource	  = null;
			m_BaseSlotWheelConfig = null;
			m_BaseSlotWheelView = null;
		}
		public function MoveWindow(xp:int,yp:int):void
		{
			this.x = xp;
			this.y = yp;
		}

		public function IsScrolling():Boolean
		{
			return m_SlotWheelScrollMotion.IsScrolling();
		}
		public function IsAdjusting():Boolean
		{
			return m_cbRequestLastSlotType == 0 ? false : true;
		}
		public function Start():void
		{
			if(IsScrolling() == false)
			{
				m_bAddLastSlotItem		= false;
				m_cbRequestLastSlotType = 0;
				for(var i:uint = 0; i < m_SingleSlotItems.length; i ++)
				{
					m_SingleSlotItems[i].SetLastRequstSlotItemType(false);
				}
				m_SlotWheelScrollMotion.SpeedUp();
			}
		}
		public function Stop(lastSlotType:uint):void
		{
			if(IsScrolling())
			{
				m_cbRequestLastSlotType = lastSlotType;
				m_SlotWheelScrollMotion.SpeedDown();
			}
		}
		/////
		protected function GetLastScrollDeltaVaule():*
		{
			return 0;
		}
		protected function UpdateSingleSlotItem(item:SingleSlotItem,deltaValue:*):void
		{
			item.SetScrollDeltaValue(item.GetScrollDeltaValue() + deltaValue);
		}
		protected function CanRemoveSlotItem(item:SingleSlotItem):Boolean
		{
			return false;
		}
		protected function AdjustSlotDeltaVaule(lastItem:SingleSlotItem,p:Number = 0):Number
		{
			return p;
		}
		protected function IsBestSlotDeltaVaule(item:SingleSlotItem):Boolean
		{
			return true;
		}

		public function OnSlotWheelScrollMotionSpeedDown():void
		{
			if(m_bAddLastSlotItem == true)
				return;
			//trace("Requst Last Slot " + m_cbRequestLastSlotType);
			var i:int = m_SingleSlotItems.length - 1;
			
			m_SingleSlotItems[i].Destroy();
			removeChild(m_SingleSlotItems[i]);
			m_SingleSlotItems[i] = null;
					
			CompactSlotItemElement(0,i);
			
			m_SingleSlotItems[0] = m_SlotWheelResource.CreateFixedSingleSlotItem(m_cbRequestLastSlotType,
																				 GetLastScrollDeltaVaule());
			addChild(m_SingleSlotItems[0]);
			m_SingleSlotItems[0].SetLastRequstSlotItemType(true);
			UpdateSingleSlotItem(m_SingleSlotItems[0],0);
			
			m_bAddLastSlotItem = true;
		}
		public function OnSlotWheelScrollMotionLoop(p:Number):Boolean
		{
			var AddLastSlotItem:SingleSlotItem = null;
			if(m_bAddLastSlotItem)
			{
				for(var i:int = 0; i < m_SingleSlotItems.length; i ++)
				{
					if(m_SingleSlotItems[i].IsLastRequstSlotItemType())
					{
						p = AdjustSlotDeltaVaule(m_SingleSlotItems[i],p);
						
						AddLastSlotItem = m_SingleSlotItems[i];
						break;
					}
				}
			}
			if(p != 0)
			{
				for(var i:int = 0; i < m_SingleSlotItems.length; i ++)
				{
					UpdateSingleSlotItem(m_SingleSlotItems[i],p);
				}
			
				for(i= 0; i < m_SingleSlotItems.length; i ++)
				{
					if(CanRemoveSlotItem(m_SingleSlotItems[i]))
					{
						m_SingleSlotItems[i].Destroy();
						removeChild(m_SingleSlotItems[i]);
						m_SingleSlotItems[i] = null;
					
						CompactSlotItemElement(0,i);
					}
				}
			
				for(i= m_SingleSlotItems.length-1; i >=0; i --)
				{
					if(m_SingleSlotItems[i] == null)
					{
						m_SingleSlotItems[i] = m_SlotWheelResource.CreateRandomSingleSlotItem(GetLastScrollDeltaVaule());
						addChild(m_SingleSlotItems[i]);
						UpdateSingleSlotItem(m_SingleSlotItems[i],0);
					}
				}
			}
			
			if(AddLastSlotItem == null)
				return false;
			else
			{
				return IsBestSlotDeltaVaule(AddLastSlotItem);
			}
		}
		public function OnSlotWheelScrollMotionStart():void
		{
			if(m_BaseSlotWheelView)
				m_BaseSlotWheelView.OnSlotWheelScrollMotionStart(this);
		}
		public function OnSlotWheelScrollMotionFinish():void
		{
			if(m_BaseSlotWheelView)
				m_BaseSlotWheelView.OnSlotWheelScrollMotionFinish(this);
		}
		////
		protected function CompactSlotItemElement(beginIndex:int,endIndex:int):void
		{
			for(var i:int = endIndex; i > beginIndex; i --)
			{
				m_SingleSlotItems[i] = m_SingleSlotItems[i-1];
			}
			m_SingleSlotItems[beginIndex] = null;
		}
	}
}