package org.god.SkinControl
{
	import flash.events.Event;
	import flash.events.TextEvent;
	
	import org.aswing.JTextField;
	
	public class SkinTextField extends JTextField
	{
		protected var m_bOnlyNumber:Boolean = false;
		protected var m_bOnlyASCIICharAndNumber:Boolean = false;
		protected var m_strPrefix:String = "";
		
		public function SkinTextField()
		{
			super();
			getTextField().addEventListener(TextEvent.TEXT_INPUT , OnEventTextInput);
			getTextField().addEventListener(Event.CHANGE , OnEventTextChange);
		}
		public function Destroy():void
		{
			getTextField().removeEventListener(TextEvent.TEXT_INPUT , OnEventTextInput);
			getTextField().removeEventListener(Event.CHANGE , OnEventTextChange);
		}
		override public function setDisplayAsPassword(b:Boolean):void
		{
			super.setDisplayAsPassword(b);
			setOnlyASCIICharAndNumber(b);
		}
		public function setOnlyNumber(bNumber:Boolean):void
		{
			m_bOnlyNumber =  bNumber;
			if(m_bOnlyNumber)
			{
				getTextField().restrict = "0-9.";
			}
			else
			{
				getTextField().restrict = null;
			}
			setText("0");
		}
		public function setPrefix(strPrefix:String):void
		{
			m_strPrefix = strPrefix;
			setText(m_strPrefix);
		}
		public function setOnlyASCIICharAndNumber(bOnlyASCIICharAndNumber:Boolean):void
		{
			m_bOnlyASCIICharAndNumber = bOnlyASCIICharAndNumber;
			if(m_bOnlyASCIICharAndNumber)
			{
				getTextField().restrict = "0-9A-Za-z";
			}
			else
			{
				getTextField().restrict = null;
			}
		}
		public function getNumber():Number
		{
			return Number(getText());
		}
		protected function OnEventTextInput(e:TextEvent):void
		{
			if(m_bOnlyNumber)
			{
				if(HavePointChar(e.text))
				{
					var str:String = getText();
					if(HavePointChar(str))
					{   
						e.preventDefault();
						return;
					}
				}
			}
		}
		protected function OnEventTextChange(e:Event):void
		{
			if(m_strPrefix.length)
			{
				var str:String = getText();
				var tmp:String = str.substr(0, m_strPrefix.length);
				var tmp1:String = str.substr(m_strPrefix.length);
				if(tmp != m_strPrefix)
				{
					str = m_strPrefix + tmp1;
				}
				setText(str);
			}
		}
		protected function HavePointChar(str:String):Boolean
		{
			for(var i:uint = 0; i < str.length; i ++)
			{
				if(str.charAt(i)== ".")
					return true;
			}
			return false;
		}
		
	}
}