package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//游戏进程结构
	public class tagGameProcess
	{
		static const sizeof_tagGameProcess:int = 2 + 2 + 2 + 1 + 2 + 4 + 4 + 4 + KIND_LEN;
		public var wSortID:uint;//排序号码
		public var wTypeID:uint;//类型号码
		public var wKindID:uint;//名称号码
		public var wServerPort:uint;//房间端口
		public var dwServerAddr:uint;//房间地址
		public var dwMaxVersion:uint;//最新版本
		public var dwOnLineCount:uint;//在线数目
		public var szKindName:String = new String;//游戏名字
		public function tagGameProcess()
		{
			//szKindName.length = KIND_LEN;
		}
		public static function readData(data:ByteArray):tagGameProcess
		{
			var pos:int=data.position;
			var result:tagGameProcess=new tagGameProcess;
			result.wSortID = data.readUnsignedShort();
			result.wTypeID = data.readUnsignedShort();
			result.wKindID = data.readUnsignedShort();
			result.wServerPort = data.readUnsignedShort();
			result.dwServerAddr = data.readUnsignedInt();
			result.dwMaxVersion = data.readUnsignedInt();
			result.dwOnLineCount = data.readUnsignedInt();
			result.szKindName = readStringByByteArray(data, KIND_LEN);
			data.position = pos;
			return result;
		}
	}
}
include "Globaldef.as"
include "../Common/Memory.as"
