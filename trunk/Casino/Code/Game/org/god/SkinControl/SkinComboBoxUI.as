package org.god.SkinControl
{
	import org.aswing.plaf.basic.BasicComboBoxUI;
	import org.aswing.JButton;
	import org.aswing.Insets;
	import org.aswing.Component;

	public class SkinComboBoxUI extends BasicComboBoxUI
	{

		public function SkinComboBoxUI()
		{
			super();
		}
		override protected function createDropDownButton():Component
		{
			var btn:JButton = new JButton();
			btn.setFocusable(false);
			btn.setBorder(null);
			btn.setOpaque(false);
			btn.setMargin(new Insets());
			btn.setBackgroundDecorator(null);
			btn.setIcon(new SkinButtonIcon(-1, -1, getPropertyPrefix()+"arrowButton."));
			return btn;
		}
	}
}