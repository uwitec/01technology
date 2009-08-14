package org.god.SkinControl
{
	import org.aswing.graphics.Graphics2D;
	import org.aswing.geom.IntRectangle;
	import org.aswing.GroundDecorator;
	import org.aswing.Component;
	import org.aswing.AsWingUtils;
	import org.aswing.EditableComponent;
	import org.aswing.plaf.UIResource;
	import org.aswing.plaf.ComponentUI;
	import org.aswing.error.ImpMissError;
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	
	public class SkinAbsEditorBackground implements GroundDecorator,UIResource
	{

		protected var imageContainer:Sprite;
		protected var defaultImage:DisplayObject;
		protected var uneditableImage:DisplayObject;
		protected var disabledImage:DisplayObject;
		protected var lastViewedImage:DisplayObject;

		protected var loaded:Boolean;

		public function SkinAbsEditorBackground()
		{
			imageContainer = AsWingUtils.createSprite(null, "imageContainer");
			imageContainer.mouseChildren = false;
			imageContainer.mouseEnabled = false;
			loaded = false;
		}
		protected function getPropertyPrefix():String
		{
			throw new ImpMissError  ;
			return null;
		}
		protected function isEditable(c:Component):Boolean
		{
			var text:EditableComponent = EditableComponent(c);
			return text.isEditable();
		}
		protected function reloadAssets(ui:ComponentUI):void
		{
			var pp:String = getPropertyPrefix();
			addImage(defaultImage = ui.getInstance(pp+"defaultImage") as DisplayObject);
			addImage(uneditableImage = ui.getInstance(pp+"uneditableImage") as DisplayObject);
			addImage(disabledImage = ui.getInstance(pp+"disabledImage") as DisplayObject);
			defaultImage.visible = true;
			lastViewedImage = defaultImage;
		}
		protected function addImage(image:DisplayObject):void
		{
			if (image)
			{
				image.visible = false;
				imageContainer.addChild(image);
			}
		}
		public function updateDecorator(c:Component, g:Graphics2D, bounds:IntRectangle):void
		{
			if (!loaded)
			{
				reloadAssets(c.getUI());
				loaded = true;
			}
			var image:DisplayObject = null;
			if (!c.isEnabled())
			{
				image = disabledImage;
			}
			else if (!isEditable(c))
			{
				image = uneditableImage;
			}
			if (image == null)
			{
				image = defaultImage;
			}
			if (lastViewedImage != image)
			{
				lastViewedImage.visible = false;
				lastViewedImage = image;
				lastViewedImage.visible = true;
			}
			//not use bounds, avoid the border
			lastViewedImage.width = c.width;
			lastViewedImage.height = c.height;
		}
		public function getDisplay(c:Component):DisplayObject
		{
			return imageContainer;
		}
	}
}