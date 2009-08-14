package org.god.Common.BetHistory.GameModule.B_Sparrow
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.geom.Point;
	
	import org.god.GameModule.Common.enDirection;

	//组合麻将
	public class WeaveSparrow 
	{
		//控制变量
		protected var m_bDisplayItem:Boolean;//显示标志
		protected var m_ControlPoint:Point=new Point  ;//基准位置
		protected var m_SparrowDirection:uint;//麻将方向

		//麻将数据
		protected var m_wSparrowCount:uint;//麻将数目
		protected var m_cbSparrowData:Array=new Array(4);//麻将数据

		protected var m_SparrowResource:SparrowResource;

		public function WeaveSparrow(res:SparrowResource)
		{
			m_SparrowResource=res;

			//状态变量
			m_bDisplayItem=false;

			m_SparrowDirection=enDirection.Direction_South;

			//麻将数据
			m_wSparrowCount=0;
			ZeroArray(m_cbSparrowData);

		}
		static public function Destroy(data:WeaveSparrow):void
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
		//设置显示
		public function SetDisplayItem(bDisplayItem:Boolean):void
		{
			m_bDisplayItem=bDisplayItem;
			
		}
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

		//内部函数
		//获取麻将
		public function GetSparrowData(wIndex:uint):uint
		{
			ASSERT(wIndex < CountArray(m_cbSparrowData));
			return (m_bDisplayItem == true || wIndex == 1)?m_cbSparrowData[wIndex]:0;
		}
		
		public function DrawSparrowControl(pDC:BitmapData):void
		{
			//显示判断
			if (m_wSparrowCount == 0)
			{
				return;
			}

			//变量定义
			var nXScreenPos:Number=0;
			var nYScreenPos:Number=0;
			var nItemWidth:Number=0;
			var nItemHeight:Number=0;
			var nItemWidthEx:Number=0;
			var nItemHeightEx:Number=0;

			//绘画麻将
			switch (m_SparrowDirection)
			{
				case enDirection.Direction_East ://东向
					{
						//变量定义
						nItemWidth=m_SparrowResource.m_ImageTableRight.GetViewWidth();
						nItemHeight=m_SparrowResource.m_ImageTableRight.GetViewHeight();
						nItemWidthEx=m_SparrowResource.m_ImageTableTop.GetViewWidth();
						nItemHeightEx=m_SparrowResource.m_ImageTableTop.GetViewHeight();

						//第四麻将
						if (m_wSparrowCount == 4)
						{
							nXScreenPos=m_ControlPoint.x;
							nYScreenPos=m_ControlPoint.y;
							m_SparrowResource.m_ImageTableTop.DrawSparrowItem(pDC,GetSparrowData(3),nXScreenPos,nYScreenPos);
						}

						//第三麻将
						nYScreenPos=m_ControlPoint.y;
						nXScreenPos=m_ControlPoint.x + nItemWidthEx;
						m_SparrowResource.m_ImageTableTop.DrawSparrowItem(pDC,GetSparrowData(2),nXScreenPos,nYScreenPos);

						//第二麻将
						nYScreenPos=m_ControlPoint.y + nItemHeightEx;
						nXScreenPos=m_ControlPoint.x + nItemWidthEx * 2 - nItemWidth;
						m_SparrowResource.m_ImageTableRight.DrawSparrowItem(pDC,GetSparrowData(1),nXScreenPos,nYScreenPos);

						//第一麻将
						nXScreenPos=m_ControlPoint.x + nItemWidthEx * 2 - nItemWidth;
						nYScreenPos=m_ControlPoint.y + nItemHeightEx + nItemHeight;
						m_SparrowResource.m_ImageTableRight.DrawSparrowItem(pDC,GetSparrowData(0),nXScreenPos,nYScreenPos);

						break;

					};
				case enDirection.Direction_South ://南向
					{
						//变量定义
						nItemWidth=m_SparrowResource.m_ImageTableBottom.GetViewWidth();
						nItemHeight=m_SparrowResource.m_ImageTableBottom.GetViewHeight();
						nItemWidthEx=m_SparrowResource.m_ImageTableRight.GetViewWidth();
						nItemHeightEx=m_SparrowResource.m_ImageTableRight.GetViewHeight();

						//第四麻将
						if (m_wSparrowCount == 4)
						{
							nYScreenPos=m_ControlPoint.y;
							nXScreenPos=m_ControlPoint.x - nItemWidthEx;
							m_SparrowResource.m_ImageTableRight.DrawSparrowItem(pDC,GetSparrowData(3),nXScreenPos,nYScreenPos);
						}

						//第三麻将
						nXScreenPos=m_ControlPoint.x - nItemWidthEx;
						nYScreenPos=m_ControlPoint.y + nItemHeightEx;
						m_SparrowResource.m_ImageTableRight.DrawSparrowItem(pDC,GetSparrowData(2),nXScreenPos,nYScreenPos);

						//第二麻将
						nXScreenPos=m_ControlPoint.x - nItemWidthEx - nItemWidth;
						nYScreenPos=m_ControlPoint.y + nItemHeightEx * 2 - nItemHeight;
						m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(pDC,GetSparrowData(1),nXScreenPos,nYScreenPos);

						//第一麻将
						nXScreenPos=m_ControlPoint.x - nItemWidthEx - nItemWidth * 2;
						nYScreenPos=m_ControlPoint.y + nItemHeightEx * 2 - nItemHeight;
						m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(pDC,GetSparrowData(0),nXScreenPos,nYScreenPos);

						break;

					};
				case enDirection.Direction_West ://西向
					{
						//变量定义
						nItemWidth=m_SparrowResource.m_ImageTableLeft.GetViewWidth();
						nItemHeight=m_SparrowResource.m_ImageTableLeft.GetViewHeight();
						nItemWidthEx=m_SparrowResource.m_ImageTableBottom.GetViewWidth();
						nItemHeightEx=m_SparrowResource.m_ImageTableBottom.GetViewHeight();

						//第一麻将
						nXScreenPos=m_ControlPoint.x - nItemWidthEx * 2;
						nYScreenPos=m_ControlPoint.y - nItemHeightEx - nItemHeight * 2;
						m_SparrowResource.m_ImageTableLeft.DrawSparrowItem(pDC,GetSparrowData(0),nXScreenPos,nYScreenPos);

						//第二麻将
						nXScreenPos=m_ControlPoint.x - nItemWidthEx * 2;
						nYScreenPos=m_ControlPoint.y - nItemHeightEx - nItemHeight;
						m_SparrowResource.m_ImageTableLeft.DrawSparrowItem(pDC,GetSparrowData(1),nXScreenPos,nYScreenPos);

						//第三麻将
						nYScreenPos=m_ControlPoint.y - nItemHeightEx;
						nXScreenPos=m_ControlPoint.x - nItemWidthEx * 2;
						m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(pDC,GetSparrowData(2),nXScreenPos,nYScreenPos);

						//第四麻将
						if (m_wSparrowCount == 4)
						{
							nXScreenPos=m_ControlPoint.x - nItemWidthEx;
							nYScreenPos=m_ControlPoint.y - nItemHeightEx;
							m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(pDC,GetSparrowData(3),nXScreenPos,nYScreenPos);
						}

						break;

					};
				case enDirection.Direction_North ://北向
					{
						//变量定义
						nItemWidth=m_SparrowResource.m_ImageTableTop.GetViewWidth();
						nItemHeight=m_SparrowResource.m_ImageTableTop.GetViewHeight();
						nItemWidthEx=m_SparrowResource.m_ImageTableLeft.GetViewWidth();
						nItemHeightEx=m_SparrowResource.m_ImageTableLeft.GetViewHeight();

						//第三麻将
						nXScreenPos=m_ControlPoint.x;
						nYScreenPos=m_ControlPoint.y - nItemHeightEx * 2;
						m_SparrowResource.m_ImageTableLeft.DrawSparrowItem(pDC,GetSparrowData(2),nXScreenPos,nYScreenPos);

						//第四麻将
						if (m_wSparrowCount == 4)
						{
							nXScreenPos=m_ControlPoint.x;
							nYScreenPos=m_ControlPoint.y - nItemHeightEx;
							m_SparrowResource.m_ImageTableLeft.DrawSparrowItem(pDC,GetSparrowData(3),nXScreenPos,nYScreenPos);
						}

						//第二麻将
						nXScreenPos=m_ControlPoint.x + nItemWidthEx;
						nYScreenPos=m_ControlPoint.y - nItemHeightEx * 2;
						m_SparrowResource.m_ImageTableTop.DrawSparrowItem(pDC,GetSparrowData(1),nXScreenPos,nYScreenPos);

						//第一麻将
						nYScreenPos=m_ControlPoint.y - nItemHeightEx * 2;
						nXScreenPos=m_ControlPoint.x + nItemWidthEx + nItemWidth;
						m_SparrowResource.m_ImageTableTop.DrawSparrowItem(pDC,GetSparrowData(0),nXScreenPos,nYScreenPos);

						break;

				}
			}
		}
	}
}
include "../../../Memory.as"