package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.GameModule.Common.BaseUserInfoView;
	import org.god.SkinControl.SkinImage;
	import org.god.Common.DrawUtil;
	
	public class UserInfoView extends BaseUserInfoView
	{
		public function UserInfoView()
		{
			super(new SkinImage(IMAGE_UserInfoView,246,60));
		}
		//调整控件
		override protected function RectifyUserInfoView(nWidth:int,nHeight:int):void
		{
			/*m_ptUserName.x = 66;
			m_ptUserName.y = 38;
			m_ptScore.x = 66;
			m_ptScore.y = 38;*/
			m_ptUserName.x = 66;
			m_ptUserName.y = 38;
			m_ptScore.x = 178;
			m_ptScore.y = 38;
		}
		//重画函数
		/*override protected function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			m_bmBackGround.DrawBitmapData(dc,0,0);
			
			DrawUtil.TextOut(dc,
					 m_ptUserName.x,
					 m_ptUserName.y,
					 m_strName,
				 	 m_nFontSize,
					 m_nFontColor,0,0, "center");
			
			DrawUtil.TextOut(dc,
					 m_ptScore.x,
					 m_ptScore.y,
					 m_fScore.toFixed(2) + "分",
				 	 m_nFontSize,
					 m_nFontColor,0,0, "center");
		}*/
	}
}