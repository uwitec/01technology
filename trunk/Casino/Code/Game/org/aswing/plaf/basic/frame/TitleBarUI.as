/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.plaf.basic.frame{

import org.aswing.plaf.BaseComponentUI;
import org.aswing.ASColor;
import org.aswing.AsWingConstants;
import org.aswing.AsWingUtils;
import org.aswing.Component;
import org.aswing.geom.*;
import org.aswing.graphics.*;
import org.aswing.Icon;
import org.aswing.JButton;
import org.aswing.JFrame;
import org.aswing.plaf.basic.frame.FrameTitleBar;
import org.aswing.plaf.basic.frame.TitleBarLayout;
import org.aswing.plaf.*;
import flash.events.Event;
import org.aswing.event.*;
import flash.text.*;
import flash.display.StageScaleMode;
import flash.geom.Rectangle;
import org.aswing.plaf.UIResource;
import flash.geom.Matrix;
import org.aswing.Insets;
import org.aswing.LayoutManager;

/**
 * Basic Frame title bar UI.
 * @author iiley
 * @private
 */
public class TitleBarUI extends BaseComponentUI{
	
	private static const ICON_TEXT_GAP:int = 2;
	
	private var titleTextField:TextField;
	private var titleBar:FrameTitleBar;
	private var frame:JFrame;
	
	private var iconifiedButton:JButton;
	private var resizeButton:JButton;
	private var closeButton:JButton;
	
	private var iconifiedIcon:Icon;
	private var normalIcon:Icon;
	private var maximizeIcon:Icon;
	private var closeIcon:Icon;
	
	private var activeColor:ASColor;
	private var activeTextColor:ASColor;
	private var inactiveColor:ASColor;
	private var inactiveTextColor:ASColor;
	private var activeBorderColor:ASColor;
	private var inactiveBorderColor:ASColor;
	
	private var stateChangeSize:Boolean;
	private var stateChangeLocation:Boolean;
	private var lastNormalStateBounds:IntRectangle;
	
	public function TitleBarUI() {
		super();
		stateChangeSize = false;
		stateChangeLocation = false;
		lastNormalStateBounds = new IntRectangle();
	}
	
	override public function installUI(c:Component):void{
    	titleBar = FrameTitleBar(c);
    	titleBar.setFocusable(false);
    	frame = titleBar.getFrame();
		
		installDefaults();
		installComponents();
		installListeners();
    }
    
    protected function installDefaults():void{
		titleBar.setLayout(getInstance("Frame.titleBarLayout") as LayoutManager);
		if(frame.getIcon() != null && frame.getIcon() is UIResource){
			frame.setIcon(getIcon("Frame.icon"));
		}
		activeColor         = getColor("Frame.activeCaption");
		activeTextColor     = getColor("Frame.activeCaptionText");
		inactiveColor       = getColor("Frame.inactiveCaption");
		inactiveTextColor   = getColor("Frame.inactiveCaptionText"); 	
		activeBorderColor   = getColor("Frame.activeCaptionBorder");
		inactiveBorderColor = getColor("Frame.inactiveCaptionBorder"); 	
		
		iconifiedIcon = getIcon("Frame.iconifiedIcon");
	 	normalIcon    = getIcon("Frame.normalIcon");
		maximizeIcon  = getIcon("Frame.maximizeIcon");
		closeIcon     = getIcon("Frame.closeIcon");
		
		
        titleBar.setBackground(activeColor);
        titleBar.setForeground(activeTextColor);
        if(!titleBar.isOpaqueSet()){
	        titleBar.setOpaque(getBoolean("Frame.titleBarOpaque"));
	        titleBar.setOpaqueSet(false);
        }
    }
    
	protected function installComponents() : void {
		iconifiedButton = new JButton(null, iconifiedIcon);
		resizeButton    = new JButton(null, maximizeIcon);
		closeButton     = new JButton(null, closeIcon);
		iconifiedButton.setBackgroundDecorator(null);
		resizeButton.setBackgroundDecorator(null);
		closeButton.setBackgroundDecorator(null);
		iconifiedButton.setMargin(new Insets(1, 1, 1, 1));
		resizeButton.setMargin(new Insets(1, 1, 1, 1));
		closeButton.setMargin(new Insets(1, 1, 1, 1));
		titleBar.append(iconifiedButton);
		titleBar.append(resizeButton);
		titleBar.append(closeButton);
		
		iconifiedButton.addActionListener(__iconifiedPressed);
		resizeButton.addActionListener(__resizePressed);
		closeButton.addActionListener(__closePressed);

		titleTextField = new TextField();
		titleTextField.name = "title";
		titleTextField.selectable = false;
		titleTextField.mouseEnabled = false;
 		titleTextField.autoSize = TextFieldAutoSize.LEFT;
		titleBar.addChild(titleTextField);
		
		adjustButtons();
		adjustResizerEnable();
		
		if(frame.getState() == JFrame.NORMAL){
			lastNormalStateBounds.setRect(frame.getComBounds());
		}		
	}
	
	protected function installListeners():void{
		frame.addEventListener(InteractiveEvent.STATE_CHANGED, __stateChanged);
		frame.addEventListener(ResizedEvent.RESIZED, __sizeChanged);
		frame.addEventListener(MovedEvent.MOVED, __frameMoved);
		frame.addEventListener(Event.ADDED_TO_STAGE, __frameShown);
		
		frame.addEventListener(WindowEvent.WINDOW_ACTIVATED, __activeChange);
		frame.addEventListener(WindowEvent.WINDOW_DEACTIVATED, __activeChange);
	}
	
    override public function uninstallUI(c:Component):void{
		uninstallDefaults();
		uninstallComponents();
		uninstallListeners();
    }
    
	protected function uninstallDefaults():void{
	}
	
	protected function uninstallComponents():void{
		titleBar.remove(iconifiedButton);
		titleBar.remove(resizeButton);
		titleBar.remove(closeButton);
		titleBar.removeChild(titleTextField);
	}
	
	protected function uninstallListeners():void{
		frame.removeEventListener(InteractiveEvent.STATE_CHANGED, __stateChanged);
		frame.removeEventListener(ResizedEvent.RESIZED, __sizeChanged);
		frame.removeEventListener(MovedEvent.MOVED, __frameMoved);
		frame.removeEventListener(Event.ADDED_TO_STAGE, __frameShown);
		
		frame.removeEventListener(WindowEvent.WINDOW_ACTIVATED, __activeChange);
		frame.removeEventListener(WindowEvent.WINDOW_DEACTIVATED, __activeChange);
	}
	
	//------------------------------------------
	private function __activeChange(e:Event):void{
		titleBar.repaint();
	}	
	private function __stageChanged(e:Event=null):void{
		if(frame.stage == null || frame.stage.scaleMode != StageScaleMode.NO_SCALE){
			return;
		}
		if(isMaximized()){
			setSizeToFixMaxmimized();
			frame.revalidateIfNecessary();
		}
	}
	private function __iconifiedPressed(e:Event):void{
		frame.setState(JFrame.ICONIFIED, false);
	}
	private function __resizePressed(e:Event):void{
		if(isNormalIcon()){
			frame.setState(JFrame.NORMAL, false);
		}else{
			frame.setState(JFrame.MAXIMIZED, false);
		}
	}
	private function __closePressed(e:Event):void{
		frame.closeReleased();
	}
	
	private function __frameMoved(e:MovedEvent):void{
		if(stateChangeLocation){
			stateChangeLocation = false;
		}else{
			lastNormalStateBounds.setLocation(e.getNewLocation());
		}
	}
	
	private function __sizeChanged(e:ResizedEvent):void{
		if(stateChangeSize || frame.getState() == JFrame.ICONIFIED){
			stateChangeSize = false;
		}else{
			lastNormalStateBounds.setSize(e.getNewSize());
		}
	}
	
	private var stageResizeListenerAdded:Boolean = false;
	
	private function __stateChanged(e:Event):void{
		var state:Number = frame.getState();
		if(state != JFrame.ICONIFIED 
			&& state != JFrame.NORMAL
			&& state != JFrame.MAXIMIZED_HORIZ
			&& state != JFrame.MAXIMIZED_VERT
			&& state != JFrame.MAXIMIZED){
			state = JFrame.NORMAL;
		}
		if(state == JFrame.ICONIFIED){
			iconifiedButton.setVisible(false);
			switchResizeIcon();
			var iconifiedSize:IntDimension = titleBar.getMinimumSize();
			stateChangeSize = true;
			frame.setSize(frame.getInsets().getOutsideSize(iconifiedSize));
			stateChangeSize = false;
			if(frame.isOnStage()){
				frame.stage.removeEventListener(Event.RESIZE, __stageChanged);
				stageResizeListenerAdded = false;
			}
		}else if(state == JFrame.NORMAL){
			stateChangeSize = true;
			frame.setBounds(lastNormalStateBounds);
			stateChangeSize = false;
			if(isNeedToViewIconifiedButton())
				iconifiedButton.setVisible(true);
			switchToMaximizButton();
			if(frame.isOnStage()){
				frame.stage.removeEventListener(Event.RESIZE, __stageChanged);
				stageResizeListenerAdded = false;
			}
		}else{
			setSizeToFixMaxmimized();
		}
		frame.revalidateIfNecessary();
	}
	
	private function setSizeToFixMaxmimized():void{
		var state:int = frame.getState();
		var maxBounds:IntRectangle = frame.getMaximizedBounds();
		var b:IntRectangle = frame.getComBounds();
		if((state & JFrame.MAXIMIZED_HORIZ) == JFrame.MAXIMIZED_HORIZ){
			b.x = maxBounds.x;
			b.width = maxBounds.width;
		}
		if((state & JFrame.MAXIMIZED_VERT) == JFrame.MAXIMIZED_VERT){
			b.y = maxBounds.y;
			b.height = maxBounds.height;
		}
		stateChangeSize = true;
		stateChangeLocation = true;
		frame.setBounds(b);
		stateChangeSize = false;
		stateChangeLocation = false;
		if(isNeedToViewIconifiedButton())
			iconifiedButton.setVisible(true);
		switchToNormalButton();
		
		if(frame.isOnStage()){
			frame.stage.addEventListener(Event.RESIZE, __stageChanged);
			stageResizeListenerAdded = true;
		}
	}
	
	private function __frameShown(e:Event):void{
		if(stageResizeListenerAdded){
			return;
		}
		var state:int = frame.getState();
		if(((state & JFrame.MAXIMIZED_HORIZ) == JFrame.MAXIMIZED_HORIZ) || ((state & JFrame.MAXIMIZED_VERT) == JFrame.MAXIMIZED_VERT)){
			if(frame.isOnStage()){
				frame.stage.addEventListener(Event.RESIZE, __stageChanged);
				stageResizeListenerAdded = true;
				__stageChanged();
			}
		}
	}
	
	
	public function isNormalIcon():Boolean{
		return resizeButton.getIcon() == normalIcon;
	}
		
	public function switchResizeIcon():void{
		if(isNormalIcon()){
			switchToMaximizButton();
		}else{
			switchToNormalButton();
		}
	}
	
	public function switchToMaximizButton():void{
		resizeButton.setIcon(maximizeIcon);
	}
	
	public function switchToNormalButton():void{
		resizeButton.setIcon(normalIcon);
	}
	
	private static var viewRect:IntRectangle = new IntRectangle();
    private static var textRect:IntRectangle = new IntRectangle();
    private static var iconRect:IntRectangle = new IntRectangle();
    
    private var paintedIcon:Icon;
    
	override public function paint(c:Component, g:Graphics2D, r:IntRectangle):void{
		super.paint(c, g, r);
		
		var icon:Icon = frame.getIcon();
		
		if(paintedIcon != icon){
			if(paintedIcon != null){
				if(paintedIcon.getDisplay(c) != null){
					titleBar.removeChild(paintedIcon.getDisplay(c));
				}
			}
			if(icon != null){
				if(icon.getDisplay(c) != null){
					titleBar.addChild(icon.getDisplay(c));
				}
			}
		}
		
    	viewRect.setRect(r);
    	var tlayout:TitleBarLayout = TitleBarLayout(titleBar.getLayout());
    	var buttonsWidth:int = 0;
    	var buttonCount:int = 0;
    	if(iconifiedButton.isVisible()){
    		buttonCount ++;
    		buttonsWidth += iconifiedButton.getWidth();
    	} 
    	if(resizeButton.isVisible()){
    		buttonCount ++;
    		buttonsWidth += resizeButton.getWidth();
    	} 
    	if(closeButton.isVisible()){
    		buttonCount ++;
    		buttonsWidth += closeButton.getWidth();
    	} 
    	viewRect.width -= buttonsWidth + Math.max(0, buttonCount-1)*tlayout.getHorizontalGap();
    		
    	textRect.x = textRect.y = textRect.width = textRect.height = 0;
        iconRect.x = iconRect.y = iconRect.width = iconRect.height = 0;

        // layout the text and icon
        var title:String = frame.getTitle();
        var text:String = AsWingUtils.layoutCompoundLabel(c, 
            c.getFont(), title, icon, 
            AsWingConstants.CENTER, AsWingConstants.LEFT,
            AsWingConstants.CENTER, AsWingConstants.RIGHT,
            viewRect, iconRect, textRect, 
	    	title == null || title == "" ? 0 : ICON_TEXT_GAP);
	   	
    	if(icon != null){
    		icon.updateIcon(c, g, iconRect.x, iconRect.y);
    	}
    	
        if (text != null && text != ""){
        	if(titleTextField.text != text){
				titleTextField.text = text;
        	}
			if(!frame.isFontValidated()){
				AsWingUtils.applyTextFont(titleTextField, frame.getFont());
				frame.setFontValidated(true);
			}
			AsWingUtils.applyTextColor(titleTextField, frame.getFrameUI().isPaintActivedFrame() ? activeTextColor : inactiveTextColor);
			titleTextField.x = textRect.x;
			titleTextField.y = textRect.y;
        }
		
		adjustButtons();
		adjustResizerEnable();
	}
	
	/**
	 * Override this method to paint different bg for title bar.
	 */
	override protected function paintBackGround(c:Component, g:Graphics2D, b:IntRectangle):void{
		if(!c.isOpaque()){
			 return;
		}
        var x:Number = b.x;
		var y:Number = b.y;
		var w:Number = b.width;
		var h:Number = b.height;
		
		var colors:Array;
		if(frame.getFrameUI().isPaintActivedFrame()){
	    	colors = [activeColor.darker(0.9).getRGB(), activeColor.getRGB()];
		}else{
	    	colors = [inactiveColor.darker(0.9).getRGB(), inactiveColor.getRGB()];
		}
		var alphas:Array = [1, 1];
		var ratios:Array = [75, 255];
		var matrix:Matrix = new Matrix();
		matrix.createGradientBox(w, h, (90/180)*Math.PI, x, y);
	    var brush:GradientBrush = new GradientBrush(GradientBrush.LINEAR, colors, alphas, ratios, matrix);
	    g.fillRoundRect(brush, x, y, w, h, 4, 4, 0, 0);    
		
		if(frame.getFrameUI().isPaintActivedFrame()){
			colors = [activeColor.getRGB(), activeColor.getRGB()];
		}else{
			colors = [inactiveColor.getRGB(), inactiveColor.getRGB()];
		}
        
		alphas = [1, 0];
		ratios = [0, 100];
        brush=new GradientBrush(GradientBrush.LINEAR, colors, alphas, ratios, matrix);
        g.fillRoundRect(brush, x, y, w, h-h/4, 4, 4, 0, 0);
        var penTool:Pen=new Pen(activeBorderColor);
        g.drawLine(penTool, x, y+h-1, x+w, y+h-1);
	}	
	
	private function isMaximized():Boolean{
		var state:Number = frame.getState();
		return ((state & JFrame.MAXIMIZED_HORIZ) == JFrame.MAXIMIZED_HORIZ)
			|| ((state & JFrame.MAXIMIZED_VERT) == JFrame.MAXIMIZED_VERT);
	}
	
	private function isNeedToViewIconifiedButton():Boolean{
		return frame.isResizable() && frame.getState() != JFrame.ICONIFIED;
	}
	
	private function isNeedToViewResizeButton():Boolean{
		return frame.isResizable();
	}
	
	private function isNeedToViewCloseButton():Boolean{
		return frame.isClosable();
	}

	private function adjustButtons() : void {
		iconifiedButton.setVisible(isNeedToViewIconifiedButton());
		resizeButton.setVisible(isNeedToViewResizeButton());
		closeButton.setVisible(isNeedToViewCloseButton());
	}

	private function adjustResizerEnable() : void {
		if(frame.getResizer() != null){
			frame.getResizer().setEnabled(frame.isResizable() && frame.getState() == JFrame.NORMAL);
		}
	}

}
}