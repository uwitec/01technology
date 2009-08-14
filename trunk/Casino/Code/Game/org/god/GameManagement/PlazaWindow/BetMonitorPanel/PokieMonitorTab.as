package
{
	import flash.display.BitmapData;
	//电子游戏现场投注按钮
	public class PokieMonitorTab extends BaseMonitorTab
	{
		public function PokieMonitorTab()
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