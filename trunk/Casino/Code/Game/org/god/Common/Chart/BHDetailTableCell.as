package org.god.Common.Chart
{
	import flash.events.Event;
	
	import org.aswing.table.DefaultTextCell;
	import org.aswing.Container;
	import org.aswing.JTable;
	import org.aswing.Component;	
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.geom.IntRectangle;
	import org.aswing.table.TableCell;
	
	import org.god.Common.MasterRight;
	import org.god.Common.GlobalCommonMain;
	import org.god.SkinControl.SkinButton;
	//游戏局记录详细信息表单元
	public class BHDetailTableCell extends Container implements TableCell
	{
		public static const FunctionData:Array = [IDS_BHVIEW];
		public static  var fontcell:ASFont;
		
		protected var value:*;
		protected var m_arrFuncButton:Array;
		
		public function BHDetailTableCell()
		{
			super();
			m_arrFuncButton = new Array(FunctionData.length);
			if(m_arrFuncButton)
			{
				for(var i:uint = 0; i < m_arrFuncButton.length; i ++)
				{
					m_arrFuncButton[i] = new SkinButton(FunctionData[i]);
					addChild(m_arrFuncButton[i]);
					m_arrFuncButton[i].addActionListener(OnEventFuncBtnClick);
					if(FunctionData[i] == IDS_BHVIEW)
					{
						m_arrFuncButton[i].setEnabled(AllowRightView());
					}
				}
			}
		}
		protected function AllowRightView():Boolean
		{
			return MasterRight.CanBetHistoryView(GlobalCommonMain.getGlobalCommonMain().GetMasterRight());
		}
		override public function setComBounds(b:IntRectangle):void
		{
			readyToPaint = true;
			if (!b.equals(bounds))
			{
				if(m_arrFuncButton)
				{
					var cx:Number = b.width / m_arrFuncButton.length;
					for(var i:uint = 0; i < m_arrFuncButton.length; i ++)
					{
						m_arrFuncButton[i].setComBounds(new IntRectangle(cx * i, 0, cx, b.height));
					}
				}
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
			if(m_arrFuncButton)
			{
				for(var i:uint = 0; i < m_arrFuncButton.length; i ++)
				{
					m_arrFuncButton[i].setFont(fontcell);
				}
			}

		}
		public function getCellComponent():Component
		{
			return this;
		}
		override public function toString():String
		{
			return "TextCell[label:" + super.toString() + "]\n";
		}
		protected function OnEventFuncBtnClick(e:Event):void
		{
			dispatchEvent(new BHDetailTableCellBtnEvent(e.currentTarget.getText()));
		}
	}
}
include "./StringTableDef.as"
