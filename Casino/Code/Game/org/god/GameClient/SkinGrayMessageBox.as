package
{
	import org.god.SkinControl.BaseSkinMessageBox;
	//灰色信息对话框
	public class SkinGrayMessageBox extends BaseSkinMessageBox
	{
		private static  var _mySelf:SkinGrayMessageBox = null;
		//构造函数
		public function SkinGrayMessageBox(strMsg:String)
		{
			super(strMsg, new IMAGE_GRAYSKINMESSAGEBOX(368,170));
			m_btnOK.setVisible(false);
		}
		
		/////////////////////////////////////////////////////
		//显示信息对话框
		public static function DoModalMessageBox(strMsg:String):void
		{
			if (_mySelf == null)
			{
				_mySelf = new SkinGrayMessageBox(strMsg);
				_mySelf.Center();
				_mySelf.DoModal();
				return;
			}
			_mySelf.setMessage(strMsg);
		}
		//隐藏信息对话框
		public static function HideMessageBox():void
		{
			if (_mySelf == null)
			{
				return;
			}
			_mySelf.OnEventOK(null);
			_mySelf = null;
		}
	}
}
