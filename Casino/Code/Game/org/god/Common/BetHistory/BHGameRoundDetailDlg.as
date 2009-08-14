package org.god.Common.BetHistory
{
	import flash.display.BitmapData;
	
	import org.god.Common.FuncUtil;
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.GameUserStatusUtil;
	import org.god.Common.BetHistory.GameModule.Common.BaseGameEndView;
	import org.god.Common.BetHistory.GameModule.B_BlackJack.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_GoldFlower.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_Hoe.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_Land.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_SiChuan_Land.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_ShowHand.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_Sparrow.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_UncoverPig.GameEndView;
	import org.god.Common.BetHistory.GameModule.B_Upgrade.GameEndView;
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.ClientSocket;
	import org.god.Net.BetHistory.tagBetHistoryData;
	
	import org.aswing.JLabel;
	import org.aswing.JPanel;
	import org.aswing.JScrollPane;
	import org.aswing.BorderLayout;
	import org.aswing.geom.IntDimension;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	//详细游戏记录对话框
	public class BHGameRoundDetailDlg extends BaseDialog
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_strAccount:String;
		protected var m_bh:tagBetHistoryData;
		protected var m_ClientSocket:ClientSocket;
		protected var m_IBetHistoryResource:IBetHistoryResource;
		
		protected var m_labelAccount:JLabel;
		protected var m_labelGameRoundID:JLabel;
		protected var m_labelTime:JLabel;
		protected var m_labelGameKind:JLabel;
		protected var m_labelPosition:JLabel;
		protected var m_labelBetScore:JLabel;
		protected var m_labelWinScore:JLabel;
		protected var m_labelRollbackScore:JLabel;
		
		protected var m_tableGameRoundDetailBetScore:BHGameRoundDetailBetScoreTable;
		protected var m_scollpaneTable:JScrollPane;
		protected var m_panelTableContainer:JPanel;
		
		protected var m_BaseGameEndView:BaseGameEndView;

		public function BHGameRoundDetailDlg(strAccount:String,
											 bh:tagBetHistoryData,
											 pClientSocket:ClientSocket,
											 pIBetHistoryResource:IBetHistoryResource,
											 bmBackground:BitmapData,
											 pUserData:* = null,owner:* = null)
		{
			super(bmBackground,pUserData,owner,BaseDialog.DLGTYPE_OK);
			m_strAccount   = strAccount;
			m_bh		   = bh;
			m_ClientSocket = pClientSocket;
			m_IBetHistoryResource = pIBetHistoryResource;
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 MyFont = MyFont.changeBold(false);
			}
			var col:ASColor = MyFontColor;
			var font:ASFont = MyFont;
			
			setFont(font);

			m_labelAccount = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelAccount);
			m_labelAccount.setForeground(col);
			m_labelAccount.setFont(font);
			
			m_labelGameRoundID = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelGameRoundID);
			m_labelGameRoundID.setForeground(col);
			m_labelGameRoundID.setFont(font);
			
			m_labelTime = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelTime);
			m_labelTime.setForeground(col);
			m_labelTime.setFont(font);
			
			m_labelGameKind = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelGameKind);
			m_labelGameKind.setForeground(col);
			m_labelGameKind.setFont(font);
			
			m_labelPosition = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelPosition);
			m_labelPosition.setForeground(col);
			m_labelPosition.setFont(font);
			
			m_labelBetScore = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelBetScore);
			m_labelBetScore.setForeground(col);
			m_labelBetScore.setFont(font);
			
			m_labelWinScore = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelWinScore);
			m_labelWinScore.setForeground(col);
			m_labelWinScore.setFont(font);
			
			m_labelRollbackScore = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelRollbackScore);
			m_labelRollbackScore.setForeground(col);
			m_labelRollbackScore.setFont(font);
			
			m_tableGameRoundDetailBetScore = new BHGameRoundDetailBetScoreTable();
			m_scollpaneTable = new JScrollPane(m_tableGameRoundDetailBetScore);
			m_panelTableContainer = new JPanel(new BorderLayout());
			m_panelTableContainer.append(m_scollpaneTable, BorderLayout.CENTER);
			addChild(m_panelTableContainer);
			
			m_BaseGameEndView = CreateGameEndView(m_bh.wProcessType);
			addChild(m_BaseGameEndView);
			
			Center();
			UpdateGameRoundDetailDlg(m_strAccount,m_bh);
		}
		public override function Destroy():void
		{
			removeChild(m_labelAccount);
			m_labelAccount = null;
			removeChild(m_labelGameRoundID);
			m_labelGameRoundID = null;
			removeChild(m_labelTime);
			m_labelTime = null;
			removeChild(m_labelGameKind);
			m_labelGameKind = null;
			removeChild(m_labelPosition);
			m_labelPosition = null;
			removeChild(m_labelBetScore);
			m_labelBetScore = null;
			removeChild(m_labelWinScore);
			m_labelWinScore = null;
			removeChild(m_labelRollbackScore);
			m_labelRollbackScore = null;
		
			removeChild(m_panelTableContainer);
			m_panelTableContainer = null;
			m_tableGameRoundDetailBetScore = null;
			m_scollpaneTable = null;
		
			m_BaseGameEndView.Destroy();
			removeChild(m_BaseGameEndView);
			m_BaseGameEndView = null;

			super.Destroy();
		}
		public override function Center(w:Number=1024,h:Number=768,hd:Number = -30):void
		{
			var rv:IntDimension = getSize();
			setGlobalLocationXY((w - rv.width)/2, (h-rv.height)/2);
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
			RectifyGameRoundDetailDlg();
		}
		protected function RectifyGameRoundDetailDlg():void
		{
			if(m_labelAccount == null)
				return;
			var nYP:Number = 50;
			var nXP:Number = 16;
			var nCX:Number = 300;
			var nCY:Number = 20;
			
			m_labelAccount.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelGameRoundID.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelTime.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelGameKind.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelPosition.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelBetScore.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelWinScore.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			nYP += nCY;
			m_labelRollbackScore.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			
			nXP	= 20;
			nYP = 228;
			nCX = 264;
			nCY = 438;
			m_panelTableContainer.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_tableGameRoundDetailBetScore.MysetComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_panelTableContainer.doLayout();
			
			nXP	= 294;
			nYP = 47;
			nCX = 505;
			nCY = 405;
			//m_BaseGameEndView.setComBoundsXYWH(nXP, nYP, nCX, nCY);
			m_BaseGameEndView.MoveWindow(nXP, nYP);
		}
		protected function UpdateGameRoundDetailDlg(strAccount:String,
													bh:tagBetHistoryData):void
		{
			if(m_labelAccount == null)
				return;
			m_labelAccount.setText(IDS_ACCOUNT + IDS_COLON + strAccount);
			m_labelGameRoundID.setText(IDS_BHGAMEROUNDID + IDS_COLON + bh.fGameRoundID.toFixed(0));
			m_labelTime.setText(IDS_BHTIME + IDS_COLON + FuncUtil.fu_GetDateTimeString(bh.fStartTime));
			m_labelGameKind.setText(IDS_BHGAMEKIND + IDS_COLON + GameModuleNameFactory.GetGameKindPrixName(bh.wProcessType));
			m_labelPosition.setText(IDS_BHPOSITION + IDS_COLON +
									bh.szTypeName + "-" + bh.szKindName + "-" +
								    bh.szServerName + "-" + GameUserStatusUtil.GetTableIDName(bh.wTableID));
			m_labelBetScore.setText(IDS_BHBETSCORE + IDS_COLON + bh.fTotalBetScore.toFixed(2));
			m_labelWinScore.setText(IDS_BHWINSCORE + IDS_COLON + bh.fTotalWinScore.toFixed(2));
			m_labelRollbackScore.setText(IDS_BHROLLBACKSCORE + IDS_COLON + bh.fValidScore_Less_Rollback.toFixed(2));
		
			UpdateGameRoundDetailBetScoreTable(bh);
			
			m_BaseGameEndView.ShowWindow(true);
		}
		protected function UpdateGameRoundDetailBetScoreTable(bh:tagBetHistoryData):void
		{
			var dwGameTypeMark:uint = GameModuleNameFactory.GetGameTypeMark(bh.wProcessType);
			switch(dwGameTypeMark)
			{
				case dwGameTypeMark_Battle:
					{
						var strBetRegionNameArray:Array = [IDS_BR_B_BANK];
						var fBetScoreArray:Array = [bh.fTotalBetScore];
						var fWinScoreArray:Array = [bh.fTotalWinScore];
						
						m_tableGameRoundDetailBetScore.InsertItems(strBetRegionNameArray,
																  fBetScoreArray, 
																  fWinScoreArray);
						
						break;
					}
				case dwGameTypeMark_Video:
					{
						break;
					}
				case dwGameTypeMark_Pokie:
					{
						break;
					}
				case dwGameTypeMark_Lottery:
					{
						break;
					}
				default:
				break;
			}
		}
		protected function CreateGameEndView(wProcessType:uint):BaseGameEndView
		{
			switch(wProcessType)
			{
				case ID_B_HOE:
					return new org.god.Common.BetHistory.GameModule.B_Hoe.GameEndView(m_bh,m_IBetHistoryResource);
				case ID_B_LAND:
					return new org.god.Common.BetHistory.GameModule.B_Land.GameEndView(m_bh,m_IBetHistoryResource);
				case ID_B_SICHUAN_LAND:
					return new org.god.Common.BetHistory.GameModule.B_SiChuan_Land.GameEndView(m_bh,m_IBetHistoryResource);
			    case ID_B_UNCOVERPIG:
					return new org.god.Common.BetHistory.GameModule.B_UncoverPig.GameEndView(m_bh,m_IBetHistoryResource);
			    case ID_B_BACK_JACK:
					return new org.god.Common.BetHistory.GameModule.B_BlackJack.GameEndView(m_bh,m_IBetHistoryResource);
			    case ID_B_UPGRADE:
					return new org.god.Common.BetHistory.GameModule.B_Upgrade.GameEndView(m_bh,m_IBetHistoryResource);
			    case ID_B_SPARROW:
					return new org.god.Common.BetHistory.GameModule.B_Sparrow.GameEndView(m_bh,m_IBetHistoryResource);
			    case ID_B_SHOWHAND:
					return new org.god.Common.BetHistory.GameModule.B_ShowHand.GameEndView(m_bh,m_IBetHistoryResource);
			    case ID_B_THREELEAF:
					return new org.god.Common.BetHistory.GameModule.B_GoldFlower.GameEndView(m_bh,m_IBetHistoryResource);

			    case ID_L_MARKSIX:
					//return new L_MarkSix_GameEndView(m_bh,m_IBetHistoryResource);
				default:
					break;
			}
			return new BaseGameEndView(m_bh,m_IBetHistoryResource);
		}
	}
}
include "./StringTableDef.as"
include "../../Net/GlobalDef.as"