package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//新增子帐号
	public class tagOperationData_NewSubAccount
	{
		public static  const sizeof_tagOperationData_NewSubAccount:int=NAME_LEN + NAME_LEN;

		public var szName:String = "";//名称
		public var szParentAccount:String = "";		//父帐号
		
		public function tagOperationData_NewSubAccount()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_NewSubAccount
		{
			var pos:int=data.position;
			var result:tagOperationData_NewSubAccount=new tagOperationData_NewSubAccount;

			result.szName = readStringByByteArray(data, NAME_LEN);
			result.szParentAccount = readStringByByteArray(data, NAME_LEN);

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"