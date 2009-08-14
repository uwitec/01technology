package 
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.geom.Point;
	
	import org.god.GameModule.Common.enXCollocateMode;
	import org.god.GameModule.Common.enYCollocateMode;
	import org.god.GameModule.Common.enDirection;

	//麻将控件
	public class SparrowControl
	{
		//麻将大小
		public static  var SPARROW_WIDTH:uint=42;//麻将宽度
		public static  var SPARROW_HEIGHT:uint=60;//麻将高度
		//公共定义
		public static  var POS_SHOOT:uint=5;//弹起象素
		public static  var POS_SPACE:uint=8;//分隔间隔
		public static  var ITEM_COUNT:uint=43;//子项数目
		public static  var INVALID_ITEM:uint=0xFFFF;//无效索引

		//状态变量
		protected var m_bPositively:Boolean;//响应标志
		protected var m_bDisplayItem:Boolean;//显示标志

		//位置变量
		protected var m_BenchmarkPos:Point=new Point  ;//基准位置
		protected var m_XCollocateMode:uint;//显示模式
		protected var m_YCollocateMode:uint;//显示模式

		//麻将数据
		protected var m_wSparrowCount:uint;//麻将数目
		protected var m_wHoverItem:uint;//盘旋子项
		protected var m_CurrentSparrow:tagSparrowItem=new tagSparrowItem  ;//当前麻将
		protected var m_SparrowItemArray:Array=new Array(13);//麻将数据

		//资源变量
		protected var m_ControlSize:Point=new Point  ;//控件大小
		protected var m_ControlPoint:Point=new Point  ;//控件位置

		protected var m_SparrowResource:SparrowResource;

		public function SparrowControl(res:SparrowResource)
		{
			m_SparrowResource=res;

			for (var i:uint=0; i < m_SparrowItemArray.length; i++)
			{
				m_SparrowItemArray[i]=new tagSparrowItem  ;
			}
			//状态变量
			m_bPositively=false;
			m_bDisplayItem=false;

			//位置变量
			m_XCollocateMode=enXCollocateMode.enXCenter;
			m_YCollocateMode=enYCollocateMode.enYCenter;


			//麻将数据
			m_wSparrowCount=0;
			m_wHoverItem=INVALID_ITEM;

			//加载设置
			m_ControlSize.y=SPARROW_HEIGHT + POS_SHOOT;
			m_ControlSize.x=(CountArray(m_SparrowItemArray) + 1) * SPARROW_WIDTH + POS_SPACE;
		}
		static public function Destroy(data:SparrowControl):void
		{
			if(data)
			{
				data.m_SparrowResource = null;
				data.m_BenchmarkPos = null;
				data.m_CurrentSparrow = null;
				ZeroArray(data.m_SparrowItemArray, null);
			}
		}
		//设置响应
		public function SetPositively(bPositively:Boolean):void
		{
			m_bPositively=bPositively;
			
		}
		//设置显示
		public function SetDisplayItem(bDisplayItem:Boolean):void
		{
			m_bDisplayItem=bDisplayItem;
			
		}

		//基准位置
		public function SetBenchmarkPos(nXPos:int,nYPos:int,XCollocateMode:uint,YCollocateMode:int):void
		{
			//设置变量
			m_BenchmarkPos.x=nXPos;
			m_BenchmarkPos.y=nYPos;
			
			m_XCollocateMode=XCollocateMode;
			m_YCollocateMode=YCollocateMode;

			//横向位置
			switch (m_XCollocateMode)
			{
				case enXCollocateMode.enXLeft :
					{
						m_ControlPoint.x=m_BenchmarkPos.x;
						break;



					};
				case enXCollocateMode.enXCenter :
					{
						m_ControlPoint.x=m_BenchmarkPos.x - m_ControlSize.x / 2;
						break;



					};
				case enXCollocateMode.enXRight :
					{
						m_ControlPoint.x=m_BenchmarkPos.x - m_ControlSize.x;
						break;



				}
			};

			//竖向位置
			switch (m_YCollocateMode)
			{
				case enYCollocateMode.enYTop :
					{
						m_ControlPoint.y=m_BenchmarkPos.y;
						break;



					};
				case enYCollocateMode.enYCenter :
					{
						m_ControlPoint.y=m_BenchmarkPos.y - m_ControlSize.y / 2;
						break;



					};
				case enYCollocateMode.enYBottom :
					{
						m_ControlPoint.y=m_BenchmarkPos.y - m_ControlSize.y;
						break;



				}
			};
			
			return;
		}
		//获取麻将
		public function GetHoverSparrow():uint
		{
			//获取麻将
			if (m_wHoverItem != INVALID_ITEM)
			{
				if (m_wHoverItem == CountArray(m_SparrowItemArray))
				{
					return m_CurrentSparrow.cbSparrowData;
				}
				return m_SparrowItemArray[m_wHoverItem].cbSparrowData;
			}
			return 0;
		}
		//获取麻将
		public function GetCurrentSparrow():uint
		{
			return m_CurrentSparrow.cbSparrowData;
		}
		//设置麻将
		public function SetCurrentSparrowData(cbSparrowData:uint):Boolean
		{
			//设置变量
			m_CurrentSparrow.bShoot=false;
			m_CurrentSparrow.cbSparrowData=cbSparrowData;
			
			return true;
		}
		//设置麻将
		public function SetCurrentSparrow(SparrowItem:tagSparrowItem):Boolean
		{
			//设置变量
			m_CurrentSparrow.bShoot=SparrowItem.bShoot;
			m_CurrentSparrow.cbSparrowData=SparrowItem.cbSparrowData;
			
			return true;
		}
		//设置麻将
		public function SetSparrowData(cbSparrowData:Array,wSparrowCount:uint,cbCurrentSparrow:uint):Boolean
		{
			//效验大小
			ASSERT(wSparrowCount <= CountArray(m_SparrowItemArray));
			if (wSparrowCount > CountArray(m_SparrowItemArray))
			{
				return false;
			}

			//当前麻将
			m_CurrentSparrow.bShoot=false;
			m_CurrentSparrow.cbSparrowData=cbCurrentSparrow;

			//设置麻将
			m_wSparrowCount=wSparrowCount;
			for (var i:uint=0; i < m_wSparrowCount; i++)
			{
				m_SparrowItemArray[i].bShoot=false;
				m_SparrowItemArray[i].cbSparrowData=cbSparrowData[i];
			}
			
			return true;
		}
		//设置麻将
		public function SetSparrowItem(SparrowItemArray:Array,wSparrowCount:uint):Boolean
		{
			//效验大小
			ASSERT(wSparrowCount <= CountArray(m_SparrowItemArray));
			if (wSparrowCount > CountArray(m_SparrowItemArray))
			{
				return 0;
			}

			//设置麻将
			m_wSparrowCount=wSparrowCount;
			for (var i:uint=0; i < m_wSparrowCount; i++)
			{
				m_SparrowItemArray[i].bShoot=SparrowItemArray[i].bShoot;
				m_SparrowItemArray[i].cbSparrowData=SparrowItemArray[i].cbSparrowData;
			}
			
			return true;
		}
		//索引切换
		public function SwitchSparrowPoint(MousePoint:Point):uint
		{
			//基准位置
			var nXPos:uint=MousePoint.x - m_ControlPoint.x;
			var nYPos:uint=MousePoint.y - m_ControlPoint.y;

			//范围判断
			if (nXPos < 0 || nXPos > m_ControlSize.x)
			{
				return INVALID_ITEM;
			}
			if (nYPos < POS_SHOOT || nYPos > m_ControlSize.y)
			{
				return INVALID_ITEM;
			}

			//牌列子项
			if (nXPos < SPARROW_WIDTH * CountArray(m_SparrowItemArray))
			{
				var wViewIndex:uint=nXPos / SPARROW_WIDTH + m_wSparrowCount;
				if (wViewIndex >= CountArray(m_SparrowItemArray))
				{
					return wViewIndex - CountArray(m_SparrowItemArray);
				}
				return INVALID_ITEM;
			}
			//当前子项
			if (m_CurrentSparrow.cbSparrowData != 0 && nXPos >= (m_ControlSize.x - SPARROW_WIDTH))
			{
				return CountArray(m_SparrowItemArray);
			}

			return INVALID_ITEM;
		}
		//光标消息
		public function OnEventMouse(point:Point):Array
		{
			var bRePaint:Boolean=false;
			//获取索引
			var wHoverItem:uint=SwitchSparrowPoint(point);

			//响应判断
			if (m_bPositively == false && m_wHoverItem != INVALID_ITEM)
			{
				bRePaint=true;
				m_wHoverItem=INVALID_ITEM;
			}
			//更新判断
			if (wHoverItem != m_wHoverItem && m_bPositively == true)
			{
				bRePaint=true;
				m_wHoverItem=wHoverItem;

			}
			var result:Array = new Array(2);
			result[0] = bRePaint;
			result[1] = (wHoverItem != INVALID_ITEM)? true : false;
			return result;
		}
		
		public function DrawSparrowControl(pDC:BitmapData):void
		{
			//绘画准备
			var nXExcursion:Number=m_ControlPoint.x + (CountArray(m_SparrowItemArray) - m_wSparrowCount) * SPARROW_WIDTH;

			//绘画麻将
			for (var i:uint=0; i < m_wSparrowCount; i++)
			{
				//计算位置
				var nXScreenPos:Number=nXExcursion + SPARROW_WIDTH * i;
				var nYScreenPos:Number=m_ControlPoint.y + ((m_SparrowItemArray[i].bShoot == false && m_wHoverItem != i)?POS_SHOOT:0);

				//绘画麻将
				var cbSparrowData:uint=((m_bDisplayItem == true)?m_SparrowItemArray[i].cbSparrowData:0);
				m_SparrowResource.m_ImageUserBottom.DrawSparrowItem(pDC,cbSparrowData,nXScreenPos,nYScreenPos);
			}
			//当前麻将
			if (m_CurrentSparrow.cbSparrowData != 0)
			{
				//计算位置
				var nXScreenPos:Number=m_ControlPoint.x + m_ControlSize.x - SPARROW_WIDTH;
				var nYScreenPos:Number=m_ControlPoint.y + ((m_CurrentSparrow.bShoot == false && m_wHoverItem != CountArray(m_SparrowItemArray))?POS_SHOOT:0);

				//绘画麻将
				var cbSparrowData:uint=((m_bDisplayItem == true)?m_CurrentSparrow.cbSparrowData:0);
				m_SparrowResource.m_ImageUserBottom.DrawSparrowItem(pDC,cbSparrowData,nXScreenPos,nYScreenPos);
			}
		}
	}
}
include "../../Common/Memory.as";