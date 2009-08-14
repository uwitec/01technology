package org.god.SkinControl
{
	import org.aswing.ASFont;
	import org.aswing.Icon;
	import org.aswing.AbstractButton;
	import org.aswing.DefaultButtonModel;
	import org.aswing.geom.IntRectangle;
	import org.aswing.UIManager;
	import org.aswing.plaf.basic.BasicButtonUI;
	import org.aswing.plaf.basic.SkinButtonUI;
	
	public class SkinButton extends AbstractButton
	{
		protected static var MyFont:ASFont;
		
		public function SkinButton(strLabel:String,
		xp:int = 1,yp:int = 1, 
		icon:Icon=null,
		   cx:int=82,cy:int=32)
		{
			super(strLabel,icon);
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 //MyFont = MyFont.changeBold(true);
			}
			setName("SkinButton");
			setModel(new DefaultButtonModel());
			setComBounds(new IntRectangle(xp, yp,cx, cy));
			setFont(MyFont);
		}

		override public function updateUI():void
		{
			setUI(UIManager.getUI(this));
		}
		override public function getDefaultBasicUIClass():Class
		{
			return org.aswing.plaf.basic.SkinButtonUI;
		}
		override public function getUIClassID():String
		{
			return "SkinButtonUI";
		}
	}

}