package
{
	import org.god.GameModule.Common.BaseScoreView;
	import org.god.SkinControl.SkinImage;
	
	public class ScoreView extends BaseScoreView
	{
		public function ScoreView()
		{
			super(new SkinImage(IMAGE_GAMEEND,329,272), 3);
		}
		//调整控件
		override protected function RectifyScoreView(nWidth:int,nHeight:int):void
		{
			for(var i:uint = 0; i < m_ptChairID.length; i ++)
			{
				m_ptChairID[i].x = 64;
				m_ptChairID[i].y = 125 + i * 50;
				m_ptScore[i].x = 200;
				m_ptScore[i].y = 125 + i * 50;
			}
		}
	}
}