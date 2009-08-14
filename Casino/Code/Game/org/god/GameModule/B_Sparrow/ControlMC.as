package 
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Shape;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.events.MouseEvent;

	import org.god.SkinControl.SkinImage;
	import org.god.SkinControl.ImageButton;
	import org.god.GameModule.B_Sparrow.Common.*;
	
	import org.aswing.graphics.*;
	import org.aswing.ASColor;

	public class ControlMC extends Sprite
	{
		//位置标识
		public static const ITEM_WIDTH:Number = 90;	//子项宽度
		public static const ITEM_HEIGHT:Number = 46;//子项高度
		public static const CONTROL_WIDTH:Number = 173;	//控制宽度
		public static const CONTROL_HEIGHT:Number = 41;	//控制高度

		//配置变量
		protected var m_cbActionMask:uint;//类型掩码
		protected var m_cbCenterSparrow:uint;//中心麻将
		protected var m_cbGangSparrow:Array = new Array(5);//杠牌数据
		protected var m_PointBenchmark:Point = new Point;//基准位置

		//状态变量
		protected var m_cbItemCount:uint;//子项数目
		protected var m_cbCurrentItem:uint;//当前子项

		//控件变量
		protected var m_btChiHu:ImageButton;//吃胡按钮
		protected var m_btGiveUp:ImageButton;//放弃按钮

		//资源变量
		protected var m_ImageControlTop:SkinImage;//资源图片
		protected var m_ImageControlMid:SkinImage;//资源图片
		protected var m_ImageControlButtom:SkinImage;//资源图片
		protected var m_ImageActionExplain:SkinImage;//动作解释
		
		protected var m_nWidth:Number = 0;
		protected var m_nHeight:Number = 0;
		
		protected var m_SparrowResource:SparrowResource;

		public function ControlMC(res:SparrowResource)
		{

			m_btChiHu = new ImageButton(IMAGE_BT_CHIHU,250,24);
			addChild(m_btChiHu);
			m_btChiHu.addEventListener(MouseEvent.CLICK, OnEventChihu);
			m_btGiveUp = new ImageButton(IMAGE_BT_GIVEUP,250,24);
			addChild(m_btGiveUp);
			m_btGiveUp.addEventListener(MouseEvent.CLICK, OnEventGiveup);
			
			m_SparrowResource = res;
			
			//配置变量
			m_cbActionMask=0;
			m_cbCenterSparrow=0;
			
			ZeroArray(m_cbGangSparrow);

			//状态变量
			m_cbItemCount=0;
			m_cbCurrentItem=0xFF;

			//加载资源
			m_ImageControlTop= new SkinImage(IMAGE_CONTROL_TOP,173,15);
			m_ImageControlMid= new SkinImage(IMAGE_CONTROL_MID,173,20);
			m_ImageControlButtom= new SkinImage(IMAGE_CONTROL_BOTTOM,173,20);
			m_ImageActionExplain= new SkinImage(IMAGE_ACTION_EXPLAIN,259,37);

			addEventListener(MouseEvent.CLICK, OnEventClick);
			addEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
		}
		
		//基准位置
		public function SetBenchmarkPos( nXPos:Number, nYPos:Number):void
		{
			//位置变量
			m_PointBenchmark.x = nXPos;
			m_PointBenchmark.y = nYPos;

			//调整控件
			RectifyControl();

			return;
		}
		//设置麻将
		public function SetControlInfo( cbCenterSparrow:uint, 
									   cbActionMask:uint, 
									   GangSparrowResult:tagGangSparrowResult):void
		{
			//设置变量
			m_cbItemCount=0;
			m_cbCurrentItem=0xFF;
			m_cbActionMask=cbActionMask;
			m_cbCenterSparrow=cbCenterSparrow;
		
			//杠牌信息
			ZeroArray(m_cbGangSparrow);
			for (var i:uint=0;i<GangSparrowResult.cbSparrowCount;i++) 
			{
				m_cbItemCount++;
				m_cbGangSparrow[i]=GangSparrowResult.cbSparrowData[i];
			}
		
			//计算数目
			var cbItemKind:Array = new Array(WIK_LEFT,WIK_CENTER,WIK_RIGHT,WIK_PENG);
			for (var i:uint=0;i<CountArray(cbItemKind);i++)
			{
				 if ((m_cbActionMask&cbItemKind[i])!=0)
				 {
				 	 m_cbItemCount++;
				 }
			}
		
			//按钮控制
			m_btChiHu.EnableWindow((cbActionMask&WIK_CHI_HU) ? true : false);
		
			//调整控件
			RectifyControl();
		
			//显示窗口
			ShowWindow(true);
		
			return;
		}
		public function ShowWindow(b:Boolean):void
		{
			this.visible = b;
		}
		//调整控件
		public function RectifyControl():void
		{
			//设置位置
			var rect:Rectangle = new Rectangle(m_PointBenchmark.x - CONTROL_WIDTH,
											   m_PointBenchmark.y - ITEM_HEIGHT*m_cbItemCount+CONTROL_HEIGHT,
											   CONTROL_WIDTH,
											   ITEM_HEIGHT*m_cbItemCount+CONTROL_HEIGHT);

			if (m_cbItemCount>0) 
			{
				rect.y-=5;
				rect.height+=5;
			}
			
			
			//调整按钮
			var nYPos:uint=rect.height-32-9 + 8;
			var w:Number  = rect.width;
			m_btGiveUp.MoveWindow(w-50-25,nYPos);
			m_btChiHu.MoveWindow(w-50*2-45,nYPos); 
			
			//设置位置
			this.x =rect.x;
			this.y =rect.y;
			m_nWidth = rect.width;
			m_nHeight= rect.height;
			
			Invalidate();
			
			return;
		}
			
		//重画函数
		public function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			//绘画背景
			m_ImageControlTop.DrawBitmapData(dc,0,0);
			for (var nImageYPos:Number=m_ImageControlTop.GetHeight();
				nImageYPos<rcClient.height;
				nImageYPos+=m_ImageControlMid.GetHeight())
			{
				m_ImageControlMid.DrawBitmapData(dc,0,nImageYPos);
			}
			m_ImageControlButtom.DrawBitmapData(dc,
				0,rcClient.height-m_ImageControlButtom.GetHeight());
		
			//变量定义
			var nYPos:Number=5;
			var cbCurrentItem:uint=0;
			var cbExcursion:Array=new Array(0,1,2);
			var cbItemKind:Array=new Array(WIK_LEFT,WIK_CENTER,WIK_RIGHT,WIK_PENG);
		
			//绘画麻将
			for (var i:uint=0;i<CountArray(cbItemKind);i++)
			{
				if ((m_cbActionMask&cbItemKind[i])!=0)
				{
					//绘画麻将
					for (var j:uint=0;j<3;j++)
					{
						var cbSparrowData:uint=m_cbCenterSparrow;
						if (i<CountArray(cbExcursion))
						{
							cbSparrowData=m_cbCenterSparrow+j-cbExcursion[i];
						}
						m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(dc,
								cbSparrowData,j*26+12,nYPos+5);
					}
		
					//计算位置
					var nXImagePos:uint=0;
					var nItemWidth:Number=m_ImageActionExplain.GetWidth()/7;
					if ((m_cbActionMask&cbItemKind[i])&WIK_PENG)
						nXImagePos=nItemWidth;
		
					//绘画标志
					var nItemHeight:Number=m_ImageActionExplain.GetHeight();
					m_ImageActionExplain.DrawBitmapData(dc,
														126,
														nYPos+5,
														nXImagePos,
														0,
														nItemWidth,
														nItemHeight);
		
					//绘画边框
					if (cbCurrentItem==m_cbCurrentItem)
					{
						var s:Shape = new Shape;
						var g:Graphics2D = new Graphics2D(s.graphics);
						var pen:Pen = new Pen(new ASColor(0xffff00ff), 2);
						g.drawRectangle(pen,
										7, nYPos, rcClient.width-14,
									  	ITEM_HEIGHT);
						dc.draw(s);
						g = null;
						pen=null;
						s = null;
					}
		
					//设置变量
					++cbCurrentItem;
					nYPos+=ITEM_HEIGHT;
				}
			}
		
			//杠牌麻将
			for (var i:uint=0;i<CountArray(m_cbGangSparrow);i++)
			{
				if (m_cbGangSparrow[i]!=0)
				{
					//绘画麻将
					for (var j:uint=0;j<4;j++)
					{
						m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(dc,m_cbGangSparrow[i],j*26+12,nYPos+5);
						//m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(dc,m_cbGangSparrow[i],j*26+12,nYPos+ITEM_HEIGHT+5);
					}
		
					//绘画边框
					if (cbCurrentItem==m_cbCurrentItem)
					{
						var s:Shape = new Shape;
						var g:Graphics2D = new Graphics2D(s.graphics);
						var pen:Pen = new Pen(new ASColor(0xffff00ff), 2);
						g.drawRectangle(pen,
										7,nYPos,
									  	rcClient.width-14,
									  	ITEM_HEIGHT);
						dc.draw(s);
						g = null;
						pen=null;
						s = null;
						
					}
		
					//绘画标志
					var nItemWidth:Number=m_ImageActionExplain.GetWidth()/7;
					var nItemHeight:Number=m_ImageActionExplain.GetHeight();
					m_ImageActionExplain.DrawBitmapData(dc,
														126,
														nYPos+5,
														nItemWidth*3,
														0,
														nItemWidth,
														nItemHeight);
		
					//设置变量
					cbCurrentItem++;
					nYPos+=ITEM_HEIGHT;
				}
				else break;
			}
		
		}
		//鼠标消息
		private function OnEventClick(e:MouseEvent):void
		{
			//索引判断
			if (m_cbCurrentItem!=0xFF)
			{
				//变量定义
				var cbIndex:uint=0;
				var cbItemKind:Array = new Array(WIK_LEFT,WIK_CENTER,WIK_RIGHT,WIK_PENG);
		
				//类型子项
				for (var i:uint=0;i<CountArray(cbItemKind);i++)
				{
					if (((m_cbActionMask&cbItemKind[i])!=0)&&(m_cbCurrentItem==cbIndex++))
					{
						dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SPARROW_OPERATE,
														cbItemKind[i],m_cbCenterSparrow));
						return;
					}
				}
		
				//杠牌子项
				for (var i:uint=0;i<CountArray(m_cbGangSparrow);i++)
				{
					if ((m_cbGangSparrow[i]!=0)&&(m_cbCurrentItem==cbIndex++))
					{
						dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SPARROW_OPERATE,
														WIK_GANG,m_cbGangSparrow[i]));
						return;
					}
				}
		
				//错误断言
				ASSERT(false);
			}
		
			return;
		}
		
		//光标消息
		private function OnEventMouseMove(e:MouseEvent):void
		{
			//位置测试
			if (m_cbItemCount!=0)
			{
				//获取位置
				var MousePoint:Point = new Point(mouseX,mouseY);
				
				//计算索引
				var bCurrentItem:uint=0xFF;
				var rcItem:Rectangle = new Rectangle(5,5,
													m_nWidth,
													 ITEM_HEIGHT*m_cbItemCount);
				if (rcItem.containsPoint(MousePoint))
				{
					bCurrentItem=((MousePoint.y-7)/ITEM_HEIGHT);
				}
		
				//设置索引
				if (m_cbCurrentItem!=bCurrentItem)
				{
					m_cbCurrentItem=bCurrentItem;
					Invalidate();
				}
		
				//设置光标
				if (m_cbCurrentItem!=0xFF)
				{
					return ;
				}
			}
		}
		private function OnEventChihu(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SPARROW_OPERATE,WIK_CHI_HU,0));
		}
		private function OnEventGiveup(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SPARROW_OPERATE,WIK_NULL,0));
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
include "./Common/GameLogicDef.as"
include "../../Common/Memory.as"