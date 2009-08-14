package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//设定洗码比
	public class tagOperationData_SetRollbackRate
	{
		public static  const sizeof_tagOperationData_SetRollbackRate:int=4 + 4;

		public var fRollbackRate:Number;//洗码比
		public var fOldRollbackRate:Number;//原洗码比

		public function tagOperationData_SetRollbackRate()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_SetRollbackRate
		{
			var pos:int=data.position;
			var result:tagOperationData_SetRollbackRate=new tagOperationData_SetRollbackRate;

			result.fRollbackRate = data.readDouble();
			result.fOldRollbackRate = data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"