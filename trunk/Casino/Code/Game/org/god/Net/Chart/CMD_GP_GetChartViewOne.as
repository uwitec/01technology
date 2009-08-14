package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录
	public class CMD_GP_GetChartViewOne
	{
		public static  const sizeof_CMD_GP_GetChartViewOne:int=4 +1 +  NAME_LEN + 8 + 8 + 2 + 2 + 1 + 1;

		public var dwOperationUserID:uint;//操作用户ID
		public var cbParentFlag:uint;//查询上线标记
		public var szAccount:String = "";//帐号
		public var fBeginTime:Number = 0;//开始时间
		public var fEndTime:Number = 0;//结束时间
		public var wGameType:uint;//游戏类型
		public var wGameKind:uint;//游戏种类
		public var cbSortType:uint;//排序类型
		public var cbGameRoundType:uint;//结算有效性

		public function CMD_GP_GetChartViewOne()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeByte(cbParentFlag);
			writeStringToByteArray(result, szAccount, NAME_LEN);
			result.writeDouble(fBeginTime);
			result.writeDouble(fEndTime);
			result.writeShort(wGameType);
			result.writeShort(wGameKind);
			result.writeByte(cbSortType);
			result.writeByte(cbGameRoundType);
			
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"