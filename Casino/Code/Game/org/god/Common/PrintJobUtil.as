package org.god.Common
{
	import flash.printing.PrintJob;
	import flash.printing.PrintJobOptions;
	import flash.printing.PrintJobOrientation;
	import flash.display.Sprite;
	import flash.display.BitmapData;
	import flash.display.Bitmap;
	import flash.geom.Rectangle;
	import flash.text.TextField;

	import org.aswing.graphics.Graphics2D;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.BitmapBrush;
	//打印辅助
	public class PrintJobUtil
	{
		//构造函数
		public function PrintJobUtil()
		{
		}
		//打印影片
		public static  function PrintSprite(sp:Sprite,x:int,y:int,w:int,h:int):Boolean
		{
			if(w == 0 || h == 0)
				return false;
				
			var pj:PrintJob = new PrintJob();
            var pagesToPrint:uint = 0;
            if (pj.start())
            {
                try
                {
					var fScale:Number = w / pj.pageWidth;
					var fOldScaleY:Number = sp.scaleY;
					var fOldScaleX:Number = sp.scaleX;
					sp.scaleY = sp.scaleY / fScale;
					sp.scaleX = sp.scaleX / fScale;
                    pj.addPage(sp, null, new PrintJobOptions(true));
					sp.scaleY = fOldScaleY;
					sp.scaleX = fOldScaleX;
                    pagesToPrint++;
                }
                catch (error:Error)
                {
               		return false;
                }

                if (pagesToPrint > 0)
                {
                    pj.send();
                }
				return true;
            }
			else
			{
				return false;
			}
		}
	}
}