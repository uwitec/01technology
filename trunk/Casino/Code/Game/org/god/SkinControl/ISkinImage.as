package org.god.SkinControl
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.ColorTransform;
	
	public interface ISkinImage
	{
		function Load(c:Class,width:int, height:int):Boolean;
		function Unload():void;
		function GetBitmapData():BitmapData;
		function GetBitmap():Bitmap;
	    function GetWidth():Number;
		function GetHeight():Number;
		
		function DrawBitmapData(b:BitmapData,
					nDestXPos:Number=0,
					nDestYPos:Number=0,
					nSrcXPos:Number=0,
					nSrcYPos:Number=0,
					nSrcWidth:Number=-1,
					nSrcHeight:Number=-1):void ;
		function DrawGraphics(g:Graphics,
					nDestXPos:Number=0,
					nDestYPos:Number=0,
					nSrcXPos:Number=0,
					nSrcYPos:Number=0,
					nSrcWidth:Number=-1,
					nSrcHeight:Number=-1,
					smoothing:Boolean = true):void 	;
	}
}