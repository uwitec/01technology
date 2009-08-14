package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	
	import org.god.GameModule.Common.INetSpeedView;
	
	public class NetSpeedView extends MovieClip implements INetSpeedView
	{
		private static const nMaxNetDelayScale:int = 5;
		
		private var m_wNetDelay:int;
		private var m_timer:Timer;
		
		public function NetSpeedView()
		{
			super();
			setNetSpeed(nMaxNetDelayScale * 100);
			m_timer = new Timer(1000);
			m_timer.addEventListener(TimerEvent.TIMER, OnEventTimer);
			m_timer.start();
		}
		public function setNetSpeed(nNetDelay:int):void
		{
			var p:int = nMaxNetDelayScale * 100 - nNetDelay;
			p = p / 5;
			if(p < 1)
				p = 1;
			else if(p > 100)
				p = 100;
			gotoAndStop(p);
			m_wNetDelay = nNetDelay;
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
		private function OnEventTimer(e:TimerEvent):void
		{
			if(m_wNetDelay >= nMaxNetDelayScale * 100)
				return;
			var wOldDelay:int = m_wNetDelay;
			var wNetDelay:int = m_wNetDelay + GetRandNetDelayOffset(GetRandBound());
			setNetSpeed(wNetDelay);
			m_wNetDelay = wOldDelay;
		}
		private function GetRandBound():int
		{
			var wBound:int = (Math.random() * 10000) % 100;
			if(wBound < 70)
				wBound = 20;
			else
				wBound = (Math.random() * 10000) % 30 + 40;
			return wBound;
		}
		private function GetRandNetDelayOffset(wBound:int = 20):int
		{
			if((Math.random() * 10000) % 2 == 0)
				return -((Math.random() * 10000) % 10 * nMaxNetDelayScale);
			else
				return (Math.random() * 10000) % 10 * nMaxNetDelayScale;
		}
	}
}