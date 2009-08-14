package org.god.Net
{
	import flash.utils.ByteArray;

	//修改详细资料成功
	public class CMD_GP_SetDetailUserInfoSuccess
	{
		public static  const sizeof_CMD_GP_SetDetailUserInfoSuccess:uint = 
		4 + 8 + 1 + 8 + 1 + 8 + 1 + 8 + 8 + 4;

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

		public function CMD_GP_SetDetailUserInfoSuccess()
		{

		}
		public static function readData(data:ByteArray):CMD_GP_SetDetailUserInfoSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetDetailUserInfoSuccess=new CMD_GP_SetDetailUserInfoSuccess;

			result.dwUserID= data.readUnsignedInt();
			result.fOccupancyRate = data.readDouble();
			result.cbOccupancyRate_NoFlag= data.readByte();
			result.fLowOccupancyRate_Max = data.readDouble();
			result.cbLowOccupancyRate_Max_NoFlag= data.readByte();
			result.fLowOccupancyRate_Min = data.readDouble();
			result.cbLowOccupancyRate_Min_NoFlag= data.readByte();
			result.fTaxOccupancyRate = data.readDouble();
			result.fRollbackRate= data.readDouble();
			result.dwBetLimit= data.readUnsignedInt();
			
			data.position=pos;
			return result;
		}
	}
}

include "../Common/Memory.as"