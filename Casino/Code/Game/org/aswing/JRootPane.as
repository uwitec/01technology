/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing{

import flash.ui.Keyboard;
import flash.events.*;
import flash.text.TextField;
import org.aswing.util.HashMap;

/**
 * The general AsWing window root container, it is the popup, window and frame's ancestor.
 * It manages the key accelerator and mnemonic for a pane.
 * @see #registerMnemonic()
 * @author iiley
 */	
public class JRootPane extends Container{
	
	private var defaultButton:JButton;
	private var mnemonics:HashMap;
	private var mnemonicJustActed:Boolean;
	private var keymap:KeyMap;
	private var mnemonicsForcedWorking:Boolean;
	
	//TODO imp
	private var menuBar:*;
	
	public function JRootPane(){
		super();
		setName("JRootPane");
		mnemonicJustActed = false;
		layout = new BorderLayout();
		mnemonics = new HashMap();
		keymap = new KeyMap();
		mnemonicsForcedWorking = false;
		addEventListener(TextEvent.TEXT_INPUT, __textInput, true);
		addEventListener(KeyboardEvent.KEY_DOWN, __keyDown, true);
		addEventListener(Event.REMOVED_FROM_STAGE, __removedFromStage);
	}
	
	public function setDefaultButton(button:JButton):void{
		if(defaultButton != button){
			if(defaultButton != null){
				defaultButton.repaint();
			}
			defaultButton = button;
			defaultButton.repaint();
		}
	}
	
	public function getDefaultButton():JButton{
		return defaultButton;
	}
	
	/**
	 * Sets the main menuBar of this root pane.(Main menu bar means that 
	 * if user press Alt key, the first menu of the menu bar will be actived)
	 * The menuBar must be located in this root pane(or in its child), 
	 * otherwise, it will not have the main menu bar ability.
	 * @menuBar the menu bar, or null 
	 */
	public function setMenuBar(menuBar:*):void{
		//TODO imp
	}
	
	/**
	 * Returns the key -> action map of this window.
	 * When a window is actived, it's keymap will be in working, or it is out of working.
	 * @see org.aswing.KeyMap
	 * @see org.aswing.KeyboardController
	 */
	public function getKeyMap():KeyMap{
		return keymap;
	}
	
	/**
	 * Sets whether or not the kay map action will be fired.
	 * @param b true to make it work, false not.
	 */
	public function setKeyMapActived(b:Boolean):void{
		if(b){
			KeyboardManager.getInstance().registerKeyMap(getKeyMap());
		}else{
			KeyboardManager.getInstance().unregisterKeyMap(getKeyMap());
		}
	}
	
	/**
	 * Sets the mnemonic be forced to work or not.
	 * <p>
	 * true, to make the mnemonic be forced to work, it means what ever the root pane and 
	 * it children has focused or not, it will listen the key to make mnemonic works.<br>
	 * false, to make the mnemonic works in normal way, it means the mnenonic will only works 
	 * when the root pane or its children has focus.
	 * </p>
	 * @param b forced work or not.
	 */
	public function setMnemonicForcedToWork(b:Boolean):void{
		if(b != mnemonicsForcedWorking){
			if(b){
				removeEventListener(TextEvent.TEXT_INPUT, __textInput, true);
				removeEventListener(KeyboardEvent.KEY_DOWN, __keyDown, true);
				AsWingManager.getStage().addEventListener(TextEvent.TEXT_INPUT, __textInput, true, 0, true);
				AsWingManager.getStage().addEventListener(KeyboardEvent.KEY_DOWN, __keyDown, true, 0, true);
			}else{
				AsWingManager.getStage().removeEventListener(TextEvent.TEXT_INPUT, __textInput, true);
				AsWingManager.getStage().removeEventListener(KeyboardEvent.KEY_DOWN, __keyDown, true);
				addEventListener(TextEvent.TEXT_INPUT, __textInput, true);
				addEventListener(KeyboardEvent.KEY_DOWN, __keyDown, true);
			}
			mnemonicsForcedWorking = b;
		}
	}
	
	/**
	 * Register a button with its mnemonic.
	 */
	internal function registerMnemonic(button:AbstractButton):void{
		if(button.getMnemonic() >= 0){
			mnemonics.put(button.getMnemonic(), button);
		}
	}
	
	internal function unregisterMnemonic(button:AbstractButton):void{
		if(mnemonics.get(button.getMnemonic()) == button){
			mnemonics.remove(button.getMnemonic());
		}
	}
	
	//-------------------------------------------------------
	//        Event Handlers
	//-------------------------------------------------------
	
	private function __keyDown(e:KeyboardEvent):void{
		mnemonicJustActed = false;
		
		var code:uint = e.keyCode;
		
		if(code == Keyboard.ENTER){
			var dfBtn:AbstractButton = getDefaultButton();
			if(dfBtn != null){
				if(dfBtn.isShowing() && dfBtn.isEnabled()){
					dfBtn.doClick();
					mnemonicJustActed = true;
					return;
				}
			}
		}
		
		//try to trigger the mnemonic
		if(AsWingManager.getStage().focus is TextField){
			if(!KeyboardManager.getInstance().isMnemonicModifierDown()){
				return;
			}
		}
		var mnBtn:AbstractButton = mnemonics.getValue(int(code));
		if(mnBtn != null){
			if(mnBtn.isShowing() && mnBtn.isEnabled()){
				mnBtn.doClick();
				FocusManager.getCurrentManager().setTraversing(true);
				mnBtn.paintFocusRect();
				mnemonicJustActed = true;
			}
		}
	}
	
	private function __textInput(e:TextEvent):void{
		if(KeyboardManager.getInstance().isMnemonicModifierDown() || KeyboardManager.getInstance().isKeyJustActed()){
			e.preventDefault();
		}
	}
	
	private function __removedFromStage(e:Event):void{
		mnemonics.clear();
	}
}

}