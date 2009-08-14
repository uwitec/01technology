package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//操作记录数据头
	public class tagOperationHeader
	{
		public static  const sizeof_tagOperationHeader:int=4 + 1 +  NAME_LEN + 4 + 1 + NAME_LEN + 4 + 8 + 4;

		public var dwUserID:uint;//被操作用户ID
		public var cbUserType:uint;//被操作用户类型
		public var szAccount:String="";//被操作帐号
		public var dwOperationUserID:uint;//操作用户ID
		public var cbOperationUserType:uint;//操作用户类型
		public var szOperationAccount:String="";//操作帐号
		public var dwOPType:uint;//操作类型
		public var fTime:Number;//操作时间
		public var dwIPAddress:uint;//操作地址

		public function tagOperationHeader()
		{
		}
		public static function readData(data:ByteArray):tagOperationHeader
		{
			var pos:int=data.position;
			var result:tagOperationHeader=new tagOperationHeader;

			result.dwUserID=data.readUnsignedInt();
			result.cbUserType=data.readUnsignedByte();
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			result.dwOperationUserID=data.readUnsignedInt();
			result.cbOperationUserType=data.readUnsignedByte();
			result.szOperationAccount = readStringByByteArray(data, NAME_LEN);
			result.dwOPType=data.readUnsignedInt();
			result.fTime=data.readDouble();
			result.dwIPAddress=data.readUnsignedInt();
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"