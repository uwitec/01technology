package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//邀请用户 
	public class CMD_GR_UserInvite
	{
		public var wTableID:uint;//桌子号码
		public var dwUserID:uint;//用户 I D
		public function CMD_GR_UserInvite()
		{
		}
	}
}
include "GLOBALDEF.as";
