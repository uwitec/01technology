package 
{
	import org.god.GameModule.Common.BaseUserInfoView;
	import org.god.SkinControl.SkinImage;
	
	public class UserInfoView extends BaseUserInfoView
	{
		public function UserInfoView()
		{
			super(new SkinImage(IMAGE_UserInfoView,246,60));
		}
		//调整控件
		override protected function RectifyUserInfoView(nWidth:int,nHeight:int):void
		{
			m_ptUserName.x = 66;
			m_ptUserName.y = 38;
			m_ptScore.x = 178;
			m_ptScore.y = 38;
		}
	}
}