package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;	
	
	import org.god.GameModule.Common.BaseScoreView;
	import org.god.SkinControl.SkinImage;
	import org.god.Common.DrawUtil;
	
	public class ScoreView extends BaseScoreView
	{
		//数值掩码
		public static  var CARD_MASK_COLOR:uint=0xF0;//花色掩码
		public static  var CARD_MASK_VALUE:uint=0x0F;//数值掩码
		
		public var m_wGameScore:uint;//游戏得分
		public var m_wConcealTime:uint;//扣底倍数
		public var m_wConcealScore:uint;//底牌积分
		public var m_cbConcealCount:uint;//暗藏数目
		public var m_cbConcealCard:Array = new Array(8);//暗藏扑克
		public var m_ImageCard:SkinImage;
		public var m_CardSize:Point=new Point  ;//扑克大小
		
		public function ScoreView()
		{
			super(new SkinImage(IMAGE_GAMEEND,293,260),4, 16);
			m_ImageCard = new SkinImage(IMAGE_CARD);
			//获取大小
			m_CardSize.x=m_ImageCard.GetWidth()/13;
			m_CardSize.y=m_ImageCard.GetHeight()/5;
		}
		override protected function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);
			//绘画扑克
			if (m_cbConcealCount>0)
			{
				//计算大小
				var ControlSize:Point = new Point;
				ControlSize.y=m_CardSize.y;
				ControlSize.x=m_CardSize.x+(m_cbConcealCount-1)*18;
		
				//定义位置
				var nXPos:Number=(rcClient.width-ControlSize.x)/2;
				var nYPos:Number=18;
		
				//绘画扑克
				for (var i:uint=0;i<m_cbConcealCount;i++)
				{
					//变量定义
					var nXImagePos:Number=0;
					var nYImagePos:Number=0;
		
					//图片位置
					if (m_cbConcealCard[i]!=0)
					{
						if ((m_cbConcealCard[i]==0x4E)||(m_cbConcealCard[i]==0x4F))
						{
							nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.y;
							nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)%14)*m_CardSize.x;
						}
						else
						{
							nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)-1)*m_CardSize.x;
							nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.y;
						}
					}
					else
					{
						nXImagePos=m_CardSize.x*2;
						nYImagePos=m_CardSize.y*4;
					}
		
					//绘画扑克
					m_ImageCard.DrawBitmapData(dc,
					nXPos+i*18,nYPos,
					nXImagePos,nYImagePos,
					m_CardSize.x,m_CardSize.y);
				}
			}
			
			var szBuffer:String = "遊戲得分 " + Number(m_wGameScore).toString() + 
			" +  底牌分 " + Number(m_wConcealScore).toString() + " X " +
			Number(m_wConcealTime).toString() + " 倍 = " + 
			Number(m_wGameScore+m_wConcealScore*m_wConcealTime).toString();	
			
			DrawUtil.TextOut(dc,
					 5,129,
					 szBuffer,
				 	 15,
					 0xffffffff,280,17, "center");
			
		}
		//调整控件
		override protected function RectifyScoreView(nWidth:int,nHeight:int):void
		{
			for(var i:uint = 0; i < m_ptChairID.length; i ++)
			{
				m_ptChairID[i].x = 96;
				m_ptChairID[i].y = 187 + i * 19;
				m_ptScore[i].x = 198;
				m_ptScore[i].y = 187 + i * 19;
			}
		}

	}
}