package org.god.Common.BetHistory
{
	import org.aswing.table.JTableHeader;
	import org.aswing.table.TableModel;
	import org.aswing.table.sorter.TableSorter;
	//游戏记录表排序
	public class BetHistoryTableSorter extends TableSorter
	{
		public function BetHistoryTableSorter(tableModel:TableModel, tableHeader:JTableHeader=null)
		{
			super(tableModel,tableHeader);
		}
		override public function getComparator(column:int):Function 
		{
			if(column == 5 ||
			   column == 6 ||
			   column == 7)
				return NUMBER_COMAPRATOR;
			
			return super.getComparator(column);
		}
		
	}
}