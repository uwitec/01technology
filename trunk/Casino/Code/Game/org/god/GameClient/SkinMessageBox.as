package
{
	import org.god.SkinControl.BaseSkinMessageBox;
	
	import org.aswing.ASColor;
	//信息对话框
	public class SkinMessageBox extends BaseSkinMessageBox
	{
		//构造函数
		public function SkinMessageBox(strMsg:String,bSplitMsg:Boolean = true)
		{
			super(strMsg, new IMAGE_SKINMESSAGEBOX(345,255),bSplitMsg);
			
			
			var MyFontColor:ASColor = new ASColor(0xf1e8a5);
			m_labelMessage.setForeground(MyFontColor);
		}
		//显示信息对话框
		public static function DoModalMessageBox(strMsg:String,bSplitMsg:Boolean = true):void
		{
			var _mySelf:SkinMessageBox = new SkinMessageBox(strMsg,bSplitMsg);
			_mySelf.Center();
			_mySelf.DoModal();
		}
	}
}
