package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//设定密码及名称
	public class tagOperationData_KickUser
	{
		public static  const sizeof_tagOperationData_KickUser:int=NAME_LEN + NAME_LEN + NAME_LEN + 2;

		public var szTypeName:String = "";//种类名字
		public var szKindName:String = "";//游戏名字
		public var szServerName:String = "";//房间名称
		public var wTableID:uint;//桌ID

		public function tagOperationData_KickUser()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_KickUser
		{
			var pos:int=data.position;
			var result:tagOperationData_KickUser=new tagOperationData_KickUser;

			result.szTypeName = readStringByByteArray(data, NAME_LEN);
			result.szKindName = readStringByByteArray(data, NAME_LEN);
			result.szServerName = readStringByByteArray(data, NAME_LEN);
			result.wTableID = data.readUnsignedShort();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"