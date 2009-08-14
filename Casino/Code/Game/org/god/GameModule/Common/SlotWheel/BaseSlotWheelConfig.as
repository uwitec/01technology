package org.god.GameModule.Common.SlotWheel
{
	import flash.geom.Rectangle;
	
	public class BaseSlotWheelConfig
	{
		//转动盘变量
		public var m_cbSlotWheelRow:int;//转动盘数目
		public var m_cbSlotWheelCol:int;//转动盘数目
		public var m_cbSlotItemCount:int;//转动单元数目
		public var m_cbSlotItemType:Array;//转动单元类型数组
		public var m_cbWeaveSlotItemPoolSize:int;//转动单元缓冲数目
		//转动变量
		public var m_fSpeedupDuration:Number;//加速时间长
		public var m_fSpeeddownDuration:Number;//加速时间长
		public var m_fScrollMaxValue:Number;//转动最大加速值
		public var m_fScrollAdjustValue:Number;//转动调整值

		//位置大小变量
		public var m_ViewBasicStationX:int;//转动视图基准X位置
		public var m_ViewBasicStationY:int;//转动视图基准Y位置
		public var m_ViewBasicSizeW:int;//转动视图W宽度
		public var m_ViewBasicSizeH:int;//转动视图H宽度
		
		public var m_rectWeaveSlotItemClipMask:Rectangle;//转动盘遮蒙显示大小
		
		public var m_SlotItemSizeW:int;//转动单元大小W
		public var m_SlotItemSizeH:int;//转动单元大小H
		public var m_SlotItemBitmapX:int;//转动单元位图X位置
		public var m_SlotItemBitmapY:int;//转动单元位图X位置
		
		public function BaseSlotWheelConfig()
		{
			Init();
		}
		public function Init():void
		{
			m_cbSlotWheelRow = 1;
			m_cbSlotWheelCol = 1;
			m_cbSlotItemCount= 0;
			m_cbSlotItemType = new Array(m_cbSlotItemCount);
			m_cbWeaveSlotItemPoolSize = 0;
			m_fSpeedupDuration	= 1;
			m_fSpeeddownDuration= 1;
			m_fScrollMaxValue	= 100;
			m_fScrollAdjustValue= 1;
			
			m_rectWeaveSlotItemClipMask = new Rectangle;
			m_ViewBasicStationX = 0;
			m_ViewBasicStationY = 0;
			m_ViewBasicSizeW	= 0;
			m_ViewBasicSizeH	= 0;
			m_SlotItemSizeW		= 0;
			m_SlotItemSizeH		= 0;
			m_SlotItemBitmapX	= 0;
			m_SlotItemBitmapY	= 0;
		}
		public function Destroy():void
		{
			m_rectWeaveSlotItemClipMask = null;
		}
		public function GetSlotTypeByIndex(i:int):uint
		{
			return m_cbSlotItemType[i];
		}
		public function GetIndexBySlotType(t:uint):int
		{
			return m_cbSlotItemType.indexOf(t);
		}
	}
}