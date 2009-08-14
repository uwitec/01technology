package 
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.geom.Point;

	import org.god.GameModule.Common.enDirection;
	
	//堆立麻将
	public class HeapSparrow
	{
		//控制变量
		protected var m_ControlPoint:Point=new Point  ;//基准位置
		protected var m_SparrowDirection:uint;//麻将方向

		//麻将变量
		protected var m_wFullCount:uint;//全满数目
		protected var m_wMinusHeadCount:uint;//头部空缺
		protected var m_wMinusLastCount:uint;//尾部空缺

		protected var m_SparrowResource:SparrowResource;

		public function HeapSparrow(res:SparrowResource)
		{
			m_SparrowResource=res;

			//控制变量
			m_SparrowDirection=enDirection.Direction_East;

			//麻将变量
			m_wFullCount=0;
			m_wMinusHeadCount=0;
			m_wMinusLastCount=0;

		}
		static public function Destroy(data:HeapSparrow):void
		{
			if(data)
			{
				data.m_SparrowResource = null;
				data.m_ControlPoint = null;
			}
		}
		//设置麻将
		public function SetSparrowData(wMinusHeadCount:uint,wMinusLastCount:uint,wFullCount:uint):Boolean
		{
			//设置变量
			m_wFullCount=wFullCount;
			m_wMinusHeadCount=wMinusHeadCount;
			m_wMinusLastCount=wMinusLastCount;
			
			return true;
		}

		//控件控制

		//设置方向
		public function SetDirection(Direction:uint):void
		{
			m_SparrowDirection=Direction;
			
		}
		//基准位置
		public function SetControlPoint(nXPos:int,nYPos:int):void
		{
			m_ControlPoint.x=nXPos;
			m_ControlPoint.y=nYPos;
		}
		
		public function DrawSparrowControl(pDC:BitmapData):void
		{
			switch (m_SparrowDirection)
			{
				case enDirection.Direction_East ://东向
					{
						//绘画麻将
						if (m_wFullCount - m_wMinusHeadCount - m_wMinusLastCount > 0)
						{
							//变量定义
							var nXPos:Number=0;
							var nYPos:Number=0;
							var wHeapIndex:uint=m_wMinusHeadCount / 2;
							var wDoubleHeap:uint=(m_wMinusHeadCount + 1) / 2;
							var wDoubleLast:uint=(m_wFullCount - m_wMinusLastCount) / 2;
							var wFinallyIndex:uint=(m_wFullCount - m_wMinusLastCount) / 2;

							//头部麻将
							if (m_wMinusHeadCount % 2 != 0)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + wHeapIndex * 13 + 9;
								m_SparrowResource.m_ImageHeapSingleV.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//中间麻将
							for (var i:uint=wDoubleHeap; i < wFinallyIndex; i++)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + i * 13;
								m_SparrowResource.m_ImageHeapDoubleV.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//尾部麻将
							if (m_wMinusLastCount % 2 != 0)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + wFinallyIndex * 13 + 9;
								m_SparrowResource.m_ImageHeapSingleV.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						break;

					};
				case enDirection.Direction_South ://南向
					{
						//绘画麻将
						if (m_wFullCount - m_wMinusHeadCount - m_wMinusLastCount > 0)
						{
							//变量定义
							var nXPos:Number=0;
							var nYPos:Number=0;
							var wHeapIndex:uint=m_wMinusLastCount / 2;
							var wDoubleHeap:uint=(m_wMinusLastCount + 1) / 2;
							var wDoubleLast:uint=(m_wFullCount - m_wMinusHeadCount) / 2;
							var wFinallyIndex:uint=(m_wFullCount - m_wMinusHeadCount) / 2;

							//尾部麻将
							if (m_wMinusLastCount % 2 != 0)
							{
								nYPos=m_ControlPoint.y + 11;
								nXPos=m_ControlPoint.x + wHeapIndex * 17;
								m_SparrowResource.m_ImageHeapSingleH.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//中间麻将
							for (var i:uint=wDoubleHeap; i < wFinallyIndex; i++)
							{
								nYPos=m_ControlPoint.y;
								nXPos=m_ControlPoint.x + i * 17;
								m_SparrowResource.m_ImageHeapDoubleH.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//头部麻将
							if (m_wMinusHeadCount % 2 != 0)
							{
								nYPos=m_ControlPoint.y + 11;
								nXPos=m_ControlPoint.x + wFinallyIndex * 17;
								m_SparrowResource.m_ImageHeapSingleH.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						break;

					};
				case enDirection.Direction_West ://西向
					{
						//绘画麻将
						if (m_wFullCount - m_wMinusHeadCount - m_wMinusLastCount > 0)
						{
							//变量定义
							var nXPos:Number=0;
							var nYPos:Number=0;
							var wHeapIndex:uint=m_wMinusLastCount / 2;
							var wDoubleHeap:uint=(m_wMinusLastCount + 1 )/ 2;
							var wDoubleLast:uint=(m_wFullCount - m_wMinusHeadCount) / 2;
							var wFinallyIndex:uint=(m_wFullCount - m_wMinusHeadCount) / 2;

							//尾部麻将
							if (m_wMinusLastCount % 2 != 0)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + wHeapIndex * 13 + 9;
								m_SparrowResource.m_ImageHeapSingleV.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//中间麻将
							for (var i:uint=wDoubleHeap; i < wFinallyIndex; i++)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + i * 13;
								m_SparrowResource.m_ImageHeapDoubleV.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//头部麻将
							if (m_wMinusHeadCount % 2 != 0)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + wFinallyIndex * 13 + 9;
								m_SparrowResource.m_ImageHeapSingleV.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						break;

					};
				case enDirection.Direction_North ://北向
					{
						//绘画麻将
						if (m_wFullCount - m_wMinusHeadCount - m_wMinusLastCount > 0)
						{
							//变量定义
							var nXPos:Number=0;
							var nYPos:Number=0;
							var wHeapIndex:uint=m_wMinusHeadCount / 2;
							var wDoubleHeap:uint=(m_wMinusHeadCount + 1 )/ 2;
							var wDoubleLast:uint=(m_wFullCount - m_wMinusLastCount) / 2;
							var wFinallyIndex:uint=(m_wFullCount - m_wMinusLastCount) / 2;

							//头部麻将
							if (m_wMinusHeadCount % 2 != 0)
							{
								nYPos=m_ControlPoint.y + 11;
								nXPos=m_ControlPoint.x + wHeapIndex * 17;
								m_SparrowResource.m_ImageHeapSingleH.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//中间麻将
							for (var i:uint=wDoubleHeap; i < wFinallyIndex; i++)
							{
								nYPos=m_ControlPoint.y;
								nXPos=m_ControlPoint.x + i * 17;
								m_SparrowResource.m_ImageHeapDoubleH.DrawBitmapData(pDC,nXPos,nYPos);
							}

							//尾部麻将
							if (m_wMinusLastCount % 2 != 0)
							{
								nYPos=m_ControlPoint.y + 11;
								nXPos=m_ControlPoint.x + wFinallyIndex * 17;
								m_SparrowResource.m_ImageHeapSingleH.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						break;

					}
			}
		}
	}
}