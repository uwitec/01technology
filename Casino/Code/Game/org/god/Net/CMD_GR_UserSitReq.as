package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//请求坐下
	public class CMD_GR_UserSitReq
	{
		public static const sizeof_CMD_GR_UserSitReq = 2 + 2 + 2 + 1 + PASS_LEN;
		
		public var wTableID;//桌子位置
		public var wChairID;//椅子位置
		public var wNetTimelag;//网络延时
		public var cbPassLen;//密码长度
		public var szTablePass:String=new String  ;//桌子密码
		public function CMD_GR_UserSitReq()
		{
			//szTablePass.length=PASS_LEN;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeShort(wTableID);
			result.writeShort(wChairID);
			result.writeShort(wNetTimelag);
			result.writeByte(cbPassLen);
			writeStringToByteArray(result, szTablePass, PASS_LEN);
			
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"