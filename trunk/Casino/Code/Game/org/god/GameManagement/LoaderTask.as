package
{
	import flash.system.ApplicationDomain;
	//装载任务
	public class LoaderTask
	{
		public  var nTaskIndex:int;//任务索引
		public  var strName:String = "";//任务名称
		public  var strURL:String = "";//任务地址
		public  var nAppMode:uint;//任务应用域模式
		public  var appDomain : ApplicationDomain;//任务应用域
		public  var bAppModeFlag:Boolean;//任务应用域标记
		
		public  var nLoaderIndex:int;//装载索引
		public  var bLoadingFlag:Boolean;//是否正在在装载
		//构造函数
		public function LoaderTask()
		{
			nLoaderIndex = -1;
			bLoadingFlag = false;
		}
		//销毁
		public function Destroy():void
		{
			strName = null;
			strURL = null;
			appDomain = null;
		}
	}
}