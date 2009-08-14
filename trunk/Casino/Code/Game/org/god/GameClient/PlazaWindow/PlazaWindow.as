package 
{
	import fl.controls.Label;
	import flash.display.MovieClip;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.events.MouseEvent;
	import fl.managers.FocusManager;
	import flash.utils.ByteArray;
	
	import org.god.IGameFrame.IPlazaWindow;
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.IPaneImpl;
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketSink;
	import org.god.Common.Media.SoundManager;
	import org.god.Common.Media.QuietEvent;
	
	import org.aswing.Icon;
	//大厅窗口
	public class PlazaWindow extends MovieClip implements IPlazaWindow
	{
		static const _MaxPaneCount:uint = 5;

		private var m_IMain:IMain;
		private var m_arrIPaneImpl:Array;
		private var m_PlazaWindowNavBar:PlazaWindowNavBar;
		private var m_nCurPane:int = -1;
		private var m_PlazaWindowSocketSink:PlazaWindowSocketSink;
		//构造函数
		public function PlazaWindow(mcParent:IMain)
		{
			m_IMain = mcParent;
			
			m_arrIPaneImpl = new Array(_MaxPaneCount);
			m_PlazaWindowNavBar = new PlazaWindowNavBar(this,_MaxPaneCount);
			m_PlazaWindowSocketSink = new PlazaWindowSocketSink(this);
			
			SoundManager.GetGlobalSoundManager().addEventListener(QuietEvent.NAME_QUIETEVENT,
																  OnEventQueit);
			
			this.x = 0;
			this.y = 0;
			this.visible = false;
		}
		//////////////////////////////////////////////////
		//获取主类接口
		public function getIMain():IMain
		{
			return m_IMain;
		}		
		//显示
		public function Show():void
		{
			if (m_nCurPane == -1)
			{
				var pane:IPaneImpl = CreatePlazaWindowPane();
				activePane(pane);
			}
			else
			{
				var plazapane:PlazaPane = findPlazaPane();
				if(plazapane != null)
				{
					activePane(plazapane);
					plazapane.Active(null);
				}
			}
			this.visible = true;
		}
		//隐藏
		public function Hide():void
		{
			this.visible = false;
			var plazapane:PlazaPane = findPlazaPane();
			if(plazapane != null)
			{
				plazapane.Deactive();
			}
		}
		//获取当前活动显示容器
		public function getActiveDOC():DisplayObjectContainer
		{
			return this;
		}
		//复位
		public function resetAll():void
		{
			for (var i:int = 0; i < m_arrIPaneImpl.length; i ++)
			{
				var pane:IPaneImpl = m_arrIPaneImpl[i];
				if (pane)
				{
					pane.Destroy();
					pane.showPane(this, false);
					pane = null;
					
					m_arrIPaneImpl[i] = null;
					m_PlazaWindowNavBar.setBtnBindData(i, null);
				}
			}
			m_nCurPane = -1;
		}
		//添加子显示对象
		public function addChildDO(d:DisplayObject):void
		{
			addChild(d);
			if(getChildIndex(d) > getChildIndex(m_PlazaWindowNavBar))
			{
				swapChildren(d,m_PlazaWindowNavBar);
			}
		}
		//删除子显示对象
		public function removeChildDO(d:DisplayObject):void
		{
			removeChild(d);
		}

		/////////////////////////////////////////////////////
		//活动面板
		public function activePane(pane:IPaneImpl):int
		{
			if (pane == null)
			{
				return -1;
			}
			var nNewIPaneImplIndex:int = findPaneIndex(pane);
			if (nNewIPaneImplIndex == -1)
			{
				return -1;
			}
			if (nNewIPaneImplIndex == m_nCurPane)
			{
				return 0;
			}
			if (m_nCurPane != -1)
			{
				var oldIPaneImpl:IPaneImpl = m_arrIPaneImpl[m_nCurPane];
				if (oldIPaneImpl)
				{
					oldIPaneImpl.Deactive();
					m_PlazaWindowNavBar.DeactiveBtn(m_nCurPane);
				}
			}
			m_nCurPane = nNewIPaneImplIndex;
			var paneActive:IPaneImpl = m_arrIPaneImpl[m_nCurPane];
			paneActive.Active(null);
			m_PlazaWindowNavBar.setBtnText(m_nCurPane,m_arrIPaneImpl[m_nCurPane].getPaneName());
			m_PlazaWindowNavBar.ActiveBtn(m_nCurPane);

			return 0;
		}
		//获取面板数目
		public function getGamePaneCount():uint
		{
			var c:uint = 0;
			for (var i:int = 0; i < m_arrIPaneImpl.length; i ++)
			{
				var pane0:IPaneImpl = m_arrIPaneImpl[i];
				if (pane0 && 
				pane0.getPaneType()!= nPaneType_Plaza )
				{
					c ++;
				}
			}
			return c;
		}
		//获取最大面板数目
		public function getMaxGamePaneCount():uint
		{
			return _MaxPaneCount - 1;
		}
		//查找面板
		public function findPane(i:uint):IPaneImpl
		{
			return  m_arrIPaneImpl[i];
		}
		//查找面板
		public function findEqualPane(param:*):int
		{
			for (var i:int = 0; i < m_arrIPaneImpl.length; i ++)
			{
				var pane0:IPaneImpl = m_arrIPaneImpl[i];
				if (pane0 && 
				pane0.getPaneType()!= nPaneType_Plaza &&
				pane0.EqualPane(param))
				{
					return i;
				}
			}
			return -1;
		}
		//查找下一面板
		public function findNextActivePane(nBeginPane:int):int
		{
			for (var i:int = nBeginPane; i < m_arrIPaneImpl.length; i ++)
			{
				var pane0:IPaneImpl = m_arrIPaneImpl[i];
				if (pane0)
				{
					return i;
				}
			}
			for (i = 1; i < nBeginPane; i ++)
			{
				var pane1:IPaneImpl = m_arrIPaneImpl[i];
				if (pane1)
				{
					return i;
				}
			}
			return 0;
		}
		//查找面板索引
		public function findPaneIndex(pane:IPaneImpl):int
		{
			for (var i:int = 0; i < m_arrIPaneImpl.length; i ++)
			{
				var pane0:IPaneImpl = m_arrIPaneImpl[i];
				if (pane == pane0)
				{
					return i;
				}
			}
			return -1;
		}
		//获取空闲面板位置
		public function getFreeGamePanePosition():int
		{
			for (var i:int = 0; i < m_PlazaWindowNavBar.getNavBtnCount(); i ++)
			{
				if (m_PlazaWindowNavBar.getBtnBindData(i) == null)
				{
					return i;
				}
			}
			return -1;
		}
		//销毁面板
		public function destroyGamePane(nGamePane:int):void
		{
			var pane:IPaneImpl = m_arrIPaneImpl[nGamePane];
			if (pane)
			{
				pane.Destroy();
				pane.showPane(this, false);
				pane = null;
				
				m_PlazaWindowNavBar.setBtnBindData(nGamePane,null);
				m_PlazaWindowNavBar.setBtnText(nGamePane,IDS_NOGAMEROOM);			
				m_arrIPaneImpl[nGamePane] = null;
				
			}
			for (var i:int = nGamePane; i < _MaxPaneCount - 1; i ++)
			{
				m_PlazaWindowNavBar.setBtnBindData(i,m_PlazaWindowNavBar.getBtnBindData(i+1));
				m_PlazaWindowNavBar.setBtnText(i,m_PlazaWindowNavBar.getBtnText(i+1));
				m_PlazaWindowNavBar.setBtnIcon(i,m_PlazaWindowNavBar.getBtnIcon(i+1));
				m_arrIPaneImpl[i] = m_arrIPaneImpl[i+1];
			}
			m_PlazaWindowNavBar.setBtnBindData(i,null);
			m_PlazaWindowNavBar.setBtnText(i,IDS_NOGAMEROOM);
			m_PlazaWindowNavBar.setBtnIcon(i,GameIcon.loadGameIcon(253));
			m_arrIPaneImpl[i] = null;

			m_nCurPane = -1;
			i = findNextActivePane(nGamePane);
			activePane(m_arrIPaneImpl[i]);
		}
		//创建大厅面板
		public function CreatePlazaWindowPane():IPaneImpl
		{
			for (var i:int = 0; i < m_arrIPaneImpl.length; i ++)
			{
				var pane:IPaneImpl = m_arrIPaneImpl[i];
				if (pane)
				{
					if (pane.getPaneType() == nPaneType_Plaza)
					{
						return pane;
					}
				}
			}
			var nFreeBtnGame:int = getFreeGamePanePosition();
			if (nFreeBtnGame == -1)
			{
				return null;
			}
			var plaza:IPaneImpl = PlazaPane.CreatePlazaPane();
			if (plaza.Create(m_IMain) != 0)
			{
				plaza.Destroy();
				plaza = null;
				return null;
			}
			plaza.showPane(this, true);
			m_arrIPaneImpl[nFreeBtnGame] = plaza;
			m_PlazaWindowNavBar.setBtnBindData(nFreeBtnGame,plaza);
			m_PlazaWindowNavBar.setBtnText(nFreeBtnGame,plaza.getPaneName());
			m_PlazaWindowNavBar.setBtnIcon(nFreeBtnGame,getPaneIcon(plaza));
			return plaza;
		}
		//进入大厅面板
		public function enterGamePane(pane:IPaneImpl):int
		{
			if(pane == null)
			{
				return -1;
			}
			var nEqualPane:int = findEqualPane(pane.getPaneParam());
			if (nEqualPane != -1)
			{
				return activePane(m_arrIPaneImpl[nEqualPane]);
			}
			var nFreeBtnGame:int = getFreeGamePanePosition();
			if (nFreeBtnGame == -1)
			{
				return -1;
			}
			pane.showPane(this,true);
			
			m_arrIPaneImpl[nFreeBtnGame] = pane;
			m_PlazaWindowNavBar.setBtnBindData(nFreeBtnGame,pane);
			m_PlazaWindowNavBar.setBtnText(nFreeBtnGame,pane.getPaneName());
			m_PlazaWindowNavBar.setBtnIcon(nFreeBtnGame,getPaneIcon(pane));
			
			activePane(pane);
			
			return 0;
		}
		//退出游戏面板
		public function exitGamePane(pane:IPaneImpl):void
		{
			destroyGamePane(findPaneIndex(pane));
		}
		//退出大厅窗口
		public function SendEventExitPlazaWindow():void
		{
			if(m_IMain)
			{
				m_IMain.resetAll();
				m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
			}
		}
		//获取网络钩子接口
		public function getSocketSink():IClientSocketSink
		{
			return m_PlazaWindowSocketSink as IClientSocketSink;
		}
		//获取大厅面板
		protected function findPlazaPane():PlazaPane
		{
			for (var i:int = 0; i < m_arrIPaneImpl.length; i ++)
			{
				var pane0:IPaneImpl = m_arrIPaneImpl[i];
				if (pane0 && 
				pane0.getPaneType()== nPaneType_Plaza)
				{
					return pane0 as PlazaPane;
				}
			}
			var pane:IPaneImpl = CreatePlazaWindowPane();
			activePane(pane);
			return pane as PlazaPane;
		}
		//////////////////////////////////////////////////////
		//网络消息
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			var Pane:IPaneImpl = findPlazaPane();
			if (Pane != null)
			{
				return Pane.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
			}
			else
			{
				return false;
			}
		}
		//网络连接消息
		public function OnSocketEventConnect(iErrorCode:int,pszErrorDesc:String,pIClientSocke:IClientSocket):Boolean
		{	
			return true;
		}
		//网络读取消息
		public function OnSocketEventRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if(m_IMain)
				return m_IMain.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
			return false;
		}
	
		//网络关闭消息
		public function OnSocketEventClose(pIClientSocke:IClientSocket,bCloseByServer:Boolean):Boolean
		{
			trace("断开连接");
			if (pIClientSocke.GetRecvPacketCount()==0)
			{
				m_IMain.ShowMessageBox(IDS_ERR_SERVERBUZY);
			}
			else
			{
				m_IMain.ShowMessageBox(IDS_ERR_NETDISCONNECTED);
			}
			m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
			m_IMain.HideStatusMessage();
			m_IMain.resetAll();
			return true;
		}
		//////////////////////////////////////////
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			var pane:PlazaPane = findPlazaPane();
			if(pane)
				pane.OnEventFinishLogon();
		}
		//切换全频事件
		public function OnEventToggleFullScreen(b:Boolean):void
		{
			if(m_PlazaWindowNavBar)
				m_PlazaWindowNavBar.OnEventToggleFullScreen(b);
		}
		//切换静音事件
		public function OnEventQueit(e:QuietEvent):void
		{
			if(m_PlazaWindowNavBar)
				m_PlazaWindowNavBar.OnEventToggleSound(e.IsQuiet());
		}
		//获取游戏面板图标
		public function getPaneIcon(plaza:IPaneImpl):Icon
		{
			if(plaza == null)
				return null;
			if(plaza.getPaneType() == nPaneType_Plaza)
			{
				return GameIconUtil.getLeafIcon(SUB_GP_LIST_ROOT,null);
			}
			else
			{
				return GameIconUtil.getLeafIcon(SUB_GP_LIST_KIND,plaza.getGameKind());
			}
			return null;
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/InterfaceModuleIDef.as"
include "../../Net/NetModuleIDef.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"
include "./ConstantDef.as"