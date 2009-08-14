package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录
	public class tagChartViewTwoData
	{
		public static  const sizeof_tagChartViewTwoData:int=NAME_LEN + 4 + 8 + 8 + 8+
		8 + 8 ;

		public var szAccount:String;//帐号
		//投注
		public var dwGameRoundCount:uint;//局数
		public var fTotalBetScore:Number;//投注总额
		public var fTotalWinScore:Number;//总派彩
		public var fTotalTaxScore:Number;//总抽水

		//洗码
		public var fValidScore_Total:Number;//洗码-货量总额
		public var fValidScore_Less_Rollback:Number;//洗码-下线洗码

		public function tagChartViewTwoData()
		{
		}
		public static function readData(data:ByteArray):tagChartViewTwoData
		{
			var pos:int=data.position;
			var result:tagChartViewTwoData=new tagChartViewTwoData;
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			
			result.dwGameRoundCount = data.readUnsignedInt();
			result.fTotalBetScore = data.readDouble();
			result.fTotalWinScore = data.readDouble();
			result.fTotalTaxScore = data.readDouble();

			result.fValidScore_Total = data.readDouble();
			result.fValidScore_Less_Rollback = data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"