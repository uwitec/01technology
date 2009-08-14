package org.god.Common.BetHistory.GameModule.B_Sparrow
{
	import org.god.SkinControl.SkinImage;
	
	//方向枚举
	public class SparrowResource
	{
		//用户麻将
		public var m_ImageUserTop:SkinImage;//图片资源
		public var m_ImageUserLeft:SkinImage;//图片资源
		public var m_ImageUserRight:SkinImage;//图片资源
		public var m_ImageUserBottom:SparrowListImage;//图片资源

		//桌子麻将
		public var m_ImageTableTop:SparrowListImage;//图片资源
		public var m_ImageTableLeft:SparrowListImage;//图片资源
		public var m_ImageTableRight:SparrowListImage;//图片资源
		public var m_ImageTableBottom:SparrowListImage;//图片资源

		//牌堆麻将
		public var m_ImageBackH:SkinImage;//图片资源
		public var m_ImageBackV:SkinImage;//图片资源
		public var m_ImageHeapSingleV:SkinImage;//图片资源
		public var m_ImageHeapSingleH:SkinImage;//图片资源
		public var m_ImageHeapDoubleV:SkinImage;//图片资源
		public var m_ImageHeapDoubleH:SkinImage;//图片资源

		public function SparrowResource()
		{
		}
		//功能函数
		//加载资源
		public function NewResource():Boolean
		{
			if (m_ImageUserTop)
			{
				return true;
			}
			//用户麻将
			m_ImageUserTop = new SkinImage;
			m_ImageUserLeft = new SkinImage;
			m_ImageUserRight = new SkinImage;
			m_ImageUserBottom = new SparrowListImage;

			//桌子麻将
			m_ImageTableTop = new SparrowListImage;
			m_ImageTableLeft = new SparrowListImage;
			m_ImageTableRight = new SparrowListImage;
			m_ImageTableBottom = new SparrowListImage;

			//牌堆麻将
			m_ImageBackH = new SkinImage;
			m_ImageBackV = new SkinImage;
			m_ImageHeapSingleV = new SkinImage;
			m_ImageHeapSingleH = new SkinImage;
			m_ImageHeapDoubleV = new SkinImage;
			m_ImageHeapDoubleH = new SkinImage;
			
			return true;
		}
		//消耗资源
		public function UnloadResource():Boolean
		{
			if (m_ImageUserTop == null)
			{
				return true;
			}

			//用户麻将
			m_ImageUserTop.Unload();
			m_ImageUserLeft.Unload();
			m_ImageUserRight.Unload();
			m_ImageUserBottom.UnloadResource();

			//桌子麻将
			m_ImageTableTop.UnloadResource();
			m_ImageTableLeft.UnloadResource();
			m_ImageTableRight.UnloadResource();
			m_ImageTableBottom.UnloadResource();

			//牌堆麻将
			m_ImageBackH.Unload();
			m_ImageBackV.Unload();
			m_ImageHeapSingleV.Unload();
			m_ImageHeapSingleH.Unload();
			m_ImageHeapDoubleV.Unload();
			m_ImageHeapDoubleH.Unload();
			
			//用户麻将
			m_ImageUserTop = null;
			m_ImageUserLeft = null;
			m_ImageUserRight = null;
			m_ImageUserBottom = null;

			//桌子麻将
			m_ImageTableTop = null;
			m_ImageTableLeft = null;
			m_ImageTableRight = null;
			m_ImageTableBottom = null;

			//牌堆麻将
			m_ImageBackH = null;
			m_ImageBackV = null;
			m_ImageHeapSingleV = null;
			m_ImageHeapSingleH = null;
			m_ImageHeapDoubleV = null;
			m_ImageHeapDoubleH = null;
			
			return true;
		}
	}
}