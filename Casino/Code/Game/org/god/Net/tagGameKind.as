package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//游戏名称结构
	public class tagGameKind
	{
		public static  const sizeof_tagGameKind:int = 2 + 2 + 2 + 4 + 4 + 2 + 2 + 
		4 + 4 + 4 + 8 + KIND_LEN + 2 + 1;
		public var wSortID:uint;//排序号码
		public var wTypeID:uint;//类型号码
		public var wKindID:uint;//名称号码
		public var dwOnLineCount:uint;//在线数目
		public var dwMaxVersion:uint;//最新版本
		public var wProcessType:uint;//进程类型
		public var wTableCount:uint;//桌子数目
		public var dwCellScore:uint;//单位额度
		public var dwHighScore:uint;//最高额度
		public var dwLessScore:uint;//最低额度
		public var fTaxRate:Number;//CT++
		public var szKindName:String = new String;//游戏名字
		//AI属性
		public var wAIUserCount:uint;//机器人数目
		public var cbAILevel:uint;//机器人智能等级

		public function tagGameKind()
		{
			//szKindName.length = KIND_LEN;
		}
		public static function readData(data:ByteArray):tagGameKind
		{
			var pos:int=data.position;
			var result:tagGameKind=new tagGameKind;
			result.wSortID = data.readUnsignedShort();
			result.wTypeID = data.readUnsignedShort();
			result.wKindID = data.readUnsignedShort();
			result.dwOnLineCount = data.readUnsignedInt();
			result.dwMaxVersion  = data.readUnsignedInt();
			result.wProcessType = data.readUnsignedShort();
			result.wTableCount = data.readUnsignedShort();
			result.dwCellScore = data.readUnsignedInt();
			result.dwHighScore = data.readUnsignedInt();
			result.dwLessScore = data.readUnsignedInt();
			result.fTaxRate = data.readDouble();
			result.szKindName = readStringByByteArray(data, KIND_LEN);
			result.wAIUserCount = data.readUnsignedShort();
			result.cbAILevel = data.readUnsignedByte();
			data.position = pos;
			return result;
		}
	}
}
include "Globaldef.as"
include "../Common/Memory.as"