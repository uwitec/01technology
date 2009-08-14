package org.god.Common
{
	import org.aswing.util.HashMap;
	//时间对应表
	public class TimerMap
	{
		public var m_timerMap:HashMap;
		//构造函数
		public function TimerMap():void
		{
			m_timerMap = new HashMap;
		}
		//销毁
		public function Destroy():void
		{
			var arr:Array = m_timerMap.values();
			for(var i:uint = 0; i < arr.length; i ++)
			{
				arr[i].kill();
			}
			m_timerMap.clear();
			m_timerMap = null;
		}
		//设置定时
		public function SetTimer(nIDEvent:int,
								 uElapse:int,
								 listener:Function):int
		{
			var timer:TimerPair = m_timerMap.get(nIDEvent);
			if(timer == null)
			{
				timer = new TimerPair(nIDEvent,
									  listener,
									  uElapse);
				m_timerMap.put(nIDEvent, timer);
				timer.start();
				
				return nIDEvent;
			}
			else
			{
				timer.restart(uElapse,listener);

				return nIDEvent;
			}
		}
		//停止定时
		public function KillTimer(nIDEvent:int):void
		{
			var timer:TimerPair = m_timerMap.get(nIDEvent);
			if(timer)
			{
				timer.kill();
				m_timerMap.remove(nIDEvent);
				timer = null;
			}
		}
	}
}