package org.god.SkinControl.SkinDateField
{
	import flash.events.Event;
	
	import org.aswing.JButton;
	import org.aswing.JTextArea;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.geom.IntDimension;
	import org.aswing.geom.IntPoint;
	import org.aswing.AsWingUtils;
	import org.aswing.SolidBackground;
	import org.aswing.border.LineBorder;
	import org.aswing.plaf.basic.icon.ArrowIcon;
	
	public class DateField extends JTextArea
	{
		public static  const nDefaultWidth:uint=200;
		public static  const nDefaultHeight:uint=24;
		public static  const IDS_YEAR:String="-";
		public static  const IDS_MONTH:String="-";
		public static  const IDS_DATE:String="";
		public static  const IDS_HOUR:String=":";
		public static  const IDS_MINUTE:String=":";
		public static  const IDS_SECOND:String=" ";

		protected var m_DateTime:Date;
		protected var m_labelYear:DateTimeLabel;
		protected var m_labelMonth:DateTimeLabel;
		protected var m_labelDate:DateTimeLabel;
		protected var m_labelHour:DateTimeLabel;
		protected var m_labelMinute:DateTimeLabel
		protected var m_labelSecond:DateTimeLabel;
		protected var m_labelArray:Array=new Array;
		
		protected var m_btnSelectDate:JButton;
		protected var m_btnSelectTimeUp:JButton;
		protected var m_btnSelectTimeDown:JButton;
		protected var m_DateChooseBox:DateChooseBox;
		
		public function DateField()
		{
			super("");
			
			this.alpha = 0.8;

			m_labelYear=new DateTimeLabel(9999, IDS_YEAR, false);
			m_labelMonth=new DateTimeLabel(12, IDS_MONTH, false);
			m_labelDate=new DateTimeLabel( 31, IDS_DATE, false);
			m_labelHour=new DateTimeLabel( 23, IDS_HOUR, true);
			m_labelMinute=new DateTimeLabel( 59, IDS_MINUTE, true);
			m_labelSecond=new DateTimeLabel( 59, IDS_SECOND, true);
			m_labelArray.push(m_labelYear);
			m_labelArray.push(m_labelMonth);
			m_labelArray.push(m_labelDate);
			m_labelArray.push(m_labelHour);
			m_labelArray.push(m_labelMinute);
			m_labelArray.push(m_labelSecond);
			for (var i:uint=0; i < m_labelArray.length; i++)
			{
				addChild(m_labelArray[i]);
				m_labelArray[i].setIconTextGap(0);
				m_labelArray[i].addEventListener(DateTimeLabelSelEvent.EVENTNAME_DTLS,
												 OnEventDTLSel);
			}
			m_btnSelectDate = createDropDownButton();
			addChild(m_btnSelectDate);
			m_btnSelectDate.addActionListener(OnEventSelectDate);
			m_btnSelectTimeUp = createDropDownButton(false);
			addChild(m_btnSelectTimeUp);
			m_btnSelectTimeUp.addActionListener(OnEventSelectTimeUp);
			m_btnSelectTimeDown = createDropDownButton();
			addChild(m_btnSelectTimeDown);
			m_btnSelectTimeDown.addActionListener(OnEventSelectTimeDown);
		
			var f:ASFont=new ASFont  ;
			setFont(f.changeSize(12));
			setEditable(false);
			setEnabled(false);
			setSizeWH(nDefaultWidth,nDefaultHeight);

			setDateTime(new Date);
			
			m_labelSecond.setSelected(true);
			
			setBorder(new LineBorder );
		}
		override public  function setSizeWH(w:int,h:int):void
		{
			super.setSizeWH(w,h);

			var nXP:Number=4;
			var nYP:Number=1;
			var nCY:Number=h - 2;

			var textS:IntDimension=AsWingUtils.computeStringSize(m_labelYear.getFont().getTextFormat(),"2007" + IDS_YEAR);
			textS.width=textS.width + 2;
			nCY=textS.height;
			nYP=h / 2 - nCY / 2;
			m_labelYear.setComBoundsXYWH(nXP,nYP,textS.width,nCY);
			nXP+= textS.width;

			textS=AsWingUtils.computeStringSize(m_labelMonth.getFont().getTextFormat(),"01" + IDS_MONTH);
			textS.width=textS.width + 2;
			m_labelMonth.setComBoundsXYWH(nXP,nYP,textS.width,nCY);
			nXP+= textS.width;

			textS=AsWingUtils.computeStringSize(m_labelDate.getFont().getTextFormat(),"01" + IDS_DATE);
			textS.width=textS.width + 2;
			m_labelDate.setComBoundsXYWH(nXP,nYP,textS.width,nCY);
			nXP+= textS.width + 5;

			textS=AsWingUtils.computeStringSize(m_labelHour.getFont().getTextFormat(),"01" + IDS_HOUR);
			textS.width=textS.width + 2;
			m_labelHour.setComBoundsXYWH(nXP,nYP,textS.width,nCY);
			nXP+= textS.width;

			textS=AsWingUtils.computeStringSize(m_labelMinute.getFont().getTextFormat(),"01" + IDS_MINUTE);
			textS.width=textS.width + 2;
			m_labelMinute.setComBoundsXYWH(nXP,nYP,textS.width,nCY);
			nXP+= textS.width;

			textS=AsWingUtils.computeStringSize(m_labelSecond.getFont().getTextFormat(),"01" + IDS_SECOND);
			textS.width=textS.width + 2;
			m_labelSecond.setComBoundsXYWH(nXP,nYP,textS.width,nCY);
			
			nCY = 20;
			nYP=h / 2 - nCY / 2;
			m_btnSelectDate.setComBoundsXYWH(w - nCY - nCY -4, nYP, nCY, nCY);
			m_btnSelectTimeUp.setComBoundsXYWH(w - nCY - 2, nYP, nCY, nCY/2);
			m_btnSelectTimeDown.setComBoundsXYWH(w - nCY - 2, nYP + nCY/2, nCY, nCY/2);
		}
		override public  function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if (m_labelYear)
			{
				for (var i:uint=0; i < m_labelArray.length; i++)
				{
					m_labelArray[i].setFont(newFont);
				}
				getDateChooseBox().setFont(newFont);
			}
		}
		public function getDateTime():Date
		{
			m_DateTime = new Date(m_labelYear.getNumValue(),
								m_labelMonth.getNumValue()-1,
								m_labelDate.getNumValue(),
								m_labelHour.getNumValue(),
								m_labelMinute.getNumValue(),
								m_labelSecond.getNumValue());
			return m_DateTime;
		}
		public function setDateTime(dt:Date):void
		{
			m_DateTime=dt;
			m_labelYear.setNumValue(m_DateTime.fullYear);
			m_labelMonth.setNumValue(m_DateTime.month + 1);
			m_labelDate.setNumValue(m_DateTime.date);
			m_labelHour.setNumValue(m_DateTime.hours);
			m_labelMinute.setNumValue(m_DateTime.minutes);
			m_labelSecond.setNumValue(m_DateTime.seconds);
			getDateChooseBox().setDateTime(m_DateTime);
		}
		public function OnEventDTLSel(e:DateTimeLabelSelEvent):void
		{
			var timelabel:DateTimeLabel = e.currentTarget as DateTimeLabel;
			for (var i:uint=0; i < m_labelArray.length; i++)
			{
				if (m_labelArray[i] == timelabel)
				{
					m_labelArray[i].setSelected(true);
				}
				else
				{
					m_labelArray[i].setSelected(false);
				}
			}
		}
		protected function createDropDownButton(bDown:Boolean = true):JButton
		{
			var btn:JButton=new JButton("",new ArrowIcon(bDown ? Math.PI / 2 : -Math.PI /2,8,new ASColor(0x000000),new ASColor(0x444444)));
			btn.setFocusable(false);
			btn.setPreferredSize(new IntDimension(20,20));
			return btn;
		}
		protected function getDateChooseBox():DateChooseBox
		{
			if(m_DateChooseBox == null)
			{
				m_DateChooseBox = new DateChooseBox;
				m_DateChooseBox.addEventListener(DateChooseOKEvent.EVENTNAME_DCOE, OnEventDateChooseOK);
			}
			return m_DateChooseBox;
		}
		protected function OnEventSelectDate(e:Event):void
		{
			var pt:IntPoint = m_btnSelectDate.getGlobalLocation();
			var id:IntDimension = getDateChooseBox().getSize();
			if(id.width == 0)
				id.width = DateChooseBox.nDefaultWidth;
			getDateChooseBox().setGlobalLocation(new IntPoint(pt.x - id.width/2, pt.y + nDefaultHeight));
			getDateChooseBox().show();
		}
		protected function OnEventSelectTimeUp(e:Event):void
		{
			for (var i:uint=0; i < m_labelArray.length; i++)
			{
				if (m_labelArray[i] && m_labelArray[i].getSelected())
				{
					m_labelArray[i].IncreaseOne();
					break;
				}
			}
		}
		protected function OnEventSelectTimeDown(e:Event):void
		{
			for (var i:uint=0; i < m_labelArray.length; i++)
			{
				if (m_labelArray[i] && m_labelArray[i].getSelected())
				{
					m_labelArray[i].DecreaseOne();
					break;
				}
			}
		}
		protected function OnEventDateChooseOK(e:DateChooseOKEvent):void
		{
			setDateTime(e.getDate());
		}
	}
}