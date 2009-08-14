package org.god.Common.Operation
{
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.JTable;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	
	import org.god.Net.Operation.tagOperationData;
	import org.god.Net.Operation.tagOperationContent;
	import org.god.Net.Operation.tagOperationHeader;
	//操作记录表
	public class OperationTable extends JTable
	{
		public static var column:Array = [IDS_ACCOUNT, 
							IDS_OPERATOR, 
							IDS_OPTYPE, 
							IDS_OPTIME,
							IDS_OPADDRESS,
							IDS_OPDETAIL];		
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_modelOperation:DefaultTableModel;
		protected var m_bHalfAddressString:Boolean;
		
		public function OperationTable(bHalfAddressString:Boolean = true)
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(true);
			}
			
			m_bHalfAddressString = bHalfAddressString;
			
			m_modelOperation = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			var sorter:TableSorter = new TableSorter(m_modelOperation);
			super(sorter);
			sorter.setTableHeader(this.getTableHeader());
			for(var i:uint = 0; i < column.length; i ++)
			{
				m_modelOperation.setColumnEditable(i, false);
			}
			setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			setBackground(new ASColor(0xffffed));
			setForeground(MyFontColor);
			setFont(MyFont);
			getTableHeader().setBackground(new ASColor(0xF7E444));
			getTableHeader().setForeground(MyFontColor);
			getTableHeader().setFont(MyFont);
			setRowHeight(26);
			getColumn(IDS_OPDETAIL).setCellFactory(new GeneralTableCellFactory(OPDetailTableCell));
		}
		
		public function MysetComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);
			getColumn(IDS_ACCOUNT).setPreferredWidth(w * 0.14);
			getColumn(IDS_OPERATOR).setPreferredWidth(w * 0.14);
			getColumn(IDS_OPTYPE).setPreferredWidth(w * 0.12);
			getColumn(IDS_OPTIME).setPreferredWidth(w * 0.17);
			getColumn(IDS_OPADDRESS).setPreferredWidth(w * 0.11);
			getColumn(IDS_OPDETAIL).setPreferredWidth(w * 0.32);
		}
		public function ClearItem():void
		{
			m_modelOperation.setRowCount(0);
		}
		public function InsertItems(OperationDatas:Array):void
		{
			for(var i:uint = 0; i < OperationDatas.length; i ++)
			{
				InsertItem(OperationDatas[i]);
			}
		}
		public function InsertItem(op:tagOperationData):void
		{					
			var tableData:Array = [op.Header.szAccount, 
								   op.Header.szOperationAccount,
								   OperationUtil.getOPTypeName(op.Header.dwOPType), 
								   OperationUtil.GetOPTimeString(op.Header.fTime),
								   OperationUtil.GetOPAddressString(op.Header.dwIPAddress, m_bHalfAddressString),
								   OperationUtil.GetOPDetailString(op.Header,op.Content)];
					
			m_modelOperation.addRow(tableData);
		}
	}
}
include "./StringTableDef.as"