/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.plaf.basic{

import org.aswing.*;
import org.aswing.geom.*;
import org.aswing.plaf.*;
import org.aswing.plaf.basic.frame.*;
import org.aswing.resizer.Resizer;
import flash.display.Sprite;
import flash.events.MouseEvent;
import org.aswing.event.*;
import org.aswing.graphics.*;
import flash.events.Event;
import flash.display.DisplayObjectContainer;
import flash.utils.Timer;
import flash.events.TimerEvent;
import flash.geom.Rectangle;

/**
 * Basic frame ui imp.
 * @author iiley
 * @private
 */
public class BasicFrameUI extends BaseComponentUI implements FrameUI{
	
	protected var frame:JFrame;
	protected var titleBar:FrameTitleBar;
	
	private var resizeArrowColor:ASColor;
	private var resizeArrowLightColor:ASColor;
	private var resizeArrowDarkColor:ASColor;
	
	protected var mouseMoveListener:Object;
	protected var boundsMC:Sprite;
	protected var flashTimer:Timer;
	
	public function BasicFrameUI() {
		super();
	}

    override public function installUI(c:Component):void {
        frame = JFrame(c);
        installDefaults();
		installComponents();
		installListeners();
    }
    
	protected function getPropertyPrefix():String {
		return "Frame.";
	}
	
    protected function installDefaults():void {
    	var pp:String = getPropertyPrefix();
		LookAndFeel.installColorsAndFont(frame, pp);
		LookAndFeel.installBorderAndBFDecorators(frame, pp);
		LookAndFeel.installBasicProperties(frame, pp);
		
	    resizeArrowColor = getColor("resizeArrow");
	    resizeArrowLightColor = getColor("resizeArrowLight");
	    resizeArrowDarkColor = getColor("resizeArrowDark");
    }
    
    protected function installComponents():void {
    	titleBar = createTitleBar();
    	titleBar.setUIElement(true);
    	frame.insert(0, titleBar, WindowLayout.TITLE);
    	
    	if(frame.getResizer() == null || frame.getResizer() is UIResource){
	    	var resizer:Resizer = getInstance(getPropertyPrefix()+"resizer") as Resizer;
	    	frame.setResizer(resizer);
    	}
    	if(!frame.isDragDirectlySet()){
    		frame.setDragDirectly(getBoolean(getPropertyPrefix()+"dragDirectly"));
    		frame.setDragDirectlySet(false);
    	}
    	boundsMC = new Sprite();
    	boundsMC.name = "drag_bounds";
	}
	
	protected function installListeners():void{
		titleBar.addEventListener(MouseEvent.MOUSE_DOWN, __onTitleBarPress);
		titleBar.addEventListener(ReleaseEvent.RELEASE, __onTitleBarRelease);
		titleBar.doubleClickEnabled = true;
		titleBar.addEventListener(MouseEvent.DOUBLE_CLICK, __onTitleBarDoubleClick);
		
		frame.addEventListener(WindowEvent.WINDOW_ACTIVATED, __activeChange);
		frame.addEventListener(WindowEvent.WINDOW_DEACTIVATED, __activeChange);
		frame.addEventListener(PopupEvent.POPUP_CLOSED, __frameClosed);
		frame.addEventListener(Event.REMOVED_FROM_STAGE, __frameClosed);
	}

    override public function uninstallUI(c:Component):void {
        var frame:JFrame = JFrame(c);
        uninstallDefaults();
		uninstallComponents();
		uninstallListeners();
    }
    
    protected function uninstallDefaults():void {
		LookAndFeel.uninstallBorderAndBFDecorators(frame);
		frame.filters = [];
    }
    
	protected function uninstallComponents():void{
		frame.remove(titleBar);
		removeBoundsMC();
	}
	
	protected function uninstallListeners():void{
		titleBar.removeEventListener(MouseEvent.MOUSE_DOWN, __onTitleBarPress);
		titleBar.removeEventListener(ReleaseEvent.RELEASE, __onTitleBarRelease);
		titleBar.removeEventListener(MouseEvent.DOUBLE_CLICK, __onTitleBarDoubleClick);
		
		frame.removeEventListener(WindowEvent.WINDOW_ACTIVATED, __activeChange);
		frame.removeEventListener(WindowEvent.WINDOW_DEACTIVATED, __activeChange);
		frame.removeEventListener(PopupEvent.POPUP_CLOSED, __frameClosed);
		frame.removeEventListener(Event.REMOVED_FROM_STAGE, __frameClosed);
		if(flashTimer != null){
			flashTimer.stop();
			flashTimer = null;
		}
	}
	
	private var flashing:Boolean;
	private var flashingActivedColor:Boolean;

	/**
	 * Flash the modal frame. (User clicked other where is not in the modal frame, 
	 * flash the frame to make notice this frame is modal.)
	 */
	public function flashModalFrame():void{
		if(flashTimer == null){
			flashTimer = new Timer(50, 8);
			flashTimer.addEventListener(TimerEvent.TIMER, __flashTick);
			flashTimer.addEventListener(TimerEvent.TIMER_COMPLETE, __flashComplete);
		}
		flashing = true;
		flashingActivedColor = false;
		flashTimer.reset();
		flashTimer.start();
	}
	
	private function __flashTick(e:TimerEvent):void{
		flashingActivedColor = !flashingActivedColor;
		frame.repaint();
		titleBar.repaint();
	}
    
	private function __flashComplete(e:TimerEvent):void{
		flashing = false;
		frame.repaint();
		titleBar.repaint();
	}

	/**
	 * For <code>flashModalFrame</code> to judge whether paint actived color or inactived color.
	 */    
	public function isPaintActivedFrame():Boolean{
		if(flashing){
			return flashingActivedColor;
		}else{
			return frame.isActive();
		}
	}
    //----------------------------------------------------------
    override protected function paintBackGround(c:Component, g:Graphics2D, b:IntRectangle):void{
    	var bgMargin:Insets = c.getUI().getInsets(getPropertyPrefix()+"backgroundMargin");
    	if(bgMargin){
    		b = bgMargin.getInsideBounds(b);
    	}
    	super.paintBackGround(c, g, b);
    }
    
    /**
     * Override this method to create different title bar
     */
    protected function createTitleBar():FrameTitleBar{
    	var tb:FrameTitleBar = new FrameTitleBar(frame);
    	//To get default UI from Defauls property
    	tb.updateUI();
    	return tb;
    }
    //----------------------------------------------------------
	
	private function isMaximizedFrame():Boolean{
		var state:Number = frame.getState();
		return ((state & JFrame.MAXIMIZED_HORIZ) == JFrame.MAXIMIZED_HORIZ)
				|| ((state & JFrame.MAXIMIZED_VERT) == JFrame.MAXIMIZED_VERT);
	}
	
	private function __activeChange(e:Event):void{
		frame.repaint();
	}
	
	private var startPos:IntPoint;
	private var startMousePos:IntPoint;
    private function __onTitleBarPress(e:MouseEvent):void{
    	if(e.target != titleBar){
    		return;
    	}
    	if(frame.isDragable() && !isMaximizedFrame()){
    		if(frame.isDragDirectly()){
    			var db:Rectangle = frame.getInsets().getInsideBounds(frame.getMaximizedBounds()).toRectangle();
    			var gap:Number = titleBar.getHeight();
    			db.x -= (frame.width - gap);
    			db.y -= frame.getInsets().top;
    			db.width += (frame.width - gap*2);
    			db.height -= gap;
    			
    			frame.startDrag(false, db);
    		}else{
    			startMousePos = frame.getMousePosition();
    			startPos = frame.getLocation();
    			AsWingManager.getStage().addEventListener(MouseEvent.MOUSE_MOVE, __onMouseMove, false, 0, true);
    		}
    	}
    }
    
    private function __onTitleBarRelease(e:ReleaseEvent):void{
    	if(e.getPressTarget() != titleBar){
    		return;
    	}
    	frame.stopDrag();
    	AsWingManager.getStage().removeEventListener(MouseEvent.MOUSE_MOVE, __onMouseMove);
    	if(frame.isDragable() && !isMaximizedFrame() && !frame.isDragDirectly()){
	    	var dest:IntPoint = representMoveBounds();
	    	frame.setLocation(dest);
	    	frame.validate();
    	}
    	removeBoundsMC();
    }
    
    private function __onTitleBarDoubleClick(e:Event):void{
    	if(e.target != titleBar){
    		return;
    	}
		if(frame.isResizable()){
			var state:int = frame.getState();
			
			if((state & JFrame.MAXIMIZED_HORIZ) == JFrame.MAXIMIZED_HORIZ
				|| (state & JFrame.MAXIMIZED_VERT) == JFrame.MAXIMIZED_VERT
				|| (state & JFrame.ICONIFIED) == JFrame.ICONIFIED){
					frame.setState(JFrame.NORMAL, false);
			}else{
				frame.setState(JFrame.MAXIMIZED, false);
			}
		}
    }
    
    private function __frameClosed(e:Event):void{
    	removeBoundsMC();
    	if(flashTimer != null){
    		flashTimer.stop();
    		flashTimer = null;
    	}
    }
    
    private function removeBoundsMC():void{
    	if(frame.parent != null && frame.parent.contains(boundsMC)){
    		frame.parent.removeChild(boundsMC);
    	}
    }
    
    private function representMoveBounds(e:MouseEvent=null):IntPoint{
    	var par:DisplayObjectContainer = frame.parent;
    	if(boundsMC.parent != par){
    		par.addChild(boundsMC);
    	}
    	var currentMousePos:IntPoint = frame.getMousePosition();
    	var bounds:IntRectangle = frame.getComBounds();
    	bounds.x = startPos.x + currentMousePos.x - startMousePos.x;
    	bounds.y = startPos.y + currentMousePos.y - startMousePos.y;
    	
    	//these make user can't drag frames out the stage
    	var gap:Number = titleBar.getHeight();
    	var frameMaxBounds:IntRectangle = frame.getMaximizedBounds();
    	
    	var topLeft:IntPoint = frameMaxBounds.leftTop();
    	var topRight:IntPoint = frameMaxBounds.rightTop();
    	var bottomLeft:IntPoint = frameMaxBounds.leftBottom();
    	if(bounds.x < topLeft.x - bounds.width + gap){
    		bounds.x = topLeft.x - bounds.width + gap;
    	}
    	if(bounds.x > topRight.x - gap){
    		bounds.x = topRight.x - gap;
    	}
    	if(bounds.y < topLeft.y){
    		bounds.y = topLeft.y;
    	}
    	if(bounds.y > bottomLeft.y - gap){
    		bounds.y = bottomLeft.y - gap;
    	}
    	
		var x:Number = bounds.x;
		var y:Number = bounds.y;
		var w:Number = bounds.width;
		var h:Number = bounds.height;
		var g:Graphics2D = new Graphics2D(boundsMC.graphics);
		boundsMC.graphics.clear();
		g.drawRectangle(new Pen(resizeArrowLightColor, 1), x-1,y-1,w+2,h+2);
		g.drawRectangle(new Pen(resizeArrowColor, 1), x,y,w,h);
		g.drawRectangle(new Pen(resizeArrowDarkColor, 1), x+1,y+1,w-2,h-2);
		return bounds.leftTop();
    }
    private function __onMouseMove(e:MouseEvent):void{
    	representMoveBounds(e);
    }
}
}