package 
{
	import org.aswing.table.DefaultTextCell;
	import org.aswing.JComboBox;
	import org.aswing.JTable;
	import org.aswing.Component;	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.geom.IntRectangle;
	import org.aswing.table.TableCell;
	import org.aswing.event.InteractiveEvent;
	
	import org.god.Common.MasterRight;
	import org.god.IGameManagement.GlobalMain;
	import org.god.IGameManagement.IMain;
	//子帐号用户状态表单元
	public class SubUserStateTableCell extends JComboBox implements TableCell
	{
		public static  const statebands:Array = [IDS_NORMAL, IDS_CONGEAL];
		public static  var fontcell:ASFont;
		
		protected var value:*;
		public function SubUserStateTableCell()
		{
			super();
			this.setListData(statebands);
			this.addSelectionListener(OnEventSelChangeState);
			setEnabled(AllowRight());
		}
		protected function AllowRight():Boolean
		{
			var main:IMain = GlobalMain.getMain();
			if(main)
			{
				if(main.getUserData())
				{
					return MasterRight.CanSubAccountSet(main.getUserData().dwMasterRight)&& MasterRight.SimpleCheck_UserRight();
				}
			}
			return false;
		}
		override public function setComBounds(b:IntRectangle):void
		{
			readyToPaint = true;
			if (!b.equals(bounds))
			{
				if (b.width != bounds.width || b.height != bounds.height)
				{
					repaint();
				}
				bounds.setRect(b);
				locate();
				valid = false;
			}
		}
		override public function invalidate():void
		{
			valid = false;
		}
		override public function revalidate():void
		{
			valid = false;
		}
		public function setCellValue(value:*):void
		{
			this.value = value;
			setSelectedItem(value.toString());
			if(value.toString() == IDS_CONGEAL)
			{
				getEditor().getEditorComponent().setForeground(ASColor.RED);
			}
			else
			{
				getEditor().getEditorComponent().setForeground(ASColor.BLACK);
			}
		}
		public function getCellValue():*
		{
			return value;
		}
		public function setTableCellStatus(table:JTable, isSelected:Boolean, row:int, column:int):void
		{
			if(fontcell == null)
			{
				fontcell = table.getFont().changeBold(false);
			}
			setFont(fontcell);
		}
		public function getCellComponent():Component
		{
			return this;
		}
		override public function toString():String
		{
			return "TextCell[label:" + super.toString() + "]\n";
		}
		public function OnEventSelChangeState(e:InteractiveEvent):void
		{
			if(this.value != null &&
			   this.value.toString() != getSelectedItem())
			{
				dispatchEvent(new UserStateTableCellEvent(getSelectedItem()));
			}
		}
	}
}
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"