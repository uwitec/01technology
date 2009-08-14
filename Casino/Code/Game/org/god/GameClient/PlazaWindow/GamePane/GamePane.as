package 
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.display.DisplayObjectContainer;
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	import flash.net.URLRequest;
	import flash.display.Loader;
	import flash.geom.Rectangle;
	import flash.system.System;

	import org.aswing.geom.IntRectangle;
	import org.aswing.geom.IntPoint;
	import org.aswing.util.LinkedList;
	import org.aswing.ASFont;
	import org.aswing.JLabel;
	import org.aswing.JScrollPane;
	import org.aswing.ASColor;
	import org.aswing.graphics.Graphics2D;
	
	import org.god.Common.MD5;
	import org.god.Common.ClassLoader;
	import org.god.Common.enServiceStatus;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.ServerItem;
	import org.god.Common.FlashMovieClipHelper;
	import org.god.SkinControl.SkinButton;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.IUnknownEx;
	import org.god.Net.IClientSocket;
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
	import org.god.Net.tagGameServer;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameType;	
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.tagTableStatus;
	import org.god.Net.tagUserData;
	import org.god.Net.tagUserStatus;
	import org.god.Net.tagGameServerOnLineCountInfo;
	import org.god.Net.IPC_UserStatus;
	import org.god.Net.IPC_GF_ServerInfo;
	import org.god.Net.IPC_UserScore;

	import org.god.IGameFrame.IEnterGameEventListener;
	import org.god.IGameFrame.IGamePaneImpl;
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.IPlazaWindow;
	import org.god.IGameFrame.ITableFrameSink;
	import org.god.IGameFrame.IGameClassContainer;	
	import org.god.IGameFrame.GlobalMain;
	import org.god.IGameFrame.IGameClass;
	import org.god.IGameFrame.IUserItem;
	import org.god.IGameFrame.ITableView;

	//游戏选桌区面板
	public class GamePane extends MovieClip implements IGamePaneImpl,
	IClientSocketSink,IUnknownEx,ITableFrameSink,IGameClassContainer
	{
		public static  const nGamePaneWidth:int = 1024;
		public static  const nGamePaneHeight:int = 768;

		private var m_clientSocket:ClientSocket;
		private var m_mcParent:IPlazaWindow;
		private var m_GameServerItem:ServerItem;
		private var m_GameKindItem:ServerItem;
		private var m_GameTypeItem:ServerItem;
		private var m_labelGameRoomPath:JLabel;
		private var m_labelBetLimit:JLabel;
		private var m_eventlistener:IEnterGameEventListener;
		private var m_isLogonOK:Boolean;
		private var m_ServiceStatus:uint;
		
		private var m_wReqTableID:uint;
		private var m_wReqChairID:uint;
		private var m_wFindTableID:uint;
		private var m_wFindChairID:uint;
		
		//管理变量
		private var m_pMeUserItem:IUserItem;//自己信息
		private var m_ClientUserManager:ClientUserManager;//用户管理

		private var m_TableFrameViewContainer:JScrollPane;
		private var m_TableFrameView:TableFrameView;
		private var m_btnAutoSitChair:SkinButton;
		private var m_btnExitGameRoot:SkinButton;
		private var m_FlashMovieClipHelper:FlashMovieClipHelper;
		
		private var m_loaderProgressPane:SkinLoaderPane;
		private var m_loaderGameClass:ClassLoader;
		private var m_GameClassInstance:IGameClass;

		public function GamePane(eventlistener:IEnterGameEventListener)
		{
			this.x = 0;
			this.y = 0;
			this.visible = false;
			
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;
			m_wFindTableID=INVALID_TABLE;
			m_wFindChairID=INVALID_CHAIR;

			m_ClientUserManager = new ClientUserManager;
			m_ClientUserManager.SetUserManagerSink(this);

			m_ServiceStatus= enServiceStatus.ServiceStatus_Null;
			m_eventlistener= eventlistener;
			m_clientSocket = new ClientSocket;

			m_labelGameRoomPath = new JLabel("§",null,JLabel.LEFT);
			addChild(m_labelGameRoomPath);
			m_labelGameRoomPath.x = 10;
			m_labelGameRoomPath.y = 138;
			m_labelGameRoomPath.width = 810;
			m_labelGameRoomPath.height = 34;
			var font:ASFont = new ASFont;
			font = font.changeSize(22);
			font = font.changeBold(true);
			m_labelGameRoomPath.setForeground(new ASColor(0xe9e311));
			m_labelGameRoomPath.setFont(font);
			
			m_labelBetLimit = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelBetLimit);
			m_labelBetLimit.x = 232;
			m_labelBetLimit.y = 100;
			m_labelBetLimit.width = 350;
			m_labelBetLimit.height = 30;
			m_labelBetLimit.setForeground(new ASColor(0xeeeeee));
			font = font.changeSize(16);
			m_labelBetLimit.setFont(font);
		
			m_btnAutoSitChair = new SkinButton(IDS_AUTOJIONIN,830,138);
			addChild(m_btnAutoSitChair);
			m_btnAutoSitChair.setEnabled(false);
			m_btnAutoSitChair.addEventListener(MouseEvent.CLICK, OnEventAutoSitChair);

			m_btnExitGameRoot = new SkinButton(IDS_RETURNPLAZA,920,138);
			addChild(m_btnExitGameRoot);
			m_btnExitGameRoot.addEventListener(MouseEvent.CLICK, OnExitGameRoot);

			m_TableFrameViewContainer = new JScrollPane;
			addChild(m_TableFrameViewContainer);
			m_TableFrameViewContainer.setComBounds(TableFrameView.rectDefaultPosition);
									
			m_FlashMovieClipHelper = new FlashMovieClipHelper(this, FlashStarCell);
			m_FlashMovieClipHelper.Start();
		}

		public function Create(param0:*,param1:* = null,param2:* = null,param3:* = null):int
		{
			if (param0 == null || param1 == null || param2 == null)
			{
				return -1;
			}
			m_GameServerItem = param0;
			m_GameKindItem   = param1;
			m_GameTypeItem   = param2;
			
			UpdateGameServerOnLine(m_GameServerItem.getItemData().wServerID,0);
			UpdateGameBetLimit(m_GameKindItem.getItemData().dwCellScore,
								m_GameKindItem.getItemData().dwHighScore,
								m_GameKindItem.getItemData().dwLessScore);
			
			return 0;
		}
		public function Destroy():void
		{
			if(m_FlashMovieClipHelper)
			{
				m_FlashMovieClipHelper.Stop();
				m_FlashMovieClipHelper = null;
			}
			if(m_GameClassInstance)
			{
				m_GameClassInstance.DestroyGameClient();
				m_GameClassInstance = null;
			}
			if (m_TableFrameView)
			{
				m_TableFrameView.DestroyTableFrame();
				m_TableFrameView = null;
			}
			if(m_labelGameRoomPath)
			{
				removeChild(m_labelGameRoomPath);
				m_labelGameRoomPath = null;
				
				removeChild(m_labelBetLimit);
				m_labelBetLimit = null;
				
				m_btnAutoSitChair.removeEventListener(MouseEvent.CLICK, OnEventAutoSitChair);
				removeChild(m_btnAutoSitChair);
				m_btnAutoSitChair = null;
				
				m_btnExitGameRoot.removeEventListener(MouseEvent.CLICK, OnExitGameRoot);
				removeChild(m_btnExitGameRoot);
				m_btnExitGameRoot = null;
			}
			
			if (m_TableFrameViewContainer)
			{
				removeChild(m_TableFrameViewContainer);
				m_TableFrameViewContainer = null;
			}
			if (m_clientSocket)
			{
				m_clientSocket.CloseSocket(false);
				m_clientSocket = null;
			}
			if (m_labelGameRoomPath)
			{
				removeChild(m_labelGameRoomPath);
				m_labelGameRoomPath = null;
			}
			if (m_ClientUserManager)
			{
				m_ClientUserManager.Destroy();
				m_ClientUserManager = null;
			}
			if(m_loaderGameClass)
			{
				m_loaderGameClass.Destroy();
				m_loaderGameClass = null;
			}
			m_GameServerItem = null;
			m_GameKindItem   = null;
			m_GameTypeItem   = null;
			m_eventlistener  = null;
		}
		public function getPaneType():int
		{
			return nPaneType_Game;
		}
		public function getPaneName():String
		{
			return m_GameServerItem.toString();
		}
		public function showPane(mcParent:MovieClip,bShow:Boolean):void
		{
			if (bShow && mcParent != null)
			{
				m_mcParent = mcParent as IPlazaWindow;
				m_mcParent.addChildDO(this);
			}
			else
			{
				if (m_mcParent != null)
				{
					m_mcParent.removeChildDO(this);
					m_mcParent = null;
				}
			}
		}
		public function Active(data:*):int
		{
			if(m_FlashMovieClipHelper)
				m_FlashMovieClipHelper.Resume();
			this.visible = true;
			return 0;
		}
		public function Deactive():int
		{
			if(m_FlashMovieClipHelper)
				m_FlashMovieClipHelper.Pause();
			this.visible = false;
			return 0;
		}
		public function Flash():void
		{
		}
		public function EqualPane(param:*):Boolean
		{
			if (param == null)
			{
				return false;
			}
			else
			{
				var item:ServerItem = param;
				if(item.getItemData().wKindID == m_GameKindItem.getItemData().wKindID)
					return true;
				else
					return false;
			}
		}
		public function getPaneParam():*
		{
			return m_GameServerItem;
		}
		public function getGameKind():*
		{
			return m_GameKindItem.getItemData();
		}
		public function getActiveDOC():DisplayObjectContainer
		{
			return this;
		}
		///////////////////////////
		public function getClientSocket():ClientSocket
		{
			return m_clientSocket;
		}
		public function DoEnterGameWork():void
		{
			getClientSocket().SetSocketSink(this);
			var server:tagGameServer  = m_GameServerItem.getItemData();
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
			var main:IMain = GlobalMain.getMain();
			if (main)
			{
				main.ShowStatusMessage(strMsg);
			}
		}
		public function HideStatusMessage():void
		{
			var main:IMain = GlobalMain.getMain();
			if (main)
			{
				main.HideStatusMessage();
			}
		}
		public function ShowMessageBox(strMsg:String):void
		{
			var main:IMain = GlobalMain.getMain();
			if (main)
			{
				main.ShowMessageBox(strMsg);
			}
		}
		public function ShowErrorMessageBox(wErrorCode:uint):void
		{
			var main:IMain = GlobalMain.getMain();
			if (main)
			{
				main.ShowErrorMessageBox(wErrorCode);
			}
		}
		//鼠标左键按下
		public function OnLButtonHitTable(wTableID:uint, wChairID:uint, bMirror:Boolean):void
		{
		}
		//鼠标右键按下
		public function OnRButtonHitTable(wTableID:uint, wChairID:uint, bMirror:Boolean):void
		{
		}
		//鼠标双击
		public function OnDoubleHitTable(wTableID:uint, bMirror:Boolean):void
		{
		}
		//坐下
		public function SitChair(wTableID:uint,  wChairID:uint):void
		{
			
			//判断状态
			if (m_ServiceStatus!=
				enServiceStatus.ServiceStatus_Serviceing) 
			{
				return;
			}
			if ((m_wReqTableID!=INVALID_TABLE)||
				(m_wReqChairID!=INVALID_CHAIR))
			{
				ShowMessageBox(IDS_ERR_SERVERREQBUZY);
				return;
			}
			if(m_pMeUserItem == null)
			{
				return;
			}
			//判断状态
			var pUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
			if (pUserData.UserStatus.cbUserStatus==US_PLAY)
			{
				ShowMessageBox(IDS_ERR_YOUALREADYPLAY);
				return;
			}
			m_wFindChairID = wTableID;
			PerformSitDownAction(wTableID,wChairID);

		}
		////////////////////////////////////////////
		//IGameClassContainer接口
		/////////////////////////////////////////////
		public function DestroyGameClass(game:IGameClass, bAutoSitChairAgain:Boolean = false):void
		{
			if(m_GameClassInstance == game)
			{
				CloseGameClient();
				//再随机坐下
				if(bAutoSitChairAgain)
					OnEventAutoSitChair(null);
			}
		}
		public function ActiveGameClass(game:IGameClass):void
		{
			if(m_GameClassInstance == game &&
			   m_mcParent != null)
			{
				m_mcParent.activePane(this);
			}
		}
		////////////////////////////////////////////
		private function OnEventAutoSitChair(e:MouseEvent):void
		{
			if(m_loaderGameClass == null)
			{
				StartLoadGameModule();
			}
			else
			{
				AutoSitChair();
			}
		}
		private function OnExitGameRoot(e:MouseEvent):void
		{
			if(m_mcParent)
			{
				m_mcParent.exitGamePane(this);
			}
		}
		public function AutoSitChair():void
		{
			//判断状态
			if (m_ServiceStatus!=
				enServiceStatus.ServiceStatus_Serviceing) 
			{
				return;
			}
			if ((m_wReqTableID!=INVALID_TABLE)||
				(m_wReqChairID!=INVALID_CHAIR))
			{
				ShowMessageBox(IDS_ERR_SERVERREQBUZY);
				return;
			}
			if(m_pMeUserItem == null)
			{
				return;
			}
			//判断状态
			var pUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
			if (pUserData.UserStatus.cbUserStatus==US_PLAY)
			{
				ShowMessageBox(IDS_ERR_YOUALREADYPLAY);
				return;
			}
			m_wFindChairID = AUTO_TABLEID;
			PerformSitDownAction(AUTO_TABLEID,AUTO_CHAIRID);
			return;
			
			//先搜索桌子不全空的
			var FindInfo:tagFindTable = new tagFindTable;
			FindInfo.bAllNull=false;
			FindInfo.bFilterPass=true;
			FindInfo.bNotFull=true;
			FindInfo.bOneNull=true;
			FindInfo.bTwoNull=(m_TableFrameView.GetChairCount()!=2);
			FindInfo.wBeginTableID=m_wFindTableID+1;
			FindInfo.wResultTableID=INVALID_TABLE;
			FindInfo.wResultChairID=INVALID_CHAIR;
			var bSuccess:Boolean=FindGameTable(FindInfo);
			m_wFindTableID=FindInfo.wResultTableID;
			
			//搜索全部游戏桌
			if (bSuccess==false)
			{
				FindInfo.bAllNull=true;
				FindInfo.bFilterPass=true;
				FindInfo.bNotFull=true;
				FindInfo.bOneNull=true;
				FindInfo.bTwoNull=true;
				FindInfo.wBeginTableID=m_wFindTableID+1;
				FindInfo.wResultTableID=INVALID_TABLE;
				FindInfo.wResultChairID=INVALID_CHAIR;
				bSuccess=FindGameTable(FindInfo);
				m_wFindTableID=FindInfo.wResultTableID;
			}
			//结果判断
			if (bSuccess==true)
			{
				//效验数据
				//设置数据
				var wChairID:uint=FindInfo.wResultChairID;
				m_TableFrameView.EnsureVisibleTable(m_wFindTableID);
				m_TableFrameView.FlashChair(m_wFindTableID,wChairID);
				
				m_wFindChairID = wChairID;
				//自动坐下
				PerformSitDownAction(m_wFindTableID,wChairID);
			}
			else
			{
				ShowMessageBox(IDS_ERR_CANFINDTABLE);
			}	
		}
		//坐下动作
		public function PerformSitDownAction(wTableID:uint,  wChairID:uint):Boolean
		{
			//动作效验
			if (CanSitDownTable(wTableID,wChairID,false,true)==false) return false;

			//清理界面
			if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR) && 
				(m_wReqTableID!=AUTO_TABLEID)&&(m_wReqChairID!=AUTO_CHAIRID))
			{
				var pIUserItem:IUserItem=m_TableFrameView.GetUserInfo(m_wReqTableID,m_wReqChairID);
				if (pIUserItem==m_pMeUserItem)
					m_TableFrameView.SetUserInfo(m_wReqTableID,m_wReqChairID,null);
			}

			//动作处理
			m_wReqTableID=wTableID;
			m_wReqChairID=wChairID;
			m_wFindTableID=INVALID_TABLE;
			m_wFindChairID=INVALID_CHAIR;
			m_TableFrameView.EnsureVisibleTable(wTableID);
			m_TableFrameView.SetUserInfo(wTableID,wChairID,m_pMeUserItem);
			SendSitDownPacket(wTableID,wChairID,"");

			return true;
		}
		//查找桌子
		private function FindGameTable(FindInfo:tagFindTable):Boolean
		{
			//变量定义
			var wNullCount:uint=0;
			var pTableView:ITableView=null;
			var wChairUser:uint=m_TableFrameView.GetChairCount();
			var wMeTableID:uint=m_pMeUserItem.GetUserData().UserStatus.wTableID;

			//搜索桌子
			for (var i:uint=0;i<m_TableFrameView.GetTableCount();i++)
			{
				FindInfo.wResultTableID=(FindInfo.wBeginTableID+i)%m_TableFrameView.GetTableCount();
				if (wMeTableID!=FindInfo.wResultTableID)
				{
					//获取桌子
					pTableView=m_TableFrameView.GetTableArrayPtr(FindInfo.wResultTableID);
			

					//判断是否开始游戏
					if (pTableView.QueryPlayFlag()==true) continue;

					//过滤密码
					var bTablePass:Boolean=pTableView.QueryPassFlag();
					if ((FindInfo.bFilterPass==true)&&(bTablePass==true)) continue;

					//寻找空位置
					var r:IntPoint=pTableView.GetNullChairCount(FindInfo.wResultChairID);
					wNullCount = r.x;
					FindInfo.wResultChairID = r.y;
					
					//效验规则
					if (CanSitDownTable(FindInfo.wResultTableID,FindInfo.wResultChairID,false,false)==false) continue;

					//判断数目
					if (wNullCount>0)
					{
						if ((FindInfo.bNotFull==true)&&(wNullCount<wChairUser)) return true;
						if ((FindInfo.bOneNull==true)&&(wNullCount==1)) return true;
						if ((FindInfo.bTwoNull==true)&&(wNullCount==2)) return true;
						if ((FindInfo.bAllNull==true)&&(wNullCount==wChairUser)) return true;
					}
				}
			}

			//设置数据
			FindInfo.wResultTableID=INVALID_TABLE;
			FindInfo.wResultChairID=INVALID_CHAIR;

			return false;
		}
		//能否坐下
		private function CanSitDownTable(wTableID:uint, wChairID:uint, bLookon:Boolean, bShowError:Boolean):Boolean
		{
			//效验参数
			if (wTableID != AUTO_TABLEID && 
				wTableID>=m_TableFrameView.GetTableCount()) 
					return false;
			if (wChairID != AUTO_CHAIRID &&
				wChairID>=m_TableFrameView.GetChairCount())
					return false;
			if (m_ServiceStatus!=enServiceStatus.ServiceStatus_Serviceing) return false;

			//判断响应
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR))
			{
				if (bShowError==true)
				{
					ShowMessageBox(IDS_ERR_SERVERREQBUZY);
					return false;
				}
			}

			//判断状态
			var cbUserStatus:uint=m_pMeUserItem.GetUserData().UserStatus.cbUserStatus;
			if (cbUserStatus==US_PLAY)
			{
				if (bShowError==true)
				{
					ShowMessageBox(IDS_ERR_YOUALREADYPLAY);
				}
				return false;
			}
			
			//判断玩家
			if(wTableID != AUTO_TABLEID && wChairID != AUTO_CHAIRID)
			{
				var pITableView:ITableView=m_TableFrameView.GetTableArrayPtr(wTableID);
				var pITableUserItem:IUserItem=pITableView.GetUserInfo(wChairID);
				if ((bLookon==true)&&(pITableUserItem==null)) return false;
				if ((bLookon==false)&&(pITableUserItem!=null)) return false;
				
				//判断游戏状态
				var bGameStart:Boolean=pITableView.QueryPlayFlag();
				if ((bGameStart==true)&&(bLookon==false))
				{
					if (bShowError==true)
					{
						ShowMessageBox(IDS_ERR_TABLEPLAYING);
					}
					return false;
				}
				if ((bGameStart==false)&&(bLookon==true)) return false;

				//判断密码
				if ((pITableView.QueryPassFlag()==true))
				{
					if (bShowError==true)
					{
						ShowMessageBox(IDS_ERR_TABLESETPASSWD);
					}
					return false;
				}

				//旁观判断
				if (bLookon==true) return true;
			}

			return true;
		}
		/////////////////////////////////////////
		//是否有效
		public function IsValid():Boolean
		{
			return true;
		}
		//释放对象
		public function Release():Boolean
		{
			return true;
		}
		//接口查询
		public function QueryInterface(Guid:String,dwQueryVer:uint):*
		{
			if (Guid == IID_IUnknownEx)
			{
				return this;
			}
			else if (Guid == IID_IClientSocketSink)
			{
				return this;
			}
			return null;
		}
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
					m_eventlistener.CancelEnterGame(this);
				}
				this.ShowStatusMessage(IDS_STATUS_CHECKUSERINFO);
			}
			else
			{
				this.HideStatusMessage();
				this.ShowMessageBox(IDS_ERR_SERVERBUZY);
				m_eventlistener.CancelEnterGame(this);
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

			return true;
		}
		//网络关闭消息
		public function OnSocketClose(pIClientSocket:IClientSocket,bCloseByServer:Boolean):Boolean
		{
			CloseGameClient();
			
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
			if (m_eventlistener)
			{
				m_eventlistener.CancelEnterGame(this);
				m_eventlistener = null;
			}
			else
			{
				if(m_mcParent)
					m_mcParent.activePane(this);
			}
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

						if (m_eventlistener)
						{
							m_eventlistener.CancelEnterGame(this);
							m_eventlistener = null;
						}
						return true;


					};
				case SUB_GR_LOGON_FINISH ://登录完成
					{
						m_ServiceStatus=enServiceStatus.ServiceStatus_Serviceing;
						if (m_eventlistener)
						{
							m_eventlistener.FinishEnterGame(this);
							m_eventlistener = null;
						}
						m_btnAutoSitChair.setEnabled(true);
						
						if(m_pMeUserItem)
						{
							var pUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
							if (pUserData &&
								pUserData.UserStatus.wTableID!=INVALID_TABLE)
							{
								if(m_loaderGameClass != null)
								{
									var iResult:int=StartGameClient();
									if(iResult != 0)
									{
										this.HideStatusMessage();
										this.ShowMessageBox(IDS_ERR_UNKNOWN);
									}
								}
								else
								{
									StartLoadGameModule();
								}
							}							
						}
						return true;
				}
			};
			return true;
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
				case SUB_GR_SIT_FAILED ://坐下失败
					{
						return OnSocketSubSitFailed(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);




					};


				case SUB_GR_USER_INVITE ://邀请玩家
					{
						return OnSocketSubUserInvite(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);




				}
			};
			return true;
		}
		//配置消息
		protected function OnSocketMainInfo(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_SERVER_INFO ://房间信息
					{
						if (m_TableFrameView == null)
						{
							var ServerInfo:CMD_GR_ServerInfo = CMD_GR_ServerInfo.readData(pBuffer);

							m_TableFrameView = new TableFrameView(this);
							if (m_TableFrameView.InitTableFrame(ServerInfo.wTableCount,
							   ServerInfo.wChairCount,
							   m_GameServerItem,
							   m_GameKindItem,
							   this) != true)
							{
								m_TableFrameView.DestroyTableFrame();
								m_TableFrameView = null;

								m_ServiceStatus= enServiceStatus.ServiceStatus_NetShutDown;
								this.HideStatusMessage();
								this.ShowMessageBox(IDS_ERR_UNKNOWN);

								if (m_eventlistener)
								{
									m_eventlistener.CancelEnterGame(this);
									m_eventlistener = null;
								}
							}
							else
							{
								m_TableFrameViewContainer.setView(m_TableFrameView);

							}
						}
						return true;





					};
				case SUB_GR_CONFIG_FINISH ://配置完成
					{
						m_ServiceStatus=enServiceStatus.ServiceStatus_RecvRoomInfo;
						return true;





				}
			};
			return true;
		}
		//状态消息
		protected function OnSocketMainStatus(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_TABLE_INFO:		//桌子信息
				{
					//变量定义
					var  pTableInfo:CMD_GR_TableInfo=CMD_GR_TableInfo.readData(pBuffer);
					var wHeadSize:uint=CMD_GR_TableInfo.sizeof_CMD_GR_TableInfo
										-CMD_GR_TableInfo.sizeof_TableStatus;

					if (wDataSize<wHeadSize)
					{
						return false;
					}
					if ((wHeadSize+pTableInfo.wTableCount*tagTableStatus.sizeof_tagTableStatus)!=wDataSize)
					{
						return false;
					}

					//消息处理
					for (var n:uint=0;n<pTableInfo.wTableCount;n++)
					{
						m_TableFrameView.SetPassFlag(n,pTableInfo.TableStatus[n].bTableLock?true:false);
						m_TableFrameView.SetPlayFlag(n,pTableInfo.TableStatus[n].bPlayStatus?true:false);
					}

					return true;
				}
				case SUB_GR_TABLE_STATUS:	//桌子状态
				{
					//效验数据
					if (wDataSize<CMD_GR_TableStatus.sizeof_CMD_GR_TableStatus) return false;

					//消息处理
					var pTableStatus:CMD_GR_TableStatus=CMD_GR_TableStatus.readData(pBuffer);
			
					if (pTableStatus.wTableID<m_TableFrameView.GetTableCount())
					{
						//设置用户
						var pIUserItem:IUserItem=null;
						var pUserData:tagUserInfoHead=null;
						var cbUserStatus:uint=pTableStatus.bPlayStatus?US_PLAY:US_SIT;
						for (var i:uint=0;i<m_TableFrameView.GetChairCount();i++)
						{
							pIUserItem=m_TableFrameView.GetUserInfo(pTableStatus.wTableID,i);
							if (pIUserItem!=null)
							{
								pUserData=pIUserItem.GetUserData();
								if (pUserData.UserStatus.cbUserStatus!=US_OFFLINE) 
								{
									pUserData.UserStatus.cbUserStatus=cbUserStatus;
								
								}
							}
						}

						//设置桌子
						m_TableFrameView.SetPlayFlag(pTableStatus.wTableID,pTableStatus.bPlayStatus?true:false);

						//通知游戏
						if(m_pMeUserItem)
						{
							var pMeUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
							if (pMeUserData.UserStatus.wTableID==pTableStatus.wTableID)
							{
								var wSubCmdID:uint=pTableStatus.bPlayStatus?IPC_SUB_GAME_START:IPC_SUB_GAME_FINISH;
								SendGameClientProcessData(IPC_MAIN_USER,wSubCmdID,null,0);
							}							
						}					
				 	 }
				  	return true;
			 	 }
			}
			return true;
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
					if(wTypeMark0 != 0 ||
						wTypeMark1 != 0)
					{
						DestroyGameClass(m_GameClassInstance);
						m_mcParent.exitGamePane(this);
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
			if(SendGameClientSocketData(wMainCmdID,wSubCmdID,pBuffer,wDataSize)== false)
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
						if(wTypeMark0 != 0 ||
							wTypeMark1 != 0)
						{
							DestroyGameClass(m_GameClassInstance);
							m_mcParent.exitGamePane(this);
						}
						
						return true;
					}
				}
				return false;
			}
			else
			return true;
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
				//判断自己
				if (m_pMeUserItem==null)
				{
					var main:IMain = GlobalMain.getMain();
					if (main)
					{
						var mydata:tagUserData = main.getUserData();
						if(mydata && mydata.dwUserID == pIUserItem.GetUserID())
						{
							m_pMeUserItem=pIUserItem;
						}
					}
					
				}
				//设置界面
				var cbUserStatus:uint=pUserInfoHead.UserStatus.cbUserStatus;
				if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
				{
					m_TableFrameView.SetUserInfo(pUserInfoHead.UserStatus.wTableID,
											pUserInfoHead.UserStatus.wChairID,
											pIUserItem);
				}
			}
			

			UpdateGameServerOnLine(m_GameServerItem.getItemData().wServerID,
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
			if (pIUserItem==null || m_pMeUserItem == null) 
			{
				return true;
			}
			//定义变量
	 		var pUserData:tagUserInfoHead=pIUserItem.GetUserData();
			var pMeUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
			
			var wNowTableID:uint=pUserStatus.wTableID;
			var wLastTableID:uint=pUserData.UserStatus.wTableID;
			
			var wNowChairID:uint=pUserStatus.wChairID;
			var wLastChairID:uint=pUserData.UserStatus.wChairID;
			
			var cbNowStatus:uint=pUserStatus.cbUserStatus;
			var cbLastStatus:uint=pUserData.UserStatus.cbUserStatus;
			
			//清理旧状态
			if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
			{
				var pITableUserItem:IUserItem=m_TableFrameView.GetUserInfo(wLastTableID,wLastChairID);
				if (pITableUserItem==pIUserItem) m_TableFrameView.SetUserInfo(wLastTableID,wLastChairID,null);
			}
			//用户离开
			if (cbNowStatus==US_NULL)
			{
				//通知游戏
				if ((pMeUserData.UserStatus.wTableID!=INVALID_TABLE)&&
					(pMeUserData.UserStatus.wTableID==wLastTableID))
				{
					var ipc_UserStatus:IPC_UserStatus=new IPC_UserStatus;
					
					ipc_UserStatus.dwUserID=pUserData.dwUserID;
					ipc_UserStatus.wNetDelay=pUserData.UserStatus.wNetDelay;
					ipc_UserStatus.cbUserStatus=pUserData.UserStatus.cbUserStatus;
					SendGameClientProcessData(IPC_MAIN_USER,
											  IPC_SUB_USER_STATUS,
											  ipc_UserStatus,
											  IPC_UserStatus.sizeof_IPC_UserStatus);
				}

				//删除用户
				m_ClientUserManager.DeleteUserItem(pIUserItem);

				
				UpdateGameServerOnLine(m_GameServerItem.getItemData().wServerID,
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

			//设置新状态
			if ((wNowTableID!=INVALID_TABLE)&&
				((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
			{
				//设置桌子
				if (cbNowStatus!=US_LOOKON)
				{
					m_TableFrameView.SetUserInfo(wNowTableID,
												wNowChairID,
												pIUserItem);
				}

				//发送用户
				if ((m_pMeUserItem!=pIUserItem)&&
				 (pMeUserData.UserStatus.wTableID==wNowTableID))
				{
					SendTableUser(pIUserItem);
				}
			}
			//更新界面
			if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)
			&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
			{
				m_TableFrameView.UpdateTableView(wNowTableID,false);
			}

			//判断发送
			var bNotifyGame:Boolean=false;
			if (pIUserItem==m_pMeUserItem) 
			{
				bNotifyGame=true;
			}
			else if ((pMeUserData.UserStatus.wTableID!=INVALID_TABLE)&&
						(pMeUserData.UserStatus.wTableID==wNowTableID))
			{
				bNotifyGame=true;
			}
			else if ((pMeUserData.UserStatus.wTableID!=INVALID_TABLE)&&
			(pMeUserData.UserStatus.wTableID==wLastTableID))
			{
				bNotifyGame=true;
			}

			//发送状态
			if (bNotifyGame==true)
			{
				var ipc_GameUserStatus:IPC_UserStatus = new IPC_UserStatus;
				
				ipc_GameUserStatus.dwUserID=pUserData.dwUserID;
				ipc_GameUserStatus.wNetDelay=pUserData.UserStatus.wNetDelay;
				ipc_GameUserStatus.cbUserStatus=pUserData.UserStatus.cbUserStatus;
				SendGameClientProcessData(IPC_MAIN_USER,
								IPC_SUB_USER_STATUS,
								ipc_GameUserStatus,
								IPC_UserStatus.sizeof_IPC_UserStatus);
			}

			//判断自己
			if (pIUserItem==m_pMeUserItem)
			{
				//设置变量
				if ((wNowTableID==m_wReqTableID&&
					wNowChairID==m_wReqChairID) ||
					(m_wReqTableID == AUTO_TABLEID && 
					 m_wReqChairID == AUTO_CHAIRID))
				{
					m_wReqTableID=INVALID_TABLE;
					m_wReqChairID=INVALID_CHAIR;
				}

				//启动游戏
				if ((wNowTableID!=INVALID_TABLE)&&
				((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
				{
					if(m_loaderGameClass != null)
					{
						var iResult:int=StartGameClient();
						if(iResult != 0)
						{
							this.HideStatusMessage();
							this.ShowMessageBox(IDS_ERR_UNKNOWN);
						}
					}
					else
					{
						StartLoadGameModule();
					}
				}
			}
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

			//更新游戏
			var pUserData:tagUserInfoHead=pIUserItem.GetUserData();
			var pMeUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
			if ((pMeUserData.UserStatus.wTableID!=INVALID_TABLE)
				&&(pMeUserData.UserStatus.wTableID==pUserData.UserStatus.wTableID))
			{
				var UserScore:IPC_UserScore = new IPC_UserScore;
				UserScore.dwUserID=pUserScore.dwUserID;
				UserScore.UserScore=pUserScore.UserScore;
				SendGameClientProcessData(IPC_MAIN_USER,
								IPC_SUB_USER_SCORE,
								UserScore,
								IPC_UserScore.sizeof_IPC_UserScore);
			}
			if(pMeUserData.dwUserID == pUserData.dwUserID)
			{
				var main:IMain = GlobalMain.getMain();
				if(main)
				{
					var MainUserData:tagUserData = main.getUserData();
					MainUserData.UserScore.fScore = pUserScore.UserScore.fScore;
					main.setUserData(MainUserData);
				}
			}
			return true;
		}
		//坐下失败
		protected function OnSocketSubSitFailed(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			//消息处理
			var pSitFailed:CMD_GR_SitFailed=CMD_GR_SitFailed.readData(pBuffer);
			
			if((m_wReqTableID!=AUTO_TABLEID)&&(m_wReqChairID!=AUTO_CHAIRID))
			{
				var pISendUserItem:IUserItem=m_TableFrameView.GetUserInfo(m_wReqTableID,
																	 m_wReqChairID);
				if (pISendUserItem==m_pMeUserItem)
				{
					m_TableFrameView.SetUserInfo(m_wReqTableID,
									  m_wReqChairID,null);
				}
			}

			//设置变量
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;
			m_wFindTableID=INVALID_TABLE;
			m_wFindChairID=INVALID_CHAIR;
		
			//显示消息
			this.ShowErrorMessageBox(pSitFailed.wErrorCode);
	
			return true;
		}
		//邀请玩家
		protected function OnSocketSubUserInvite(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			return true;
		}
		//////////////////////////////
		private function StartGameClient():int
		{			
			if(m_loaderGameClass && m_GameClassInstance == null)
			{
				try
				{
					m_GameClassInstance =m_loaderGameClass.newDefaultIGameClass();
					if(m_GameClassInstance)
					{
						var gameMC:MovieClip = m_GameClassInstance.GetMovieClip();
						addChild(gameMC);
						if(m_FlashMovieClipHelper)
							m_FlashMovieClipHelper.Pause();
						
						if(m_GameClassInstance.CreateGameClient(this))
						{
							return 0;
						}
						else
						{
							if(m_FlashMovieClipHelper)
								m_FlashMovieClipHelper.Resume();
							removeChild(gameMC);
						}
					}
					m_GameClassInstance = null;
				}
				catch(e:Error)
				{
					trace(e);
					trace(e.getStackTrace());
					System.setClipboard(e.getStackTrace());
				}
			}   
			return -1;
		}
		private function CloseGameClient():int
		{
			if(m_GameClassInstance)
			{
				SendGameClientProcessData(IPC_MAIN_CONCTROL,IPC_SUB_CLOSE_FRAME,null,0);
				m_GameClassInstance.DestroyGameClient();
				var game:MovieClip = m_GameClassInstance.GetMovieClip();
				removeChild(game);
				m_GameClassInstance = null;
				if(m_FlashMovieClipHelper)
					m_FlashMovieClipHelper.Resume();
			}
			return 0;
		}
		private function SendGameClientProcessData(wMainCmdID:uint, 
												   wSubCmdID:uint, 
												   pBuffer:* = null, 
												   wDataSize:uint = 0):Boolean
		{
			if(m_GameClassInstance == null)
			{
				return false;
			}
			return m_GameClassInstance.OnIPCChannelMessage(wMainCmdID,wSubCmdID,pBuffer,wDataSize);
		}
		private function SendGameClientSocketData(wMainCmdID:uint, 
												   wSubCmdID:uint, 
												   pBuffer:ByteArray, 
												   wDataSize:uint):Boolean
		{
			if(m_GameClassInstance == null)
			{
				return true;
			}
			return m_GameClassInstance.OnIPCSocketMessage(wMainCmdID,wSubCmdID,pBuffer,wDataSize);
		}		
		//信道数据处理
		public function OnIPCChannelMessage(wMainCmdID:uint,
								  wSubCmdID:uint,
								  pIPCBuffer:*, 
								  wDataSize:uint):Boolean
		{
			if(wMainCmdID!=IPC_MIAN_IPC_KERNEL)
			{
				return false;
			}
			switch (wSubCmdID)
			{
				case IPC_SUB_IPC_CLIENT_CONNECT:	//连接消息
				{
					if(IPCSendGameInfo())
					{
						if(IPCSendTableUsers())
						{				
							SendGameClientProcessData(IPC_MAIN_CONCTROL,IPC_SUB_START_FINISH);
							return true;
						}
					}
					return false;
				}
				case IPC_SUB_IPC_CLIENT_CLOSE:		//关闭消息
				{
						//发送消息
					var pUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
					if (pUserData.UserStatus.wTableID!=(INVALID_TABLE))
					{
						if (pUserData.UserStatus.cbUserStatus==US_PLAY) 
						{
							SendLeftGamePacket();
						}
						SendStandUpPacket();
					}
					return true;
				}
			}
			return true;
		}
		public function OnIPCSocketMessage(wMainCmdID:uint,
								wSubCmdID:uint,
								pBuffer:ByteArray,
								wDataSize:int):Boolean
		{
			if(m_clientSocket)
			{
				return m_clientSocket.SendData(wMainCmdID,wSubCmdID,pBuffer,wDataSize);
			}
			else
			return true;
		}
		private function SendTableUser(pIUserItem:IUserItem):Boolean
		{
			var pUserInfoHead:tagUserInfoHead=pIUserItem.GetUserData();
			return SendGameClientProcessData(IPC_MAIN_USER,
											 IPC_SUB_USER_COME,
											 pUserInfoHead,
											 tagUserInfoHead.sizeof_tagUserInfoHead);
		}
		private function IPCSendGameInfo():Boolean
		{
			if(m_pMeUserItem == null)
			{
				return true;
			}
				//定义变量
			var  pUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();

			var pGameKind:tagGameKind = m_GameKindItem.getItemData();
			var pGameServer:tagGameServer = m_GameServerItem.getItemData();

			//构造数据
			var ServerInfo:IPC_GF_ServerInfo = new IPC_GF_ServerInfo;
			
			ServerInfo.dwUserID=pUserData.dwUserID;
			ServerInfo.wTableID=pUserData.UserStatus.wTableID;
			ServerInfo.wChairID=pUserData.UserStatus.wChairID;
			
			ServerInfo.wKindID=pGameKind.wKindID;
			ServerInfo.wServerID=pGameServer.wServerID;
			ServerInfo.wGameGenre=0;
			
			ServerInfo.wChairCount= m_TableFrameView.GetChairCount();
			ServerInfo.szKindName = String(m_GameKindItem.toString());
			ServerInfo.szServerName = String(m_GameServerItem.toString());

			ServerInfo.fCellScore = m_GameKindItem.getItemData().dwCellScore;
			ServerInfo.fHighScore = m_GameKindItem.getItemData().dwHighScore;
			ServerInfo.fLessScore = m_GameKindItem.getItemData().dwLessScore;
			
			//发送数据
			return SendGameClientProcessData(IPC_MAIN_CONFIG,
											 IPC_SUB_SERVER_INFO,
											 ServerInfo,
											 IPC_GF_ServerInfo.sizeof_IPC_GF_ServerInfo);

		}
		private function IPCSendTableUsers():Boolean
		{
			if(m_pMeUserItem == null)
			{
				return true;
			}
			var  pMeUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
				
			if (pMeUserData.UserStatus.wTableID==INVALID_TABLE)
			{
				return false;
			}
			SendTableUser(m_pMeUserItem);

			//发送其他用户
			var pIUserItem:IUserItem=null;
			var pUserData:tagUserInfoHead=null;

			//游戏用户
			var wEnumIndex:uint=0;
			while (true)
			{
				pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
				if (pIUserItem==null) break;
				if (pIUserItem==m_pMeUserItem) continue;
				pUserData=pIUserItem.GetUserData();
				if ((pUserData.UserStatus.wTableID==pMeUserData.UserStatus.wTableID)&&
					(pUserData.UserStatus.cbUserStatus!=US_LOOKON))
				{
					SendTableUser(pIUserItem);
				}
			}

			//旁观用户
			wEnumIndex=0;
			while (true)
			{
				pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
				if (pIUserItem==null) break;
				if (pIUserItem==m_pMeUserItem) continue;
				pUserData=pIUserItem.GetUserData();
				if ((pUserData.UserStatus.wTableID==pMeUserData.UserStatus.wTableID)&&
				(pUserData.UserStatus.cbUserStatus==US_LOOKON))
				{
						SendTableUser(pIUserItem);
				}
			}

			return true;
		}
		///////////////////////////////////////////////////////////////////////////////////
		private function StartLoadGameModule():void
		{
			LoadGameModule(GameModuleNameFactory.GetGameModuleName(m_GameKindItem.getItemData().wProcessType));
		}
		private function LoadGameModule(url:String):void
		{
			if (m_loaderGameClass == null && url.length)
			{
				if(m_loaderProgressPane == null)
				{
					m_loaderProgressPane = new SkinLoaderPane(IDS_GAME);
					addChild(m_loaderProgressPane);
				}
		
				m_loaderGameClass = new ClassLoader();
				m_loaderGameClass.addEventListener(ClassLoader.LOAD_ERROR,
												   loadErrorHandler);
            	m_loaderGameClass.addEventListener(ClassLoader.CLASS_LOADED,
												   classLoadedHandler);
            	m_loaderGameClass.addEventListener(ClassLoader.LOAD_PROGRESS,
												   progressHandler);           
				try
				{
					m_loaderGameClass.load(url);
				}
				catch(e:Error)
				{
					trace(e);
					trace(e.getStackTrace());
					System.setClipboard(e.getStackTrace());
					this.HideStatusMessage();
					this.ShowMessageBox(IDS_ERR_UNKNOWN);
				}
			}
		}
		
		private function loadErrorHandler(e:Event):void 
		{
			if(m_loaderProgressPane)
			{
				m_loaderProgressPane.visible = false;
				removeChild(m_loaderProgressPane);
				m_loaderProgressPane = null;
			}
 			this.HideStatusMessage();
			this.ShowMessageBox(IDS_ERR_UNKNOWN);
       }

        private function classLoadedHandler(e:Event):void
		{
         	if(m_loaderProgressPane)
			{
				m_loaderProgressPane.visible = false;
				removeChild(m_loaderProgressPane);
				m_loaderProgressPane = null;
			}
			var bStartFlag:Boolean = false;
			if(m_pMeUserItem)
			{
				var pUserData:tagUserInfoHead=m_pMeUserItem.GetUserData();
				if(pUserData &&
				   pUserData.UserStatus.wTableID!=INVALID_TABLE)
				{
					bStartFlag = true;
					var iResult:int=StartGameClient();
					if(iResult != 0)
					{
						this.HideStatusMessage();
						this.ShowMessageBox(IDS_ERR_UNKNOWN);
					}
				}
			}
			if(bStartFlag == false)
			{
				AutoSitChair();
			}
        }

		private function progressHandler(event:ProgressEvent):void
		{
			if(m_loaderProgressPane != null)
			{
				var nPos:int = event.bytesLoaded * 100 / event.bytesTotal;
				m_loaderProgressPane.setPos(nPos);
			}
		}
		/////////////////////////////////////////////////////////////////////////
		private function UpdateGameBetLimit(dwCellScore:uint,
		dwHighScore:uint,
		dwLessScore:uint):void
		{
			var pGameKind:tagGameKind = m_GameKindItem.getItemData();
			if (IsVideoGameProcessType(pGameKind.wProcessType) == false)
			{
				var strBetLimit:String = IDS_GAME + IDS_LESSCORE + ":" + dwLessScore + " " 
				+ IDS_HIGHSCORE + ":" + dwHighScore + " " 
				+ IDS_CELLSCORE + ":" + dwCellScore;
				m_labelBetLimit.setText(strBetLimit);
			}
		}
		private function UpdateGameOnLineCount(dwOnLineCount:uint):void
		{
			
		}
		private function UpdateGameServerOnLine(wServerID:uint,dwOnLineCount:uint):void
		{
			var pGameServer:tagGameServer = m_GameServerItem.getItemData();
			if(pGameServer.wServerID == wServerID)
			{
				var path:String = "§ " + m_GameTypeItem.toString() + " -- " + m_GameKindItem.toString() + " -- " + m_GameServerItem.toString();
				var strOnLineCount:String = " (" + IDS_ONLINECOUNT +": " + dwOnLineCount + " )";
				
				m_labelGameRoomPath.setText(path + strOnLineCount);
			}
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
include "../ConstantDef.as"