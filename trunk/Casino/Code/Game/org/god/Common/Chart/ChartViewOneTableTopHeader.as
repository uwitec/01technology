package org.god.Common.Chart
{
	import flash.events.MouseEvent;
	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.JTable;
	import org.aswing.Container;
	import org.aswing.geom.IntPoint;
	import org.aswing.table.sorter.TableSorter;
	import org.aswing.table.DefaultTableModel;
	import org.aswing.table.GeneralTableCellFactory;
	
	//游戏报表方式-1列行
	public class ChartViewOneTableTopHeader extends JTable
	{
		public static var column:Array = [IDS_CHLEVEL, 
							IDS_CHBET, 
							IDS_CHWINSCORE_OCCAPANCY, 
							IDS_CHTAXSCORE_OCCAPANCY, 
							IDS_CHVALIDSCORE,
							IDS_CHPAIDSCORE,
							IDS_CHPAIDSOCRE_REMARK];		
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_modelChart:DefaultTableModel;
		protected var m_parentChartViewOneTable:ChartViewOneTable;
		protected var m_bExtendFlag:Array = new Array(column.length);
		
		public function ChartViewOneTableTopHeader(parent:ChartViewOneTable)
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(14);
				 MyFont = MyFont.changeBold(false);
			}
			m_parentChartViewOneTable = parent;
			m_modelChart = (new DefaultTableModel()).initWithNamesRowcount(column, 0);
			super(m_modelChart);
			for(var i:uint = 0; i < column.length; i ++)
			{
				m_modelChart.setColumnEditable(i, false);
				m_bExtendFlag[i] = true;
			}
			setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			setBackground(new ASColor(0xffffed));
			setForeground(MyFontColor);
			setFont(MyFont);
			getTableHeader().setBackground(new ASColor(0xF7E444));
			getTableHeader().setForeground(MyFontColor);
			getTableHeader().setFont(MyFont);
			getTableHeader().setResizingAllowed(false);
			setRowHeight(26);
			
			getTableHeader().addEventListener(MouseEvent.MOUSE_DOWN, onHeaderClick);
		}
		public function setColPreferredWidth(strCol:String,w:int):void
		{
			getColumn(strCol).setPreferredWidth(w);
			getColumn(strCol).setWidth(w);
		}
		override public function doLayout():void
		{
			lastTotalColumnWidth = getColumnModel().getTotalColumnWidth();
			super.doLayoutSuper();
		}
		public function onHeaderClick(e:MouseEvent):void
		{
			var p:IntPoint = getTableHeader().getMousePosition();
			var index:int = getTableHeader().columnAtPoint(p);
			if(index >= 0)
			{
				trace("onHeaderClick:" + index );
				m_parentChartViewOneTable.ExtendCategoriseColumn(index,!m_bExtendFlag[index]);
			}
		}
		public function SetExtendFlag(i:int, b:Boolean):void
		{
			m_bExtendFlag[i] = b;
		}
		public function GetExtendFlag(i:int):Boolean
		{
			return m_bExtendFlag[i];
		}
	}
}
include "./StringTableDef.as"