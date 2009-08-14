package org.god.SkinControl
{
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.Bitmap;

	public class ImageButton extends SkinSimpleButton
	{
		private var m_image:SkinImage;
		private var m_bm_up:Bitmap;
		private var m_bm_over:Bitmap;
		private var m_bm_down:Bitmap;
		private var m_bm_disabled:Bitmap;

		public function ImageButton(bmc:Class,
		bmw:Number, bmh:Number)
		{
			m_image = new SkinImage(bmc, bmw, bmh);
			var cx:Number = bmw / 5.0;

			m_bm_up  = GetButtonSkin(m_image, 0, 0, cx, bmh);
			m_bm_over= GetButtonSkin(m_image, cx* 3.0, 0, cx, bmh);
			m_bm_down= GetButtonSkin(m_image, cx , 0, cx, bmh);
			m_bm_disabled= GetButtonSkin(m_image, cx * 4.0, 0, cx, bmh);

			super(m_bm_up, m_bm_over, m_bm_down, m_bm_disabled, cx, bmh);
		}
		public function SetButtonImage(bmc:Class,
		bmw:Number, bmh:Number):void
		{
			Destroy();
			
			m_image = new SkinImage(bmc, bmw, bmh);
			var cx:Number = bmw / 5.0;

			m_bm_up  = GetButtonSkin(m_image, 0, 0, cx, bmh);
			m_bm_over= GetButtonSkin(m_image, cx, 0, cx, bmh);
			m_bm_down= GetButtonSkin(m_image, cx * 3.0, 0, cx, bmh);
			m_bm_disabled= GetButtonSkin(m_image, cx * 4.0, 0, cx, bmh);

			SetButtonSkin(m_bm_up, m_bm_over, m_bm_down, m_bm_disabled, cx, bmh);
		}
		public override function Destroy():void
		{
			super.Destroy();
			DestroyMe();
		}
		protected function DestroyMe():void
		{
			if(m_image)
			{
				m_image.Unload();
				m_image = null;
			}
			m_bm_up.bitmapData.dispose();
			m_bm_up = null;
			m_bm_over.bitmapData.dispose();
			m_bm_over = null;
			m_bm_down.bitmapData.dispose();
			m_bm_down = null;
			m_bm_disabled.bitmapData.dispose();
			m_bm_disabled = null;
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