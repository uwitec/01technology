package org.god.Common.BetHistory.GameModule.B_SiChuan_Land
{
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.events.MouseEvent;
	
	import org.god.SkinControl.SkinImage;
	import org.god.GameModule.Common.enXCollocateMode;
	import org.god.GameModule.Common.enYCollocateMode;
	
	import org.aswing.graphics.Graphics2D;
	import org.aswing.graphics.BitmapBrush;

	public class CardControl extends Sprite
	{
		//配置变量
		protected var m_bDisplay:Boolean;//显示标志
		protected var m_bHorizontal:Boolean;//横放标志
		protected var m_bPositively:Boolean;//响应标志

		//间隔变量
		protected var m_dwCardHSpace:uint;//横向间隔
		protected var m_dwCardVSpace:uint;//竖向间隔
		protected var m_dwShootAltitude:uint;//弹起高度

		//位置变量
		protected var m_BenchmarkPos:Point=new Point  ;//基准位置
		protected var m_XCollocateMode:uint;//显示模式
		protected var m_YCollocateMode:uint;//显示模式

		//运行变量
		protected var m_dwCurrentIndex:uint;//点击索引

		//内部数据
		protected var m_CardDataItem:Array=new Array  ;//扑克数据

		//资源变量
		protected var m_CardSize:Point = new Point;//扑克大小
		protected var m_ImageCard:SkinImage;//图片资源
		protected var m_nWidth:Number = 0;
		protected var m_nHeight:Number = 0;
		
		public function CardControl(pImageCard:SkinImage)
		{
			for(var i:uint= 0; i < m_CardDataItem.length; i ++)
			{
				m_CardDataItem[i] = new tagCardItem;
			}
			//配置变量
			m_bDisplay=false;
			m_bHorizontal=true;
			m_bPositively=false;
		
			//间隔变量
			m_dwCardHSpace=20;
			m_dwCardVSpace=19;
			m_dwShootAltitude=8;
		
			//位置变量
			m_XCollocateMode=enXCollocateMode.enXCenter;
			m_YCollocateMode=enYCollocateMode.enYCenter;
		
			//运行变量
			m_dwCurrentIndex=0xFFFFFFFF;
			
			m_ImageCard =pImageCard;
			m_CardSize.x=m_ImageCard.GetWidth()/13.0;
			m_CardSize.y=m_ImageCard.GetHeight()/5.0;
			addEventListener(MouseEvent.MOUSE_DOWN, OnEventMouseDown);
			addEventListener(MouseEvent.MOUSE_UP, OnEventMouseUp);
		}
		public function Destroy():void
		{
			m_BenchmarkPos = null;
			ZeroArray(m_CardDataItem,null);
			m_CardDataItem = null;
			m_ImageCard = null;
			removeEventListener(MouseEvent.MOUSE_DOWN, OnEventMouseDown);
			removeEventListener(MouseEvent.MOUSE_UP, OnEventMouseUp);
		}
		//设置方向
		public function SetDirection(bHorizontal:Boolean):void
		{
			//状态判断
			if (m_bHorizontal==bHorizontal) return;
		
			//设置变量
			m_bHorizontal=bHorizontal;
		
			//调整控件
			RectifyControl();
		
			return;
		}
		
		//弹起扑克
		public function ShootAllCard( bShoot:Boolean):void
		{
			//变量定义
			var bRectify:Boolean=false;
			var CardItem:tagCardItem=null;
			var dwCardCount:uint=m_CardDataItem.length;
		
			//设置扑克
			for (var i:uint=0;i<dwCardCount;i++)
			{
				//获取扑克
				var pCardItem:tagCardItem=m_CardDataItem[i];
		
				//扑克分析
				if (pCardItem.bShoot!=bShoot)
				{
					bRectify=true;
					pCardItem.bShoot=bShoot;
				}
			}
		
			//调整控件
			if (bRectify==true) RectifyControl();
		
			return;
		}
		
		//设置显示
		public function SetDisplayFlag( bDisplay:Boolean)
		{
			//状态判断
			if (m_bDisplay==bDisplay) return;
		
			//设置变量
			m_bDisplay=bDisplay;
		
			//重画控件
			Invalidate();
		
			return;
		}
		
		//设置响应
		public function SetPositively( bPositively:Boolean):void
		{
			//设置变量
			m_bPositively=bPositively;
		
			return;
		}
		
		//设置间距
		public function SetCardSpace(dwCardHSpace:uint,  dwCardVSpace:uint,  dwShootAltitude:uint):void
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
		public function SetBenchmarkPos( nXPos:Number,  nYPos:Number,  XCollocateMode:uint,  YCollocateMode:uint):void
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
		
		//获取扑克
		public function GetShootCard(bCardData:Array,  dwMaxCount:uint):uint
		{
			//变量定义
			var bShootCount:uint=0;
			var dwCardCount:uint=m_CardDataItem.length;
		
			//搜索扑克
			var pCardItem:tagCardItem=null;
			for (var i:uint=0;i<dwCardCount;i++)
			{
				//获取扑克
				pCardItem=m_CardDataItem[i];
		
				//扑克分析
				if (pCardItem.bShoot==true) bCardData[bShootCount++]=pCardItem.bCardData;
			}
		
			return bShootCount;
		}
		
		//设置扑克
		public function SetCardData(bCardData:Array, dwCardCount:uint):uint
		{
			//设置扑克
			SetCardDataItemSize(dwCardCount);
			for (var i:uint=0;i<dwCardCount;i++)
			{
				m_CardDataItem[i].bShoot=false;
				m_CardDataItem[i].bCardData=bCardData[i];
			}
		
			//调整控件
			RectifyControl();
		
			return dwCardCount;
		}
		
		//获取扑克
		public function GetCardData(CardItemArray:Array,  dwMaxCount:uint):uint
		{
			//效验参数
			ASSERT(dwMaxCount>=m_CardDataItem.length);
		
			//拷贝扑克
			var dwCardCount:uint=m_CardDataItem.length;
			CopyArray(CardItemArray,m_CardDataItem,dwCardCount);
		
			return dwCardCount;
		}
		
		//设置扑克
		public function SetCardItem(CardItemArray:Array,dwCardCount:uint):uint
		{
			//设置扑克
			SetCardDataItemSize(dwCardCount);
			CopyArray(m_CardDataItem,CardItemArray,dwCardCount);
		
			//调整控件
			RectifyControl();
		 
			return 0;
		}
		//设置弹起扑克
		public function SetShootCard(bCardDataIndex:Array,dwCardCount:uint):uint
		{
			for(var i:uint=0;i<dwCardCount;i++)
			{
				var pCardItem:tagCardItem=m_CardDataItem[bCardDataIndex[i]];
				pCardItem.bShoot=true;
				RectifyControl();
			}
			return 0;
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
			case  enXCollocateMode.enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
			case  enXCollocateMode.enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.x/2; break; }
			case  enXCollocateMode.enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.x; break; }
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
			m_nHeight = ControlSize.y;
			
			//重画界面
			Invalidate();
		
			return;
		}
		private function SetCardDataItemSize(dwCardCount:uint):void
		{
			if (dwCardCount < 0)
			{
				dwCardCount=0;
			}
			if (dwCardCount == m_CardDataItem.length)
			{
				return;
			}
			if (dwCardCount > m_CardDataItem.length)
			{
				for (var i:uint=m_CardDataItem.length; i < dwCardCount; i++)
				{
					m_CardDataItem[i] = new tagCardItem ;
				}
			}
			else
			{
				var delCard:Array=m_CardDataItem.splice(dwCardCount);
				if(delCard != null)
					ZeroArray(delCard, null);
			}
		}
		//索引切换
		public function SwitchCardPoint(MousePoint:Point):uint
		{
			if (m_bHorizontal==true)
			{
				//变量定义
				var dwCardCount:uint=m_CardDataItem.length;
		
				//获取索引
				var dwCardIndex:uint=MousePoint.x/m_dwCardHSpace;
				if (dwCardIndex>=dwCardCount) dwCardIndex=(dwCardCount-1);
		
				//判断按键
				var bCurrentCard:Boolean=true;
				var bCardShoot:Boolean=m_CardDataItem[dwCardIndex].bShoot;
				if ((bCardShoot==true)&&(MousePoint.y>m_CardSize.y)) bCurrentCard=false;
				if ((bCardShoot==false)&&(MousePoint.y<m_dwShootAltitude)) bCurrentCard=false;
		
				var dwFindCardIndex:uint = dwCardIndex;
				//向前寻找
				if (bCurrentCard==false)
				{
					while (dwCardIndex>0)
					{
						dwCardIndex--;
						bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
						if ((bCardShoot==false)&&(MousePoint.y>m_CardSize.x))
						{
							dwFindCardIndex = dwCardIndex;
							break;
						}
						if ((bCardShoot==true)&&(MousePoint.y<m_dwShootAltitude))
						{
							dwFindCardIndex = dwCardIndex;
							break;
						}
					}
				}
		
				return dwFindCardIndex;
			}
		
			return 0xFFFFFFFF;
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
				var pCardItem:tagCardItem=m_CardDataItem[i];
		
				//图片位置
				if ((m_bDisplay==true)&&(pCardItem.bCardData!=0))
				{
					dwXImagePos=Number((pCardItem.bCardData&LOGIC_MASK_VALUE)-1)*m_CardSize.x;
					dwYImagePos=Number((pCardItem.bCardData&LOGIC_MASK_COLOR)>>4)*m_CardSize.y;
					
				}
				else
				{
					dwXImagePos=m_CardSize.x*2;
					dwYImagePos=m_CardSize.y*4;
				}
		
				//屏幕位置
				if (m_bHorizontal==true) 
				{
					dwXScreenPos=m_dwCardHSpace*i;
					dwYScreenPos=(pCardItem.bShoot==false)?m_dwShootAltitude:0;
				}
				else
				{
					dwXScreenPos=0;
					dwYScreenPos=m_dwCardVSpace*i;
				}
		
				//绘画扑克
				m_ImageCard.DrawBitmapData(pDC,
				dwXScreenPos,
				dwYScreenPos,
				dwXImagePos,
				dwYImagePos,
				m_CardSize.x,
				m_CardSize.y);
			}
		
			
		
			return;
		}
		
		
		//鼠标消息
		public function OnEventMouseUp(e:MouseEvent):void
		{
		  var point:Point = new Point(mouseX,mouseY);
			//状态判断
			if (m_dwCurrentIndex==0xFFFFFFFF) return;
			if ((m_bHorizontal==false)||(m_bPositively==false)) return;
		
			//获取索引
			var dwCurrentIndex:uint=SwitchCardPoint(point);
			if (dwCurrentIndex!=m_dwCurrentIndex) return;
		
			//设置控件
			var  pCardItem:tagCardItem=m_CardDataItem[dwCurrentIndex];
			pCardItem.bShoot=!pCardItem.bShoot;
		
			//调整控件
			RectifyControl();
		
			return;
		}
		
		//鼠标消息
		public function OnEventMouseDown(e:MouseEvent):void
		{
			 var point:Point = new Point(mouseX,mouseY);
		
			//状态判断
			if ((m_bHorizontal==false)||(m_bPositively==false)) return;
		
			//获取索引
			m_dwCurrentIndex=SwitchCardPoint(point);
		
			return;
		}
		
		//光标消息
	   public function OnEventMouseMove(e:MouseEvent):void
		{
			//设置光标
			if (m_bPositively==true)
			{
			
				
			}
		
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
include "../../../../GameModule/B_Land/Common/GameLogicDef.as"
include "../../../Memory.as"