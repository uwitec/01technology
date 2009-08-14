package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//操作记录数据描述
	public class tagOperationContent
	{
		public static  const sizeof_tagOperationContent:int=128;

		public var data_Nothing:ByteArray = newLitteEndianByteArray();
		
		public function tagOperationContent()
		{
		}
		public static function readData(data:ByteArray):tagOperationContent
		{
			var pos:int=data.position;
			var result:tagOperationContent=new tagOperationContent;
			result.data_Nothing.length = sizeof_tagOperationContent;
			CopyMemory(result.data_Nothing, 
					   data, 
					   sizeof_tagOperationContent,
					   0, pos);

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"