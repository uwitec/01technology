package 
{
	import flash.display.Sprite;
	import flash.system.Security;
	import flash.system.ApplicationDomain;
	import flash.system.System;
	
	import org.god.ISWFModule.IModule;
	//模块类
	public class PlazaWindow_CommonModule extends Sprite implements IModule
	{
		public var ClassMap:Array = [
									 "ChartDlg",ChartDlg,
									  "BetHistoryDlg",BetHistoryDlg,
									  "OperationDlg",OperationDlg,
									  "ChangePasswdDlg",ChangePasswdDlg
									 ];

		public function PlazaWindow_CommonModule()
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
