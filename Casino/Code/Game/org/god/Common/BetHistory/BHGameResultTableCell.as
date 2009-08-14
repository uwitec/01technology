package org.god.Common.BetHistory
{
	import org.god.Common.GameModuleNameFactory;
	import org.god.Common.GameEndReasonUtil;
	import org.god.Net.BetHistory.tagBetHistoryData;
	
	import org.aswing.ASColor;
	import org.aswing.JTable;
	import org.aswing.table.DefaultTextCell;
	//游戏结果表单元
	public class BHGameResultTableCell extends DefaultTextCell
	{
		
		public function BHGameResultTableCell()
		{
			super();

		}
		override public function setCellValue(value:*):void
		{
			this.value = value;
			var bh:tagBetHistoryData = value as tagBetHistoryData;
			if(bh)
			{
				setText(GetGameResultText(bh));
			}
		}
		protected function GetGameResultText(bh:tagBetHistoryData):String
		{
			var dwGameTypeMark:uint = GameModuleNameFactory.GetGameTypeMark(bh.wProcessType);
			switch(dwGameTypeMark)
			{
				case dwGameTypeMark_Battle:
					return GetGameResultText_Battle(bh);
				case dwGameTypeMark_Video:
					return GetGameResultText_Video(bh);
				case dwGameTypeMark_Pokie:
					return GetGameResultText_Pokie(bh);
				case dwGameTypeMark_Lottery:
					return GetGameResultText_Lottery(bh);
				default:
				break;
			}
			return IDS_NOTHING;
		}
		protected function GetGameResultText_Battle(bh:tagBetHistoryData):String
		{
			return GameEndReasonUtil.GetGameEndReasonText(bh.cbEndReason);
		}
		protected function GetGameResultText_Video(bh:tagBetHistoryData):String
		{
			return IDS_NOTHING;
		}
		protected function GetGameResultText_Pokie(bh:tagBetHistoryData):String
		{
			return IDS_NOTHING;
		}
		protected function GetGameResultText_Lottery(bh:tagBetHistoryData):String
		{
			return IDS_NOTHING;
		}
	}
}
include "./StringTableDef.as"
include "../../Net/GlobalDef.as"