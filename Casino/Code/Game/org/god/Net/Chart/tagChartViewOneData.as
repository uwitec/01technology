package org.god.Net.Chart
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录
	public class tagChartViewOneData
	{
		public static  const sizeof_tagChartViewOneData:int=NAME_LEN + 4 + 8 + 8 + 8 +
		8 + 8 + 8  +8 + 8 + 8  + 8 + 8 + 8 + 8 + 8   + 8 + 8;

		public var szAccount:String;//帐号
		//投注
		public var dwGameRoundCount:uint;//局数
		public var fTotalBetScore:Number;//投注总额
		public var fTotalWinScore:Number;//总派彩
		public var fTotalTaxScore:Number;//总抽水

		//派彩占成
		public var fWinScore_High:Number;//派彩占成-上线
		public var fWinScore_Self:Number;//派彩占成-自身
		public var fWinScore_Less:Number;//派彩占成-下线
		
		//抽水占成
		public var fTaxScore_High:Number;//抽水占成-上线
		public var fTaxScore_Self:Number;//抽水占成-自身
		public var fTaxScore_Less:Number;//抽水占成-下线

		//洗码
		public var fValidScore_Total:Number;//洗码-货量总额
		public var fValidScore_High:Number;//洗码-上线货量
		public var fValidScore_High_Rollback:Number;//洗码-上线洗码
		public var fValidScore_Less:Number;//洗码-下线货量
		public var fValidScore_Less_Rollback:Number;//洗码-下线洗码

		//交收额度
		public var fPaidScore_High:Number;//交收额度-上线交收
		public var fPaidScore_Less:Number;//交收额度-下线交收

		public function tagChartViewOneData()
		{
		}
		public function ZeroData():void
		{
			//投注
			dwGameRoundCount = 0;//局数
			fTotalBetScore = 0;//投注总额
			fTotalWinScore = 0;//总派彩
			fTotalTaxScore = 0;//总抽水

			//派彩占成
			fWinScore_High = 0//派彩占成-上线
			fWinScore_Self = 0;//派彩占成-自身
			fWinScore_Less = 0;//派彩占成-下线
			
			//抽水占成
			fTaxScore_High = 0;//抽水占成-上线
			fTaxScore_Self = 0;//抽水占成-自身
			fTaxScore_Less = 0;//抽水占成-下线

			//洗码
			fValidScore_Total = 0;//洗码-货量总额
			fValidScore_High = 0;//洗码-上线货量
			fValidScore_High_Rollback = 0;//洗码-上线洗码
			fValidScore_Less = 0;//洗码-下线货量
			fValidScore_Less_Rollback = 0;//洗码-下线洗码

			//交收额度
			fPaidScore_High = 0;//交收额度-上线交收
			fPaidScore_Less = 0;//交收额度-下线交收
		}
		public function SumData(cd:tagChartViewOneData):void
		{
			//投注
			dwGameRoundCount += cd.dwGameRoundCount;//局数
			fTotalBetScore += cd.fTotalBetScore;//投注总额
			fTotalWinScore += cd.fTotalWinScore;//总派彩
			fTotalTaxScore += cd.fTotalTaxScore;//总抽水

			//派彩占成
			fWinScore_High += cd.fWinScore_High//派彩占成-上线
			fWinScore_Self += cd.fWinScore_Self;//派彩占成-自身
			fWinScore_Less += cd.fWinScore_Less;//派彩占成-下线
			
			//派彩占成
			fTaxScore_High += cd.fTaxScore_High//派彩占成-上线
			fTaxScore_Self += cd.fTaxScore_Self;//派彩占成-自身
			fTaxScore_Less += cd.fTaxScore_Less;//派彩占成-下线

			//洗码
			fValidScore_Total += cd.fValidScore_Total;//洗码-货量总额
			fValidScore_High += cd.fValidScore_High;//洗码-上线货量
			fValidScore_High_Rollback += cd.fValidScore_High_Rollback;//洗码-上线洗码
			fValidScore_Less += cd.fValidScore_Less;//洗码-下线货量
			fValidScore_Less_Rollback += cd.fValidScore_Less_Rollback;//洗码-下线洗码

			//交收额度
			fPaidScore_High += cd.fPaidScore_High;//交收额度-上线交收
			fPaidScore_Less += cd.fPaidScore_Less;//交收额度-下线交收
		}
		public static function readData(data:ByteArray):tagChartViewOneData
		{
			var pos:int=data.position;
			var result:tagChartViewOneData=new tagChartViewOneData;
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			
			result.dwGameRoundCount = data.readUnsignedInt();
			result.fTotalBetScore = data.readDouble();
			result.fTotalWinScore = data.readDouble();
			result.fTotalTaxScore = data.readDouble();
		
			result.fWinScore_High = data.readDouble();
			result.fWinScore_Self = data.readDouble();
			result.fWinScore_Less = data.readDouble();
			
			result.fTaxScore_High = data.readDouble();
			result.fTaxScore_Self = data.readDouble();
			result.fTaxScore_Less = data.readDouble();
			
			result.fValidScore_Total = data.readDouble();
			result.fValidScore_High = data.readDouble();
			result.fValidScore_High_Rollback = data.readDouble();
			result.fValidScore_Less = data.readDouble();
			result.fValidScore_Less_Rollback = data.readDouble();
			
			result.fPaidScore_High = data.readDouble();
			result.fPaidScore_Less = data.readDouble();

			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"