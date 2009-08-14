package org.god.Common.Chart
{
	import org.aswing.table.JTableHeader;
	import org.aswing.table.TableModel;
	import org.aswing.table.sorter.TableSorter;
	//游戏报表方式-2排序
	public class ChartViewTwoTableSorter extends TableSorter
	{
		public function ChartViewTwoTableSorter(tableModel:TableModel, tableHeader:JTableHeader=null)
		{
			super(tableModel,tableHeader);
		}
		override public function getComparator(column:int):Function 
		{
			if(column >= 1 && column < ChartViewTwoTable.column.length-1)
				return NUMBER_COMAPRATOR;
			
			return super.getComparator(column);
		}
		
	}
}