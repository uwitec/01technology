package org.god.SkinControl
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Rectangle;
	import flash.geom.Point;
	//图片类
	public class SkinImage implements ISkinImage
	{
		private var m_bmData:BitmapData;//图片数据
		private var m_bitmap:Bitmap;//图片影片
		
		//构造函数
		public function SkinImage(c:Class = null,width:int=0, height:int=0)
		{
			Load(c,width,height);
		}
		//装载图片
		public function Load(c:Class,width:int, height:int):Boolean
		{
			if(c != null)
			{
				m_bmData = new c(width,height);//创建图片数据
				m_bitmap = new Bitmap(m_bmData);//创建图片影片
				return m_bmData ? true : false;
			}
			else
			{
				return false;
			}
		}
		//释放数据
		public function Unload():void
		{
			if(m_bmData)
			{
				m_bitmap = null;
				m_bmData.dispose();
				m_bmData = null;
			}
		}
		//获取图片数据
		public function GetBitmapData():BitmapData
		{
			return m_bmData;
		}
		//获取图片影片
		public function GetBitmap():Bitmap
		{
			return m_bitmap;
		}
		//获取图片高度
		public function GetHeight():Number
		{
			if(m_bmData)
				return m_bmData.height;
			else
				return 0;
		}
		//获取图片宽度
		public function GetWidth():Number
		{
			if(m_bmData)
				return m_bmData.width;
			else
				return 0;
		}
		//绘画图片数据
		public function DrawBitmapData(b:BitmapData,//目标图片数据
							 nDestXPos:Number=0,//目标X位置
							 nDestYPos:Number=0,//目标Y位置
							 nSrcXPos:Number=0,//源X位置
							 nSrcYPos:Number=0,//源Y位置
							 nSrcWidth:Number=-1,//源宽度
							 nSrcHeight:Number=-1):void //源高度
		{
			if(m_bmData && b)
			{
				if(nSrcWidth == -1)
					nSrcWidth = m_bmData.width;
				if(nSrcHeight== -1)
					nSrcHeight= m_bmData.height;
				var r:Rectangle = new Rectangle(nSrcXPos, nSrcYPos, nSrcWidth, nSrcHeight);
				var p:Point = new Point(nDestXPos, nDestYPos);
				b.copyPixels(m_bmData,r, p);//拷贝像素部分数据
				r = null;
				p = null;
			}
		}
		//绘画图片数据
		public function DrawGraphics(g:Graphics,//目标图片数据
					nDestXPos:Number=0,//目标X位置
					nDestYPos:Number=0,//目标Y位置
					nSrcXPos:Number=0,//源X位置
					nSrcYPos:Number=0,//源Y位置
					nSrcWidth:Number=-1,//源宽度
					nSrcHeight:Number=-1,//源高度
					smoothing:Boolean = true):void 	
		{
			if(m_bmData)
			{
				//var m:Matrix = new Matrix();
				//m.translate(nDestXPos, nDestYPos);
				if(nSrcWidth == -1)
					nSrcWidth = m_bmData.width;
				if(nSrcHeight== -1)
					nSrcHeight= m_bmData.height;
					
				g.beginBitmapFill(m_bmData,null,false,smoothing);
				g.endFill();
				//m = null;
			}
		}
	}
}