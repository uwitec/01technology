package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//用户状态
	public class IPC_UserScore
	{
		public static  const sizeof_IPC_UserScore = 4 + tagUserScore.sizeof_tagUserScore;
		public var dwUserID:uint;//用户 I D
		public var UserScore:tagUserScore = new tagUserScore;//用户积分
		public function IPC_UserScore()
		{

		}
		public static function readData(data:ByteArray):IPC_UserScore
		{
			var pos:int=data.position;
			var result:IPC_UserScore=new IPC_UserScore;
			result.dwUserID = data.readUnsignedInt();
			result.UserScore = tagUserScore.readData(data);
			data.position = pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
