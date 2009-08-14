package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	
	import org.god.GameManagement.PlazaWindow.Common.BasePanel;
	
	import org.god.Net.IClientSocket;
	import org.god.IGameManagement.*;
	import org.god.Common.DrawUtil;
	import org.god.Common.TimeUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.MasterRight;
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.SkinDateField.DateField;
	
	import org.aswing.JTabbedPane;
	import org.aswing.JPanel;
	import org.aswing.JScrollPane;
	import org.aswing.JLabel;
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.BorderLayout;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.Insets;
	import org.aswing.event.InteractiveEvent;
	import org.aswing.event.AWEvent;
	//游戏报表面板
	public class ChartPanel extends BasePanel implements IPanelImpl
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		
		protected var m_btnQuery:SkinButton;
		protected var m_tabpaneChart:JTabbedPane;
		protected var m_tabChartViewOne:ChartViewOneTab;
		protected var m_tabChartViewTwo:ChartViewTwoTab;
		
		protected var m_dfEndTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelTo:JLabel;
		protected var m_dfBeginTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelDate:JLabel;

		public function ChartPanel()
		{
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(true);
			}
		}
		public function Create(param0:IMain,param1:IPlazaWindow):int
		{
			m_IMain		   = param0;
			m_IPlazaWindow = param1;
			m_ClientSocket = m_IMain.getClientSocket();
			
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;
			
			var nYP:Number = 14;
			var nXP:Number = 924;
			var nCX:Number = 0;
			var nCY:Number = 0;
			var nYPOffset:Number = 8;
	
			nXP	= 10;
			nYP = 14;
			nCX = 1004;
			nCY = 624;
			m_tabChartViewOne = new ChartViewOneTab();	
			m_tabChartViewTwo = new ChartViewTwoTab();	
			m_tabpaneChart = new JTabbedPane;
			m_tabpaneChart.setBorderInsets(new Insets(0,736,0,0));
			m_tabpaneChart.appendTab(m_tabChartViewOne, IDS_CHARTVIEWONE);
			m_tabpaneChart.appendTab(m_tabChartViewTwo, IDS_CHARTVIEWTWO);
			addChild(m_tabpaneChart);
			m_tabpaneChart.setFont(font.changeBold(false));
			m_tabpaneChart.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tabpaneChart.doLayout();
			m_tabChartViewOne.Create(m_IMain,m_IPlazaWindow);
			m_tabChartViewTwo.Create(m_IMain,m_IPlazaWindow);
			
			nYP = 14;
			nXP = 924;
			m_btnQuery = new SkinButton(IDS_QUERY, nXP, nYP, null, 92,32);
			addChild(m_btnQuery);
			m_btnQuery.addEventListener(MouseEvent.CLICK, OnEventQuery);
			m_btnQuery.setFont(font.changeBold(false));

			nYP = 9;
			nXP = 938;
			nXP-= 400;
			nYP = 10 + nYPOffset;
			m_dfEndTime = new org.god.SkinControl.SkinDateField.DateField;
			addChild(m_dfEndTime);
			m_dfEndTime.setLocationXY(nXP, nYP);
			
			nXP-= 20;
			nYP = 12+ nYPOffset;
			m_labelTo = new JLabel(IDS_TO, null, JLabel.CENTER);
			addChild(m_labelTo);
			m_labelTo.setComBoundsXYWH(nXP, nYP, 20, 20);
			m_labelTo.setFont(font);
			m_labelTo.setForeground(MyFontColor);
			
			nXP-= 200;
			nYP = 10+ nYPOffset;
			m_dfBeginTime = new org.god.SkinControl.SkinDateField.DateField;
			addChild(m_dfBeginTime);
			m_dfBeginTime.setLocationXY(nXP, nYP);
			
			nXP-= 38;
			nYP = 12+ nYPOffset;
			m_labelDate = new JLabel(IDS_TIME + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelDate);
			m_labelDate.setComBoundsXYWH(nXP, nYP, 38, 20);
			m_labelDate.setFont(font);
			m_labelDate.setForeground(MyFontColor);

			m_dfBeginTime.setDateTime(TimeUtil.getQueryBeginTime(7));
			m_dfEndTime.setDateTime(TimeUtil.getQueryEndTime());
			
			m_btnQuery.setEnabled(MasterRight.CanChartView(m_IMain.getUserData().dwMasterRight));

			return 0;
		}
		public function Destroy():void
		{
			m_btnQuery.removeEventListener(MouseEvent.CLICK, OnEventQuery);
			removeChild(m_btnQuery);
			m_btnQuery = null;
			
			removeChild(m_dfEndTime);
			m_dfEndTime = null;
			removeChild(m_labelTo);
			m_labelTo = null;
			removeChild(m_dfBeginTime);
			m_dfBeginTime = null;
			removeChild(m_labelDate);
			m_labelDate = null;
			
			removeChild(m_tabpaneChart);
			m_tabpaneChart = null;

			m_bmBackground.dispose();
			m_bmBackground = null;
			m_IMain = null;
		 	m_IPlazaWindow = null;
			m_ClientSocket = null;

		}
		public function getNavCmd():int
		{
			return nNavCmd_Chart;
		}
		public function getTitle():String
		{
			return IDS_PANELTITLE_CHART;
		}
		public function Active(data:*):int
		{
			doLayout();
			return 0;
		}
		public function Deactive():int
		{
			return 0;
		}
		public function getActiveDC():Sprite
		{
			return this;
		}
		public function moveDC(cb:IntRectangle):void
		{
			setComBounds(cb);
		}
		public function validateDC():void
		{
			validate();
			paintImmediately();
		}
		public function setVisibleDC(b:Boolean):void
		{
			setVisible(b);
		}
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			if(m_tabChartViewOne.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket))
				return true;
			if(m_tabChartViewTwo.OnSocketRead(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket))
				return true;
			return false;
		}
		/////////////////////////////////////////////
		override protected function DrawPanel(g:Graphics,pDC:BitmapData):void
		{
			DrawPanelTitle(pDC,getTitle());
		}
		override protected function SetupPanel():void
		{
			m_bmBackground = new IMAGE_PANEL(1024,635);
		}
		//////////////////////////////////////////////
		protected function OnEventQuery(e:MouseEvent):void
		{
			var BeginTime:Date = m_dfBeginTime.getDateTime();
			var EndTime:Date = m_dfEndTime.getDateTime();
			
			var nCurTabIndex:int = m_tabpaneChart.getSelectedIndex();
			switch(nCurTabIndex)
			{
				case 0:
				{
					m_tabChartViewOne.QueryResult(BeginTime,EndTime);
					break;
				}
				case 1:
				{
					m_tabChartViewTwo.QueryResult(BeginTime,EndTime);
					break;
				}
				default:
				break;
			}
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"