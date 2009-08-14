package org.god.GameModule.B_Upgrade.Common
{
	//牌型结构
	public class tagAnalyseResult
	{
		public var cbCardColor:uint;//扑克花色
		public var SameDataInfo:Array = new Array(2);//同牌数据
		public var TractorDataInfo:Array = new Array(2-1);//拖牌数据
		
		public function tagAnalyseResult()
		{
			for(var i:uint = 0;i < SameDataInfo.length; i ++)
			{
				SameDataInfo[i] = new tagSameDataInfo;
			}
			for(i = 0;i < TractorDataInfo.length; i ++)
			{
				TractorDataInfo[i] = new tagTractorDataInfo;
			}
		}
		public static function ZeroData(data:tagAnalyseResult):void
		{
			data.cbCardColor = 0;
			EachArray(data.SameDataInfo,tagSameDataInfo.ZeroData);
			EachArray(data.TractorDataInfo,tagTractorDataInfo.ZeroData);
		}
		public static function CopyData(dst:tagAnalyseResult,src:tagAnalyseResult):void
		{
			dst.cbCardColor = src.cbCardColor;
			for(var i:uint = 0;i < dst.SameDataInfo.length; i ++)
			{
				tagSameDataInfo.CopyData(dst.SameDataInfo[i],src.SameDataInfo[i]);
			}
			for(i = 0;i < dst.TractorDataInfo.length; i ++)
			{
				tagTractorDataInfo.CopyData(dst.TractorDataInfo[i],src.TractorDataInfo[i]);
			}
		}
	}
}
include "../../../Common/Memory.as"
include "./GameLogicDef.as"