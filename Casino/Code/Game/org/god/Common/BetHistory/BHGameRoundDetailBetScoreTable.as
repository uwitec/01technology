package org.god.Common.BetHistory
{
	import org.god.Common.NumericTableCell;
	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.JTable;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	//游戏记录详细投注表
	public class BHGameRoundDetailBetScoreTable extends JTable
	{
		public static var column:Array = [IDS_BETREGIONITEM, 
							IDS_BHBETSCORE, 
							IDS_BHWINSCORE];		
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_modelGameRoundDetailBetScore:DefaultTableModel;
		
		public function BHGameRoundDetailBetScoreTable()
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(false);
			}
			m_modelGameRoundDetailBetScore = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			var sorter:BHGameRoundDetailBetScoreTableSorter = new BHGameRoundDetailBetScoreTableSorter(m_modelGameRoundDetailBetScore);
			super(sorter);
			sorter.setTableHeader(this.getTableHeader());
			for(var i:uint = 0; i < column.length; i ++)
			{
				m_modelGameRoundDetailBetScore.setColumnEditable(i, false);
			}
			setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			setBackground(new ASColor(0xffffed));
			setForeground(MyFontColor);
			setFont(MyFont);
			getTableHeader().setBackground(new ASColor(0xF7E444));
			getTableHeader().setForeground(MyFontColor);
			getTableHeader().setFont(MyFont);
			setRowHeight(20);
			
			getColumn(IDS_BHBETSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_BHWINSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
		}
		
		public function MysetComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);
			getColumn(IDS_BETREGIONITEM).setPreferredWidth(w * 0.30);
			getColumn(IDS_BHBETSCORE).setPreferredWidth(w * 0.30);
			getColumn(IDS_BHWINSCORE).setPreferredWidth(w * 0.30);
		}
		
		public function ClearItem():void
		{
			m_modelGameRoundDetailBetScore.setRowCount(0);
		}
		public function InsertItems(strBetRegionNameArray:Array,
									fBetScoreArray:Array,
									fWinScoreArray:Array):void
		{
			for(var i:uint = 0; i < strBetRegionNameArray.length; i ++)
			{
				InsertItem(strBetRegionNameArray[i],fBetScoreArray[i],fWinScoreArray[i]);
			}
		}
		public function InsertItem(strBetRegionName:String,
								   fBetScore:Number,
								   fWinScore:Number):void
		{					
			var tableData:Array = [strBetRegionName, 
								   fBetScore,
								   fWinScore];
					
			m_modelGameRoundDetailBetScore.addRow(tableData);
		}
		
	}
}
include "./StringTableDef.as"