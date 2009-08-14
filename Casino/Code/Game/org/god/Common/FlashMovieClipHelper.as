package org.god.Common
{
	import flash.display.MovieClip;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	//闪烁单元帮助
	public class FlashMovieClipHelper
	{
		private var m_mcParent:MovieClip;//父影片
		private var m_claFlash:Class;//闪烁影片
		private var m_nCount:uint;//闪烁数目
		private var m_nFlashMaxSecond:uint;//闪烁最大时间秒
		private var m_nWidth:uint;//宽度
		private var m_nHeight:uint;//高度
		
		private var m_Timer:Timer;//定时器
		private var m_FlashMCArray:Array;//闪烁影片数组
		
		private var m_bStart:Boolean;//开始标记
		//构造函数
		public function FlashMovieClipHelper(mcParent:MovieClip,
											 claFlash:Class,
											 nCount:uint=4,
											 nWidth:uint=1024,
											 nHeight:uint=768)
		{
			m_mcParent = mcParent;
			m_claFlash = claFlash;
			m_nCount   = nCount;
			m_nWidth   = nWidth;
			m_nHeight  = nHeight;
			
			m_bStart   = false;
		}
		//开始
		public function Start():void
		{
			if(m_bStart)
				return;
			m_FlashMCArray = new Array(m_nCount);
			for(var i:uint = 0; i < m_nCount; i ++)
			{
				m_FlashMCArray[i] = new m_claFlash;
				m_FlashMCArray[i].x = (Math.random() * 1000000) % m_nWidth;
				m_FlashMCArray[i].y = (Math.random() * 1000000) % m_nHeight;
				m_mcParent.addChild(m_FlashMCArray[i]);
				m_FlashMCArray[i].play();
			}
			m_Timer = new Timer(1000);
			m_Timer.addEventListener(TimerEvent.TIMER, OnEventTimer);
			m_Timer.start();
			m_bStart = true;
		}
		//停止
		public function Stop():void
		{
			if(!m_bStart)
				return;
			m_Timer.stop();
			m_Timer.removeEventListener(TimerEvent.TIMER, OnEventTimer);
			m_Timer = null;
			for(var i:uint = 0; i < m_nCount; i ++)
			{
				m_mcParent.removeChild(m_FlashMCArray[i]);
				m_FlashMCArray[i] = null;
			}
			m_FlashMCArray	   = null;
			m_bStart = false;
		}
		//暂停
		public function Pause():void
		{
			if(m_Timer&&m_Timer.running)
				m_Timer.stop();
		}
		//恢复
		public function Resume():void
		{
			if(m_Timer&&m_Timer.running==false)
				m_Timer.start();
		}
		//定时器事件-闪烁
		private function OnEventTimer(e:TimerEvent):void
		{
			for(var i:uint = 0; i < m_nCount; i ++)
			{
				if(m_FlashMCArray[i].currentFrame >= m_FlashMCArray[i].totalFrames)
				{
					if(uint((Math.random() *1000)%3) == 0)
					{
						m_FlashMCArray[i].x = (Math.random() * 1000000) % m_nWidth;
						m_FlashMCArray[i].y = (Math.random() * 1000000) % m_nHeight;
						m_FlashMCArray[i].gotoAndPlay(1);
					}
				}
			}
		}
	}
}