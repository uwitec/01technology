package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//新增下线帐号
	public class tagOperationData_NewAccount
	{
		public static  const sizeof_tagOperationData_NewAccount:int=1 + 8 + 1 + 8 + 4;

		public var cbLevel:uint;//等级
		public var fOccupancyRate:Number = 0;//占成比
		public var cbOccupancyRate_NoFlag:uint = 0;
		public var fRollbackRate:Number = 0;//洗码比
		public var dwBetLimit:uint;//上下限

		public function tagOperationData_NewAccount()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_NewAccount
		{
			var pos:int=data.position;
			var result:tagOperationData_NewAccount=new tagOperationData_NewAccount;

			result.cbLevel=data.readUnsignedByte();
			result.fOccupancyRate=data.readDouble();
			result.cbOccupancyRate_NoFlag= data.readUnsignedByte();
			result.fRollbackRate=data.readDouble();
			result.dwBetLimit=data.readUnsignedInt();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"