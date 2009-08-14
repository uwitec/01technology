/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing{
	
import flash.display.Stage;
import flash.events.KeyboardEvent;
import flash.events.Event;
import org.aswing.util.*;
import flash.ui.Keyboard;
import flash.events.EventDispatcher;

/**
 * Dispatched when key is down.
 * @eventType flash.events.KeyboardEvent.KEY_DOWN
 */
[Event(name="keyDown", type="flash.events.KeyboardEvent")]

/**
 * Dispatched when key is up.
 * @eventType flash.events.KeyboardEvent.KEY_UP
 */
[Event(name="keyUp", type="flash.events.KeyboardEvent")]

/**
 * KeyboardController controlls the key map for the action firing.
 * <p>
 * Thanks Romain for his Fever{@link http://fever.riaforge.org} accelerator framworks implementation, 
 * this is a simpler implementation study from his.
 * 
 * @see org.aswing.KeyMap
 * @see org.aswing.KeyType
 * @author iiley
 */
public class KeyboardManager extends EventDispatcher{
	
	private static var instance:KeyboardManager;
	
	private var keymaps:Vector;
	private var keySequence:Vector;
	private var selfKeyMap:KeyMap;
	private var inited:Boolean;
	private var keyJustActed:Boolean;
	
	private var mnemonicModifier:Array;
	
	/**
	 * Singleton class, 
	 * Don't create instance directly, in stead you should call <code>getInstance()</code>.
	 */
	public function KeyboardManager(){
		inited = false;
		keyJustActed = false;
		keymaps = new Vector();
		keySequence = new Vector();
		selfKeyMap = new KeyMap();
		mnemonicModifier = [Keyboard.CONTROL, Keyboard.SHIFT];
		registerKeyMap(selfKeyMap);
	}
	
	/**
	 * Returns the global keyboard controller.
	 */
	public static function getInstance():KeyboardManager{
		if(instance == null){
			instance = new KeyboardManager();
		}
		return instance;
	}
	
	/**
	 * Init the keyboad manager, it will only start works when it is inited.
	 * By default, it will be inited when a component is added to stage automatically.
	 */
	public function init(stage:Stage):void{
		if(!inited){
			inited = true;
			stage.addEventListener(KeyboardEvent.KEY_DOWN, __onKeyDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, __onKeyUp);
			stage.addEventListener(Event.DEACTIVATE, __deactived);
			
			//stage.addEventListener(KeyboardEvent.KEY_DOWN, __onKeyDownCap, true);
			//stage.addEventListener(KeyboardEvent.KEY_UP, __onKeyUpCap, true);
		}
	}
	
	/**
	 * Registers a key action map to the controller
	 */
	public function registerKeyMap(keyMap:KeyMap):void{
		if(!keymaps.contains(keyMap)){
			keymaps.append(keyMap);
		}
	}
	
	/**
	 * Unregisters a key action map to the controller
	 * @param keyMap the key map
	 */
	public function unregisterKeyMap(keyMap:KeyMap):void{
		keymaps.remove(keyMap);
	}
	
	/**
	 * Registers a key action to the default key map of this controller.
	 * @param key the key type
	 * @param action the action
	 * @see KeyMap#registerKeyAction()
	 */
	public function registerKeyAction(key:KeyType, action:Function):void{
		selfKeyMap.registerKeyAction(key, action);
	}
	
	/**
	 * Unregisters a key action to the default key map of this controller.
	 * @param key the key type
	 * @see KeyMap#unregisterKeyAction()
	 */
	public function unregisterKeyAction(key:KeyType):void{
		selfKeyMap.unregisterKeyAction(key);
	}
	
	/**
	 * Returns whether or not the key is down.
	 * @param the key code
	 * @return true if the specified key is down, false if not.
	 */
	public function isKeyDown(keyCode:uint):Boolean{
		return keySequence.contains(keyCode);
	}
	
	/**
	 * Returns whether or not the key is down. 
	 * This method is same to <code>getInstance().isKeyDown()</code> 
	 * @param the key code
	 * @return true if the specified key is down, false if not.
	 */
	public static function isDown(keyCode:uint):Boolean{
		return getInstance().isKeyDown(keyCode);
	}
	
	/**
	 * Sets the mnemonic modifier key codes, the default is [Ctrl, Shift], however 
	 * for normal UI frameworks, it is [Alt], but because the flashplayer or explorer will 
	 * eat [Alt] for thier own mnemonic modifier, so we set our default to [Ctrl, Shift].
	 * @param keyCodes the array of key codes to be the mnemoic modifier.
	 */
	public function setMnemonicModifier(keyCodes:Array):void{
		mnemonicModifier = keyCodes.concat();
	}
	
	/**
	 * Returns whether or not the mnemonic modifier keys is down.
	 * @return whether or not the mnemonic modifier keys is down.
	 */
	public function isMnemonicModifierDown():Boolean{
		for(var i:int=0; i<mnemonicModifier.length; i++){
			if(!isKeyDown(mnemonicModifier[i])){
				return false;
			}
		}
		return true;
	}
	
	/**
	 * Returns whether or not just a key action acted when the last key down.
	 * @return true if there's key actions acted at last key down, false not.
	 */
	public function isKeyJustActed():Boolean{
		return keyJustActed;
	}
		
	private function __onKeyDown(e:KeyboardEvent) : void {
		dispatchEvent(e);
		var code:uint = e.keyCode;
		if(!keySequence.contains(code)){
			keySequence.append(code);
		}
		keyJustActed = false;
		var n:int = keymaps.size();
		for(var i:int=0; i<n; i++){
			var keymap:KeyMap = KeyMap(keymaps.get(i));
			if(keymap.fireKeyAction(keySequence.toArray())){
				keyJustActed = true;
			}
		}
	}

	private function __onKeyUp(e:KeyboardEvent) : void {
		dispatchEvent(e);
		var code:uint = e.keyCode;
		keySequence.remove(code);
	}
	
	private function __deactived(e:Event):void{
		keySequence.clear();
	}
}

}