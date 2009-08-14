package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	
	import org.god.GameManagement.PlazaWindow.Common.BasePanel;
	
	import org.god.Net.IClientSocket;
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagOnLineCountInfo;
	import org.god.IGameManagement.*;
	import org.god.Common.DrawUtil;
	import org.god.Common.GameTreeNode;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.MasterRight;
	
	import org.aswing.JTree;
	import org.aswing.JTabbedPane;
	import org.aswing.JPanel;
	import org.aswing.JScrollPane;
	import org.aswing.JLabel;
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.BorderLayout;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.Insets;
	import org.aswing.event.InteractiveEvent;
	import org.aswing.event.AWEvent;
	import org.aswing.event.TreeSelectionEvent;
	import org.aswing.event.ClickCountEvent;
	import org.aswing.tree.DefaultMutableTreeNode;
	import org.aswing.tree.DefaultTreeModel;
	import org.aswing.tree.TreePath;
	import org.aswing.tree.GeneralTreeCellFactoryUIResource;
	//现场投注面板
	public class BetMonitorPanel extends BasePanel implements IPanelImpl
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		
		protected var m_treeGameItem:JTree;
		protected var m_scollpaneTree:JScrollPane;
		protected var m_panelTreeContainer:JPanel;
		protected var m_nodeTreeRoot:DefaultMutableTreeNode;
		protected var m_treeModel:DefaultTreeModel;
		
		protected var m_tabMonitor:JTabbedPane;
		
		public function BetMonitorPanel()
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(true);
			}
		}
		public function Create(param0:IMain,param1:IPlazaWindow):int
		{
			m_IMain		   = param0;
			m_IPlazaWindow = param1;
			m_ClientSocket = m_IMain.getClientSocket();
			
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;
			
			var nYP:Number = 46;
			var nXP:Number = 195;
			var nCX:Number = 820;
			var nCY:Number = 582;

			var tab:BaseMonitorTab = CreateMonitorTab();
			m_tabMonitor = new JTabbedPane;
			m_tabMonitor.appendTab(tab, tab.GetMonitorTitle());
			addChild(m_tabMonitor);
			m_tabMonitor.setFont(font.changeBold(false));
			m_tabMonitor.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tabMonitor.setTabPlacement(JTabbedPane.BOTTOM);
			m_tabMonitor.doLayout();
			
			nYP = 52;
			nXP = 12;
			nCX = 172;
			nCY = 567;
			m_nodeTreeRoot = new DefaultMutableTreeNode(new GameTreeNode(null, SUB_GP_LIST_ROOT));
			m_panelTreeContainer = new JPanel(new BorderLayout());
			m_treeModel = new DefaultTreeModel(m_nodeTreeRoot);
			m_treeGameItem = new JTree(m_treeModel);
			m_scollpaneTree = new JScrollPane(m_treeGameItem);
			m_panelTreeContainer.append(m_scollpaneTree, BorderLayout.CENTER);
			addChild(m_panelTreeContainer);
			m_panelTreeContainer.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_panelTreeContainer.doLayout();
			m_treeGameItem.setBackground(new ASColor(0xffffed));
			m_treeGameItem.setForeground(col);
			m_treeGameItem.setFont(font);
			m_treeGameItem.addEventListener(ClickCountEvent.CLICK_COUNT, OnEventTreeClickCount);
			m_treeGameItem.setSelectionMode(JTree.SINGLE_TREE_SELECTION);

			m_treeModel.setRoot(m_nodeTreeRoot);
			
			return 0;
		}
		public function Destroy():void
		{
			while( m_tabMonitor.getTabCount() > 0)
			{
				var tab:BaseMonitorTab =  m_tabMonitor.removeAt(0) as BaseMonitorTab;
				tab.Destroy();
				m_tabMonitor.remove(tab);
			}
			removeChild(m_tabMonitor);
			m_tabMonitor = null;
			
			m_treeGameItem.removeEventListener(ClickCountEvent.CLICK_COUNT, OnEventTreeClickCount);
			removeChild(m_panelTreeContainer);
			m_panelTreeContainer.remove(m_scollpaneTree);
			m_treeModel = null;
			m_treeGameItem = null;
			m_scollpaneTree = null;
			m_panelTreeContainer = null;

			m_bmBackground.dispose();
			m_bmBackground = null;
			m_IMain = null;
		 	m_IPlazaWindow = null;
			m_ClientSocket = null;

		}
		public function getNavCmd():int
		{
			return nNavCmd_BetMonitor;
		}
		public function getTitle():String
		{
			return IDS_PANELTITLE_BETMONITOR;
		}
		public function Active(data:*):int
		{
			doLayout();
			return 0;
		}
		public function Deactive():int
		{
			return 0;
		}
		public function getActiveDC():Sprite
		{
			return this;
		}
		public function moveDC(cb:IntRectangle):void
		{
			setComBounds(cb);
		}
		public function validateDC():void
		{
			validate();
			paintImmediately();
		}
		public function setVisibleDC(b:Boolean):void
		{
			setVisible(b);
		}
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wMainCmdID)
			{
				case MDM_GP_SERVER_LIST :
					{
						return OnSocketMainServerList(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

				}
				case MDM_GP_SERVER_INFO :
					{
						return OnSocketMainServerInfo(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

				}
			};
			return false;
		}
		//列表消息
		public function OnSocketMainServerInfo(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_ONLINE_COUNT_INFO://在线信息
				{
					//效验数据
					if (wDataSize%tagOnLineCountInfo.sizeof_tagOnLineCountInfo!=0) return false;

					//消息处理
					var wTypeID:uint=0;
					var wKindID:uint=0;
					var dwKindOnLineCount:uint=0;
					var dwAllOnLineCount:uint=0;
					var wInfoCount:uint=wDataSize/tagOnLineCountInfo.sizeof_tagOnLineCountInfo;
					
					for (var i:uint=0;i<wInfoCount;i++)
					{
						var pOnLineCountInfo:tagOnLineCountInfo=tagOnLineCountInfo.readData(pBuffer);
						wTypeID=pOnLineCountInfo.wTypeID;
						wKindID=pOnLineCountInfo.wKindID;
						dwKindOnLineCount=pOnLineCountInfo.dwOnLineCount;
						dwAllOnLineCount+=dwKindOnLineCount;
						UpdateGameKindOnLine(wTypeID,wKindID,dwKindOnLineCount);
						pBuffer.position += tagOnLineCountInfo.sizeof_tagOnLineCountInfo;
					}

					//更新总数
					UpdateGameOnLineCount(dwAllOnLineCount);

					return true;
				}
			}
			return false;
		}
		//列表消息
		public function OnSocketMainServerList(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			var i:int = 0;
			var wItemCount:uint = 0;
			switch (wSubCmdID)
			{
					//类型信息
				case SUB_GP_LIST_TYPE :
					{
						if (wDataSize%tagGameType.sizeof_tagGameType!=0)
						{
							return false;
						}
						wItemCount=wDataSize/tagGameType.sizeof_tagGameType;
						for(i = 0; i < wItemCount; i ++)
						{
							var itemType:tagGameType = tagGameType.readData(pBuffer);
							insertTypeItem(itemType);
							
							pBuffer.position += tagGameType.sizeof_tagGameType;
						}
						return true;

					};
					//种类消息
				case SUB_GP_LIST_KIND :
					{
						if (wDataSize%tagGameKind.sizeof_tagGameKind!=0)
						{
							return false;
						}
						wItemCount=wDataSize/tagGameKind.sizeof_tagGameKind;
						for(i = 0; i < wItemCount; i ++)
						{
							var itemKind:tagGameKind = tagGameKind.readData(pBuffer);
							insertKindItem(itemKind);
							pBuffer.position += tagGameKind.sizeof_tagGameKind;
						}
						return true;

					};
					//进程信息
				case SUB_GP_LIST_PROCESS :
					{
						return true;

					};
					//站点消息
				case SUB_GP_LIST_STATION :
					{
						return true;

					};
					//服务器房间
				case SUB_GP_LIST_SERVER :
					{
						if (wDataSize%tagGameServer.sizeof_tagGameServer!=0)
						{
							return false;
						}
						wItemCount=wDataSize/tagGameServer.sizeof_tagGameServer;
						for(i = 0; i < wItemCount; i ++)
						{
							var itemServer:tagGameServer = tagGameServer.readData(pBuffer);
							insertServerItem(itemServer);
							pBuffer.position += tagGameServer.sizeof_tagGameServer;
						}
						return true;

					};
					//列表发送完成
				case SUB_GP_LIST_FINISH :
					{
						m_treeModel.reload();
						return true;

					};
					//列表配置
				case SUB_GP_LIST_CONFIG :
					{
						return true;

				}
			};
			return false;
		}
		
		/////////////////////////////////////////////
		override protected function DrawPanel(g:Graphics,pDC:BitmapData):void
		{
			DrawPanelTitle(pDC,getTitle());
		}
		override protected function SetupPanel():void
		{
			m_bmBackground = new IMAGE_PANEL(1024,635);
		}
		//////////////////////////////////////////////
		private function UpdateGameOnLineCount(dwOnLineCount:uint):void
		{
			
		}
		private function UpdateGameKindOnLine(wTypeID:uint, wKindID:uint,dwOnLineCount:uint):void
		{
			
		}
		protected function CreateMonitorTab(gameType:GameTreeNode = null,
											gameKind:GameTreeNode = null,
											gameServer:GameTreeNode = null):BaseMonitorTab
		{
			var tab:BaseMonitorTab = null;
			if(gameType == null && gameKind == null && gameServer == null)
			{
				tab = new BlankMonitorTab;
			}
			else
			{
				switch(GameModuleNameFactory.GetGameTypeMark(gameKind.getItemData().wProcessType))
				{
					case dwGameTypeMark_Battle:
						tab = new BattleMonitorTab;
						break;
					case dwGameTypeMark_Video:
						tab = new VideoMonitorTab;
						break;
					case dwGameTypeMark_Pokie:
						tab = new PokieMonitorTab;
						break;
					case dwGameTypeMark_Lottery:
						tab = new LotteryMonitorTab;
						break;
					default:
					break;
				}
				
			}
			if(tab == null)
				return null;
			if(tab.Create(m_IMain,m_IPlazaWindow,this,gameType,gameKind,gameServer) == 0)
			{
				return tab;
			}
			tab.Destroy();
			tab = null;
			return null;
		}
		public function CloseMonitorTab(tab:BaseMonitorTab):void
		{
			tab = m_tabMonitor.remove(tab) as BaseMonitorTab;
			if(tab)
			{
				tab.StopMonitor();
				tab.Destroy();
				if(m_tabMonitor.getTabCount() > 1)
					m_tabMonitor.setSelectedIndex(1);
				else
					m_tabMonitor.setSelectedIndex(0);
			}
		}
		protected function insertTypeItem(item:tagGameType):Boolean
		{
			var parent_node:DefaultMutableTreeNode = 
			FindGameItemMutableTreeNode(m_nodeTreeRoot, SUB_GP_LIST_TYPE, item.wTypeID);
			if(parent_node != null)
				return true;
			m_nodeTreeRoot.append(new DefaultMutableTreeNode(new GameTreeNode(item,SUB_GP_LIST_TYPE)));
			return true;
		}
		protected function insertKindItem(item:tagGameKind):Boolean
		{
			var parent_node:DefaultMutableTreeNode = 
			FindGameItemMutableTreeNode(m_nodeTreeRoot, SUB_GP_LIST_TYPE, item.wTypeID);
			if(parent_node != null)
			{
				parent_node.append(new DefaultMutableTreeNode(new GameTreeNode(item,SUB_GP_LIST_KIND)));
			}
			return true;
		}
		protected function insertServerItem(item:tagGameServer):Boolean
		{
			var parent_node:DefaultMutableTreeNode = 
			FindGameItemMutableTreeNode(m_nodeTreeRoot, SUB_GP_LIST_KIND, item.wKindID);
			if(parent_node != null)
			{
				parent_node.append(new DefaultMutableTreeNode(new GameTreeNode(item,SUB_GP_LIST_SERVER)));
			}
			return true;
		}
		protected function FindGameItemMutableTreeNode(node:DefaultMutableTreeNode,
													   wItemType:uint,
													   wItemID:uint):DefaultMutableTreeNode
		{
			if(node == null || node.getUserObject() == null)
				return null;
			if(node.getUserObject().getItemType() == wItemType &&
				node.getUserObject().equalItemID(wItemID) == true)
				return node;
			for(var i:uint = 0; i < node.getChildCount(); i ++)
			{
				var node_child = node.getChildAt(i) as DefaultMutableTreeNode;
				var node_find:DefaultMutableTreeNode = FindGameItemMutableTreeNode(node_child,wItemType,wItemID);
				if(node_find != null)
					return node_find;
			}
			return null;
		}
		////////////////////////////////////////////////////////
		protected function OnEventTreeClickCount(e:ClickCountEvent):void
		{
			if(MasterRight.CanBetMonitorView(m_IMain.getUserData().dwMasterRight) == false)
			{
				m_IMain.ShowMessageBox(IDS_ERR_LIMITRIGHT);
				return;
			}
			if(e.getCount() == 2)
			{
				var node:DefaultMutableTreeNode = m_treeGameItem.getLastSelectedPathComponent() as DefaultMutableTreeNode;
				if(node != null && node.getUserObject() != null &&
					node.getUserObject().getItemType() == SUB_GP_LIST_SERVER)
				{
					var gameServer:GameTreeNode = node.getUserObject();
					if(gameServer != null)
					{
						//查找现场投注查看页
						for(var i:int = 1; i < m_tabMonitor.getTabCount(); i ++)
						{
							var tab:BaseMonitorTab =  m_tabMonitor.getComponent(i) as BaseMonitorTab;
							if(tab != null && tab.EqualGameTreeNode(gameServer) == true)
							{
								m_tabMonitor.setSelectedIndex(i);
								return;
							}
						}
						//最多5个
						if(m_tabMonitor.getTabCount() >= 6)
						{
							m_IMain.ShowMessageBox(IDS_ERR_MAXBETMONITORITEMCOUNT);
							return;
						}
						//查找游戏种类
						var parent_node:DefaultMutableTreeNode = node.getParent() as DefaultMutableTreeNode;
						if(parent_node == null)
							return;
						var gameKind:GameTreeNode = parent_node.getUserObject();
						//查找游戏类型
						parent_node = parent_node.getParent() as DefaultMutableTreeNode;
						if(parent_node == null)
							return;
						var gameType:GameTreeNode = parent_node.getUserObject();
						if(gameKind == null || gameType == null)
							return;
											
						var tab:BaseMonitorTab = CreateMonitorTab(gameType,gameKind,gameServer);
						if(tab != null)
						{
							i = m_tabMonitor.getTabCount();
							m_tabMonitor.appendTab(tab, tab.GetMonitorTitle());
							m_tabMonitor.setSelectedIndex(i);
							tab.StartMonitor();
						}
					}
				}
			}
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../Net/GLOBALDEF.as"
include "../../../Net/CMD_Plaza.as"