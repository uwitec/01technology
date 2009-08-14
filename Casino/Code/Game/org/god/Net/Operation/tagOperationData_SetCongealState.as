package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//设定状态
	public class tagOperationData_SetCongealState
	{
		public static  const sizeof_tagOperationData_SetCongealState:int=1 + 4;

		public var cbState:uint;//0-启用 1-停用
		public var dwStateMark:uint;//掩码

		public function tagOperationData_SetCongealState()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_SetCongealState
		{
			var pos:int=data.position;
			var result:tagOperationData_SetCongealState=new tagOperationData_SetCongealState;

			result.cbState = data.readUnsignedByte();
			result.dwStateMark = data.readUnsignedInt();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"