package 
{
	import flash.display.MovieClip;
	import flash.display.BitmapData;
	import flash.geom.Point;
	
	import org.god.GameModule.Common.enDirection;

	//用户麻将
	public class UserSparrow
	{
		//麻将数据
		protected var m_wSparrowCount:uint;//麻将数目
		protected var m_bCurrentSparrow:Boolean;//当前标志

		//控制变量
		protected var m_ControlPoint:Point=new Point  ;//基准位置
		protected var m_SparrowDirection:uint;//麻将方向

		protected var m_SparrowResource:SparrowResource;

		public function UserSparrow(res:SparrowResource)
		{
			m_SparrowResource=res;
			//麻将数据
			m_wSparrowCount=0;
			m_bCurrentSparrow=false;

			//控制变量
			m_SparrowDirection=enDirection.Direction_East;
		}
		static public function Destroy(data:UserSparrow):void
		{
			if(data)
			{
				data.m_SparrowResource = null;
				data.m_ControlPoint = null;
				
			}
		}
		//设置麻将
		public function SetCurrentSparrow(bCurrentSparrow:Boolean):Boolean
		{
			//设置变量
			m_bCurrentSparrow=bCurrentSparrow;
			
			return true;
		}
		//设置麻将
		public function SetSparrowData(wSparrowCount:uint,bCurrentSparrow:Boolean):Boolean
		{

			//设置变量
			m_wSparrowCount=wSparrowCount;
			m_bCurrentSparrow=bCurrentSparrow;
			
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
						//当前麻将
						if (m_bCurrentSparrow == true)
						{
							var nXPos:Number=m_ControlPoint.x;
							var nYPos:Number=m_ControlPoint.y;
							m_SparrowResource.m_ImageUserRight.DrawBitmapData(pDC,nXPos,nYPos);
						}
						//正常麻将
						if (m_wSparrowCount > 0)
						{
							var nXPos:Number=0;
							var nYPos:Number=0;
							for (var i:uint=0; i < m_wSparrowCount; i++)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y + i * 25 + 40;
								m_SparrowResource.m_ImageUserRight.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						break;

					};
				case enDirection.Direction_West ://西向
					{
						//正常麻将
						if (m_wSparrowCount > 0)
						{
							var nXPos:Number=0;
							var nYPos:Number=0;
							for (var i:uint=0; i < m_wSparrowCount; i++)
							{
								nXPos=m_ControlPoint.x;
								nYPos=m_ControlPoint.y - (m_wSparrowCount - i - 1 )* 25 - 92;
								m_SparrowResource.m_ImageUserLeft.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						//当前麻将
						if (m_bCurrentSparrow == true)
						{
							var nXPos:Number=m_ControlPoint.x;
							var nYPos:Number=m_ControlPoint.y - 49;
							m_SparrowResource.m_ImageUserLeft.DrawBitmapData(pDC,nXPos,nYPos);
						}
						break;

					};
				case enDirection.Direction_North ://北向
					{
						//当前麻将
						if (m_bCurrentSparrow == true)
						{
							var nXPos:Number=m_ControlPoint.x;
							var nYPos:Number=m_ControlPoint.y;
							m_SparrowResource.m_ImageUserTop.DrawBitmapData(pDC,nXPos,nYPos);
						}
						//正常麻将
						if (m_wSparrowCount > 0)
						{
							var nXPos:Number=0;
							var nYPos:Number=0;
							for (var i:uint=0; i < m_wSparrowCount; i++)
							{
								nYPos=m_ControlPoint.y;
								nXPos=m_ControlPoint.x + i * 25 + 40;
								m_SparrowResource.m_ImageUserTop.DrawBitmapData(pDC,nXPos,nYPos);
							}
						}
						break;

				}
			}

		}
	}
}