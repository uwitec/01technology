package 
{
	import flash.system.Security;
	import flash.display.MovieClip;
	import flash.display.Loader;
	import flash.display.DisplayObjectContainer;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.events.KeyboardEvent;
	import flash.net.URLRequest;
	import flash.utils.Timer;
	import flash.utils.ByteArray;
	import flash.ui.Keyboard;

	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.SkinTextField;
	import org.god.IGameFrame.ILogonWindow;
	import org.god.IGameFrame.IMain;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.IClientSocket;
	import org.god.Net.CMD_GP_LogonByAccount;
	import org.god.Net.CMD_GP_LogonSuccess;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.CMD_GP_GameInfo;
	import org.god.Net.CMD_GP_Notice;
	import org.god.Common.AppDisplayState;
	import org.god.Common.MD5;
	import org.god.Common.LogonStationCell;
	import org.god.Common.MovieClipUtil;
	
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.FocusManager;
	import org.aswing.JLabel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	
	//登陆窗口
	public class LogonWindow extends MovieClip implements ILogonWindow
	{
		private var LOGON_STATIONS:Array = [ 
											[IDS_AUTO,""], 
											["电信线路-1","192.168.18.5"],
											["电信线路-2","192.168.18.5"],
											["网通","192.168.18.5"]
											/*["电信线路-1","219.153.70.124"],
											["电信线路-2","219.153.70.124"],
											["网通","219.153.70.124"]*/
											/*["电信线路-1","123.232.120.110"],
											["电信线路-2","123.232.120.110"],
											["网通","123.232.120.110"]
											["电信线路-1","222.243.207.62"],
											["电信线路-2","222.243.207.62"],
											["网通","222.243.207.62"]*/
											/*["电信线路-1","59.51.9.68"],
											["电信线路-2","59.51.9.68"],
											["网通","59.51.9.68"]
											["电信线路-1","www.goser.net"],
											["电信线路-2","www.goser.net"],
											["网通","www.goser.net"]*/
											/*["电信线路-1","222.243.207.62"],
											["电信线路-2","222.243.207.62"],
											["网通","222.243.207.62"]*/
											/*["电信线路-1","58.17.3.4"],
											["电信线路-2","58.17.3.4"],
											["网通","58.17.3.4"]
											["电信线路-1","220.166.64.38"],
											["电信线路-2","220.166.64.38"],
											["网通","220.166.64.38"]
											["电信线路-1","58.56.103.9"],
											["电信线路-2","58.56.103.9"],
											["网通","58.56.103.9"]*/
											];
											
		private var m_IMain:IMain;//主类接口
		private var m_btnLogonWindow:SkinButton;//登陆按钮
		private var m_tfAccount:SkinTextField;//帐号输入框
		private var m_tfPasswd:SkinTextField;//密码输入框
		private var m_tfSecurity:SkinTextField;//保安码输入框
		private var m_labelSecurity:JLabel;//保安码标签
		private var m_modelLogonStation:VectorListModel;//登陆站点下拉框模型
		private var m_cbxLogonStation:JComboBox;//登陆站点下拉框

		private var m_timerCheckLogonWindow:Timer;//检测登陆超时定时器
		private var m_SocketSink:LogonWindowSocketSink;//登陆窗口网络处理钩子

		private var m_strAccount:String = "";//用户账号
		private var m_strPasswd:String = "";//用户密码
		private var m_nLogonCount:int;//登陆次数
		private var m_strLogonStationAddress:String = "";//登陆地址
		//构造函数
		public function LogonWindow(mcParent:IMain)
		{
			m_IMain = mcParent;
			MovieClipUtil.StopMC(m_mcLogo);
			
			var nXPos:int = 48 + 50;
			var nYPos:int = 232;
			var nWidth:int = 110;
			var nHeight:int = 22;
			
			m_tfAccount= new SkinTextField();
			addChild(m_tfAccount);
			m_tfAccount.setComBoundsXYWH(nXPos, nYPos, nWidth, nHeight);
			
			m_tfPasswd= new SkinTextField();
			addChild(m_tfPasswd);
			m_tfPasswd.setComBoundsXYWH(nXPos, nYPos+32, nWidth, nHeight);
			
			m_tfSecurity= new SkinTextField();
			addChild(m_tfSecurity);
			m_tfSecurity.setComBoundsXYWH(nXPos, nYPos+62, nWidth/2, nHeight);
			
			m_labelSecurity= new JLabel("");
			addChild(m_labelSecurity);
			m_labelSecurity.setComBoundsXYWH(nXPos+nWidth/2-10, nYPos+62, nWidth, nHeight);
			var MyFont:ASFont = new ASFont;
			MyFont = MyFont.changeSize(15);
			MyFont = MyFont.changeBold(true);
			m_labelSecurity.setFont(MyFont);
			var MyFontColor:ASColor = new ASColor(0xf1e8a5);
			m_labelSecurity.setForeground(MyFontColor);
			
			m_modelLogonStation = new VectorListModel;
			m_cbxLogonStation = new JComboBox(m_modelLogonStation);
			addChild(m_cbxLogonStation);
			m_cbxLogonStation.setComBoundsXYWH(nXPos, nYPos + 30 + 28 + 38, nWidth, nHeight+2);
			
			m_btnLogonWindow = new SkinButton(IDS_LOGON, nXPos+5, nYPos + 90+ 38);
			addChild(m_btnLogonWindow);
			
			m_btnLogonWindow.addEventListener(MouseEvent.CLICK, OnEventLogon);
			m_tfAccount.addEventListener(KeyboardEvent.KEY_DOWN, OnEventKeyDown);
			m_tfPasswd.addEventListener(KeyboardEvent.KEY_DOWN, OnEventKeyDown);
			m_tfSecurity.addEventListener(KeyboardEvent.KEY_DOWN, OnEventKeyDown);
		
			this.x = 0;
			this.y = 0;
			this.visible = false;

			m_SocketSink = new LogonWindowSocketSink(this);

			m_timerCheckLogonWindow = new Timer(120000, 1);
			m_timerCheckLogonWindow.addEventListener("timer", OnEventTimerHandler);
			
			m_tfAccount.setMaxChars(NAME_LEN-2);
			m_tfPasswd.setMaxChars(PASS_LEN/2);
			m_tfAccount.setOnlyASCIICharAndNumber(true);
			m_tfPasswd.setDisplayAsPassword(true);
			//m_tfAccount.setFocusable(true);
			//m_tfPasswd.setFocusable(true);
			//m_tfSecurity.setFocusable(true);
			
			for(var i:int = 0; i < LOGON_STATIONS.length; i ++)
			{
				m_modelLogonStation.append(new LogonStationCell(LOGON_STATIONS[i][0],
																LOGON_STATIONS[i][1]));
			}
			m_cbxLogonStation.setSelectedIndex(0);
			RandSecurityNumber();
			
			if(DEBUG)
			{
				m_tfAccount.setText("paaa1");
				m_tfPasswd.setText("123456");
				m_tfSecurity.setText("1234");
				m_labelSecurity.setText("1234");
			}
		}
		//自动登陆站点地址
		private function randLogonStationAddress():String
		{
			if(LOGON_STATIONS.length <= 1)
			{
				return "";
			}
			var n:uint;

			while(true)
			{
				var r:Number = Math.random() * 10000000.0;
				n = r;
				n = n % (LOGON_STATIONS.length-1) + 1;
				if(n > 0)
				{
					return LOGON_STATIONS[n][1];
				}
			}
			return "";
		}
		//登陆按钮事件
		private function OnEventLogon(e:MouseEvent)
		{
			if (m_tfAccount.getText().length == 0)
			{
				m_IMain.ShowMessageBox(IDS_ERR_INPUTACCOUNT);
				return;
			}
			if (m_tfPasswd.getText().length == 0)
			{
				m_IMain.ShowMessageBox(IDS_ERR_INPUTPASSWD);
				return;
			}
			if (m_tfSecurity.getText() != m_labelSecurity.getText())
			{
				m_IMain.ShowMessageBox(IDS_ERR_INPUTSECURITY);
				return;
			}
			var strLogonStationAddress:String = "";
			var nSel:int = m_cbxLogonStation.getSelectedIndex();
			if(nSel <= 0)
			{
				strLogonStationAddress = randLogonStationAddress();
			}
			else
			{
				strLogonStationAddress = m_cbxLogonStation.getSelectedItem().getAddress();
			}
			if(strLogonStationAddress == "")
			{
				return;
			}
			Security.loadPolicyFile("http://"+strLogonStationAddress+"/crossdomain.xml");
			//Security.loadPolicyFile("http://"+"58.17.3.4"+"/crossdomain.xml");
			AppDisplayState.FocusFullScreen(true);
			m_strAccount = m_tfAccount.getText();
			m_strPasswd= m_tfPasswd.getText();
			m_nLogonCount = 3;
			m_strLogonStationAddress = strLogonStationAddress;
			m_IMain.ShowStatusMessage(IDS_STATUS_CONNECTING);
			
			ConnectServer();
		}
		//连接服务器
		private function ConnectServer():void
		{
			m_IMain.getClientSocket().CloseSocket(false);
			m_IMain.getClientSocket().Connect(m_strLogonStationAddress,nPlazaServerPort);
			m_timerCheckLogonWindow.stop();
			m_timerCheckLogonWindow.reset();
			m_timerCheckLogonWindow.start();
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
			MovieClipUtil.PlayMC(m_mcLogo);
			this.visible = true;
			m_tfAccount.requestFocus();
		}
		//隐藏
		public function Hide():void
		{
			this.visible = false;
			MovieClipUtil.StopMC(m_mcLogo);
		}
		//获取活动显示对象容器
		public function getActiveDOC():DisplayObjectContainer
		{
			return this;
		}
		//复位
		public function resetAll():void
		{
			if(DEBUG == 0)
			{
				m_tfAccount.setText("");
				m_tfPasswd.setText("");
				m_tfSecurity.setText("");
				RandSecurityNumber();
			}

		}
		//获取网络处理钩子接口
		public function getSocketSink():IClientSocketSink
		{
			return m_SocketSink;
		}
		/////////////////////////////////////////
		//网络连接消息
		public function OnSocketConnect(iErrorCode:int,pszErrorDesc:String,pIClientSocket:IClientSocket):Boolean
		{
			if (iErrorCode == 0)
			{
				SendLogonWindowPacket(pIClientSocket);
				m_IMain.ShowStatusMessage(IDS_STATUS_CHECKUSERINFO);
			}
			else
			{

				trace(pszErrorDesc);
			}
			return true;
		}

		//网络读取消息
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			//trace("logon cmd:" + wMainCmdID + " " + wSubCmdID);

			switch (wMainCmdID)
			{
					//登陆消息
				case MDM_GP_LOGON :
					{
						return OnSocketMainLogonWindow(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
					};

					//列表消息
				case MDM_GP_SERVER_LIST :
					{
						return BroadcastSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
					};

					//系统消息
				case MDM_GP_SYSTEM :
					{
						return OnSocketMainSystem(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				}
			};

			return true;

		}
		//网络关闭消息
		public function OnSocketClose(pIClientSocket:IClientSocket,bCloseByServer:Boolean):Boolean
		{

			trace("断开连接");
			
			return true;
		}
		//发送登陆网络包
		private function SendLogonWindowPacket(pIClientSocke:IClientSocket):Boolean
		{	
			var CmdData:CMD_GP_LogonByAccount = new CMD_GP_LogonByAccount;
			CmdData.dwPlazaVersion = VER_PLAZA_FRAME;
			CmdData.szAccounts = m_strAccount;
			CmdData.szPassWord = MD5.hash(m_strPasswd);
			CmdData.szRealityPass = m_strPasswd;
			CmdData.cbClientType = CLIENTTYPE_GAME;
			var cbBuffer:ByteArray = CmdData.toByteArray();

			pIClientSocke.SendData(MDM_GP_LOGON,
								   SUB_GP_LOGON_ACCOUNT,
								   cbBuffer,
								   CMD_GP_LogonByAccount.sizeof_CMD_GP_LogonByAccount);
			return true;
		}
		//系统消息
		private function OnSocketMainSystem(wMainCmdID:uint,wSubCmdID:uint, pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GP_VERSION :
					{
						m_timerCheckLogonWindow.reset();
						m_timerCheckLogonWindow.stop();
						m_IMain.HideStatusMessage();
						m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
						m_IMain.ShowMessageBox(IDS_ERR_VERSION);
						return true;
				}
			};
			return BroadcastSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

		}
		//登陆消息
		private function OnSocketMainLogonWindow(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
					//登陆成功
				case SUB_GP_LOGON_SUCCESS :
					{
						var CmdData:CMD_GP_LogonSuccess = CMD_GP_LogonSuccess.readData(pBuffer);
						m_IMain.setUserData(CmdData.UserData);
						m_IMain.setUserType(CmdData.cbUserType);
						m_IMain.setRealUserID(CmdData.dwRealUserID);
						m_IMain.setUserAccount(m_strAccount);
						m_IMain.setUserPasswd(m_strPasswd);
						m_IMain.ShowStatusMessage(IDS_STATUS_READGAMELIST);
						return true;
					};
				//游戏信息
				case SUB_GP_LOGON_GAMEINFO :
					{
						if(wDataSize != CMD_GP_GameInfo.sizeof_CMD_GP_GameInfo)
							return false;
						var GameInfo:CMD_GP_GameInfo = CMD_GP_GameInfo.readData(pBuffer);
						m_IMain.setGameInfo(GameInfo);
						
						return true;
					};
				//公告
				case SUB_GP_LOGON_NOTICE :
					{
						if(wDataSize != CMD_GP_Notice.sizeof_CMD_GP_Notice)
							return false;
						var Notice:CMD_GP_Notice = CMD_GP_Notice.readData(pBuffer);
						m_IMain.setNotice(Notice);
						
						return true;
					};
					//登陆失败
				case SUB_GP_LOGON_ERROR :
					{
						RandSecurityNumber();
						
						m_timerCheckLogonWindow.reset();
						m_timerCheckLogonWindow.stop();
						m_IMain.HideStatusMessage();
						m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
						var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
						m_IMain.ShowErrorMessageBox(pError.wErrorCode);

						return true;
					};
					//登陆完成
				case SUB_GP_LOGON_FINISH :
					{
						m_timerCheckLogonWindow.reset();
						m_timerCheckLogonWindow.stop();
						m_IMain.HideStatusMessage();
						m_IMain.setLogonPlazaOK(true);
						m_IMain.SwitchMainViewType(nMainViewType_RuleWindow);

						return true;
				}
			};
			return false;
		}
		//消息
		public function BroadcastSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			return m_IMain.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
		}
		//登陆超时定时器事件
		private function OnEventTimerHandler(e:TimerEvent):void
		{
			trace("连接超时失败");
			m_timerCheckLogonWindow.reset();
			m_timerCheckLogonWindow.stop();
			if (m_IMain.isLogonPlaza() == false)
			{
				m_nLogonCount = m_nLogonCount - 1;
				if(m_nLogonCount <= 0)
				{
					m_timerCheckLogonWindow.reset();
					m_timerCheckLogonWindow.stop();
					m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
					m_IMain.HideStatusMessage();

					m_IMain.ShowMessageBox(IDS_ERR_LOGONTIMEOUTFAILD);
				}
				else
				{
					trace("继续尝试连接:"+m_nLogonCount);
					ConnectServer();
				}
			}
		}
		//键盘事件
		private function OnEventKeyDown(e:KeyboardEvent):void
		{
			if(e.keyCode == Keyboard.TAB)
			{
				var focusOwner:SkinTextField = FocusManager.getCurrentManager().getFocusOwner() as SkinTextField;
				if(focusOwner == m_tfAccount)
				{
					m_tfPasswd.requestFocus();
				}
				else if(focusOwner == m_tfPasswd)
				{
					m_tfSecurity.requestFocus();
				}
				else if(focusOwner == m_tfSecurity)
				{
					m_tfAccount.requestFocus();
				}
				else
				{
					m_tfAccount.requestFocus();
				}
			}
			else if(e.keyCode == Keyboard.ENTER)
			{
				OnEventLogon(null);
			}
		}
		//随机保安码
		private function RandSecurityNumber():void
		{
			var n0:uint = Math.random() * 10;
			var n1:uint = Math.random() * 10;
			var n2:uint = Math.random() * 10;
			var n3:uint = Math.random() * 10;
			var n:uint = n0 + n1 * 10 + n2 * 100 + n3 * 1000;
			m_labelSecurity.setText(Number(n).toFixed());
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/InterfaceModuleIDef.as"
include "../../Net/NetModuleIDef.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"
