package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	
	import org.god.GameManagement.PlazaWindow.Common.BasePanel;
	
	import org.god.Net.IClientSocket;
	import org.god.Net.BetHistory.*;
	import org.god.Net.CMD_GP_Error;
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
	import org.god.Common.MasterRight;
	import org.god.Common.PrintJobUtil;
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
	
	public class BetHistoryPanel extends BasePanel implements IPanelImpl
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		protected static const wDefaultPageSize:uint = 13;

		protected var m_btnQuery:SkinButton;
		protected var m_dfEndTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelTo:JLabel;
		protected var m_dfBeginTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelDate:JLabel;
		protected var m_cbxAccount:JComboBox;
		protected var m_modelAccount:VectorListModel;
		protected var m_labelAccount:JLabel;
		
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
	
		protected var m_tableBetHistory:*;/*BetHistoryTable*/;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;
		
		protected var m_PageBar:PageBar;
		protected var m_GetBetHistoryData:CMD_GP_GetBetHistory;
		protected var m_dwTotalResultCount:uint;
		
		protected var m_BetHistoryResource:*/*BetHistoryResource*/;
		
		public function BetHistoryPanel()
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
			var nYPOffset:Number = 4;
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
			
			nXP-= 104;
			nYP = 14;
			m_modelAccount = new VectorListModel;
			m_cbxAccount = new JComboBox(m_modelAccount);
			addChild(m_cbxAccount);
			m_cbxAccount.setComBoundsXYWH(nXP, nYP, 100, 24);
			m_cbxAccount.setFont(font.changeBold(false));
			m_cbxAccount.setEditable(false);

			nXP-= 38;
			nYP = 16;
			m_labelAccount = new JLabel(IDS_USER + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelAccount);
			m_labelAccount.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelAccount.setFont(font);
			m_labelAccount.setForeground(MyFontColor);
			//second row
			//nXP = 926;
			
			nXP = 924;
			nYP = 594;
			m_btnPrint = new SkinButton(IDS_PRINT, nXP, nYP, null, 92,32);
			addChild(m_btnPrint);
			m_btnPrint.addEventListener(MouseEvent.CLICK, OnEventPrint);
			m_btnPrint.setFont(font.changeBold(false));
			
			nXP = 936;
			nYP = 40+ nYPOffset;
			m_modelBetValidType = new VectorListModel;
			m_cbxBetValidType = new JComboBox(m_modelBetValidType);
			addChild(m_cbxBetValidType);
			m_cbxBetValidType.setComBoundsXYWH(nXP, nYP, 74, 24);
			m_cbxBetValidType.setFont(font.changeBold(false));
			m_cbxBetValidType.setEditable(false);

			nXP-= 88;
			nYP = 42+ nYPOffset;
			m_labelBetValidType = new JLabel(IDS_VALID + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelBetValidType);
			m_labelBetValidType.setComBoundsXYWH(nXP, nYP, 86, 20);
			m_labelBetValidType.setFont(font);
			m_labelBetValidType.setForeground(MyFontColor);
			
			nXP-= 94;
			nYP = 40+ nYPOffset;
			m_modelBetSortType = new VectorListModel;
			m_cbxBetSortType = new JComboBox(m_modelBetSortType);
			addChild(m_cbxBetSortType);
			m_cbxBetSortType.setComBoundsXYWH(nXP, nYP, 90, 24);
			m_cbxBetSortType.setFont(font.changeBold(false));
			m_cbxBetSortType.setEditable(false);

			nXP-= 38;
			nYP = 42+ nYPOffset;
			m_labelBetSortType = new JLabel(IDS_SORT + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelBetSortType);
			m_labelBetSortType.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelBetSortType.setFont(font);
			m_labelBetSortType.setForeground(MyFontColor);
			
			nXP-= 109;
			nYP = 40+ nYPOffset;
			m_modelGameKind = new VectorListModel;
			m_cbxGameKind = new JComboBox(m_modelGameKind);
			addChild(m_cbxGameKind);
			m_cbxGameKind.setComBoundsXYWH(nXP, nYP, 105, 24);
			m_cbxGameKind.setFont(font.changeBold(false));
			m_cbxGameKind.setEditable(false);
			m_cbxGameKind.addSelectionListener(OnEventSelChangeGameKind);

			nXP-= 67;
			nYP = 42+ nYPOffset;
			m_labelGameKind = new JLabel(IDS_GAMEKIND + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelGameKind);
			m_labelGameKind.setComBoundsXYWH(nXP, nYP, 67, 20);
			m_labelGameKind.setFont(font);
			m_labelGameKind.setForeground(MyFontColor);
			
			
			nXP-= 76;
			nYP = 40+ nYPOffset;
			m_modelGameType = new VectorListModel;
			m_cbxGameType = new JComboBox(m_modelGameType);
			addChild(m_cbxGameType);
			m_cbxGameType.setComBoundsXYWH(nXP, nYP, 72, 24);
			m_cbxGameType.setFont(font.changeBold(false));
			m_cbxGameType.setEditable(false);
			m_cbxGameType.addSelectionListener(OnEventSelChangeGameType);

			nXP-= 68;
			nYP = 42+ nYPOffset;
			m_labelGameType = new JLabel(IDS_GAMETYPE + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelGameType);
			m_labelGameType.setComBoundsXYWH(nXP, nYP, 68, 20);
			m_labelGameType.setFont(font);
			m_labelGameType.setForeground(MyFontColor);

			m_PageBar = new PageBar;
			addChild(m_PageBar);
			m_PageBar.setFont(font.changeBold(false));
			m_PageBar.addEventListener(GotoPageIndexEvent.EVENTNAME, OnEventGotoPageIndex);
			m_PageBar.setLocationXY(512, 608);
			
			CheckBetHistoryTable();
			
			///////////////////////////////////////////////////////////////
			
			m_modelAccount.append(m_IMain.getUserData().szAccount);
			m_cbxAccount.setSelectedIndex(0);
			
			m_dfBeginTime.setDateTime(TimeUtil.getQueryBeginTime());
			m_dfEndTime.setDateTime(TimeUtil.getQueryEndTime());
			
			var arrGameType:Array = GameTypeAndGameKindUtil.GetGameTypeArray();
			for(var i:uint = 0; i < arrGameType.length; i ++)
				m_modelGameType.append(new GameTypeCell(arrGameType[i]));
			m_cbxGameType.setSelectedIndex(0);
			
			UpdateGameKind();
			
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_TimeDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_BetScoreDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_BetScoreAsc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_WinScoreDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_WinScoreAsc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_RollbackDesc));
			m_modelBetSortType.append(new SortTypeCell(SortTypeUtil.SORTTYPE_RollbackAsc));
			m_cbxBetSortType.setSelectedIndex(0);
			
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Valid));
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Invalid));
			m_cbxBetValidType.setSelectedIndex(0);
			
			m_btnQuery.setEnabled(MasterRight.CanBetHistoryView(m_IMain.getUserData().dwMasterRight));

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
			
			removeChild(m_cbxAccount);
			m_cbxAccount = null;
			m_modelAccount = null;
			removeChild(m_labelAccount);
			m_labelAccount = null;
			
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
	
			if(m_tableBetHistory != null)
			{
				m_tableBetHistory = null;
				m_scollpaneTable = null;
				removeChild(m_panelTableContainer);
				m_panelTableContainer = null;
			
				m_BetHistoryResource.Destroy();
				m_BetHistoryResource = null;
			}
			
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
			return nNavCmd_BetHistory;
		}
		public function getTitle():String
		{
			return IDS_PANELTITLE_BETHISTORY;
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
				
				var dtBeginTime:Date = new Date;
				var dtEndTime:Date = new Date;
				dtBeginTime.setTime(data.fBeginTime * 1000);
				dtEndTime.setTime(data.fEndTime * 1000);
				if(TimeUtil.getTimeOffsetDates(dtBeginTime, dtEndTime) > 3)
				{
					dtBeginTime = TimeUtil.getTimeBeforeDayOffset(dtEndTime, 3);
				}
				m_dfBeginTime.setDateTime(dtBeginTime);
				m_dfEndTime.setDateTime(dtEndTime);
				
				var wGameType:uint = data.wGameType;
				for( i= 0; i < m_modelGameType.getSize(); i ++)
				{
					if(m_modelGameType.get(i).getGameType() == wGameType)
					{
						m_cbxGameType.setSelectedIndex(i);
						break;
					}
				}
			
				var wGameKind:uint = data.wGameKind;
				for( i= 0; i < m_modelGameKind.getSize(); i ++)
				{
					if(m_modelGameKind.get(i).getGameKind() == wGameKind)
					{
						m_cbxGameKind.setSelectedIndex(i);
						break;
					}
				}
				
				var cbGameRoundType:uint = data.cbGameRoundType;
				for( i= 0; i < m_modelBetValidType.getSize(); i ++)
				{
					if(m_modelBetValidType.get(i).getGameRoundType() == cbGameRoundType)
					{
						m_cbxBetValidType.setSelectedIndex(i);
						break;
					}
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
		private function GetCommonClass(strName:String):Class
		{
			var c:Class = m_IMain.getDefClass(strName,"BetHistoryPanel_Common");
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
		protected function OnEventPrint(e:MouseEvent):void
		{
			if(CheckBetHistoryTable() == false || m_tableBetHistory == null)
				return;
			if(PrintJobUtil.PrintSprite(m_tableBetHistory, 
									 0,
									 0,
									 m_tableBetHistory.getSize().width,
									 m_tableBetHistory.getSize().height) == false)
			{
				m_IMain.ShowMessageBox(IDS_ERR_PRINT);
			}
		}
		protected function OnEventGotoPageIndex(e:GotoPageIndexEvent):void
		{
			QueryResult(e.getPageIndex());
		}
		protected function OnEventSelChangeGameKind(e:InteractiveEvent):void
		{
		
		}
		protected function OnEventSelChangeGameType(e:InteractiveEvent):void
		{
			UpdateGameKind();
		}
		/////////////////////////////////////////////////
		protected function QueryResult(wPageIndex:uint = 0xffff):void
		{
			if(CheckBetHistoryTable() == false || m_tableBetHistory == null)
				return;
			if(m_GetBetHistoryData == null || wPageIndex == 0xffff)
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
				if(TimeUtil.getTimeOffsetDates(BeginTime, EndTime) > 3)
				{
					m_IMain.ShowMessageBox(IDS_ERR_MAXQUERYDATE3);
					return ;
				}
				
				var gtc:GameTypeCell = m_cbxGameType.getSelectedItem() as GameTypeCell;
				var gkc:GameKindCell = m_cbxGameKind.getSelectedItem() as GameKindCell;
				var stc:SortTypeCell = m_cbxBetSortType.getSelectedItem() as SortTypeCell;
				var grtc:GameRoundTypeCell = m_cbxBetValidType.getSelectedItem() as GameRoundTypeCell;
				
				m_GetBetHistoryData = new CMD_GP_GetBetHistory;
				m_GetBetHistoryData.dwOperationUserID = m_IMain.getRealUserID();
				m_GetBetHistoryData.wPageSize = wDefaultPageSize;
				m_GetBetHistoryData.szAccount = szAccount;
				m_GetBetHistoryData.wGameType    = gtc.getGameType();
				m_GetBetHistoryData.wGameKind    = gkc.getGameKind();
				m_GetBetHistoryData.cbSortType   = stc.getSortType();
				m_GetBetHistoryData.cbGameRoundType   = grtc.getGameRoundType();
				m_GetBetHistoryData.fBeginTime = BeginTime.getTime() / 1000;
				m_GetBetHistoryData.fEndTime = EndTime.getTime() / 1000;
			}
			m_tableBetHistory.ClearItem();
			m_IMain.ShowStatusMessage(IDS_STATUS_GETBETHISTORY);

			m_GetBetHistoryData.wPageIndex = wPageIndex;
			
			var cbBuffer:ByteArray = m_GetBetHistoryData.toByteArray();

			SendData(MDM_GP_GET_RECORD,
					SUB_GP_GET_BETHISTORY,
					cbBuffer,
					CMD_GP_GetBetHistory.sizeof_CMD_GP_GetBetHistory);
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
		////////////////////////////////////////
		public function OnSocketGetRecord(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if(CheckBetHistoryTable() == false || m_tableBetHistory == null)
				return true;
			switch(wSubCmdID)
			{
				case SUB_GP_LIST_BETHISTORYCONFIG:
				{
					if(wDataSize != CMD_GP_BetHistoryListConfig.sizeof_CMD_GP_BetHistoryListConfig)return true;
					var pBetHistoryListConfig:CMD_GP_BetHistoryListConfig = CMD_GP_BetHistoryListConfig.readData(pBuffer);
					var wMaxPageCount:uint;
					if(pBetHistoryListConfig.dwTotalResultCount != 0xffffffff)
					{
						wMaxPageCount = Math.ceil(pBetHistoryListConfig.dwTotalResultCount / pBetHistoryListConfig.wPageSize);
						m_dwTotalResultCount = pBetHistoryListConfig.dwTotalResultCount;
					}
					else
						wMaxPageCount = Math.ceil(m_dwTotalResultCount  / pBetHistoryListConfig.wPageSize);
					m_PageBar.setMaxPageCount(wMaxPageCount);
					m_PageBar.setCurPageIndex(pBetHistoryListConfig.wPageIndex);
					m_tableBetHistory.ClearItem();
					m_tableBetHistory.SetCurAccount(m_GetBetHistoryData.szAccount);
					return true;
				}
				case SUB_GP_LIST_BETHISTORY:
				{
					var pBetHistoryList:CMD_GP_BetHistoryList = CMD_GP_BetHistoryList.readData(pBuffer,wDataSize);
					m_tableBetHistory.InsertItems(pBetHistoryList.BetHistoryDatas);
					return true;
				}
				case SUB_GP_LIST_BETHISTORYFINISH:
				{
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_LIST_BETHISTORYERROR:
				{
					m_IMain.HideStatusMessage();
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
					
					return true;
				}
			}
			return false;
		}
		private function CheckBetHistoryTable():Boolean
		{
			if(m_tableBetHistory == null)
			{
				var col:ASColor = MyFontColor;
				var font:ASFont = MyFont;
			
				var nYP:Number = 9;
				var nXP:Number = 924;
				var nCX:Number = 0;
				var nCY:Number = 0;
				var nYPOffset:Number = 4;
			
				var classBetHistoryResource:Class = GetCommonClass("BetHistoryResource");
				if(classBetHistoryResource == null)
					return false;
				m_BetHistoryResource = new classBetHistoryResource;
				m_BetHistoryResource.Create();
			
				nXP	= 10;
				nYP = 70;
				nCX = 1004;
				nCY = 516;
				var classBetHistoryTable:Class = GetCommonClass("BetHistoryTable");
				if(classBetHistoryTable == null)
					return false;
				m_tableBetHistory = new classBetHistoryTable(m_ClientSocket,
													m_BetHistoryResource,
													IMAGE_GAMEROUNDDETAILDLG,
													968,691);
				m_scollpaneTable = new JScrollPane(m_tableBetHistory);
				m_panelTableContainer = new JPanel(new BorderLayout());
				m_panelTableContainer.append(m_scollpaneTable, BorderLayout.CENTER);
				addChild(m_panelTableContainer);
				m_panelTableContainer.setComBoundsXYWH(nXP, nYP, nCX, nCY);
				m_tableBetHistory.MysetComBoundsXYWH(nXP, nYP, nCX, nCY);
				m_panelTableContainer.doLayout();
				
				return true;
			}
			else
			{
				return true;
			}
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/CMD_Plaza.as"