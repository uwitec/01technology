package org.god.GameModule.Common.SlotWheel
{
	import flash.display.BitmapData;
	
	import org.god.SkinControl.SkinImage;
	
	public class SlotWheelResource
	{
		protected var m_BaseSlotWheelConfig:BaseSlotWheelConfig;
		protected var m_SlotItemImage:SkinImage;
		protected var m_dwSingleSlotItemImageWidth:uint;
		protected var m_dwSingleSlotItemImageHeight:uint;
		
		public function SlotWheelResource(classSlotItemImage:Class,
										  dwSlotItemImageWidth:uint,
										  dwSlotItemImageHeight:uint,
										  config:BaseSlotWheelConfig)
		{
			m_BaseSlotWheelConfig = config;
			m_SlotItemImage = new SkinImage(classSlotItemImage,dwSlotItemImageWidth,dwSlotItemImageHeight);
			m_dwSingleSlotItemImageWidth = m_BaseSlotWheelConfig.m_SlotItemSizeW;
			m_dwSingleSlotItemImageHeight= m_BaseSlotWheelConfig.m_SlotItemSizeH;
		}
		public function Destroy():void
		{
			m_SlotItemImage.Unload();
			m_SlotItemImage = null;
			m_BaseSlotWheelConfig = null;
		}
		public function CreateRandomSingleSlotItem(deltaValue:*):SingleSlotItem
		{
			var i:uint = uint(Math.random() * 10000) % m_BaseSlotWheelConfig.m_cbSlotItemCount;
			var bmd:BitmapData = new BitmapData(m_dwSingleSlotItemImageWidth,
												m_dwSingleSlotItemImageHeight);
			m_SlotItemImage.DrawBitmapData(bmd,0,0,
										   i*m_dwSingleSlotItemImageWidth,
										   0,
										   m_dwSingleSlotItemImageWidth,
										   m_dwSingleSlotItemImageHeight);
			var type:uint = m_BaseSlotWheelConfig.GetIndexBySlotType(i);
			
			return new SingleSlotItem(type,deltaValue,
									  bmd,
									  m_BaseSlotWheelConfig.m_SlotItemBitmapX,
									  m_BaseSlotWheelConfig.m_SlotItemBitmapY);
		}
		public function CreateFixedSingleSlotItem(type:int,deltaValue:*):SingleSlotItem
		{
			var i:uint = m_BaseSlotWheelConfig.GetIndexBySlotType(type);
			var bmd:BitmapData = new BitmapData(m_dwSingleSlotItemImageWidth,
												m_dwSingleSlotItemImageHeight);
			m_SlotItemImage.DrawBitmapData(bmd,0,0,
										   i*m_dwSingleSlotItemImageWidth,
										   0,
										   m_dwSingleSlotItemImageWidth,
										   m_dwSingleSlotItemImageHeight);

			return new SingleSlotItem(type,deltaValue,
									  bmd,
									  m_BaseSlotWheelConfig.m_SlotItemBitmapX,
									  m_BaseSlotWheelConfig.m_SlotItemBitmapY);
		}
	}
}