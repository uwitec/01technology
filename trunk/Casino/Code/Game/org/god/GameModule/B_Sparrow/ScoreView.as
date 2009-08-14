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
		private var m_SparrowResource:SparrowResource;
		private var m_wBankerUser:uint;
		private var m_cbChiHuSparrow:uint;
		private var m_wProvideUser:uint;
		private var m_SparrowType:Array = new Array(4);
		private var m_ptStatus:Array = new Array(4);
		private var m_ptBanker:Array = new Array(4);

		public function ScoreView(res:SparrowResource)
		{
			for(var i:uint = 0; i < m_ptStatus.length; i ++)
			{
				m_ptStatus[i] = new Point;
				m_ptBanker[i] = new Point;
			}
			super(new SkinImage(IMAGE_GAMEEND,419,322), 4, 20);
			m_SparrowResource = res;
		}
		public override function Destroy():void
		{
			ZeroArray(m_ptStatus,null);
			m_ptStatus = null;
			ZeroArray(m_ptBanker,null);
			m_ptBanker = null;
			
			super.Destroy();
		}
		public function SetBankerUser(wBankerUser:uint):void
		{
			m_wBankerUser = wBankerUser;
		}
		public function SetChiHuSparrow(cbChiHuSparrow:uint):void
		{
			m_cbChiHuSparrow = cbChiHuSparrow;
		}
		public function SetProvideUser(wProvideUser:uint):void
		{
			m_wProvideUser = wProvideUser;
		}
		public function SetSparrowType(wChairID:uint,
									   wSparrowType:uint):void
		{
			m_SparrowType[wChairID] = wSparrowType;
		}
		override protected function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);
			

			m_SparrowResource.m_ImageUserBottom.DrawSparrowItem(dc,
															 m_cbChiHuSparrow,
															 188,45);

			for(var i:uint = 0; i < m_ptStatus.length; i ++)
			{
				if(m_SparrowType[i]!=0)
				{
					DrawUtil.TextOut(dc,
					 m_ptStatus[i].x,
					 m_ptStatus[i].y,
					 IDS_HUPAI,
				 	 m_nFontSize,
					 0xffffffff,0,0, "center");
				}
				else if(m_wProvideUser==i)
				{
					DrawUtil.TextOut(dc,
					 m_ptStatus[i].x,
					 m_ptStatus[i].y,
					 IDS_FANGPAO,
				 	 m_nFontSize,
					 0xffffffff,0,0, "center");
				}
				if(m_wBankerUser==i)
				{
					DrawUtil.TextOut(dc,
					 m_ptBanker[i].x,
					 m_ptBanker[i].y,
					 IDS_YES,
				 	 m_nFontSize,
					 0xffffffff,0,0, "center");
				}
			}
		}
		//调整控件
		override protected function RectifyScoreView(nWidth:int,nHeight:int):void
		{
			for(var i:uint = 0; i < m_ptChairID.length; i ++)
			{
				m_ptChairID[i].x = 74;
				m_ptChairID[i].y = 175 + i * 32;
				m_ptScore[i].x = 184;
				m_ptScore[i].y = 175 + i * 32;
				m_ptStatus[i].x = 281;
				m_ptStatus[i].y = 175 + i * 32;
				m_ptBanker[i].x = 342;
				m_ptBanker[i].y = 175 + i * 32;
			}
		}
	}
}
include "StringTableDef.as"
include "../../Common/Memory.as"