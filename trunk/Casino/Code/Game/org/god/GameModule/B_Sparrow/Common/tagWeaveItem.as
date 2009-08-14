package org.god.GameModule.B_Sparrow.Common
{
	//组合子项
	public class tagWeaveItem
	{
		public var cbWeaveKind:uint;//组合类型
		public var cbCenterSparrow:uint;//中心麻将
		public var cbPublicSparrow:uint;//公开标志
		public var wProvideUser:uint;//供应用户
		public function tagWeaveItem()
		{
		}
		public static function ZeroMemory(data:tagWeaveItem):void
		{
			data.cbWeaveKind = 0;
			data.cbCenterSparrow = 0;
			data.cbPublicSparrow = 0;
			data.wProvideUser = 0;
		}
	}
}
include "../../../Common/Memory.as"