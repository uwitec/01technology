package org.god.Common.Chart
{
	import org.god.Net.Chart.tagChartViewOneData;
	
	import org.aswing.table.DefaultTextCell;
	//游戏报表方式-1表单元
	public class ChartViewOneDataTableCell extends DefaultTextCell
	{
		public function ChartViewOneDataTableCell()
		{
			super();

		}
		override public function setCellValue(value:*):void
		{
			this.value = value;
			var cd:tagChartViewOneData = value as tagChartViewOneData;
			setText(cd.szAccount);
		}
	}
}