package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//设定权限
	public class tagOperationData_SetUserRight
	{
		public static  const sizeof_tagOperationData_SetUserRight:int=4 + 4 + 4 + 4;

		public var dwUserRight:uint;//游戏权限
		public var dwMasterRight:uint;//管理权限
		public var dwOldUserRight:uint;//旧游戏权限
		public var dwOldMasterRight:uint;//旧管理权限
		public function tagOperationData_SetUserRight()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_SetUserRight
		{
			var pos:int=data.position;
			var result:tagOperationData_SetUserRight=new tagOperationData_SetUserRight;

			result.dwUserRight = data.readUnsignedInt();
			result.dwMasterRight = data.readUnsignedInt();
			result.dwOldUserRight = data.readUnsignedInt();
			result.dwOldMasterRight = data.readUnsignedInt();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"