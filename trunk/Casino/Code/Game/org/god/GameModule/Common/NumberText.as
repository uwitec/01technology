package org.god.GameModule.Common
{
	import flash.display.Sprite;
	import flash.display.MovieClip;

	public class NumberText extends Sprite
	{
		protected var m_fValue:Number;
		protected var m_fractionDigits:uint;
				
		protected var m_NumberClipArray:Array;
		protected var m_NumberFixedCellArray:Array;
		protected var m_NumberFloatCellArray:Array;
		protected var m_NumberClipClass:Class;
		protected var m_NumberClipWidth:int;
		protected var m_NumberClipHeight:int;

		public function NumberText(c:Class,
								   value:Number = 0,
								   fractionDigits:uint = 0,
								   w:int =  14,
								   h:int =  14)
		{
			super();
			m_NumberClipClass = c;
			m_NumberClipWidth = w;
			m_NumberClipHeight= h;
			m_NumberClipArray = new Array;
			m_fractionDigits  = fractionDigits;
			SetValue(value);
		}
		public function Destroy():void
		{
			for(var i:int = 0; i < m_NumberClipArray.length; i ++)
			{
				removeChild(m_NumberClipArray[i]);
				m_NumberClipArray[i] = null;
			}
			m_NumberClipArray = null;
			m_NumberClipClass = null;
			m_NumberFixedCellArray = null;
			m_NumberFloatCellArray = null;
		}
		public function SetValue(value:Number):void
		{
			m_fValue = value;
			m_NumberFixedCellArray = new Array;
			m_NumberFloatCellArray = new Array;
			var nIndex:uint=0;
			if(m_fractionDigits > 0)
			{
				var fValue:Number = m_fValue;
				var nfractionDigits:int = m_fractionDigits;
				while(nfractionDigits > 0)
				{
					fValue *= 10;
					nfractionDigits --;
				}
				var n:int = fValue;
				nfractionDigits = m_fractionDigits;
				while (nfractionDigits>0)
				{
					m_NumberFloatCellArray[nIndex++]=n%10;
					n/=10;
					nfractionDigits --;
				}
			}
			
			nIndex=0;
			var n:int = m_fValue;
			
			if(n == 0)
			{
				m_NumberFixedCellArray[nIndex++]=0;
			}
			else
			{
				while (n>0)
				{
					m_NumberFixedCellArray[nIndex++]=n%10;
					n/=10;
				}
			}

			//trace(m_NumberFixedCellArray.toString());
			//trace(m_NumberFloatCellArray.toString());
			
			var nCellCount:int = m_NumberFixedCellArray.length;
			if(m_NumberFloatCellArray.length > 0)
				nCellCount += (m_NumberFloatCellArray.length + 1);
			if(nCellCount > m_NumberClipArray.length)
			{
				var nDelta:int = nCellCount - m_NumberClipArray.length;
				while(nDelta > 0)
				{
					var mc:MovieClip = new m_NumberClipClass;
					m_NumberClipArray.push(mc);
					addChild(mc);
					
					nDelta --;
				}
			}
			else if(nCellCount < m_NumberClipArray.length)
			{
				var nDelta:int = m_NumberClipArray.length - nCellCount;
				var deleteMC:Array = m_NumberClipArray.splice(nCellCount, nDelta);
				
				for(var i:int = 0; i < deleteMC.length; i ++)
				{
					removeChild(deleteMC[i]);
					deleteMC[i] = null;
				}
				
				deleteMC = null;
			}
			
			nIndex=0;
			if(m_NumberFloatCellArray.length)
			{
				for(var i:int = 0; i < m_NumberFloatCellArray.length; i ++)
				{
					m_NumberClipArray[nIndex++].gotoAndStop(m_NumberFloatCellArray[i] + 1);
				}
				m_NumberClipArray[nIndex++].gotoAndStop(11);
			}
			
			if(m_NumberFixedCellArray.length)
			{
				for(var i:int = 0; i < m_NumberFixedCellArray.length; i ++)
				{
					m_NumberClipArray[nIndex++].gotoAndStop(m_NumberFixedCellArray[i] + 1);
				}
			}
			
			var xp:Number = (m_NumberClipArray.length * m_NumberClipWidth / 2);
			for(var i:int = 0; i < m_NumberClipArray.length; i ++)
			{
				m_NumberClipArray[i].x = xp - m_NumberClipWidth * i;
			}
			
		}
		public function GetValue():Number
		{
			return m_fValue;
		}
		public function MoveWindow(xp:int,yp:int):void
		{
			this.x = xp;
			this.y = yp;
		}
	}
}