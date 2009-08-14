package 
{
	import org.aswing.table.DefaultTextCell;
	//用户表单元
	public class UserDataTableCell extends DefaultTextCell
	{
		public function UserDataTableCell()
		{
			super();

		}
		override public function setCellValue(value:*):void
		{
			this.value = value;
			var nodeData:UserDataTreeNodeData = value as UserDataTreeNodeData;
			setText(nodeData.getAccount());
		}
	}
}