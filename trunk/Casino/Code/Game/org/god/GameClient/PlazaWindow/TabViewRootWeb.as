package 
{
	import flash.display.MovieClip;
	import flash.events.IOErrorEvent;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	import flash.net.URLRequest;
	import flash.display.Loader;
	
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.GlobalMain;

	import org.aswing.Component;
	//首页标签
	public class TabViewRootWeb extends Component
	{
		private var m_mcParent:ServerTypeTab;//父亲服务类型页
		private var m_RootWeb:Loader;//首页装载器
		//创建首页视图面板
		static public function CreateTabViewRootWeb(url:String,
		mcParent:ServerTypeTab)
		{
			return new TabViewRootWeb(url,mcParent);
		}
		//构造函数
		public function TabViewRootWeb(url:String,mcParent:ServerTypeTab)
		{
			super();
			m_mcParent = mcParent;
			m_mcLoadProgress.visible = true;
		}
		//销毁
		public function Destroy():void
		{
			if(m_RootWeb != null)
			{
				m_RootWeb.contentLoaderInfo.removeEventListener(ProgressEvent.PROGRESS,OnEventProgressHandler);
				m_RootWeb.contentLoaderInfo.removeEventListener(Event.INIT,OnEventCompleteFirstHandler);
				m_RootWeb.contentLoaderInfo.removeEventListener(IOErrorEvent.IO_ERROR, OnEventIOErrorHandler);
				removeChild(m_RootWeb);
				m_RootWeb = null;
			}
			m_mcParent = null;
		}
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			var main:IMain = GlobalMain.getMain();
			if(main && main.getGameInfo())
			{
				var szMainWebPageAddress:String = main.getGameInfo().szMainWebPageAddress;
				if(szMainWebPageAddress.length == 0)
					return;
				if(szMainWebPageAddress == "http://")
					return;
				if(szMainWebPageAddress.lastIndexOf("http://") == -1)
					return;
				ShowRootWeb(szMainWebPageAddress);
			}
		}
		//显示首页
		private function ShowRootWeb(strURL:String):void
		{
			if(m_RootWeb == null)
			{
				m_RootWeb = new Loader();
				m_RootWeb.contentLoaderInfo.addEventListener(ProgressEvent.PROGRESS,OnEventProgressHandler);
				m_RootWeb.contentLoaderInfo.addEventListener(Event.INIT,OnEventCompleteFirstHandler);
				m_RootWeb.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, OnEventIOErrorHandler);
				var urlReq:URLRequest = new URLRequest(strURL);
				m_RootWeb.load(urlReq);
				addChild(m_RootWeb);
				m_RootWeb.visible = false;
				m_RootWeb.x = 0;
				m_RootWeb.y = 0;
			}
		}
		//装载首页进度事件
		private function OnEventProgressHandler(e:ProgressEvent):void
		{
			var nPos:int = e.bytesLoaded * 100 / e.bytesTotal;
			
		}
		//装载首页完成事件
		private function OnEventCompleteFirstHandler(e:Event):void
		{
			m_RootWeb.visible = true;
			m_mcLoadProgress.visible = false;
		}
		//装载首页错误事件
		private function OnEventIOErrorHandler(e:Event):void
		{
			trace(e);
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"