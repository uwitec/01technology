package org.god.Common.Media
{
	import flash.media.SoundMixer;
	import flash.events.EventDispatcher;
	//声音管理
	public class SoundManager extends EventDispatcher
	{
		public static var g_SoundManager:SoundManager = null;
		
		protected var m_bQuiet:Boolean;
	
		public function SoundManager()
		{
			m_bQuiet = false;
		}
		public static function GetGlobalSoundManager():SoundManager
		{
			if(g_SoundManager == null)
				g_SoundManager = new SoundManager;
			return g_SoundManager;
		}
		public function PlaySound(s:SoundPlayer,bRepeat:Boolean = false):Boolean
		{
			if(m_bQuiet == true || s == null)
				return false;
			return s.Play(bRepeat);
		}
		public function StopSound(s:SoundPlayer):void
		{
			if(s == null)
				return;
			s.Stop();
		}
		public function SetQuiet(b:Boolean):void
		{
			m_bQuiet = b;
			if(b)
				SoundMixer.stopAll();
			dispatchEvent(new QuietEvent(m_bQuiet));
		}
		public function IsQuiet():Boolean
		{
			return m_bQuiet;
		}
	}
}