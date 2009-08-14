package
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.utils.ByteArray;
	
	import org.god.Common.MD5;
	import org.god.Common.GameTreeNode;
	import org.god.Common.enServiceStatus;
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.IUnknownEx;
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagTableStatus;
	import org.god.Net.tagUserData;
	import org.god.Net.tagUserStatus;
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.ClientSocket;
	import org.god.Net.CMD_GR_LogonByAccount;
	import org.god.Net.CMD_GR_UserSitReq;
	import org.god.Net.CMD_GR_ServerInfo;
	import org.god.Net.CMD_GR_TableInfo;
	import org.god.Net.CMD_GR_TableStatus;
	import org.god.Net.CMD_GR_UserStatus;
	import org.god.Net.CMD_GR_UserScore;
	import org.god.Net.CMD_GR_SitFailed;
	import org.god.Net.CMD_GR_LogonError;
	import org.god.Net.CMD_GF_Message;
	import org.god.Net.CMD_GR_Message;
	import org.god.Net.tagGameServerOnLineCountInfo;
	import org.god.IGameManagement.*;
	import org.god.IGameFrame.IUserItem;

	import org.aswing.JPanel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	//基本现场投注页面
	public class BaseMonitorTab extends JPanel implements IClientSocketSink
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		
		protected var m_IMain:IMain;
		protected var m_IPlazaWindow:IPlazaWindow;
		protected var m_BetMonitorPanel:BetMonitorPanel;
		protected var m_gameType:GameTreeNode;
		protected var m_gameKind:GameTreeNode;
		protected var m_gameServer:GameTreeNode;
		
		protected var m_clientSocket:ClientSocket;
		protected var m_isLogonOK:Boolean;
		protected var m_ServiceStatus:uint;
		protected var m_ClientUserManager:ClientUserManager;//用户管理

		protected var m_bmBackground:Bitmap;
		
		public function BaseMonitorTab()
		{
			super();
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(true);
			}
		}
		public function Create(param0:IMain,
							   param1:IPlazaWindow,
							   param2:BetMonitorPanel,
							   gameType:GameTreeNode = null,
								gameKind:GameTreeNode = null,
								gameServer:GameTreeNode = null):int
		{
			m_IMain = param0;
			m_IPlazaWindow = param1;
			m_BetMonitorPanel = param2;
			m_gameType = gameType;
			m_gameKind = gameKind;
			m_gameServer = gameServer;
			
			m_bmBackground = new Bitmap(CreateBackgroundBitmapData());
			addChild(m_bmBackground);
			m_bmBackground.y = 0;
			m_bmBackground.x = 0;
			
			m_ClientUserManager = new ClientUserManager;
			m_ClientUserManager.SetUserManagerSink(this);

			m_ServiceStatus= enServiceStatus.ServiceStatus_Null;
			m_clientSocket = new ClientSocket;
			
			return 0;
		}
		public function Destroy():void
		{
			if (m_clientSocket)
			{
				m_clientSocket.CloseSocket(false);
				m_clientSocket = null;
			}
			m_ClientUserManager.Destroy();
			m_ClientUserManager = null;
			
			removeChild(m_bmBackground);
			m_bmBackground.bitmapData.dispose();
			m_bmBackground = null;
			
			m_IMain = null;
			m_IPlazaWindow = null;
			m_BetMonitorPanel = null;
		}
		public function StartMonitor():void
		{
			DoEnterGameWork();
		}
		public function StopMonitor():void
		{
			if (m_clientSocket)
			{
				m_clientSocket.CloseSocket(false);
			}
		}
		////////////////////////////////////
		protected function CreateBackgroundBitmapData():BitmapData
		{
			return new IMAGE_BLANK_TAB(820,548);
		}
		public function GetMonitorTitle():String
		{
			return "";
		}
		protected function UpdateGameServerOnLine(wServerID:uint,dwOnLineCount:uint):void
		{
		}
		protected function UpdateGameOnLineCount(dwOnLineCount:uint):void
		{
		}
		public function EqualGameTreeNode(gameServer:GameTreeNode):Boolean
		{	
			if(gameServer == null || m_gameServer == null)
				return false;
			return m_gameServer.equalItemID(gameServer.getItemID());
		}
		
		public function getClientSocket():ClientSocket
		{
			return m_clientSocket;
		}
		public function DoEnterGameWork():void
		{
			getClientSocket().SetSocketSink(this);
			var server:tagGameServer  = m_gameServer.getItemData();
			var strServerAddr:String = inet_ntoa(server.dwServerAddr);
			m_ServiceStatus= enServiceStatus.ServiceStatus_Connecting;
			getClientSocket().Connect(strServerAddr, server.wServerPort);
		}
		public function isLogonOK():Boolean
		{
			return m_isLogonOK;
		}
		public function ShowStatusMessage(strMsg:String):void
		{
			m_IMain.ShowStatusMessage(strMsg);
		}
		public function HideStatusMessage():void
		{
			m_IMain.HideStatusMessage();
		}
		public function ShowMessageBox(strMsg:String):void
		{
			m_IMain.ShowMessageBox(strMsg);
		}
		public function ShowErrorMessageBox(wErrCode:uint):void
		{
			m_IMain.ShowErrorMessageBox(wErrCode);
		}
		///////////////////////////////
		//网络连接消息
		public function OnSocketConnect(iErrorCode:int,pszErrorDesc:String,pIClientSocket:IClientSocket):Boolean
		{
			if (iErrorCode == 0)
			{
				m_ServiceStatus= enServiceStatus.ServiceStatus_EfficacyUser;
				if (SendLogonPacket(pIClientSocket) == false)
				{
					this.HideStatusMessage();
					this.ShowMessageBox(IDS_ERR_ENTERGAMEFAILED);
					m_BetMonitorPanel.CloseMonitorTab(this);
				}
				this.ShowStatusMessage(IDS_STATUS_CHECKUSERINFO);
			}
			else
			{
				this.HideStatusMessage();
				this.ShowMessageBox(IDS_ERR_SERVERBUZY);
				m_BetMonitorPanel.CloseMonitorTab(this);
			}
			return true;
		}

		//网络读取消息
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wMainCmdID)
			{
				case MDM_GR_LOGON ://登录消息
					{
						return OnSocketMainLogon(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case MDM_GR_USER ://用户消息
					{
						return OnSocketMainUser(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case MDM_GR_INFO ://配置信息
					{
						return OnSocketMainInfo(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case MDM_GR_STATUS ://状态信息
					{
						return OnSocketMainStatus(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case MDM_GR_SYSTEM ://系统消息
					{
						return OnSocketMainSystem(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case MDM_GR_SERVER_INFO ://房间信息
					{
						return OnSocketMainServerInfo(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case MDM_GF_GAME ://游戏消息
				case MDM_GF_FRAME ://框架消息
					{
						return OnSocketMainGameFrame(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

				}
			};

			return false;
		}
		//网络关闭消息
		public function OnSocketClose(pIClientSocket:IClientSocket,bCloseByServer:Boolean):Boolean
		{
			this.HideStatusMessage();

			if (pIClientSocket.GetRecvPacketCount()==0)
			{
				this.ShowMessageBox(IDS_ERR_SERVERBUZY);
			}
			else
			{
				this.ShowMessageBox(IDS_ERR_NETDISCONNECTED);
			}
			m_ServiceStatus= enServiceStatus.ServiceStatus_NetShutDown;
			m_BetMonitorPanel.CloseMonitorTab(this);
				
			return true;
		}
		////////////////////////////////
		//发送登陆命令
		private function SendLogonPacket(pIClientSocket:IClientSocket):Boolean
		{
			var main:IMain = GlobalMain.getMain();
			if (main == null)
			{
				return false;
			}
			var CmdData:CMD_GR_LogonByAccount = new CMD_GR_LogonByAccount;
			CmdData.dwPlazaVersion = VER_PLAZA_FRAME;
			CmdData.dwProcessVersion = 0;
			CmdData.szAccounts = String(main.getUserAccount());
			CmdData.szPassWord = MD5.hash((main.getUserPasswd()));
			var cbBuffer:ByteArray = CmdData.toByteArray();

			pIClientSocket.SendData(MDM_GR_LOGON,
			SUB_GR_LOGON_ACCOUNT,
			cbBuffer,
			CMD_GR_LogonByAccount.sizeof_CMD_GR_LogonByAccount);
			return true;
		}
		//发送强退命令
		private function SendLeftGamePacket():Boolean
		{
			if(m_clientSocket)
			{
				m_clientSocket.SendCmd(MDM_GR_USER,SUB_GR_USER_LEFT_GAME_REQ);
	
				return true;
			}
			else
			{
				return false;
			}
		}
		//发送起来命令
		private function SendStandUpPacket():Boolean
		{
			if(m_clientSocket)
			{
				m_clientSocket.SendCmd(MDM_GR_USER,SUB_GR_USER_STANDUP_REQ);
	
				return true;
			}
			else
			{
				return false;
			}
			return true;
		}


		//发送坐下命令
		public function SendSitDownPacket( wTableID:uint,  wChairID:uint,pszTablePass:String):Boolean
		{
			if(m_clientSocket == null)
			{
				return false;
			}
			//构造数据包
			var UserSitReq:CMD_GR_UserSitReq = new CMD_GR_UserSitReq;
			
			UserSitReq.wTableID=wTableID;
			UserSitReq.wChairID=wChairID;
			UserSitReq.wNetTimelag=0;
			UserSitReq.szTablePass = String(pszTablePass);
			UserSitReq.cbPassLen=pszTablePass.length;

			//发送数据包
			var wSendSize:uint=CMD_GR_UserSitReq.sizeof_CMD_GR_UserSitReq;
			
			var pPackageBuf:ByteArray = UserSitReq.toByteArray();
			m_clientSocket.SendData(MDM_GR_USER,SUB_GR_USER_SIT_REQ,pPackageBuf,wSendSize);

			return true;
		}
		//网络主命令//////////////////////////////////////
		//登录消息
		protected function OnSocketMainLogon(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_LOGON_SUCCESS ://登录成功
					{
						m_ServiceStatus=enServiceStatus.ServiceStatus_RecvConfigInfo;
						this.ShowStatusMessage(IDS_STATUS_READINGCONFIG);
						return true;

					};
				case SUB_GR_LOGON_ERROR ://登录失败
					{
						m_ServiceStatus= enServiceStatus.ServiceStatus_NetShutDown;
						this.HideStatusMessage();
						
						var pError:CMD_GR_LogonError = CMD_GR_LogonError.readData(pBuffer);
						this.ShowErrorMessageBox(pError.wErrorCode);

						m_BetMonitorPanel.CloseMonitorTab(this);

						return true;
					};
				case SUB_GR_LOGON_FINISH ://登录完成
					{
						m_ServiceStatus=enServiceStatus.ServiceStatus_Serviceing;
						this.HideStatusMessage();
						return true;
				}
			};
			return false;
		}
		//用户消息
		protected function OnSocketMainUser(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_USER_COME ://用户进入
					{
						return OnSocketSubUserCome(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);


					};
				case SUB_GR_USER_STATUS ://用户状态
					{
						return OnSocketSubStatus(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				case SUB_GR_USER_SCORE ://用户分数
					{
						return OnSocketSubScore(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
				
			};
			return false;
		}
		//配置消息
		protected function OnSocketMainInfo(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_SERVER_INFO ://房间信息
					{
						
						return true;
					};
				case SUB_GR_CONFIG_FINISH ://配置完成
					{
						m_ServiceStatus=enServiceStatus.ServiceStatus_RecvRoomInfo;
						return true;

				}
			};
			return false;
		}
		//状态消息
		protected function OnSocketMainStatus(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_TABLE_INFO:		//桌子信息
				{
		
					return true;
				}
				case SUB_GR_TABLE_STATUS:	//桌子状态
				{
					return true;
			 	 }
			}
			return false;
		}
		//系统消息
		protected function OnSocketMainSystem(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GR_MESSAGE:
				{
					if(wDataSize < CMD_GR_Message.sizeof_CMD_GR_Message)
						return false;
					var Message:CMD_GR_Message = CMD_GR_Message.readData(pBuffer);
					if(Message.wErrorCode != 0)
					{
						this.ShowErrorMessageBox(Message.wErrorCode);
					}
					else
					{
						this.ShowMessageBox(Message.szContent);
					}
					var wTypeMark0:uint = Message.wMessageType&SMT_INTERMIT_LINE;
					var wTypeMark1:uint = Message.wMessageType&SMT_CLOSE_ROOM;
					if(wTypeMark0  != 0 ||
						wTypeMark1 != 0)
					{
						m_BetMonitorPanel.CloseMonitorTab(this);
					}
					return true;
				}
			}
			return false;
		}
		//房间消息
		protected function OnSocketMainServerInfo(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
			case SUB_GR_ONLINE_COUNT_INFO:		//在线信息
				{
					//效验数据
					ASSERT(wDataSize%tagGameServerOnLineCountInfo.sizeof_tagGameServerOnLineCountInfo==0);
					if (wDataSize%tagGameServerOnLineCountInfo.sizeof_tagGameServerOnLineCountInfo!=0) return false;

					var pOnLineCountInfo:tagGameServerOnLineCountInfo=tagGameServerOnLineCountInfo.readData(pBuffer);
						
					UpdateGameServerOnLine(pOnLineCountInfo.wServerID,
										   pOnLineCountInfo.dwOnLineCount);
					
					return true;
				}
			}
			return false;
		}
		//游戏消息
		protected function OnSocketMainGameFrame(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
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
						this.ShowErrorMessageBox(Message.wErrorCode);
					}
					else
					{
						this.ShowMessageBox(Message.szContent);
					}
					var wTypeMark0:uint = Message.wMessageType&SMT_INTERMIT_LINE;
					var wTypeMark1:uint = Message.wMessageType&SMT_CLOSE_ROOM;
					if(wTypeMark0  != 0 ||
						wTypeMark1 != 0)
					{
						m_BetMonitorPanel.CloseMonitorTab(this);
					}
					return true;
				}
			}
			return false;
		}
		//网络子命令
		//用户进入
		protected function OnSocketSubUserCome(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if (wDataSize<tagUserInfoHead.sizeof_tagUserInfoHead)
			{
				return false;
			}
			var pUserInfoHead:tagUserInfoHead = tagUserInfoHead.readData(pBuffer);
			//查找用户
			var pIUserItem:IUserItem=m_ClientUserManager.SearchUserByID(pUserInfoHead.dwUserID);
			if (pIUserItem==null)
			{
				pIUserItem=m_ClientUserManager.ActiveUserItem(pUserInfoHead);
			}
			else
			{
				pIUserItem.UpdateUserData(pUserInfoHead);
			}
			if (pIUserItem != null)
			{
				
				//设置界面
				var cbUserStatus:uint=pUserInfoHead.UserStatus.cbUserStatus;
				if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
				{
					
				}

			}
			
			UpdateGameServerOnLine(m_gameServer.getItemData().wServerID,
					 m_ClientUserManager.GetOnLineCount());

			return true;
		}
		//用户状态
		protected function OnSocketSubStatus(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if (wDataSize<CMD_GR_UserStatus.sizeof_CMD_GR_UserStatus)
			{
				return false;
			}
			var pUserStatus:CMD_GR_UserStatus = CMD_GR_UserStatus.readData(pBuffer);
			var pIUserItem:IUserItem=m_ClientUserManager.SearchUserByID(pUserStatus.dwUserID);
			if (pIUserItem==null) 
			{
				return true;
			}
			//定义变量
	 		var pUserData:tagUserInfoHead=pIUserItem.GetUserData();
			var wNowTableID:uint=pUserStatus.wTableID;
			var wLastTableID:uint=pUserData.UserStatus.wTableID;
			
			var wNowChairID:uint=pUserStatus.wChairID;
			var wLastChairID:uint=pUserData.UserStatus.wChairID;
			
			var cbNowStatus:uint=pUserStatus.cbUserStatus;
			var cbLastStatus:uint=pUserData.UserStatus.cbUserStatus;

			//用户离开
			if (cbNowStatus==US_NULL)
			{
				//删除用户
				m_ClientUserManager.DeleteUserItem(pIUserItem);
				
				UpdateGameServerOnLine(m_gameServer.getItemData().wServerID,
																 m_ClientUserManager.GetOnLineCount());
				
				return true;
			}
			
			//更新状态
			var UserStatus:tagUserStatus = new tagUserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			UserStatus.wNetDelay=pUserStatus.wNetDelay;
			m_ClientUserManager.UpdateUserItemStatus(pIUserItem,
												UserStatus);

			return true;
		}
		//用户分数
		protected function OnSocketSubScore(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			
			if (wDataSize<CMD_GR_UserScore.sizeof_CMD_GR_UserScore) 
			{
				return false;
			}

			//处理数据
			var pUserScore:CMD_GR_UserScore=CMD_GR_UserScore.readData(pBuffer);;
			var pIUserItem:IUserItem=
			m_ClientUserManager.SearchUserByID(pUserScore.dwUserID);
	
			if (pIUserItem==null) 
			{
				return true;
			}

			//更新分数
			m_ClientUserManager.UpdateUserItemScore(pIUserItem,pUserScore.UserScore);
			
			
			return true;
		}
		
		
	}
}
include "../../../StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../IGameFrame/ChannelMessageDef.as"
include "../../../Net/InterfaceModuleIDef.as"
include "../../../Net/NetModuleIDef.as"
include "../../../Net/GLOBALDEF.as"
include "../../../Net/GLOBALFrame.as"
include "../../../Net/CMD_Plaza.as"
include "../../../Net/CMD_Game.as"
include "../../../Common/NetWork.as"
include "../../../Common/Memory.as"
