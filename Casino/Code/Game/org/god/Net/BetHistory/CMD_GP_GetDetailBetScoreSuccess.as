package org.god.Net.BetHistory
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//游戏局记录详细投注信息
	public class CMD_GP_GetDetailBetScoreSuccess
	{
		public static  const sizeof_CMD_GP_GetDetailBetScoreSuccess:int=8 + 2;

		public var fUCID:Number;//报表记录ID

		public var wDetailBetScoreSize:uint;//投注详细大小
		public var cbDetailBetScore:ByteArray = newLitteEndianByteArray();//投注详细

		public function CMD_GP_GetDetailBetScoreSuccess()
		{
		}
		public function getBetHistoryDataSize():uint
		{
			return sizeof_CMD_GP_GetDetailBetScoreSuccess + wDetailBetScoreSize;
		}
		public static function readData(data:ByteArray):CMD_GP_GetDetailBetScoreSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_GetDetailBetScoreSuccess=new CMD_GP_GetDetailBetScoreSuccess;

			result.fUCID=data.readDouble();
			result.wDetailBetScoreSize=data.readUnsignedShort();

			result.cbDetailBetScore.length = result.wDetailBetScoreSize;
			if (result.cbDetailBetScore.length > 0)
			{
				CopyMemory(result.cbDetailBetScore, 
				   data, 
				   result.wDetailBetScoreSize,
				   0, data.position);
			}

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"