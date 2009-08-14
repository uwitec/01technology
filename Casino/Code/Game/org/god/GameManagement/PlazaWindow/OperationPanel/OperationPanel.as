package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	
	import org.god.GameManagement.PlazaWindow.Common.BasePanel;
	import org.god.GameManagement.PlazaWindow.Common.ActiveOperationPanelData;
	
	import org.god.Net.IClientSocket;
	import org.god.Net.Operation.CMD_GP_GetOperation;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.Operation.CMD_GP_OperationList;
	import org.god.Net.Operation.tagOperationData;
	import org.god.Net.Operation.tagOperationContent;
	import org.god.Net.Operation.tagOperationHeader;
	import org.god.Net.Operation.CMD_GP_OperationListConfig;
	import org.god.IGameManagement.*;
	import org.god.Common.MasterRight;
	import org.god.Common.DrawUtil;
	import org.god.Common.TimeUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.Operation.OperationTypeCell;
	import org.god.Common.Operation.OperationTable;
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.SkinDateField.DateField;
	import org.god.SkinControl.SkinPageBar.PageBar;
	import org.god.SkinControl.SkinPageBar.GotoPageIndexEvent;
	
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
	import org.aswing.event.AWEvent;
	//操作记录面板
	public class OperationPanel extends BasePanel implements IPanelImpl
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		protected static const wDefaultPageSize:uint = 20;
		
		protected var m_btnQuery:SkinButton;
		protected var m_dfEndTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelTo:JLabel;
		protected var m_dfBeginTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelDate:JLabel;
		protected var m_cbxOperationType:JComboBox;
		protected var m_modelOperationType:VectorListModel;
		protected var m_labelOperationType:JLabel;
		protected var m_cbxAccount:JComboBox;
		protected var m_modelAccount:VectorListModel;
		protected var m_labelAccount:JLabel;
		protected var m_tableOperation:OperationTable;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;
		
		protected var m_PageBar:PageBar;
		
		protected var m_GetOperationData:CMD_GP_GetOperation;
		protected var m_dwTotalResultCount:uint;
		
		public function OperationPanel()
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
			
			var nYP:Number = 9;
			var nXP:Number = 924;
			var nCX:Number = 0;
			var nCY:Number = 0;
			m_btnQuery = new SkinButton(IDS_QUERY, nXP, nYP, null, 92,32);
			addChild(m_btnQuery);
			m_btnQuery.addEventListener(MouseEvent.CLICK, OnEventQuery);
			m_btnQuery.setFont(font.changeBold(false));
			
			nXP-= 200;
			nYP = 12;
			m_dfEndTime = new org.god.SkinControl.SkinDateField.DateField;
			addChild(m_dfEndTime);
			m_dfEndTime.setLocationXY(nXP, nYP);
			
			nXP-= 20;
			nYP = 15;
			m_labelTo = new JLabel(IDS_TO, null, JLabel.CENTER);
			addChild(m_labelTo);
			m_labelTo.setComBoundsXYWH(nXP, nYP, 20, 20);
			m_labelTo.setFont(font);
			m_labelTo.setForeground(MyFontColor);
			
			nXP-= 200;
			nYP = 12;
			m_dfBeginTime = new org.god.SkinControl.SkinDateField.DateField;
			addChild(m_dfBeginTime);
			m_dfBeginTime.setLocationXY(nXP, nYP);
			
			nXP-= 38;
			nYP = 16;
			m_labelDate = new JLabel(IDS_TIME + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelDate);
			m_labelDate.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelDate.setFont(font);
			m_labelDate.setForeground(MyFontColor);
		
			nXP-= 124;
			nYP = 14;
			m_modelOperationType = new VectorListModel;
			m_cbxOperationType = new JComboBox(m_modelOperationType);
			addChild(m_cbxOperationType);
			m_cbxOperationType.setComBoundsXYWH(nXP, nYP, 120, 24);
			m_cbxOperationType.setFont(font.changeBold(false));
			//m_cbxOperationType.setForeground(MyFontColor);
			m_cbxOperationType.setEditable(false);
			
			nXP-= 38;
			nYP = 16;
			m_labelOperationType = new JLabel(IDS_OPERATIONTYPE + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelOperationType);
			m_labelOperationType.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelOperationType.setFont(font);
			m_labelOperationType.setForeground(MyFontColor);
			
			nXP-= 104;
			nYP = 14;
			m_modelAccount = new VectorListModel;
			m_cbxAccount = new JComboBox(m_modelAccount);
			addChild(m_cbxAccount);
			m_cbxAccount.setComBoundsXYWH(nXP, nYP, 100, 24);
			m_cbxAccount.setFont(font.changeBold(false));
			//m_cbxAccount.setForeground(MyFontColor);
			m_cbxAccount.setEditable(false);

			nXP-= 38;
			nYP = 16;
			m_labelAccount = new JLabel(IDS_USER + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelAccount);
			m_labelAccount.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelAccount.setFont(font);
			m_labelAccount.setForeground(MyFontColor);
			
			nXP	= 10;
			nYP = 42;
			nCX = 1004;
			nCY = 542;
			m_tableOperation = new OperationTable(m_IMain.getUserData().cbLevel < 2 ? false : true);
			m_scollpaneTable = new JScrollPane(m_tableOperation);
			m_panelTableContainer = new JPanel(new BorderLayout());
			m_panelTableContainer.append(m_scollpaneTable, BorderLayout.CENTER);
			addChild(m_panelTableContainer);
			m_panelTableContainer.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tableOperation.MysetComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_panelTableContainer.doLayout();
			
			m_PageBar = new PageBar;
			addChild(m_PageBar);
			m_PageBar.setFont(font.changeBold(false));
			m_PageBar.addEventListener(GotoPageIndexEvent.EVENTNAME, OnEventGotoPageIndex);
			m_PageBar.setLocationXY(512, 605);
			////////////////////////////////////////////////////////////
			m_modelOperationType.append(new OperationTypeCell(OPT_All));
			m_modelOperationType.append(new OperationTypeCell(OPT_Logon));
			m_modelOperationType.append(new OperationTypeCell(OPT_NewAccount));
			m_modelOperationType.append(new OperationTypeCell(OPT_NewSubAccount));
			m_modelOperationType.append(new OperationTypeCell(OPT_SetBasicInfo));
			m_modelOperationType.append(new OperationTypeCell(OPT_SetScore));
			m_modelOperationType.append(new OperationTypeCell(OPT_SetCongealState));
			m_modelOperationType.append(new OperationTypeCell(OPT_SetOccupancyRate));
			m_modelOperationType.append(new OperationTypeCell(OPT_SetRollbackRate));
			m_modelOperationType.append(new OperationTypeCell(OPT_SetUserRight));
			m_modelOperationType.append(new OperationTypeCell(OPT_KickUser));
			m_cbxOperationType.setSelectedIndex(0);
			
			m_modelAccount.append(m_IMain.getUserAccount());
			m_cbxAccount.setSelectedIndex(0);
			
			m_dfBeginTime.setDateTime(TimeUtil.getQueryBeginTime());
			m_dfEndTime.setDateTime(TimeUtil.getQueryEndTime());
			
			m_btnQuery.setEnabled(MasterRight.CanOperationView(m_IMain.getUserData().dwMasterRight));
			
			return 0;
		}
		public function Destroy():void
		{
			removeChild(m_btnQuery);
			m_btnQuery = null;
			removeChild(m_dfEndTime);
			m_dfEndTime = null;
			removeChild(m_labelTo);
			m_labelTo = null;
			removeChild(m_dfBeginTime);
			m_dfBeginTime = null;
			removeChild(m_labelDate);
			m_labelDate = null;
			removeChild(m_cbxOperationType);
			m_cbxOperationType = null;
			m_modelOperationType = null;
			removeChild(m_labelOperationType);
			m_labelOperationType = null;
			removeChild(m_cbxAccount);
			m_cbxAccount = null;
			m_modelAccount = null;
			removeChild(m_labelAccount);
			m_labelAccount = null;
			m_tableOperation = null;
			m_scollpaneTable = null;
			removeChild(m_panelTableContainer);
			m_panelTableContainer = null;
			
			m_PageBar.removeEventListener(GotoPageIndexEvent.EVENTNAME, OnEventGotoPageIndex);
			removeChild(m_PageBar);
			m_PageBar = null;
		
			m_bmBackground.dispose();
			m_bmBackground = null;
			m_IMain = null;
		 	m_IPlazaWindow = null;
			m_ClientSocket = null;

		}
		public function getNavCmd():int
		{
			return nNavCmd_Operation;
		}
		public function getTitle():String
		{
			return IDS_PANELTITLE_OPERATION;
		}
		public function Active(data:*):int
		{
			if(data != null)
			{
				var szAccount:String = data.szAccount;
				var b:Boolean = false;
				for(var i:uint = 0; i < m_modelAccount.getSize(); i ++)
				{
					if(m_modelAccount.get(i) == szAccount)
					{
						m_cbxAccount.setSelectedIndex(i);
						b = true;
						break;
					}
				}
				if(b == false)
				{
					m_modelAccount.append(szAccount, 1);
					m_cbxAccount.setSelectedIndex(1);
				}
				if(m_modelAccount.getSize() > 10)
				{
					m_modelAccount.removeAt(m_modelAccount.getSize()-1);
				}
				
	
				QueryResult();
			}
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
				case MDM_GP_GET_RECORD:
					return OnSocketGetRecord(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
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
		protected function OnEventQuery(e:MouseEvent):void
		{
			QueryResult();
		}
		protected function OnEventGotoPageIndex(e:GotoPageIndexEvent):void
		{
			QueryResult(e.getPageIndex());
		}
		//////////////////////////////////////////////
		protected function QueryResult(wPageIndex:uint = 0xffff):void
		{
			if(m_GetOperationData == null || wPageIndex == 0xffff)
			{
				var szAccount:String = m_cbxAccount.getSelectedItem() as String;
				if(szAccount.length <= 0)
				{
					m_IMain.ShowMessageBox(IDS_ERR_ACOUNTEMPTY);
					return ;
				}
				
				var BeginTime:Date = m_dfBeginTime.getDateTime();
				var EndTime:Date = m_dfEndTime.getDateTime();
				if(BeginTime.getTime() >= EndTime.getTime())
				{
					m_IMain.ShowMessageBox(IDS_ERR_INVALIDQUERYTIME);
					return ;
				}
				if(TimeUtil.getTimeOffsetDates(BeginTime, EndTime) > 7)
				{
					m_IMain.ShowMessageBox(IDS_ERR_MAXQUERYDATE7);
					return ;
				}
				
				var OPType:OperationTypeCell = m_cbxOperationType.getSelectedItem() as OperationTypeCell;
				
				m_GetOperationData = new CMD_GP_GetOperation;
				m_GetOperationData.dwOperationUserID = m_IMain.getRealUserID();
				m_GetOperationData.wPageSize = wDefaultPageSize;
				m_GetOperationData.szAccount = szAccount;
				m_GetOperationData.cbUserType = 0;
				m_GetOperationData.dwOPType = OPType.getOperationType();
				m_GetOperationData.fBeginTime = BeginTime.getTime() / 1000;
				m_GetOperationData.fEndTime = EndTime.getTime() / 1000;
			}
			m_tableOperation.ClearItem();
			m_IMain.ShowStatusMessage(IDS_STATUS_GETOPERATION);

			m_GetOperationData.wPageIndex = wPageIndex;
			
			var cbBuffer:ByteArray = m_GetOperationData.toByteArray();

			SendData(MDM_GP_GET_RECORD,
					SUB_GP_GET_OPERATION,
					cbBuffer,
					CMD_GP_GetOperation.sizeof_CMD_GP_GetOperation);
			
		}
		public function OnSocketGetRecord(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_LIST_OPERATIONCONFIG:
				{
					if(wDataSize != CMD_GP_OperationListConfig.sizeof_CMD_GP_OperationListConfig)return true;
					var pOperationListConfig:CMD_GP_OperationListConfig = CMD_GP_OperationListConfig.readData(pBuffer);
					var wMaxPageCount:uint;
					if(pOperationListConfig.dwTotalResultCount != 0xffffffff)
					{
						wMaxPageCount = Math.ceil(pOperationListConfig.dwTotalResultCount / pOperationListConfig.wPageSize);
						m_dwTotalResultCount = pOperationListConfig.dwTotalResultCount;
					}
					else
						wMaxPageCount = Math.ceil(m_dwTotalResultCount  / pOperationListConfig.wPageSize);
					m_PageBar.setMaxPageCount(wMaxPageCount);
					m_PageBar.setCurPageIndex(pOperationListConfig.wPageIndex);
					m_tableOperation.ClearItem();
					return true;
				}
				case SUB_GP_LIST_OPERATION:
				{
					if(wDataSize % tagOperationData.sizeof_tagOperationData != 0) return true;
					var pOperationList:CMD_GP_OperationList = CMD_GP_OperationList.readData(pBuffer,wDataSize);
					m_tableOperation.InsertItems(pOperationList.OperationDatas);
					return true;
				}
				case SUB_GP_LIST_OPERATIONFINISH:
				{
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_LIST_OPERATIONERROR:
				{
					m_IMain.HideStatusMessage();
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
					
					return true;
				}
			}
			return false;
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/CMD_Plaza.as"
include "../../../Net/Operation/GlobalOperation.as"