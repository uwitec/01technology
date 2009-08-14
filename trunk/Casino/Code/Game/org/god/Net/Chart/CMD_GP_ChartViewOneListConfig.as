package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录配置
	public class CMD_GP_ChartViewOneListConfig
	{
		public static  const sizeof_CMD_GP_ChartViewOneListConfig:int=NAME_LEN + 1 + 4;

		public var szAccount:String;//帐号
		public var cbLevel:uint;//用户级别
		public var dwTotalResultCount:uint;//记录总数

		public function CMD_GP_ChartViewOneListConfig()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_ChartViewOneListConfig
		{
			var pos:int=data.position;
			var result:CMD_GP_ChartViewOneListConfig=new CMD_GP_ChartViewOneListConfig;
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			result.cbLevel = data.readUnsignedByte();
			result.dwTotalResultCount = data.readUnsignedInt();
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"