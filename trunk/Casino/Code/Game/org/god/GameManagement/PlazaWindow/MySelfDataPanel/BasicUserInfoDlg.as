package 
{
	import flash.events.Event;
	
	import org.god.Common.MD5;
	import org.god.IGameManagement.IMain;
	import org.god.IGameManagement.GlobalMain;
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.SkinTextField;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_MyselfBasicUserInfo;
	
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.JLabel;
	import org.aswing.event.InteractiveEvent;
	import org.aswing.geom.IntRectangle;
	//修改密码对话框
	public class BasicUserInfoDlg extends BaseDialog
	{
		protected var m_pSetBaiscInfoUserData:tagUserData;
		
		protected var m_labelTitle:JLabel;

		protected var m_tfName:SkinTextField;
		protected var m_tfPasswd:SkinTextField;
		protected var m_tfNewPasswd:SkinTextField;
		protected var m_tfPasswsAgain:SkinTextField;
		
		public function BasicUserInfoDlg(
											pSetBaiscInfoUserData:tagUserData,
										   pUserData:*,owner:* = null)
		{
			super(new IMAGE_DLG_BASICUSERINFO(449,285), pUserData, owner);
			
			m_pSetBaiscInfoUserData = pSetBaiscInfoUserData;
			
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
			
			m_tfName = new SkinTextField();
			m_tfPasswd = new SkinTextField();
			m_tfNewPasswd = new SkinTextField();
			m_tfPasswsAgain = new SkinTextField();
			addChild(m_tfName);
			addChild(m_tfPasswd);
			addChild(m_tfNewPasswd);
			addChild(m_tfPasswsAgain);
			m_tfName.setComBoundsXYWH(nXPos, nYPos, nWidth, nHeight);
			m_tfPasswd.setComBoundsXYWH(nXPos, nYPos + nYOffset, nWidth, nHeight);
			m_tfNewPasswd.setComBoundsXYWH(nXPos, nYPos+ nYOffset*2, nWidth, nHeight);
			m_tfPasswsAgain.setComBoundsXYWH(nXPos, nYPos+ nYOffset*3, nWidth, nHeight);
			m_tfName.setMaxChars(NAME_LEN-2);
			m_tfPasswd.setMaxChars(PASS_LEN/2);
			m_tfNewPasswd.setMaxChars(PASS_LEN/2);
			m_tfPasswsAgain.setMaxChars(PASS_LEN/2);
			m_tfPasswd.setOnlyASCIICharAndNumber(true);
			m_tfName.setOnlyASCIICharAndNumber(true);
			m_tfPasswd.setDisplayAsPassword(true);
			m_tfNewPasswd.setDisplayAsPassword(true);
			m_tfPasswsAgain.setDisplayAsPassword(true);

		}
		protected function InitControl():void
		{
			var strTitle:String = IDS_SET + IDS_BASICUSERINFO;
			m_labelTitle.setText(strTitle);
					
			m_tfName.setText(m_pSetBaiscInfoUserData.szName);
		
		}
		public override function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_tfPasswd)
			{
				m_tfName.setFont(newFont);
				m_tfPasswd.setFont(newFont);
				m_tfNewPasswd.setFont(newFont);
				m_tfPasswsAgain.setFont(newFont);
				
			}
		}
		public override function Destroy():void
		{
			m_pSetBaiscInfoUserData = null;
			
			removeChild(m_labelTitle);
			m_labelTitle = null;
			removeChild(m_tfName);
			m_tfName = null;
			removeChild(m_tfPasswd);
			m_tfPasswd = null;
			removeChild(m_tfNewPasswd);
			m_tfNewPasswd = null;
			removeChild(m_tfPasswsAgain);
			m_tfNewPasswd = null;
			
			super.Destroy();
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
			if(m_tfName.getText().length <= 0)
			{
				ShowMessageBox(IDS_ERR_NAMEEMPTY);
				m_tfName.makeFocus();
				return;
			}
			if(m_tfPasswd.getText().length <= 5)
			{
				ShowMessageBox(IDS_ERR_PASSWDEMPTY);
				m_tfNewPasswd.makeFocus();
				return;
			}
			if(m_tfNewPasswd.getText().length <= 5)
			{
				ShowMessageBox(IDS_ERR_PASSWDEMPTY);
				m_tfNewPasswd.makeFocus();
				return;
			}
			if(m_tfPasswsAgain.getText().length <= 5)
			{
				ShowMessageBox(IDS_ERR_PASSWDEMPTY);
				m_tfPasswsAgain.makeFocus();
				return;
			}
			if(m_tfNewPasswd.getText() != m_tfPasswsAgain.getText())
			{
				ShowMessageBox(IDS_ERR_PASSWDVERIFTY);
				m_tfPasswsAgain.makeFocus();
				return;
			}
					
			var BasicUserInfo:CMD_GP_MyselfBasicUserInfo = m_pUserData  as CMD_GP_MyselfBasicUserInfo;
			BasicUserInfo.dwUserID = m_pSetBaiscInfoUserData.dwUserID;
			BasicUserInfo.szName = m_tfName.getText();
			BasicUserInfo.szPassWord = MD5.hash(m_tfPasswd.getText());
			BasicUserInfo.szNewPassWord = MD5.hash(m_tfNewPasswd.getText());
			BasicUserInfo.szNewRealityPassWord = m_tfNewPasswd.getText();
			
			dispatchEvent(new DialogEvent(DialogEvent.CMD_OK, m_pUserData, null,m_pSetBaiscInfoUserData));
			
			Destroy();
		}
	}
}
include "./../StringTableDef.as"
include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/GLOBALDEF.as"