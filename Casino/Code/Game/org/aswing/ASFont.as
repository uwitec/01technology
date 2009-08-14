/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing
{
	
import flash.text.TextFormat;
import flash.text.TextField;

/**
 * Font that specified the font name, size, style and whether or not embed.
 * @author iiley
 */
public class ASFont
{
 	private var name:String;
 	private var size:uint;
 	private var bold:Boolean;
 	private var italic:Boolean;
 	private var underline:Boolean;
 	private var embedFonts:Boolean;
 	private var textFormat:TextFormat;
 	
 	/**
 	 * Create a font.
 	 */
	public function ASFont(name:String="Tahoma", size:Number=11, bold:Boolean=false, italic:Boolean=false, underline:Boolean=false, embedFonts:Boolean=false){
		this.name = name;
		this.size = size;
		this.bold = bold;
		this.italic = italic;
		this.underline = underline;
		this.embedFonts = embedFonts;
		textFormat = getTextFormat();
	}
	
	public function getName():String{
		return name;
	}
	
	public function changeName(name:String):ASFont{
		return new ASFont(name, size, bold, italic, underline, embedFonts);
	}
	
	public function getSize():uint{
		return size;
	}
	
	public function changeSize(size:int):ASFont{
		return new ASFont(name, size, bold, italic, underline, embedFonts);
	}
	
	public function isBold():Boolean{
		return bold;
	}
	
	public function changeBold(bold:Boolean):ASFont{
		return new ASFont(name, size, bold, italic, underline, embedFonts);
	}
	
	public function isItalic():Boolean{
		return italic;
	}
	
	public function changeItalic(italic:Boolean):ASFont{
		return new ASFont(name, size, bold, italic, underline, embedFonts);
	}
	
	public function isUnderline():Boolean{
		return underline;
	}
	
	public function changeUnderline(underline:Boolean):ASFont{
		return new ASFont(name, size, bold, italic, underline, embedFonts);
	}
	
	public function isEmbedFonts():Boolean{
		return embedFonts;
	}
	
	/**
	 * Applys the font to the specified text field.
	 * @param textField the text filed to be applied font.
	 * @param beginIndex The zero-based index position specifying the first character of the desired range of text. 
	 * @param endIndex The zero-based index position specifying the last character of the desired range of text. 
	 */
	public function apply(textField:TextField, beginIndex:int=-1, endIndex:int=-1):void{
		textField.setTextFormat(textFormat, beginIndex, endIndex);
		textField.defaultTextFormat = textFormat;
		textField.embedFonts = embedFonts;
	}
	
	/**
	 * Return a new text format that contains the font properties.
	 * @return a new text format.
	 */
	public function getTextFormat():TextFormat{
		return new TextFormat(name, size, null, bold, italic, underline);
	}
}

}