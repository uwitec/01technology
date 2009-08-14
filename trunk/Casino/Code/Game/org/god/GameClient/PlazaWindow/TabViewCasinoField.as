package 
{
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	
	import org.god.Common.Media.MediaPlayer;
	import org.god.SkinControl.SkinButton;
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.GlobalMain;

	import org.aswing.Component;
	import org.aswing.ASFont;
	//现场标签
	public class TabViewCasinoField extends Component
	{
		private var m_mcParent:ServerTypeTab;//父亲服务类型页
		
		protected var m_btnPlay:SkinButton;//播放按钮
		protected var m_btnStop:SkinButton;//停止按钮
		protected var m_pMediaPlayer:MediaPlayer;//播放器
		protected var m_Timer:Timer;//定时器
		
		private var m_strVideoUrl:String;//视频播发地址
		//创建
		static public function CreateTabViewCasinoField(url:String,
		mcParent:ServerTypeTab)
		{
			return new TabViewCasinoField(url,mcParent);
		}
		//构造函数
		public function TabViewCasinoField(url:String,mcParent:ServerTypeTab)
		{
			super();
			m_mcParent = mcParent;
			m_strVideoUrl = String(url);
			
			var font:ASFont = new ASFont;
			font = font.changeSize(12);
			
			var nXP:int = 675;
			var nYP:int = 220;
			
			m_btnPlay = new SkinButton(IDS_PLAYVIDEO, nXP, nYP);
			addChild(m_btnPlay);
			m_btnPlay.addEventListener(MouseEvent.CLICK, OnEventPlay);
			m_btnPlay.setFont(font);
			
			m_btnStop = new SkinButton(IDS_STOPVIDEO, nXP, nYP + 50);
			addChild(m_btnStop);
			m_btnStop.addEventListener(MouseEvent.CLICK, OnEventStop);
			m_btnStop.setFont(font);
			
			m_pMediaPlayer = new MediaPlayer(640,480);
			addChild(m_pMediaPlayer);
			m_pMediaPlayer.x = 10;
			m_pMediaPlayer.y = 27;
			
			m_Timer = new Timer(1000 * 60 * 10, 1);
			m_Timer.addEventListener(TimerEvent.TIMER, OnEventTimer);
		}
		//销毁
		public function Destroy():void
		{
			removeChild(m_btnPlay);
			m_btnPlay.removeEventListener(MouseEvent.CLICK, OnEventPlay);
			m_btnPlay = null;
			removeChild(m_btnStop);
			m_btnStop.removeEventListener(MouseEvent.CLICK, OnEventStop);
			m_btnStop = null;
			m_Timer.stop();
			m_Timer.removeEventListener(TimerEvent.TIMER, OnEventTimer);
			m_Timer = null;
			m_pMediaPlayer.Stop();
			removeChild(m_pMediaPlayer);
			m_pMediaPlayer.Destroy();
			m_pMediaPlayer = null;
			m_mcParent = null;
		}
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			var main:IMain = GlobalMain.getMain();
			if(main && main.getGameInfo())
			{
				m_strVideoUrl = main.getGameInfo().szFieldLiveVideoAddress;
			}
		}
		//播放按钮事件
		protected function OnEventPlay(e:MouseEvent):void
		{
			if(m_strVideoUrl.length == 0)
				return;
			if(m_strVideoUrl == "rtmp://")
				return;
			if(m_strVideoUrl.lastIndexOf("rtmp://") == -1)
				return;
			m_pMediaPlayer.Play(m_strVideoUrl);
		}
		//停止按钮事件
		protected function OnEventStop(e:MouseEvent):void
		{
			m_Timer.stop();
			m_pMediaPlayer.Stop();
		}
		//定时器事件
		protected function OnEventTimer(e:TimerEvent):void
		{
			if(m_pMediaPlayer != null)
				m_pMediaPlayer.Stop();
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"