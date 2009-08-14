package 
{
	import fl.controls.Label;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import fl.managers.FocusManager;
	import flash.display.DisplayObjectContainer;
	import flash.display.SimpleButton;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.aswing.Icon;
	import org.aswing.geom.IntRectangle;
	import org.aswing.JLabel;
	import org.aswing.ASColor;
	import org.aswing.ASFont;

	import org.god.IGameFrame.IPlazaWindow;
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.IPaneImpl;
	import org.god.IGameFrame.UserDataChangeEvent;
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketSink;
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagUserData;
	import org.god.Net.tagOnLineCountInfo;
	import org.god.Net.CMD_GF_Message;
	import org.god.Net.CMD_GR_Message;	
	import org.god.SkinControl.SkinScrollTextField.ScrollTextField;
	//大厅面板
	public  class PlazaPane extends MovieClip implements IPaneImpl
	{
		private var m_mcParent:PlazaWindow;//大厅窗口
		private var m_paneServerListTabbed:ServerItemTabPane;//服务项标签面板
		private var m_labelUserName:JLabel;//用户帐号
		private var m_labelUserScore:JLabel;//用户密码
		private var m_IMain:IMain;//游戏主类接口
		
		private var m_ScrollTextField:ScrollTextField;//公告滚动控件
		//创建大厅面板
		static public function CreatePlazaPane():IPaneImpl
		{
			return (new PlazaPane()) as IPaneImpl;
		}
		//构造函数
		public function PlazaPane()
		{
			this.x = 0;
			this.y = 0;
			this.visible = false;
		}
		//创建初始话
		public function Create(param0:*,param1:* = null,param2:* = null,param3:* = null):int
		{
			m_IMain = param0;
			m_IMain.addUserDataChangeEventListener(OnEventUserDataChange);
			
			m_paneServerListTabbed = new ServerItemTabPane(this);
			m_paneServerListTabbed.setComBounds(new IntRectangle(218, 148,804, 572));

			this.m_btnChart.addEventListener(MouseEvent.CLICK, OnEventChart);
			this.m_btnBetHistory.addEventListener(MouseEvent.CLICK, OnEventBetHistory);
			this.m_btnChangePasswd.addEventListener(MouseEvent.CLICK, OnEventChangePasswd);
			this.m_btnOperation.addEventListener(MouseEvent.CLICK, OnEventOperation);
			this.m_btnLogout.addEventListener(MouseEvent.CLICK, OnEventLogout);

			var MyFontColor:ASColor = new ASColor(0xCC9900);
		 	var MyFont = new ASFont;
			MyFont = MyFont.changeSize(12);
			m_ScrollTextField = new ScrollTextField(null,
													new NotifyCell,376,120,
													ScrollTextField.eDirect_Top,
													true,7,7,362,105);
			addChild(m_ScrollTextField);
			m_ScrollTextField.setLocationXY(635,13);
			m_ScrollTextField.setFont(MyFont);
			m_ScrollTextField.setForeground(MyFontColor);
			//m_ScrollTextField.Start();
			
			m_labelUserName = new JLabel("",null,JLabel.CENTER);
			addChild(m_labelUserName);
			m_labelUserName.x = 55;
			m_labelUserName.y = 190;
			m_labelUserName.width = 144;
			m_labelUserName.height = 26;
			var font:ASFont = new ASFont;
			font = font.changeSize(18);
			font = font.changeBold(true);
			m_labelUserName.setForeground(new ASColor(0xf1e8a5));
			m_labelUserName.setFont(font);
			m_labelUserScore = new JLabel("0",null,JLabel.CENTER);
			addChild(m_labelUserScore);
			m_labelUserScore.x = 55;
			m_labelUserScore.y = 234;
			m_labelUserScore.width = 144;
			m_labelUserScore.height = 26;
			m_labelUserScore.setForeground(new ASColor(0xf1e8a5));
			font = font.changeSize(18);
			m_labelUserScore.setFont(font);

			return 0;
		}
		//销毁
		public function Destroy():void
		{
			if(m_paneServerListTabbed)
			{
				m_paneServerListTabbed.Destroy();
				removeChild(m_paneServerListTabbed);
				m_paneServerListTabbed = null;
			}
			
			this.m_btnChart.removeEventListener(MouseEvent.CLICK, OnEventChart);
			this.m_btnBetHistory.removeEventListener(MouseEvent.CLICK, OnEventBetHistory);
			this.m_btnChangePasswd.removeEventListener(MouseEvent.CLICK, OnEventChangePasswd);
			this.m_btnOperation.removeEventListener(MouseEvent.CLICK, OnEventOperation);
			this.m_btnLogout.removeEventListener(MouseEvent.CLICK, OnEventLogout);

			if(m_ScrollTextField)
			{
				m_ScrollTextField.Stop();
				m_ScrollTextField.Destroy();
				removeChild(m_ScrollTextField);
				m_ScrollTextField = null;
			}
			if(m_labelUserName)
			{
				removeChild(m_labelUserName);
				m_labelUserName = null;
				
				removeChild(m_labelUserScore);
				m_labelUserScore = null;
			}
	
			m_mcParent = null;
			if(m_IMain)
			{
				m_IMain.removeUserDataChangeEventListener(OnEventUserDataChange);
				m_IMain	   = null;
			}
		}
		//获取面板类型
		public function getPaneType():int
		{
			return nPaneType_Plaza;
		}
		//获取面板名称
		public function getPaneName():String
		{
			return IDS_GAMEPLAZA;
		}
		//获取面板图标
		public function getPaneIcon():Icon
		{
			return GameIcon.loadGameIcon(255);
		}
		//显示面板
		public function showPane(mcParent:MovieClip,bShow:Boolean):void
		{
			if(bShow && mcParent != null)
			{
				m_mcParent = mcParent as PlazaWindow;
				m_mcParent.addChildDO(this);
			}
			else
			{
				if(m_mcParent != null)
				{
					m_mcParent.removeChildDO(this);
					m_mcParent = null;	
				}
			}
		}
		//活动
		public function Active(data:*):int
		{
			if(m_ScrollTextField)
				m_ScrollTextField.Start();
			this.visible = true;

			return 0;
		}
		//取消活动
		public function Deactive():int
		{
			if(m_ScrollTextField)
				m_ScrollTextField.Stop();
			this.visible = false;
			return 0;
		}
		//闪烁
		public function Flash():void
		{
		}
		//判断是否相等面板
		public function EqualPane(param:*):Boolean
		{
			if (param == null)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//获取面板参数
		public function getPaneParam():*
		{
			return null;
		}
		//获取游戏类型
		public function getGameKind():*
		{
			return null;
		}
		//获取活动显示容器
		public function getActiveDOC():DisplayObjectContainer
		{
			return this;
		}
		/////////////////////////////
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			if(m_ScrollTextField)
				m_ScrollTextField.UpdateText(m_IMain.getNotice().szNotice);
			if(m_paneServerListTabbed)
				m_paneServerListTabbed.OnEventFinishLogon();
			if(m_labelUserName)
			{
				var userData:tagUserData = m_IMain.getUserData();
				
				m_labelUserName.setText(String(userData.szName));
				m_labelUserScore.setText(userData.UserScore.fScore.toFixed(2));
			}
		}
		//报表按钮事件
		private function OnEventChart(e:MouseEvent):void
		{
			var c:Class = GetCommonClass("ChartDlg");
			if(c != null)
			{
				var chartDlg:* = new c;
				chartDlg.DoModal();
			}
		}
		//游戏记录按钮事件
		private function OnEventBetHistory(e:MouseEvent):void
		{
			var c:Class = GetCommonClass("BetHistoryDlg");
			if(c != null)
			{
				var chartDlg:* = new c;
				chartDlg.DoModal();
			}
		}
		//更改密码按钮事件
		private function OnEventChangePasswd(e:MouseEvent):void
		{
			var c:Class = GetCommonClass("ChangePasswdDlg");
			if(c != null)
			{
				var chartDlg:* = new c;
				chartDlg.DoModal();
			}
		}
		//操作记录按钮事件
		private function OnEventOperation(e:MouseEvent):void
		{
			var c:Class = GetCommonClass("OperationDlg");
			if(c != null)
			{
				var chartDlg:* = new c;
				chartDlg.DoModal();
			}
		}
		//登出按钮事件
		private function OnEventLogout(e:MouseEvent):void
		{
			if(m_mcParent)
			{
				m_mcParent.SendEventExitPlazaWindow();
			}
		}
		//网络消息
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
				case MDM_GP_SYSTEM :
					{
						return OnSocketMainSystem(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				}
			};
			return false;
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
						if(m_mcParent)
						{
							m_mcParent.SendEventExitPlazaWindow();
						}
					}
					
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
							m_paneServerListTabbed.insertTypeItem(itemType);
							
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
							m_paneServerListTabbed.insertKindItem(itemKind);
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
							m_paneServerListTabbed.insertServerItem(itemServer);
							pBuffer.position += tagGameServer.sizeof_tagGameServer;
						}
						return true;

					};
					//列表发送完成
				case SUB_GP_LIST_FINISH :
					{
						m_paneServerListTabbed.insertItemFinish();
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
		//更新总在线人数
		private function UpdateGameOnLineCount(dwOnLineCount:uint):void
		{
			
		}
		//更新游戏在线人数
		private function UpdateGameKindOnLine(wTypeID:uint,wKindID:uint,dwOnLineCount:uint):void
		{
			if(m_paneServerListTabbed)
				m_paneServerListTabbed.UpdateGameKindOnLine(wTypeID,wKindID,dwOnLineCount);
		}
		//自身用户消息更改事件
		private function OnEventUserDataChange(e:UserDataChangeEvent):void
		{
			if(m_labelUserScore)
			{
				var userData:tagUserData = e.GetUserData();
				m_labelUserName.setText(String(userData.szName));
				m_labelUserScore.setText(userData.UserScore.fScore.toFixed(2));
			}
		}
		//获取定义类
		private function GetCommonClass(strName:String):Class
		{
			var c:Class = m_IMain.getDefClass(strName,"PlazaWindow");
			if(c != null)
			{
				return c;
			}
			else
			{
				m_IMain.ShowMessageBox(IDS_ERR_MODULENOCOMPLETE);
				return null;
			}
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Common/StringTableDef.as"
include "../../Net/InterfaceModuleIDef.as"
include "../../Net/NetModuleIDef.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/GLOBALFRAME.as"
include "../../Net/CMD_Plaza.as"
include "./ConstantDef.as"