package org.god.GameModule.B_Upgrade.Common
{
	//需求结构
	public class tagDemandInfo
	{
		public var cbMaxTractor:Array=new Array(2-1);//长度需求
		public var cbSameCardCount:Array=new Array(2);//同牌需求
		public var cbTractorCardCount:Array=new Array(2-1);//拖牌需求
		public function tagDemandInfo()
		{
		}
		public static function ZeroData(data:tagDemandInfo):void
		{
			ZeroArray(data.cbMaxTractor);
			ZeroArray(data.cbSameCardCount);
			ZeroArray(data.cbTractorCardCount);
		}
	}
}
include "../../../Common/Memory.as"
include "./GameLogicDef.as"