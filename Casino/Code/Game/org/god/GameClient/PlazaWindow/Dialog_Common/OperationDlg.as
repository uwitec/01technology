package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketRecvSink;
	import org.god.Net.Operation.CMD_GP_GetOperation;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.Operation.CMD_GP_OperationList;
	import org.god.Net.Operation.tagOperationData;
	import org.god.Net.Operation.tagOperationContent;
	import org.god.Net.Operation.tagOperationHeader;
	import org.god.Net.Operation.CMD_GP_OperationListConfig;
	import org.god.Common.DrawUtil;
	import org.god.Common.TimeUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.Operation.OperationTypeCell;
	import org.god.Common.Operation.OperationTable;
	import org.god.Common.GlobalCommonMain;
	import org.god.Common.MasterRight;
	import org.god.IGameFrame.IMain;
	import org.god.SkinControl.BaseDialog;
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
	import org.aswing.geom.IntDimension;
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.event.AWEvent;
	//操作记录对话框
	public class OperationDlg extends BaseDialog implements IClientSocketRecvSink
	{
		protected static const MyFontColor:ASColor = new ASColor(0xf1e8a5);
		protected static var MyFont:ASFont;
		protected static const wDefaultPageSize:uint = 20;
		
		protected var m_IMain:IMain;
		protected var m_ClientSocket:IClientSocket;

		protected var m_btnQuery:SkinButton;
		protected var m_dfEndTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelTo:JLabel;
		protected var m_dfBeginTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelDate:JLabel;
		protected var m_tableOperation:OperationTable;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;
		
		protected var m_PageBar:PageBar;
		
		protected var m_GetOperationData:CMD_GP_GetOperation;
		protected var m_dwTotalResultCount:uint;
		
		public function OperationDlg()
		{
			super(new IMAGE_OPERATIONDLG(1024,625),
				null,null,BaseDialog.DLGTYPE_OK);
			
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 //MyFont = MyFont.changeBold(true);
			}
			setFont(MyFont);
		}
		public override function Center(w:Number=1024,h:Number=768,hd:Number = -30):void
		{
			var rv:IntDimension = getSize();
			setGlobalLocationXY((w - rv.width)/2, (h-rv.height)/2 + hd);
			if(m_nDlgType == DLGTYPE_OK)
			{
				if(m_btnOK)
					m_btnOK.setLocationXY(rv.width - btn_rv.width - 16, 8);
			}
			else
			{
				if(m_btnOK)
					m_btnOK.setLocationXY(rv.width/2 -btn_rv.width- 25, rv.height - 75);
				if(m_btnCancel)
					m_btnCancel.setLocationXY(rv.width/2 + 25, rv.height - 75);
			}
			
		}
		public override function DoModal():void
		{
			Create(GlobalCommonMain.getGlobalCommonMain().GetIGameFrameMain());
			super.DoModal();
		}
		public function Create(param0:IMain):int
		{
			m_IMain		   = param0;
			m_ClientSocket = m_IMain.getClientSocket();
			
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;
			
			var nYP:Number = 9;
			var nXP:Number = 842;
			var nCX:Number = 0;
			var nCY:Number = 0;
			m_btnQuery = new SkinButton(IDS_QUERY, nXP, nYP);
			addChild(m_btnQuery);
			m_btnQuery.addEventListener(MouseEvent.CLICK, OnEventQuery);
			m_btnQuery.setFont(font.changeBold(false));
			
			nXP-= 212;
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
			
			m_dfBeginTime.setDateTime(TimeUtil.getQueryBeginTime());
			m_dfEndTime.setDateTime(TimeUtil.getQueryEndTime());
				
			m_btnQuery.setEnabled(MasterRight.CanOperationView(m_IMain.getUserData().dwMasterRight));
		
			m_ClientSocket.AddSocketRecvSink(this as IClientSocketRecvSink);
			
			return 0;
		}
		public override function Destroy():void
		{
			m_ClientSocket.RemoveSocketRecvSink(this as IClientSocketRecvSink);
			
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
			m_tableOperation = null;
			m_scollpaneTable = null;
			removeChild(m_panelTableContainer);
			m_panelTableContainer = null;
			
			m_PageBar.removeEventListener(GotoPageIndexEvent.EVENTNAME, OnEventGotoPageIndex);
			removeChild(m_PageBar);
			m_PageBar = null;

			m_IMain = null;
			m_ClientSocket = null;
			
			super.Destroy();
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
				var szAccount:String = m_IMain.getUserAccount();
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
				
				m_GetOperationData = new CMD_GP_GetOperation;
				m_GetOperationData.dwOperationUserID = m_IMain.getRealUserID();
				m_GetOperationData.wPageSize = wDefaultPageSize;
				m_GetOperationData.szAccount = szAccount;
				m_GetOperationData.cbUserType = 0;
				m_GetOperationData.dwOPType = OPT_All;
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
		//发送函数
		protected function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean
		{
			if(m_ClientSocket == null)
				return false;
			return m_ClientSocket.SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
		}
	}
}
include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/CMD_Plaza.as"
include "../../../Net/Operation/GlobalOperation.as"