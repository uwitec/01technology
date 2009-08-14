package 
{
	import flash.display.MovieClip;
	import flash.display.DisplayObjectContainer;
	import flash.events.IOErrorEvent;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	import flash.net.URLRequest;
	import flash.display.Loader;
	import flash.events.MouseEvent;
	
	import org.god.Common.GameModuleNameFactory;
	import org.god.SkinControl.SkinButton;
	
	import org.aswing.JScrollPane;
	import org.aswing.Component;
	//游戏帮助视图
	public class GameHelpView extends MovieClip
	{
		private var m_mcParent:DisplayObjectContainer;//父显示容器
		private var m_btnOK:SkinButton;//确定按钮
		private var m_wProcessType:uint;//游戏类型
		private var m_HelpLoader:Loader;//游戏帮助下载器
		private var m_HelpComponent:Component;//游戏帮助
		private var m_scollPane:JScrollPane;//滚动面板
		//构造函数
		public function GameHelpView(mcParent:DisplayObjectContainer,
								  wProcessType:uint)
		{
			m_mcParent = mcParent;
			m_wProcessType = wProcessType;
			m_btnOK=new SkinButton(IDS_OK, 640, 15);
			m_btnOK.addEventListener(MouseEvent.CLICK, OnEventOK);
			addChild(m_btnOK);
			this.x = 1024/2 - 746/2;
			this.y = 768/2 - 520/2;
			m_HelpComponent = new Component;
			m_scollPane = new JScrollPane(m_HelpComponent);
			m_scollPane.setVisible(false);
			addChild(m_scollPane);
			m_scollPane.setComBoundsXYWH(19, 58, 710, 440);
			ShowHelp(wProcessType);
		}
		//显示
		public function DoModal():void
		{
			m_mcParent.addChild(this);
		}
		//销毁
		public function Destroy():void
		{
			if(m_HelpLoader != null)
			{
				removeChild(m_scollPane);
				m_HelpLoader.contentLoaderInfo.removeEventListener(ProgressEvent.PROGRESS,OnEventProgressHandler);
				m_HelpLoader.contentLoaderInfo.removeEventListener(Event.INIT,OnEventCompleteHandler);
				m_HelpLoader.contentLoaderInfo.removeEventListener(IOErrorEvent.IO_ERROR, OnEventIOErrorHandler);
				m_HelpLoader = null;
				m_scollPane = null;
				m_HelpComponent = null;
			}
			m_mcParent.removeChild(this);
			m_mcParent = null;
			removeChild(m_btnOK);
			m_btnOK.removeEventListener(MouseEvent.CLICK, OnEventOK);
			m_btnOK = null;
			
		}
		//确定按钮事件
		private function OnEventOK(e:Event):void
		{
			Destroy();
		}
		//显示游戏帮助
		private function ShowHelp(wProcessType:uint):void
		{
			if(m_HelpLoader == null)
			{
				m_HelpLoader = new Loader();
				m_HelpLoader.contentLoaderInfo.addEventListener(ProgressEvent.PROGRESS,OnEventProgressHandler);
				m_HelpLoader.contentLoaderInfo.addEventListener(Event.INIT,OnEventCompleteHandler);
				m_HelpLoader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, OnEventIOErrorHandler);

				var strURL:String = GameModuleNameFactory.GetGameHelpContext(wProcessType);
				var urlReq:URLRequest = new URLRequest(strURL);
				m_HelpLoader.load(urlReq);
				m_HelpLoader.x = 0;
				m_HelpLoader.y = 0;
				m_HelpComponent.addChild(m_HelpLoader);
			}
		}
		//下载游戏帮助进度事件
		private function OnEventProgressHandler(e:ProgressEvent):void
		{
			var nPos:int = e.bytesLoaded * 100 / e.bytesTotal;
		}
		//下载游戏帮助完成事件
		private function OnEventCompleteHandler(e:Event):void
		{
			m_mcLoadProgress.visible = false;
			m_HelpComponent.setComBoundsXYWH(0,0,m_HelpLoader.contentLoaderInfo.width,
											 m_HelpLoader.contentLoaderInfo.height);
			m_scollPane.setVisible(true);
		}
		//下载游戏帮助失败事件
		private function OnEventIOErrorHandler(e:Event):void
		{
			trace(e);
		}
	}
}
include "../../StringTableDef.as"