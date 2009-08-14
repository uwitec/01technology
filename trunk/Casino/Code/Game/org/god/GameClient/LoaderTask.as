package
{
	import flash.system.ApplicationDomain;
	
	public class LoaderTask
	{
		public  var nTaskIndex:int;
		public  var strName:String = "";
		public  var strURL:String = "";
		public  var nAppMode:uint;
		public  var appDomain : ApplicationDomain;
		public  var bAppModeFlag:Boolean;
		
		public  var nLoaderIndex:int;
		public  var bLoadingFlag:Boolean;
		
		public function LoaderTask()
		{
			nLoaderIndex = -1;
			bLoadingFlag = false;
		}
		public function Destroy():void
		{
			strName = null;
			strURL = null;
			appDomain = null;
		}
	}
}