package org.god.Common.BetHistory
{
	import org.aswing.table.JTableHeader;
	import org.aswing.table.TableModel;
	import org.aswing.table.sorter.TableSorter;
	//详细游戏记录表排序
	public class BHGameRoundDetailBetScoreTableSorter extends TableSorter
	{
		public function BHGameRoundDetailBetScoreTableSorter(tableModel:TableModel, tableHeader:JTableHeader=null)
		{
			super(tableModel,tableHeader);
		}
		override public function getComparator(column:int):Function 
		{
			if(column == 1 ||
			   column == 2 )
				return NUMBER_COMAPRATOR;
			
			return super.getComparator(column);
		}
		
	}
}