package org.god.IGameFrame
{
	//全局类
	public class GlobalMain
	{
		private static var _main:IMain;
		public function GlobalMain()
		{
		}
		//获取主类接口
		public static function getMain():IMain
		{
			return _main;
		}
		//设置主类接口
		public static function setMain(main:IMain):void
		{
			_main = main;
		}
		//获取错误信息描述
		public static  function GetErrorDescribe(wErrCode:uint):String
		{
			return _main.GetErrorDescribe(wErrCode);
		}
	}
}