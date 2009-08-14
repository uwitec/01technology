package 
{
	import flash.events.Event;
	
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.GlobalMain;
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.SkinTextField;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_SetUserScore;
	
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.JLabel;
	import org.aswing.geom.IntRectangle;
	//存取信用额度对话框
	public class SetUserScoreDlg extends BaseDialog
	{
		protected var m_pParentUserData:tagUserData;
		protected var m_pSetScoreUserData:tagUserData;
		
		protected var m_labelTitle:JLabel;
		protected var m_labelParentScore:JLabel;
		
		protected var m_tfAccount:SkinTextField;
		protected var m_tfScore:SkinTextField;
		
		public function SetUserScoreDlg(pParentUserData:tagUserData,
										   pSetScoreUserData:tagUserData,
										   pUserData:*,owner:* = null)
		{
			super(new IMAGE_DLG_SETUSERSCORE(449,285), pUserData, owner);
			
			m_pParentUserData = pParentUserData;
			m_pSetScoreUserData = pSetScoreUserData;
			
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
			var nYPos:Number = 74;
			var nWidth:Number = 200;
			var nHeight:Number = 22;
			var nYOffset:Number = 29; 
			
			m_tfAccount = new SkinTextField();
			m_tfScore = new SkinTextField();
			m_labelParentScore = new JLabel("",null,JLabel.LEFT);
			addChild(m_tfAccount);
			addChild(m_tfScore);
			addChild(m_labelParentScore);
			m_tfAccount.setComBoundsXYWH(nXPos, nYPos, nWidth, nHeight);
			m_tfScore.setComBoundsXYWH(nXPos, nYPos+ nYOffset, nWidth/2, nHeight);
			m_labelParentScore.setComBoundsXYWH(nXPos + nWidth/2, nYPos+ nYOffset, nWidth/2 + 120, nHeight);
			m_tfAccount.setMaxChars(NAME_LEN);
			m_tfAccount.setOnlyASCIICharAndNumber(true);
			m_tfAccount.setEditable(false);
			m_tfScore.setOnlyNumber(true);
			
		}
		protected function InitControl():void
		{
			var strTitle:String = IDS_SETUSERSCORE + 
			"[" + IDS_HIGHUSER + ":" + m_pParentUserData.szAccount + "]";
			m_labelTitle.setText(strTitle);
			
			var strTmp:String = "/" + m_pParentUserData.UserScore.fScore.toFixed(2);
			m_labelParentScore.setText(String(strTmp));
			
			m_tfAccount.setText(m_pSetScoreUserData.szAccount);
			m_tfScore.setText(m_pSetScoreUserData.UserScore.fScore.toFixed(2));
		}
		public override function Destroy():void
		{
			m_pParentUserData = null;
			m_pSetScoreUserData = null;
		
			removeChild(m_labelTitle);
			m_labelTitle = null;
			removeChild(m_labelParentScore);
			m_labelParentScore = null;
		
			m_tfAccount.Destroy();
			removeChild(m_tfAccount);
			m_tfAccount = null;
			m_tfScore.Destroy();
			removeChild(m_tfScore);
			m_tfScore = null;
			
			super.Destroy();
		}
		public override function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_tfAccount)
			{
				m_tfAccount.setFont(newFont);
				
				m_tfScore.setFont(newFont);
				m_labelParentScore.setFont(newFont);
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
			if(m_tfScore.getNumber() == m_pSetScoreUserData.UserScore.fScore)
			{
				Destroy();
				return;
			}

			var SetUserScore:CMD_GP_SetUserScore = m_pUserData  as CMD_GP_SetUserScore;
			SetUserScore.dwParentUserID =m_pParentUserData.dwUserID;
			SetUserScore.dwUserID =m_pSetScoreUserData.dwUserID;
			SetUserScore.fScore =m_tfScore.getNumber();

			dispatchEvent(new DialogEvent(DialogEvent.CMD_OK, m_pUserData, m_pParentUserData, m_pSetScoreUserData));
			
			Destroy();
		}
	}
}
include "../../StringTableDef.as"
include "../../../../Common/StringTableDef.as"
include "../../../../GlobalConst.as"
include "../../../../Net/GLOBALDEF.as"