package 
{
	import flash.display.Sprite;
	import flash.system.Security;
	import flash.system.ApplicationDomain;
	import flash.system.System;
	
	import org.god.ISWFModule.IModule;
	//模块类
	public class AccountPanel_CommonModule extends Sprite implements IModule
	{
		public var ClassMap:Array = [
									 "BasicSubUserInfoDlg",BasicSubUserInfoDlg,
									 "BasicUserInfoDlg",BasicUserInfoDlg,
									 "DetailUserInfoDlg",DetailUserInfoDlg,
									 "RegisterSubUserDlg",RegisterSubUserDlg,
									 "RegisterUserDlg",RegisterUserDlg,
									 "SetUserRightDlg",SetUserRightDlg,
									 "SetUserScoreDlg",SetUserScoreDlg
									 ];

		public function AccountPanel_CommonModule()
		{
			Security.allowDomain("*");
			System.useCodePage=true;
		}
		public function GetClass(strName:String):Class
		{
			var index:int = ClassMap.indexOf(strName);
			if(index == -1)
				return null;
			return ClassMap[index+1];
		}
		public function getApplicationDomain():ApplicationDomain
		{
			return ApplicationDomain.currentDomain;
		}
	}
}
