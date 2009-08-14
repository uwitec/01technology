package org.god.Common
{
	import org.aswing.ASColor;
	import org.aswing.JTable;
	import org.aswing.table.DefaultTextCell;
	//百份比表单元
	public class PercentTableCell extends DefaultTextCell
	{
		public static const myRedColor:ASColor = new ASColor(0xff1100);//单元颜色
		//构造函数
		public function PercentTableCell()
		{
			super();

		}
		//设置单元变量
		override public function setCellValue(value:*):void
		{
			this.value = value;
			var n:Number = value as Number;
			n = n * 100;
			setText(n.toFixed(1) + "%");
		}
		//获取选定颜色
		override public function getMySelectionForeground(table:JTable):ASColor
		{
			if(this.value == null)
				return super.getMySelectionForeground(table);
			else
			{
				if(this.value < 0)
					return myRedColor;
				else
					return super.getMySelectionForeground(table);
			}
		}
		//获取前景颜色
		override public function getMyForeground(table:JTable):ASColor
		{
			if(this.value == null)
				return super.getMyForeground(table);
			else
			{
				if(this.value < 0)
					return myRedColor;
				else
					return super.getMyForeground(table);
			}
		}
	}
}