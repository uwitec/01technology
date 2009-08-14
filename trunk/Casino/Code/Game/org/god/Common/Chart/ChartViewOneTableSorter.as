package org.god.Common.Chart
{
	import org.aswing.table.JTableHeader;
	import org.aswing.table.TableModel;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.sorter.Row;

	public class ChartViewOneTableSorter extends TableSorter
	{
		public function ChartViewOneTableSorter(tableModel:TableModel, tableHeader:JTableHeader=null)
		{
			super(tableModel,tableHeader);
		}
		override public function getComparator(column:int):Function 
		{
			if(column >= 1 && column < ChartViewOneTable.column.length-1)
				return NUMBER_COMAPRATOR;
			
			return super.getComparator(column);
		}
		override protected function getViewToModel():Array
		{
			if (viewToModel == null)
			{
				var tableModelRowCount:int = tableModel.getRowCount();
				if (tableModelRowCount > 0)
				{
					tableModelRowCount -= 1;
				}
				viewToModel = new Array(tableModelRowCount);
				for (var row:int = 0; row < tableModelRowCount; row++)
				{
					viewToModel[row] = new Row(this, row);
				}

				if (isSorting())
				{
					viewToModel.sort(sortImp);
				}
				if (tableModelRowCount > 0)
				{
					viewToModel.push(new Row(this, tableModel.getRowCount()-1));
				}
			}
			return viewToModel;
		}
	}
}