package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//存取额度
	public class tagOperationData_SetScore
	{
		public static  const sizeof_tagOperationData_SetScore:int=8 + 8;

		public var fScoreResult:Number;//额度
		public var fSetScoreOffset:Number;//额度差

		public function tagOperationData_SetScore()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_SetScore
		{
			var pos:int=data.position;
			var result:tagOperationData_SetScore=new tagOperationData_SetScore;

			result.fScoreResult = data.readDouble();
			result.fSetScoreOffset = data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"