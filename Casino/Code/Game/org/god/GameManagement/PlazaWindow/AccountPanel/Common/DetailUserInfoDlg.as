package 
{
	import flash.events.Event;
	
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.GlobalMain;
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.SkinTextField;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_DetailUserInfo;
	import org.god.Common.OccupancyRateCell;
	import org.god.Common.RollbackRateCell;
	
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.JLabel;
	import org.aswing.geom.IntRectangle;
	//修改用户详细信息对话框
	public class DetailUserInfoDlg extends BaseDialog
	{
		protected var m_pParentUserData:tagUserData;
		protected var m_pSetDetailInfoUserData:tagUserData;
		
		protected var m_labelTitle:JLabel;

		protected var m_tfAccount:SkinTextField;
			
		protected var m_modelOccupancyRate:VectorListModel;
		protected var m_modelLowOccupancyRate_Max:VectorListModel;
		protected var m_modelLowOccupancyRate_Min:VectorListModel;
		protected var m_modelTaxOccupancyRate:VectorListModel;
		protected var m_modelRollbackRate:VectorListModel;
		
		protected var m_cbxOccupancyRate:JComboBox;
		protected var m_cbxLowOccupancyRate_Max:JComboBox;
		protected var m_cbxLowOccupancyRate_Min:JComboBox;
		protected var m_cbxTaxOccupancyRate:JComboBox;
		protected var m_cbxRollbackRate:JComboBox;
		
		public function DetailUserInfoDlg(pParentUserData:tagUserData,
											 pSetDetailInfoUserData:tagUserData,
										   pUserData:*,owner:* = null)
		{
			super(new IMAGE_DLG_DETAILUSERINFO(449,285), pUserData, owner);
			
			m_pParentUserData = pParentUserData;
			m_pSetDetailInfoUserData = pSetDetailInfoUserData;
			
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

			var nXPos:Number = 150;
			var nYPos:Number = 68;
			var nWidth:Number = 200;
			var nHeight:Number = 22;
			var nYOffset:Number = 29; 
			
			m_tfAccount = new SkinTextField();
			addChild(m_tfAccount);
			m_tfAccount.setComBoundsXYWH(nXPos, nYPos, nWidth, nHeight);
			m_tfAccount.setMaxChars(NAME_LEN-2);
			m_tfAccount.setOnlyASCIICharAndNumber(true);
			m_tfAccount.setEditable(false);
			
			nYPos += nYOffset;
			nWidth = 70;
			nXPos += 45;
			nHeight += 2;
			m_modelOccupancyRate = new VectorListModel;
			m_modelLowOccupancyRate_Max = new VectorListModel;
			m_modelLowOccupancyRate_Min = new VectorListModel;
			m_modelTaxOccupancyRate = new VectorListModel;
			m_modelRollbackRate = new VectorListModel;
			m_cbxOccupancyRate = new JComboBox(m_modelOccupancyRate);
			m_cbxLowOccupancyRate_Max = new JComboBox(m_modelLowOccupancyRate_Max);
			m_cbxLowOccupancyRate_Min = new JComboBox(m_modelLowOccupancyRate_Min);
			m_cbxTaxOccupancyRate = new JComboBox(m_modelTaxOccupancyRate);
			m_cbxRollbackRate = new JComboBox(m_modelRollbackRate);
			//m_cbxOccupancyRate.setEditable(true);
			//m_cbxLowOccupancyRate_Max.setEditable(true);
			//m_cbxLowOccupancyRate_Min.setEditable(true);
			//m_cbxRollbackRate.setEditable(true);
			addChild(m_cbxOccupancyRate);
			addChild(m_cbxLowOccupancyRate_Max);
			addChild(m_cbxLowOccupancyRate_Min);
			addChild(m_cbxTaxOccupancyRate);
			addChild(m_cbxRollbackRate);
			m_cbxOccupancyRate.setComBoundsXYWH(nXPos, nYPos, nWidth, nHeight);
			m_cbxLowOccupancyRate_Max.setComBoundsXYWH(nXPos, nYPos + nYOffset, nWidth, nHeight);
			m_cbxLowOccupancyRate_Min.setComBoundsXYWH(nXPos, nYPos+ nYOffset*2, nWidth, nHeight);
			m_cbxTaxOccupancyRate.setComBoundsXYWH(nXPos, nYPos+ nYOffset*3, nWidth, nHeight);
			m_cbxRollbackRate.setComBoundsXYWH(nXPos, nYPos+ nYOffset*4, nWidth, nHeight);
			m_cbxOccupancyRate.setEditable(false);
			m_cbxLowOccupancyRate_Max.setEditable(false);
			m_cbxLowOccupancyRate_Min.setEditable(false);
			m_cbxTaxOccupancyRate.setEditable(false);
			m_cbxRollbackRate.setEditable(false);
			m_cbxOccupancyRate.addSelectionListener(OnEventSelChangeOccupancyRate);
			m_cbxLowOccupancyRate_Max.addSelectionListener(OnEventSelChangeOccupancyRate_Max);
			/*
			m_cbxOccupancyRate.setVisible(false);
			m_cbxLowOccupancyRate_Max.setVisible(false);
			m_cbxLowOccupancyRate_Min.setVisible(false);
			m_cbxRollbackRate.setVisible(false);*/
		}
		protected function InitControl():void
		{
			var strTitle:String = IDS_SET + IDS_DETAILUSERINFO  + 
			"[" + IDS_HIGHUSER + ":" + m_pParentUserData.szAccount + "]";
			m_labelTitle.setText(strTitle);
			
			
			m_tfAccount.setText(m_pSetDetailInfoUserData.szAccount);
			
			var fTmp_Min:Number = 0;
			var fTmp_Max:Number = m_pParentUserData.fRollbackRate;
			if(m_pParentUserData.cbLevel == 0)
				fTmp_Max = 0.01;
			
			do 
			{
				m_modelRollbackRate.append(new RollbackRateCell(fTmp_Max));		
				if(fTmp_Max == fTmp_Min)
					break;
				fTmp_Max -= 0.0005;
				if(fTmp_Max < fTmp_Min)
				{
					m_modelRollbackRate.append(new RollbackRateCell(fTmp_Min));
					break;
				}
			} while(true);
			m_cbxRollbackRate.setSelectedIndex(m_modelRollbackRate.getSize() -1);
			
			if(m_pParentUserData.cbLevel+1 < nMinUserLevel)
			{
				if((m_pParentUserData.cbLowOccupancyRate_Max_NoFlag ||
				m_pParentUserData.cbLowOccupancyRate_Min_NoFlag) || 
				   m_pParentUserData.cbLevel == 0)
				{
					m_modelOccupancyRate.append(new OccupancyRateCell(m_pParentUserData.fLowOccupancyRate_Max, 1));
				}
				fTmp_Max = m_pParentUserData.fLowOccupancyRate_Max;
				fTmp_Min = m_pParentUserData.fLowOccupancyRate_Min;
				do {
					m_modelOccupancyRate.append(new OccupancyRateCell(fTmp_Max));
					if(fTmp_Max == fTmp_Min)
						break;
					fTmp_Max -= 0.05;
					if(fTmp_Max < fTmp_Min)
					{
						m_modelOccupancyRate.append(new OccupancyRateCell(fTmp_Min));
						break;
					}
				} while(true);
			}
			else
			{
				m_modelOccupancyRate.append(new OccupancyRateCell(0.0));
			}
			m_cbxOccupancyRate.setSelectedIndex(0);
			
			if(m_pParentUserData.cbLevel+1 < nMinUserLevel)
			{
				fTmp_Max = m_pParentUserData.fTaxOccupancyRate;
				fTmp_Min = 0;
				do {
					m_modelTaxOccupancyRate.append(new OccupancyRateCell(fTmp_Max,0));
					if(fTmp_Max == fTmp_Min)
						break;
					fTmp_Max -= 0.05;
					if(fTmp_Max <= fTmp_Min)
					{
						m_modelTaxOccupancyRate.append(new OccupancyRateCell(fTmp_Min,0));
						break;
					}
				} while(true);
			}
			else
			{
				m_modelTaxOccupancyRate.append(new OccupancyRateCell(0.0,0));
			}
			m_cbxTaxOccupancyRate.setSelectedIndex(m_modelTaxOccupancyRate.getSize()-1);
			
			UpdateOccupancyRate();
			
			var ordata:OccupancyRateCell = 
			new OccupancyRateCell(m_pSetDetailInfoUserData.fOccupancyRate,
								  m_pSetDetailInfoUserData.cbOccupancyRate_NoFlag);
			for(var i:uint = 0; i < m_modelOccupancyRate.getSize(); i ++)
			{
				if(m_modelOccupancyRate.get(i).equal(ordata))
				{
					m_cbxOccupancyRate.setSelectedIndex(i);
					break;
				}
			}
			
			var ordataMax:OccupancyRateCell = 
			new OccupancyRateCell(m_pSetDetailInfoUserData.fLowOccupancyRate_Max,
								  m_pSetDetailInfoUserData.cbLowOccupancyRate_Max_NoFlag);
			for(i= 0; i < m_modelLowOccupancyRate_Max.getSize(); i ++)
			{
				if(m_modelLowOccupancyRate_Max.get(i).equal(ordataMax))
				{
					m_cbxLowOccupancyRate_Max.setSelectedIndex(i);
					break;
				}
			}
			var ordataMin:OccupancyRateCell = 
			new OccupancyRateCell(m_pSetDetailInfoUserData.fLowOccupancyRate_Min,
								  m_pSetDetailInfoUserData.cbLowOccupancyRate_Min_NoFlag);
			for(i = 0; i < m_modelLowOccupancyRate_Min.getSize(); i ++)
			{
				if(m_modelLowOccupancyRate_Min.get(i).equal(ordataMin))
				{
					m_cbxLowOccupancyRate_Min.setSelectedIndex(i);
						break;
				}
			}
			
			var ortaxdata:OccupancyRateCell = 
			new OccupancyRateCell(m_pSetDetailInfoUserData.fTaxOccupancyRate,
								  0);
			for(var i:uint = 0; i < m_modelTaxOccupancyRate.getSize(); i ++)
			{
				if(m_modelTaxOccupancyRate.get(i).equal(ortaxdata))
				{
					m_cbxTaxOccupancyRate.setSelectedIndex(i);
					break;
				}
			}
			
			var rrdata:RollbackRateCell = new RollbackRateCell(m_pSetDetailInfoUserData.fRollbackRate);
			for(i = 0; i < m_modelRollbackRate.getSize(); i ++)
			{
				if(m_modelRollbackRate.get(i).equal(rrdata))
				{
					m_cbxRollbackRate.setSelectedIndex(i);
						break;
				}
			}

			if(m_pParentUserData.cbLevel >= nMinUserLevel - 2)
			{
				m_cbxLowOccupancyRate_Max.setEnabled(false);
				m_cbxLowOccupancyRate_Min.setEnabled(false);
			}
			if(m_pParentUserData.cbLevel >= nMinUserLevel -1)
			{
				m_cbxOccupancyRate.setEnabled(false);
			}
			
		}
		public override function Destroy():void
		{
			//m_pParentUserData = null;
			//m_pSetDetailInfoUserData = null;
			
			removeChild(m_labelTitle);
			m_labelTitle = null;

			m_tfAccount.Destroy();
			removeChild(m_tfAccount);
			m_tfAccount = null;
			
			m_modelOccupancyRate = null;
			m_modelLowOccupancyRate_Max = null;
			m_modelLowOccupancyRate_Min = null;
			m_modelTaxOccupancyRate = null;
			m_modelRollbackRate = null;
		
			removeChild(m_cbxOccupancyRate);
			m_cbxOccupancyRate = null;
			removeChild(m_cbxLowOccupancyRate_Max);
			m_cbxLowOccupancyRate_Max = null;
			removeChild(m_cbxLowOccupancyRate_Min);
			m_cbxLowOccupancyRate_Min = null;
			removeChild(m_cbxTaxOccupancyRate);
			m_cbxTaxOccupancyRate = null;
			removeChild(m_cbxRollbackRate);
			m_cbxRollbackRate = null;
			
			super.Destroy();
		}
		public override function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_tfAccount)
			{
				m_tfAccount.setFont(newFont);
				
			}
			if(m_cbxOccupancyRate)
			{
				m_cbxOccupancyRate.setFont(newFont);
				m_cbxLowOccupancyRate_Max.setFont(newFont);
				m_cbxLowOccupancyRate_Min.setFont(newFont);
				m_cbxTaxOccupancyRate.setFont(newFont);
				m_cbxRollbackRate.setFont(newFont);
			}
		}
		protected function UpdateOccupancyRate():void
		{
			var ordataMax:OccupancyRateCell = m_cbxLowOccupancyRate_Max.getSelectedItem() as OccupancyRateCell;
			var ordataMin:OccupancyRateCell = m_cbxLowOccupancyRate_Min.getSelectedItem() as OccupancyRateCell;
			if(ordataMax == null)
				ordataMax = new OccupancyRateCell(0.0, 1) ;
			if(ordataMin == null)
				ordataMin = new OccupancyRateCell(0.0, 1) ;
			
			m_modelLowOccupancyRate_Max.clear();
			m_modelLowOccupancyRate_Min.clear();

			if(m_pParentUserData.cbLevel+1 < nMinUserLevel)
			{
				var fTmp_Min:Number = 0;
				var fTmp_Max:Number = 0;
			
				
				m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(0.0, 1));
				
				var ordata:OccupancyRateCell = m_cbxOccupancyRate.getSelectedItem();
				if(ordata == null)
					ordata = new OccupancyRateCell(0.0, 1) ;
				if(ordata.getOccupancyRate_NoFlag() == 1)
				{
					fTmp_Max = m_pParentUserData.fOccupancyRate;
					m_cbxTaxOccupancyRate.setSelectedIndex(m_modelTaxOccupancyRate.getSize()-1);
				}
				else
				{
					fTmp_Max = ordata.getOccupancyRate();
					for(var i:uint = 0; i < m_modelTaxOccupancyRate.getSize(); i ++)
					{
						if(m_modelTaxOccupancyRate.get(i).equal(ordata))
						{
							m_cbxTaxOccupancyRate.setSelectedIndex(i);
							break;
						}
					}
				}
				m_modelLowOccupancyRate_Max.append(new OccupancyRateCell(fTmp_Max, 1));
				fTmp_Min = 0;
				do {
					m_modelLowOccupancyRate_Max.append(new OccupancyRateCell(fTmp_Max));
					if(fTmp_Max == fTmp_Min)
						break;
					fTmp_Max -= 0.05;
					if(fTmp_Max < fTmp_Min)
					{
						m_modelLowOccupancyRate_Max.append(new OccupancyRateCell(fTmp_Min));
						break;
					}
				} while(true);
				m_cbxLowOccupancyRate_Max.setSelectedIndex(0);
				for(var i:uint = 0; i < m_modelLowOccupancyRate_Max.getSize(); i ++)
				{
					if(m_modelLowOccupancyRate_Max.get(i).equal(ordataMax))
					{
						m_cbxLowOccupancyRate_Max.setSelectedIndex(i);
						break;
					}
				}
				
				ordataMax = m_cbxLowOccupancyRate_Max.getSelectedItem() as OccupancyRateCell;
				if(ordataMax == null)
					ordataMax = new OccupancyRateCell(0.0, 1) ;
				if(ordataMax.getOccupancyRate_NoFlag() == 0)
				{
					fTmp_Max = ordataMax.getOccupancyRate();
				}
				else
				{
					if(ordata.getOccupancyRate_NoFlag() == 0)
						fTmp_Max = ordata.getOccupancyRate();
					else
						fTmp_Max = m_pParentUserData.fOccupancyRate;
				}
				fTmp_Min = 0;
				do {
					m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(fTmp_Max));
					if(fTmp_Max == fTmp_Min)
						break;
					fTmp_Max -= 0.05;
					if(fTmp_Max < fTmp_Min)
					{
						m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(fTmp_Min));
						break;
					}
				} while(true);
				m_cbxLowOccupancyRate_Min.setSelectedIndex(0);
				for(i = 0; i < m_modelLowOccupancyRate_Min.getSize(); i ++)
				{
					if(m_modelLowOccupancyRate_Min.get(i).equal(ordataMin))
					{
						m_cbxLowOccupancyRate_Min.setSelectedIndex(i);
						break;
					}
				}

			}
			else
			{
				m_modelLowOccupancyRate_Max.append(new OccupancyRateCell(0.0, 0));
				m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(0.0, 0));
				m_cbxLowOccupancyRate_Max.setSelectedIndex(0);
				m_cbxLowOccupancyRate_Min.setSelectedIndex(0);
			}
		}
		protected function UpdateOccupancyRate_Max():void
		{
			var ordataMax:OccupancyRateCell = m_cbxLowOccupancyRate_Max.getSelectedItem() as OccupancyRateCell;
			var ordataMin:OccupancyRateCell = m_cbxLowOccupancyRate_Min.getSelectedItem() as OccupancyRateCell;
			if(ordataMax == null)
				ordataMax = new OccupancyRateCell(0.0, 1) ;
			if(ordataMin == null)
				ordataMin = new OccupancyRateCell(0.0, 1) ;
	
			m_modelLowOccupancyRate_Min.clear();

			if(m_pParentUserData.cbLevel+1 < nMinUserLevel)
			{
				var fTmp_Min:Number = 0;
				var fTmp_Max:Number = 0;
			
				m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(0.0, 1));
				
				var ordata:OccupancyRateCell = m_cbxOccupancyRate.getSelectedItem();
				if(ordata == null)
					ordata = new OccupancyRateCell(0.0, 1) ;
				
				if(ordataMax.getOccupancyRate_NoFlag() == 0)
				{
					fTmp_Max = ordataMax.getOccupancyRate();
				}
				else
				{
					if(ordata.getOccupancyRate_NoFlag() == 0)
						fTmp_Max = ordata.getOccupancyRate();
					else
						fTmp_Max = m_pParentUserData.fOccupancyRate;
				}
				fTmp_Min = 0;
				do {
					m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(fTmp_Max));
					if(fTmp_Max == fTmp_Min)
						break;
					fTmp_Max -= 0.05;
					if(fTmp_Max < fTmp_Min)
					{
						m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(fTmp_Min));
						break;
					}
				} while(true);
				m_cbxLowOccupancyRate_Min.setSelectedIndex(0);
				for(var i:uint = 0; i < m_modelLowOccupancyRate_Min.getSize(); i ++)
				{
					if(m_modelLowOccupancyRate_Min.get(i).equal(ordataMin))
					{
						m_cbxLowOccupancyRate_Min.setSelectedIndex(i);
						break;
					}
				}
				
			}
			else
			{
				m_modelLowOccupancyRate_Min.append(new OccupancyRateCell(0.0, 0));
				m_cbxLowOccupancyRate_Min.setSelectedIndex(0);
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
		protected function OnEventSelChangeOccupancyRate(e:*):void
		{
			UpdateOccupancyRate();
		}
		protected function OnEventSelChangeOccupancyRate_Max(e:*):void
		{
			UpdateOccupancyRate_Max();
		}
		override public function OnEventOK(e:Event):void
		{
			if(m_tfAccount.getText().length <= 1)
			{
				ShowMessageBox(IDS_ERR_ACOUNTEMPTY);
				m_tfAccount.makeFocus();
				return;
			}
			
			var ordata:OccupancyRateCell = m_cbxOccupancyRate.getSelectedItem() as OccupancyRateCell;
	    	var ordataMax:OccupancyRateCell = m_cbxLowOccupancyRate_Max.getSelectedItem() as OccupancyRateCell;
			var ordataMin:OccupancyRateCell = m_cbxLowOccupancyRate_Min.getSelectedItem() as OccupancyRateCell;
			var ortaxdata:OccupancyRateCell = m_cbxTaxOccupancyRate.getSelectedItem() as OccupancyRateCell;
			if(ordata == null)
				ordata = new OccupancyRateCell(0.0, 1) ;
			if(ordataMax == null)
				ordataMax = new OccupancyRateCell(0.0, 1) ;
			if(ordataMin == null)
				ordataMin = new OccupancyRateCell(0.0, 1) ;
			if(ortaxdata == null)
				ortaxdata = new OccupancyRateCell(0.0, 0) ;
			var rrdata:RollbackRateCell = m_cbxRollbackRate.getSelectedItem() as RollbackRateCell;;
			if(rrdata == null)
				rrdata = new RollbackRateCell(0.0) ;
				
			var DetailUserInfo:CMD_GP_DetailUserInfo = m_pUserData  as CMD_GP_DetailUserInfo;
			DetailUserInfo.dwUserID = m_pSetDetailInfoUserData.dwUserID;
			DetailUserInfo.fOccupancyRate = ordata.getOccupancyRate();
			DetailUserInfo.cbOccupancyRate_NoFlag = ordata.getOccupancyRate_NoFlag();
			DetailUserInfo.fLowOccupancyRate_Max = ordataMax.getOccupancyRate();
			DetailUserInfo.cbLowOccupancyRate_Max_NoFlag = ordataMax.getOccupancyRate_NoFlag();
			DetailUserInfo.fLowOccupancyRate_Min = ordataMin.getOccupancyRate();
			DetailUserInfo.cbLowOccupancyRate_Min_NoFlag = ordataMin.getOccupancyRate_NoFlag();
			DetailUserInfo.fTaxOccupancyRate= ortaxdata.getOccupancyRate();
			DetailUserInfo.fRollbackRate = rrdata.getRollbackRate();
			
			dispatchEvent(new DialogEvent(DialogEvent.CMD_OK, m_pUserData, m_pParentUserData,m_pSetDetailInfoUserData));
			
			Destroy();
		}
	}
}
include "../../StringTableDef.as"
include "../../../../Common/StringTableDef.as"
include "../../../../GlobalConst.as"
include "../../../../Net/GLOBALDEF.as"