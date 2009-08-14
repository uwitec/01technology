package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//设定占成比
	public class tagOperationData_SetOccupancyRate
	{
		public static  const sizeof_tagOperationData_SetOccupancyRate:int=8+1+8+1+8+1+8 + 8+1+8+1+8+1+8;

		public var fOccupancyRate:Number;//占成比
		public var cbOccupancyRate_NoFlag:uint;//不占成比标志
		public var fLowOccupancyRate_Max:Number;//下线最高占成比
		public var cbLowOccupancyRate_Max_NoFlag:uint;//下线最高不占成比标志
		public var fLowOccupancyRate_Min:Number;//下线最低占成比
		public var cbLowOccupancyRate_Min_NoFlag:uint;//下线最低不占成比标志
	  public var fTaxOccupancyRate:Number;				//抽水占成比

		public var fOldOccupancyRate:Number;//占成比
		public var cbOldOccupancyRate_NoFlag:uint;//不占成比标志
		public var fOldLowOccupancyRate_Max:Number;//下线最高占成比
		public var cbOldLowOccupancyRate_Max_NoFlag:uint;//下线最高不占成比标志
		public var fOldLowOccupancyRate_Min:Number;//下线最低占成比
		public var cbOldLowOccupancyRate_Min_NoFlag:uint;//下线最低不占成比标志
		public var fOldTaxOccupancyRate:Number;				//抽水占成比

		public function tagOperationData_SetOccupancyRate()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_SetOccupancyRate
		{
			var pos:int=data.position;
			var result:tagOperationData_SetOccupancyRate=new tagOperationData_SetOccupancyRate;

			result.fOccupancyRate = data.readDouble();
			result.cbOccupancyRate_NoFlag = data.readUnsignedByte();
			result.fLowOccupancyRate_Max = data.readDouble();
			result.cbLowOccupancyRate_Max_NoFlag = data.readUnsignedByte();
			result.fLowOccupancyRate_Min = data.readDouble();
			result.cbLowOccupancyRate_Min_NoFlag = data.readUnsignedByte();
			result.fTaxOccupancyRate = data.readDouble();
			
			result.fOldOccupancyRate = data.readDouble();
			result.cbOldOccupancyRate_NoFlag = data.readUnsignedByte();
			result.fOldLowOccupancyRate_Max = data.readDouble();
			result.cbOldLowOccupancyRate_Max_NoFlag = data.readUnsignedByte();
			result.fOldLowOccupancyRate_Min = data.readDouble();
			result.cbOldLowOccupancyRate_Min_NoFlag = data.readUnsignedByte();
			result.fOldTaxOccupancyRate = data.readDouble();
			
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"