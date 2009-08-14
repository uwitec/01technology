package org.god.SkinControl
{
	import flash.display.DisplayObject;
	import flash.display.Shape;
	import flash.display.BitmapData;
	
	import org.aswing.graphics.Graphics2D;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.BitmapBrush;
	import org.aswing.GroundDecorator;
	import org.aswing.Component;
		
	public class BitmapBackground implements GroundDecorator
	{
		private var m_bmBackground:BitmapData;
		private var shape:Shape;

		public function BitmapBackground(bmBackground:BitmapData)
		{
			this.m_bmBackground = bmBackground;
			shape = new Shape();
		}
		public function updateDecorator(c:Component, g:Graphics2D, bounds:IntRectangle):void
		{
			shape.graphics.clear();
			g = new Graphics2D(shape.graphics);
			var bBrush:BitmapBrush = new BitmapBrush(m_bmBackground, null, false, true);	
			g.fillRectangle(bBrush, bounds.x, bounds.y, bounds.width, bounds.height);
		}
		public function getDisplay(c:Component):DisplayObject
		{
			return shape;
		}
	}
}