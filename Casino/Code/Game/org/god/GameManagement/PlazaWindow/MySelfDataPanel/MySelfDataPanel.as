package 
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.utils.ByteArray;
	import flash.events.MouseEvent;
	
	import org.god.GameManagement.PlazaWindow.Common.BasePanel;
	import org.god.GameManagement.PlazaWindow.Common.ActiveOperationPanelData;
	import org.god.SkinControl.SkinButton;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.IClientSocket;
	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_MyselfBasicUserInfo;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.CMD_GP_SetMySelfBasicUserInfoSuccess;
	import org.god.IGameManagement.IPlazaWindow;
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.IPanelImpl;
	import org.god.Common.DrawUtil;
	import org.god.Common.FuncUtil;
	import org.god.Common.MasterRight;
	import org.god.Common.UserRight;
	
	import org.aswing.JCheckBox;
	import org.aswing.JLabel;
	import org.aswing.JPanel;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	//自身资料面板
	public class MySelfDataPanel extends BasePanel implements IPanelImpl
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;
		
		protected var m_labelAccount:JLabel;
		protected var m_labelAccountDescribe:JLabel;
		protected var m_labelName:JLabel;
		protected var m_labelScore:JLabel;
		protected var m_labelRegisterTime:JLabel;
		
		protected var m_labelOccupancyRate:JLabel;
		protected var m_labelLowOccupancyRate_Max:JLabel;
		protected var m_labelLowOccupancyRate_Min:JLabel;
		protected var m_labelRollbackRate:JLabel;
		
		protected var m_arrMasterRightCheckBox:Array;
		protected var m_arrUserRightCheckBox:Array;
		
		protected var m_btnChangePassword:SkinButton;
		protected var m_btnOperation:SkinButton;
		
		public function MySelfDataPanel()
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
			
			var xpos:Number = 398;
			var ypos:Number = 81;
			var xd:Number = 0;
			var yd:Number = 25;
			var ch:Number = 28;
			var cx:Number = 640;
			
			m_labelAccount = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelAccount);
			m_labelAccount.setForeground(col);
			m_labelAccount.setFont(font);
			m_labelAccount.setComBounds(new IntRectangle(xpos,ypos,cx,ch));
			m_labelAccountDescribe = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelAccountDescribe);
			m_labelAccountDescribe.setForeground(col);
			m_labelAccountDescribe.setFont(font);
			m_labelAccountDescribe.setComBounds(new IntRectangle(xpos,ypos + yd,cx,ch));
			m_labelName = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelName);
			m_labelName.setForeground(col);
			m_labelName.setFont(font);
			m_labelName.setComBounds(new IntRectangle(xpos,ypos + yd*2,cx,ch));
			m_labelScore = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelScore);
			m_labelScore.setForeground(col);
			m_labelScore.setFont(font);
			m_labelScore.setComBounds(new IntRectangle(xpos,ypos + yd*3,cx,ch));
			m_labelRegisterTime = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelRegisterTime);
			m_labelRegisterTime.setForeground(col);
			m_labelRegisterTime.setFont(font);
			m_labelRegisterTime.setComBounds(new IntRectangle(xpos,ypos + yd*4,cx,ch));
			
			ypos = 259;
			m_labelOccupancyRate = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelOccupancyRate);
			m_labelOccupancyRate.setForeground(col);
			m_labelOccupancyRate.setFont(font);
			m_labelOccupancyRate.setComBounds(new IntRectangle(xpos,ypos,cx,ch));
			m_labelLowOccupancyRate_Max = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelLowOccupancyRate_Max);
			m_labelLowOccupancyRate_Max.setForeground(col);
			m_labelLowOccupancyRate_Max.setFont(font);
			m_labelLowOccupancyRate_Max.setComBounds(new IntRectangle(xpos,ypos + yd,cx,ch));
			m_labelLowOccupancyRate_Min = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelLowOccupancyRate_Min);
			m_labelLowOccupancyRate_Min.setForeground(col);
			m_labelLowOccupancyRate_Min.setFont(font);
			m_labelLowOccupancyRate_Min.setComBounds(new IntRectangle(xpos,ypos + yd*2,cx,ch));
			m_labelRollbackRate = new JLabel("",null,JLabel.LEFT);
			addChild(m_labelRollbackRate);
			m_labelRollbackRate.setForeground(col);
			m_labelRollbackRate.setFont(font);
			m_labelRollbackRate.setComBounds(new IntRectangle(xpos,ypos + yd*3,cx,ch));
			
			m_btnChangePassword = new SkinButton(IDS_CHANGEPASSWD, 858, 50, null, 92,32);
			addChild(m_btnChangePassword);
			m_btnChangePassword.addEventListener(MouseEvent.CLICK, OnEventChangePassword);
			m_btnChangePassword.setFont(font.changeBold(false));
			m_btnOperation = new SkinButton(IDS_OPERATION, 858, 80, null, 92,32);
			addChild(m_btnOperation);
			m_btnOperation.addEventListener(MouseEvent.CLICK, OnEventOperation);
			m_btnOperation.setFont(font.changeBold(false));
			
			font = font.changeSize(12);
			var nXPos:Number = 360;
			var nYPos:Number = 426;
			var nWidth:Number = 50;
			var nHeight:Number = 20;
			var nXOffset:Number = 52; 
			var nYOffset:Number = 25; 
			var nYOffset1:Number = 0;
			m_arrMasterRightCheckBox = new Array(7 * 3);
			for(var i:uint = 0; i < m_arrMasterRightCheckBox.length; i ++)
			{
				var r:uint = i /3;
				var c:uint = i % 3;
				if(c == 0)
					m_arrMasterRightCheckBox[i] = new JCheckBox(IDS_NEW);
				else if(c == 1)
					m_arrMasterRightCheckBox[i] = new JCheckBox(IDS_SET);
				else
					m_arrMasterRightCheckBox[i] = new JCheckBox(IDS_VIEW);
				addChild(m_arrMasterRightCheckBox[i]);
				if(r > 1)
				{
					nYOffset1 = 8;
				}
				if(r > 5)
				{
					nYOffset1 = 14;
				}
				m_arrMasterRightCheckBox[i].setComBoundsXYWH(nXPos + c *nXOffset ,
													   nYPos + r *nYOffset + nYOffset1,
													   nWidth,nHeight);
				m_arrMasterRightCheckBox[i].setEnabled(false);
				m_arrMasterRightCheckBox[i].setForeground(col);
				m_arrMasterRightCheckBox[i].setFont(font);
			}
			
			nXPos = 548;
			nYPos = 426;
			nWidth= 200;
			
			var arrUserRightData:Array = [IDS_CANPLAY + IDS_BATTLEGAME,
										  IDS_CANPLAY + IDS_VIDEOGAME,
										  IDS_CANPLAY + IDS_POKIEGAME,
										  IDS_CANPLAY + IDS_LOTTERYGAME];
			m_arrUserRightCheckBox = new Array(4);
			for(i = 0; i < m_arrUserRightCheckBox.length; i ++)
			{
				m_arrUserRightCheckBox[i] = new JCheckBox(arrUserRightData[i]);
				addChild(m_arrUserRightCheckBox[i]);
				
				m_arrUserRightCheckBox[i].setComBoundsXYWH(nXPos ,
													   nYPos + i *nYOffset,
													   nWidth, nHeight);
				m_arrUserRightCheckBox[i].setEnabled(false);
				m_arrUserRightCheckBox[i].setForeground(col);
				m_arrUserRightCheckBox[i].setFont(font);
			}
			
			return 0;
		}
		public function Destroy():void
		{
			removeChild(m_labelAccount);
			removeChild(m_labelAccountDescribe);
			removeChild(m_labelName);
			removeChild(m_labelScore);
			removeChild(m_labelRegisterTime);
		
			removeChild(m_labelOccupancyRate);
			removeChild(m_labelLowOccupancyRate_Max);
			removeChild(m_labelLowOccupancyRate_Min);
			removeChild(m_labelRollbackRate);
			
			m_labelAccount = null;
			m_labelAccountDescribe = null;
			m_labelName = null;
			m_labelScore = null;
			m_labelRegisterTime = null;
		
			m_labelOccupancyRate = null;
			m_labelLowOccupancyRate_Max = null;
			m_labelLowOccupancyRate_Min = null;
			m_labelRollbackRate = null;
			
			for(var i:uint = 0; i < m_arrMasterRightCheckBox.length; i ++)
			{
				removeChild(m_arrMasterRightCheckBox[i]);
				m_arrMasterRightCheckBox[i] = null;
			}
			m_arrMasterRightCheckBox = null;
			
			for(i = 0; i < m_arrUserRightCheckBox.length; i ++)
			{
				removeChild(m_arrUserRightCheckBox[i]);
				m_arrUserRightCheckBox[i] = null;
			}
			m_arrUserRightCheckBox = null;
			
			m_btnChangePassword.removeEventListener(MouseEvent.CLICK, OnEventChangePassword);
			removeChild(m_btnChangePassword);
			m_btnChangePassword = null;
			m_btnOperation.removeEventListener(MouseEvent.CLICK, OnEventOperation);
			removeChild(m_btnOperation);
			m_btnOperation = null;
			
			m_bmBackground.dispose();
			m_bmBackground = null;
			m_IMain = null;
		 	m_IPlazaWindow = null;
			m_ClientSocket = null;

		}
		public function getNavCmd():int
		{
			return nNavCmd_MySelfData;
		}
		public function getTitle():String
		{
			return IDS_PANELTITLE_MYSELFDATA;
		}
		public function Active(data:*):int
		{
			doLayout();
			
			var UserData:tagUserData = m_IMain.getUserData();
			m_labelAccount.setText(m_IMain.getUserAccount());
			if(m_IMain.getUserType() == 0)
				m_labelAccountDescribe.setText(FuncUtil.fu_GetPeerLevelName(UserData.cbLevel));
			else if(m_IMain.getUserType() == 1)
				m_labelAccountDescribe.setText(FuncUtil.fu_GetPeerLevelName(UserData.cbLevel) + "-" + IDS_SUBUSER);
			m_labelName.setText(UserData.szName);
			
			var fMidScore:Number = UserData.fMidScore;
			if(UserData.cbLevel == nMinUserLevel-1 ||
			   UserData.cbLevel == nMinUserLevel)
				fMidScore = 0;
			var strTmp:String = UserData.UserScore.fScore.toFixed(2) + "   "
			+ IDS_MIDSCORE + ":" + fMidScore.toFixed(2) + "   " 
			+ IDS_GAMEUSER + ":" + UserData.fLowScore.toFixed(2);
			m_labelScore.setText(strTmp);
			m_labelRegisterTime.setText(FuncUtil.fu_GetDateTimeString(UserData.fRegisterTime));
			
			m_labelOccupancyRate.setText(FuncUtil.fu_GetOccupancyRateName(UserData.fOccupancyRate, 
														   UserData.cbOccupancyRate_NoFlag));
			m_labelLowOccupancyRate_Max.setText(FuncUtil.fu_GetOccupancyRateName(UserData.fLowOccupancyRate_Max, 
												UserData.cbLowOccupancyRate_Max_NoFlag));
			m_labelLowOccupancyRate_Min.setText(FuncUtil.fu_GetOccupancyRateName(UserData.fLowOccupancyRate_Min, 
												UserData.cbLowOccupancyRate_Min_NoFlag));
			m_labelRollbackRate.setText(FuncUtil.fu_GetRollbackRate(UserData.fRollbackRate));
			
			for(var i:uint = 0; i < m_arrMasterRightCheckBox.length; i ++)
			{
				var r:uint = i /3;
				var c:uint = i % 3;
				
				if(r <= 1)
				{
					if(c == 2)
					{
						
					}
					else
					{
						if(r == 0 && c == 0)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanAccountNew(UserData.dwMasterRight)&& MasterRight.SimpleCheck_UserRight());
						else if(r == 0 && c == 1)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanAccountSet(UserData.dwMasterRight)&& MasterRight.SimpleCheck_UserRight());
						else if(r == 1 && c == 0)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanSubAccountNew(UserData.dwMasterRight)&& MasterRight.SimpleCheck_UserRight());
						else if(r == 1 && c == 1)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanSubAccountSet(UserData.dwMasterRight)&& MasterRight.SimpleCheck_UserRight());
					}
				}
				else if(r > 1 && r <= 5)
				{
					
					if(r == 2 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanChartView(UserData.dwMasterRight));
					else if(r == 3 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanBetHistoryView(UserData.dwMasterRight));
					else if(r == 4 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanBetMonitorView(UserData.dwMasterRight));
					else if(r == 5 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanOperationView(UserData.dwMasterRight));

				}
				else
				{
					
					if(r == 6 && c == 1)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanMyselfDataSet(UserData.dwMasterRight));
				}
			}
			for(i = 0; i < m_arrUserRightCheckBox.length; i ++)
			{
				
				switch(i)
				{
					case 0:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayBattleGame(UserData.dwUserRight));
					}
					break;
					
					case 1:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayVideoGame(UserData.dwUserRight));
					}
					break;
					
					case 2:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayPokieGame(UserData.dwUserRight));
					}
					break;
					
					case 3:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayLotteryGameGame(UserData.dwUserRight));
					}
					break;
					
					default:
					break;
				}
			}
			
			m_btnChangePassword.setEnabled(MasterRight.CanMyselfDataSet(UserData.dwMasterRight));
			m_btnOperation.setEnabled(MasterRight.CanOperationView(UserData.dwMasterRight));

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
			switch(wMainCmdID)
			{
				case MDM_GP_EDIT_USERINFO:
					return OnSocketEditUser(wMainCmdID,wSubCmdID,pBuffer,wDataSize,pIClientSocket);
				default:
					break;
			}
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
		protected function OnEventChangePassword(e:MouseEvent):void
		{
			var UserData:tagUserData = m_IMain.getUserData();
			var BasicUserInfo:CMD_GP_MyselfBasicUserInfo = new CMD_GP_MyselfBasicUserInfo;
			var dlgBasicUserInfo:BasicUserInfoDlg = new BasicUserInfoDlg(UserData,
																		BasicUserInfo);
			dlgBasicUserInfo.setFont(MyFont.changeBold(false));
			dlgBasicUserInfo.addDialogEventListener(OnDlgEventSetBasicUserInfo);
			dlgBasicUserInfo.DoModal();
		}
		protected function OnEventOperation(e:MouseEvent):void
		{
			var ad:ActiveOperationPanelData = new ActiveOperationPanelData;
			ad.szAccount = m_IMain.getUserData().szAccount;
			m_IPlazaWindow.activePanel(nNavCmd_Operation,ad);
		}
		protected function OnDlgEventSetBasicUserInfo(e:DialogEvent):void
		{
			switch(e.GetCmd())
			{
				case DialogEvent.CMD_OK:
				{
					m_IMain.ShowStatusMessage(IDS_STATUS_SETBASICUSERINFO);
					
					var pBasicUserInfo:CMD_GP_MyselfBasicUserInfo =  e.GetUserObject();
					
					pBasicUserInfo.dwOperationUserID = m_IMain.getRealUserID();
					
					var cbBuffer:ByteArray = pBasicUserInfo.toByteArray();

					SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_MYSELFBASICUSERINFO,
						cbBuffer,
						CMD_GP_MyselfBasicUserInfo.sizeof_CMD_GP_MyselfBasicUserInfo);
				}
				break;
			
				default:
				break;
			}
		}
		///////////////////////////////////////////////////////////////////
		public function OnSocketEditUser(wMainCmdID:uint,
										   wSubCmdID:uint,
										   pBuffer:ByteArray,
										   wDataSize:int,
										   pIClientSocket:IClientSocket):Boolean
		{
			switch(wSubCmdID)
			{
				case SUB_GP_SET_MYSELFBASICUSERINFO_SUCCESS:
				{
					//效验数据
					if(wDataSize != CMD_GP_SetMySelfBasicUserInfoSuccess.sizeof_CMD_GP_SetMySelfBasicUserInfoSuccess)
						return true;
					//变量定义
					var pSetMySelfBasicUserInfoSuccess:CMD_GP_SetMySelfBasicUserInfoSuccess=
					CMD_GP_SetMySelfBasicUserInfoSuccess.readData(pBuffer);
					
					m_IMain.setUserPasswd(pSetMySelfBasicUserInfoSuccess.szNewRealityPassWord);
					var UserData:tagUserData = m_IMain.getUserData();
					UserData.szName = pSetMySelfBasicUserInfoSuccess.szName;
					m_IMain.setUserData(UserData);
					m_labelName.setText(pSetMySelfBasicUserInfoSuccess.szName);
					m_IMain.HideStatusMessage();
					return true;
					
				}
				case SUB_GP_SET_MYSELFBASICUSERINFO_ERROR:
				{
					m_IMain.HideStatusMessage();
					var pError:CMD_GP_Error = CMD_GP_Error.readData(pBuffer);
					m_IMain.ShowErrorMessageBox(pError.wErrorCode);
					return true;
				}
			}
			return false;
		}
	}
}
include "./../ConstantDef.as"
include "./../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/CMD_Plaza.as"
