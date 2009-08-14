/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.plaf.basic.frame{
	
import org.aswing.*;

/**
 * the title bar for frame.
 * @author iiley
 * @private
 */
public class FrameTitleBar extends Container{
		
	private var frame:JFrame;
	
	public function FrameTitleBar(frame:JFrame) {
		super();
		setName("TitleBar");
		this.frame = frame;
		//updateUI is called in FrameUI to make it be controlled by FrameUI
		//updateUI();
	}
	
	override public function setLayout(l:LayoutManager):void{
		if(l is TitleBarLayout){
			super.setLayout(l);
		}else{
			throw new TypeError("FrameTitleBar just can accept FrameTitleBar!");
		}
	}
		
	public function getFrame():JFrame{
		return frame;
	}
	
	override public function updateUI():void{
    	setUI(UIManager.getUI(this));
    }
	
	override public function getUIClassID():String{
		return "Frame.titleBarUI";
	}
	
}
}