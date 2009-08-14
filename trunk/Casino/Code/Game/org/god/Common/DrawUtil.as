package org.god.Common
{
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.geom.Rectangle;
	
	import org.aswing.geom.IntDimension;
	import org.aswing.AsWingUtils;
	//绘画辅助
	public class DrawUtil
	{
		//构造函数
		public function DrawUtil()
		{
		}
		//文本绘画
		public static function TextOut(pDC:BitmapData,
								x:Number,y:Number,
								strText:String,
								txSize:uint = 18,
								txColor:uint = 0xffffff,
								width:Number = 0, height:Number = 0,
								align:String = "left"
								):Rectangle
		{
			var s:Sprite = new Sprite;
			var textField:TextField = new TextField();
			s.addChild(textField);
			var textFormat:TextFormat = new TextFormat;
			textFormat.size  = txSize;
			textFormat.align = align;
			textFormat.bold = true;
			textField.setTextFormat(textFormat);
			textField.defaultTextFormat = textFormat;
 			textField.autoSize = align;
 			textField.selectable = false;
 			textField.mouseEnabled = false;
 			textField.mouseWheelEnabled = false;
			textField.textColor = txColor;
			if(strText != null)
				textField.text = strText;
			var textS:IntDimension = AsWingUtils.computeStringSize(textFormat, strText);
			if(align == "center")
			{
				textField.x = x + width/2 -  textS.width/2;
				textField.y = y + height/2 - textS.height/2;
			}
			else
			{
				textField.x = x;
				textField.y = y;
			}
			var rect:Rectangle = new Rectangle(textField.x,textField.y,textS.width,textS.height);

			pDC.draw(s,null,null,null,null,true);
			s.removeChild(textField);
			textField = null;
			s = null;
			textFormat = null;
			return rect;
		}
		//获取RGB
		public static function RGB(r:uint,g:uint,b:uint,a:uint = 0xff):uint
		{
			return (0x000000ff & r)|((0x000000ff & g)<<8)|((0x000000ff & b)<<16) |((0x000000ff & a)<<24);
		}
	}
}