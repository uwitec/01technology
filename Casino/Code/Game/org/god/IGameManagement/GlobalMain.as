package org.god.IGameManagement
{
	//全局主类
	public class GlobalMain
	{
		private static var _main:IMain;
		public function GlobalMain()
		{
		}
		public static function getMain():IMain
		{
			return _main;
		}
		public static function setMain(main:IMain):void
		{
			_main = main;
		}
		public static  function GetErrorDescribe(wErrCode:uint):String
		{
			return _main.GetErrorDescribe(wErrCode);
		}
	}
}