package org.god.SkinControl
{
	public class SkinComboBoxBackground extends SkinAbsEditorRolloverEnabledBackground
	{

		public function SkinComboBoxBackground()
		{
			super();
		}
		override protected function getPropertyPrefix():String
		{
			return "ComboBox.";
		}
	}
}