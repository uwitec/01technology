package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录
	public class CMD_GP_ChartViewThreeSucess
	{
		public static  const sizeof_CMD_GP_ChartViewThreeSucess:int=4 + 8 + 8 +
		8 ;

		//投注
		public var dwGameRoundCount:uint;//局数
		public var fTotalBetScore:Number;//投注总额
		public var fTotalWinScore:Number;//总派彩

		//洗码
		public var fValidScore_Less_Rollback:Number;//洗码-下线洗码

		
		public function CMD_GP_ChartViewThreeSucess()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_ChartViewThreeSucess
		{
			var pos:int=data.position;
			var result:CMD_GP_ChartViewThreeSucess=new CMD_GP_ChartViewThreeSucess;

			result.dwGameRoundCount = data.readUnsignedInt();
			result.fTotalBetScore = data.readDouble();
			result.fTotalWinScore = data.readDouble();

			result.fValidScore_Less_Rollback = data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"