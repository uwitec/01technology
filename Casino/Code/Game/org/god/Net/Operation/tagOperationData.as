package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//操作记录数据
	public class tagOperationData
	{
		public static  const sizeof_tagOperationData:int=tagOperationHeader.sizeof_tagOperationHeader +
		tagOperationContent.sizeof_tagOperationContent;

		public var Header:tagOperationHeader;
		public var Content:tagOperationContent;
		
		public function tagOperationData()
		{
		}
		public static function readData(data:ByteArray):tagOperationData
		{
			var pos:int=data.position;
			var result:tagOperationData=new tagOperationData;
			result.Header = tagOperationHeader.readData(data);
			data.position = data.position + tagOperationHeader.sizeof_tagOperationHeader;
			result.Content = tagOperationContent.readData(data);
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"