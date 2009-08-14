package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//游戏状态
	public class CMD_S_StatusPlay
	{
		public static  const sizeof_CMD_S_StatusPlay:uint = 8 + 2 + 2 + 2 +
		1 + 1 + 1 +  2 + 1 + 4 + 4*55 +  1 + 14 +  4 + 4 * 4 * CMD_WeaveItem.sizeof_CMD_WeaveItem;

		//游戏变量
		public var fCellScore:Number= 0;//单元积分
		public var wSiceCount:uint;//骰子点数
		public var wBankerUser:uint;//庄家用户
		public var wCurrentUser:uint;//当前用户

		//状态变量
		public var cbActionSparrow:uint;//动作扑克
		public var cbActionMask:uint;//动作掩码
		public var cbLeftSparrowCount:uint;//剩余数目

		//出牌信息
		public var wOutSparrowUser:uint;//出牌用户
		public var cbOutSparrowData:uint;//出牌扑克
		public var cbDiscardCount:Array = new Array(4);//丢弃数目
		public var cbDiscardSparrow:Array = new Array(4);//丢弃记录

		//扑克数据
		public var cbSparrowCount:uint;//扑克数目
		public var cbSparrowData:Array = new Array(14);//扑克列表

		//组合扑克
		public var cbWeaveCount:Array = new Array(4);//组合数目
		public var WeaveItemArray:Array = new Array(4);//组合扑克

		public function CMD_S_StatusPlay()
		{
			for (var i:uint = 0; i < cbDiscardSparrow.length; i ++)
			{
				cbDiscardSparrow[i] = new Array(55);
			}
			for (i = 0; i < WeaveItemArray.length; i ++)
			{
				WeaveItemArray[i]= new Array(4);
				for(var k:uint = 0; k < WeaveItemArray[i].length; k ++)
					WeaveItemArray[i][k] = new CMD_WeaveItem;
			}
		}
		public static function readData(data:ByteArray):CMD_S_StatusPlay
		{
			var pos:int=data.position;
			var result:CMD_S_StatusPlay=new CMD_S_StatusPlay;

			//游戏变量
			result.fCellScore= data.readDouble();//单元积分
			result.wSiceCount = data.readUnsignedShort();//骰子点数
			result.wBankerUser = data.readUnsignedShort();//庄家用户
			result.wCurrentUser = data.readUnsignedShort();//当前用户

			//状态变量
			result.cbActionSparrow = data.readUnsignedByte();//动作扑克
			result.cbActionMask= data.readUnsignedByte();//动作掩码
			result.cbLeftSparrowCount= data.readUnsignedByte();//剩余数目

			//出牌信息
			result.wOutSparrowUser= data.readUnsignedShort();//出牌用户
			result.cbOutSparrowData= data.readUnsignedByte();//出牌扑克
			for (var i:uint = 0; i < result.cbDiscardCount.length; i++)
			{
				result.cbDiscardCount[i] = data.readUnsignedByte() ;//丢弃数目
			}
			for (i =0; i < result.cbDiscardSparrow.length; i++)
			{
				for (var k:uint = 0; k < result.cbDiscardSparrow[i].length; k++)
				{
					result.cbDiscardSparrow[i][k] = data.readUnsignedByte() ;
				}//丢弃记录
			}


			//扑克数据
			result.cbSparrowCount = data.readUnsignedByte();//扑克数目
			for (i =0; i < result.cbSparrowData.length; i++)
			{
				result.cbSparrowData[i] = data.readUnsignedByte() ;//扑克列表
			}

			//组合扑克
			for (i =0; i < result.cbWeaveCount.length; i++)
			{
				result.cbWeaveCount[i] = data.readUnsignedByte() ;//组合数目
			}
			for (i =0; i < result.WeaveItemArray.length; i++)
			{
				for (var k:uint = 0; k < result.WeaveItemArray[i].length; k++)
				{
					result.WeaveItemArray[i][k] =CMD_WeaveItem.readData(data);//组合扑克
				}
			}

			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"