package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取操作记录
	public class CMD_GP_GetOperation
	{
		public static  const sizeof_CMD_GP_GetOperation:int=4 + 2 + 2 + NAME_LEN + 1 + 4 + 8 + 8;

		public var dwOperationUserID:uint;//操作用户ID
		public var wPageIndex:uint;//页索引
		public var wPageSize:uint;//页大小
		public var szAccount:String="";//帐号
		public var cbUserType:uint;//用户类型
		public var dwOPType:uint;//操作类型
		public var fBeginTime:Number;//操作开始时间
		public var fEndTime:Number;//操作开始时间

		public function CMD_GP_GetOperation()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray=newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeShort(wPageIndex);
			result.writeShort(wPageSize);
			writeStringToByteArray(result, szAccount, NAME_LEN);
			result.writeByte(cbUserType);
			result.writeUnsignedInt(dwOPType);
			result.writeDouble(fBeginTime);
			result.writeDouble(fEndTime);

			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"