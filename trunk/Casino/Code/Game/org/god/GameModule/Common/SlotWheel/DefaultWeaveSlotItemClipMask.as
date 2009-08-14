package org.god.GameModule.Common.SlotWheel
{
	import flash.display.Sprite;
	import flash.geom.Rectangle;
	
	public class DefaultWeaveSlotItemClipMask extends Sprite
	{
		public function DefaultWeaveSlotItemClipMask(rect:Rectangle)
		{
			super();
			this.graphics.beginFill(0xFF0000);
			this.graphics.drawRect(rect.x, 
									 rect.y, 
									 rect.width, 
									 rect.height);
			this.graphics.endFill();
		}
		public function MoveWindow(xp:int,yp:int):void
		{
			this.x = xp;
			this.y = yp;
		}
	}
}