package 
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.geom.Rectangle;
	import flash.geom.Point;
	import flash.events.MouseEvent;
	
	import org.god.SkinControl.SkinImage;
	import org.god.GameModule.Common.enXCollocateMode;
	import org.god.GameModule.Common.enYCollocateMode;
	
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	
	//扑克控件
	public class CardControl extends MovieClip
	{
		protected  static const CARD_SPACE:uint=16;//扑克间距
		protected  static const CARD_WIDTH:uint=70;//扑克宽度
		protected  static const CARD_HEIGHT:uint=95;//扑克高度

		//状态变量
		protected var bDrawCard:Boolean;//盖看牌标志               //add
		protected var m_bShowFirst:Boolean;//显示扑克
		protected var m_bPositively:Boolean;//响应标志
		protected var m_bCaptureMouse:Boolean;//捕获标志
		protected var m_CardDataArray:Array = new Array;//扑克数组

		//位置信息
		protected var m_BasicStation:Point =new Point;//基准位置
		protected var m_XCollocateMode:uint;//显示模式
		protected var m_YCollocateMode:uint;//显示模式

		//静态变量
		protected static var m_ImageCard:SkinImage;//扑克图片
		protected static var m_ImageBack:SkinImage;//扑克图片
		
		protected var m_nWidth:Number = 0;
		protected var m_nHeight:Number= 0;
		//构造函数
		public function CardControl()
		{
			ZeroArray(m_CardDataArray);
			
			//状态变量
			bDrawCard       = false;
			m_bShowFirst    = false;
			m_bPositively   = false;
			m_bCaptureMouse = false;

			//位置变量
			m_XCollocateMode = enXCollocateMode.enXLeft;
			m_YCollocateMode = enYCollocateMode.enYTop;

			//资源变量
			if (m_ImageCard == null)
			{
				m_ImageCard = new SkinImage(IMAGE_CARD, 910, 475);
				m_ImageBack = new SkinImage(IMAGE_CARD, 910, 475);			
			}
			addEventListener(MouseEvent.MOUSE_DOWN, OnEventMouseDown);
			addEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
			addEventListener(MouseEvent.MOUSE_UP, OnEventMouseUp);
		}
		//销毁函数
		public function Destroy():void
		{
			m_CardDataArray = null;
			m_BasicStation = null;
			//m_ImageCard.Unload();
			//m_ImageCard = null;
			//m_ImageBack.Unload();
			//m_ImageBack = null;
			removeEventListener(MouseEvent.MOUSE_DOWN, OnEventMouseDown);
			removeEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
			removeEventListener(MouseEvent.MOUSE_UP, OnEventMouseUp);
		}
		//重画函数
		public function OnPaint(pDC:BitmapData, rcClient:Rectangle):void
		{
			 	if(bDrawCard)																	               //add
   				{																							   //add
	    		 	for (var i:uint=0;i<m_CardDataArray.length;i++)				   				     
	   			 	 {
		   
					 	var bCardData:uint=m_CardDataArray[i];
						 m_ImageCard.DrawBitmapData(pDC,
													i*CARD_SPACE,
													0,
													GetCardXPos(bCardData),
													GetCardYPos(bCardData),
													CARD_WIDTH,
													CARD_HEIGHT); //非常好的画图函数
	    			 }
		    
	 			}
   				 /////////////////////////////////////////////////////////////////////////////////////////begin add
    			 else																			
   			    {
		 
	   			   for(var i:uint=0;i<m_CardDataArray.length;i++)
	   			  {
					m_ImageBack.DrawBitmapData(pDC,
					i*CARD_SPACE,0,
					2*CARD_WIDTH,4*CARD_HEIGHT,CARD_WIDTH,CARD_HEIGHT);     //画扑克背面
				  }
				}
			
		
			return;
		}
		
		//鼠标消息
		public function OnEventMouseUp(e:MouseEvent):void
		{
		
			//状态判断
			if (m_bCaptureMouse == false) return;
		
			//释放捕获
			m_bCaptureMouse = false;
		
			//刷新界面
			Invalidate();
		
			return;
		}
		
		//鼠标消息
		public function OnEventMouseDown(e:MouseEvent):void
		{
			var point:Point = new Point(mouseX,mouseY);
			//状态判断
			if ((m_bPositively == false)||(m_bShowFirst==true)) return;
			if ((point.x > CARD_SPACE)||(m_CardDataArray.length == 0)) return;
		
			m_bCaptureMouse = true;
		
			Invalidate();
			return;
		}
		
		//光标消息
		public function OnEventMouseMove(e:MouseEvent):void
		{
			if ((m_bPositively == true) && (m_bShowFirst == false))
			{
				//获取光标
				var MousePoint:Point = new Point(mouseX,mouseY);
		
				//位置判断
				if (MousePoint.x < CARD_SPACE)
				{
					
					return ;
				}
			}
			
			
		}
		
		//获取数目
		public function GetCardCount():uint
		{
			return m_CardDataArray.length;
		}
		
		//排列扑克
		public function SortCardData():void
		{
			if (m_CardDataArray.length > 1)
			{
				//变量定义
				var bFinished:Boolean     = true;
				var bCardDataTemp:uint;
				
				var nLastPos:int   = m_CardDataArray.length - 1;
		
				//排序操作
				do
				{
					bFinished = true;
					for (var i:int = 0; i < nLastPos; i++) 
					{
						if (CompareOnlyOne(m_CardDataArray[i],m_CardDataArray[i+1])==false)
						{	
							bCardDataTemp        = m_CardDataArray[i];
							m_CardDataArray[i]   = m_CardDataArray[i+1];
							m_CardDataArray[i+1] = bCardDataTemp;
							bFinished            = false;
						}	
					}
					nLastPos--;
				} while (bFinished == false);
			}
		
			return;
		}
		
		//显示扑克
		public function ShowFirstCard( bShowFirst:Boolean):void
		{
			//设置变量
			m_bShowFirst = bShowFirst;
		
			//刷新界面
			
			Invalidate();
		
			return;
		}
		
		//设置响应
		public function AllowPositively( bPositively:Boolean):void
		{
			m_bPositively = bPositively;
			return;
		}
		
		//设置扑克
		public function GetCardData(bCardBuffer:Array,  wBufferSize:uint):uint
		{
			//拷贝数据
			var wCopyCount:uint = m_CardDataArray.length;
			if (wCopyCount > (wBufferSize)) wCopyCount = wBufferSize;
			if(bCardBuffer != null)CopyArray(bCardBuffer,m_CardDataArray,wCopyCount);
		
			return m_CardDataArray.length;
		}
		
		//设置扑克
		public function SetCardData(bCardData:Array, wCardCount:uint):Boolean
		{
			//设置扑克
			ZeroArray(m_CardDataArray);
			m_CardDataArray.length = wCardCount;
			CopyArray(m_CardDataArray,bCardData,wCardCount);
		
			//调整位置
			RectifyControl();
		
			return true;
		}
		
		//设置基准点函数
		public function SetBasicStation(BasicStation:Point, 
		 XCollocateMode:uint, 
		 YCollocateMode:uint):void
		{
			//设置变量
			m_BasicStation   = BasicStation;
			m_XCollocateMode = XCollocateMode;
			m_YCollocateMode = YCollocateMode;
		
			//调整位置
			RectifyControl();
		
			return;
		}
		
		//获取扑克数值
		public function GetCardValue(bCardData:uint):uint
		{
			var bCardVolue:uint = (bCardData - 1) % 13;
			
			if (bCardVolue == 0) bCardVolue = 14;
			
			return bCardVolue;
		}
		
		//扑克位置
		public function GetCardXPos(bCardData:uint):uint
		{
			var bCardValue:uint = uint((bCardData) % 13);
			if (bCardValue == 0) 
			{
				return 12 * CARD_WIDTH;
			}
			return uint(bCardValue - 1) * CARD_WIDTH;
		}
		
		//扑克位置
		public function GetCardYPos(bCardData:uint):uint
		{
			return uint((bCardData-1)/13)*CARD_HEIGHT;
		}
		
		//对比扑克
		public function CompareOnlyOne(bFirstCard:uint, bLastCard:uint):Boolean
		{
			var bFirstVolae:uint = GetCardValue(bFirstCard);
			var bLastVolae:uint  = GetCardValue(bLastCard);
		
			if (bFirstVolae == bLastVolae) return bFirstCard > bLastCard;
			return bFirstVolae > bLastVolae;
		}
		
		//调整窗口位置
		public function RectifyControl():void
		{
			//获取大小 
			var nCardCount:int=m_CardDataArray.length;
			var CardSize:Point = new Point((nCardCount>0)?(CARD_WIDTH+(int)(nCardCount-1)*CARD_SPACE):0,
							CARD_HEIGHT);
		
			//调整位置
			var rcCardRect:Rectangle = new Rectangle;
			switch (m_XCollocateMode)
			{
			case enXCollocateMode.enXLeft:	{ rcCardRect.left = m_BasicStation.x; break; }
			case enXCollocateMode.enXCenter: { rcCardRect.left = m_BasicStation.x-CardSize.x/2; break; }
			case enXCollocateMode.enXRight:	{ rcCardRect.left = m_BasicStation.x-CardSize.x; break; }
			}
		
			switch (m_YCollocateMode)
			{
			case enYCollocateMode.enYTop:	{ rcCardRect.top=m_BasicStation.y; break; }
			case enYCollocateMode.enYCenter: { rcCardRect.top=m_BasicStation.y-CardSize.y/2; break; }
			case enYCollocateMode.enYBottom: { rcCardRect.top=m_BasicStation.y-CardSize.y; break; }
			}
		
			//移动位置
			rcCardRect.right=rcCardRect.left+CardSize.x;
			rcCardRect.bottom=rcCardRect.top+CardSize.y;
			
			this.x = rcCardRect.x;
			this.y = rcCardRect.y;
			m_nWidth = rcCardRect.width;
			m_nHeight= rcCardRect.height;
		
			//重画界面
			Invalidate();
		
			return;
		}
		//设置显示
		public function SetShowCardFlag( flag:Boolean):Boolean               
		{
			bDrawCard=flag;

			Invalidate();                                                           //此句不可缺少，这是关键所在

			return true;
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