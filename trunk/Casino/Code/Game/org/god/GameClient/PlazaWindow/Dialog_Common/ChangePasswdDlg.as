package 
{
	import flash.events.Event;
	import flash.utils.ByteArray;
	
	import org.god.Common.MD5;
	import org.god.Common.GlobalCommonMain;
	import org.god.IGameFrame.IMain;
	import org.god.SkinControl.BaseDialog;
	import org.god.SkinControl.SkinTextField;
	import org.god.SkinControl.DialogEvent;
	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_MyselfBasicUserInfo;
	import org.god.Net.CMD_GP_SetMySelfBasicUserInfoSuccess;
	import org.god.Net.CMD_GP_Error;
	import org.god.Net.IClientSocket;
	import org.god.Net.IClientSocketRecvSink;
	
	import org.aswing.JComboBox;
	import org.aswing.VectorListModel;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	import org.aswing.JLabel;
	import org.aswing.event.InteractiveEvent;
	import org.aswing.geom.IntRectangle;
	//修改密码对话框
	public class ChangePasswdDlg extends BaseDialog implements IClientSocketRecvSink
	{
		protected static const MyFontColor:ASColor = new ASColor(0xCC9900);
		protected static var MyFont:ASFont;

		protected var m_IMain:IMain;
		protected var m_ClientSocket:IClientSocket;
		protected var m_pSetBaiscInfoUserData:tagUserData;
		
		protected var m_labelTitle:JLabel;

		protected var m_tfName:SkinTextField;
		protected var m_tfPasswd:SkinTextField;
		protected var m_tfNewPasswd:SkinTextField;
		protected var m_tfPasswsAgain:SkinTextField;
		
		public function ChangePasswdDlg()
		{
			super(new IMAGE_CHANGEPASSWDDLG(449,285), null, null);
			if(MyFont == null)
			{
				 MyFont = new ASFont;
				 MyFont = MyFont.changeSize(12);
				 //MyFont = MyFont.changeBold(true);
			}
			m_IMain		   = GlobalCommonMain.getGlobalCommonMain().GetIGameFrameMain();
			m_ClientSocket = m_IMain.getClientSocket();
			
			m_pSetBaiscInfoUserData = m_IMain.getUserData();
			
			CreateControl();
			InitControl();
			setFont(MyFont);
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
		
			m_ClientSocket.AddSocketRecvSink(this as IClientSocketRecvSink);
			
		}
		public override function Destroy():void
		{
			m_ClientSocket.RemoveSocketRecvSink(this as IClientSocketRecvSink);

			removeChild(m_labelTitle);
			m_labelTitle = null;
			m_tfName.Destroy();
			removeChild(m_tfName);
			m_tfName = null;
			m_tfPasswd.Destroy();
			removeChild(m_tfPasswd);
			m_tfPasswd = null;
			m_tfNewPasswd.Destroy();
			removeChild(m_tfNewPasswd);
			m_tfNewPasswd = null;
			m_tfPasswsAgain.Destroy();
			removeChild(m_tfPasswsAgain);
			m_tfNewPasswd = null;
			
			m_IMain		   = null;
			m_ClientSocket = null;
			
			super.Destroy();
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
		
		//////////////////////////
		protected function ShowMessageBox(strMsg:String):void
		{
			m_IMain.ShowMessageBox(strMsg);
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
					
			var pBasicUserInfo:CMD_GP_MyselfBasicUserInfo = new CMD_GP_MyselfBasicUserInfo;
			pBasicUserInfo.dwUserID = m_pSetBaiscInfoUserData.dwUserID;
			pBasicUserInfo.szName = m_tfName.getText();
			pBasicUserInfo.szPassWord = MD5.hash(m_tfPasswd.getText());
			pBasicUserInfo.szNewPassWord = MD5.hash(m_tfNewPasswd.getText());
			pBasicUserInfo.szNewRealityPassWord = m_tfNewPasswd.getText();
			pBasicUserInfo.dwOperationUserID = m_IMain.getRealUserID();
			m_IMain.ShowStatusMessage(IDS_STATUS_SETBASICUSERINFO);

			var cbBuffer:ByteArray = pBasicUserInfo.toByteArray();

			SendData(MDM_GP_EDIT_USERINFO,
						SUB_GP_SET_MYSELFBASICUSERINFO,
						cbBuffer,
						CMD_GP_MyselfBasicUserInfo.sizeof_CMD_GP_MyselfBasicUserInfo);
			
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
					m_IMain.HideStatusMessage();
					Destroy();
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
		//发送函数
		protected function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean
		{
			if(m_ClientSocket == null)
				return false;
			return m_ClientSocket.SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
		}
	}
}

include "../../../Common/StringTableDef.as"
include "../../../GlobalConst.as"
include "../../../Net/GLOBALDEF.as"
include "../../../Net/CMD_Plaza.as"