/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing{
	
import flash.display.DisplayObject;
import flash.display.DisplayObjectContainer;
import flash.display.Sprite;
import flash.ui.Mouse;
import org.aswing.util.DepthManager;
import flash.events.MouseEvent;
import flash.events.Event;
import flash.display.InteractiveObject;
import flash.utils.Dictionary;
import org.aswing.util.Reflection;
	
/**
 * The CursorManager, manage the cursor, hide system mouse cursor, show custom cursor, 
 * etc.
 * @author iiley
 */
public class CursorManager{
	
	private static var root:DisplayObjectContainer = null;
	private static var cursorHolder:DisplayObjectContainer = null;
	private static var currentCursor:DisplayObject = null;
	
	/**
	 * Sets the container to hold the cursors(in fact it will hold the cursor's parent--a sprite).
	 * By default(if you have not set one), it is the stage if <code>AsWingManager</code> is inited.
	 * @param theRoot the container to hold the cursors.
	 * @see org.aswing.AsWingManager#getStage()
	 */
	public static function setCursorContainerRoot(theRoot:DisplayObjectContainer):void{
		if(theRoot != root){
			root = theRoot;
			if(cursorHolder != null && cursorHolder.parent != root){
				root.addChild(cursorHolder);
			}
		}
	}
	
	private static function getCursorContainerRoot():DisplayObjectContainer{
		if(root == null){
			return AsWingManager.getStage();
		}
		return root;
	}
	
	/**
	 * Shows your display object as the cursor and/or not hide the system cursor. 
	 * If current another custom cursor is showing, the current one will be removed 
	 * and then the new one is shown.
	 * @param cursor the display object to be add to the cursor container to be the cursor
	 * @param hideSystemCursor whether or not hide the system cursor when custom cursor shows.
	 */
	public static function showCustomCursor(cursor:DisplayObject, hideSystemCursor:Boolean=true):void{
		if(hideSystemCursor){
			Mouse.hide();
		}else{
			Mouse.show();
		}
		if(cursor == currentCursor){
			return;
		}
		
		var ro:DisplayObjectContainer = getCursorContainerRoot();
		if(cursorHolder == null){
			if(ro != null){
				cursorHolder = new Sprite();
				cursorHolder.mouseEnabled = false;
				cursorHolder.tabEnabled = false;
				cursorHolder.mouseChildren = false;
				ro.addChild(cursorHolder);
			}
		}
		if(cursorHolder != null){
			if(currentCursor != cursor){
				if(currentCursor != null){
					cursorHolder.removeChild(currentCursor);
				}
				currentCursor = cursor;
				cursorHolder.addChild(currentCursor);
			}
			DepthManager.bringToTop(cursorHolder);
			ro.stage.addEventListener(MouseEvent.MOUSE_MOVE, __mouseMove);
			__mouseMove(null);
		}
	}
	
	private static function __mouseMove(e:MouseEvent):void{
		cursorHolder.x = cursorHolder.parent.mouseX;
		cursorHolder.y = cursorHolder.parent.mouseY;
		DepthManager.bringToTop(cursorHolder);
	}
	
	/**
	 * Hides the custom cursor which is showing and show the system cursor.
	 * @param cursor the showing cursor, if it is not the showing cursor, nothing 
	 * will happen
	 */
	public static function hideCustomCursor(cursor:DisplayObject):void{
		if(cursor != currentCursor){
			return;
		}
		if(cursorHolder != null){
			if(currentCursor != null){
				cursorHolder.removeChild(currentCursor);
			}
		}
		currentCursor = null;
		Mouse.show();
		var ro:DisplayObjectContainer = getCursorContainerRoot();
		if(ro != null){
			ro.stage.removeEventListener(MouseEvent.MOUSE_MOVE, __mouseMove);
		}
	}
	
	private static var tiggerCursorMap:Dictionary = new Dictionary(true);
	
	/**
	 * Sets the cursor when mouse on the specified trigger.
	 * @param trigger where the cursor will shown when the mouse on the trigger
	 * @param cursor the cursor object
	 */
	public static function setCursor(trigger:InteractiveObject, cursor:DisplayObject):void{
		tiggerCursorMap[trigger] = cursor;
		trigger.addEventListener(MouseEvent.ROLL_OVER, __triggerOver, false, 0, true);
		trigger.addEventListener(MouseEvent.ROLL_OUT, __triggerOut, false, 0, true);
	}
	
	private static function __triggerOver(e:MouseEvent):void{
		var trigger:Object = e.currentTarget;
		var cursor:DisplayObject = tiggerCursorMap[trigger] as DisplayObject;
		if(cursor){
			showCustomCursor(cursor);
		}
	}
	
	private static function __triggerOut(e:MouseEvent):void{
		var trigger:Object = e.currentTarget;
		var cursor:DisplayObject = tiggerCursorMap[trigger] as DisplayObject;
		if(cursor){
			hideCustomCursor(cursor);
		}
	}
}
}