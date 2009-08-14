package org.god.SkinControl
{
	import org.aswing.JLabel;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	
	public class PGoldTextField extends JLabel
	{
		public function PGoldTextField(strText:String = "",bCenter:Boolean = false)
		{
			super(strText, null, bCenter ? JLabel.CENTER:JLabel.LEFT);

			var col:ASColor = new ASColor(0xF0F0F0);
			var font:ASFont = new ASFont;
			font = font.changeSize(12);
			
			this.setForeground(col);
			this.setFont(font);
		}
	}
}