package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//用户分数
	public class CMD_GR_UserScore
	{
		public static const sizeof_CMD_GR_UserScore:int = 4 + tagUserScore.sizeof_tagUserScore;
		public var dwUserID:uint;//用户 I D
		public var UserScore:tagUserScore=new tagUserScore  ;//积分信息
		public function CMD_GR_UserScore()
		{
			
		}
		public static function readData(data:ByteArray):CMD_GR_UserScore
		{
			var pos:int=data.position;
			var result:CMD_GR_UserScore=new CMD_GR_UserScore;

			result.dwUserID=data.readUnsignedInt();
			result.UserScore=tagUserScore.readData(data);
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as";
