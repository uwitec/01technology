package org.god.Net.BetHistory
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录
	public class CMD_GP_GetDetailBetScore
	{
		public static  const sizeof_CMD_GP_GetDetailBetScore:int=4 + 8;

		public var dwOperationUserID:uint;//操作用户ID
		public var fUCID:Number;//报表记录ID

		public function CMD_GP_GetDetailBetScore()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeDouble(fUCID);

			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"