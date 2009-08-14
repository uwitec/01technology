package org.god.GameModule.Common.SlotWheel
{
	public class BaseScrollMotion
	{
		public static const STATE_STOP:uint 		= 0;//停止转动
		public static const STATE_SPEEDUP:uint 		= STATE_STOP + 1;//加速转动
		public static const STATE_SPEEDMAX:uint 	= STATE_SPEEDUP + 1;//最高速转动
		public static const STATE_SPEEDDOWN:uint 	= STATE_SPEEDMAX + 1;//减速转动
		public static const STATE_ADJUST:uint 		= STATE_SPEEDDOWN + 1;//调整缓冲转动
		
		protected var m_SpeedUpScrollEngine:SpeedUpScrollEngine;
		protected var m_SpeedMaxScrollEngine:SpeedMaxScrollEngine;
		protected var m_SpeedDownScrollEngine:SpeedDownScrollEngine;
		protected var m_AdjustScrollEngine:AdjustScrollEngine;
	
		protected var m_ScrollObj:*;
		protected var m_speedupDuration:Number;
		protected var m_speeddownDuration:Number;
		protected var m_scrollMaxValue:Number;
		protected var m_scrollAdjustValue:Number;
		protected var m_cbState:uint;
		protected var m_bStopScroll:Boolean;

		public function BaseScrollMotion(obj:*,
									 speedupDuration:Number,
									 speeddownDuration:Number,
									 scrollMaxValue:Number,
									 scrollAdjustValue:Number)
		{
			m_ScrollObj			= obj;
			m_speedupDuration 	= speedupDuration;
			m_speeddownDuration	= speeddownDuration;
			m_scrollMaxValue 	= scrollMaxValue;
			m_scrollAdjustValue = scrollAdjustValue;
			m_cbState			= STATE_STOP;
			
			m_SpeedUpScrollEngine	= new SpeedUpScrollEngine(this,obj,0,m_scrollMaxValue,m_speedupDuration);
			m_SpeedMaxScrollEngine	= new SpeedMaxScrollEngine(this,obj,m_scrollMaxValue,0);
			m_SpeedDownScrollEngine = new SpeedDownScrollEngine(this,obj,m_scrollMaxValue,0,speeddownDuration);
			m_AdjustScrollEngine 	= new AdjustScrollEngine(this,obj,scrollAdjustValue,0);
	
			m_SpeedUpScrollEngine.addEventListener(ScrollEngineEvent.MOTION_START,OnEvent_SpeedUpMotionStart);
			m_SpeedUpScrollEngine.addEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_SpeedUpMotionFinish);
			m_SpeedUpScrollEngine.addEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_SpeedUpMotionFinish);	
			m_SpeedMaxScrollEngine.addEventListener(ScrollEngineEvent.MOTION_START,OnEvent_SpeedMaxMotionStart);
			m_SpeedMaxScrollEngine.addEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_SpeedMaxMotionFinish);
			m_SpeedMaxScrollEngine.addEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_SpeedMaxMotionFinish);
			m_SpeedDownScrollEngine.addEventListener(ScrollEngineEvent.MOTION_START,OnEvent_SpeedDownMotionStart);
			m_SpeedDownScrollEngine.addEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_SpeedDownMotionFinish);
			m_SpeedDownScrollEngine.addEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_SpeedDownMotionFinish);
			m_AdjustScrollEngine.addEventListener(ScrollEngineEvent.MOTION_START,OnEvent_AdjustMotionStart);
			m_AdjustScrollEngine.addEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_AdjustMotionFinish);
			m_AdjustScrollEngine.addEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_AdjustMotionFinish);
		
			m_SpeedMaxScrollEngine.looping = true;
			m_AdjustScrollEngine.looping = true;
		}

		public function Destroy():void
		{
			m_SpeedUpScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_START,OnEvent_SpeedUpMotionStart);
			m_SpeedUpScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_SpeedUpMotionFinish);
			m_SpeedUpScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_SpeedUpMotionFinish);	
			m_SpeedMaxScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_START,OnEvent_SpeedMaxMotionStart);
			m_SpeedMaxScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_SpeedMaxMotionFinish);
			m_SpeedMaxScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_SpeedMaxMotionFinish);
			m_SpeedDownScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_START,OnEvent_SpeedDownMotionStart);
			m_SpeedDownScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_SpeedDownMotionFinish);
			m_SpeedDownScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_SpeedDownMotionFinish);
			m_AdjustScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_START,OnEvent_AdjustMotionStart);
			m_AdjustScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_FINISH,OnEvent_AdjustMotionFinish);
			m_AdjustScrollEngine.removeEventListener(ScrollEngineEvent.MOTION_STOP,OnEvent_AdjustMotionFinish);

			m_SpeedUpScrollEngine.Destroy();
			m_SpeedUpScrollEngine = null;
			m_SpeedMaxScrollEngine.Destroy();
			m_SpeedMaxScrollEngine = null;
			m_SpeedDownScrollEngine.Destroy();
			m_SpeedDownScrollEngine = null;
			m_AdjustScrollEngine.Destroy();
			m_AdjustScrollEngine = null;
			m_ScrollObj = null;
		}
		
		public function SpeedUp():void
		{
			if(IsScrolling())
				return ;
			m_bStopScroll = false;
			if(m_SpeedDownScrollEngine.isPlaying)
				m_SpeedDownScrollEngine.stop();
			if(m_SpeedMaxScrollEngine.isPlaying)
				m_SpeedMaxScrollEngine.stop();
			if(m_AdjustScrollEngine.isPlaying)
				m_AdjustScrollEngine.stop();
			m_SpeedUpScrollEngine.start();
		}
		public function SpeedDown():void
		{
			m_bStopScroll = true;
		}
		public function GetState():uint
		{
			return m_cbState;
		}
		public function IsScrolling():Boolean
		{
			return m_cbState == STATE_STOP ? false : true;
		}
		////
		protected function OnEvent_SpeedUpMotionStart(e:ScrollEngineEvent):void
		{
			m_cbState			= STATE_SPEEDUP;
			OnEventScrollMotionStart();
			//trace("STATE_SPEEDUP Start");
		}
		protected function OnEvent_SpeedUpMotionFinish(e:ScrollEngineEvent):void
		{
			if(e.type == ScrollEngineEvent.MOTION_FINISH)
			{
				if(m_SpeedMaxScrollEngine.isPlaying == false)
					m_SpeedMaxScrollEngine.start();
			}
			else if(e.type == ScrollEngineEvent.MOTION_STOP)
			{
			}
		}
		protected function OnEvent_SpeedMaxMotionStart(e:ScrollEngineEvent):void
		{
			m_cbState			= STATE_SPEEDMAX;
			//trace("STATE_SPEEDMAX Start");
		}
		protected function OnEvent_SpeedMaxMotionFinish(e:ScrollEngineEvent):void
		{
			if(e.type == ScrollEngineEvent.MOTION_FINISH)
			{
			}
			else if(e.type == ScrollEngineEvent.MOTION_STOP)
			{
			}
		}

		protected function OnEvent_SpeedDownMotionStart(e:ScrollEngineEvent):void
		{
			m_cbState			= STATE_SPEEDDOWN;
			//trace("STATE_SPEEDDOWN Start");
			OnEventScrollMotionSpeedDown();
		}
		protected function OnEvent_SpeedDownMotionFinish(e:ScrollEngineEvent):void
		{
			if(e.type == ScrollEngineEvent.MOTION_FINISH)
			{
				if(m_AdjustScrollEngine.isPlaying == false)
					m_AdjustScrollEngine.start();
			}
			else if(e.type == ScrollEngineEvent.MOTION_STOP)
			{
			}
		}
		protected function OnEvent_AdjustMotionStart(e:ScrollEngineEvent):void
		{
			m_cbState			= STATE_ADJUST;
			//trace("STATE_ADJUST Start");
		}
		protected function OnEvent_AdjustMotionFinish(e:ScrollEngineEvent):void
		{
			if(e.type == ScrollEngineEvent.MOTION_FINISH)
			{
			}
			else if(e.type == ScrollEngineEvent.MOTION_STOP)
			{
			}
			m_cbState			= STATE_STOP;
			OnEventScrollMotionFinish();
		}
		////
		public function OnScrollLoop(engine:BaseScrollEngine,p:Number):void
		{
			if(engine == m_SpeedMaxScrollEngine)
			{
				if(m_bStopScroll)
				{
					if(m_SpeedUpScrollEngine.isPlaying)
						m_SpeedUpScrollEngine.stop();
					if(m_SpeedMaxScrollEngine.isPlaying)
						m_SpeedMaxScrollEngine.stop();
					if(m_AdjustScrollEngine.isPlaying)
						m_AdjustScrollEngine.stop();
					if(m_SpeedDownScrollEngine.isPlaying == false)
						m_SpeedDownScrollEngine.start();
				}
			}
			else if(engine == m_SpeedDownScrollEngine)
			{
				if(p < m_scrollAdjustValue)
				{
					p = m_scrollAdjustValue;
				}
			}
			if(OnEventScrollMotion(p)&& 
			(engine == m_AdjustScrollEngine) && 
			m_bStopScroll)
			{
				if(m_SpeedUpScrollEngine.isPlaying)
					m_SpeedUpScrollEngine.stop();
				if(m_SpeedMaxScrollEngine.isPlaying)
					m_SpeedMaxScrollEngine.stop();
				if(m_SpeedDownScrollEngine.isPlaying)
					m_SpeedDownScrollEngine.stop();
				if(m_AdjustScrollEngine.isPlaying)
					m_AdjustScrollEngine.stop();			
			}
		}
		public function OnEventScrollMotionSpeedDown():void
		{
		}
		public function OnEventScrollMotion(p:Number):Boolean
		{
			return true;
		}
		public function OnEventScrollMotionStart():void
		{
		}
		public function OnEventScrollMotionFinish():void
		{
		}
	}
}