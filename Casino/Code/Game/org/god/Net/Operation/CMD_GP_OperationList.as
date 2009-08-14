package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//操作记录
	public class CMD_GP_OperationList
	{
		public static  const sizeof_CMD_GP_OperationList:int=0;

		public var OperationDatas:Array = new Array();
		
		public function CMD_GP_OperationList()
		{
		}
		public static function readData(data:ByteArray,wDataSize:int):CMD_GP_OperationList
		{
			var pos:int=data.position;
			var result:CMD_GP_OperationList=new CMD_GP_OperationList;
			
			var count:int = wDataSize / tagOperationData.sizeof_tagOperationData;
			for(var i:int = 0; i < count; i ++)
			{
				var opdata:tagOperationData = tagOperationData.readData(data);
				result.OperationDatas.push(opdata);
				data.position += tagOperationData.sizeof_tagOperationData;
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"