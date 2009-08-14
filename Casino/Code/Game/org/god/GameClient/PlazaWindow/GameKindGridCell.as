package 
{
	import flash.events.TimerEvent;
	import flash.events.MouseEvent;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLRequest;
	import flash.display.MovieClip;
	import flash.display.Loader;
	import flash.utils.Timer;
	
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.ImageButton;
	import org.god.Net.tagGameServer;
	import org.god.IGameFrame.IEnterGameEventListener;
	import org.god.IGameFrame.IGamePaneImpl;
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.IPlazaWindow;
	import org.god.IGameFrame.GlobalMain;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.ServerItem;

	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.Component;
	import org.aswing.JLabel;
	import org.aswing.geom.IntRectangle;
	import org.aswing.util.LinkedList;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.graphics.Graphics2D;
	//游戏类型单元
	public class GameKindGridCell extends Component implements IEnterGameEventListener
	{
		public static const nDefaultWidth:int  = 260;
		public static const nDefaultHeight:int = 152;
		
		private var m_mcParent:TabViewGameKindGrid;//父标签
		private var m_comboServerList:JComboBox;//站点下拉框
		private var m_vecServerList:VectorListModel;//站点下拉框模型
		private var m_btnEnterGame:SkinButton;//进入游戏按钮
		private var m_labelGameKindName:JLabel;//游戏类型名称
		private var m_GameKindItem:ServerItem;//GameKind
		private var m_GameServerList:LinkedList;//GameServerList
		private var m_EnteringGamePane:IGamePaneImpl;//游戏面板接口
		private var m_timerCheckLogon:flash.utils.Timer;//登陆超时检测定时器
		private var m_MiniGameUI:Loader;//简图下载器
		private var m_btGameHelp:ImageButton;//游戏规则按钮
		private var m_labelGameKindOnLine:JLabel;//在线人数
		
		//创建
		static public function CreateGameKindGridCell(GameItem:ServerItem,
		mcParent:TabViewGameKindGrid)
		{
			return new GameKindGridCell(GameItem,mcParent);
		}
		//构造函数
		public function GameKindGridCell(GameItem:ServerItem,mcParent:TabViewGameKindGrid)
		{
			super();
			
			m_timerCheckLogon = new flash.utils.Timer(10000, 1);
			m_timerCheckLogon.addEventListener("timer", OnEventTimerHandler);

			m_GameKindItem = GameItem;
			m_mcParent = mcParent;
			
			m_vecServerList = new VectorListModel();
			m_comboServerList  = new JComboBox(m_vecServerList);
			addChild(m_comboServerList);
			m_comboServerList.x = 142;
			m_comboServerList.y = 64;
			m_comboServerList.width = 96;
			m_comboServerList.height = 24;
			m_vecServerList.append(IDS_AUTO);
			m_comboServerList.setSelectedIndex(0);
			
			m_GameServerList = new LinkedList();
			m_GameServerList.append(null);
			
			m_btnEnterGame = new SkinButton(IDS_ENTERGAME,155,94,
										   null,70, 26);
			addChild(m_btnEnterGame);
			m_btnEnterGame.addEventListener(MouseEvent.CLICK, OnEventEnterGame);
			m_btnEnterGame.setEnabled(false);
			
			m_labelGameKindOnLine = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelGameKindOnLine);
			m_labelGameKindOnLine.x = 142;
			m_labelGameKindOnLine.y = 120;
			m_labelGameKindOnLine.width = 90;
			m_labelGameKindOnLine.height = 22;
			var font:ASFont = new ASFont;
			font = font.changeSize(11);
			m_labelGameKindOnLine.setFont(font);
			var col:ASColor = new ASColor(0x0a0a0a);
			m_labelGameKindOnLine.setForeground(col);
			
			m_btGameHelp = new ImageButton(IMAGE_BT_GAMEHELP,160,28);
			addChild(m_btGameHelp);
			m_btGameHelp.MoveWindow(208,8);
			m_btGameHelp.addEventListener(MouseEvent.CLICK, OnEventHelp);
	
			m_labelGameKindName = new JLabel(m_GameKindItem.toString(),null,JLabel.LEFT);
			addChild(m_labelGameKindName);
			m_labelGameKindName.x = 36;
			m_labelGameKindName.y = 7;
			m_labelGameKindName.width = 168;
			m_labelGameKindName.height = 22;
			var font:ASFont = new ASFont;
			font = font.changeSize(22);
			m_labelGameKindName.setFont(font);
			var col:ASColor = new ASColor(0xf1e8a5);
			m_labelGameKindName.setForeground(col);

			UpdateGameKindOnLine(0);
		}
		//销毁
		public function Destroy():void
		{
			if(m_timerCheckLogon == null)
				return;
			m_timerCheckLogon.stop();
			m_timerCheckLogon.removeEventListener("timer", OnEventTimerHandler);
			m_timerCheckLogon = null;

			m_GameKindItem = null;
			m_mcParent = null;
			
			removeChild(m_comboServerList);
			m_comboServerList = null;
			m_vecServerList = null;
			
			m_GameServerList.clear();
			m_GameServerList = null;
			
			removeChild(m_btnEnterGame);
			m_btnEnterGame.removeEventListener(MouseEvent.CLICK, OnEventEnterGame);
			m_btnEnterGame = null;
			
			m_btGameHelp.removeEventListener(MouseEvent.CLICK, OnEventHelp);
			removeChild(m_btGameHelp);
			m_btGameHelp.Destroy();
			m_btGameHelp = null;
			
			removeChild(m_labelGameKindName);
			m_labelGameKindName = null;
			
			if(m_MiniGameUI != null)
			{
				m_MiniGameUI.contentLoaderInfo.removeEventListener(IOErrorEvent.IO_ERROR, OnEventMUIIOErrorHandler);
				removeChild(m_MiniGameUI);
				m_MiniGameUI = null;
			}
		}
		//更新游戏在线人数
		public function UpdateGameKindOnLine(dwOnLineCount:uint):void
		{
			m_labelGameKindOnLine.setText(IDS_ONLINECOUNT + ": " + dwOnLineCount);
		}
		//完成登陆大厅事件
		public function OnEventFinishLogon():void
		{
			ShowMiniGameUI(m_GameKindItem.getItemData().wProcessType);
		}
		//重画
		override protected function paint(b:IntRectangle):void
		{
			super.paint(b);
			graphics.clear();
			var g:Graphics2D = new Graphics2D(graphics);
		}
		//获取服务项
		public function getServerItem():ServerItem
		{
			return m_GameKindItem;
		}
		//插入游戏站点项
		public function insertServerItem(item:tagGameServer):Boolean
		{
			if(findServerItem(item.wServerID) != null)
			{
				return false;
			}
			var server:ServerItem = new ServerItem(item, SUB_GP_LIST_SERVER);
			m_vecServerList.append(server.toString());
			m_GameServerList.append(server);
			m_btnEnterGame.setEnabled(true);
			return true;
		}
		//查找服务项
		public function findServerItem(wServerID:uint):ServerItem
		{
			for(var i:int = 0; i < m_GameServerList.size(); i ++)
			{
				var item:ServerItem = m_GameServerList.get(i);
				if(item != null)
				{
					var GameServer:tagGameServer = item.getItemData();
					if(GameServer != null && 
					   GameServer.wServerID == wServerID)
					{
						return item;
					}
				}
			}
			return null;
		}
		//自动随机游戏站点
		private function randServerItem():ServerItem
		{
			if(m_GameServerList.size() <= 1)
			{
				return null;
			}
			var n:uint;

			while(true)
			{
				var r:Number = Math.random() * 10000000.0;
				n = r;
				n = n % (m_GameServerList.size()-1) + 1;
				if(n > 0)
				{
					var item:ServerItem = m_GameServerList.get(n);
					if(item != null)
					{
						return item;
					}
				}
			}
			return null;
		}
		//进入游戏按钮事件
		private function OnEventEnterGame(e:MouseEvent):void
		{
			if(m_EnteringGamePane)
			{
				return;
			}
			var main:IMain = GlobalMain.getMain();
			if(main)
			{
				var plaza:IPlazaWindow = main.getIPlazaWindow();
				if(plaza)
				{
					var item:ServerItem = randServerItem();
					if(item)
					{
						var n:int = plaza.findEqualPane(item); 
						if(n != -1)
						{
							plaza.activePane(plaza.findPane(n));
							return;
						}
						if(plaza.getGamePaneCount() >= plaza.getMaxGamePaneCount())
						{
							main.ShowMessageBox(IDS_ERR_TOOMANYGAMES);
							return;
						}
						m_EnteringGamePane = BeginEnterGame(item,
														   m_GameKindItem,
														   m_mcParent.getGameTypeItem());
						if(m_EnteringGamePane == null)
						{
							main.ShowMessageBox(IDS_ERR_ENTERGAMEFAILED);
						}
						else
						{							
							main.ShowStatusMessage(IDS_STATUS_ENTERINGGAME);
							m_EnteringGamePane.DoEnterGameWork();
							m_timerCheckLogon.reset();
							m_timerCheckLogon.start();
						}
					}
					
				}
			}
		}
		//游戏规则按钮事件
		private function OnEventHelp(e:MouseEvent):void
		{
			var main:IMain = GlobalMain.getMain();
			if(main)
			{
				var plaza:IPlazaWindow = main.getIPlazaWindow();
				if(plaza)
				{
					var helpView:GameHelpView = new GameHelpView(plaza.getActiveDOC(),
													m_GameKindItem.getItemData().wProcessType);
					helpView.DoModal();
				}
			}
		}		
		//开始进入游戏
		public function BeginEnterGame(param0:*,param1:* = null,param2:* = null,param3:* = null):IGamePaneImpl
		{
			var c:Class = GetGamePaneClass();
			if(c == null)
				return null;
			var game:IGamePaneImpl = new /*GamePane*/c(this);
			if(game.Create(param0, param1, param2, param3) != 0)
			{
				game = null;
				return null;
			}
			return game;
		}
		//取消进入游戏
		public function CancelEnterGame(game:IGamePaneImpl):void
		{
			if(m_EnteringGamePane == game && 
			   m_EnteringGamePane)
			{
				m_timerCheckLogon.stop();
				m_EnteringGamePane.Destroy();
				m_EnteringGamePane = null;
			}
		}
		//完成进入游戏
		public function FinishEnterGame(game:IGamePaneImpl):void
		{
			if(m_EnteringGamePane == game &&
			   m_EnteringGamePane)
			{
				var main:IMain = GlobalMain.getMain();
				if(main)
				{					
					var plaza:IPlazaWindow = main.getIPlazaWindow();
					if(plaza)
					{
						if(plaza.enterGamePane(game)!=0)
						{
							CancelEnterGame(game);
							return;
						}
					}
				}
				main.HideStatusMessage();		
				m_timerCheckLogon.stop();
				m_EnteringGamePane = null;
			}
		}
		//获取游戏面板类
		private function GetGamePaneClass():Class
		{
			var main:IMain = GlobalMain.getMain();
			if(main == null)
				return null;
			var c:Class = main.getDefClass("GamePane","PlazaWindow");
			if(c != null)
			{
				return c;
			}
			else
			{
				return null;
			}
		}
		//超时检测事件
		private function OnEventTimerHandler(e:TimerEvent):void
		{
			m_timerCheckLogon.stop();
			if (m_EnteringGamePane &&
				m_EnteringGamePane.isLogonOK() == false)
			{
				CancelEnterGame(m_EnteringGamePane);
				var main:IMain = GlobalMain.getMain();
				main.HideStatusMessage();
				main.ShowMessageBox(IDS_ERR_TIMEOUT);
			}
		}
		//显示简图
		private function ShowMiniGameUI(wProcessType:uint):void
		{
			if(m_MiniGameUI == null)
			{
				m_MiniGameUI = new Loader();
				m_MiniGameUI.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, OnEventMUIIOErrorHandler);

				var url:String = GameModuleNameFactory.GetGameMiniUI(wProcessType);
				var urlReq:URLRequest = new URLRequest(url);
				m_MiniGameUI.load(urlReq);
				addChild(m_MiniGameUI);
				m_MiniGameUI.x = 12;
				m_MiniGameUI.y = 46;
			}
		}
		//下载简图失败
		private function OnEventMUIIOErrorHandler(e:Event):void
		{
			trace(e);
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"
include "../../Common/NetWork.as"
