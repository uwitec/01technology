package
{
	import flash.display.BitmapData;
	//真人游戏游戏现场投注页面
	public class VideoMonitorTab extends BaseMonitorTab
	{
		public function VideoMonitorTab()
		{
			super();
		}
		protected override function CreateBackgroundBitmapData():BitmapData
		{
			return new IMAGE_BATTLE_TAB(820,548);
		}
		public override function GetMonitorTitle():String
		{
			return m_gameKind.toString() + "--" + m_gameServer.toString();
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"