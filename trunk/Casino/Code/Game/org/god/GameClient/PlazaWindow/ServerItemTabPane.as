package 
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Common.ServerItem;

	import org.aswing.JTabbedPane;
	import org.aswing.Component;
	import org.aswing.Icon;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	//游戏服务项节点面板
	public class ServerItemTabPane extends JTabbedPane
	{
		protected static const MyColor:ASColor = new ASColor(0xf1e8a5);
		protected static var MyFont:ASFont;
	
		private var m_mcParent:MovieClip;//父影片
		private var m_nTabIndexOffset:int = 0;//标签偏移
		//构造函数
		public function ServerItemTabPane(mcParent:MovieClip)
		{
			super();
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 //MyFont = MyFont.changeBold(true);
			}
			m_mcParent = mcParent;
			m_mcParent.addChild(this);
			this.setFont(MyFont);
			this.setForeground(MyColor);

			var cr:ServerTypeTabContainer = ServerTypeTabContainer.CreateServerTypeTabContainer_RootWeb(this);
			var tr:ServerTypeTab = cr.getTab();
			appendTab(cr, tr.getTitle(), tr.getIcon());
			
			var cc:ServerTypeTabContainer = ServerTypeTabContainer.CreateServerTypeTabContainer_CasinoField(this);
			var tc:ServerTypeTab = cc.getTab();
			appendTab(cc, tc.getTitle(), tc.getIcon());
			
			m_nTabIndexOffset = 1;
		}
		//销毁
		public function Destroy():void
		{
			for(var i:int = 0; i < getTabCount(); i ++)
			{
				var c:ServerTypeTabContainer = getComponent(i) as ServerTypeTabContainer;
				if(c != null)
				{
					c.Destroy();
				}
			}
			this.removeAll();
		}
		//插入游戏类型项
		public function insertTypeItem(item:tagGameType):Boolean
		{
			if (item == null)
			{
				return false;
			}
			if(findServerTypeTab(item.wTypeID) != null)
			{
				return false;
			}
			var ck:ServerTypeTabContainer = ServerTypeTabContainer.CreateServerTypeTabContainer_GameKindGrid(this,item);
			var tk:ServerTypeTab = ck.getTab();
			appendTab(ck, tk.getTitle(), tk.getIcon());
			return true;
		}
		//插入游戏种类项
		public function insertKindItem(item:tagGameKind):Boolean
		{
			if (item == null)
			{
				return false;
			}
			var tab:ServerTypeTab = findServerTypeTab(item.wTypeID);
			if(tab == null)
			{
				return false;
			}
			return tab.insertKindItem(item);
		}
		//插入游戏服务项
		public function insertServerItem(item:tagGameServer):Boolean
		{
			if (item == null)
			{
				return false;
			}
			var tab:ServerTypeTab = findServerTypeTab(item.wTypeID);
			if(tab == null)
			{
				return false;
			}
			return tab.insertServerItem(item);
		}
		//查找游戏类型标签
		public function findServerTypeTab(wTypeID:uint):ServerTypeTab
		{
			for(var i:int = 0; i < getTabCount(); i ++)
			{
				var c:ServerTypeTabContainer = getComponent(i) as ServerTypeTabContainer;
				var tab:ServerTypeTab = c.getTab();
				if(tab != null && tab.getTabType() == ServerTypeTab.nServerTypeTab_GameKindGrid)
				{
					var item:ServerItem = tab.getServerItem();
					if(item != null && item.getItemType() == SUB_GP_LIST_TYPE)
					{
						var GameType:tagGameType = item.getItemData();
						if(GameType != null && GameType.wTypeID == wTypeID)
						{
							return tab;
						}
					}
				}
			}
			return null;
		}
		//插入项完成
		public function insertItemFinish():void
		{

		}
		//完成登陆
		public function OnEventFinishLogon():void
		{
			for(var i:int = 0; i < getTabCount(); i ++)
			{
				var c:ServerTypeTabContainer = getComponent(i) as ServerTypeTabContainer;
				if(c != null)
				{
					c.OnEventFinishLogon();
				}
			}
		}
		//更新游戏在线人数
		public function UpdateGameKindOnLine(wTypeID:uint,wKindID:uint,dwOnLineCount:uint):void
		{
			var tab:ServerTypeTab = findServerTypeTab(wTypeID);
			if(tab == null)
			{
				return ;
			}
			tab.UpdateGameKindOnLine(wKindID,dwOnLineCount);
		}
		//改变大小
		override protected function size():void
		{
			super.size();
			doLayout();
		}
		//添加标签
		override public function appendTab(com:Component, title:String="", icon:Icon=null, tip:String=null):void
		{
			insertTab(getComponentCount()-m_nTabIndexOffset, com, title, icon, tip);
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"
