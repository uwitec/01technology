package 
{
	import org.aswing.table.DefaultTextCell;
	import org.god.Net.tagSubUserData;
	//子帐号表单元
	public class SubUserDataTableCell extends DefaultTextCell
	{
		public function SubUserDataTableCell()
		{
			super();

		}
		override public function setCellValue(value:*):void
		{
			this.value = value;
			var UserData:tagSubUserData = value as tagSubUserData;
			setText(UserData.szAccount);
		}
	}
}