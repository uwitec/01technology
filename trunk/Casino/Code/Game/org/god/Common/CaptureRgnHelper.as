package org.god.Common
{
	import flash.display.Sprite;
	import flash.display.Shape;
	import flash.events.MouseEvent;
	import flash.events.EventDispatcher;
	import flash.geom.Point;
	//鼠标选择区域帮助类
	public class CaptureRgnHelper extends EventDispatcher
	{
		private var m_CaptureRgnSprite:Sprite;
		private var m_bDrawRgnFlag:Boolean;
		private var m_ptMouseDownPoint:Point;
		private var m_Square:Shape;
		
		public function CaptureRgnHelper(CaptureRgnSprite:Sprite)
		{
			m_CaptureRgnSprite = CaptureRgnSprite;
			m_ptMouseDownPoint = new Point;
			m_Square		   = new Shape;
				
			var squareSize:uint = 100;
			m_Square.graphics.beginFill(0xFFFFFF, 0.5);
			m_Square.graphics.lineStyle(1, 0.5,0x1010EE);
			m_Square.graphics.drawRect(0, 0, squareSize, squareSize);
			m_Square.graphics.endFill();

		}
		public function Create():Boolean
		{
			m_bDrawRgnFlag = false;
			m_CaptureRgnSprite.addEventListener(MouseEvent.MOUSE_DOWN, OnEventMouseDown);
			m_CaptureRgnSprite.addEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
			m_CaptureRgnSprite.addEventListener(MouseEvent.MOUSE_UP, OnEventMouseUp);
			m_CaptureRgnSprite.addEventListener(MouseEvent.ROLL_OUT, OnEventRollOut);
			return true;
		}
		public function Destroy():void
		{
			m_CaptureRgnSprite.removeEventListener(MouseEvent.MOUSE_DOWN, OnEventMouseDown);
			m_CaptureRgnSprite.removeEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
			m_CaptureRgnSprite.removeEventListener(MouseEvent.MOUSE_UP, OnEventMouseUp);
			m_CaptureRgnSprite.removeEventListener(MouseEvent.ROLL_OUT, OnEventRollOut);
			m_CaptureRgnSprite = null;
			m_Square = null;
		}
		protected function OnEventMouseDown(e:MouseEvent):void
		{
			if(m_bDrawRgnFlag == false)
			{
				m_ptMouseDownPoint.x = m_CaptureRgnSprite.mouseX;
				m_ptMouseDownPoint.y = m_CaptureRgnSprite.mouseY;
				
				
				m_Square.width = 0;
				m_Square.height= 0;
				m_Square.x = m_ptMouseDownPoint.x;
				m_Square.y = m_ptMouseDownPoint.y;
				m_CaptureRgnSprite.addChild(m_Square);
				
				m_bDrawRgnFlag = true;
			}
		}
		protected function OnEventMouseMove(e:MouseEvent):void
		{
			if(m_bDrawRgnFlag == true)
			{
				var w:Number = m_CaptureRgnSprite.mouseX - m_ptMouseDownPoint.x;
				var h:Number = m_CaptureRgnSprite.mouseY - m_ptMouseDownPoint.y;
				var x:Number = 0;
				var y:Number = 0;
				
				if(w > 0)
				{
					if(h > 0)
					{
						x = m_ptMouseDownPoint.x;
						y = m_ptMouseDownPoint.y;
					}
					else
					{
						x = m_ptMouseDownPoint.x;
						y = m_ptMouseDownPoint.y + h;
					}
				}
				else
				{
					if(h > 0)
					{
						x = m_ptMouseDownPoint.x + w;
						y = m_ptMouseDownPoint.y;
					}
					else
					{
						x = m_ptMouseDownPoint.x + w;
						y = m_ptMouseDownPoint.y + h;
					}
				}
				
				m_Square.x = x;
				m_Square.y = y;
				m_Square.width = Math.abs(w);
				m_Square.height= Math.abs(h);
				
				//trace("w:" + w + " h:" + h);
			}
		}
		protected function OnEventMouseUp(e:MouseEvent):void
		{
			if(m_bDrawRgnFlag == true)
			{
				var w:Number = m_CaptureRgnSprite.mouseX - m_ptMouseDownPoint.x;
				var h:Number = m_CaptureRgnSprite.mouseY - m_ptMouseDownPoint.y;
				var x:Number = 0;
				var y:Number = 0;
				
				if(w > 0)
				{
					if(h > 0)
					{
						x = m_ptMouseDownPoint.x;
						y = m_ptMouseDownPoint.y;
					}
					else
					{
						x = m_ptMouseDownPoint.x;
						y = m_ptMouseDownPoint.y + h;
					}
				}
				else
				{
					if(h > 0)
					{
						x = m_ptMouseDownPoint.x + w;
						y = m_ptMouseDownPoint.y;
					}
					else
					{
						x = m_ptMouseDownPoint.x + w;
						y = m_ptMouseDownPoint.y + h;
					}
				}

				dispatchEvent(new CaptureRgnEvent(x,y,Math.abs(w),Math.abs(h)));
				m_CaptureRgnSprite.removeChild(m_Square);
				m_bDrawRgnFlag = false;
			}
		}
		protected function OnEventRollOut(e:MouseEvent):void
		{
			if(m_bDrawRgnFlag == true)
			{
				var w:Number = m_CaptureRgnSprite.mouseX - m_ptMouseDownPoint.x;
				var h:Number = m_CaptureRgnSprite.mouseY - m_ptMouseDownPoint.y;
				var x:Number = 0;
				var y:Number = 0;
				
				if(w > 0)
				{
					if(h > 0)
					{
						x = m_ptMouseDownPoint.x;
						y = m_ptMouseDownPoint.y;
					}
					else
					{
						x = m_ptMouseDownPoint.x;
						y = m_ptMouseDownPoint.y + h;
					}
				}
				else
				{
					if(h > 0)
					{
						x = m_ptMouseDownPoint.x + w;
						y = m_ptMouseDownPoint.y;
					}
					else
					{
						x = m_ptMouseDownPoint.x + w;
						y = m_ptMouseDownPoint.y + h;
					}
				}

				dispatchEvent(new CaptureRgnEvent(x,y,Math.abs(w),Math.abs(h)));
				m_CaptureRgnSprite.removeChild(m_Square);
				m_bDrawRgnFlag = false;
			}
		}
	}
}