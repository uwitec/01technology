package org.god.Common.BetHistory
{
	import org.god.Net.ClientSocket;
	import org.god.Net.BetHistory.tagBetHistoryData;
	import org.god.Common.FuncUtil;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.NumericTableCell;
	import org.god.Common.GameUserStatusUtil;
	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.JTable;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	//游戏记录表
	public class BetHistoryTable extends JTable
	{
		public static var column:Array = [IDS_BHGAMEROUNDID, 
							IDS_BHTIME, 
							IDS_BHGAMEKIND, 
							IDS_BHPOSITION,
							IDS_BHGAMERESULT,
							IDS_BHBETSCORE,
							IDS_BHWINSCORE,
							IDS_BHTAXSCORE,
							IDS_BHROLLBACKSCORE,
							IDS_BHDETAIL];		
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_modelBetHistory:DefaultTableModel;

		protected var m_ClientSocket:ClientSocket;
		protected var m_bmGHDlgBackgroundClass:Class;
		protected var m_nGHDlgBackgroundWidth:int;
		protected var m_nGHDlgBackgroundHeight:int;
		protected var m_IBetHistoryResource:IBetHistoryResource;
		protected var m_strCurAccount:String ="";
		
		public function BetHistoryTable(pClientSocket:ClientSocket,
										pIBetHistoryResource:IBetHistoryResource,
										bmGHDlgBackgroundClass:Class,
										nGHDlgBackgroundWidth:int,
										nGHDlgBackgroundHeight:int)
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(false);
			}
			m_ClientSocket = pClientSocket;
			m_bmGHDlgBackgroundClass = bmGHDlgBackgroundClass;
			m_nGHDlgBackgroundWidth = nGHDlgBackgroundWidth;
			m_nGHDlgBackgroundHeight= nGHDlgBackgroundHeight;
			m_IBetHistoryResource = pIBetHistoryResource;
			
			m_modelBetHistory = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			var sorter:BetHistoryTableSorter = new BetHistoryTableSorter(m_modelBetHistory);
			super(sorter);
			sorter.setTableHeader(this.getTableHeader());
			for(var i:uint = 0; i < column.length; i ++)
			{
				m_modelBetHistory.setColumnEditable(i, false);
			}
			setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			setBackground(new ASColor(0xffffed));
			setForeground(MyFontColor);
			setFont(MyFont);
			getTableHeader().setBackground(new ASColor(0xF7E444));
			getTableHeader().setForeground(MyFontColor);
			getTableHeader().setFont(MyFont);
			setRowHeight(38);
			
			getColumn(IDS_BHGAMERESULT).setCellFactory(new GeneralTableCellFactory(BHGameResultTableCell));
			getColumn(IDS_BHBETSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_BHWINSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_BHTAXSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_BHROLLBACKSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_BHDETAIL).setCellFactory(new GeneralTableCellFactory(BHDetailTableCell));
		
			addEventListener(BHDetailTableCellBtnEvent.FTCBTNEVENTNAME, OnEventBHDetail);
		}
		
		public function MysetComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);
			getColumn(IDS_BHGAMEROUNDID).setPreferredWidth(w * 0.06);
			getColumn(IDS_BHTIME).setPreferredWidth(w * 0.15);
			getColumn(IDS_BHGAMEKIND).setPreferredWidth(w * 0.08);
			getColumn(IDS_BHPOSITION).setPreferredWidth(w * 0.12);
			getColumn(IDS_BHGAMERESULT).setPreferredWidth(w * 0.20);
			getColumn(IDS_BHBETSCORE).setPreferredWidth(w * 0.08);
			getColumn(IDS_BHWINSCORE).setPreferredWidth(w * 0.08);
			getColumn(IDS_BHTAXSCORE).setPreferredWidth(w * 0.1);
			getColumn(IDS_BHROLLBACKSCORE).setPreferredWidth(w * 0.05);
			getColumn(IDS_BHDETAIL).setPreferredWidth(w * 0.08);
		}
		public function SetCurAccount(strAccount:String):void
		{
			m_strCurAccount = strAccount;
		}
		public function ClearItem():void
		{
			m_modelBetHistory.setRowCount(0);
		}
		public function InsertItems(BetHistoryDatas:Array):void
		{
			for(var i:uint = 0; i < BetHistoryDatas.length; i ++)
			{
				InsertItem(BetHistoryDatas[i]);
			}
		}
		public function InsertItem(bh:tagBetHistoryData):void
		{					
			var tableData:Array = [bh.fGameRoundID.toFixed(0), 
								   FuncUtil.fu_GetDateTimeString(bh.fStartTime),
								   GameModuleNameFactory.GetGameKindPrixName(bh.wProcessType), 
								   bh.szTypeName + "-" + bh.szKindName + "-\n" +
								   bh.szServerName + "-" + GameUserStatusUtil.GetTableIDName(bh.wTableID), 
								   
								   bh,
								   
								   bh.fTotalBetScore,
								   bh.fTotalWinScore,
								   -bh.fTotalTaxScore,
								   bh.fValidScore_Less_Rollback,
								   
								   ""
								   ];
					
			m_modelBetHistory.addRow(tableData);
		}
		///////////////////////////////////////////////
		protected function OnEventBHDetail(e:BHDetailTableCellBtnEvent):void
		{
			var sel:int = this.getSelectedRow();
			if(sel == -1)
				return;
			var bh:tagBetHistoryData = m_modelBetHistory.getValueAt(sel, 
										column.indexOf(IDS_BHGAMERESULT))  as tagBetHistoryData;
			if(bh == null)
				return;
			var dlg:BHGameRoundDetailDlg = new BHGameRoundDetailDlg(m_strCurAccount,
																	bh,
																	m_ClientSocket,
																	m_IBetHistoryResource,
																	new m_bmGHDlgBackgroundClass(m_nGHDlgBackgroundWidth,m_nGHDlgBackgroundHeight));
			dlg.DoModal();
		}
	}
}
include "./StringTableDef.as"