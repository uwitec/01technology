/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.border{

import org.aswing.graphics.Graphics2D;
import org.aswing.Border;
import org.aswing.geom.IntRectangle;
import org.aswing.Component;
import org.aswing.Insets;
import flash.display.DisplayObject;

/**
 * EmptyBorder not draw any graphics, only use to hold a blank space around component.
 * @author iiley
 */
public class EmptyBorder extends DecorateBorder{
	
	private var margin:Insets;
	
	public function EmptyBorder(interior:Border=null, margin:Insets=null){
		super(interior);
		if(margin == null){
			this.margin = new Insets();
		}else{
			this.margin = margin.clone();
		}
	}
	
	public static function createIndent(indent:int):EmptyBorder{
		return new EmptyBorder(null, new Insets(indent, indent, indent, indent));
	}
	
	override public function updateBorderImp(com:Component, g:Graphics2D, bounds:IntRectangle):void{
	}
	
    override public function getBorderInsetsImp(c:Component, bounds:IntRectangle):Insets{
    	return margin.clone();
    }
	
}
}