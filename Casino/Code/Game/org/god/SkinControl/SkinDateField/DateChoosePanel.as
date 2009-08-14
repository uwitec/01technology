package org.god.SkinControl.SkinDateField
{
	import org.aswing.JPanel;
	import org.aswing.ASFont;
	import org.aswing.JLabel;
	import org.aswing.geom.IntRectangle;
	
	public class DateChoosePanel extends JPanel
	{
		public static const millisecondsPerMinute:int = 1000 * 60;
		public static const millisecondsPerHour:int = 1000 * 60 * 60;
		public static const millisecondsPerDay:int = 1000 * 60 * 60 * 24;
		
		public static const arrWeekDayTitle:Array = ["日","一","二","三","四","五","六"];
		
		public static const iRows:uint = 6;
		public static const iColumns:uint = 7;
		
		protected var m_iFirstDayOfWeek:uint;
		protected var m_iDaysInMonth:uint;
		protected var m_arrWeekDayTitleLabel:Array;
		protected var m_arrDateLabel:Array;
		protected var m_rectDayLocations:Array;
		protected var m_rectDays:IntRectangle;
		protected var m_Date:Date;
		
		public function DateChoosePanel()
		{
			super();
			
			m_iFirstDayOfWeek = 0;
			m_arrWeekDayTitleLabel = new Array(7);
			for(var i:uint = 0; i < m_arrWeekDayTitleLabel.length; i ++)
			{
				m_arrWeekDayTitleLabel[i] = new DateTimeLabel(0, "", false);
				addChild(m_arrWeekDayTitleLabel[i]);
				m_arrWeekDayTitleLabel[i].setHorizontalAlignment(JLabel.CENTER);
				m_arrWeekDayTitleLabel[i].setText(arrWeekDayTitle[i]);
			}
			
			m_rectDayLocations = new Array(31);
			for(i = 0; i < m_rectDayLocations.length; i ++)
			{
				m_rectDayLocations[i] = new IntRectangle;
			}
			m_arrDateLabel = new Array(31);
			for(i=0; i < m_arrDateLabel.length; i ++)
			{
				m_arrDateLabel[i] = new DateTimeLabel(31, "", true, 0);
				addChild(m_arrDateLabel[i]);
				m_arrDateLabel[i].setHorizontalAlignment(JLabel.CENTER);
				m_arrDateLabel[i].addEventListener(DateTimeLabelSelEvent.EVENTNAME_DTLS,
												   OnEventDateTimeLabelSel);
			}
			m_rectDays = new IntRectangle;
		}
		public function setDateTime(dt:Date):void
		{
			m_Date=new Date(dt);
			RebuildLocations();
			for(var i:uint = 0; i < m_arrDateLabel.length; i ++)
			{
				m_arrDateLabel[i].setText("");
				m_arrDateLabel[i].setSelected(false);
			}
			m_iDaysInMonth = GetDaysInMonth(m_Date.month + 1,m_Date.fullYear);
			
			for (var iDay:int = 1; iDay <= m_iDaysInMonth; iDay ++)
			{
				var rect:IntRectangle = GetDateLabelRect(iDay);
				if(rect)
				{
					
					m_arrDateLabel[iDay-1].setComBoundsXYWH(rect.x, rect.y, rect.width, rect.height);
					m_arrDateLabel[iDay-1].setNumValue(iDay);
					
					if(iDay == m_Date.date)
						m_arrDateLabel[iDay-1].setSelected(true);
				}
			}
		}
		override public function setComBoundsXYWH(x:int, y:int, w:int, h:int):void
		{
			super.setComBoundsXYWH(x,y,w,h);
			if(m_arrWeekDayTitleLabel)
			{
				var cx:Number = w / m_arrWeekDayTitleLabel.length ;
				for(var i:uint = 0; i < m_arrWeekDayTitleLabel.length; i ++)
				{
					m_arrWeekDayTitleLabel[i].setComBoundsXYWH(cx * i, y, cx, 18);
				}
			}
			m_rectDays.x = 0; 
			m_rectDays.y = y + 16; 
			m_rectDays.width = w;
			m_rectDays.height= h - 26;
		}
		
		override public  function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_arrWeekDayTitleLabel)
			{
				var f:ASFont = newFont.changeBold(true);
				for(var i:uint = 0; i < m_arrWeekDayTitleLabel.length; i ++)
				{
					m_arrWeekDayTitleLabel[i].setFont(f);
				}
			}
			if(m_arrDateLabel)
			{
				for(var i:uint = 0; i < m_arrDateLabel.length; i ++)
				{
					m_arrDateLabel[i].setFont(newFont);
				}
			}
		}
		protected function GetDateLabelRect(iID:int):IntRectangle
		{
			if (iID < 1 || iID > m_iDaysInMonth)
			{
				return null;
			}

			return m_rectDayLocations [iID - 1];
		}
		protected function RebuildLocations():void
		{
			if(m_Date == null || m_rectDays == null)
				return;
			var dateCurr:Date = new Date(m_Date.fullYear, m_Date.month, 1, 0, 0, 0);

			var y:Number = m_rectDays.y;

			var iRowHeight:int = m_rectDays.height / iRows;
			var iColumnWidth:int = m_rectDays.width / iColumns;

			while (dateCurr.month  == m_Date.month)
			{
				var iWeekDay:uint = (dateCurr.day + 1) - m_iFirstDayOfWeek;
				if (iWeekDay < 1)
				{
					iWeekDay += 7;
				}

				var iDay:uint = dateCurr.date;
		
				var x:int = m_rectDays.x + iColumnWidth * (iWeekDay - 1);

				m_rectDayLocations [iDay - 1] = 
					new IntRectangle (x, y, iColumnWidth, iRowHeight);

				if (iWeekDay == 7)
				{
					y += iRowHeight;
					x = m_rectDays.x;
				}
				else
				{
					x += iColumnWidth;
				}

				dateCurr.setTime(dateCurr.getTime() + millisecondsPerDay);
			}
		}
		static public function GetDaysInMonth (iMonth:uint, iYear:uint):uint
		{
			var inMonthLen:Array = new Array (31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);

			var nRes:int = inMonthLen [iMonth - 1];
			if (iMonth == 2 && iYear % 4 == 0 && 
				(int(iYear % 100) != 0 || int(iYear % 400) == 0))
			{
				nRes = 29;
			}

			return nRes;
		}
		protected function OnEventDateTimeLabelSel(e:DateTimeLabelSelEvent):void
		{
			dispatchEvent(new DateTimeLabelSelEvent(e.getNumValue()));
		}
	}
}