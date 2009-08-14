package org.god.SkinControl
{
	import org.aswing.Icon;
	
	public class NavButton extends SkinButton
	{
		private var m_mcBindData:* = null;

		public function NavButton(strLabel:String,
		  xp:int,yp:int,
		  icon:Icon=null,
		   cx:int=92,cy:int=36)
		{
			super(strLabel,xp,yp,icon,cx,cy);
			setEnabled(false);
		}

		public function Deactive():void
		{

		}
		public function Active():void
		{

		}
		public function SetBindData(mcBindData:*):void
		{
			m_mcBindData = mcBindData;
			if (m_mcBindData)
			{
				setEnabled(true);
			}
			else
			{
				setEnabled(false);
			}
		}
		public function GetBindData():*
		{
			return m_mcBindData;
		}
	}

}

include "../GlobalConst.as"