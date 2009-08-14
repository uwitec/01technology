package 
{
	import flash.display.Sprite;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.events.MouseEvent;
	import flash.utils.ByteArray;

	import org.god.IGameManagement.IPlazaWindow;
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.IPanelImpl;
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.CMD_GF_Message;
	import org.god.Net.CMD_GR_Message;	
	import org.god.Common.AppDisplayState;
	import org.god.SkinControl.SkinScrollTextField.ScrollTextField;
	
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.geom.IntRectangle;
	//大厅窗口
	public class PlazaWindow extends Sprite implements IPlazaWindow
	{
		private var m_IMain:IMain;//主类接口
		private var m_arrIPanelImpl:Array;//面板数组
		private var m_arrNavCmdButton:Array;//导航按钮数组
		private var m_nCurPanel:uint = 0xff;//当前面板索引
		private var m_bSetup:Boolean;//初始话标记
		
		private var m_TDPanelMotion:TDPanelMotion;//3d转动
		private var m_PanelContainer:Sprite;//面板容器
		private var m_Panel3DMotion:Sprite;//3d转动面板容器
		private var m_PlazaWindowSocketSink:PlazaWindowSocketSink;//大厅窗口网络钩子

		private var m_ScrollTextField:ScrollTextField;//公告滚动控件
		
		public function PlazaWindow(mcParent:IMain)
		{
			m_IMain = mcParent;
			m_bSetup = false;			
			this.x = 0;
			this.y = 0;
			this.visible = false;
			m_PlazaWindowSocketSink = new PlazaWindowSocketSink(this);
		}
		//初始化大厅窗口
		private function SetupPlazaWindow():void
		{
			m_PanelContainer = new PanelContainer;
			addChild(m_PanelContainer);
			m_Panel3DMotion = new PanelContainer;
			addChild(m_Panel3DMotion);
			m_Panel3DMotion.visible = false;
			//if(DEBUG == 0)
			{
				m_TDPanelMotion = new TDPanelMotion(m_Panel3DMotion,1024,635);
				m_TDPanelMotion.addEventListener(MotionCompleteEvent.strEventName_MotionComplete,OnEventMotionComplete);
			}
			m_arrIPanelImpl = new Array();
			m_arrNavCmdButton = new Array();
			
			var panelimpl_MySelfDataPanel:IPanelImpl = getIPanelImpl("MySelfDataPanel");
			m_arrIPanelImpl.push(panelimpl_MySelfDataPanel);
			
			var panelimpl_AccountPanel:IPanelImpl = getIPanelImpl("AccountPanel");
			m_arrIPanelImpl.push(panelimpl_AccountPanel);

			var panelimpl_ChartPanel:IPanelImpl = getIPanelImpl("ChartPanel");
			m_arrIPanelImpl.push(panelimpl_ChartPanel);
			
			var panelimpl_BetHistoryPanel:IPanelImpl = getIPanelImpl("BetHistoryPanel");
			m_arrIPanelImpl.push(panelimpl_BetHistoryPanel);
			
			var panelimpl_BetMonitorPanel:IPanelImpl = getIPanelImpl("BetMonitorPanel");
			m_arrIPanelImpl.push(panelimpl_BetMonitorPanel);
			
			var panelimpl_OperationPanel:IPanelImpl = getIPanelImpl("OperationPanel");
			m_arrIPanelImpl.push(panelimpl_OperationPanel);
			
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_MySelfData,IMAGE_NAVBT_MYSELFDATA,360,32));
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_Account,IMAGE_NAVBT_ACCOUNT,360,32));
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_Chart,IMAGE_NAVBT_CHART,360,32));
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_BetHistory,IMAGE_NAVBT_BETHISTORY,360,32));
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_BetMonitor,IMAGE_NAVBT_BETMONITOR,360,32));
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_Operation,IMAGE_NAVBT_OPERATION,360,32));
			m_arrNavCmdButton.push(new NavCmdButton(nNavCmd_Logout,IMAGE_NAVBT_LOGOUT,360,32));
			
			for(var i:uint = 0; i < m_arrNavCmdButton.length; i ++)
			{
				if(m_arrNavCmdButton[i] != null)
				{
					addChild(m_arrNavCmdButton[i]);
					m_arrNavCmdButton[i].addEventListener(MouseEvent.CLICK, OnEventNavCmd);
				}
				if(m_arrIPanelImpl[i] != null)
				{
					m_PanelContainer.addChild(m_arrIPanelImpl[i].getActiveDC());
					m_arrIPanelImpl[i].Create(m_IMain, this);
				}
			}
			m_btnFullScreen.addEventListener(MouseEvent.CLICK, OnEventFullScreen);
			m_btnDisplayNormal.addEventListener(MouseEvent.CLICK, OnEventDisplayNormal);
			m_btnDisplayNormal.visible = false;
			m_btnFullScreen.visible = false;
			
		 	var MyFontColor:ASColor = new ASColor(0xCC9900);
		 	var MyFont = new ASFont;
			MyFont = MyFont.changeSize(12);
			MyFont = MyFont.changeBold(true);
			m_ScrollTextField = new ScrollTextField(new IMAGE_NOTIFYPANE(502,36),
													null,502,36,
													ScrollTextField.eDirect_Left,
													false,6,4,485,17);
			addChild(m_ScrollTextField);
			m_ScrollTextField.setLocationXY(342,20);
			m_ScrollTextField.setFont(MyFont);
			m_ScrollTextField.setForeground(MyFontColor);
			m_ScrollTextField.Start();
		}
		//获取面板接口
		private function getIPanelImpl(strModuleName:String):IPanelImpl
		{
			var c:Class = m_IMain.getDefClass(strModuleName,strModuleName);
			var t:* = new c();
			return t;
		}
		//布局
		private function RectifyPlazaWindow():void
		{
			m_PanelContainer.x = 0;
			m_PanelContainer.y = 134;
			m_Panel3DMotion.x = 0;
			m_Panel3DMotion.y = 134;
		
			var cb:IntRectangle = new IntRectangle(0,0,1024,635);
			
			for(var i:uint = 0; i < m_arrNavCmdButton.length; i ++)
			{
				if(m_arrNavCmdButton[i] != null)
				{
					m_arrNavCmdButton[i].x = 210 + 78 * i;
					m_arrNavCmdButton[i].y = 95;
				}
				if(m_arrIPanelImpl[i] != null)
				{
					m_arrIPanelImpl[i].moveDC(cb);
					m_arrIPanelImpl[i].validateDC();
					m_arrIPanelImpl[i].setVisibleDC(false);
				}
			}
		}
		//////////////////////////////////////////////////
		//IPlazaWindow接口实现
		//////////////////////////////////////////////////
		//获取主类接口
		public function getIMain():IMain
		{
			return m_IMain;
		}		
		//显示
		public function Show():void
		{
			if(m_bSetup == false)
			{
				SetupPlazaWindow();
				RectifyPlazaWindow();
			
				activePanel(nNavCmd_MySelfData);
				
				m_bSetup = true;
			}
			this.visible = true;
		}
		//隐藏
		public function Hide():void
		{
			this.visible = false;
		}
		//获取活动显示容器
		public function getActiveDOC():DisplayObjectContainer
		{
			return this;
		}
		//复位
		public function resetAll():void
		{
			if(m_bSetup)
			{
				if(m_TDPanelMotion)
				{
					m_TDPanelMotion.removeEventListener(MotionCompleteEvent.strEventName_MotionComplete,OnEventMotionComplete);
					m_TDPanelMotion.Destroy();
					m_TDPanelMotion = null;
				}
				
				for (var i:int = 0; i < m_arrIPanelImpl.length; i ++)
				{
					var pane:IPanelImpl = m_arrIPanelImpl[i];
					if (pane)
					{
						m_PanelContainer.removeChild(m_arrIPanelImpl[i].getActiveDC());
						pane.Destroy();

						m_arrIPanelImpl[i] = null;
					}
				}
				for (i = 0; i < m_arrNavCmdButton.length; i ++)
				{
					if (m_arrNavCmdButton[i])
					{
						removeChild(m_arrNavCmdButton[i]);
						m_arrNavCmdButton[i] = null;
					}
				}
				m_arrIPanelImpl = null;
				m_arrNavCmdButton = null;
				
				removeChild(m_PanelContainer);
				m_PanelContainer = null;
				removeChild(m_Panel3DMotion);
				m_Panel3DMotion = null;
				
				m_ScrollTextField.Stop();
				m_ScrollTextField.Destroy();
				removeChild(m_ScrollTextField);
				m_ScrollTextField = null;

				m_nCurPanel = 0xff;
				m_bSetup = false;
			}
		}
		//退出大厅
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

		//网络读取消息
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wMainCmdID)
			{
				case MDM_GP_SYSTEM :
				{
					return OnSocketMainSystem(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				}
				default:
				break;
			}
			for(var i:uint = 0; i < m_arrIPanelImpl.length; i ++)
			{
				if(m_arrIPanelImpl[i] &&
					m_arrIPanelImpl[i].OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket))
					return true;
			}
			return true;
		}
		//活动管理面板
		public function activePanel(nNavCmd:uint,pParam0:*=null, pParam1:*=null):uint
		{
			var nFindNavCmd:uint = 0xff;
			if(m_TDPanelMotion != null)
			{
				if(m_TDPanelMotion.IsMotioning())
					return nFindNavCmd;
			}
			
			for(var i:uint = 0; i < m_arrNavCmdButton.length - 1; i ++)
			{
				if(m_arrNavCmdButton[i])
				{
					if( m_arrNavCmdButton[i].GetNavCmd() == nNavCmd)
					{
						nFindNavCmd = nNavCmd;
						m_arrNavCmdButton[i].SetToggle(true);
					}
					else
					{
						m_arrNavCmdButton[i].SetToggle(false);
					}
				}
				if(m_arrIPanelImpl[i])
				{
					m_arrIPanelImpl[i].setVisibleDC(false);
					if( m_arrIPanelImpl[i].getNavCmd() == nNavCmd &&
						m_nCurPanel != nNavCmd)
					{
						m_arrIPanelImpl[i].setVisibleDC(false);
						m_arrIPanelImpl[i].Active(pParam0);
					}
					else
					{
						m_arrIPanelImpl[i].setVisibleDC(false);
						m_arrIPanelImpl[i].Deactive();
					}
				}
			}
			if(nFindNavCmd != 0xff)
			{
				if(m_nCurPanel != 0xff &&
				   m_nCurPanel != nFindNavCmd &&
				   m_TDPanelMotion)
				{
					if(m_arrIPanelImpl[m_nCurPanel] &&
						m_arrIPanelImpl[nFindNavCmd])
					{
						m_TDPanelMotion.Motion_SwitchTab(m_arrIPanelImpl[m_nCurPanel].getActiveDC(),
										m_nCurPanel,
										m_arrIPanelImpl[nFindNavCmd].getActiveDC(),
										nFindNavCmd,
										m_nCurPanel > nFindNavCmd ? true : false);
						m_Panel3DMotion.visible = true;
					}
				}
				else
				{
					if(m_arrIPanelImpl[nFindNavCmd])
						m_arrIPanelImpl[nFindNavCmd].setVisibleDC(true);
				}
				m_nCurPanel = nFindNavCmd;
			}
			return nFindNavCmd;
		}
		//系统消息
		private function OnSocketMainSystem(wMainCmdID:uint,wSubCmdID:uint, pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			
			switch(wSubCmdID)
			{
				case SUB_GF_MESSAGE:
				{
					if(wDataSize < CMD_GF_Message.sizeof_CMD_GF_Message)
						return false;
					var Message:CMD_GF_Message = CMD_GF_Message.readData(pBuffer);
					if(Message.wErrorCode != 0)
					{
						m_IMain.ShowErrorMessageBox(Message.wErrorCode);
					}
					else
					{
						m_IMain.ShowMessageBox(Message.szContent);
					}
					var wTypeMark0:uint = Message.wMessageType&SMT_INTERMIT_LINE;
					var wTypeMark1:uint = Message.wMessageType&SMT_CLOSE_ROOM;
					if(wTypeMark0 != 0 ||
						wTypeMark1 != 0)
					{
						SendEventExitPlazaWindow();
					}
					
					return true;
				}
			}
			return false;
		}
		//////////////////////////////////////
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
			m_IMain.resetAll();
			m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
			m_IMain.HideStatusMessage();

			if (pIClientSocke.GetRecvPacketCount()==0)
			{
				m_IMain.ShowMessageBox(IDS_ERR_SERVERBUZY);
			}
			else
			{
				m_IMain.ShowMessageBox(IDS_ERR_NETDISCONNECTED);
			}
			return true;
		}
		//切换全屏事件
		public function OnEventToggleFullScreen(b:Boolean):void
		{
			return;
			if(b)
			{
				m_btnFullScreen.visible = false;
				m_btnDisplayNormal.visible = true;
			}
			else
			{
				m_btnFullScreen.visible = true;
				m_btnDisplayNormal.visible = false;
			}
		}
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			if(m_ScrollTextField)
				m_ScrollTextField.UpdateText(m_IMain.getNotice().szNotice);
		}

		///////////////////////////////////////////////////////////////
		//导航按钮事件
		private function OnEventNavCmd(e:MouseEvent):void
		{
			var nNavCmd:uint = e.currentTarget.GetNavCmd();
			if(nNavCmd == nNavCmd_Logout)
			{
				SendEventExitPlazaWindow();
			}
			else
			{
				activePanel(nNavCmd);
			}
		}
		//3d转动完成事件
		private function OnEventMotionComplete(e:MotionCompleteEvent):void
		{
			if(m_arrIPanelImpl[e.m_nIndex0])
				m_arrIPanelImpl[e.m_nIndex0].setVisibleDC(false);
			if(m_arrIPanelImpl[e.m_nIndex1])
				m_arrIPanelImpl[e.m_nIndex1].setVisibleDC(true);
			m_Panel3DMotion.visible = false;
		}
		//全屏按钮事件
		private function OnEventFullScreen(e:MouseEvent):void
		{
			AppDisplayState.toggleFullScreen();
		}
		//普通显示按钮事件
		private function OnEventDisplayNormal(e:MouseEvent):void
		{
			AppDisplayState.toggleFullScreen();
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/InterfaceModuleIDef.as"
include "../../Net/NetModuleIDef.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/GLOBALFRAME.as"
include "../../Net/CMD_Plaza.as"
include "./ConstantDef.as"
