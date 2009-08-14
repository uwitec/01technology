package org.god.Net
{
	import flash.utils.ByteArray;

	//详细信息
	public class CMD_GP_DetailUserInfo
	{
		public static  const sizeof_CMD_GP_DetailUserInfo:uint = 
		4 + 4 + 8 + 1 + 8 + 1 + 8 + 1 + 8+ 8 + 4;

		public var dwOperationUserID:uint;//操作用户ID
		public var dwUserID:uint;//被操作用户ID
		public var fOccupancyRate:Number;//占成比
		public var cbOccupancyRate_NoFlag:uint;//不占成比标志
		public var fLowOccupancyRate_Max:Number;//下线最高占成比
		public var cbLowOccupancyRate_Max_NoFlag:uint;//下线最高不占成比标志
		public var fLowOccupancyRate_Min:Number;//下线最低占成比
		public var cbLowOccupancyRate_Min_NoFlag:uint;//下线最低不占成比标志
		public var fTaxOccupancyRate:Number;//抽水占成比
		public var fRollbackRate:Number;//洗码比
		public var dwBetLimit:uint;//上下限

		public function CMD_GP_DetailUserInfo()
		{

		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeUnsignedInt(dwOperationUserID);
			result.writeUnsignedInt(dwUserID);
			result.writeDouble(fOccupancyRate);
			result.writeByte(cbOccupancyRate_NoFlag);
			result.writeDouble(fLowOccupancyRate_Max);
			result.writeByte(cbLowOccupancyRate_Max_NoFlag);
			result.writeDouble(fLowOccupancyRate_Min);
			result.writeByte(cbLowOccupancyRate_Min_NoFlag);
			result.writeDouble(fTaxOccupancyRate);
			result.writeDouble(fRollbackRate);
			result.writeUnsignedInt(dwBetLimit);

			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"