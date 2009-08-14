package org.god.SkinControl.SkinDateField
{
	import flash.events.MouseEvent;
	import flash.events.Event;
	
	import org.aswing.JPopup;
	import org.aswing.JLabel;
	import org.aswing.JButton;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.SolidBackground;
	import org.aswing.border.BevelBorder;
	import org.aswing.plaf.basic.icon.ArrowIcon;
	import org.aswing.geom.IntDimension;

	public class DateChooseBox extends JPopup
	{
		public static  const nDefaultWidth:uint=220;
		public static  const nDefaultHeight:uint=180;
		
		protected static const PREV_MONTH:uint = 0;
		protected static const NEXT_MONTH:uint = 1;
		protected static const TODAY:uint = 2;

		protected var m_DateChoosePanel:DateChoosePanel;
		protected var m_labelDate:JLabel;
		protected var m_btnSelectTimeLeft:JButton;
		protected var m_btnSelectTimeRight:JButton;
		protected var m_btnToday:JButton;
		protected var m_Date:Date;

		public function DateChooseBox()
		{
			super(null,true);
			setClipMasked(false);
			m_labelDate=new JLabel("",null,JLabel.CENTER);

			append(m_labelDate);
			append(getDateChoosePanel());
			getDateChoosePanel().addEventListener(DateTimeLabelSelEvent.EVENTNAME_DTLS,
												   OnEventDateTimeLabelSel);
			m_btnSelectTimeLeft = createDropDownButton();
			addChild(m_btnSelectTimeLeft);
			m_btnSelectTimeLeft.addActionListener(OnEventSelectTimeLeft);
			m_btnSelectTimeRight = createDropDownButton(false);
			addChild(m_btnSelectTimeRight);
			m_btnSelectTimeRight.addActionListener(OnEventSelectTimeRight);
			m_btnToday=new JButton("今日");
			addChild(m_btnToday);
			m_btnToday.addActionListener(OnEventSelectToday);
			setBackgroundDecorator(new SolidBackground(new ASColor(0xFEFEFE)));
			setBorder(new BevelBorder );

			setSizeWH(nDefaultWidth,nDefaultHeight);
		}
		public function getDateChoosePanel():DateChoosePanel
		{
			if (m_DateChoosePanel == null)
			{
				m_DateChoosePanel=new DateChoosePanel  ;
			}
			return m_DateChoosePanel;
		}
		public function setDateTime(dt:Date):void
		{
			m_Date=new Date(dt);
			if (m_labelDate)
			{
				m_labelDate.setText(getDateString());
			}
			getDateChoosePanel().setDateTime(dt);
		}
		override public  function setSizeWH(w:int,h:int):void
		{
			super.setSizeWH(w,h);
			if (m_labelDate)
			{
				m_labelDate.setComBoundsXYWH(0,0,w,20);
				getDateChoosePanel().setComBoundsXYWH(0,20,w,h-20-20);
				m_btnSelectTimeLeft.setComBoundsXYWH(10, h - 20 -6, 20, 20);
				m_btnSelectTimeRight.setComBoundsXYWH(w - 20-10, h - 20 -6, 20, 20);
				m_btnToday.setComBoundsXYWH(w/2 - 25, h - 20 -6, 50, 20);
			}
		}
		protected function getDateString():String
		{
			return m_Date.fullYear + "年" + (m_Date.month + 1) + "月" + m_Date.date + "日";
		}
		override public  function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_labelDate)
			{
				m_labelDate.setFont(newFont);
				getDateChoosePanel().setFont(newFont);
				m_btnSelectTimeLeft.setFont(newFont);
				m_btnSelectTimeRight.setFont(newFont);
				m_btnToday.setFont(newFont);
			}
		}
		protected function OnEventDateTimeLabelSel(e:DateTimeLabelSelEvent):void
		{
			dispatchEvent(new DateChooseOKEvent(new Date(m_Date.fullYear,
			m_Date.month,
			e.getNumValue(),
			m_Date.hours,
			m_Date.minutes,
			m_Date.seconds)));
			dispose();
		}
		override protected  function initModalMC():void
		{
			super.initModalMC();
			getModalMC().addEventListener(MouseEvent.MOUSE_DOWN,__flashModelFrame);
		}
		private function __flashModelFrame(e:MouseEvent):void
		{
			dispose();
		}
		protected function createDropDownButton(bLeft:Boolean = true):JButton
		{
			var btn:JButton=new JButton("",new ArrowIcon(bLeft ?  Math.PI : 0,8,new ASColor(0x000000),new ASColor(0x444444)));
			btn.setFocusable(false);
			btn.setPreferredSize(new IntDimension(20,20));
			btn.setUIElement(true);

			return btn;
		}
		protected function OnEventSelectTimeLeft(e:Event):void
		{
			ChangeDateTime(PREV_MONTH);
		}
		protected function OnEventSelectTimeRight(e:Event):void
		{
			ChangeDateTime(NEXT_MONTH);
		}
		protected function OnEventSelectToday(e:Event):void
		{
			ChangeDateTime(TODAY);
		}
		public function ChangeDateTime(iID:uint):void
		{
			var iDay:uint = m_Date.date;
			var iMonth:uint = m_Date.month + 1;
			var iYear:uint = m_Date.fullYear;
			var iHour:uint = m_Date.hours;
			var iMin:uint = m_Date.minutes;

			var iDir = 0;
			switch (iID)
			{
				case PREV_MONTH:
				{
					iMonth --;
					iDir = -1;
					if (iMonth == 0)
					{
						iMonth = 12;
						iYear --;
					}
					break;
				}
				case NEXT_MONTH:
				{
					iDir = 1;
					iMonth ++;
					if (iMonth == 13)
					{
						iMonth = 1;
						iYear ++;
					}
					break;
				}
				case TODAY:
				{
					var today:Date = new Date;
					iDay = today.date;
					iMonth = today.month + 1;
					iYear = today.fullYear;
					break;
				}

				default:
					return;
			}
			
			var iDaysInMonth:uint = DateChoosePanel.GetDaysInMonth (iMonth, iYear);
			if (iDay > iDaysInMonth)
			{
				iDay = iDaysInMonth;
			}
			
			var date :Date = new Date(iYear, iMonth-1, iDay, iHour, iMin, m_Date.seconds);
			if(date.getTime() != m_Date.getTime())
			{
				setDateTime(date);
			}
		}
	}
}