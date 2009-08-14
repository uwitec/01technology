package org.god.Net.BetHistory
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//获取报表记录
	public class tagBetHistoryData
	{
		public static  const sizeof_tagBetHistoryData:int=8 + 8 + 8 + 2 +
		TYPE_LEN + KIND_LEN + SERVER_LEN + 2 +
		8 + 8 + 8 + 8  + 1 + 2;

		public var fUCID:Number;//报表记录ID
		
		public var fGameRoundID:Number;//游戏局记录ID
		public var fStartTime:Number;//游戏开始时间
		public var wProcessType:uint;//游戏类型

		public var szTypeName:String;//种类名字
		public var szKindName:String;//游戏名字
		public var szServerName:String;//房间名称
		public var wTableID:uint;//桌ID

		public var fTotalBetScore:Number;//投注总额
		public var fTotalWinScore:Number;//总派彩
		public var fTotalTaxScore:Number;//总抽水
		public var fValidScore_Less_Rollback:Number;//洗码-下线洗码

		public var cbEndReason:uint;//游戏结束原因
		public var wEndDataSize:uint;//游戏结束信息大小
		
		public var cbEndData:ByteArray = newLitteEndianByteArray();

		public function tagBetHistoryData()
		{
		}
		public function getBetHistoryDataSize():uint
		{
			return sizeof_tagBetHistoryData + wEndDataSize;
		}
		public static  function readData(data:ByteArray):tagBetHistoryData
		{
			var pos:int=data.position;
			var result:tagBetHistoryData=new tagBetHistoryData  ;
			
			result.fUCID=data.readDouble();
			
			result.fGameRoundID=data.readDouble();
			result.fStartTime=data.readDouble();
			result.wProcessType=data.readUnsignedShort();

			result.szTypeName=readStringByByteArray(data,TYPE_LEN);
			result.szKindName=readStringByByteArray(data,KIND_LEN);
			result.szServerName=readStringByByteArray(data,SERVER_LEN);
			result.wTableID=data.readUnsignedShort();

			result.fTotalBetScore=data.readDouble();
			result.fTotalWinScore=data.readDouble();
			result.fTotalTaxScore=data.readDouble();
			result.fValidScore_Less_Rollback=data.readDouble();
			
			result.cbEndReason=data.readUnsignedByte();
			result.wEndDataSize=data.readUnsignedShort();
			
			result.cbEndData.length = result.wEndDataSize;
			if(result.cbEndData.length > 0)
				CopyMemory(result.cbEndData, 
					   	data, 
					  	 result.wEndDataSize,
					  	 0, data.position);
			
			data.position=pos;
			return result;
		}
	}
}
include "../GLOBALDEF.as"
include "../../Common/Memory.as"