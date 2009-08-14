package 
{
	import flash.events.Event;
	
	import org.god.Common.MasterRight;
	import org.god.Common.UserRight;
	
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.GlobalMain;
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.SkinTextField;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_SetUserRight;
	
	import org.aswing.JCheckBox;
	import org.aswing.VectorListModel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.JLabel;
	import org.aswing.geom.IntRectangle;
	//设置用户权限对话框
	public class SetUserRightDlg extends BaseDialog
	{
		protected var m_pParentUserData:tagUserData;
		protected var m_pSetUserRightUserData:tagUserData;
		
		protected var m_labelTitle:JLabel;
		
		protected var m_tfAccount:SkinTextField;
		protected var m_arrMasterRightCheckBox:Array;
		protected var m_arrUserRightCheckBox:Array;
		
		public function SetUserRightDlg(pParentUserData:tagUserData,
											pSetUserRightUserData:tagUserData,
										   pUserData:*,owner:* = null)
		{
			super(new IMAGE_DLG_RIGHT(449,532), pUserData, owner);
			
			m_pParentUserData = pParentUserData;
			m_pSetUserRightUserData = pSetUserRightUserData;
			
			CreateControl();
			InitControl();
			
		}
		protected function CreateControl():void
		{
			m_labelTitle = new JLabel("",null,JLabel.CENTER);
			addChild(m_labelTitle);
			m_labelTitle.setComBounds(new IntRectangle(0,12,449,24));
			var MyFont:ASFont = new ASFont;
			MyFont = MyFont.changeSize(20);
			MyFont = MyFont.changeBold(true);
			m_labelTitle.setFont(MyFont);
			m_labelTitle.setForeground(new ASColor(0x020202));
			
			var nXPos:Number = 168;
			var nYPos:Number = 50;
			var nWidth:Number = 50;
			var nHeight:Number = 20;
			var nXOffset:Number = 58; 
			var nYOffset:Number = 21; 
			var nYOffset1:Number = 0;
			
			m_tfAccount = new SkinTextField();
			addChild(m_tfAccount);
			m_tfAccount.setComBoundsXYWH(nXPos, nYPos, 200, 22);
			m_tfAccount.setMaxChars(NAME_LEN);
			m_tfAccount.setOnlyASCIICharAndNumber(true);
			m_tfAccount.setEditable(false);
			
			nXPos = 168;
			nYPos = 98;
			
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
					nYOffset1 = 12;
				}
				if(r > 5)
				{
					nYOffset1 = 28;
				}
				m_arrMasterRightCheckBox[i].setComBoundsXYWH(nXPos + c *nXOffset ,
													   nYPos + r *nYOffset + nYOffset1,
													   nWidth,nHeight);
				if(r <= 1)
				{
					if(c == 2)
					{
						m_arrMasterRightCheckBox[i].setEnabled(false);
					}
					else
					{
						if(r == 0 && c == 0)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanAccountNew(m_pSetUserRightUserData.dwMasterRight));
						else if(r == 0 && c == 1)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanAccountSet(m_pSetUserRightUserData.dwMasterRight));
						else if(r == 1 && c == 0)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanSubAccountNew(m_pSetUserRightUserData.dwMasterRight));
						else if(r == 1 && c == 1)
							m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanSubAccountSet(m_pSetUserRightUserData.dwMasterRight));
					}
				}
				else if(r > 1 && r <= 5)
				{
					if(c != 2)
						m_arrMasterRightCheckBox[i].setEnabled(false);
					if(r == 2 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanChartView(m_pSetUserRightUserData.dwMasterRight));
					else if(r == 3 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanBetHistoryView(m_pSetUserRightUserData.dwMasterRight));
					else if(r == 4 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanBetMonitorView(m_pSetUserRightUserData.dwMasterRight));
					else if(r == 5 && c == 2)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanOperationView(m_pSetUserRightUserData.dwMasterRight));

				}
				else
				{
					if(c != 1)
						m_arrMasterRightCheckBox[i].setEnabled(false);
					if(r == 6 && c == 1)
						m_arrMasterRightCheckBox[i].setSelected(MasterRight.CanMyselfDataSet(m_pSetUserRightUserData.dwMasterRight));
				}
			}
			
			nXPos = 126;
			nYPos = 306;
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
				switch(i)
				{
					case 0:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayBattleGame(m_pSetUserRightUserData.dwUserRight));
					}
					break;
					
					case 1:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayVideoGame(m_pSetUserRightUserData.dwUserRight));
					}
					break;
					
					case 2:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayPokieGame(m_pSetUserRightUserData.dwUserRight));
					}
					break;
					
					case 3:
					{
						m_arrUserRightCheckBox[i].setSelected(UserRight.CanPlayLotteryGameGame(m_pSetUserRightUserData.dwUserRight));
					}
					break;
					
					default:
					break;
				}
			}
			
		}
		protected function InitControl():void
		{
			var strTitle:String = IDS_SET + IDS_RIGHT + 
			"[" + IDS_HIGHUSER + ":" + m_pParentUserData.szAccount + "]";
			m_labelTitle.setText(strTitle);
					
			m_tfAccount.setText(m_pSetUserRightUserData.szAccount);
		}
		public override function Destroy():void
		{
			//m_pParentUserData = null;
			//m_pSetUserRightUserData = null;
		
			removeChild(m_labelTitle);
			m_labelTitle = null;
		
			m_tfAccount.Destroy();
			removeChild(m_tfAccount);
			m_tfAccount = null;
			
			for(var i:uint = 0; i < m_arrMasterRightCheckBox.length; i ++)
			{
				removeChild(m_arrMasterRightCheckBox[i]);
				m_arrMasterRightCheckBox[i] = null;
			}
			m_arrMasterRightCheckBox = null;
			for(i = 0; i < m_arrUserRightCheckBox.length; i ++)
			{
				removeChild(m_arrUserRightCheckBox[i]);
			}
			m_arrUserRightCheckBox = null;
		
			super.Destroy();
		}
		public override function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			for(var i:uint = 0; i < m_arrMasterRightCheckBox.length; i ++)
			{
				m_arrMasterRightCheckBox[i].setFont(newFont);
			}
			for(i = 0; i < m_arrUserRightCheckBox.length; i ++)
			{
				m_arrUserRightCheckBox[i].setFont(newFont);
			}
			if(m_tfAccount)
			{
				m_tfAccount.setFont(newFont);
			}
		}
		
		//////////////////////////
		protected function ShowMessageBox(strMsg:String):void
		{
			var main:IMain = GlobalMain.getMain();
			if(main)
			{
				main.ShowMessageBox(strMsg);
			}
		}
		//////////////////////////
		
		override public function OnEventOK(e:Event):void
		{
			var SetUserRight:CMD_GP_SetUserRight = m_pUserData  as CMD_GP_SetUserRight;
			SetUserRight.dwUserID = m_pSetUserRightUserData.dwUserID;
			
			var dwMasterRight:uint = 0;
			for(var i:uint = 0; i < m_arrMasterRightCheckBox.length; i ++)
			{
				var r:uint = i /3;
				var c:uint = i % 3;
				
				if(r <= 1)
				{
					
					if(r == 0 && c == 0 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitAccountNew(dwMasterRight);
					else if(r == 0 && c == 1 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitAccountSet(dwMasterRight);
					else if(r == 1 && c == 0 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitSubAccountNew(dwMasterRight);
					else if(r == 1 && c == 1 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitSubAccountSet(dwMasterRight);
				}
				else if(r > 1 && r <= 5)
				{
					if(r == 2 && c == 2 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitChartView(dwMasterRight);
					else if(r == 3 && c == 2 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitBetHistoryView(dwMasterRight);
					else if(r == 4 && c == 2 && !m_arrMasterRightCheckBox[i].isSelected())
						dwMasterRight = MasterRight.LimitBetMonitorView(dwMasterRight);
					else if(r == 5 && c == 2&& !m_arrMasterRightCheckBox[i].isSelected() )
						dwMasterRight = MasterRight.LimitOperationView(dwMasterRight);

				}
				else
				{
					
					if(r == 6 && c == 1 && !m_arrMasterRightCheckBox[i].isSelected() )
						dwMasterRight = MasterRight.LimitMyselfDataSet(dwMasterRight);
				}
			}
			
			var dwUserRight:uint = 0;
			if(!m_arrUserRightCheckBox[0].isSelected())
				dwUserRight = UserRight.LimitPlayBattleGame(dwUserRight);
			if(!m_arrUserRightCheckBox[1].isSelected())
				dwUserRight = UserRight.LimitPlayVideoGame(dwUserRight);
			if(!m_arrUserRightCheckBox[2].isSelected())
				dwUserRight = UserRight.LimitPlayPokieGame(dwUserRight);
			if(!m_arrUserRightCheckBox[3].isSelected())
				dwUserRight = UserRight.LimitPlayLotteryGameGame(dwUserRight);
				
			
			if(m_pSetUserRightUserData.dwMasterRight != dwMasterRight ||
			   m_pSetUserRightUserData.dwUserRight != dwUserRight)
			{
				SetUserRight.dwMasterRight = dwMasterRight;
				SetUserRight.dwUserRight = dwUserRight;
				dispatchEvent(new DialogEvent(DialogEvent.CMD_OK, m_pUserData, m_pParentUserData,m_pSetUserRightUserData));
			}
			Destroy();
		}
	}
}
include "../../StringTableDef.as"
include "../../../../Common/StringTableDef.as"
include "../../../../GlobalConst.as"
include "../../../../Net/GLOBALDEF.as"