﻿package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//游戏配置
	//聊天结构
	public class CMD_GF_UserChat
	{
		public var wChatLength:uint;//信息长度
		public var dwSendUserID:uint;//发送用户
		public var dwTargetUserID:uint;//目标用户
		public var szChatMessage:String = new String;//聊天信息
		public function CMD_GF_UserChat()
		{
			//szChatMessage.length = MAX_CHAT_LEN;
		}
	}
}
include "../Common/Memory.as"