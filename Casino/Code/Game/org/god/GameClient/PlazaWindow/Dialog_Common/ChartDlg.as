package 
{
	import flash.events.MouseEvent;
	import flash.utils.ByteArray;
	
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.DialogEvent;
	import org.god.SkinControl.SkinDateField.DateField;
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketRecvSink;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.Chart.CMD_GP_ChartViewThreeSucess;
	import org.god.Net.Chart.CMD_GP_GetChartViewThree;
	import org.god.Common.TimeUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.GameTypeAndGameKindUtil;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.SortTypeUtil;
	import org.god.Common.GameRoundTypeUtil;
	import org.god.Common.GameTypeCell;
	import org.god.Common.SortTypeCell;
	import org.god.Common.GameRoundTypeCell;
	import org.god.Common.GameKindCell;
	import org.god.Common.MasterRight;
	import org.god.Common.GlobalCommonMain;
	import org.god.IGameFrame.IMain;
	
	import org.aswing.JLabel;
	import org.aswing.JPanel;
	import org.aswing.JScrollPane;
	import org.aswing.BorderLayout;
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.geom.IntDimension;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.event.InteractiveEvent;
	//游戏报表对话框
	public class ChartDlg extends BaseDialog implements IClientSocketRecvSink
	{
		protected static const MyFontColor:ASColor = new ASColor(0xf1e8a5);
		protected static var MyFont:ASFont;
		
		protected var m_IMain:IMain;
		protected var m_ClientSocket:IClientSocket;
		
		protected var m_btnQuery:SkinButton;
		protected var m_dfEndTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelTo:JLabel;
		protected var m_dfBeginTime:org.god.SkinControl.SkinDateField.DateField;
		protected var m_labelDate:JLabel;

		protected var m_cbxBetValidType:JComboBox;
		protected var m_modelBetValidType:VectorListModel;
		protected var m_labelBetValidType:JLabel;
		protected var m_cbxGameKind:JComboBox;
		protected var m_modelGameKind:VectorListModel;
		protected var m_labelGameKind:JLabel;
		protected var m_cbxGameType:JComboBox;
		protected var m_modelGameType:VectorListModel;
		protected var m_labelGameType:JLabel;
		
		protected var m_labelTotalBetScore:JLabel;
		protected var m_labelTotalWinScore:JLabel;
		protected var m_labelTotalRollback:JLabel;

		public function ChartDlg()
		{
			super(new IMAGE_CHARTDLG(752,240),
				null,null,BaseDialog.DLGTYPE_OK);
			
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				// MyFont = MyFont.changeBold(false);
			}
			
			setFont(MyFont);
		}
		public override function Center(w:Number=1024,h:Number=768,hd:Number = -30):void
		{
			var rv:IntDimension = getSize();
			setGlobalLocationXY((w - rv.width)/2, (h-rv.height)/2 + hd);
			if(m_nDlgType == DLGTYPE_OK)
			{
				if(m_btnOK)
					m_btnOK.setLocationXY(rv.width - btn_rv.width - 16, 10);
			}
			else
			{
				if(m_btnOK)
					m_btnOK.setLocationXY(rv.width/2 -btn_rv.width- 25, rv.height - 75);
				if(m_btnCancel)
					m_btnCancel.setLocationXY(rv.width/2 + 25, rv.height - 75);
			}
			
		}
		public override function DoModal():void
		{
			Create(GlobalCommonMain.getGlobalCommonMain().GetIGameFrameMain());
			super.DoModal();
		}
		public function Create(param0:IMain):int
		{
			m_IMain		   = param0;
			m_ClientSocket = m_IMain.getClientSocket();
			
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;
			
			var nYP:Number = 14;
			var nXP:Number = 924;
			var nCX:Number = 0;
			var nCY:Number = 0;
			var nYPOffset:Number = 4;

			nYP = 10;
			nXP = 570;
			m_btnQuery = new SkinButton(IDS_QUERY, nXP, nYP);
			addChild(m_btnQuery);
			m_btnQuery.addEventListener(MouseEvent.CLICK, OnEventQuery);
			m_btnQuery.setFont(font.changeBold(false));

			nYP = 6;
			nXP = 356;
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
			
			////
			//
			nXP = 556;
			nYPOffset = 32;
			nXP-= 72;
			nYP = 10+ nYPOffset;
			m_modelBetValidType = new VectorListModel;
			m_cbxBetValidType = new JComboBox(m_modelBetValidType);
			addChild(m_cbxBetValidType);
			m_cbxBetValidType.setComBoundsXYWH(nXP, nYP, 70, 24);
			m_cbxBetValidType.setFont(font.changeBold(false));
			m_cbxBetValidType.setEditable(false);

			nXP-= 88;
			nYP = 12+ nYPOffset;
			m_labelBetValidType = new JLabel(IDS_VALID + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelBetValidType);
			m_labelBetValidType.setComBoundsXYWH(nXP, nYP, 86, 20);
			m_labelBetValidType.setFont(font);
			m_labelBetValidType.setForeground(MyFontColor);
			
			nXP-= 102;
			nYP = 10+ nYPOffset;
			m_modelGameKind = new VectorListModel;
			m_cbxGameKind = new JComboBox(m_modelGameKind);
			addChild(m_cbxGameKind);
			m_cbxGameKind.setComBoundsXYWH(nXP, nYP, 98, 24);
			m_cbxGameKind.setFont(font.changeBold(false));
			m_cbxGameKind.setEditable(false);
			m_cbxGameKind.addSelectionListener(OnEventSelChangeGameKind);

			nXP-= 67;
			nYP = 12+ nYPOffset;
			m_labelGameKind = new JLabel(IDS_GAMEKIND + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelGameKind);
			m_labelGameKind.setComBoundsXYWH(nXP, nYP, 67, 20);
			m_labelGameKind.setFont(font);
			m_labelGameKind.setForeground(MyFontColor);
			
			nXP-= 76;
			nYP = 10+ nYPOffset;
			m_modelGameType = new VectorListModel;
			m_cbxGameType = new JComboBox(m_modelGameType);
			addChild(m_cbxGameType);
			m_cbxGameType.setComBoundsXYWH(nXP, nYP, 72, 24);
			m_cbxGameType.setFont(font.changeBold(false));
			m_cbxGameType.setEditable(false);
			m_cbxGameType.addSelectionListener(OnEventSelChangeGameType);

			nXP-= 68;
			nYP = 12+ nYPOffset;
			m_labelGameType = new JLabel(IDS_GAMETYPE + IDS_COLON, null, JLabel.CENTER);
			addChild(m_labelGameType);
			m_labelGameType.setComBoundsXYWH(nXP, nYP, 68, 20);
			m_labelGameType.setFont(font);
			m_labelGameType.setForeground(MyFontColor);
			
			font = font.changeSize(24);
			nXP = 288;
			nYP = 60 + nYPOffset;
			m_labelTotalBetScore = new JLabel("", null, JLabel.LEFT);
			addChild(m_labelTotalBetScore);
			m_labelTotalBetScore.setComBoundsXYWH(nXP, nYP, 268, 28);
			m_labelTotalBetScore.setFont(font);
			//m_labelTotalBetScore.setForeground(MyFontColor);
			
			nYP += 36;
			m_labelTotalWinScore = new JLabel("", null, JLabel.LEFT);
			addChild(m_labelTotalWinScore);
			m_labelTotalWinScore.setComBoundsXYWH(nXP, nYP, 268, 28);
			m_labelTotalWinScore.setFont(font);
			//m_labelTotalWinScore.setForeground(MyFontColor);
			
			nYP += 36;
			m_labelTotalRollback = new JLabel("", null, JLabel.LEFT);
			addChild(m_labelTotalRollback);
			m_labelTotalRollback.setComBoundsXYWH(nXP, nYP, 268, 28);
			m_labelTotalRollback.setFont(font);
			//m_labelTotalRollback.setForeground(MyFontColor);
			
			//////////////////////

			m_dfBeginTime.setDateTime(TimeUtil.getQueryBeginTime(7));
			m_dfEndTime.setDateTime(TimeUtil.getQueryEndTime());
			
			var arrGameType:Array = GameTypeAndGameKindUtil.GetGameTypeArray();
			for(var i:uint = 0; i < arrGameType.length; i ++)
				m_modelGameType.append(new GameTypeCell(arrGameType[i]));
			m_cbxGameType.setSelectedIndex(0);
			
			UpdateGameKind();

			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Valid));
			m_modelBetValidType.append(new GameRoundTypeCell(GameRoundTypeUtil.GameRoundType_Invalid));
			m_cbxBetValidType.setSelectedIndex(0);
			
			SetChartInfo(0,0,0);
			
			m_btnQuery.setEnabled(MasterRight.CanChartView(m_IMain.getUserData().dwMasterRight));

			m_ClientSocket.AddSocketRecvSink(this as IClientSocketRecvSink);
			
			return 0;
		}
		public override function Destroy():void
		{
			m_ClientSocket.RemoveSocketRecvSink(this as IClientSocketRecvSink);

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
			
			removeChild(m_cbxBetValidType);
			m_cbxBetValidType = null;
			m_modelBetValidType = null;
			removeChild(m_labelBetValidType);
			m_labelBetValidType = null;
			m_cbxGameKind.removeSelectionListener(OnEventSelChangeGameKind);
			removeChild(m_cbxGameKind);
			m_cbxGameKind = null;
			m_modelGameKind = null;
			removeChild(m_labelGameKind);
			m_labelGameKind = null;
			m_cbxGameType.removeSelectionListener(OnEventSelChangeGameType);
			removeChild(m_cbxGameType);
			m_cbxGameType = null;
			m_modelGameType = null;
			removeChild(m_labelGameType);
			m_labelGameType = null;
			
			removeChild(m_labelTotalBetScore);
			m_labelTotalBetScore = null;
			removeChild(m_labelTotalWinScore);
			m_labelTotalWinScore = null;
			removeChild(m_labelTotalRollback);
			m_labelTotalRollback = null;
			
			m_IMain = null;
			m_ClientSocket = null;

			super.Destroy();
		}
		protected function UpdateGameKind():void
		{
			var gtc:GameTypeCell = m_cbxGameType.getSelectedItem() as GameTypeCell;
			var arrGameKind:Array = GameTypeAndGameKindUtil.GetGameKindArray(gtc.getGameType());
			m_modelGameKind.clear();
			for(var i:uint = 0; i < arrGameKind.length; i ++)
			{
				m_modelGameKind.append(new GameKindCell(arrGameKind[i]));
			}
			m_cbxGameKind.setSelectedIndex(0);
		}
		protected function SetChartInfo(fTotalBetScore:Number,fTotalWinScore:Number,
										fTotalRollback:Number):void
		{
			m_labelTotalBetScore.setText(IDS_CHTOTALBETSCORE + IDS_COLON + fTotalBetScore.toFixed(2));
			m_labelTotalWinScore.setText(IDS_CHTOTALWINSCORE + IDS_COLON + fTotalWinScore.toFixed(2));
			m_labelTotalRollback.setText(IDS_CHTOTALVALIDSCORE_LESS_ROLLBACK + IDS_COLON + fTotalRollback.toFixed(2));
		}
		protected function OnEventSelChangeGameKind(e:InteractiveEvent):void
		{
		
		}
		protected function OnEventSelChangeGameType(e:InteractiveEvent):void
		{
			UpdateGameKind();
		}
		protected function OnEventQuery(e:MouseEvent):void
		{
			var BeginTime:Date = m_dfBeginTime.getDateTime();
			var EndTime:Date = m_dfEndTime.getDateTime();
			if(BeginTime.getTime() >= EndTime.getTime())
			{
				m_IMain.ShowMessageBox(IDS_ERR_INVALIDQUERYTIME);
				return ;
			}
			if(TimeUtil.getTimeOffsetDates(BeginTime, EndTime) > 31)
			{
				m_IMain.ShowMessageBox(IDS_ERR_MAXQUERYDATE31);
				return ;
			}
			var gtc:GameTypeCell = m_cbxGameType.getSelectedItem() as GameTypeCell;
			var gkc:GameKindCell = m_cbxGameKind.getSelectedItem() as GameKindCell;
			var grtc:GameRoundTypeCell = m_cbxBetValidType.getSelectedItem() as GameRoundTypeCell;

			var pGetChartViewThree:CMD_GP_GetChartViewThree = new CMD_GP_GetChartViewThree;
			pGetChartViewThree.dwOperationUserID =  m_IMain.getRealUserID();
			pGetChartViewThree.dwUserID =  m_IMain.getUserData().dwUserID;
			pGetChartViewThree.fBeginTime =  BeginTime.getTime() / 1000;
			pGetChartViewThree.fEndTime =   EndTime.getTime() / 1000;
			pGetChartViewThree.wGameType    = gtc.getGameType();
			pGetChartViewThree.wGameKind    = gkc.getGameKind();
			pGetChartViewThree.cbGameRoundType   = grtc.getGameRoundType();
			
			m_IMain.ShowStatusMessage(IDS_STATUS_GETCHART);
			var cbBuffer:ByteArray = pGetChartViewThree.toByteArray();

			SendData(MDM_GP_GET_RECORD,
					SUB_GP_GET_CHARTVIEWTHREE,
					cbBuffer,
					CMD_GP_GetChartViewThree.sizeof_CMD_GP_GetChartViewThree);
		}
		public function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch(wMainCmdID)
			{
				case MDM_GP_GET_RECORD:
					return OnSocketGetRecord(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				default:
				break;
			}
			return false;
		}
		public function OnSocketGetRecord(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_CHARTVIEWTHREESUCCESS:
				{
					if(wDataSize != CMD_GP_ChartViewThreeSucess.sizeof_CMD_GP_ChartViewThreeSucess)return true;
					var pChartViewThreeSucess:CMD_GP_ChartViewThreeSucess = CMD_GP_ChartViewThreeSucess.readData(pBuffer);
					SetChartInfo(pChartViewThreeSucess.fTotalBetScore,
								 pChartViewThreeSucess.fTotalWinScore,
								 pChartViewThreeSucess.fValidScore_Less_Rollback);
					m_IMain.HideStatusMessage();
					return true;
				}
				case SUB_GP_CHARTVIEWTHREEERROR:
				{
					m_IMain.HideStatusMessage();
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
	
					return true;
				}
				default:
				break;
			}
			return false;
		}
		//发送函数
		protected function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean
		{
			if(m_ClientSocket == null)
				return false;
			return m_ClientSocket.SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
		}
	}
}
include "../../../Net/CMD_Plaza.as"
include "../../../Common/StringTableDef.as"
include "../../../Common/Chart/StringTableDef.as"