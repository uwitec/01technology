package org.god.Common.Chart
{
	import org.god.Net.Chart.tagChartViewTwoData;
	import org.god.Common.NumericTableCell;
	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.JTable;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	//游戏报表方式-2
	public class ChartViewTwoTable extends JTable
	{
		public static var column:Array = [IDS_CHACCOUNT, 
							IDS_CHROUNDCOUNT, 
							IDS_CHBET, 
							IDS_CHWINSCORE,
							IDS_CHTAXSCORE,
							IDS_CHVALIDSCORE_REAL,
							IDS_CHVALIDSCORE_ROLLBACK,
							IDS_BSDETAIL];		
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_modelChart:DefaultTableModel;

		public function ChartViewTwoTable()
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(false);
			}
			
			m_modelChart = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			var sorter:ChartViewTwoTableSorter = new ChartViewTwoTableSorter(m_modelChart);
			super(sorter);
			sorter.setTableHeader(this.getTableHeader());
			for(var i:uint = 0; i < column.length; i ++)
			{
				m_modelChart.setColumnEditable(i, false);
			}
			setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			setBackground(new ASColor(0xffffed));
			setForeground(MyFontColor);
			setFont(MyFont);
			getTableHeader().setBackground(new ASColor(0xF7E444));
			getTableHeader().setForeground(MyFontColor);
			getTableHeader().setFont(MyFont);
			setRowHeight(24);
			
			getColumn(IDS_CHBET).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHWINSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHTAXSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_REAL).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_ROLLBACK).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_BSDETAIL).setCellFactory(new GeneralTableCellFactory(BHDetailTableCell));
		}
		
		public function MysetComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);
			getColumn(IDS_CHACCOUNT).setPreferredWidth(w * 0.14);
			getColumn(IDS_CHROUNDCOUNT).setPreferredWidth(w * 0.14);
			getColumn(IDS_CHBET).setPreferredWidth(w * 0.12);
			getColumn(IDS_CHWINSCORE).setPreferredWidth(w * 0.12);
			getColumn(IDS_CHTAXSCORE).setPreferredWidth(w * 0.12);
			getColumn(IDS_CHVALIDSCORE_REAL).setPreferredWidth(w * 0.12);
			getColumn(IDS_CHVALIDSCORE_ROLLBACK).setPreferredWidth(w * 0.14);
			getColumn(IDS_BSDETAIL).setPreferredWidth(w * 0.08);
		}
		public function ClearItem():void
		{
			m_modelChart.setRowCount(0);
		}
		public function InsertItems(ChartDatas:Array):void
		{
			for(var i:uint = 0; i < ChartDatas.length; i ++)
			{
				InsertItem(ChartDatas[i]);
			}
		}
		public function InsertItem(cd:tagChartViewTwoData):void
		{					
			var tableData:Array = [cd.szAccount, 
								   cd.dwGameRoundCount,
								   cd.fTotalBetScore, 
								   cd.fTotalWinScore, 
								   -cd.fTotalTaxScore, 
								   
								   cd.fValidScore_Total,
								   cd.fValidScore_Less_Rollback,
								   ""
								   ];
					
			m_modelChart.addRow(tableData);
		}
	}
}
include "./StringTableDef.as"