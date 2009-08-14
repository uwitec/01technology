package 
{
	import org.aswing.UIManager;
	import org.aswing.AsWingManager;
	import org.god.IGameManagement.GlobalMain;
	import org.god.Common.AppDisplayState;
	import org.god.Common.GlobalCommonMain;
	//初始化主类
	public class InitMain
	{
		//构造函数
		public function InitMain()
		{
		}
		//初始化
		public static function Init(main:Main):void
		{
			AppDisplayState.setMain(main, false);			
			GlobalMain.setMain(main);
			GlobalCommonMain.getGlobalCommonMain().SetIGameManagementMain(main);
			AsWingManager.initAsStandard(main);
			UIManager.setLookAndFeel(new GOMLAF());
		}
	}
}