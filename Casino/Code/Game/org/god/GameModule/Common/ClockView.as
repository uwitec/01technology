package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import org.god.GameModule.Common.IClockView;
	
	public class ClockView extends MovieClip implements IClockView
	{
		private var m_nSecond:int;
		public function ClockView(nSecond:int = 0)
		{
			super();
			createClockView(nSecond);
		}
		public function createClockView(nSecond:int):Boolean
		{
			setSecond(nSecond)
			return true;
		}
		public function destroy():void
		{
		}
		public function setSecond(nSecond:int):void
		{
			m_nSecond = nSecond;
			if(m_nSecond > 0)
			{
				this.visible = true;
				this.gotoAndStop(m_nSecond);
			}
			else
			{
				this.visible = false;
			}
		}
		public function getSecond():int
		{
			return m_nSecond;
		}
		
		public function getMovieClip():MovieClip
		{
			return this;
		}
		public function moveMovieClip(x:Number,y:Number):void
		{
			this.x = x;
			this.y = y;
		}
	}
}