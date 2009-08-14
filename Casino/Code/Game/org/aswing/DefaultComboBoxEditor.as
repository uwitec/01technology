/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing{

import flash.events.EventDispatcher;
import flash.events.IEventDispatcher;
import flash.events.Event;
import org.aswing.event.AWEvent;
import flash.events.KeyboardEvent;
import flash.ui.Keyboard;
import flash.events.FocusEvent;

public class DefaultComboBoxEditor extends EventDispatcher implements ComboBoxEditor{

    private var textField:JTextField;
    protected var value:*;
    	
	public function DefaultComboBoxEditor(){
	}
	
	public function selectAll():void{
		if(getTextField().isEditable()){
			getTextField().selectAll();
		}
		//getTextField().makeFocus();
	}
	
	public function setValue(value:*):void{
		this.value = value;
		getTextField().setText(value+"");
	}
	
	public function addActionListener(listener:Function, priority:int=0, useWeakReference:Boolean=false):void{
		addEventListener(AWEvent.ACT, listener, false,  priority, useWeakReference);
	}
	
	public function getValue():*{
		return value;
	}
	
	public function removeActionListener(listener:Function):void{
		removeEventListener(AWEvent.ACT, listener, false);
	}
	
	public function setEditable(b:Boolean):void{
        getTextField().setEditable(b);
        getTextField().setEnabled(b);
	}
	
	public function getEditorComponent():Component{
		return getTextField();
	}
	
	public function isEditable():Boolean{
		return getTextField().isEditable();
	}

    override public function toString():String{
        return "DefaultComboBoxEditor[]";
    }	
    
    //------------------------------------------------------

    
    protected function getTextField():JTextField{
        if(textField == null){
            textField = new JTextField("", 1); //set rows 1 to ensure the JTextField has a perfer height when empty
            textField.setBorder(null);
            textField.setOpaque(false);
            textField.setFocusable(false);
            textField.setBackgroundDecorator(null);

            initHandler();
        }
        return textField;
    }

    private function initHandler():void{
        textField.getTextField().addEventListener(KeyboardEvent.KEY_DOWN, __textKeyDown);
        textField.getTextField().addEventListener(FocusEvent.FOCUS_OUT, __grapValueFormText);
    }
	
    private function __grapValueFormText(e:Event):void{
    	if(textField.isEditable() && value !== getTextField().getText()){
    		value = getTextField().getText();
	        dispatchEvent(new AWEvent(AWEvent.ACT));
     	}
    }

    private function __textKeyDown(e:KeyboardEvent):void{
    	if(textField.isEditable() && e.keyCode == Keyboard.ENTER){
	        __grapValueFormText(null);
	        dispatchEvent(new AWEvent(AWEvent.ACT));
     	}
    }   
}
}