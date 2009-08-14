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
	import org.god.Common.UserLevelCell;
	import org.god.Common.Chart.*;
	import org.god.Common.PrintJobUtil;
	import org.god.Net.IClientSocket;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.Chart.*;
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.SkinDateField.DateField;
	import org.god.SkinControl.SkinPageBar.PageBar;
	import org.god.SkinControl.SkinPageBar.GotoPageIndexEvent;
	import org.god.GameManagement.PlazaWindow.Common.ActiveBetHistoryPanelData;
	
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
	//报表查看方式-2页面
	public class ChartViewTwoTab extends JPanel
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		protected static const wDefaultPageSize:uint = 20;

		protected var m_IMain:IMain;
		protected var m_IPlazaWindow:IPlazaWindow;
		protected var m_ClientSocket:IClientSocket;
		
		protected var m_bmToolbar:Bitmap;
		
		protected var m_btnPrint:SkinButton;
		
		protected var m_cbxBetValidType:JComboBox;
		protected var m_modelBetValidType:VectorListModel;
		protected var m_labelBetValidType:JLabel;
		protected var m_cbxBetSortType:JComboBox;
		protected var m_modelBetSortType:VectorListModel;
		protected var m_labelBetSortType:JLabel;
		protected var m_cbxGameKind:JComboBox;
		protected var m_modelGameKind:VectorListModel;
		protected var m_labelGameKind:JLabel;
		protected var m_cbxGameType:JComboBox;
		protected var m_modelGameType:VectorListModel;
		protected var m_labelGameType:JLabel;
		protected var m_cbxLevelName:JComboBox;
		protected var m_modelLevelName:VectorListModel;
		protected var m_labelLevelName:JLabel;
		
		protected var m_tableChart:ChartViewTwoTable;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;

		protected var m_PageBar:PageBar;
		protected var m_GetChartViewTwoData:CMD_GP_GetChartViewTwo;
		protected var m_dwTotalResultCount:uint;

		public function ChartViewTwoTab()
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

			//
			nXP= 926;
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
			
			nXP-= 110;
			nYP = 10+ nYPOffset;
			m_modelBetSortType = new VectorListModel;
			m_cbxBetSortType = new JComboBox(m_modelBetSortType);
			addChild(m_cbxBetSortType);
			m_cbxBetSortType.setComBoundsXYWH(nXP, nYP, 106, 24);

			m_cbxBetSortType.setFont(font.changeBold(false));
			m_cbxBetSortType.setEditable(false);

			nXP-= 38;
			nYP = 12+ nYPOffset;
			m_labelBetSortType = new JLabel(IDS_SORT + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelBetSortType);
			m_labelBetSortType.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelBetSortType.setFont(font);
			m_labelBetSortType.setForeground(MyFontColor);
			
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
			
			nXP-= 76;
			nYP = 10+ nYPOffset;
			m_modelLevelName = new VectorListModel;
			m_cbxLevelName = new JComboBox(m_modelLevelName);
			addChild(m_cbxLevelName);
			m_cbxLevelName.setComBoundsXYWH(nXP, nYP, 72, 24);
			m_cbxLevelName.setFont(font.changeBold(false));
			m_cbxLevelName.setEditable(false);

			nXP-= 68;
			nYP = 12+ nYPOffset;
			m_labelLevelName = new JLabel(IDS_LEVELNAME + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelLevelName);
			m_labelLevelName.setComBoundsXYWH(nXP, nYP, 68, 20);
			m_labelLevelName.setFont(font);
			m_labelLevelName.setForeground(MyFontColor);
			
			nXP	= 2;
			nYP = 42;
			nCX = 1004;
			nCY = 500;
			m_tableChart = new ChartViewTwoTable();
			m_scollpaneTable = new JScrollPane(m_tableChart);
			m_panelTableContainer = new JPanel(new BorderLayout());
			m_panelTableContainer.append(m_scollpaneTable, BorderLayout.CENTER);
			addChild(m_panelTableContainer);
			m_panelTableContainer.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tableChart.MysetComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_panelTableContainer.doLayout();
			m_tableChart.addEventListener(BHDetailTableCellBtnEvent.FTCBTNEVENTNAME, 
										  OnEventFuncCellBtnClick);
			
			m_PageBar = new PageBar;
			addChild(m_PageBar);
			m_PageBar.setFont(font.changeBold(false));
			m_PageBar.addEventListener(GotoPageIndexEvent.EVENTNAME, OnEventGotoPageIndex);
			m_PageBar.setLocationXY(512, 564);
			
			///////////////////////////
			
			var arrGameType:Array = GameTypeAndGameKindUtil.GetGameTypeArray();
			for(var i:uint = 0; i < arrGameType.length; i ++)
				m_modelGameType.append(new GameTypeCell(arrGameType[i]));
			m_cbxGameType.setSelectedIndex(0);
			
			UpdateGameKind();
			
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_BetScoreDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_BetScoreAsc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_WinScoreDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_WinScoreAsc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_ValidScoreDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_ValidScoreAsc));		m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_RollbackDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_RollbackAsc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_RoundCountDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_RoundCountAsc));
			m_cbxBetSortType.setSelectedIndex(0);
			
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Valid));
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Invalid));
			m_cbxBetValidType.setSelectedIndex(0);
			
			for(i = m_IMain.getUserData().cbLevel + 1;
											  i <= nMinUserLevel; i ++)
			{
				m_modelLevelName.append(new UserLevelCell(i));
			}
			m_cbxLevelName.setSelectedIndex(m_modelLevelName.getSize()-1);

			m_dwTotalResultCount = 0;
			
			return 0;
		}
		public function Destroy():void
		{
			m_btnPrint.removeEventListener(MouseEvent.CLICK, OnEventPrint);
			removeChild(m_btnPrint);
			m_btnPrint = null;
			
			removeChild(m_cbxBetValidType);
			m_cbxBetValidType = null;
			m_modelBetValidType = null;
			removeChild(m_labelBetValidType);
			m_labelBetValidType = null;
			removeChild(m_cbxBetSortType);
			m_modelBetSortType = null;
			removeChild(m_labelBetSortType);
			m_labelBetSortType = null;
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
			removeChild(m_cbxLevelName);
			m_cbxLevelName = null;
			m_modelLevelName = null;
			removeChild(m_labelLevelName);
			m_labelLevelName = null;
			
			m_tableChart.removeEventListener(BHDetailTableCellBtnEvent.FTCBTNEVENTNAME, 
										  OnEventFuncCellBtnClick);
			m_tableChart= null;
			m_scollpaneTable = null;
			removeChild(m_panelTableContainer);
			m_panelTableContainer = null;

			m_PageBar.removeEventListener(GotoPageIndexEvent.EVENTNAME, OnEventGotoPageIndex);
			removeChild(m_PageBar);
			m_PageBar = null;

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
		protected function OnEventFuncCellBtnClick(e:BHDetailTableCellBtnEvent):void
		{
			if(m_GetChartViewTwoData == null)
				return;
			var sel:int = m_tableChart.getSelectedRow();
			if(sel == -1)
				return;
			var strSelAccount:String = m_tableChart.getValueAt(sel, 0) as String;
			if(strSelAccount == null || strSelAccount.length <= 0)
				return;

			var ad:ActiveBetHistoryPanelData = new ActiveBetHistoryPanelData;
			ad.szAccount  = strSelAccount;
			ad.fBeginTime = m_GetChartViewTwoData.fBeginTime;
			ad.fEndTime  = m_GetChartViewTwoData.fEndTime;
			ad.wGameType = m_GetChartViewTwoData.wGameType;
			ad.wGameKind = m_GetChartViewTwoData.wGameKind;
			ad.cbGameRoundType = m_GetChartViewTwoData.cbGameRoundType;
			
			m_IPlazaWindow.activePanel(nNavCmd_BetHistory,ad);
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
		protected function OnEventGotoPageIndex(e:GotoPageIndexEvent):void
		{
			if(m_GetChartViewTwoData == null)
				return;
			var dtBeginTime:Date = new Date;
			var dtEndTime:Date = new Date;
			dtBeginTime.setTime(m_GetChartViewTwoData.fBeginTime * 1000);
			dtEndTime.setTime(m_GetChartViewTwoData.fEndTime * 1000);
			QueryResult(dtBeginTime,dtEndTime,e.getPageIndex());
		}
		protected function OnEventSelChangeGameKind(e:InteractiveEvent):void
		{
		
		}
		protected function OnEventSelChangeGameType(e:InteractiveEvent):void
		{
			UpdateGameKind();
		}
		/////////////////////////////////////////////////
		public function QueryResult(dtBeginTime:Date,
									dtEndTime:Date,wPageIndex:uint = 0xffff):void
		{
			if(m_GetChartViewTwoData == null || wPageIndex == 0xffff)
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
				
				var ulc:UserLevelCell = m_cbxLevelName.getSelectedItem() as UserLevelCell;
				var gtc:GameTypeCell = m_cbxGameType.getSelectedItem() as GameTypeCell;
				var gkc:GameKindCell = m_cbxGameKind.getSelectedItem() as GameKindCell;
				var stc:SortTypeCell = m_cbxBetSortType.getSelectedItem() as SortTypeCell;
				var grtc:GameRoundTypeCell = m_cbxBetValidType.getSelectedItem() as GameRoundTypeCell;
				
				m_GetChartViewTwoData = new CMD_GP_GetChartViewTwo;
				m_GetChartViewTwoData.dwOperationUserID = m_IMain.getRealUserID();
				m_GetChartViewTwoData.wPageSize = wDefaultPageSize;
				m_GetChartViewTwoData.szAccount = m_IMain.getUserData().szAccount;
				m_GetChartViewTwoData.cbLevel	   = ulc.getLevel();
				m_GetChartViewTwoData.wGameType    = gtc.getGameType();
				m_GetChartViewTwoData.wGameKind    = gkc.getGameKind();
				m_GetChartViewTwoData.cbSortType   = stc.getSortType();
				m_GetChartViewTwoData.cbGameRoundType   = grtc.getGameRoundType();
				m_GetChartViewTwoData.fBeginTime = dtBeginTime.getTime() / 1000;
				m_GetChartViewTwoData.fEndTime = dtEndTime.getTime() / 1000;
			}
			
			m_tableChart.ClearItem();
			m_IMain.ShowStatusMessage(IDS_STATUS_GETCHART);

			m_GetChartViewTwoData.wPageIndex = wPageIndex;
			
			var cbBuffer:ByteArray = m_GetChartViewTwoData.toByteArray();

			SendData(MDM_GP_GET_RECORD,
					SUB_GP_GET_CHARTVIEWTWO,
					cbBuffer,
					CMD_GP_GetChartViewTwo.sizeof_CMD_GP_GetChartViewTwo);
			
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
		/////////////////////////////////////////////////
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
				case SUB_GP_LIST_CHARTVIEWTWOCONFIG:
				{
					if(wDataSize != CMD_GP_ChartViewTwoListConfig.sizeof_CMD_GP_ChartViewTwoListConfig)return true;
					var pChartViewTwoListConfig:CMD_GP_ChartViewTwoListConfig = CMD_GP_ChartViewTwoListConfig.readData(pBuffer);
					var wMaxPageCount:uint;
					if(pChartViewTwoListConfig.dwTotalResultCount != 0xffffffff)
					{
						wMaxPageCount = Math.ceil(pChartViewTwoListConfig.dwTotalResultCount / pChartViewTwoListConfig.wPageSize);
						m_dwTotalResultCount = pChartViewTwoListConfig.dwTotalResultCount;
					}
					else
						wMaxPageCount = Math.ceil(m_dwTotalResultCount  / pChartViewTwoListConfig.wPageSize);
					m_PageBar.setMaxPageCount(wMaxPageCount);
					m_PageBar.setCurPageIndex(pChartViewTwoListConfig.wPageIndex);
					m_tableChart.ClearItem();
					return true;
				}
				case SUB_GP_LIST_CHARTVIEWTWO:
				{
					if(wDataSize % tagChartViewTwoData.sizeof_tagChartViewTwoData != 0) return true;
					var pChartViewTwoList:CMD_GP_ChartViewTwoList = CMD_GP_ChartViewTwoList.readData(pBuffer,wDataSize);
					m_tableChart.InsertItems(pChartViewTwoList.ChartViewTwoDatas);
					return true;
				}
				case SUB_GP_LIST_CHARTVIEWTWOFINISH:
				{
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_LIST_CHARTVIEWTWOERROR:
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
include "../../../GlobalConst.as"
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../Net/CMD_Plaza.as"