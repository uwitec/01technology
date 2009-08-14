package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//游戏结束
	public class CMD_S_GameEnd
	{
		public static const sizeof_CMD_S_GameEnd:uint = 1 + 2 + 4 * 8 + 2 * 4 + 
		2 + 2 + 4 + 4*14 + 4 + 4 * 4 * CMD_WeaveItem.sizeof_CMD_WeaveItem;
		
		public var cbChiHuSparrow:uint;//吃胡扑克
		public var wProvideUser:uint;//点炮用户
		public var fGameScore:Array = new Array(4);//游戏积分
		public var wChiHuKind:Array = new Array(4);//胡牌类型
		//游戏变量
		public var wSiceCount:uint;//骰子点数
		public var wBankerUser:uint;//庄家用户
		
		public var cbSparrowCount:Array = new Array(4);//扑克数目
		public var cbSparrowData:Array = new Array(4);//扑克数据
		
		//组合扑克
		public var cbWeaveCount:Array = new Array(4);//组合数目
		public var WeaveItemArray:Array = new Array(4);//组合扑克

		public function CMD_S_GameEnd()
		{
			for(var i:uint = 0; i < cbSparrowData.length; i ++)
			{
				cbSparrowData[i] = new Array(14);
			}
			for (i = 0; i < WeaveItemArray.length; i ++)
			{
				WeaveItemArray[i]= new Array(4);
				for(var k:uint = 0; k < WeaveItemArray[i].length; k ++)
					WeaveItemArray[i][k] = new CMD_WeaveItem;
			}
		}
		public static function readData(data:ByteArray):CMD_S_GameEnd
		{
			var pos:int=data.position;
			var result:CMD_S_GameEnd=new CMD_S_GameEnd;
			result.cbChiHuSparrow = data.readUnsignedByte();
			result.wProvideUser = data.readUnsignedShort();
			
			for(var i:uint = 0; i < result.fGameScore.length; i ++)
			{
				result.fGameScore[i] = data.readDouble();
			}
			for(i = 0; i < result.wChiHuKind.length; i ++)
			{
				result.wChiHuKind[i] = data.readUnsignedShort();
			}
			
			//游戏变量
			result.wSiceCount = data.readUnsignedShort();//骰子点数
			result.wBankerUser = data.readUnsignedShort();//庄家用户

			for(i = 0; i < result.cbSparrowCount.length; i ++)
			{
				result.cbSparrowCount[i] = data.readUnsignedByte();
			}
			for(i = 0; i < result.cbSparrowData.length; i ++)
			{
				for(var k:uint = 0; k < result.cbSparrowData[i].length; k ++)
				{
					result.cbSparrowData[i][k] = data.readUnsignedByte();
				}
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