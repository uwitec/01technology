package org.god.SkinControl.SkinScrollTextField
{
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	import flash.text.TextFormat;
	import flash.events.MouseEvent;
	
	import org.aswing.Container;
	import org.aswing.JLabel;
	import org.aswing.geom.IntDimension;
	import org.aswing.geom.IntPoint;
	import org.aswing.AsWingUtils;
	
	public class ScrollTextPane extends Container
	{
		public static const eDirect_Top:int  = 0;
		public static const eDirect_Left:int = 1;
		protected static const nTextDelta_Horizontal:int  = 32;
		protected static const nTextDelta_Vertical:int 	  = 12;
		protected static const nTextScroll_Speed:int 	  = 5;
		
		protected var m_eDirect:int;
		protected var m_bMultiLine:Boolean;
		protected var m_nTextAreaW:int;
		protected var m_nTextAreaH:int;
		
		protected var m_bRequstStart:Boolean;
		protected var m_timer:flash.utils.Timer;
		protected var m_arrLabel:Array;
		protected var m_nSingleTextAreaCX:int;
		protected var m_nSingleTextAreaCY:int;
		protected var m_nLabelYPos:int;
		
		public function ScrollTextPane(eDirect:int = 0,
										bMultiLine:Boolean = false,
										nScrollDelay:int = 500)
		{
			super();

			m_eDirect 		= eDirect;
			m_bMultiLine 	= bMultiLine;
			
			m_timer = new flash.utils.Timer(nScrollDelay);
			m_timer.addEventListener(TimerEvent.TIMER, OnEventTimer);
			
			addEventListener(MouseEvent.ROLL_OVER, OnEventMouseRollOver);
			addEventListener(MouseEvent.ROLL_OUT, OnEventMouseRollOut);
		}
		public override function setSizeWH(w:int, h:int):void
		{
			super.setSizeWH(w,h);
			m_nTextAreaW	= w;
			m_nTextAreaH	= h;
		}
		public function UpdateText(strText:String):void
		{
			if(m_arrLabel != null)
			{
				for(var i:int = 0; i < m_arrLabel.length; i ++)
				{
					removeChild(m_arrLabel[i]);
					m_arrLabel[i] = null;
				}
			}
			var textFormat:TextFormat = getFont().getTextFormat();
			var textS:IntDimension = AsWingUtils.computeStringSize(textFormat, strText);
			
			var strNewText:String = strText;
			if(m_bMultiLine)
			{
				var nLineCount:int = Math.ceil(textS.width / m_nTextAreaW);
				var nMaxLineChar:int = strText.length / nLineCount;
				
				var strTmp:String = "";
				var strTmp1:String = "";
				var index:int = 0;
				for(var i:int = 0; i < nLineCount; i ++)
				{
					strTmp1 = strText.substring(index, index + nMaxLineChar);
					strTmp += strTmp1;
					strTmp += ("\n");
					index += nMaxLineChar;
				}
				strTmp1 =strText.substring(index, strText.length);
				strTmp += strTmp1;
				
				strNewText = strTmp;
				
				textS = AsWingUtils.computeStringSize(textFormat, strNewText);
			}
			
			m_nSingleTextAreaCY = textS.height + nTextDelta_Vertical;
			m_nSingleTextAreaCX = textS.width + nTextDelta_Horizontal;
			m_nLabelYPos		= m_nTextAreaH/2 - textS.height/2;
			
			m_arrLabel  = new Array(ComputeLabelCount(textS));
			for(var i:int = 0; i < m_arrLabel.length; i ++)
			{
				m_arrLabel[i] = new JLabel(strNewText, null, JLabel.CENTER);
				m_arrLabel[i].setFont(getFont());
				m_arrLabel[i].setForeground(getForeground());
				addChild(m_arrLabel[i]);
				switch(m_eDirect)
				{
					case eDirect_Top:
					{
						m_arrLabel[i].setLocationXY(0, m_nTextAreaH + m_nSingleTextAreaCY * i );
						m_arrLabel[i].setSizeWH(m_nTextAreaW,m_nSingleTextAreaCY);
						break;
					}
					case eDirect_Left:
					{
						m_arrLabel[i].setLocationXY(m_nTextAreaW + m_nSingleTextAreaCX * i,m_nLabelYPos);
						m_arrLabel[i].setSizeWH(m_nSingleTextAreaCX + nTextDelta_Vertical,m_nTextAreaH);
						break;
					}
					default:
					break;
				}
			}
		}
		public function Start():void
		{
			m_bRequstStart = true;
			m_timer.start();
		}
		public function Stop():void
		{
			m_bRequstStart = false;
			m_timer.stop();
		}
		public function Destroy():void
		{
			m_timer.stop();
			m_timer.removeEventListener(TimerEvent.TIMER, OnEventTimer);
			m_timer = null;
			
			if(m_arrLabel != null)
			{
				for(var i:int = 0; i < m_arrLabel.length; i ++)
				{
					removeChild(m_arrLabel[i]);
					m_arrLabel[i] = null;
				}
			}
			m_arrLabel = null;
		}
		protected function OnEventTimer(e:TimerEvent):void
		{
			if(m_arrLabel == null)
				return;
			switch(m_eDirect)
			{
				case eDirect_Top:
				{
					for(var i:int = 0; i < m_arrLabel.length; i ++)
					{
						var pos:IntPoint = m_arrLabel[i].getLocation();
						pos.y = pos.y - nTextScroll_Speed;
						m_arrLabel[i].setLocation(pos);
						if(pos.y <  (-m_nSingleTextAreaCY))
						{
							m_arrLabel[i].setLocationXY(0, GetLabelMaxYPos() + m_nSingleTextAreaCY);
						}
					}
					break;
				}
				case eDirect_Left:
				{
					for(var i:int = 0; i < m_arrLabel.length; i ++)
					{
						var pos:IntPoint = m_arrLabel[i].getLocation();
						pos.x = pos.x - nTextScroll_Speed;
						m_arrLabel[i].setLocation(pos);
						if(pos.x <  (-m_nSingleTextAreaCX))
						{
							m_arrLabel[i].setLocationXY(GetLabelMaxXPos() + m_nSingleTextAreaCX, m_nLabelYPos);
						}
					}
					break;
				}
				default:
				break;
			}
		}
		protected function GetLabelMaxYPos():int
		{
			var p:int = m_nTextAreaH;
			for(var i:int = 0; i < m_arrLabel.length; i ++)
			{
				var pos:IntPoint = m_arrLabel[i].getLocation();
				if(pos.y > p)
					 p = pos.y;
			}
			return p;
		}
		protected function GetLabelMaxXPos():int
		{
			var p:int = m_nTextAreaW;
			for(var i:int = 0; i < m_arrLabel.length; i ++)
			{
				var pos:IntPoint = m_arrLabel[i].getLocation();
				if(pos.x > p)
					 p = pos.x;
			}
			return p;
		}
		protected function ComputeLabelCount(textS:IntDimension):int
		{
			var nCount:int = 0;
			switch(m_eDirect)
			{
				case eDirect_Top:
				{
					nCount = Math.ceil(m_nTextAreaH / (textS.height + nTextDelta_Vertical));
					break;
				}
				case eDirect_Left:
				{
					nCount = Math.ceil(m_nTextAreaW / (textS.width + nTextDelta_Horizontal));
					break;
				}
				default:
				break;
			}
			return nCount + 1;
		}
		protected function OnEventMouseRollOver(e:MouseEvent):void
		{
			if(m_timer.running)
				m_timer.stop();
		}
		protected function OnEventMouseRollOut(e:MouseEvent):void
		{
			if(m_bRequstStart && m_timer.running == false)
				m_timer.start();
		}
	}
}