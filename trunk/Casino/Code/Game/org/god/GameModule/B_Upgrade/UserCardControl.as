package 
{
	import flash.display.Sprite;
	import flash.geom.Point;
	import flash.events.MouseEvent;
	import flash.display.BitmapData;

	import org.god.SkinControl.SkinImage;

	import org.god.GameModule.enXCollocateMode;
	import org.god.GameModule.enYCollocateMode;
	
	import org.aswing.graphics.*;
	import org.aswing.ASColor;

	public class UserCardControl extends Sprite
	{
		//数值掩码
		public static  var CARD_MASK_COLOR:uint=0xF0;//花色掩码
		public static  var CARD_MASK_VALUE:uint=0x0F;//数值掩码
		//间距定义
		public static  var DEF_CARD_SHOOT:uint=20;//默认间距
		public static  var DEF_CARD_V_SPACE:uint=19;//默认间距
		public static  var DEF_CARD_H_SPACE:uint=17;//默认间距

		//扑克定义
		//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,//方块 A - K
		//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,//梅花 A - K
		//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,//红桃 A - K
		//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,//黑桃 A - K

		//配置变量
		protected var m_bDisplay:Boolean;//显示标志
		protected var m_bHorizontal:Boolean;//横放标志
		
		//间隔变量
		protected var m_dwCardHSpace:uint;//横向间隔
		protected var m_dwCardVSpace:uint;//竖向间隔

		//位置变量
		protected var m_BenchmarkPos:Point=new Point  ;//基准位置
		protected var m_XCollocateMode:uint;//显示模式
		protected var m_YCollocateMode:uint;//显示模式

		//内部数据
		protected var m_CardDataItem:Array;//扑克数据

		//资源变量
		protected var m_CardSize:Point=new Point  ;//扑克大小
		protected var m_ImageCard:SkinImage;//图片资源
		
		protected var m_nWidth:Number = 0;
		protected var m_nHeight:Number= 0;

		public function UserCardControl()
		{
			//配置变量
			m_bDisplay=false;
			m_bHorizontal=true;


			//间隔变量
			m_dwCardHSpace=DEF_CARD_H_SPACE;
			m_dwCardVSpace=DEF_CARD_V_SPACE;
			m_dwShootAltitude=DEF_CARD_SHOOT;

			//位置变量
			m_XCollocateMode=enXCollocateMode.enXCenter;
			m_YCollocateMode=enYCollocateMode.enYCenter;

			//运行变量
			m_dwCurrentIndex=0xFFFFFFFF;

			//资源变量
			if (m_ImageCard == null)
			{
				//加载资源
				m_ImageCard=new SkinImage(IMAGE_CARD);

				//获取大小
				m_CardSize.cx=m_ImageCard.GetWidth() / 13;
				m_CardSize.cy=m_ImageCard.GetHeight() / 5;
			}
			
		}
				
		//设置方向
		public function SetDirection( bHorizontal:Boolean):void
		{
			//状态判断
			if (m_bHorizontal==bHorizontal) return;
		
			//设置变量
			m_bHorizontal=bHorizontal;
		
			//调整控件
			RectifyControl();
		
			return;
		}
		
		
		
		//设置显示
		public function SetDisplayFlag(bDisplay:Boolean):void
		{
			//状态判断
			if (m_bDisplay==bDisplay) return;
		
			//设置变量
			m_bDisplay=bDisplay;
		
			//重画控件
			Invalidate();
		
			return;
		}

		
		//设置间距
		public function SetCardSpace( dwCardHSpace:uint,  dwCardVSpace:uint,  dwShootAltitude:uint):void
		{
			//设置变量
			m_dwCardHSpace=dwCardHSpace;
			m_dwCardVSpace=dwCardVSpace;
			m_dwShootAltitude=dwShootAltitude;
		
			//调整控件
			RectifyControl();
		
			return;
		}
		
		//基准位置
		public function SetBenchmarkPos(nXPos:Number, 
		nYPos:Number, 
		XCollocateMode:uint,  YCollocateMode:uint):void
		{
			//设置变量
			m_BenchmarkPos.x=nXPos;
			m_BenchmarkPos.y=nYPos;
			m_XCollocateMode=XCollocateMode;
			m_YCollocateMode=YCollocateMode;
		
			//调整控件
			RectifyControl();
		
			return;
		}
		
		
		//获取数目
		 public function GetCardCount():uint
		{
			return m_CardDataItem.length;
		}
		
		
		//设置扑克
		public function SetCardData(cbCardData:Array, dwCardCount:uint):uint
		{
			m_CardDataItem.length = dwCardCount;
			for (var i:uint=0;i<dwCardCount;i++) m_CardDataItem[i]=cbCardData[i];

			//调整控件
			RectifyControl();
		
			return dwCardCount;
		}
		
		
		//调整位置
		public function RectifyControl():void
		{
			//变量定义
			var dwCardCount:uint=m_CardDataItem.length;
		
			//计算大小
			var ControlSize:Point = new Point;
			if (m_bHorizontal==true)
			{
				ControlSize.y=m_CardSize.y+m_dwShootAltitude;
				ControlSize.x=(dwCardCount>0)?(m_CardSize.x+(dwCardCount-1)*m_dwCardHSpace):0;
			}
			else
			{
				ControlSize.x=m_CardSize.x;
				ControlSize.y=(dwCardCount>0)?(m_CardSize.y+(dwCardCount-1)*m_dwCardVSpace):0;
			}
		
			//横向位置
			var nXPos:Number=0;
			switch (m_XCollocateMode)
			{
			case enXCollocateMode.enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
			case enXCollocateMode.enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.x/2; break; }
			case enXCollocateMode.enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.x; break; }
			}
		
			//竖向位置
			var nYPos:Number=0;
			switch (m_YCollocateMode)
			{
			case enYCollocateMode.enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
			case enYCollocateMode.enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.y/2; break; }
			case enYCollocateMode.enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.y; break; }
			}
		
			//移动位置
			this.x = nXPos;
			this.y = nYPos;
			m_nWidth = ControlSize.x;
			m_nHeight= ControlSize.y;
		
		
			//重画界面
			Invalidate();
		
			return;
		}
		
		//重画函数
		public function OnPaint(pDC:BitmapData,rcClient:Rectangle):void
		{
			//绘画扑克
			var dwXImagePos:Number;
			var dwYImagePos:Number;
			var dwXScreenPos:Number;
			var dwYScreenPos:Number;
			
			for (var i:uint=0;i<m_CardDataItem.length;i++)
			{
				//获取扑克
				var cbCardData:uint=m_CardDataItem[i];

				//图片位置
				if ((m_bDisplay==true)&&(cbCardData!=0))
				{
					dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.x;
					dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.y;
				}
				else
				{
					dwXImagePos=m_CardSize.x*2;
					dwYImagePos=m_CardSize.y*4;
				}

				//屏幕位置
				if (m_bHorizontal==true) 
				{
					dwYScreenPos=0;
					dwXScreenPos=m_dwCardHSpace*i;
				}
				else
				{
					dwXScreenPos=0;
					dwYScreenPos=m_dwCardVSpace*i;
				}

				//绘画扑克
				m_ImageCard.AlphaDrawImage(pDC,dwXScreenPos,dwYScreenPos,
										   dwXImagePos,dwYImagePos,
										   m_CardSize.x,m_CardSize.y
										   );
			}
		
		
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
include "../../Common/Memory.as";