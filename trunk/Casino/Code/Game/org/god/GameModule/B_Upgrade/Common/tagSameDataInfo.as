package org.god.GameModule.B_Upgrade.Common
{
	//连牌信息
	public class tagSameDataInfo
	{
		public var cbCardCount;//扑克数目
		public var cbBlockCount;//牌段数目
		public var cbSameData:Array = new Array(MAX_COUNT);//拖拉机牌
		
		public function tagSameDataInfo()
		{
		}
		public static function ZeroData(data:tagSameDataInfo):void
		{
			data.cbCardCount = 0;
			data.cbBlockCount = 0;
			ZeroArray(data.cbSameData);
		}
		public static function CopyData(dst:tagSameDataInfo,src:tagSameDataInfo):void
		{
			dst.cbCardCount = src.cbCardCount;
			dst.cbBlockCount = src.cbBlockCount;
			CopyArray(dst.cbSameData, src.cbSameData);
		}
	}
}
include "../../../Common/Memory.as"
include "./GameLogicDef.as"
