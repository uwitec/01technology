/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.resizer{

import org.aswing.Component;
import org.aswing.geom.IntRectangle;

/**
 * The strategy for DefaultResizer to count the new bounds of component would be resized to.
 * @author iiley
 */
public interface ResizeStrategy{
	
	/**
	 * Count and return the new bounds what the component would be resized to.
	 */
	function getBounds(com:Component, movedX:int, movedY:int):IntRectangle;
}
}