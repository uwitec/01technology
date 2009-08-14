package
{
	import org.god.GameModule.Common.BaseScoreView;
	import org.god.SkinControl.SkinImage;
	//得分视图
	public class ScoreView extends BaseScoreView
	{
		//构造函数
		public function ScoreView()
		{
			super(new SkinImage(IMAGE_GAMEEND,306,272),6,18);
		}
		//调整控件
		override protected function RectifyScoreView(nWidth:int,nHeight:int):void
		{
			for(var i:uint = 0; i < m_ptChairID.length; i ++)
			{
				m_ptChairID[i].x = 67;
				m_ptChairID[i].y = 90 + i * 28;
				m_ptScore[i].x = 196;
				m_ptScore[i].y = 88 + i * 28;
			}
		}
	}
}