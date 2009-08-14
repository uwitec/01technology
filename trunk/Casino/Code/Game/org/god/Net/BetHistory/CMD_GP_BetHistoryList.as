package org.god.Net.BetHistory
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//游戏局记录列表
	public class CMD_GP_BetHistoryList
	{
		public static  const sizeof_CMD_GP_BetHistoryList:int=0;

		public var BetHistoryDatas:Array = new Array();
		
		public function CMD_GP_BetHistoryList()
		{
		}
		public static function readData(data:ByteArray,wDataSize:int):CMD_GP_BetHistoryList
		{
			var pos:int=data.position;
			var result:CMD_GP_BetHistoryList=new CMD_GP_BetHistoryList;
			while(wDataSize > 0)
			{
				if(wDataSize < tagBetHistoryData.sizeof_tagBetHistoryData)
					break;
				var bhdata:tagBetHistoryData = tagBetHistoryData.readData(data);
				result.BetHistoryDatas.push(bhdata);
				data.position += bhdata.getBetHistoryDataSize();
				wDataSize -= bhdata.getBetHistoryDataSize();
			}
	
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"