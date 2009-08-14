package
{
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	import flash.display.Bitmap;
	
	import org.god.IGameManagement.*;
	import org.god.Common.DrawUtil;
	import org.god.Common.TimeUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.GameTypeAndGameKindUtil;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.SortTypeUtil;
	import org.god.Common.GameRoundTypeUtil;
	import org.god.Common.GameTypeCell;
	import org.god.Common.SortTypeCell;
	import org.god.Common.GameRoundTypeCell;
	import org.god.Common.GameKindCell;
	import org.god.Common.Chart.*;
	import org.god.Common.MasterRight;
	import org.god.Common.PrintJobUtil;
	import org.god.Net.IClientSocket;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.Chart.*;
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
	import org.aswing.event.InteractiveEvent;
	import org.aswing.event.AWEvent;
	import org.aswing.event.ClickCountEvent;
	//报表查看方式-1页面
	public class ChartViewOneTab extends JPanel
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		protected static const wDefaultPageSize:uint = 20;

		protected var m_IMain:IMain;
		protected var m_IPlazaWindow:IPlazaWindow;
		protected var m_ClientSocket:IClientSocket;
		
		protected var m_bmToolbar:Bitmap;
		
		protected var m_btnReturnParentLevel:SkinButton;
		protected var m_btnPrint:SkinButton;
		
		protected var m_labelQueryAccount:JLabel;
		
		protected var m_cbxBetValidType:JComboBox;
		protected var m_modelBetValidType:VectorListModel;
		protected var m_labelBetValidType:JLabel;
		protected var m_cbxGameKind:JComboBox;
		protected var m_modelGameKind:VectorListModel;
		protected var m_labelGameKind:JLabel;
		protected var m_cbxGameType:JComboBox;
		protected var m_modelGameType:VectorListModel;
		protected var m_labelGameType:JLabel;
		
		protected var m_tableChart:ChartViewOneTable;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;
		
		protected var m_GetChartViewOneData:CMD_GP_GetChartViewOne;
		protected var m_strCurrentAccount:String = "";
		protected var m_cbCurrentLevel:uint;

		public function ChartViewOneTab()
		{
			super();
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
			
			var nYP:Number = 10;
			var nXP:Number = 910;
			var nCX:Number = 0;
			var nCY:Number = 0;
			var nYPOffset:Number = -4;
			
			m_bmToolbar = new Bitmap(new IMAGE_TOOLBAR(998,42));
			addChild(m_bmToolbar);
			m_bmToolbar.y = -3;
			m_bmToolbar.x = 0;
			
			nYP = 548;
			m_btnPrint = new SkinButton(IDS_PRINT, nXP, nYP, null, 92,32);
			addChild(m_btnPrint);
			m_btnPrint.addEventListener(MouseEvent.CLICK, OnEventPrint);
			m_btnPrint.setFont(font.changeBold(false));
			
			nXP = 5;
			nYP = 12+ nYPOffset;
			m_labelQueryAccount = new JLabel("", null, JLabel.LEFT);
			addChild(m_labelQueryAccount);
			m_labelQueryAccount.setComBoundsXYWH(nXP, nYP, 280, 20);
			m_labelQueryAccount.setFont(font);
			m_labelQueryAccount.setForeground(MyFontColor);
			
			nXP = 910;
			nYP = 6 + nYPOffset;
			m_btnReturnParentLevel = new SkinButton(IDS_RESULTPARENTLEVEL, nXP, nYP, null, 92,32);
			addChild(m_btnReturnParentLevel);
			m_btnReturnParentLevel.addEventListener(MouseEvent.CLICK, OnEventReturnParentLevel);
			m_btnReturnParentLevel.setFont(font.changeBold(false));

			//
			nXP-= 72;
			nYP = 10+ nYPOffset;
			m_modelBetValidType = new VectorListModel;
			m_cbxBetValidType = new JComboBox(m_modelBetValidType);
			addChild(m_cbxBetValidType);
			m_cbxBetValidType.setComBoundsXYWH(nXP, nYP, 70, 24);
			m_cbxBetValidType.setFont(font.changeBold(false));
			m_cbxBetValidType.setEditable(false);

			nXP-= 88;
			nYP = 12+ nYPOffset;
			m_labelBetValidType = new JLabel(IDS_VALID + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelBetValidType);
			m_labelBetValidType.setComBoundsXYWH(nXP, nYP, 86, 20);
			m_labelBetValidType.setFont(font);
			m_labelBetValidType.setForeground(MyFontColor);
			
			nXP-= 102;
			nYP = 10+ nYPOffset;
			m_modelGameKind = new VectorListModel;
			m_cbxGameKind = new JComboBox(m_modelGameKind);
			addChild(m_cbxGameKind);
			m_cbxGameKind.setComBoundsXYWH(nXP, nYP, 98, 24);
			m_cbxGameKind.setFont(font.changeBold(false));
			m_cbxGameKind.setEditable(false);
			m_cbxGameKind.addSelectionListener(OnEventSelChangeGameKind);

			nXP-= 67;
			nYP = 12+ nYPOffset;
			m_labelGameKind = new JLabel(IDS_GAMEKIND + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelGameKind);
			m_labelGameKind.setComBoundsXYWH(nXP, nYP, 67, 20);
			m_labelGameKind.setFont(font);
			m_labelGameKind.setForeground(MyFontColor);
			
			nXP-= 76;
			nYP = 10+ nYPOffset;
			m_modelGameType = new VectorListModel;
			m_cbxGameType = new JComboBox(m_modelGameType);
			addChild(m_cbxGameType);
			m_cbxGameType.setComBoundsXYWH(nXP, nYP, 72, 24);
			m_cbxGameType.setFont(font.changeBold(false));
			m_cbxGameType.setEditable(false);
			m_cbxGameType.addSelectionListener(OnEventSelChangeGameType);

			nXP-= 68;
			nYP = 12+ nYPOffset;
			m_labelGameType = new JLabel(IDS_GAMETYPE + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelGameType);
			m_labelGameType.setComBoundsXYWH(nXP, nYP, 68, 20);
			m_labelGameType.setFont(font);
			m_labelGameType.setForeground(MyFontColor);
			
			nXP	= 2;
			nYP = 42;
			nCX = 1004;
			nCY = 506;
			m_tableChart = new ChartViewOneTable();
			m_scollpaneTable = new JScrollPane(m_tableChart);
			m_panelTableContainer = new JPanel(new BorderLayout());
			m_panelTableContainer.append(m_scollpaneTable, BorderLayout.CENTER);
			addChild(m_panelTableContainer);
			m_panelTableContainer.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tableChart.MysetComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tableChart.ExtendCategoriseColumn(2,false);
			m_tableChart.ExtendCategoriseColumn(3,false);
			m_tableChart.ExtendCategoriseColumn(4,false);
			m_panelTableContainer.doLayout();
			m_tableChart.addEventListener(ClickCountEvent.CLICK_COUNT, OnEventTableClicked);

			///////////////////////////
			
			var arrGameType:Array = GameTypeAndGameKindUtil.GetGameTypeArray();
			for(var i:uint = 0; i < arrGameType.length; i ++)
				m_modelGameType.append(new GameTypeCell(arrGameType[i]));
			m_cbxGameType.setSelectedIndex(0);
			
			UpdateGameKind();
			
			
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Valid));
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Invalid));
			m_cbxBetValidType.setSelectedIndex(0);
			
			m_GetChartViewOneData = new CMD_GP_GetChartViewOne;
			m_GetChartViewOneData.dwOperationUserID = m_IMain.getRealUserID();
			setCurrentAccount(m_IMain.getUserData().szAccount,m_IMain.getUserData().cbLevel)

			return 0;
		}
		public function Destroy():void
		{
			m_btnReturnParentLevel.removeEventListener(MouseEvent.CLICK, OnEventReturnParentLevel);
			removeChild(m_btnReturnParentLevel);
			m_btnReturnParentLevel = null;
			m_btnPrint.removeEventListener(MouseEvent.CLICK, OnEventPrint);
			removeChild(m_btnPrint);
			m_btnPrint = null;
			
			removeChild(m_labelQueryAccount);
			m_labelQueryAccount = null;

			removeChild(m_cbxBetValidType);
			m_cbxBetValidType = null;
			m_modelBetValidType = null;
			removeChild(m_labelBetValidType);
			m_labelBetValidType = null;
			m_cbxGameKind.removeSelectionListener(OnEventSelChangeGameKind);
			removeChild(m_cbxGameKind);
			m_cbxGameKind = null;
			m_modelGameKind = null;
			removeChild(m_labelGameKind);
			m_labelGameKind = null;
			m_cbxGameType.removeSelectionListener(OnEventSelChangeGameType);
			removeChild(m_cbxGameType);
			m_cbxGameType = null;
			m_modelGameType = null;
			removeChild(m_labelGameType);
			m_labelGameType = null;
	
			m_tableChart.removeEventListener(ClickCountEvent.CLICK_COUNT, OnEventTableClicked);
			m_tableChart= null;
			m_scollpaneTable = null;
			removeChild(m_panelTableContainer);
			m_panelTableContainer = null;
			
			removeChild(m_bmToolbar);
			m_bmToolbar.bitmapData.dispose();
			m_bmToolbar = null;
			
			m_IMain = null;
		 	m_IPlazaWindow = null;
			m_ClientSocket = null;

		}
		protected function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean
		{
			if(m_ClientSocket == null)
				return false;
			return m_ClientSocket.SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
		}
		//////////////////////////////////////////////
		protected function OnEventReturnParentLevel(e:MouseEvent):void
		{
			var dtBeginTime:Date = new Date;
			var dtEndTime:Date = new Date;
			dtBeginTime.setTime(m_GetChartViewOneData.fBeginTime * 1000);
			dtEndTime.setTime(m_GetChartViewOneData.fEndTime * 1000);
			QueryResult(dtBeginTime,dtEndTime,1);
		}
		protected function OnEventPrint(e:MouseEvent):void
		{
			if(PrintJobUtil.PrintSprite(m_tableChart, 
									 0,
									 0,
									 m_tableChart.getSize().width,
									 m_tableChart.getSize().height) == false)
			{
				m_IMain.ShowMessageBox(IDS_ERR_PRINT);
			}
		}
		protected function OnEventTableClicked(e:ClickCountEvent):void
		{
			if(e.getCount() == 2)
			{
				if(m_cbCurrentLevel < nMinUserLevel - 1)
				{
					var sel:int = m_tableChart.getSelectedRow();
					if(sel == -1)
						return;
					var cd:tagChartViewOneData = m_tableChart.getValueAt(sel, 0) as tagChartViewOneData;
					if(cd == null)
						return;
					var strSelAccount:String = cd.szAccount;
					if(strSelAccount.length <= 0)
						return;
					if(m_IMain.getUserData().cbLevel == 1 && m_cbCurrentLevel == 0)
					{
						if(strSelAccount != m_IMain.getUserAccount())
							return;
					}
					setCurrentAccount(strSelAccount,
									 m_cbCurrentLevel + 1)
					var dtBeginTime:Date = new Date;
					var dtEndTime:Date = new Date;
					dtBeginTime.setTime(m_GetChartViewOneData.fBeginTime * 1000);
					dtEndTime.setTime(m_GetChartViewOneData.fEndTime * 1000);
					QueryResult(dtBeginTime,dtEndTime,0);
				}
			}
		}
		protected function OnEventSelChangeGameKind(e:InteractiveEvent):void
		{
		
		}
		protected function OnEventSelChangeGameType(e:InteractiveEvent):void
		{
			UpdateGameKind();
		}
		public function QueryResult(dtBeginTime:Date,
									dtEndTime:Date,
									cbParentFlag:uint = 0):void
		{
			if(dtBeginTime.getTime() >= dtEndTime.getTime())
			{
				m_IMain.ShowMessageBox(IDS_ERR_INVALIDQUERYTIME);
				return ;
			}
			if(TimeUtil.getTimeOffsetDates(dtBeginTime, dtEndTime) > 31)
			{
				m_IMain.ShowMessageBox(IDS_ERR_MAXQUERYDATE31);
				return ;
			}
			if(m_strCurrentAccount.length <= 0)
			{
				setCurrentAccount(m_IMain.getUserData().szAccount,m_IMain.getUserData().cbLevel)
			}
			
			m_tableChart.ClearItem();
			m_IMain.ShowStatusMessage(IDS_STATUS_GETCHART);
			
			var gtc:GameTypeCell = m_cbxGameType.getSelectedItem() as GameTypeCell;
			var gkc:GameKindCell = m_cbxGameKind.getSelectedItem() as GameKindCell;
			var grtc:GameRoundTypeCell = m_cbxBetValidType.getSelectedItem() as GameRoundTypeCell;

			m_GetChartViewOneData.cbParentFlag = cbParentFlag;
			m_GetChartViewOneData.szAccount    = m_strCurrentAccount;
			m_GetChartViewOneData.fBeginTime   = dtBeginTime.getTime() / 1000;
			m_GetChartViewOneData.fEndTime     = dtEndTime.getTime() / 1000;
			m_GetChartViewOneData.wGameType    = gtc.getGameType();
			m_GetChartViewOneData.wGameKind    = gkc.getGameKind();
			m_GetChartViewOneData.cbSortType   = 0;
			m_GetChartViewOneData.cbGameRoundType   = grtc.getGameRoundType();
			
			var cbBuffer:ByteArray = m_GetChartViewOneData.toByteArray();

			SendData(MDM_GP_GET_RECORD,
					SUB_GP_GET_CHARTVIEWONE,
					cbBuffer,
					CMD_GP_GetChartViewOne.sizeof_CMD_GP_GetChartViewOne);
		}
		/////////////////////////////////////////////////
		protected function setCurrentAccount(strCurrentAccount:String, cbCurrentLevel:uint)
		{
			m_strCurrentAccount = strCurrentAccount;
			m_cbCurrentLevel 	= cbCurrentLevel;
			
			m_labelQueryAccount.setText(IDS_QUERYACCOUNT + IDS_COLON + m_strCurrentAccount);
			m_tableChart.SetLevelName(FuncUtil.fu_GetPeerLevelName(cbCurrentLevel + 1));
			
			var b:Boolean = MasterRight.CanChartView(m_IMain.getUserData().dwMasterRight);
			if(b == false)//权限不够
			{
			}
			else
			{
				if(m_cbCurrentLevel == 0)//公司级别
				{
					b = false;
				}
				else
				{
					if(m_IMain.getUserData().cbLevel < 2)//大股东级别
					{
						b = true;
					}
					else
					{
						if(m_cbCurrentLevel > m_IMain.getUserData().cbLevel)
							b = true;
						else
							b = false;
					}
				}
			}
			m_btnReturnParentLevel.setEnabled(b);

		}
		protected function UpdateGameKind():void
		{
			var gtc:GameTypeCell = m_cbxGameType.getSelectedItem() as GameTypeCell;
			var arrGameKind:Array = GameTypeAndGameKindUtil.GetGameKindArray(gtc.getGameType());
			m_modelGameKind.clear();
			for(var i:uint = 0; i < arrGameKind.length; i ++)
			{
				m_modelGameKind.append(new GameKindCell(arrGameKind[i]));
			}
			m_cbxGameKind.setSelectedIndex(0);
		}
		protected function UpdateGameType():void
		{
			var gkc:GameKindCell = m_cbxGameKind.getSelectedItem() as GameKindCell;
			var wGameType:uint = GameTypeAndGameKindUtil.GetGameTypeByGameKind(gkc.getGameKind());
			if(wGameType == 0)
				return ;
			for(var i:uint = 0; i < m_modelGameType.getSize(); i ++)
			{
				if(m_modelGameType.get(i).getGameType() == wGameType)
				{
					m_cbxGameType.setSelectedIndex(i);
					break;
				}
			}
		}
		//////////////////////////////////////////
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
		public function OnSocketGetRecord(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_LIST_CHARTVIEWONECONFIG:
				{
					if(wDataSize != CMD_GP_ChartViewOneListConfig.sizeof_CMD_GP_ChartViewOneListConfig)return true;
					var pChartViewOneListConfig:CMD_GP_ChartViewOneListConfig = CMD_GP_ChartViewOneListConfig.readData(pBuffer);
					setCurrentAccount(pChartViewOneListConfig.szAccount,
									 pChartViewOneListConfig.cbLevel)
					m_tableChart.ClearItem();
					return true;
				}
				case SUB_GP_LIST_CHARTVIEWONE:
				{
					if(wDataSize % tagChartViewOneData.sizeof_tagChartViewOneData != 0) return true;
					var pChartViewOneList:CMD_GP_ChartViewOneList = CMD_GP_ChartViewOneList.readData(pBuffer,wDataSize);
					m_tableChart.InsertItems(pChartViewOneList.ChartViewOneDatas);
					return true;
				}
				case SUB_GP_LIST_CHARTVIEWONEFINISH:
				{
					m_tableChart.FinishInsertItems();
					m_IMain.HideStatusMessage();
					
					return true;
				}
				case SUB_GP_LIST_CHARTVIEWONEERROR:
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
include "../../../GlobalConst.as"
include "../../../Common/StringTableDef.as"
include "../../../Net/CMD_Plaza.as"