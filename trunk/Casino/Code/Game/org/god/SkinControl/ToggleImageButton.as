package org.god.SkinControl
{
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.Bitmap;

	public class ToggleImageButton extends SkinSimpleButton
	{
		private var m_image:SkinImage;
		private var m_btoggle:Boolean;
		private var m_toggle:DisplayObject;

		public function ToggleImageButton(bmc:Class,
		bmw:Number, bmh:Number)
		{
			m_btoggle = false;
			
			m_image = new SkinImage(bmc, bmw, bmh);
			var cx:Number = bmw / 5.0;

			var up:DisplayObject  = GetButtonSkin(m_image, 0, 0, cx, bmh);
			var over:DisplayObject= GetButtonSkin(m_image, cx, 0, cx, bmh);
			var down:DisplayObject= GetButtonSkin(m_image, cx* 2.0, 0, cx, bmh);
			var disabled:DisplayObject= GetButtonSkin(m_image, cx * 4.0, 0, cx, bmh);

			m_toggle= GetButtonSkin(m_image, cx* 3.0, 0, cx, bmh);
			
			super(up, over, down, disabled, cx, bmh);
		}
		public function SetButtonImage(bmc:Class,
		bmw:Number, bmh:Number):void
		{
			m_image = new SkinImage(bmc, bmw, bmh);
			var cx:Number = bmw / 5.0;

			var up:DisplayObject  = GetButtonSkin(m_image, 0, 0, cx, bmh);
			var over:DisplayObject= GetButtonSkin(m_image, cx, 0, cx, bmh);
			var down:DisplayObject= GetButtonSkin(m_image, cx * 2.0, 0, cx, bmh);
			m_toggle =GetButtonSkin(m_image, cx * 3.0, 0, cx, bmh);
			var disabled:DisplayObject= GetButtonSkin(m_image, cx * 4.0, 0, cx, bmh);

			SetButtonSkin(up, over, down, disabled, cx, bmh);
		}
		public function SetToggle(bToggle:Boolean):void
		{
			if(m_btoggle == bToggle)
				return;
			m_btoggle = bToggle;
			if(bToggle)
			{
				this.upState =  m_toggle;
			}
			else
			{
				this.upState =  m_up;
			}
		}

		private function GetButtonSkin(image:SkinImage,
		   xp:Number,
		   yp:Number,
		   cx:Number,
		   cy:Number):Bitmap
		{
			var bmd:BitmapData = new BitmapData(cx, cy);
			image.DrawBitmapData(bmd, 0, 0, xp, yp, cx, cy);

			var bm:Bitmap = new Bitmap(bmd, "auto", true);
			return bm;
		}
	}
}