package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//设定密码及名称
	public class tagOperationData_SetBasicInfo
	{
		public static  const sizeof_tagOperationData_SetBasicInfo:int=NAME_LEN;

		public var szName:String = "";//名称

		public function tagOperationData_SetBasicInfo()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_SetBasicInfo
		{
			var pos:int=data.position;
			var result:tagOperationData_SetBasicInfo=new tagOperationData_SetBasicInfo;

			result.szName = readStringByByteArray(data, NAME_LEN);

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"