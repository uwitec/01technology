package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//报表记录
	public class CMD_GP_ChartViewTwoList
	{
		public static  const sizeof_CMD_GP_ChartViewTwoList:int=0;

		public var ChartViewTwoDatas:Array = new Array();
		
		public function CMD_GP_ChartViewTwoList()
		{
		}
		public static function readData(data:ByteArray,wDataSize:int):CMD_GP_ChartViewTwoList
		{
			var pos:int=data.position;
			var result:CMD_GP_ChartViewTwoList=new CMD_GP_ChartViewTwoList;
			
			var count:int = wDataSize / tagChartViewTwoData.sizeof_tagChartViewTwoData;
			for(var i:int = 0; i < count; i ++)
			{
				var cvdata:tagChartViewTwoData = tagChartViewTwoData.readData(data);
				result.ChartViewTwoDatas.push(cvdata);
				data.position += tagChartViewTwoData.sizeof_tagChartViewTwoData;
			}
			
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"