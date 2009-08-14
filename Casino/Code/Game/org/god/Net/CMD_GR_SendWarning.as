package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//发送警告
	public class CMD_GR_SendWarning
	{
		public var wChatLength:uint;//信息长度
		public var dwTargetUserID:uint;//目标用户
		public var szWarningMessage:String=new String  ;//警告消息
		public function CMD_GR_SendWarning()
		{
			//szWarningMessage.length = MAX_CHAT_LEN
		}
	}
}
include "GLOBALDEF.as";
