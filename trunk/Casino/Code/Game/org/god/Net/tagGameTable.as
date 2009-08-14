package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//游戏桌列表结构
	public class tagGameTable
	{
		public var wTableID:uint;//桌号码
		public var wKindID:uint;//遊戲号码
		public var wTypeID:uint;//類型号码
		public var wTableNumber:uint;//桌索引
		public var nbTalbeParam:ByteArray = newLitteEndianByteArray();//桌参数
		public function tagGameTable()
		{
			//nbTalbeParam.length = KIND_LEN;
		}
	}
}
include "Globaldef.as"
include "../Common/Memory.as"
