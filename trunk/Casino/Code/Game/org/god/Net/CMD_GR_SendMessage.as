package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//系统消息
	public class CMD_GR_SendMessage
	{
		public var cbGame:uint;//游戏消息
		public var cbRoom:uint;//游戏消息
		public var wChatLength:uint;//信息长度
		public var szSystemMessage:String=new String  ;//系统消息
		public function CMD_GR_SendMessage()
		{
			//szSystemMessage.length = MAX_CHAT_LEN;
		}
	}
}
include "GLOBALDEF.as";
