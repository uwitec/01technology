package org.god.SkinControl
{
	import org.aswing.plaf.UIResource;
	import org.aswing.plaf.ComponentUI;
	import org.aswing.plaf.UIResource;
	import org.aswing.AbstractButton;
	import org.aswing.GroundDecorator;
	import org.aswing.ButtonModel;
	import org.aswing.JButton;
	import org.aswing.Component;
	import org.aswing.GroundDecorator;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.Graphics2D;
	import flash.display.DisplayObject;

	public class SkinButtonBackground implements GroundDecorator,UIResource
	{

		protected var stateAsset:ButtonStateObject;
		protected var setuped:Boolean;
		protected var fixedPrefix:String;

		public function SkinButtonBackground(fixedPrefix:String=null)
		{
			setuped = false;
			this.fixedPrefix = fixedPrefix;
			stateAsset = new ButtonStateObject();
		}
		public function getStateAsset():ButtonStateObject
		{
			return stateAsset;
		}
		protected function getPropertyPrefix():String
		{
			if (fixedPrefix != null)
			{
				return fixedPrefix;
			}
			return "SkinButton.";
		}
		protected function setupAssets(ui:ComponentUI):void
		{
			stateAsset.setDefaultButtonImage(getAsset(ui, "DefaultButton.defaultImage"));
			stateAsset.setDefaultImage(getAsset(ui, "defaultImage"));
			stateAsset.setPressedImage(getAsset(ui, "pressedImage"));
			stateAsset.setPressedSelectedImage(getAsset(ui, "pressedSelectedImage"));
			stateAsset.setDisabledImage(getAsset(ui, "disabledImage"));
			stateAsset.setSelectedImage(getAsset(ui, "selectedImage"));
			stateAsset.setDisabledSelectedImage(getAsset(ui, "disabledSelectedImage"));
			stateAsset.setRolloverImage(getAsset(ui, "rolloverImage"));
			stateAsset.setRolloverSelectedImage(getAsset(ui, "rolloverSelectedImage"));
		}
		private function getAsset(ui:ComponentUI, extName:String):DisplayObject
		{
			var pp:String = getPropertyPrefix();
			return ui.getInstance(pp + extName)  as  DisplayObject;
		}
		public function getDisplay(c:Component):DisplayObject
		{
			return stateAsset;
		}
		public function updateDecorator(com:Component, g:Graphics2D, bounds:IntRectangle):void
		{
			if (!setuped)
			{
				setupAssets(com.getUI());
				setuped = true;
			}
			var button:AbstractButton = AbstractButton(com);
			var model:ButtonModel = button.getModel();
			stateAsset.setEnabled(model.isEnabled());
			stateAsset.setPressed(model.isPressed() && model.isArmed());
			stateAsset.setSelected(model.isSelected());
			stateAsset.setRollovered(button.isRollOverEnabled() && model.isRollOver());
			stateAsset.setDefaultButton(button is JButton && JButton(button).isDefaultButton());
			stateAsset.updateRepresent(bounds);
		}
	}
}