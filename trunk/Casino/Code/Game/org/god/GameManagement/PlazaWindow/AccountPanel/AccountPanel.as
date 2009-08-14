package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.god.GameManagement.PlazaWindow.Common.BasePanel;
	import org.god.GameManagement.PlazaWindow.Common.ActiveOperationPanelData;
	import org.god.Net.IClientSocket;
	import org.god.Net.tagUserData;
	import org.god.Net.tagSubUserData;
	import org.god.Net.CMD_GP_GetLessUserList;
	import org.god.Net.CMD_GP_LessUserList;
	import org.god.Net.CMD_GP_SubUserList;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.CMD_GP_RegisterUser;
	import org.god.Net.CMD_GP_RegisterSuccess;
	import org.god.Net.CMD_GP_SetUserScore;
	import org.god.Net.CMD_GP_SetUserScoreSuccess;
	import org.god.Net.CMD_GP_SetUserCongealState;
	import org.god.Net.CMD_GP_SetUserCongealStateSuccess;
	import org.god.Net.CMD_GP_BasicUserInfo;
	import org.god.Net.CMD_GP_SetBasicUserInfoSuccess;
	import org.god.Net.CMD_GP_DetailUserInfo;
	import org.god.Net.CMD_GP_SetDetailUserInfoSuccess;
	import org.god.Net.CMD_GP_SetUserRight;
	import org.god.Net.CMD_GP_SetUserRightSuccess;
	import org.god.Net.CMD_GP_SetSubUserCongealState;
	import org.god.Net.CMD_GP_SetSubUserCongealStateSuccess;
	import org.god.Net.CMD_GP_RegisterSubUser;
	import org.god.Net.CMD_GP_RegisterSubUserSuccess;
	import org.god.Net.CMD_GP_BasicSubUserInfo;
	import org.god.Net.CMD_GP_SetBasicSubUserInfoSuccess;
	
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.IPlazaWindow;
	import org.god.IGameManagement.IPanelImpl;
	import org.god.Common.DrawUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.MasterRight;
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.DialogEvent;
	
	import org.aswing.JLabel;
	import org.aswing.JPanel;
	import org.aswing.JTree;
	import org.aswing.JTable;
	import org.aswing.JButton;
	import org.aswing.DefaultComboBoxCellEditor;
	import org.aswing.BorderLayout;
	import org.aswing.JScrollPane;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	import org.aswing.tree.DefaultTreeModel;
	import org.aswing.tree.TreePath;
	import org.aswing.tree.GeneralTreeCellFactoryUIResource;
	import org.aswing.geom.IntRectangle;

	//帐号管理面板
	public class AccountPanel extends BasePanel implements IPanelImpl
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		
		protected var m_treeUser:JTree;
		protected var m_scollpaneTree:JScrollPane;
		protected var m_panelTreeContainer:JPanel;
		protected var m_nodeTreeRoot:UserDataMutableTreeNode;
		protected var m_treeModel:DefaultTreeModel;
		protected var m_nodeCurrentSelected:UserDataMutableTreeNode;
		
		protected var m_labelAccount:JLabel;
		protected var m_labelName:JLabel;
		protected var m_labelScoreUsage:JLabel;
		protected var m_labelState:JLabel;
		protected var m_labelOccupancyRate:JLabel;
		protected var m_labelRollbackRate:JLabel;
		protected var m_labelRegisterTime:JLabel;
		
		protected var m_tableUser:JTable;
		protected var m_tableModel:DefaultTableModel;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;
		
		protected var m_tableSubUser:JTable;
		protected var m_tableSubModel:DefaultTableModel;
		protected var m_scollpaneSubTable:JScrollPane;
		protected var m_panelSubTableContainer:JPanel;

		protected var m_btnRegisterUser:SkinButton;
		protected var m_btnRegisterSubAccount:SkinButton;
		
		public function AccountPanel()
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(true);
			}
		}
		//////////////////////////////////
		//IPanelImpl 实现
		//////////////////////////////////
		public function Create(param0:IMain,param1:IPlazaWindow):int
		{
			m_IMain		   = param0;
			m_IPlazaWindow = param1;
			m_ClientSocket = m_IMain.getClientSocket();
			
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;
			
			//创建帐号树型
			m_nodeTreeRoot = new UserDataMutableTreeNode(null);
			m_panelTreeContainer = new JPanel(new BorderLayout());
			m_treeModel = new DefaultTreeModel(m_nodeTreeRoot);
			m_treeUser = new JTree(m_treeModel);
			m_treeUser.setCellFactory(new GeneralTreeCellFactoryUIResource(UserDataTreeCell));
			m_scollpaneTree = new JScrollPane(m_treeUser);
			m_panelTreeContainer.append(m_scollpaneTree, BorderLayout.CENTER);
			addChild(m_panelTreeContainer);
			var cb:IntRectangle = new IntRectangle(12,52,172,567);
			m_panelTreeContainer.setComBounds(cb);
			m_panelTreeContainer.doLayout();
			m_treeUser.setBackground(new ASColor(0xffffed));
			m_treeUser.setForeground(col);
			m_treeUser.setFont(font);
			m_treeUser.addEventListener("treeSelectionChanged", OnEventTreeSelectionChanged);
			m_nodeCurrentSelected = null;			
			m_treeUser.setSelectionMode(JTree.SINGLE_TREE_SELECTION);

			//创建帐号信息版
			var xpos:Number = 200;
			var ypos:Number = 42;
			var xd:Number = 234;
			var yd:Number = 22;
			var ch:Number = 40;
			var xt:Number = xpos;		
			var cx:Number = xd/2 + 30;
			m_labelAccount = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelAccount);
			m_labelAccount.setForeground(col);
			m_labelAccount.setFont(font);
			m_labelAccount.setComBounds(new IntRectangle(xt,ypos,cx,ch));
			m_labelState = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelState);
			m_labelState.setForeground(col);
			m_labelState.setFont(font);
			m_labelState.setComBounds(new IntRectangle(xt,ypos + yd,cx,ch));
			
			xt += cx;
			cx = xd/2 + 30;
			m_labelName = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelName);
			m_labelName.setForeground(col);
			m_labelName.setFont(font);
			m_labelName.setComBounds(new IntRectangle(xt ,ypos,cx,ch));
			m_labelOccupancyRate = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelOccupancyRate);
			m_labelOccupancyRate.setForeground(col);
			m_labelOccupancyRate.setFont(font);
			m_labelOccupancyRate.setComBounds(new IntRectangle(xt,ypos + yd,cx,ch));
			
			xt += cx;
			cx = xd*3-60;
			m_labelScoreUsage = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelScoreUsage);
			m_labelScoreUsage.setForeground(col);
			m_labelScoreUsage.setFont(font);
			m_labelScoreUsage.setComBounds(new IntRectangle(xt,ypos,cx,ch));
			cx = xd/2 + 30;
			m_labelRollbackRate = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelRollbackRate);
			m_labelRollbackRate.setForeground(col);
			m_labelRollbackRate.setFont(font);
			m_labelRollbackRate.setComBounds(new IntRectangle(xt,ypos + yd,cx,ch));
			
			xt += cx;
			cx = xd*2;
			m_labelRegisterTime = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelRegisterTime);
			m_labelRegisterTime.setForeground(col);
			m_labelRegisterTime.setFont(font);
			m_labelRegisterTime.setComBounds(new IntRectangle(xt,ypos + yd,cx,ch));

			//创建帐号列表
			var column:Array = [IDS_LOWUSER, IDS_SCORE, IDS_STATE, IDS_OCCUPANCYRATE, IDS_ROLLBACKRATE,IDS_FUNCTION];		
			cb = new IntRectangle(200,112,807,323);
			cx = cb.width/column.length;
			m_panelTableContainer = new JPanel(new BorderLayout());
			m_tableModel = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			var sorter:TableSorter = new TableSorter(m_tableModel);
			m_tableUser = new JTable(sorter);
			sorter.setTableHeader(m_tableUser.getTableHeader());
			m_tableUser.getColumn(IDS_LOWUSER).setCellFactory(new GeneralTableCellFactory(UserDataTableCell));
			m_tableUser.getColumn(IDS_STATE).setCellFactory(new GeneralTableCellFactory(UserStateTableCell));
			m_tableUser.getColumn(IDS_FUNCTION).setCellFactory(new GeneralTableCellFactory(FunctionTableCell));
			m_tableUser.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			m_tableUser.getColumn(IDS_LOWUSER).setPreferredWidth(cx/2 + 58);
			m_tableUser.getColumn(IDS_SCORE).setPreferredWidth(cx/2 + 58);
			m_tableUser.getColumn(IDS_STATE).setPreferredWidth(cx/2-10);
			m_tableUser.getColumn(IDS_OCCUPANCYRATE).setPreferredWidth(cx/2);
			m_tableUser.getColumn(IDS_ROLLBACKRATE).setPreferredWidth(cx/2);
			m_tableUser.getColumn(IDS_FUNCTION).setPreferredWidth(cx/2*7-120);
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
			m_tableUser.setFont(font);
			m_tableUser.getTableHeader().setBackground(new ASColor(0xF7E444));
			m_tableUser.getTableHeader().setForeground(col);
			m_tableUser.getTableHeader().setFont(font);
			m_tableUser.setRowHeight(26);
			m_tableUser.addEventListener(FuncTableCellBtnEvent.FTCBTNEVENTNAME, OnEventFuncCellBtnClick);
			m_tableUser.addEventListener(UserStateTableCellEvent.USTCEVENTNAME, OnEventStateTableCellChange);
		
			//创建子帐号列表
			var subcolumn:Array = [IDS_SUBUSER, IDS_NAME, IDS_STATE, IDS_REGISTERTIME, IDS_FUNCTION];		
			cb = new IntRectangle(200,448,807,172);
			cx = cb.width/subcolumn.length;
			m_panelSubTableContainer = new JPanel(new BorderLayout());
			m_tableSubModel = (new DefaultTableModel()).initWithNamesRowcount(subcolumn, 0);
			sorter = new TableSorter(m_tableSubModel);
			m_tableSubUser = new JTable(sorter);
			sorter.setTableHeader(m_tableSubUser.getTableHeader());
			m_tableSubUser.getColumn(IDS_SUBUSER).setCellFactory(new GeneralTableCellFactory(SubUserDataTableCell));
			m_tableSubUser.getColumn(IDS_STATE).setCellFactory(new GeneralTableCellFactory(SubUserStateTableCell));
			m_tableSubUser.getColumn(IDS_FUNCTION).setCellFactory(new GeneralTableCellFactory(FunctionSubUserTableCell));
			m_tableSubUser.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			m_tableSubUser.getColumn(IDS_SUBUSER).setPreferredWidth(cx/2 + 118);
			m_tableSubUser.getColumn(IDS_NAME).setPreferredWidth(cx/2 + 118);
			m_tableSubUser.getColumn(IDS_STATE).setPreferredWidth(cx/2-15);
			m_tableSubUser.getColumn(IDS_REGISTERTIME).setPreferredWidth(cx/2*3);
			m_tableSubUser.getColumn(IDS_FUNCTION).setPreferredWidth(cx/2+8);
			for(i= 0; i < subcolumn.length; i ++)
			{
				m_tableSubModel.setColumnEditable(i, false);
			}
			m_scollpaneSubTable = new JScrollPane(m_tableSubUser);
			m_panelSubTableContainer.append(m_scollpaneSubTable, BorderLayout.CENTER);
			addChild(m_panelSubTableContainer);
			m_panelSubTableContainer.setComBounds(cb);
			m_panelSubTableContainer.doLayout();
			m_tableSubUser.setBackground(new ASColor(0xffffed));
			m_tableSubUser.setForeground(col);
			m_tableSubUser.setFont(font);
			m_tableSubUser.getTableHeader().setBackground(new ASColor(0xF7E444));
			m_tableSubUser.getTableHeader().setForeground(col);
			m_tableSubUser.getTableHeader().setFont(font);
			m_tableSubUser.setRowHeight(26);
			m_tableSubUser.addEventListener(FuncTableCellBtnEvent.FTCBTNEVENTNAME, OnEventFuncSubUserCellBtnClick);
			m_tableSubUser.addEventListener(UserStateTableCellEvent.USTCEVENTNAME, OnEventStateSubUserTableCellChange);

			//创建注册按钮
			m_btnRegisterUser = new SkinButton("", 828, 9, null, 92,32);
			addChild(m_btnRegisterUser);
			m_btnRegisterUser.addEventListener(MouseEvent.CLICK, OnEventRegisterUser);
			m_btnRegisterUser.setFont(font.changeBold(false));
			
			m_btnRegisterSubAccount = new SkinButton(IDS_NEW + IDS_SUBUSER, 924, 9, null, 92,32);
			addChild(m_btnRegisterSubAccount);
			m_btnRegisterSubAccount.addEventListener(MouseEvent.CLICK, OnEventRegisterSubAccount);
			m_btnRegisterSubAccount.setFont(font.changeBold(false));

			InsertUserDataTreeNode(m_IMain.getUserData());
			m_treeUser.setSelectionPath(new TreePath(m_nodeTreeRoot.getPath()));

			return 0;
		}
		
		public function Destroy():void
		{
			m_treeUser.removeEventListener("treeSelectionChanged", OnEventTreeSelectionChanged);
			removeChild(m_panelTreeContainer);
			m_panelTreeContainer.remove(m_scollpaneTree);
			m_treeModel = null;
			m_treeUser = null;
			m_scollpaneTree = null;
			m_panelTreeContainer = null;
			
			removeChild(m_labelAccount);
			removeChild(m_labelName);
			removeChild(m_labelScoreUsage);
			removeChild(m_labelState);
			removeChild(m_labelOccupancyRate);
			removeChild(m_labelRollbackRate);
			removeChild(m_labelRegisterTime);
			m_labelAccount = null;
			m_labelName = null;
			m_labelScoreUsage = null;
			m_labelState = null;
			m_labelOccupancyRate = null;
			m_labelRollbackRate = null;
			m_labelRegisterTime = null;
			
			m_tableUser.removeEventListener(FuncTableCellBtnEvent.FTCBTNEVENTNAME, OnEventFuncCellBtnClick);
			m_tableUser.removeEventListener(UserStateTableCellEvent.USTCEVENTNAME, OnEventStateSubUserTableCellChange);
			removeChild(m_panelTableContainer);
			m_tableUser = null;
			m_tableModel = null;
			m_scollpaneTable = null;
			m_panelTableContainer = null;
			
			m_tableSubUser.addEventListener(FuncTableCellBtnEvent.FTCBTNEVENTNAME, OnEventFuncSubUserCellBtnClick);
			m_tableSubUser.addEventListener(UserStateTableCellEvent.USTCEVENTNAME, OnEventStateSubUserTableCellChange);
			removeChild(m_panelSubTableContainer);
			m_tableSubUser = null;
			m_tableSubModel = null;
			m_scollpaneSubTable = null;
			m_panelSubTableContainer = null;
			
			m_btnRegisterUser.removeEventListener(MouseEvent.CLICK, OnEventRegisterUser);
			removeChild(m_btnRegisterUser);
			m_btnRegisterUser = null;
			
			m_btnRegisterSubAccount.removeEventListener(MouseEvent.CLICK, OnEventRegisterSubAccount);
			removeChild(m_btnRegisterSubAccount);
			m_btnRegisterSubAccount = null;
			
			m_bmBackground.dispose();
			m_bmBackground = null;
			m_IMain = null;
		 	m_IPlazaWindow = null;
			m_ClientSocket = null;

		}
		public function getNavCmd():int
		{
			return nNavCmd_Account;
		}
		public function getTitle():String
		{
			return IDS_PANELTITLE_USER;
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
			switch(wMainCmdID)
			{
				case MDM_GP_EDIT_USERINFO:
					return OnSocketEditUser(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				case MDM_GP_EDIT_SUBUSERINFO:
					return OnSocketEditSubUser(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				case MDM_GP_GET_USER:
					return OnSocketGetAccount(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				default:
					break;
			}
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
		protected function OnEventTreeSelectionChanged(e:*):void
		{
			var path:TreePath = e.getNewLeadSelectionPath();
			if(path && path.getLastPathComponent())
			{
				m_nodeCurrentSelected = path.getLastPathComponent();
				
				SetUserInfoTopPanel(path.getLastPathComponent().getUserObject().getUserData());
				UpdateUserTable(path.getLastPathComponent().children());
				UpdateSubAccountTable(path.getLastPathComponent(),true);
				UpdateRegisterButton(path.getLastPathComponent().getUserObject().getUserData());
				
				if(path.getLastPathComponent().getUserObject().getLessUserDataLoaded() == false)
				{
					path.getLastPathComponent().getUserObject().setLessUserDataLoaded();
					SendGetLessUserList(path.getLastPathComponent().getUserObject().getUserData().dwUserID);
				}
			}
		}
		protected function OnEventRegisterUser(e:MouseEvent):void
		{
			var node:UserDataMutableTreeNode = m_treeUser.getLastSelectedPathComponent() as UserDataMutableTreeNode;
			if(node != null && node.getUserObject() != null)
			{
				var ParentUserData:tagUserData = node.getUserObject().getUserData();
				if(ParentUserData)
				{
					var RegisterUser:CMD_GP_RegisterUser = new CMD_GP_RegisterUser;
					var c:Class = GetCommonClass("RegisterUserDlg");
					if(c == null)
						return;
					var dlg:* = new c(ParentUserData,RegisterUser);
					dlg.setFont(MyFont.changeBold(false));
					dlg.addDialogEventListener(OnDlgEventRegisterUser);
					dlg.DoModal();
				}
			}
		}
		protected function OnEventRegisterSubAccount(e:MouseEvent):void
		{
			var node:UserDataMutableTreeNode = m_treeUser.getLastSelectedPathComponent() as UserDataMutableTreeNode;
			if(node != null && node.getUserObject() != null)
			{
				var ParentUserData:tagUserData = node.getUserObject().getUserData();
				if(ParentUserData)
				{
					var RegisterSubUser:CMD_GP_RegisterSubUser = new CMD_GP_RegisterSubUser;
					var c:Class = GetCommonClass("RegisterSubUserDlg");
					if(c == null)
						return;
					var dlg:* = new c(ParentUserData,RegisterSubUser);
					dlg.setFont(MyFont.changeBold(false));
					dlg.addDialogEventListener(OnDlgEventRegisterSubUser);
					dlg.DoModal();
				}
			}
		}
		protected function OnDlgEventRegisterUser(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_REGISTERUSER);
					
					var RegisterUser:CMD_GP_RegisterUser = e.GetUserObject();
					var ParentUserData:tagUserData = e.GetWParam();
					
					RegisterUser.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = RegisterUser.toByteArray();

					SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_REGISTER_USER,
						cbBuffer,
						CMD_GP_RegisterUser.sizeof_CMD_GP_RegisterUser);
				}
				break;
			
				default:
				break;
			}
		}
		protected function OnDlgEventRegisterSubUser(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_REGISTERSUBUSER);
					
					var RegisterSubUser:CMD_GP_RegisterSubUser = e.GetUserObject();
					var ParentUserData:tagUserData = e.GetWParam();
					
					RegisterSubUser.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = RegisterSubUser.toByteArray();

					SendData(MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_REGISTER_SUBUSER,
						cbBuffer,
						CMD_GP_RegisterSubUser.sizeof_CMD_GP_RegisterSubUser);
				}
				break;
				default:
				break;
			}
		}
		protected function OnDlgEventSetUserScore(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_SETUSERSCORE);
					
					var SetUserScore:CMD_GP_SetUserScore = e.GetUserObject();
					var ParentUserData:tagUserData = e.GetWParam();
					
					SetUserScore.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = SetUserScore.toByteArray();

					SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_SETUSERSCORE,
						cbBuffer,
						CMD_GP_SetUserScore.sizeof_CMD_GP_SetUserScore);
				}
				break;
			
				default:
				break;
			}
		}
		protected function OnDlgEventSetBasicUserInfo(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_SETBASICUSERINFO);
					
					var BasicUserInfo:CMD_GP_BasicUserInfo =  e.GetUserObject();
					
					BasicUserInfo.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = BasicUserInfo.toByteArray();

					SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_BASICUSERINFO,
						cbBuffer,
						CMD_GP_BasicUserInfo.sizeof_CMD_GP_BasicUserInfo);
				}
				break;
			
				default:
				break;
			}
		}
		protected function OnDlgEventSetBasicSubUserInfo(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_SETBASICSUBUSERINFO);
					
					var BasicSubUserInfo:CMD_GP_BasicSubUserInfo =  e.GetUserObject();
					
					BasicSubUserInfo.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = BasicSubUserInfo.toByteArray();

					SendData(MDM_GP_EDIT_SUBUSERINFO,
						SUB_GP_SET_BASICSUBUSERINFO,
						cbBuffer,
						CMD_GP_BasicSubUserInfo.sizeof_CMD_GP_BasicSubUserInfo);
				}
				break;
			
				default:
				break;
			}
		}
		protected function OnDlgEventSetDetailUserInfo(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_SETDETAILUSERINFO);
					
					var DetailUserInfo:CMD_GP_DetailUserInfo =  e.GetUserObject();
					
					DetailUserInfo.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = DetailUserInfo.toByteArray();

					SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_DETAILUSERINFO,
						cbBuffer,
						CMD_GP_DetailUserInfo.sizeof_CMD_GP_DetailUserInfo);
				}
				break;
			
				default:
				break;
			}
		}
		protected function OnDlgEventSetUserRight(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_SETUSERRIGHT);
					
					var SetUserRight:CMD_GP_SetUserRight =  e.GetUserObject();
					
					SetUserRight.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = SetUserRight.toByteArray();

					SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_USERRIGHT,
						cbBuffer,
						CMD_GP_SetUserRight.sizeof_CMD_GP_SetUserRight);
				}
				break;
			
				default:
				break;
			}
		}
		
		protected function OnEventFuncCellBtnClick(e:FuncTableCellBtnEvent):void
		{
			var ParentUserData:tagUserData = null;
			var node:UserDataMutableTreeNode = m_treeUser.getLastSelectedPathComponent() as UserDataMutableTreeNode;
			if(node != null && node.getUserObject() != null)
			{
				 ParentUserData = node.getUserObject().getUserData();
				
			}
			if(ParentUserData == null)
			 	return;
			var sel:int = m_tableUser.getSelectedRow();
			if(sel == -1)
				return;
			var SelUserData:tagUserData = null;
			var SelNode:UserDataTreeNodeData = m_tableModel.getValueAt(sel, 0);
			if(SelNode != null)
				SelUserData = SelNode.getUserData();
			if(SelUserData == null)
				return;
				
			if(e.getActionBtnName() == IDS_SETUSERSCORE)
			{
				var SetUserScore:CMD_GP_SetUserScore = new CMD_GP_SetUserScore;
				var c:Class = GetCommonClass("SetUserScoreDlg");
				if(c == null)
						return;
				var dlgSetUserScore:* = new c(ParentUserData,
																				   SelUserData,
																					SetUserScore);
				dlgSetUserScore.setFont(MyFont.changeBold(false));
				dlgSetUserScore.addDialogEventListener(OnDlgEventSetUserScore);
				dlgSetUserScore.DoModal();
				
			
			}
			else if(e.getActionBtnName() == IDS_BASICUSERINFO)
			{
				var BasicUserInfo:CMD_GP_BasicUserInfo = new CMD_GP_BasicUserInfo;
				var c:Class = GetCommonClass("BasicUserInfoDlg");
				if(c == null)
						return;
				var dlgBasicUserInfo:* = new c(ParentUserData,
																				   SelUserData,
																					BasicUserInfo);
				dlgBasicUserInfo.setFont(MyFont.changeBold(false));
				dlgBasicUserInfo.addDialogEventListener(OnDlgEventSetBasicUserInfo);
				dlgBasicUserInfo.DoModal();
			}
			else if(e.getActionBtnName() == IDS_DETAILUSERINFO)
			{
				var DetailUserInfo:CMD_GP_DetailUserInfo = new CMD_GP_DetailUserInfo;
				var c:Class = GetCommonClass("DetailUserInfoDlg");
				if(c == null)
						return;
				var dlgDetailUserInfo:* = new c(ParentUserData,
																				   SelUserData,
																					DetailUserInfo);
				dlgDetailUserInfo.setFont(MyFont.changeBold(false));
				dlgDetailUserInfo.addDialogEventListener(OnDlgEventSetDetailUserInfo);
				dlgDetailUserInfo.DoModal();
			}
			else if(e.getActionBtnName() == IDS_RIGHT)
			{
				var SetUserRight:CMD_GP_SetUserRight = new CMD_GP_SetUserRight;
				var c:Class = GetCommonClass("SetUserRightDlg");
				if(c == null)
						return;
				var dlgSetUserRight:* = new c(ParentUserData,
																				   SelUserData,
																					SetUserRight);
				dlgSetUserRight.setFont(MyFont.changeBold(false));
				dlgSetUserRight.addDialogEventListener(OnDlgEventSetUserRight);
				dlgSetUserRight.DoModal();
			}
			else if(e.getActionBtnName() == IDS_OPERATION)
			{
				var ad:ActiveOperationPanelData = new ActiveOperationPanelData;
				ad.szAccount = SelUserData.szAccount;
				m_IPlazaWindow.activePanel(nNavCmd_Operation,ad);
			}
			e.preventDefault();
		}
		protected function OnEventFuncSubUserCellBtnClick(e:FuncTableCellBtnEvent):void
		{
			var ParentUserData:tagUserData = null;
			var node:UserDataMutableTreeNode = m_treeUser.getLastSelectedPathComponent() as UserDataMutableTreeNode;
			if(node != null && node.getUserObject() != null)
			{
				 ParentUserData = node.getUserObject().getUserData();
				
			}
			if(ParentUserData == null)
			 	return;
			var sel:int = m_tableSubUser.getSelectedRow();
			if(sel == -1)
				return;
			var SelUserData:tagSubUserData = null;
			SelUserData = m_tableSubModel.getValueAt(sel, 0);
			if(SelUserData == null)
				return;
				
			if(e.getActionBtnName() == IDS_BASICUSERINFO)
			{
				var BasicSubUserInfo:CMD_GP_BasicSubUserInfo = new CMD_GP_BasicSubUserInfo;
				var c:Class = GetCommonClass("BasicSubUserInfoDlg");
				if(c == null)
						return;
				var dlgBasicSubUserInfo:* = new c(ParentUserData,
																				   SelUserData,
																					BasicSubUserInfo);
				dlgBasicSubUserInfo.setFont(MyFont.changeBold(false));
				dlgBasicSubUserInfo.addDialogEventListener(OnDlgEventSetBasicSubUserInfo);
				dlgBasicSubUserInfo.DoModal();
			}
			
			e.preventDefault();
		}
		protected function OnEventStateTableCellChange(e:UserStateTableCellEvent):void
		{
			var ParentUserData:tagUserData = null;
			var node:UserDataMutableTreeNode = m_treeUser.getLastSelectedPathComponent() as UserDataMutableTreeNode;
			if(node != null && node.getUserObject() != null)
			{
				 ParentUserData = node.getUserObject().getUserData();
			}
			if(ParentUserData == null)
			 	return;
			var sel:int = m_tableUser.getSelectedRow();
			if(sel == -1)
				return;
			var SelUserData:tagUserData = null;
			var SelNode:UserDataTreeNodeData = m_tableModel.getValueAt(sel, 0);
			if(SelNode != null)
				SelUserData = SelNode.getUserData();
			if(SelUserData == null)
				return;
				
			var SetUserCongealState:CMD_GP_SetUserCongealState = new CMD_GP_SetUserCongealState;
					
			SetUserCongealState.dwOperationUserID = m_IMain.getRealUserID();
			SetUserCongealState.dwUserID = SelUserData.dwUserID;
			SetUserCongealState.cbLevel = SelUserData.cbLevel;
			if(e.getNewState() == IDS_NORMAL)
				SetUserCongealState.cbState = 0;
			else
				SetUserCongealState.cbState = 1;
			var cbBuffer:ByteArray = SetUserCongealState.toByteArray();

			SendData(MDM_GP_EDIT_USERINFO,
					SUB_GP_SET_USERCONGEALSTATE,
					cbBuffer,
					CMD_GP_SetUserCongealState.sizeof_CMD_GP_SetUserCongealState);
		}
		protected function OnEventStateSubUserTableCellChange(e:UserStateTableCellEvent):void
		{
			var ParentUserData:tagUserData = null;
			var node:UserDataMutableTreeNode = m_treeUser.getLastSelectedPathComponent() as UserDataMutableTreeNode;
			if(node != null && node.getUserObject() != null)
			{
				 ParentUserData = node.getUserObject().getUserData();
			}
			if(ParentUserData == null)
			 	return;
			var sel:int = m_tableSubUser.getSelectedRow();
			if(sel == -1)
				return;
			var SelUserData:tagSubUserData = null;
			SelUserData = m_tableSubModel.getValueAt(sel, 0);
			if(SelUserData == null)
				return;
				
			var SetSubUserCongealState:CMD_GP_SetSubUserCongealState = new CMD_GP_SetSubUserCongealState;
					
			SetSubUserCongealState.dwOperationUserID = m_IMain.getRealUserID();
			SetSubUserCongealState.dwUserID = SelUserData.dwUserID;
			
			if(e.getNewState() == IDS_NORMAL)
				SetSubUserCongealState.cbState = 0;
			else
				SetSubUserCongealState.cbState = 1;
			var cbBuffer:ByteArray = SetSubUserCongealState.toByteArray();

			SendData(MDM_GP_EDIT_SUBUSERINFO,
					SUB_GP_SET_SUBUSERCONGEALSTATE,
					cbBuffer,
					CMD_GP_SetSubUserCongealState.sizeof_CMD_GP_SetSubUserCongealState);
		}
		//////////////////////////////////////////////
		protected function InsertUserDataTreeNode(UserData:tagUserData):void
		{
			if(UserData == null)
				return;
			if(m_nodeTreeRoot.getUserObject() == null)
			{
				var nodedata:UserDataTreeNodeData = new UserDataTreeNodeData(UserData);
				m_nodeTreeRoot.setUserObject(nodedata);
				m_treeModel.setRoot(m_nodeTreeRoot);
				return ;
			}
		}
		protected function InsertUserDataTreeNodeArray(UserData:Array, dwParentUserID:uint):UserDataMutableTreeNode
		{
			if(UserData == null || UserData.length == 0)
				return null;
			var parentNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
																				 dwParentUserID);
			if(parentNode == null)
				return null;
			for(var i:uint = 0; i < UserData.length; i ++)
			{
				var nodedata:UserDataTreeNodeData = new UserDataTreeNodeData(UserData[i]);
				var treenode:UserDataMutableTreeNode = new UserDataMutableTreeNode(nodedata);
				parentNode.append(treenode);
			}
			m_treeModel.reload(parentNode);
			return parentNode;
		}
		protected function InsertSubUserDataTreeNodeArray(UserData:Array, dwParentUserID:uint):UserDataMutableTreeNode
		{
			if(UserData == null || UserData.length == 0)
				return null;
			var parentNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
																				 dwParentUserID);
			if(parentNode == null || parentNode.getUserObject() == null)
				return null;
			for(var i:uint = 0; i < UserData.length; i ++)
			{
				parentNode.getUserObject().appendSubUserData(UserData[i]);
			}
			
			return parentNode;
		}
		protected function UpdateUserTable(UserDataNode:Array, bReset:Boolean = true):Boolean
		{
			if(UserDataNode == null)
				return false;
			if(bReset)
				m_tableModel.setRowCount(0);
			for(var i:uint = 0; i < UserDataNode.length; i ++)
			{
				var nodeData:UserDataTreeNodeData = UserDataNode[i].getUserObject();
				if(nodeData)
				{
					var UserData:tagUserData = nodeData.getUserData();
					var strScore:String = UserData.UserScore.fScore.toFixed(2);
					var strState:String = FuncUtil.fu_GetSimpleStateCongealName(UserData.cbLevel,
														   UserData.cbStateCongealFlag);
					var strOccupancyRate:String = FuncUtil.fu_GetOccupancyRateName(UserData.fOccupancyRate, 
														   UserData.cbOccupancyRate_NoFlag);
					var strRollbackRate:String = FuncUtil.fu_GetRollbackRate(UserData.fRollbackRate);
					
					var tableData:Array = [nodeData, strScore, strState, strOccupancyRate,strRollbackRate, ""];
					
					m_tableModel.addRow(tableData);
				}
			}
			return true;
		}
		protected function UpdateSubAccountTable(UserDataNode:UserDataMutableTreeNode,
												 bReset:Boolean = true):Boolean
		{
			if(UserDataNode == null)
				return false;
			if(bReset)
				m_tableSubModel.setRowCount(0);
			var SubUserDatas:Array = UserDataNode.getUserObject().getSubUserDatas();
			for(var i:uint = 0; i < SubUserDatas.length; i ++)
			{
				var SubUserData:tagSubUserData = SubUserDatas[i];
				if(SubUserData)
				{
					var strName:String  = String(SubUserData.szName);
					var strState:String = FuncUtil.fu_GetSimpleSubUserStateCongealName(
														   SubUserData.cbStateCongealFlag);
					
					var strRegisterTime = FuncUtil.fu_GetDateTimeString(SubUserData.fRegisterTime);
					var tableData:Array = [SubUserData, strName, strState, strRegisterTime, ""];
					
					m_tableSubModel.addRow(tableData);
				}
			}
			return true;
		}
		protected function UpdateRegisterButton(UserData:tagUserData):void
		{
			if(UserData == null || UserData.cbLevel == nMinUserLevel)
			{
				m_btnRegisterUser.setVisible(false);
				m_btnRegisterSubAccount.setVisible(false);
				return;
			}
			m_btnRegisterSubAccount.setVisible(true);
			m_btnRegisterUser.setVisible(true);
			m_btnRegisterUser.setText(IDS_NEW + FuncUtil.fu_GetPeerLevelName(UserData.cbLevel + 1));
			
			m_btnRegisterUser.setEnabled(MasterRight.CanAccountNew(m_IMain.getUserData().dwMasterRight)&& MasterRight.SimpleCheck_UserRight());
			m_btnRegisterSubAccount.setEnabled(MasterRight.CanSubAccountNew(m_IMain.getUserData().dwMasterRight)&& MasterRight.SimpleCheck_UserRight());
		}
		protected function UpdateUserDataMutableTreeNodeScore(dwUserID:uint, fScoreOffset:Number):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													dwUserID);
			if(UserNode == null)
				return ;
			var UserData:tagUserData = UserNode.getUserObject().getUserData();
			UserData.UserScore.fScore += fScoreOffset;
			
			var ParentNode:UserDataMutableTreeNode = UserNode.getParent() as UserDataMutableTreeNode;
			if(ParentNode == null)
				return ;
			var ParentUserData:tagUserData = ParentNode.getUserObject().getUserData();
			ParentUserData.UserScore.fScore -= fScoreOffset;
			
			if(UserData.cbLevel == nMinUserLevel)
			{
				do
				{
					ParentUserData = ParentNode.getUserObject().getUserData();
					ParentUserData.fMidScore -= fScoreOffset;
					ParentUserData.fLowScore += fScoreOffset;
					ParentNode = ParentNode.getParent() as UserDataMutableTreeNode;
				}while(ParentNode!=null)
			}
			else
			{
				ParentUserData.fMidScore += fScoreOffset;
			}
		}
		protected function UpdateUserDataMutableTreeNodeCongealState(pSetUserCongealStateSuccess:CMD_GP_SetUserCongealStateSuccess):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													pSetUserCongealStateSuccess.dwUserID);
			if(UserNode == null)
				return ;
			EachChildTreeNodeFunction(UserNode, ECTNFunc_CongealState,pSetUserCongealStateSuccess);
		}
		protected function UpdateSubUserDataMutableTreeNodeCongealState(pSetSubUserCongealStateSuccess:CMD_GP_SetSubUserCongealStateSuccess):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													pSetSubUserCongealStateSuccess.dwParentUserID);
			if(UserNode == null)
				return ;
			var SubUserDatas:Array = UserNode.getUserObject().getSubUserDatas();
			for(var i:uint = 0; i < SubUserDatas.length; i ++)
			{
				var SubUserData:tagSubUserData = SubUserDatas[i];
				if(SubUserData && SubUserData.dwUserID == pSetSubUserCongealStateSuccess.dwUserID)
				{
					if(pSetSubUserCongealStateSuccess.cbState)
					{
						SubUserData.cbStateCongealFlag = 
						SubUserData.cbStateCongealFlag | pSetSubUserCongealStateSuccess.dwStateMark;
					}
					else
					{
						SubUserData.cbStateCongealFlag = 
						SubUserData.cbStateCongealFlag & pSetSubUserCongealStateSuccess.dwStateMark;
					}
					break;
				}
			}
		}
		protected function UpdateSubUserDataMutableTreeNodeBasicInfo(pSetBasicSubUserInfoSuccess:CMD_GP_SetBasicSubUserInfoSuccess):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													pSetBasicSubUserInfoSuccess.dwParentUserID);
			if(UserNode == null)
				return ;
			var SubUserDatas:Array = UserNode.getUserObject().getSubUserDatas();
			for(var i:uint = 0; i < SubUserDatas.length; i ++)
			{
				var SubUserData:tagSubUserData = SubUserDatas[i];
				if(SubUserData && SubUserData.dwUserID == pSetBasicSubUserInfoSuccess.dwUserID)
				{
					SubUserData.szName = pSetBasicSubUserInfoSuccess.szName;
					break;
				}
			}
		}
		protected function UpdateUserDataMutableTreeNodeBasicInfo(pSetBasicUserInfoSuccess:CMD_GP_SetBasicUserInfoSuccess):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													pSetBasicUserInfoSuccess.dwUserID);
			if(UserNode == null)
				return ;
			var UserData:tagUserData = UserNode.getUserObject().getUserData();
			UserData.szName = pSetBasicUserInfoSuccess.szName;
			UserData.cbFaceID = pSetBasicUserInfoSuccess.cbFaceID;
		}
		protected function UpdateUserDataMutableTreeNodeDetailInfo(pSetDetailUserInfoSuccess:CMD_GP_SetDetailUserInfoSuccess):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													pSetDetailUserInfoSuccess.dwUserID);
			if(UserNode == null)
				return ;
			var UserData:tagUserData = UserNode.getUserObject().getUserData();
			UserData.fOccupancyRate  = pSetDetailUserInfoSuccess.fOccupancyRate;
			UserData.cbOccupancyRate_NoFlag = pSetDetailUserInfoSuccess.cbOccupancyRate_NoFlag;
			UserData.fLowOccupancyRate_Max  = pSetDetailUserInfoSuccess.fLowOccupancyRate_Max;
			UserData.cbLowOccupancyRate_Max_NoFlag = pSetDetailUserInfoSuccess.cbLowOccupancyRate_Max_NoFlag;
			UserData.fLowOccupancyRate_Min  = pSetDetailUserInfoSuccess.fLowOccupancyRate_Min;
			UserData.cbLowOccupancyRate_Min_NoFlag = pSetDetailUserInfoSuccess.cbLowOccupancyRate_Min_NoFlag;
			UserData.fTaxOccupancyRate  = pSetDetailUserInfoSuccess.fTaxOccupancyRate;
			UserData.fRollbackRate  = pSetDetailUserInfoSuccess.fRollbackRate;
			UserData.dwBetLimit 	= pSetDetailUserInfoSuccess.dwBetLimit;
			
			EachChildTreeNodeFunction(UserNode, ECTNFunc_DetailInfo,pSetDetailUserInfoSuccess);
		}
		protected function UpdateUserDataMutableTreeNodeRight(pSetUserRightSuccess:CMD_GP_SetUserRightSuccess):void
		{
			var UserNode:UserDataMutableTreeNode = FindUserDataMutableTreeNode(m_nodeTreeRoot,
													pSetUserRightSuccess.dwUserID);
			if(UserNode == null)
				return ;
			var UserData:tagUserData = UserNode.getUserObject().getUserData();
			UserData.dwUserRight   = pSetUserRightSuccess.dwUserRight;
			UserData.dwMasterRight = pSetUserRightSuccess.dwMasterRight;
		}
		protected function UpdateUserTableScore(dwUserID:uint, 
												   fScoreOffset:Number):void
		{
			for(var i:uint = 0; i < m_tableModel.getRowCount(); i ++)
			{
				var UserNode:UserDataTreeNodeData = m_tableModel.getValueAt(i, 0);
				if(UserNode != null && UserNode.getUserData() != null)
				{
					if(UserNode.getUserData().dwUserID == dwUserID)
					{
						var fScore:Number = UserNode.getUserData().UserScore.fScore;
						var strScore:String = fScore.toFixed(2);
						m_tableModel.setValueAt(strScore, i, 1);
						break;
					}
				}
			}
		}
		protected function UpdateUserTableCongealState(dwUserID:uint,
													  cbState:uint,
													  dwStateMark:uint):void
		{
			for(var i:uint = 0; i < m_tableModel.getRowCount(); i ++)
			{
				var UserNode:UserDataTreeNodeData = m_tableModel.getValueAt(i, 0);
				if(UserNode != null && UserNode.getUserData() != null)
				{
					if(UserNode.getUserData().dwUserID == dwUserID)
					{
						var strState:String = FuncUtil.fu_GetSimpleStateCongealName(
																		   UserNode.getUserData().cbLevel,
														   UserNode.getUserData().cbStateCongealFlag);
						m_tableModel.setValueAt(strState, i, 2);
						break;
					}
				}
			}
		}
		protected function UpdateSubUserTableCongealState(dwUserID:uint,
													  cbState:uint,
													  dwStateMark:uint):void
		{
			for(var i:uint = 0; i < m_tableSubModel.getRowCount(); i ++)
			{
				var UserData:tagSubUserData = m_tableSubModel.getValueAt(i, 0);
				if(UserData)
				{
					if(UserData.dwUserID == dwUserID)
					{
						var strState:String = FuncUtil.fu_GetSimpleSubUserStateCongealName(
														   UserData.cbStateCongealFlag);
						m_tableSubModel.setValueAt(strState, i, 2);
						break;
					}
				}
			}
		}
		protected function UpdateSubUserTableBasicInfo(dwUserID:uint,
													  szName:String):void
		{
			for(var i:uint = 0; i < m_tableSubModel.getRowCount(); i ++)
			{
				var UserData:tagSubUserData = m_tableSubModel.getValueAt(i, 0);
				if(UserData != null && UserData.dwUserID == dwUserID)
				{
					var strState:String = String(szName);
					m_tableSubModel.setValueAt(szName, i, 1);
					break;
					
				}
			}
		}
		protected function UpdateUserTableDetailInfo(dwUserID:uint,
													  pSetDetailUserInfoSuccess:CMD_GP_SetDetailUserInfoSuccess):void
		{
			for(var i:uint = 0; i < m_tableModel.getRowCount(); i ++)
			{
				var UserNode:UserDataTreeNodeData = m_tableModel.getValueAt(i, 0);
				if(UserNode != null && UserNode.getUserData() != null)
				{
					if(UserNode.getUserData().dwUserID == dwUserID)
					{
						var strOccupancyRate:String = FuncUtil.fu_GetOccupancyRateName(UserNode.getUserData().fOccupancyRate, 
														   UserNode.getUserData().cbOccupancyRate_NoFlag);
						var strRollbackRate:String = FuncUtil.fu_GetRollbackRate(UserNode.getUserData().fRollbackRate);

						m_tableModel.setValueAt(strOccupancyRate, i, 3);
						m_tableModel.setValueAt(strRollbackRate, i, 4);
						break;
					}
				}
			}
		}
		protected function FindUserDataMutableTreeNode(node:UserDataMutableTreeNode,
													   dwUserID:uint):UserDataMutableTreeNode
		{
			if(node == null || node.getUserObject() == null)
				return null;
			if(node.getUserObject().getUserData().dwUserID == dwUserID)
				return node;
			for(var i:uint = 0; i < node.getChildCount(); i ++)
			{
				var node_child = node.getChildAt(i) as UserDataMutableTreeNode;
				var node_find:UserDataMutableTreeNode = FindUserDataMutableTreeNode(node_child,dwUserID);
				if(node_find != null)
					return node_find;
			}
			return null;
		}
		
		protected function SetUserInfoTopPanel(UserData:tagUserData):void
		{			
			var strTmp:String;
			
			strTmp = FuncUtil.fu_GetPeerLevelName(UserData.cbLevel) + ":" + UserData.szAccount;
			m_labelAccount.setText(String(strTmp));
			
			strTmp = IDS_NAME + ":" + UserData.szName;
			m_labelName.setText(String(strTmp));
			
			var fMidScore:Number = UserData.fMidScore;
			if(UserData.cbLevel == nMinUserLevel-1 ||
			   UserData.cbLevel == nMinUserLevel)
				fMidScore = 0;
				
			strTmp = IDS_SCORE + ":" + UserData.UserScore.fScore.toFixed(2) + "   "
			+ IDS_MIDSCORE + ":" + fMidScore.toFixed(2) + "   " 
			+ IDS_GAMEUSER + ":" + UserData.fLowScore.toFixed(2);
			m_labelScoreUsage.setText(String(strTmp));
			
			strTmp = IDS_STATE + ":" + FuncUtil.fu_GetStateCongealName(UserData.cbLevel,
														   UserData.cbStateCongealFlag);
			m_labelState.setText(String(strTmp));
			if(FuncUtil.fu_IsCongeal(UserData.cbStateCongealFlag))
				m_labelState.setForeground(ASColor.RED);
			else
				m_labelState.setForeground(MyFontColor);
			
			
			strTmp = IDS_OCCUPANCYRATE + ":" + FuncUtil.fu_GetOccupancyRateName(UserData.fOccupancyRate, 
														   UserData.cbOccupancyRate_NoFlag);
			m_labelOccupancyRate.setText(String(strTmp));
			
			strTmp = IDS_ROLLBACKRATE + ":" + FuncUtil.fu_GetRollbackRate(UserData.fRollbackRate);
			m_labelRollbackRate.setText(String(strTmp));
			
			strTmp = IDS_REGISTERTIME + ":" + FuncUtil.fu_GetDateTimeString(UserData.fRegisterTime);
			m_labelRegisterTime.setText(String(strTmp));
		}
		protected function SendGetLessUserList(dwUserID:uint):void
		{
			var CmdData:CMD_GP_GetLessUserList = new CMD_GP_GetLessUserList;
			CmdData.dwParentUserID = dwUserID;
			var cbBuffer:ByteArray = CmdData.toByteArray();

			SendData(MDM_GP_GET_USER,
					SUB_GP_GET_LOWUSERLIST,
					cbBuffer,
					CMD_GP_GetLessUserList.sizeof_CMD_GP_GetLessUserList);

		}
		protected function EachChildTreeNodeFunction(node:UserDataMutableTreeNode,
													 func:Function, param:*):void
		{
			if(node == null || node.getUserObject() == null)
				return ;
			func(node,param);
			for(var i:uint = 0; i < node.getChildCount(); i ++)
			{
				var node_child:UserDataMutableTreeNode = 
				node.getChildAt(i) as UserDataMutableTreeNode;
				
				EachChildTreeNodeFunction(node_child,func,param);
			}
			
		}
		protected function ECTNFunc_CongealState(node:UserDataMutableTreeNode,
												 pSetUserCongealStateSuccess:CMD_GP_SetUserCongealStateSuccess):void
		{
			if(node == null)
				return;
			if(pSetUserCongealStateSuccess.cbState)
			{
				node.getUserObject().getUserData().cbStateCongealFlag = 
				node.getUserObject().getUserData().cbStateCongealFlag | pSetUserCongealStateSuccess.dwStateMark;
				
			}
			else
			{
				node.getUserObject().getUserData().cbStateCongealFlag = 
				node.getUserObject().getUserData().cbStateCongealFlag & pSetUserCongealStateSuccess.dwStateMark;
				
			}
		}
		protected function ECTNFunc_DetailInfo(node:UserDataMutableTreeNode,
												 pSetDetailUserInfoSuccess:CMD_GP_SetDetailUserInfoSuccess):void
		{
			if(node == null)
				return;
			var UserData:tagUserData = node.getUserObject().getUserData();
			if(UserData.fOccupancyRate > pSetDetailUserInfoSuccess.fOccupancyRate)
				UserData.fOccupancyRate = pSetDetailUserInfoSuccess.fOccupancyRate;
			if(UserData.fLowOccupancyRate_Max > pSetDetailUserInfoSuccess.fOccupancyRate)
				UserData.fLowOccupancyRate_Max = pSetDetailUserInfoSuccess.fOccupancyRate;
			if(UserData.fLowOccupancyRate_Min > pSetDetailUserInfoSuccess.fOccupancyRate)
				UserData.fLowOccupancyRate_Min = pSetDetailUserInfoSuccess.fOccupancyRate;
			if(UserData.fRollbackRate > pSetDetailUserInfoSuccess.fRollbackRate)
				UserData.fRollbackRate = pSetDetailUserInfoSuccess.fRollbackRate;
				
		}
		public function GetCommonClass(strName:String):Class
		{
			var c:Class = m_IMain.getDefClass(strName,"AccountPanel_Common");
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
		///////////////////////////////////////////////////////////////////////////
		public function OnSocketEditUser(wMainCmdID:uint,
										   wSubCmdID:uint,
										   pBuffer:ByteArray,
										   wDataSize:int,
										   pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_REGISTER_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_RegisterSuccess.sizeof_CMD_GP_RegisterSuccess)
						return true;
					//变量定义
					var pRegisterSuccess:CMD_GP_RegisterSuccess=
					CMD_GP_RegisterSuccess.readData(pBuffer);
					
					var UserData:Array = new Array(1);
					UserData[0] = pRegisterSuccess.UserData;
					var dwParentUserID:uint = pRegisterSuccess.UserData.dwParentUserID;
					
					var node:UserDataMutableTreeNode = InsertUserDataTreeNodeArray(UserData,
												dwParentUserID);
					if(node != null)
					{
						SetUserInfoTopPanel(node.getUserObject().getUserData());
						UpdateUserTable(node.children(), true);
						UpdateSubAccountTable(node, true);
						m_treeUser.setSelectionPath(new TreePath(node.getPath()));
						m_treeUser.expandPath(new TreePath(node.getPath()));
					}
					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_SETUSERSCORE_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetUserScoreSuccess.sizeof_CMD_GP_SetUserScoreSuccess)
						return true;
					//变量定义
					var pSetUserScoreSuccess:CMD_GP_SetUserScoreSuccess=
					CMD_GP_SetUserScoreSuccess.readData(pBuffer);
					
					UpdateUserDataMutableTreeNodeScore(pSetUserScoreSuccess.dwUserID,
													  pSetUserScoreSuccess.fScoreOffset);
					UpdateUserTableScore(pSetUserScoreSuccess.dwUserID,
													  pSetUserScoreSuccess.fScoreOffset);
					if(m_nodeCurrentSelected &&
					   m_nodeCurrentSelected.getUserObject() &&
					   (m_nodeCurrentSelected.getUserObject().getUserData().dwUserID == pSetUserScoreSuccess.dwUserID ||
						m_nodeCurrentSelected.getUserObject().getUserData().dwUserID == pSetUserScoreSuccess.dwParentUserID))
					{
						SetUserInfoTopPanel( m_nodeCurrentSelected.getUserObject().getUserData());
					}
					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_USERCONGEALSTATE_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetUserCongealStateSuccess.sizeof_CMD_GP_SetUserCongealStateSuccess)
						return true;
					//变量定义
					var pSetUserCongealStateSuccess:CMD_GP_SetUserCongealStateSuccess=
					CMD_GP_SetUserCongealStateSuccess.readData(pBuffer);
					
					UpdateUserDataMutableTreeNodeCongealState(pSetUserCongealStateSuccess);
					UpdateUserTableCongealState(pSetUserCongealStateSuccess.dwUserID,
													  pSetUserCongealStateSuccess.cbState,
													  pSetUserCongealStateSuccess.dwStateMark);
					if(m_nodeCurrentSelected &&
					   m_nodeCurrentSelected.getUserObject() &&
					   (m_nodeCurrentSelected.getUserObject().getUserData().dwUserID == pSetUserCongealStateSuccess.dwUserID))
					{
						SetUserInfoTopPanel( m_nodeCurrentSelected.getUserObject().getUserData());
					}
					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_BASICUSERINFO_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetBasicUserInfoSuccess.sizeof_CMD_GP_SetBasicUserInfoSuccess)
						return true;
					//变量定义
					var pSetBasicUserInfoSuccess:CMD_GP_SetBasicUserInfoSuccess=
					CMD_GP_SetBasicUserInfoSuccess.readData(pBuffer);
					
					UpdateUserDataMutableTreeNodeBasicInfo(pSetBasicUserInfoSuccess);
					
					if(m_nodeCurrentSelected &&
					   m_nodeCurrentSelected.getUserObject() &&
					   (m_nodeCurrentSelected.getUserObject().getUserData().dwUserID == pSetBasicUserInfoSuccess.dwUserID))
					{
						SetUserInfoTopPanel( m_nodeCurrentSelected.getUserObject().getUserData());
					}
					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_DETAILUSERINFO_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetDetailUserInfoSuccess.sizeof_CMD_GP_SetDetailUserInfoSuccess)
						return true;
					//变量定义
					var pSetDetailUserInfoSuccess:CMD_GP_SetDetailUserInfoSuccess=
					CMD_GP_SetDetailUserInfoSuccess.readData(pBuffer);
					
					UpdateUserDataMutableTreeNodeDetailInfo(pSetDetailUserInfoSuccess);
					UpdateUserTableDetailInfo(pSetDetailUserInfoSuccess.dwUserID,
											pSetDetailUserInfoSuccess);

									
					if(m_nodeCurrentSelected &&
					   m_nodeCurrentSelected.getUserObject() &&
					   (m_nodeCurrentSelected.getUserObject().getUserData().dwUserID == pSetDetailUserInfoSuccess.dwUserID))
					{
						SetUserInfoTopPanel( m_nodeCurrentSelected.getUserObject().getUserData());
					}
					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_USERRIGHT_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetUserRightSuccess.sizeof_CMD_GP_SetUserRightSuccess)
						return true;
					//变量定义
					var pSetUserRightSuccess:CMD_GP_SetUserRightSuccess=
					CMD_GP_SetUserRightSuccess.readData(pBuffer);
					
					UpdateUserDataMutableTreeNodeRight(pSetUserRightSuccess);
			
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_REGISTER_ERROR:
				case SUB_GP_SET_SETUSERSCORE_ERROR:
				case SUB_GP_SET_USERCONGEALSTATE_ERROR:
				case SUB_GP_SET_BASICUSERINFO_ERROR:
				case SUB_GP_SET_DETAILUSERINFO_ERROR:
				case SUB_GP_SET_USERRIGHT_ERROR:
				{
					m_IMain.HideStatusMessage();
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
					return true;
				}
				
				default:
					break;
			}
			return false;
		}
		public function OnSocketEditSubUser(wMainCmdID:uint,
										   wSubCmdID:uint,
										   pBuffer:ByteArray,
										   wDataSize:int,
										   pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_REGISTER_SUBUSER_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_RegisterSubUserSuccess.sizeof_CMD_GP_RegisterSubUserSuccess)
						return true;
					//变量定义
					var pRegisterSubUserSuccess:CMD_GP_RegisterSubUserSuccess=
					CMD_GP_RegisterSubUserSuccess.readData(pBuffer);
					
					var UserData:Array = new Array(1);
					UserData[0] = pRegisterSubUserSuccess.UserData;
					var dwParentUserID:uint = pRegisterSubUserSuccess.UserData.dwParentUserID;
					
					var node:UserDataMutableTreeNode = InsertSubUserDataTreeNodeArray(UserData,
												dwParentUserID);
					if(node != null)
					{
						SetUserInfoTopPanel(node.getUserObject().getUserData());
						UpdateUserTable(node.children(), true);
						UpdateSubAccountTable(node,true);
						m_treeUser.setSelectionPath(new TreePath(node.getPath()));
						m_treeUser.expandPath(new TreePath(node.getPath()));
					}
					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_BASICSUBUSERINFO_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetBasicSubUserInfoSuccess.sizeof_CMD_GP_SetBasicSubUserInfoSuccess)
						return true;
					//变量定义
					var pSetBasicSubUserInfoSuccess:CMD_GP_SetBasicSubUserInfoSuccess=
					CMD_GP_SetBasicSubUserInfoSuccess.readData(pBuffer);
					
					UpdateSubUserDataMutableTreeNodeBasicInfo(pSetBasicSubUserInfoSuccess);
					UpdateSubUserTableBasicInfo(pSetBasicSubUserInfoSuccess.dwUserID,
													  pSetBasicSubUserInfoSuccess.szName);

					
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_SET_SUBUSERCONGEALSTATE_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetSubUserCongealStateSuccess.sizeof_CMD_GP_SetSubUserCongealStateSuccess)
						return true;
					//变量定义
					var pSetSubUserCongealStateSuccess:CMD_GP_SetSubUserCongealStateSuccess=
					CMD_GP_SetSubUserCongealStateSuccess.readData(pBuffer);
					UpdateSubUserDataMutableTreeNodeCongealState(pSetSubUserCongealStateSuccess);
					UpdateSubUserTableCongealState(pSetSubUserCongealStateSuccess.dwUserID,
													  pSetSubUserCongealStateSuccess.cbState,
													  pSetSubUserCongealStateSuccess.dwStateMark);
					return true;
				}
				case SUB_GP_REGISTER_SUBUSER_ERROR:
				case SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR:
				case SUB_GP_SET_BASICSUBUSERINFO_ERROR:
				{
					m_IMain.HideStatusMessage();
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
					return true;
				}
				default:
				break;
			}
			return false;
		}
		public function OnSocketGetAccount(wMainCmdID:uint,
										   wSubCmdID:uint,
										   pBuffer:ByteArray,
										   wDataSize:int,
										   pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_LIST_LOWUSER:
				{
					//变量定义
					var pLessUserList:CMD_GP_LessUserList=
					CMD_GP_LessUserList.readData(pBuffer);
					var wHeadSize:uint=CMD_GP_LessUserList.sizeof_Header_CMD_GP_LessUserList;

					//效验数据
					if (wDataSize<wHeadSize)
					{
						return true;
					}
					if (wDataSize!=(wHeadSize+pLessUserList.wUserCount * tagUserData.sizeof_tagUserData))
					{
						return true;
					}
					var node:UserDataMutableTreeNode = InsertUserDataTreeNodeArray(pLessUserList.UserData,
												pLessUserList.dwParentUserID);
					if(node != null)
					{
						SetUserInfoTopPanel(node.getUserObject().getUserData());
						UpdateUserTable(node.children(), true);
						UpdateSubAccountTable(node,true);
						m_treeUser.setSelectionPath(new TreePath(node.getPath()));
						m_treeUser.expandPath(new TreePath(node.getPath()));
					}
	
					return true;
				}
				case SUB_GP_LIST_SUBUSER:
				{
					//变量定义
					var pSubUserList:CMD_GP_SubUserList=
					CMD_GP_SubUserList.readData(pBuffer);
					var wHeadSize:uint=CMD_GP_SubUserList.sizeof_Header_CMD_GP_SubUserList;

					//效验数据
					if (wDataSize<wHeadSize)
					{
						return true;
					}
					if (wDataSize!=(wHeadSize+pSubUserList.wSubUserCount * tagSubUserData.sizeof_tagSubUserData))
					{
						return true;
					}
					var node:UserDataMutableTreeNode = InsertSubUserDataTreeNodeArray(pSubUserList.UserData,
												pSubUserList.dwParentUserID);
					if(node != null)
					{
						SetUserInfoTopPanel(node.getUserObject().getUserData());
						UpdateUserTable(node.children(), true);
						UpdateSubAccountTable(node,true);
						m_treeUser.setSelectionPath(new TreePath(node.getPath()));
						m_treeUser.expandPath(new TreePath(node.getPath()));
					}
	
					return true;
				}
				case SUB_GP_LIST_LOWUSERERROR:
				{
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
					return true;
				}
				default:
				break;
			}
			return false;
		}
		
	}
}
include "./../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/CMD_Plaza.as"