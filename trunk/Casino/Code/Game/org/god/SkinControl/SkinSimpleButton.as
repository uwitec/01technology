package org.god.SkinControl
{
	import flash.display.SimpleButton;
	import flash.display.DisplayObject;
	import flash.geom.Rectangle;

	public class SkinSimpleButton extends SimpleButton
	{
		protected var m_up:DisplayObject;
		protected var m_disabled:DisplayObject;
		protected var m_rect:Rectangle;
		
		protected var m_Old_upState:DisplayObject;
		protected var m_Old_overState:DisplayObject;
		protected var m_Old_downState:DisplayObject;
		protected var m_Old_hitTestState:DisplayObject;
			
		public function SkinSimpleButton(up:DisplayObject,
								  over:DisplayObject,
								  down:DisplayObject,
								  disabled:DisplayObject,
								  w:Number, h:Number)
		{
			super();
			SetButtonSkin(up,
						over,
						down,
						disabled,
						w, 
						h);
		}
		public function SetButtonSkin(up:DisplayObject,
								  over:DisplayObject,
								  down:DisplayObject,
								  disabled:DisplayObject,
								  w:Number, h:Number):void
		{
			m_rect = new Rectangle(0,0,w,h);
			
			m_up = up;
			m_disabled = disabled;
			
			if(m_Old_upState == null)
			{
				m_Old_upState = this.upState;
				m_Old_overState = this.overState;
				m_Old_downState = this.downState;
				m_Old_hitTestState = this.hitTestState;
			}
			
			this.upState =  up;
			this.overState =  over;
			this.downState =  down;
			this.hitTestState =  down;

		}
		public function Destroy():void
		{
			m_rect = null;
			m_up = null;
			m_disabled = null;
			this.upState =  m_Old_upState;
			this.overState =  m_Old_overState;
			this.downState =  m_Old_downState;
			this.hitTestState =  m_Old_hitTestState;
			m_Old_upState = null;
			m_Old_overState = null;
			m_Old_downState = null;
			m_Old_hitTestState = null;
		}
		public function ShowWindow(bShow:Boolean):void
		{
			this.visible = bShow;
		}
		public function EnableWindow(bEnable:Boolean):void
		{
			this.upState = bEnable ? ( m_up) : ( m_disabled);
			this.enabled = bEnable;
			this.mouseEnabled  = bEnable;
		}
		public function IsWindowVisible():Boolean
		{
			return this.visible;
		}
		public function IsWindowEnabled():Boolean
		{
			return this.enabled;
		}
		public function SetFocus():void
		{
		}
		public function MoveWindow(x:Number, y:Number):void
		{
			this.x = x;
			this.y = y;
		}
		public  function getButtonRect():Rectangle
		{
			return m_rect;
		}
	}
}