package
{
	import flash.display.BitmapData;
	import flash.events.MouseEvent;
	import flash.utils.ByteArray;
	
	import org.god.Common.GameTreeNode;
	import org.god.Common.NumericTableCell;
	import org.god.Common.GameUserStatusUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.TimeUtil;
	import org.god.Common.GameRoundTypeUtil;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Net.IClientSocket;
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Net.tagUserInfoHead;
	import org.god.Net.CMD_GR_LookUserIP;
	import org.god.Net.CMD_GR_LookUserIPResult;
	import org.god.Net.CMD_GR_KillUser;
	import org.god.IGameManagement.*;
	import org.god.SkinControl.SkinButton;
	import org.god.GameManagement.PlazaWindow.Common.ActiveBetHistoryPanelData;
	
	import org.god.IGameFrame.IUserItem;
	import org.god.IGameFrame.IUserManagerSink;
	
	import org.aswing.JLabel;
	import org.aswing.JPanel;
	import org.aswing.JFrame;
	import org.aswing.JTable;
	import org.aswing.JButton;
	import org.aswing.Container;
	import org.aswing.BorderLayout;
	import org.aswing.JScrollPane;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.geom.IntRectangle;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	//对战游戏现场投注页面
	public class BattleMonitorTab extends BaseMonitorTab implements IUserManagerSink
	{
		private var m_btnClose:SkinButton;
		private var m_labelGameRoomPath:JLabel;
		private var m_labelBetLimit:JLabel;
		
		protected var m_tableUser:JTable;
		protected var m_tableModel:DefaultTableModel;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;

		public function BattleMonitorTab()
		{
			super();
		}
		public override function Create(param0:IMain,
										param1:IPlazaWindow,
										param2:BetMonitorPanel,
							   gameType:GameTreeNode = null,
								gameKind:GameTreeNode = null,
								gameServer:GameTreeNode = null):int
		{
			if(super.Create(param0,param1,param2,gameType,gameKind,gameServer) != 0)
				return -1;
				
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;

			var nYP:Number = 10;
			var nXP:Number = 16;
			var nCX:Number = 500;
			var nCY:Number = 24;
			
			nXP = 716;
			m_btnClose = new SkinButton(IDS_CLOSE, nXP, nYP, null, 92,32);
			addChild(m_btnClose);
			m_btnClose.addEventListener(MouseEvent.CLICK, OnEventClose);
			m_btnClose.setFont(font.changeBold(false));
			
			nXP = 10;
			m_labelGameRoomPath = new JLabel("§",null,JLabel.LEFT);
			addChild(m_labelGameRoomPath);
			m_labelGameRoomPath.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			font = font.changeSize(16);
			font = font.changeBold(true);
			m_labelGameRoomPath.setForeground(col);
			m_labelGameRoomPath.setFont(font);
			
			nYP += 28;
			m_labelBetLimit = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelBetLimit);
			m_labelBetLimit.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_labelBetLimit.setForeground(col);
			font = font.changeSize(16);
			m_labelBetLimit.setFont(font);
			
			var column:Array = [IDS_GAMEUSER, IDS_SCORE, IDS_STATE, IDS_TABLEPOSITION, IDS_FUNCTION];		
			var cb:IntRectangle = new IntRectangle(18,84,760,448);
			nCX = cb.width/column.length;
			m_panelTableContainer = new JPanel(new BorderLayout());
			m_tableModel = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			var sorter:TableSorter = new TableSorter(m_tableModel);
			m_tableUser = new JTable(sorter);
			sorter.setTableHeader(m_tableUser.getTableHeader());
			m_tableUser.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			m_tableUser.getColumn(IDS_GAMEUSER).setCellFactory(new GeneralTableCellFactory(UserItemTableCell));
			m_tableUser.getColumn(IDS_SCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			m_tableUser.getColumn(IDS_FUNCTION).setCellFactory(new GeneralTableCellFactory(FunctionTableCell));
			m_tableUser.getColumn(IDS_GAMEUSER).setPreferredWidth(nCX/2 + 58);
			m_tableUser.getColumn(IDS_SCORE).setPreferredWidth(nCX/2 + 48);
			m_tableUser.getColumn(IDS_STATE).setPreferredWidth(nCX/2);
			m_tableUser.getColumn(IDS_TABLEPOSITION).setPreferredWidth(nCX/2-20);
			m_tableUser.getColumn(IDS_FUNCTION).setPreferredWidth(nCX/2*7-168);
			for(var i:uint = 0; i < column.length; i ++)
			{
				m_tableModel.setColumnEditable(i, false);
			}
			m_scollpaneTable = new JScrollPane(m_tableUser);
			m_panelTableContainer.append(m_scollpaneTable, BorderLayout.CENTER);
			addChild(m_panelTableContainer);
			m_panelTableContainer.setComBounds(cb);
			m_panelTableContainer.doLayout();
			m_tableUser.setBackground(new ASColor(0xffffed));
			m_tableUser.setForeground(col);
			m_tableUser.setFont(font.changeSize(12));
			m_tableUser.getTableHeader().setBackground(new ASColor(0xF7E444));
			font = font.changeSize(12);
			font = font.changeBold(false);
			m_tableUser.getTableHeader().setForeground(col);
			m_tableUser.getTableHeader().setFont(font);
			m_tableUser.setRowHeight(28);
			m_tableUser.addEventListener(FuncTableCellBtnEvent.FTCBTNEVENTNAME, OnEventFuncCellBtnClick);

			/////////////////////////////////////////////
			
			m_ClientUserManager.SetUserManagerSink(this);
			
			UpdateGameServerOnLine(m_gameServer.getItemData().wServerID,0);
			UpdateGameBetLimit(m_gameKind.getItemData().dwCellScore,
								m_gameKind.getItemData().dwHighScore,
								m_gameKind.getItemData().dwLessScore);
			
			return 0;
		}
		public override function Destroy():void
		{
			m_ClientUserManager.SetUserManagerSink(null); 

			m_btnClose.removeEventListener(MouseEvent.CLICK, OnEventClose);
			removeChild(m_btnClose);
			m_btnClose = null;
			
			removeChild(m_labelGameRoomPath);
			m_labelGameRoomPath = null;
			removeChild(m_labelBetLimit);
			m_labelBetLimit = null;
			
			m_tableUser.removeEventListener(FuncTableCellBtnEvent.FTCBTNEVENTNAME, OnEventFuncCellBtnClick);
			removeChild(m_panelTableContainer);
			m_tableUser = null;
			m_tableModel = null;
			m_scollpaneTable = null;
			m_panelTableContainer = null;
			
			super.Destroy();
			
		}
		protected override function CreateBackgroundBitmapData():BitmapData
		{
			return new IMAGE_BATTLE_TAB(820,548);
		}
		public override function GetMonitorTitle():String
		{
			return m_gameKind.toString() + "--" + m_gameServer.toString();
		}
		/////////////////////////////////////////////////////////////
		private function UpdateGameBetLimit(dwCellScore:uint,
		dwHighScore:uint,
		dwLessScore:uint):void
		{
			var strBetLimit:String = IDS_GAME + IDS_LESSCORE + ":" + dwLessScore + " " 
				+ IDS_HIGHSCORE + ":" + dwHighScore + " " 
				+ IDS_CELLSCORE + ":" + dwCellScore;
			m_labelBetLimit.setText(strBetLimit);
		}
		protected override function UpdateGameServerOnLine(wServerID:uint,dwOnLineCount:uint):void
		{
			if(wServerID == m_gameServer.getItemData().wServerID)
			{
				var path:String = "§ " + m_gameType.toString() + " -- " + m_gameKind.toString() + " -- " + m_gameServer.toString();
				var strOnLineCount:String = " (" + IDS_ONLINECOUNT +": " + dwOnLineCount + " )";
				
				m_labelGameRoomPath.setText(path + strOnLineCount);
			}
		}

		//////////////////////////////////////////////////////////////
		//用户激活
		public function OnUserItemAcitve(pIUserItem:IUserItem):void
		{
			if(pIUserItem == null)
				return;
			var i:int = FindUserItem(pIUserItem);
			if(i != -1)
			{
				UpdateUserItem(pIUserItem, i);
				return;
			}
			
			var pUserData:tagUserInfoHead=pIUserItem.GetUserData();
			var tableData:Array = [pIUserItem, 
								   pUserData.UserScore.fScore,
								   GameUserStatusUtil.GetUserStatusName(pUserData.UserStatus.cbUserStatus), 
								   GameUserStatusUtil.GetTableIDName(pUserData.UserStatus.wTableID),  
								   String("")
								   ];
					
			m_tableModel.addRow(tableData);
		}
		//用户更新
		public function OnUserItemUpdate( pIUserItem:IUserItem):void
		{
			UpdateUserItem(pIUserItem);
		}
		//用户删除
		public function OnUserItemDelete( pIUserItem:IUserItem):void
		{
			var i:int = FindUserItem(pIUserItem);
			if(i != -1)
			{
				m_tableModel.removeRow(i);
			}
		}
		protected function UpdateUserItem(pIUserItem:IUserItem,i:int = -1):void
		{
			if(i == -1)
			{
				i = FindUserItem(pIUserItem);
				if(i == -1)
					return;
			}
			var pUserData:tagUserInfoHead=pIUserItem.GetUserData();
			m_tableUser.setValueAt(pIUserItem, i, 0);
			m_tableUser.setValueAt(pUserData.UserScore.fScore, i, 1);
			m_tableUser.setValueAt(GameUserStatusUtil.GetUserStatusName(pUserData.UserStatus.cbUserStatus), i, 2);
			m_tableUser.setValueAt(GameUserStatusUtil.GetTableIDName(pUserData.UserStatus.wTableID), i, 3);

		}
		protected function FindUserItem(pIUserItem:IUserItem):int
		{
			for(var i:int = 0; i < m_tableUser.getRowCount(); i ++)
			{
				var pTableIUserItem:IUserItem = m_tableUser.getValueAt(i, 0) as IUserItem;
				if(pIUserItem == pTableIUserItem)
					return i;
				if(pIUserItem.GetUserData().dwUserID == pTableIUserItem.GetUserData().dwUserID)
					return i;
			}
			return -1;
		}
		//////////////////////////////////////////////////////////////
		//网络读取消息
		public override function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if(super.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket))
				return true;
			switch (wMainCmdID)
			{
				case MDM_GR_MANAGER ://管理消息
					{
						return OnSocketMainManager(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);

					};
			}
			return false;
		}
		public function OnSocketMainManager(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_GR_LOOK_USER_IP_RESULT ://查看IP
				{
					if(wDataSize != CMD_GR_LookUserIPResult.sizeof_CMD_GR_LookUserIPResult)
						return true;
					var bHalfAddressString:Boolean = m_IMain.getUserData().cbLevel > 2;
					
					var pLookUserIPResult:CMD_GR_LookUserIPResult = CMD_GR_LookUserIPResult.readData(pBuffer);
					var strIPMessage:String = IDS_USER + IDS_COLON + pLookUserIPResult.szAccount + 
					"\n" + IDS_NAME + IDS_COLON + pLookUserIPResult.szName + 
					"\n" + IDS_IPADDRESS + IDS_COLON + FuncUtil.GetIPAddressString(pLookUserIPResult.dwUserIP, bHalfAddressString) +
					"\n" + IDS_IPDESCRIBE + IDS_COLON + pLookUserIPResult.szIPDescribe;
					m_IMain.ShowMessageBox(strIPMessage, false);
					return true;
				}
			}
			return false;
		}
		//////////////////////////////////////////////////////////////
		protected function OnEventClose(e:MouseEvent):void
		{
			m_BetMonitorPanel.CloseMonitorTab(this);
		}
		protected function OnEventFuncCellBtnClick(e:FuncTableCellBtnEvent):void
		{
			var sel:int = m_tableUser.getSelectedRow();
			if(sel == -1)
				return;
			var pUserData:tagUserInfoHead = null;
			var pIUserItem:IUserItem = m_tableModel.getValueAt(sel, 0)  as IUserItem;
			if(pIUserItem != null)
				pUserData = pIUserItem.GetUserData();
			if(pUserData == null)
				return;
				
			if(e.getActionBtnName() == IDS_LOOKGAMEUSERIP)
			{
				var LookUserIP:CMD_GR_LookUserIP = new CMD_GR_LookUserIP;
				LookUserIP.dwTargetUserID = pUserData.dwUserID;
				//发送数据包
				var wSendSize:uint=CMD_GR_LookUserIP.sizeof_CMD_GR_LookUserIP;
			
				var pPackageBuf:ByteArray = LookUserIP.toByteArray();
				m_clientSocket.SendData(MDM_GR_MANAGER,SUB_GR_LOOK_USER_IP,pPackageBuf,wSendSize);

			}
			else if(e.getActionBtnName() == IDS_GAMEROUNDVIEW)
			{
				var strSelAccount:String = pUserData.szName;
				if(strSelAccount == null || strSelAccount.length <= 0)
					return;

				var ad:ActiveBetHistoryPanelData = new ActiveBetHistoryPanelData;
				ad.szAccount  = strSelAccount;
				ad.fBeginTime = TimeUtil.getQueryBeginTime(3).getTime() / 1000;
				ad.fEndTime  = TimeUtil.getQueryEndTime().getTime() / 1000;
				ad.wGameType = GameModuleNameFactory.GetGameTypeMark(m_gameKind.getItemData().wProcessType);
				ad.wGameKind = m_gameKind.getItemData().wProcessType;
				ad.cbGameRoundType = GameRoundTypeUtil.GameRoundType_Valid;
			
				m_IPlazaWindow.activePanel(nNavCmd_BetHistory,ad);
			}
			else if(e.getActionBtnName() == IDS_KICKGAMEUSER)
			{
				var KillUser:CMD_GR_KillUser = new CMD_GR_KillUser;
				KillUser.dwTargetUserID = pUserData.dwUserID;
				//发送数据包
				var wSendSize:uint=CMD_GR_KillUser.sizeof_CMD_GR_KillUser;
			
				var pPackageBuf:ByteArray = KillUser.toByteArray();
				m_clientSocket.SendData(MDM_GR_MANAGER,SUB_GR_KILL_USER,pPackageBuf,wSendSize);

			}
			else if(e.getActionBtnName() == IDS_CONGEALGAMEUSER)
			{
				
			}
			
			e.preventDefault();
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../Net/CMD_Game.as"