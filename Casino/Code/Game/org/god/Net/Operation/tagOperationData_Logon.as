package org.god.Net.Operation
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//登陆
	public class tagOperationData_Logon
	{
		public static  const sizeof_tagOperationData_Logon:int=0;


		public function tagOperationData_Logon()
		{
		}
		public static function readData(data:ByteArray):tagOperationData_Logon
		{
			var pos:int=data.position;
			var result:tagOperationData_Logon=new tagOperationData_Logon;

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"