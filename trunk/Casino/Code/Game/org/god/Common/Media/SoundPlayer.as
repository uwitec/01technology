package org.god.Common.Media
{
	import flash.media.Sound;
	import flash.media.SoundChannel;
	//声音播放器
	public class SoundPlayer
	{
		protected var m_SoundData:Sound;
		protected var m_SoundChannel:SoundChannel;
		
		public function SoundPlayer()
		{
		}
		
		public function LoadLocalSoundData(c:Class):Boolean
		{
			if(c == null)
				return false;
			m_SoundData = new c;
			return true;
		}
		public function LoadNetSoundData(strURL:String):Boolean
		{
			return false;
		}
		public function Unload():void
		{
			m_SoundData = null;
			m_SoundChannel = null;
		}
		
		public function Play(bRepeat:Boolean = false):Boolean
		{
			if(m_SoundData == null)
				return false;
			m_SoundChannel = m_SoundData.play(0, bRepeat ? 0x7fffffff : 0);
			return true;
		}
		public function Stop():void
		{
			if(m_SoundData == null ||
			   m_SoundChannel == null)
				return;
			m_SoundChannel.stop();
		}
		
	}
}