/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.tree { 

import flash.display.DisplayObject;
import org.aswing.ASColor;
import org.aswing.Component;
import org.aswing.geom.IntRectangle;
import org.aswing.graphics.*;
import org.aswing.Icon;
import org.aswing.plaf.UIResource;
import flash.geom.Matrix;

/**
 * The default folder icon for JTree.
 * TODO draw a nicer icon
 * @author iiley
 */
public class TreeFolderIcon implements Icon, UIResource{
	
	public function TreeFolderIcon(){
	}
	
	public function getIconWidth(c:Component) : int {
		return 16;
	}

	public function getIconHeight(c:Component) : int {
		return 16;
	}

	public function updateIcon(com:Component, g:Graphics2D, x:int, y:int):void{
		var b:IntRectangle = new IntRectangle(0, 0, 16, 16);
		b.grow(-2, -2);
		b.move(x, y);
		g.drawRectangle(new Pen(ASColor.BLACK, 0.5), b.x, b.y, b.width, b.height);

    var colors:Array = [0xFFFFFB, 0xDBB303];
    //var colors:Array = [0xE6E9EE, 0x8E94BD];
		var alphas:Array = [100/255, 100/255];
		var ratios:Array = [0, 255];
		var matrix:Matrix = new Matrix();
		matrix.createGradientBox(b.width, b.height, 0, b.x, b.y);     
        var brush:GradientBrush = new GradientBrush(GradientBrush.LINEAR, colors, alphas, ratios, matrix);
        g.fillRectangle(brush, b.x, b.y, b.width, b.height);
	}

	public function getDisplay(c:Component):DisplayObject{
		return null;
	}

}
}