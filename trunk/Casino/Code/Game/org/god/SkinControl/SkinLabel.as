package org.god.SkinControl
{
	import org.aswing.JLabel;
	import org.aswing.geom.IntRectangle;
	
	public class SkinLabel extends JLabel
	{
		public function SkinLabel(strLabel:String,xp:int,yp:int,cx:int=60,cy:int = 20)
		{
			super();
			setText(strLabel);
			setComBounds(new IntRectangle(xp, yp, cx, cy));
		}
	}
}
