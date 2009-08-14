package
{
	import org.god.SkinControl.ToggleImageButton;
	//导航按钮
	class NavCmdButton extends ToggleImageButton
	{
		public var m_nNavCmd:uint;
		
		public function NavCmdButton(nNavCmd:uint,
									 bmc:Class,
									bmw:Number, bmh:Number)
		{
			super(bmc,bmw,bmh);
			m_nNavCmd = nNavCmd;
		}
		
		public function GetNavCmd():uint
		{
			return m_nNavCmd;
		}
	}
}