package 
{
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.geom.Rectangle;
	import flash.geom.Point;
	import flash.events.MouseEvent;
	
	import org.god.SkinControl.SkinImage;
	import org.god.SkinControl.ImageButton;
	import org.god.Common.DrawUtil;

	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	
	//金币控制
	public class GoldControl extends Sprite
	{
		protected static  const CELL_WIDTH:uint = 18;//单元宽度
		protected static  const LESS_WIDTH:uint =CELL_WIDTH * 9;//最小宽度
		protected static  const SPACE_WIDTH:uint =0;//空袭宽度
		protected static  const BUTTON_WIDTH:uint =0;//功能区域
		protected static  const CONTROL_HEIGHT:uint =58;//控件高度

		//变量定义
		protected var m_lMaxGold:int;//最大金币
		protected var m_lGoldCell:Array =new Array(7);//单元金币
		protected var m_BasicPoint:Point = new Point;//基础位置

		//位置变量
		protected var m_nWidth:int;//控件宽度
		protected var m_nHeight:int;
		protected var m_nCellCount:int;//单元数目
		protected var m_AppendWidth:int;//附加宽度
		protected var m_rectGoldCell:Array =new Array(7);//单元金币

		//资源变量
		protected var m_ImageLeft:SkinImage;//背景资源
		protected var m_ImageMid:SkinImage;//背景资源
		protected var m_ImageRight:SkinImage;//背景资源
		
		protected var m_btZero:ImageButton;
		//构造函数
		public function GoldControl()
		{
			//界面变量
			m_nWidth=0;
			m_nHeight = CONTROL_HEIGHT;
			m_nCellCount=0;
			m_AppendWidth=0;

			//设置变量
			m_lMaxGold=0;
			ZeroArray(m_lGoldCell);
			for(var i:int = 0; i < m_rectGoldCell.length; i ++)
			{
				m_rectGoldCell[i] = new Rectangle;
			}

			//加载资源
			m_ImageLeft= new SkinImage(IMAGE_CONTROL_BACK_LEFT,20,58);
			m_ImageMid= new SkinImage(IMAGE_CONTROL_BACK_MID,54,58);
			m_ImageRight= new SkinImage(IMAGE_CONTROL_BACK_RIGHT,20,58);
			
			m_btZero = new ImageButton(IMAGE_CONTROL_BT_ZERO, 216,24);
			addChild(m_btZero);
			m_btZero.addEventListener(MouseEvent.CLICK, OnEventZero);
			
			this.addEventListener(MouseEvent.CLICK, OnEventClick);
			this.addEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
		}
		//销毁
		public function Destroy():void
		{
			m_lGoldCell = null;
			m_BasicPoint = null;
			ZeroArray(m_rectGoldCell,null);
			m_rectGoldCell = null;
			m_ImageLeft.Unload();
			m_ImageLeft = null;
			m_ImageMid.Unload();
			m_ImageMid = null;
			m_ImageRight.Unload();
			m_ImageRight = null;
			m_btZero.Destroy();
			m_btZero.removeEventListener(MouseEvent.CLICK, OnEventZero);
			removeChild(m_btZero);
			m_btZero = null;
			this.removeEventListener(MouseEvent.CLICK, OnEventClick);
			this.removeEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
		}
		//获取金币
		public function GetGold():int
		{
			var lGold:int=0;
			for (var i:int=0; i<CountArray(m_lGoldCell); i++)
			{
				lGold+=m_lGoldCell[i]*Math.pow(10,i);
			}
			return lGold;
		}
		//设置金币
		public function SetGold(lGold:int):void
		{
			//调整参数
			if (lGold>m_lMaxGold)
			{
				lGold=m_lMaxGold;
			}
			ZeroArray(m_lGoldCell);

			//设置变量
			var nIndex:uint=0;
			while (lGold>0)
			{
				m_lGoldCell[nIndex++]=lGold%10;
				lGold/=10;
			}
			//重画界面
			Invalidate();

			return;
		}
		//设置用户最高下注数
		public function SetMaxGold(lMaxGold:int):void
		{
			//效验改变
			if (m_lMaxGold==lMaxGold && lMaxGold != 0)
			{
				return;
			}

			//设置变量
			m_lMaxGold=lMaxGold;
			if (m_lMaxGold>9999999)
			{
				m_lMaxGold=9999999;
			}
			ZeroArray(m_lGoldCell);
			for(var i:int = 0; i < m_rectGoldCell.length;  i ++)
				m_rectGoldCell[i].setEmpty();

			//计算单元
			m_nCellCount=0;
			while (lMaxGold>0)
			{
				lMaxGold/=10;
				m_nCellCount++;
			}
			m_nCellCount=__min(CountArray(m_lGoldCell),__max(m_nCellCount,1));

			//设置界面
			m_AppendWidth=0;
			m_nWidth=(m_nCellCount+(m_nCellCount-1)/3)*CELL_WIDTH+SPACE_WIDTH*2+BUTTON_WIDTH;
			if (m_nWidth<LESS_WIDTH)
			{
				m_AppendWidth=LESS_WIDTH-m_nWidth;
				m_nWidth=LESS_WIDTH;
			}
			this.x = m_BasicPoint.x-m_nWidth;
			this.y = m_BasicPoint.y-CONTROL_HEIGHT;
			
			m_btZero.x = m_nWidth - 54;
			m_btZero.y = CONTROL_HEIGHT - 30;

			//重画界面
			Invalidate();

			return;
		}
		//设置位置
		public function SetBasicPoint(nXPos:Number, nYPos:Number):void
		{
			//设置变量
			m_BasicPoint.x = nXPos;
			m_BasicPoint.y = nYPos;

			//调整界面
			RectifyControl();

			return;
		}
		//调整控件
		public function RectifyControl():void
		{
			this.x = m_BasicPoint.x-m_nWidth;
			this.y = m_BasicPoint.y-CONTROL_HEIGHT;
			
			return;
		}
		//绘画金币
		public function DrawGoldCell(pDC:BitmapData, 
									 nXPos:Number, 
									 nYPox:Number,  
									 lGold:int):Rectangle
		{
			if (lGold<10)
			{
				var szBuffer:String = Number(lGold).toString();
				var rect:* = DrawUtil.TextOut(pDC, nXPos,nYPox,szBuffer, 20,0xffff00,0,0,"center");
				return new Rectangle(rect.x,rect.y,rect.width,rect.height);
			}
			else
			{
				var szBuffer:String =",";
				var rect:* = DrawUtil.TextOut(pDC, nXPos,nYPox,szBuffer, 20,0xffff00,0,0,"center");
				return new Rectangle(rect.x,rect.y,rect.width,rect.height);
			}
		}
		//重画函数
		public function OnPaint(pDC:BitmapData, ClientRect:Rectangle):void
		{
			//绘画背景
			
			m_ImageLeft.DrawBitmapData(pDC,0,0);
			var n:uint = (ClientRect.width - m_ImageLeft.GetWidth() - m_ImageRight.GetWidth()) / m_ImageMid.GetWidth() + 1;
			for(var i:int = 0; i < n ; i ++)
			{
				if(i != n -1)
				{
					m_ImageMid.DrawBitmapData(pDC,
					m_ImageLeft.GetWidth() + i * m_ImageMid.GetWidth(),0,
					0,0,
					m_ImageMid.GetWidth(),
					m_ImageMid.GetHeight());
				}
				else
				{
					m_ImageMid.DrawBitmapData(pDC,
					m_ImageLeft.GetWidth() + i * m_ImageMid.GetWidth(),0,
					0,0,
					ClientRect.width-m_ImageLeft.GetWidth()-m_ImageRight.GetWidth(),
					m_ImageMid.GetHeight());
				}
			}
			

			m_ImageRight.DrawBitmapData(pDC,
			ClientRect.width-m_ImageRight.GetWidth(),
			0);

			//绘画金币
			var nXExcursion:int=ClientRect.width-SPACE_WIDTH-BUTTON_WIDTH;
			for (var i:int=0; i<m_nCellCount; i++)
			{
				//绘画逗号
				if ((i!=0)&&(i%3)==0)
				{
					nXExcursion-=CELL_WIDTH;
					DrawGoldCell(pDC,nXExcursion,ClientRect.height/4,10);
				}
				//绘画数字
				nXExcursion-=CELL_WIDTH;
				m_rectGoldCell[i] = 
				DrawGoldCell(pDC,nXExcursion,ClientRect.height/4,m_lGoldCell[i]);
			}


			//提示信息
			var szBuffer:String = "最多：" + Number(m_lMaxGold).toString();
			var rect:Rectangle = DrawUtil.TextOut(pDC, 2,28,szBuffer, 14, 0xffff00);

	
			return;
		}
		//左键按下消息
		public function OnEventClick(e:MouseEvent):void
		{
			/*
			var px:uint = mouseX;
			var py:uint = mouseY;
			
			//变量定义
			var nViewCellCount:uint=(m_nCellCount-1)/3+m_nCellCount;

			//位置过虑
			var nHeadWidth:uint=SPACE_WIDTH+m_AppendWidth;
			if ((py<=2)||(py>=26))
			{
				return;
			}
			if ((px<=nHeadWidth)||(px>=(CELL_WIDTH*nViewCellCount+nHeadWidth)))
			{
				return;
			}

			//按钮测试
			var iCellPos:uint=(nViewCellCount-(px-nHeadWidth)/CELL_WIDTH);
			if ((iCellPos==3)||(iCellPos==7))
			{
				return;
			}
			if (iCellPos>3)
			{
				var v0:uint = (iCellPos-1)/3;
				iCellPos=iCellPos- v0;
			}
			*/
			var iCellPos:int= -1;
			var pt:Point = new Point(mouseX,mouseY);
			for (var i:int=0; i<m_nCellCount; i++)
			{
				if(m_rectGoldCell[i] != null &&
				m_rectGoldCell[i].containsPoint(pt))
				{
					iCellPos = i;
					break;
				}
			}
			if(iCellPos == -1)
				return;

			//计算限制
			ASSERT((iCellPos>=0)&&(iCellPos<CountArray(m_lGoldCell)));
			if (m_lGoldCell[iCellPos]!=9)
			{
				var lAddGold:uint=Math.pow(10.0,iCellPos);
				if ((GetGold()+lAddGold)>m_lMaxGold)
				{
					return;
				}
			}
			//设置变量
			m_lGoldCell[iCellPos]=(m_lGoldCell[iCellPos]+1)%10;
			if(m_lGoldCell[iCellPos]!=0)                               //add
				dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_ONFOLLOW,0,0));
			//重画界面
			Invalidate();

			return;
		}
		//设置光标
		public function OnEventMouseMove(e:MouseEvent):void
		{
			//获取鼠标
			var point:Point =new Point(mouseX, mouseY);

			//设置光标
			var nHeadWidth:uint = SPACE_WIDTH + m_AppendWidth;
			var nViewCellCount:uint = (m_nCellCount - 1) / 3 + m_nCellCount;
			if ((point.y > 2) && (point.y < 26) && (point.x>nHeadWidth) && (point.x < (CELL_WIDTH*nViewCellCount+nHeadWidth)))
			{
				var iCellPos:uint = (m_nCellCount - (point.x - nHeadWidth) / CELL_WIDTH) - 1;
				if ((iCellPos != 3) && (iCellPos != 7))
				{
					//SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
					return;
				}
			}
		}
		public function ShowWindow(bShow:Boolean):void
		{
			this.visible = bShow;
		}
		public function IsWindowVisible():Boolean
		{
			return this.visible;
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
		private function OnEventZero(e:MouseEvent):void
		{
			for (var i:int=0; i<CountArray(m_lGoldCell); i++)
			{
				m_lGoldCell[i] = 0;
			}
			Invalidate();
		}
	}
}
include "../../Common/Memory.as"