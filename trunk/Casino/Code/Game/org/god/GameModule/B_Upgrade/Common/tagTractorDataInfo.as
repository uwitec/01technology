package org.god.GameModule.B_Upgrade.Common
{
	//拖拉机信息
	public class tagTractorDataInfo
	{
		public var cbCardCount:uint;//扑克数目
		public var cbTractorCount:uint;//拖牌个数
		public var cbTractorMaxIndex:uint;//索引位置
		public var cbTractorMaxLength:uint;//最大长度
		public var cbTractorData:Array =new Array(uint(MAX_COUNT*3/2));//拖拉机牌
		
		public function tagTractorDataInfo()
		{
		}
		public static function ZeroData(data:tagTractorDataInfo):void
		{
			data.cbCardCount = 0;
			data.cbTractorCount = 0;
			data.cbTractorMaxIndex = 0;
			data.cbTractorMaxLength = 0;
			ZeroArray(data.cbTractorData);
		}
		public static function CopyData(dst:tagTractorDataInfo,src:tagTractorDataInfo):void
		{
			dst.cbCardCount = src.cbCardCount;
			dst.cbTractorCount = src.cbTractorCount;
			dst.cbTractorMaxIndex = src.cbTractorMaxIndex;
			dst.cbTractorMaxLength = src.cbTractorMaxLength;
			CopyArray(dst.cbTractorData, src.cbTractorData);
		}
	}
}
include "../../../Common/Memory.as"
include "./GameLogicDef.as"