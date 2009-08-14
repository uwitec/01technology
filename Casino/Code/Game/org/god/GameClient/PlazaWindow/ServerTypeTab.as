package 
{
	import flash.display.MovieClip;

	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Common.ServerItem;
	
	import org.aswing.JScrollPane;
	import org.aswing.Component;
	import org.aswing.geom.IntRectangle;
	import org.aswing.Icon;	
	//游戏服务标签
	public class ServerTypeTab extends JScrollPane
	{
		public static const nServerTypeTab_RootWeb:int 		= 0;
		public static const nServerTypeTab_CasinoField:int 	= 1;
		public static const nServerTypeTab_GameKindGrid:int = 2;
		
		private var m_mcParent:ServerItemTabPane;//服务项标签面板
		private var m_GameItem:ServerItem;//服务项
		private var m_nTabType:int;//标签类型
		private var m_view:Component;//标签视图
		//创建首页标签
		static public function CreateServerTypeTab_RootWeb(mcParent:ServerItemTabPane):ServerTypeTab
		{
			var _rootItem:ServerItem = new ServerItem(null, SUB_GP_LIST_ROOT);
			return new ServerTypeTab(mcParent, nServerTypeTab_RootWeb, _rootItem);
		}
		//创建现场标签
		static public function CreateServerTypeTab_CasinoField(mcParent:ServerItemTabPane):ServerTypeTab
		{
			var _casinofieldItem:ServerItem = new ServerItem(null, SUB_GP_LIST_CASINOFIELD);
			return new ServerTypeTab(mcParent, nServerTypeTab_CasinoField, _casinofieldItem);
		}
		//创建游戏类型标签
		static public function CreateServerTypeTab_GameKindGrid(mcParent:ServerItemTabPane,GameType:tagGameType):ServerTypeTab
		{
			var _gamekindgridItem:ServerItem = new ServerItem(GameType, SUB_GP_LIST_TYPE);
			return new ServerTypeTab(mcParent, nServerTypeTab_GameKindGrid, _gamekindgridItem);
		}
		//构造函数
		public function ServerTypeTab(mcParent:ServerItemTabPane,
								nTabType:int,
		  GameItem:ServerItem)
		{
			super();
			m_mcParent = mcParent;
			m_nTabType = nTabType;
			m_GameItem = GameItem;
			
			m_view = CreateView();
			if(m_view != null)
			{
				setView(m_view);
			}
			readyToPaint = true;

		}
		//销毁
		public function Destroy():void
		{
			if(m_view == null)
				return;
			switch(m_nTabType)
			{
				case nServerTypeTab_RootWeb:
				{
					(m_view as TabViewRootWeb).Destroy();
				}
				break;
				case nServerTypeTab_CasinoField:
				{
					(m_view as TabViewCasinoField).Destroy();
				}
				break;
				case nServerTypeTab_GameKindGrid:
				{
					(m_view as TabViewGameKindGrid).Destroy();
				}
				break;
			}
			m_mcParent = null;
			m_GameItem = null;
			m_view = null;
		}
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			if(m_view == null)
				return;
			switch(m_nTabType)
			{
				case nServerTypeTab_RootWeb:
				{
					(m_view as TabViewRootWeb).OnEventFinishLogon();
				}
				break;
				case nServerTypeTab_CasinoField:
				{
					(m_view as TabViewCasinoField).OnEventFinishLogon();
				}
				break;
				case nServerTypeTab_GameKindGrid:
				{
					(m_view as TabViewGameKindGrid).OnEventFinishLogon();
				}
				break;
			}
		}
		//更新游戏在线人数
		public function UpdateGameKindOnLine(wKindID:uint,dwOnLineCount:uint):void
		{
			if(m_view == null)
				return;
			switch(m_nTabType)
			{
				case nServerTypeTab_RootWeb:
				{
					
				}
				break;
				case nServerTypeTab_CasinoField:
				{
					
				}
				break;
				case nServerTypeTab_GameKindGrid:
				{
					(m_view as TabViewGameKindGrid).UpdateGameKindOnLine(wKindID,dwOnLineCount);
				}
				break;
			}
		}
		//创建标签视图
		private function CreateView():Component
		{
			switch(m_nTabType)
			{
				case nServerTypeTab_RootWeb:
				{
					return TabViewRootWeb.CreateTabViewRootWeb("",
															   this);
				}
				break;
				case nServerTypeTab_CasinoField:
				{
					return TabViewCasinoField.CreateTabViewCasinoField("",
															   this);
				}
				break;
				case nServerTypeTab_GameKindGrid:
				{
					return TabViewGameKindGrid.CreateTabViewGameKindGrid(m_GameItem,
															   this);
				}
				break;
			}
			return null;
		}
		//重画
		override protected function paint(b:IntRectangle):void
		{
			super.paint(b);
				
			if(m_view != null)
			{
				m_view.setComBounds(b);
			}
		}
		//插入游戏类型项
		public function insertKindItem(item:tagGameKind):Boolean
		{
			if(m_view == null)
			{
				return false;
			}
			var view:TabViewGameKindGrid = m_view as TabViewGameKindGrid;
			return view.insertKindItem(item);
		}
		//插入游戏服务项
		public function insertServerItem(item:tagGameServer):Boolean
		{
			if(m_view == null)
			{
				return false;
			}
			var view:TabViewGameKindGrid = m_view as TabViewGameKindGrid;
			return view.insertServerItem(item);		
		}
		//获取标题
		public function getTitle():String
		{
			switch(m_nTabType)
			{
				case nServerTypeTab_RootWeb:
				{
					return IDS_ROOTWEB;
				}
				case nServerTypeTab_CasinoField:
				{
					return IDS_CASINOFIELD;
				}
				
				case nServerTypeTab_GameKindGrid:
				{
					return m_GameItem.toString();
				}
				
			}
			return "";
		}
		//获取图标
		public function getIcon():Icon
		{
			switch(m_nTabType)
			{
				case nServerTypeTab_RootWeb:
				{
					return GameIcon.loadGameIcon(255);
				}
				case nServerTypeTab_CasinoField:
				{
					return GameIcon.loadGameIcon(254);
				}
				
				case nServerTypeTab_GameKindGrid:
				{
					return GameIconUtil.getLeafIcon(SUB_GP_LIST_TYPE,m_GameItem.getItemData());
				}
				
			}
			return null;
		}	
		//获取标签类型
		public function getTabType():int
		{
			return m_nTabType;
		}
		//获取服务项
		public function getServerItem():ServerItem
		{
			return m_GameItem;
		}
	
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"
