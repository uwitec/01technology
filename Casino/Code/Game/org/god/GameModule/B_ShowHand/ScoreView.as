package
{
	import org.god.GameModule.Common.BaseScoreView;
	import org.god.SkinControl.SkinImage;
	
	public class ScoreView extends BaseScoreView
	{
		public function ScoreView()
		{
			super(new SkinImage(IMAGE_GAMEEND,385,272),4,22);
		}
		//调整控件
		override protected function RectifyScoreView(nWidth:int,nHeight:int):void
		{
			for(var i:uint = 0; i < m_ptChairID.length; i ++)
			{
				m_ptChairID[i].x = 90;
				m_ptChairID[i].y = 113 + i * 32;
				m_ptScore[i].x = 225;
				m_ptScore[i].y = 113 + i * 32;
			}
		}
	}
}