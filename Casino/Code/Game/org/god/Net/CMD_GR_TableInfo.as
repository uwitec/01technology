package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//桌子状态数组
	public class CMD_GR_TableInfo
	{
		public static const defaultTableCount:uint = 512;
		public static const sizeof_CMD_GR_TableInfo:int = 2 + 
		defaultTableCount * tagTableStatus.sizeof_tagTableStatus;
		public static const sizeof_TableStatus:int = 
		defaultTableCount * tagTableStatus.sizeof_tagTableStatus;
		
		public var wTableCount:uint;//桌子数目
		public var TableStatus:Array=new Array(defaultTableCount);//状态数组
		public function CMD_GR_TableInfo()
		{
			//TableStatus = tagTableStatus[512];
		}
		public static function readData(data:ByteArray):CMD_GR_TableInfo
		{
			var pos:int=data.position;
			var result:CMD_GR_TableInfo=new CMD_GR_TableInfo;

			result.wTableCount=data.readUnsignedShort();
			for(var i:int = 0; i < result.wTableCount; i ++)
			{
				result.TableStatus[i] = tagTableStatus.readData(data);
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as";
