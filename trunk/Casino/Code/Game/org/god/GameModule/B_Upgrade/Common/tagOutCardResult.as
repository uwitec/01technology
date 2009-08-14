package org.god.GameModule.B_Upgrade.Common
{
	//出牌结果
	public class tagOutCardResult
	{
		public var cbCardCount:uint;//扑克数目
		public var cbResultCard:Array=new Array(MAX_COUNT);//结果扑克
		
		public function tagOutCardResult()
		{
		}
		public static function ZeroData(data:tagOutCardResult):void
		{
			data.cbCardCount = 0;
			ZeroArray(data.cbResultCard);
		}
	}
}
include "../../../Common/Memory.as"
include "./GameLogicDef.as"