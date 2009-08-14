/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing
{

import org.aswing.plaf.basic.BasicButtonUI;

/**
 * An implementation of a "push" button.
 * @author iiley
 */
public class JButton extends AbstractButton
{
	public function JButton(text:String="", icon:Icon=null){
		super(text, icon);
		setName("JButton");
    	setModel(new DefaultButtonModel());
	}
	
	/**
	 * Returns whether this button is the default button of its root pane or not.
	 * @return true if this button is the default button of its root pane, false otherwise.
	 */
	public function isDefaultButton():Boolean{
		var rootPane:JRootPane = getRootPaneAncestor();
		if(rootPane != null){
			return rootPane.getDefaultButton() == this;
		}
		return false;
	}
	
    override public function updateUI():void{
    	setUI(UIManager.getUI(this));
    }
	
    override public function getDefaultBasicUIClass():Class{
    	return org.aswing.plaf.basic.BasicButtonUI;
    }
    
	override public function getUIClassID():String{
		return "ButtonUI";
	}
	
}

}