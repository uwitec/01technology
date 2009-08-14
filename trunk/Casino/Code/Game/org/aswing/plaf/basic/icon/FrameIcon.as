/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.plaf.basic.icon{

import org.aswing.graphics.Graphics2D;
import org.aswing.*;
import flash.display.DisplayObject;
import org.aswing.plaf.UIResource;
import flash.display.Shape;

/**
 * Frame title bar icon base.
 * @author iiley
 * @private
 */
public class FrameIcon implements Icon, UIResource{
		
	private static const DEFAULT_ICON_WIDTH:int = 13;
	
	protected var width:int;
	protected var height:int;
	protected var shape:Shape;
	
	private var color:ASColor;
		
	/**
	 * @param width the width of the icon square.
	 */	
	public function FrameIcon(width:int=DEFAULT_ICON_WIDTH){
		this.width = width;
		height = width;
		shape = new Shape();
	}
	
	public function getColor():ASColor{
		return color;
	}
	
	public function updateIcon(c:Component, g:Graphics2D, x:int, y:int):void
	{
		if(color == null){
			color = c.getUI().getColor("Frame.activeCaptionText");
		}
		shape.graphics.clear();
		updateIconImp(c, new Graphics2D(shape.graphics), x, y);
	}
	
	public function updateIconImp(c:Component, g:Graphics2D, x:int, y:int):void{}
	
	public function getIconHeight(c:Component):int
	{
		return width;
	}
	
	public function getIconWidth(c:Component):int
	{
		return height;
	}
	
	public function getDisplay(c:Component):DisplayObject
	{
		return shape;
	}
	
}
}