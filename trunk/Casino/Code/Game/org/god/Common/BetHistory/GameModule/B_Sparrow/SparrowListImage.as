package org.god.Common.BetHistory.GameModule.B_Sparrow
{
	import flash.display.BitmapData;
	
	import org.god.SkinControl.SkinImage;
	
	//麻将图片
	public class SparrowListImage
	{
		//逻辑掩码
		protected static const MASK_COLOR:uint=0xF0;//花色掩码
		protected static const MASK_VALUE:uint=0x0F;//数值掩码
		
		//位置变量
		protected var m_nItemWidth:Number=0;//子项高度
		protected var m_nItemHeight:Number=0;//子项宽度
		protected var m_nViewWidth:Number=0;//子项高度
		protected var m_nViewHeight:Number=0;//子项宽度

		//资源变量
		protected var m_SparrowListImage:SkinImage;//图片资源

		public function SparrowListImage()
		{
		}
		//信息函数

		//获取宽度
		public function GetViewWidth():Number
		{
			return m_nViewWidth;
		}
		//获取高度
		public function GetViewHeight():Number
		{
			return m_nViewHeight;
		}
		//资源管理
		//加载资源
		public function LoadResource(uResourceID:Class,
									 nImageWidth:Number,
									 nImageHeight:Number,
									 nViewWidth:Number,
									 nViewHeight:Number,
									 ITEM_COUNT:int = 43):Boolean
		{
			m_SparrowListImage = new SkinImage(uResourceID, nImageWidth, nImageHeight);
			
			//设置变量
			m_nViewWidth=nViewWidth;
			m_nViewHeight=nViewHeight;
			m_nItemHeight=m_SparrowListImage.GetHeight();
			m_nItemWidth=m_SparrowListImage.GetWidth()/ITEM_COUNT;
			return true;
		}
		//释放资源
		public function UnloadResource():Boolean
		{
			//设置变量
			m_nItemWidth=0;
			m_nItemHeight=0;
			if(m_SparrowListImage)
			{
				m_SparrowListImage.Unload();
				m_SparrowListImage = null;
			}
			return true;
		}

		//功能函数

		//获取位置
		public function GetImageIndex(cbSparrowData:uint):uint
		{
			//背景判断
			if (cbSparrowData==0) return 0;

			//计算位置
			var cbValue:uint=cbSparrowData&MASK_VALUE;
			var cbColor:uint=(cbSparrowData&MASK_COLOR)>>4;
			return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
		}
		//绘画麻将
		public function DrawSparrowItem(pDestDC:BitmapData,
									 cbSparrowData:uint,
									 xDest:Number,
									 yDest:Number):Boolean
		{
			if(m_SparrowListImage == null)
				return false;
				
			var nImageXPos:Number=GetImageIndex(cbSparrowData)*m_nItemWidth;
			m_SparrowListImage.DrawBitmapData(pDestDC,
										 xDest,
										 yDest,
										 nImageXPos,
										 0,
										 m_nItemWidth,
										 m_nItemHeight);

			return true;
		}
	}
}
include "../../../../GameModule/B_Sparrow/SparrowDef.as"