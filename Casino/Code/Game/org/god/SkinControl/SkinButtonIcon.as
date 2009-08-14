package org.god.SkinControl
{
	import org.aswing.graphics.Graphics2D;
	import org.aswing.Icon;
	import org.aswing.Component;
	import org.aswing.AbstractButton;
	import org.aswing.ButtonModel;
	import org.aswing.plaf.UIResource;
	import org.aswing.plaf.ComponentUI;
	import org.aswing.error.ImpMissError;
	import flash.display.DisplayObject;

	public class SkinButtonIcon implements Icon,UIResource
	{

		private var forceWidth:int = -1;
		private var forceHeight:int = -1;

		protected var stateAsset:ButtonStateObject;
		protected var fixedPrefix:String;
		protected var setuped:Boolean;

		public function SkinButtonIcon(forceWidth:int=-1, forceHeight:int=-1, fixedPrefix:String=null)
		{
			this.forceWidth = forceWidth;
			this.forceHeight = forceHeight;
			setuped = false;
			this.fixedPrefix = fixedPrefix;
			stateAsset = new ButtonStateObject();
		}
		protected function getPropertyPrefix():String
		{
			if (fixedPrefix != null)
			{
				return fixedPrefix;
			}
			else
			{
				throw new ImpMissError  ;
				return null;
			}
		}
		protected function setupAssets(ui:ComponentUI):void
		{
			var pp:String = getPropertyPrefix();
			var defaultImage:DisplayObject  = ui.getInstance(pp+"defaultImage") as DisplayObject;
			var pressedImage:DisplayObject  = ui.getInstance(pp+"pressedImage") as DisplayObject;
			var pressedSelectedImage:DisplayObject  = ui.getInstance(pp+"pressedSelectedImage") as DisplayObject;
			var disabledImage:DisplayObject = ui.getInstance(pp+"disabledImage") as DisplayObject;
			var selectedImage:DisplayObject         = ui.getInstance(pp+"selectedImage") as DisplayObject;
			var disabledSelectedImage:DisplayObject = ui.getInstance(pp+"disabledSelectedImage") as DisplayObject;
			var rolloverImage:DisplayObject         = ui.getInstance(pp+"rolloverImage") as DisplayObject;
			var rolloverSelectedImage:DisplayObject = ui.getInstance(pp+"rolloverSelectedImage") as DisplayObject;

			stateAsset.setDefaultImage(defaultImage);
			stateAsset.setPressedImage(pressedImage);
			stateAsset.setPressedSelectedImage(pressedSelectedImage);
			stateAsset.setDisabledImage(disabledImage);
			stateAsset.setSelectedImage(selectedImage);
			stateAsset.setDisabledSelectedImage(disabledSelectedImage);
			stateAsset.setRolloverImage(rolloverImage);
			stateAsset.setRolloverSelectedImage(rolloverSelectedImage);
		}
		public function checkSetup(c:Component):void
		{
			if (!setuped)
			{
				setupAssets(c.getUI());
				setuped = true;
			}
		}
		public function getDisplay(c:Component):DisplayObject
		{
			checkSetup(c);
			return stateAsset;
		}
		public function updateIcon(c:Component, g:Graphics2D, x:int, y:int):void
		{
			checkSetup(c);
			var button:AbstractButton = AbstractButton(c);
			var model:ButtonModel = button.getModel();
			stateAsset.setEnabled(model.isEnabled());
			stateAsset.setPressed(model.isPressed() && model.isArmed());
			stateAsset.setSelected(model.isSelected());
			stateAsset.setRollovered(button.isRollOverEnabled() && model.isRollOver());
			stateAsset.x = x;
			stateAsset.y = y;
			stateAsset.updateRepresent();
		}
		public function getIconHeight(c:Component):int
		{
			checkSetup(c);
			if (forceHeight >= 0)
			{
				return forceHeight;
			}
			else
			{
				return stateAsset.height;
			}
		}
		public function getIconWidth(c:Component):int
		{
			checkSetup(c);
			if (forceWidth >= 0)
			{
				return forceWidth;
			}
			else
			{
				return stateAsset.width;
			}
		}
	}
}