package org.god.Common.Chart
{
	import org.god.Net.Chart.tagChartViewOneData;
	import org.god.Common.NumericTableCell;
	import org.god.Common.PercentTableCell;
	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.JTable;
	import org.aswing.Insets;
	import org.aswing.geom.IntDimension;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.DefaultTextCell;
	import org.aswing.table.GeneralTableCellFactory;
	//游戏报表方式-1
	public class ChartViewOneTable extends JTable
	{
		public static var column:Array = [IDS_CHLEVELNAME, 
										  
							IDS_CHROUNDCOUNT, 
							IDS_CHTOTALBETSCORE, 
							IDS_CHTOTALWINSCORE,
							IDS_CHTOTALTAXSCORE,
							
							IDS_CHWINSCORE_HIGH,
							IDS_CHWINSCORE_MYSELF,
							IDS_CHWINSCORE_LESS,
							
							IDS_CHTAXSCORE_HIGH,
							IDS_CHTAXSCORE_MYSELF,
							IDS_CHTAXSCORE_LESS,
							
							IDS_CHVALIDSCORE_TOTAL,
							IDS_CHVALIDSCORE_HIGH,
							IDS_CHVALIDSCORE_HIGH_ROLLBACK,
							IDS_CHVALIDSCORE_LESS,
							IDS_CHVALIDSCORE_LESS_ROLLBACK,
							IDS_CHVALIDSCORE_BALANCE,
							
							IDS_CHPAIDSOCRE_HIGH,
							IDS_CHPAIDSOCRE_LESS,
							IDS_CHPAIDSOCRE_BALANCE,
							
							IDS_CHPAIDSOCRE_REMARKNAME];
		
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static const MyFixedBackgroundColor:ASColor = new ASColor(0xb3f86f);
		protected static var MyFont:ASFont;

		protected var m_modelChart:DefaultTableModel;
		protected var m_sorterChart:ChartViewOneTableSorter;
		protected var m_topHeader:ChartViewOneTableTopHeader;
		protected var m_strColChLevelName:String;
		
		protected var m_TotalChartViewOneData:tagChartViewOneData;
		protected var m_nOldTotalRowIndex:int;
		
		public function ChartViewOneTable()
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(false);
			}
			m_strColChLevelName = IDS_CHLEVELNAME;
			
			m_modelChart = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			m_sorterChart = new ChartViewOneTableSorter(m_modelChart);
			super(m_sorterChart);
			m_sorterChart.setTableHeader(this.getTableHeader());
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
			
			getColumn(IDS_CHLEVELNAME).setCellFactory(new GeneralTableCellFactory(ChartViewOneDataTableCell));
			
			getColumn(IDS_CHTOTALBETSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHTOTALWINSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHTOTALTAXSCORE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			
			getColumn(IDS_CHWINSCORE_HIGH).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHWINSCORE_MYSELF).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHWINSCORE_LESS).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
		
			getColumn(IDS_CHTAXSCORE_HIGH).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHTAXSCORE_MYSELF).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHTAXSCORE_LESS).setCellFactory(new GeneralTableCellFactory(NumericTableCell));

			getColumn(IDS_CHVALIDSCORE_TOTAL).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_HIGH).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_LESS).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHVALIDSCORE_BALANCE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			
			getColumn(IDS_CHPAIDSOCRE_HIGH).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHPAIDSOCRE_LESS).setCellFactory(new GeneralTableCellFactory(NumericTableCell));
			getColumn(IDS_CHPAIDSOCRE_BALANCE).setCellFactory(new GeneralTableCellFactory(NumericTableCell));

			getColumn(IDS_CHPAIDSOCRE_REMARKNAME).setCellFactory(new GeneralTableCellFactory(PercentTableCell));

			m_topHeader = new ChartViewOneTableTopHeader(this);
			addChild(m_topHeader);
			
			m_TotalChartViewOneData = new tagChartViewOneData;
			m_TotalChartViewOneData.szAccount = IDS_STATISTIC;
			m_nOldTotalRowIndex = -1;
		}
		
		public function MysetComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);

			getColumn(m_strColChLevelName).setPreferredWidth(w * 0.05);
			
			getColumn(IDS_CHROUNDCOUNT).setPreferredWidth(w * 0.03);
			getColumn(IDS_CHTOTALBETSCORE).setPreferredWidth(w * 0.055);
			getColumn(IDS_CHTOTALWINSCORE).setPreferredWidth(w * 0.055);
			getColumn(IDS_CHTOTALTAXSCORE).setPreferredWidth(w * 0.05);
			
			getColumn(IDS_CHWINSCORE_HIGH).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHWINSCORE_MYSELF).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHWINSCORE_LESS).setPreferredWidth(w * 0.06);
			
			getColumn(IDS_CHTAXSCORE_HIGH).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHTAXSCORE_MYSELF).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHTAXSCORE_LESS).setPreferredWidth(w * 0.06);
			
			getColumn(IDS_CHVALIDSCORE_TOTAL).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHVALIDSCORE_HIGH).setPreferredWidth(w * 0.03);
			getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).setPreferredWidth(w * 0.03);
			getColumn(IDS_CHVALIDSCORE_LESS).setPreferredWidth(w * 0.03);
			getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).setPreferredWidth(w * 0.03);
			getColumn(IDS_CHVALIDSCORE_BALANCE).setPreferredWidth(w * 0.03);
			
			getColumn(IDS_CHPAIDSOCRE_HIGH).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHPAIDSOCRE_LESS).setPreferredWidth(w * 0.06);
			getColumn(IDS_CHPAIDSOCRE_BALANCE).setPreferredWidth(w * 0.06);
			
			getColumn(IDS_CHPAIDSOCRE_REMARKNAME).setPreferredWidth(w * 0.01);
			
			layoutCells();
		}
		protected function updateTopHeaderWH(width:int, height:int):void
		{
			if(m_topHeader == null)
				return ;

			m_topHeader.setComBoundsXYWH(0,0,width,20);
			
			var w:int = getColumn(m_strColChLevelName).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHLEVEL, w);
			
			w = getColumn(IDS_CHROUNDCOUNT).getWidth() + 
			 getColumn(IDS_CHTOTALBETSCORE).getWidth()+
			  getColumn(IDS_CHTOTALWINSCORE).getWidth() +
			  getColumn(IDS_CHTOTALTAXSCORE).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHBET, w);
			
			w = getColumn(IDS_CHWINSCORE_HIGH).getWidth() + 
			 getColumn(IDS_CHWINSCORE_MYSELF).getWidth()+
			  getColumn(IDS_CHWINSCORE_LESS).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHWINSCORE_OCCAPANCY, w);
			
			w = getColumn(IDS_CHTAXSCORE_HIGH).getWidth() + 
			 getColumn(IDS_CHTAXSCORE_MYSELF).getWidth()+
			  getColumn(IDS_CHTAXSCORE_LESS).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHTAXSCORE_OCCAPANCY, w);
			
			w = getColumn(IDS_CHVALIDSCORE_TOTAL).getWidth() + 
			 getColumn(IDS_CHVALIDSCORE_HIGH).getWidth()+
			  getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).getWidth() +
			  getColumn(IDS_CHVALIDSCORE_LESS).getWidth() + 
			 getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).getWidth()+
			  getColumn(IDS_CHVALIDSCORE_BALANCE).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHVALIDSCORE, w);
			
			w = getColumn(IDS_CHPAIDSOCRE_HIGH).getWidth() + 
			 getColumn(IDS_CHPAIDSOCRE_LESS).getWidth()+
			  getColumn(IDS_CHPAIDSOCRE_BALANCE).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHPAIDSCORE, w);
			
			w = getColumn(IDS_CHPAIDSOCRE_REMARKNAME).getWidth();
			m_topHeader.setColPreferredWidth(IDS_CHPAIDSOCRE_REMARK, w);

		}

		override protected function layoutCells():void
		{
			super.layoutCells();
			if(m_topHeader == null)
				return ;
			var sz:IntDimension = getSize();
			updateTopHeaderWH(int(sz.width), int(sz.height));
			
			var headerHeight:int = getTableHeader().getPreferredHeight();
			m_topHeader.getTableHeader().setLocationXY(- viewPosition.x, 0);
			m_topHeader.getTableHeader().setSizeWH(
				getLastTotalColumnWidth(), 
				headerHeight);
			m_topHeader.getTableHeader().validate();
			m_topHeader.getTableHeader().paintImmediately();
		}
		override public function getInsets():Insets
		{
			return new Insets(20,0,0,0);
		}
		////////////////////////////
		public function SetLevelName(strLevelName:String):void
		{
			getColumn(m_strColChLevelName).setHeaderValue(strLevelName);
			m_strColChLevelName = strLevelName;
		}
		public function ClearItem():void
		{
			if(m_nOldTotalRowIndex != -1)
			{
				SetRowColor(m_nOldTotalRowIndex, null);
			}
			m_modelChart.clearRows();
			m_TotalChartViewOneData.ZeroData();
		}
		public function InsertItems(ChartDatas:Array):void
		{
			for(var i:uint = 0; i < ChartDatas.length; i ++)
			{
				InsertItem(ChartDatas[i]);
				m_TotalChartViewOneData.SumData(ChartDatas[i]);
			}
		}
		public function InsertItem(cd:tagChartViewOneData):void
		{					
			var tableData:Array = [cd, 
								   cd.dwGameRoundCount,
								   cd.fTotalBetScore, 
								   cd.fTotalWinScore, 
								   -cd.fTotalTaxScore, 
								   
								   cd.fWinScore_High, 
								   cd.fWinScore_Self,
								   cd.fWinScore_Less,
								   
								   -cd.fTaxScore_High, 
								   -cd.fTaxScore_Self,
								   -cd.fTaxScore_Less,
								   
								   cd.fValidScore_Total,
								   cd.fValidScore_High,
								   cd.fValidScore_High_Rollback,
								   cd.fValidScore_Less,
								   cd.fValidScore_Less_Rollback,
								   Number(cd.fValidScore_High_Rollback+cd.fValidScore_Less_Rollback),
								   
								   cd.fPaidScore_High,
								   cd.fPaidScore_Less,
								   GetBalance(cd.fPaidScore_High, cd.fPaidScore_Less),
								   
								   String("")
								   ];
					
			m_modelChart.addRow(tableData);
		}
		protected function GetBalance(fPaidScore_High:Number,
									  fPaidScore_Less:Number):Number
		{
			return fPaidScore_High+fPaidScore_Less;
		}
		public function FinishInsertItems():void
		{
			var r:int = getRowCount();
			if(r > 0)
			{
				InsertItem(m_TotalChartViewOneData);
				SetRowColor(r, MyFixedBackgroundColor);
				m_nOldTotalRowIndex = r;
				
				var fTotalBalance:Number = getValueAt(getRowCount() - 1,
												column.length - 2) as Number;
				
				for(var i:int = 0; i < getRowCount() - 1; i ++)
				{
					var fOneBalance:Number = getValueAt(i, column.length - 2) as Number;
					setValueAt(Number(fOneBalance/fTotalBalance), i, column.length - 1);
				}
			}
		}
		protected function SetRowColor(r:int, color:ASColor):void
		{
			for(var c:int = 0; c < getColumnCount(); c ++)
			{
				var cell:DefaultTextCell = getTableCell(r, c) as DefaultTextCell;
				if(cell != null)
				{
					cell.setFixedBackgroundColor(color);
				}
			}
		}
		
		public function ExtendCategoriseColumn(i:int, b:Boolean):void
		{
			if(m_topHeader.GetExtendFlag(i) == b)
				return;
				
			m_topHeader.SetExtendFlag(i,b);
			if(b)
			{
				
				if(i==2)
				{
					var w:int = (getColumn(IDS_CHWINSCORE_HIGH).getPreferredWidth()-10)*10; 
					getColumn(IDS_CHWINSCORE_HIGH).setPreferredWidth(w);
					
				 	w = (getColumn(IDS_CHWINSCORE_MYSELF).getPreferredWidth()-10)*10;
					getColumn(IDS_CHWINSCORE_MYSELF).setPreferredWidth(w);
					
			 	 	w = (getColumn(IDS_CHWINSCORE_LESS).getPreferredWidth()-10)*10;
					getColumn(IDS_CHWINSCORE_LESS).setPreferredWidth(w);
				}
				else if(i==3)
				{
					var w:int = (getColumn(IDS_CHTAXSCORE_HIGH).getPreferredWidth()-30)*10; 
					getColumn(IDS_CHTAXSCORE_HIGH).setPreferredWidth(w);
					
				 	w = (getColumn(IDS_CHTAXSCORE_MYSELF).getPreferredWidth()-30)*10;
					getColumn(IDS_CHTAXSCORE_MYSELF).setPreferredWidth(w);
					
			 	 	w = (getColumn(IDS_CHTAXSCORE_LESS).getPreferredWidth()-20)*10;
					getColumn(IDS_CHTAXSCORE_LESS).setPreferredWidth(w);
				}
				else if(i==4)
				{
						
					var w:int = (getColumn(IDS_CHVALIDSCORE_TOTAL).getPreferredWidth())*5; 
					getColumn(IDS_CHVALIDSCORE_TOTAL).setPreferredWidth(w);
					
					w= (getColumn(IDS_CHVALIDSCORE_HIGH).getPreferredWidth())*5; 
					getColumn(IDS_CHVALIDSCORE_HIGH).setPreferredWidth(w);
					
					w= (getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).getPreferredWidth())*5; 
					getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).setPreferredWidth(w);
					
				 	w = (getColumn(IDS_CHVALIDSCORE_LESS).getPreferredWidth())*5;
					getColumn(IDS_CHVALIDSCORE_LESS).setPreferredWidth(w);
					
					w = (getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).getPreferredWidth())*5;
					getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).setPreferredWidth(w);
				}
			}
			else
			{
				if(i==2)
				{
					var w:int = getColumn(IDS_CHWINSCORE_HIGH).getPreferredWidth()/10+10; 
					getColumn(IDS_CHWINSCORE_HIGH).setPreferredWidth(w);
					
				 	w = getColumn(IDS_CHWINSCORE_MYSELF).getPreferredWidth()/10+10;
					getColumn(IDS_CHWINSCORE_MYSELF).setPreferredWidth(w);
					
			 	 	w = getColumn(IDS_CHWINSCORE_LESS).getPreferredWidth()/10+10;
					getColumn(IDS_CHWINSCORE_LESS).setPreferredWidth(w);
				}
				else if(i==3)
				{
					var w:int = getColumn(IDS_CHTAXSCORE_HIGH).getPreferredWidth()/10+30; 
					getColumn(IDS_CHTAXSCORE_HIGH).setPreferredWidth(w);
					
				 	w = getColumn(IDS_CHTAXSCORE_MYSELF).getPreferredWidth()/10+30;
					getColumn(IDS_CHTAXSCORE_MYSELF).setPreferredWidth(w);
					
			 	 	w = getColumn(IDS_CHTAXSCORE_LESS).getPreferredWidth()/10+20;
					getColumn(IDS_CHTAXSCORE_LESS).setPreferredWidth(w);
				}
				else if(i==4)
				{
					
					var w:int = getColumn(IDS_CHVALIDSCORE_TOTAL).getPreferredWidth()/20; 
					getColumn(IDS_CHVALIDSCORE_TOTAL).setPreferredWidth(w);
					
					w = getColumn(IDS_CHVALIDSCORE_HIGH).getPreferredWidth()/20;
					getColumn(IDS_CHVALIDSCORE_HIGH).setPreferredWidth(w);
					
					w = getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).getPreferredWidth()/20;
					getColumn(IDS_CHVALIDSCORE_HIGH_ROLLBACK).setPreferredWidth(w);
					
				 	w = getColumn(IDS_CHVALIDSCORE_LESS).getPreferredWidth()/20;
					getColumn(IDS_CHVALIDSCORE_LESS).setPreferredWidth(w);
					
					w = getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).getPreferredWidth()/20;
					getColumn(IDS_CHVALIDSCORE_LESS_ROLLBACK).setPreferredWidth(w);
				}
			}
			layoutCells();
		}
	}
}
include "./StringTableDef.as"