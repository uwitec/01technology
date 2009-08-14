package 
{

	import org.god.Net.tagUserInfoHead;
	import org.god.IGameFrame.IUserItem;
	
	import org.aswing.table.DefaultTextCell;
	//用户信息项表单元
	public class UserItemTableCell extends DefaultTextCell
	{
		public function UserItemTableCell()
		{
			super();

		}
		override public function setCellValue(value:*):void
		{
			this.value = value;
			var pIUserItem:IUserItem = value as IUserItem;
			var pUserData:tagUserInfoHead=pIUserItem.GetUserData();
			setText(pUserData.szName);
		}
	}
}