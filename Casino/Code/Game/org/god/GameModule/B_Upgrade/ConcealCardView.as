package 
{
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.SkinControl.SkinImage;
	
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	
	public class ConcealCardView extends Sprite
	{
		//数值掩码
		public static  var CARD_MASK_COLOR:uint=0xF0;//花色掩码
		public static  var CARD_MASK_VALUE:uint=0x0F;//数值掩码

		//结束信息
		protected var m_cbConcealCount:uint;//暗藏数目
		protected var m_cbConcealCard:Array=new Array(8);//暗藏扑克

		//资源变量
		protected var m_CardSize:Point=new Point  ;//扑克大小
		protected var m_ImageBack:SkinImage;//背景图案
		protected var m_ImageCard:SkinImage;//扑克位图
		
		protected var m_nWidth:Number;
		protected var m_nHeight:Number;

		public function ConcealCardView()
		{
			//设置数据
			m_cbConcealCount=0;
			ZeroArray(m_cbConcealCard);

			//加载资源
			m_ImageCard = new SkinImage(IMAGE_CARD);
			m_ImageBack = new SkinImage(IMAGE_CONCEAL_CARD_VIEW);

			//获取大小
			m_CardSize.x=m_ImageCard.GetWidth()/13;
			m_CardSize.y=m_ImageCard.GetHeight()/5;
			
			m_nWidth  = m_ImageBack.GetWidth();
			m_nHeight = m_ImageBack.GetHeight();
		}
		public function IsWindowVisible():Boolean
		{
			return this.visible;
		}
		public function ShowWindow(bShow:Boolean):void
		{
			this.visible = bShow;
		}
		public function MoveWindow(nXPos:Number, nYPos:Number):void
		{
			this.x = nXPos;
			this.y = nYPos;
		}
		//重画函数
		public function OnPaint(pDC:BitmapData, rcClient:Rectangle):void
		{

			//绘画背景
			m_ImageBack.DrawBitmapData(pDC,0,0);
		
			//绘画扑克
			if (m_cbConcealCount>0)
			{
				//计算大小
				var ControlSize:Point = new Point;
				ControlSize.y=m_CardSize.y;
				ControlSize.x=m_CardSize.x+(m_cbConcealCount-1)*18;
		
				//定义位置
				var nXPos:Number=(rcClient.width-ControlSize.x)/2+17;
				var nYPos:Number=(rcClient.height-ControlSize.y)/2;
		
				//绘画扑克
				for (var i:uint=0;i<m_cbConcealCount;i++)
				{
					//变量定义
					var nXImagePos:Number=0;
					var nYImagePos:Number=0;
		
					//图片位置
					if (m_cbConcealCard[i]!=0)
					{
						if ((m_cbConcealCard[i]==0x4E)||(m_cbConcealCard[i]==0x4F))
						{
							nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.y;
							nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)%14)*m_CardSize.x;
						}
						else
						{
							nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)-1)*m_CardSize.x;
							nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.y;
						}
					}
					else
					{
						nXImagePos=m_CardSize.x*2;
						nYImagePos=m_CardSize.y*4;
					}
		
					//绘画扑克
					m_ImageCard.DrawBitmapData(pDC,
					nXPos+i*18,nYPos,
					nXImagePos,nYImagePos,
					m_CardSize.x,m_CardSize.y);
				}
			}
		
			return;
		}
		
		//设置信息
	 	public function SetConcealCard(cbConcealCard:Array, cbConcealCount:uint):void
		{
			//设置变量
			m_cbConcealCount=cbConcealCount;
			CopyArray(m_cbConcealCard,cbConcealCard,cbConcealCount);
		
			//刷新界面
			Invalidate();
		
			return;
		}	
		private function Invalidate():void
		{
			if(this.graphics)
			{
				this.graphics.clear();
				if(m_nWidth == 0 || m_nHeight == 0)
					return;
				var g:Graphics2D = new Graphics2D(this.graphics);
				
				var rect:Rectangle = new Rectangle(0, 0, m_nWidth, m_nHeight);
				var pDC:BitmapData = new BitmapData(m_nWidth, m_nHeight, true, 0x00ffffff);
				
				OnPaint(pDC,rect);
				
 				var bBrush:BitmapBrush = new BitmapBrush(pDC, null, false, true);	
 				g.fillRectangle(bBrush,0,0,
								pDC.width,
								pDC.height);
				
				
				bBrush = null;
				g 	  = null;
				pDC    = null;
			}
		}
	}
}
include "../../Common/Memory.as"