package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//报表记录
	public class CMD_GP_ChartViewOneList
	{
		public static  const sizeof_CMD_GP_ChartViewOneList:int=0;

		public var ChartViewOneDatas:Array = new Array();
		
		public function CMD_GP_ChartViewOneList()
		{
		}
		public static function readData(data:ByteArray,wDataSize:int):CMD_GP_ChartViewOneList
		{
			var pos:int=data.position;
			var result:CMD_GP_ChartViewOneList=new CMD_GP_ChartViewOneList;
			
			var count:int = wDataSize / tagChartViewOneData.sizeof_tagChartViewOneData;
			for(var i:int = 0; i < count; i ++)
			{
				var cvdata:tagChartViewOneData = tagChartViewOneData.readData(data);
				result.ChartViewOneDatas.push(cvdata);
				data.position += tagChartViewOneData.sizeof_tagChartViewOneData;
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"