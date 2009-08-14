/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.tree { 

import org.aswing.Component;
import org.aswing.geom.IntRectangle;
import org.aswing.graphics.*;
import org.aswing.Icon;
import org.aswing.plaf.UIResource;
import flash.display.DisplayObject;
import flash.geom.Matrix;

/**
 * The default leaf icon for JTree.
 * TODO draw a nicer icon
 * @author iiley
 */
public class TreeLeafIcon implements Icon, UIResource{
	
	public function TreeLeafIcon(){
	}
	
	public function getIconWidth(c:Component) : int {
		return 16;
	}

	public function getIconHeight(c:Component) : int {
		return 16;
	}

	public function updateIcon(com:Component, g:Graphics2D, x:int, y:int):void{
		var b:IntRectangle = new IntRectangle(0, 0, 16, 16);
		b.grow(-4, -4);
		b.move(x, y);
		
        var colors:Array = [0xFFFFFF, 0x257434];
		var alphas:Array = [100/255, 100/255];
		var ratios:Array = [0, 255];
		var matrix:Matrix = new Matrix();
		matrix.createGradientBox(b.width+2, b.height+2, 0, b.x-2, b.y-2);      
        var brush:GradientBrush = new GradientBrush(GradientBrush.RADIAL, colors, alphas, ratios, matrix);
        g.fillEllipse(brush, b.x, b.y, b.width, b.height);
	}

	public function getDisplay(c:Component):DisplayObject{
		return null;
	}
}
}