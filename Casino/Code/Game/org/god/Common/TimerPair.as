package org.god.Common
{
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	//时间对子信息
	public class TimerPair
	{
		public var m_nIDEvent:int;
		public var m_handler:Function;
		public var m_timer:Timer;
		//构造函数
		public function TimerPair(nIDEvent:int,
								  handler:Function,
								  uElapse:int)
		{
			
			m_nIDEvent = nIDEvent;
			m_handler  = handler;
			m_timer    = new Timer(uElapse);
			m_timer.addEventListener("timer", timerHandler);
		}
		//开始
		public function start():void
		{
			m_timer.start();
		}
		//重新开始
		public function restart(uElapse:int, handler:Function):void
		{
			kill();
			m_handler  = handler;
			m_timer    = new Timer(uElapse);
			m_timer.addEventListener("timer", timerHandler);
			m_timer.start();
		}
		//停止
		public function stop():void
		{
			m_timer.stop();
		}
		//销毁
		public function kill():void
		{
			if(m_timer)
			{
				m_timer.removeEventListener("timer", timerHandler);
				m_timer.stop();
				m_timer = null;
				m_handler = null;
			}
		}
		//定时器事件处理
		private function timerHandler(e:TimerEvent):void
		{
			m_handler(m_nIDEvent);
		}
	}
}