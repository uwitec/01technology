package org.god.SkinControl
{
	import org.aswing.plaf.basic.accordion.BasicAccordionHeader;
	import org.aswing.JTabbedPane;
	import org.aswing.AbstractButton;
	import org.aswing.JButton;
	import org.aswing.plaf.basic.background.ButtonBackground;

	public class SkinTabbedPaneTab extends BasicAccordionHeader
	{

		protected var topBG:SkinButtonBackground;
		protected var bottomBG:SkinButtonBackground;
		protected var leftBG:SkinButtonBackground;
		protected var rightBG:SkinButtonBackground;
		protected var tabPlacement:int;

		public function SkinTabbedPaneTab()
		{
			super();
			tabPlacement = -1;
		}
		public function setTabPlacement(tp:int):void
		{
			if (tabPlacement != tp)
			{
				tabPlacement = tp;
				if (tp == JTabbedPane.TOP)
				{
					button.setBackgroundDecorator(topBG);
				}
				else if (tp == JTabbedPane.LEFT)
				{
					button.setBackgroundDecorator(leftBG);
				}
				else if (tp == JTabbedPane.RIGHT)
				{
					button.setBackgroundDecorator(rightBG);
				}
				else
				{
					button.setBackgroundDecorator(bottomBG);
				}
				button.repaint();
			}
		}
		override protected function createHeaderButton():AbstractButton
		{
			var tb:JButton = new JButton();
			topBG = new SkinButtonBackground(getPropertyPrefix() + "top.tab.");
			bottomBG = new SkinButtonBackground(getPropertyPrefix() + "bottom.tab.");
			leftBG = new SkinButtonBackground(getPropertyPrefix() + "left.tab.");
			rightBG = new SkinButtonBackground(getPropertyPrefix() + "right.tab.");
			tb.setBackgroundDecorator(topBG);
			return tb;
		}
		protected function getPropertyPrefix():String
		{
			return "TabbedPane.";
		}
		override public function setSelected(b:Boolean):void
		{
			button.setSelected(b);
		}
	}
}