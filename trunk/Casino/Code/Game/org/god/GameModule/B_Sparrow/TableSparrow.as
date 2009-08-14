package 
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.geom.Point;
	
	import org.god.GameModule.Common.enDirection;

	//丢弃麻将
	public class TableSparrow
	{
		//麻将数据
		protected var m_wSparrowCount:uint;//麻将数目
		protected var m_cbSparrowData:Array=new Array(14);//麻将数据

		//控制变量
		protected var m_ControlPoint:Point=new Point  ;//基准位置
		protected var m_SparrowDirection:uint;//麻将方向

		protected var m_SparrowResource:SparrowResource;

		public function TableSparrow(res:SparrowResource)
		{
			m_SparrowResource=res;

			//麻将数据
			m_wSparrowCount=0;
			ZeroArray(m_cbSparrowData);

			//控制变量
			m_SparrowDirection=enDirection.Direction_East;
		}
		static public function Destroy(data:TableSparrow):void
		{
			if(data)
			{
				data.m_SparrowResource = null;
				data.m_ControlPoint = null;
				data.m_cbSparrowData = null;
			
			}
		}
		//设置麻将
		public function SetSparrowData(cbSparrowData:Array,wSparrowCount:uint):Boolean
		{

			//效验大小
			ASSERT(wSparrowCount <= CountArray(m_cbSparrowData));
			if (wSparrowCount > CountArray(m_cbSparrowData))
			{
				return false;
			}

			//设置麻将
			m_wSparrowCount=wSparrowCount;
			CopyArray(m_cbSparrowData,cbSparrowData,wSparrowCount);
			
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
			
			//绘画控制
			switch (m_SparrowDirection)
			{
				case enDirection.Direction_East ://东向
					{
						//绘画麻将
						for (var i:uint=0; i < m_wSparrowCount; i++)
						{
							var nXPos:Number=m_ControlPoint.x - 33;
							var nYPos:Number=m_ControlPoint.y + i * 21;
							m_SparrowResource.m_ImageTableRight.DrawSparrowItem(pDC,m_cbSparrowData[m_wSparrowCount - i - 1],nXPos,nYPos);
						}
						break;

					};
				case enDirection.Direction_South ://南向
					{
						//绘画麻将
						for (var i:uint=0; i < m_wSparrowCount; i++)
						{
							var nYPos:Number=m_ControlPoint.y - 36;
							var nXPos:Number=m_ControlPoint.x - (m_wSparrowCount - i) * 26;
							m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(pDC,m_cbSparrowData[i],nXPos,nYPos);
						}
						break;

					};
				case enDirection.Direction_West ://西向
					{
						//绘画麻将
						for (var i:uint=0; i < m_wSparrowCount; i++)
						{
							var nXPos:Number=m_ControlPoint.x;
							var nYPos:Number=m_ControlPoint.y - (m_wSparrowCount - i) * 21;
							m_SparrowResource.m_ImageTableLeft.DrawSparrowItem(pDC,m_cbSparrowData[i],nXPos,nYPos);
						}
						break;

					};
				case enDirection.Direction_North ://北向
					{
						//绘画麻将
						for (var i:uint=0; i < m_wSparrowCount; i++)
						{
							var nYPos:Number=m_ControlPoint.y;
							var nXPos:Number=m_ControlPoint.x + i * 26;
							m_SparrowResource.m_ImageTableTop.DrawSparrowItem(pDC,m_cbSparrowData[m_wSparrowCount - i - 1],nXPos,nYPos);
						}
						break;

				}
			}

		}

	}
}
include "../../Common/Memory.as"