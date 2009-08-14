package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//游戏信息
	public class IPC_GF_ServerInfo
	{
		public static  const sizeof_IPC_GF_ServerInfo = 4 + 2 + 2 + 2 
		+ 2 + 2 + 2 + KIND_LEN + SERVER_LEN + 8 + 8 + 8;
		public var dwUserID:uint;//用户 I D
		public var wTableID:uint;//桌子号码
		public var wChairID:uint;//椅子号码
		public var wKindID:uint;//类型标识
		public var wServerID:uint;//房间标识
		public var wGameGenre:uint;//游戏类型
		public var wChairCount:uint;//椅子数目
		public var szKindName:String = new String;//类型名字
		public var szServerName:String = new String;//房间名称
		public var fCellScore:Number;//单位额度
		public var fHighScore:Number;//最高额度
		public var fLessScore:Number;//最低额度

		public function IPC_GF_ServerInfo()
		{
			//szKindName.length = KIND_LEN;
			//szServerName.length = SERVER_LEN;
		}
		
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
