package
{
	//默认现场投注页面
	public class BlankMonitorTab extends BaseMonitorTab
	{
		public function BlankMonitorTab()
		{
			super();
		}
		override public function GetMonitorTitle():String
		{
			return IDS_DEFAULTMONITORPANE;
		}
	}
}
include "../ConstantDef.as"
include "../StringTableDef.as"
include "../../../Common/StringTableDef.as"