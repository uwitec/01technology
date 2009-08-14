package 
{
	import flash.display.Sprite;
	import flash.system.Security;
	import flash.system.ApplicationDomain;
	import flash.system.System;
	
	import org.god.ISWFModule.IModule;
	//模块类
	public class LogonWindowModule extends Sprite implements IModule
	{
		public var ClassMap:Array = [
									 "LogonWindow",LogonWindow
									 ];
		//构造函数
		public function LogonWindowModule()
		{
			Security.allowDomain("*");
			System.useCodePage=true;
		}
		//查询定义类
		public function GetClass(strName:String):Class
		{
			var index:int = ClassMap.indexOf(strName);
			if(index == -1)
				return null;
			return ClassMap[index+1];
		}
		//获取当前应用域
		public function getApplicationDomain():ApplicationDomain
		{
			return ApplicationDomain.currentDomain;
		}
	}
}
