package org.god.GameModule.B_Hoe.Common
{
	public class tagAnalyseResult
	{
		public var bFourCount:uint;//四张数目
		public var bThreeCount:uint;//三张数目
		public var bDoubleCount:uint;//两张数目
		public var bSignedCount:uint;//单张数目
		public var bFourLogicVolue:Array=new Array(3);//四张列表
		public var bThreeLogicVolue:Array=new Array(4);//三张列表
		public var bDoubleLogicVolue:Array=new Array(6);//两张列表
		public var bSCardData:Array=new Array(13);//手上扑克
		public var bDCardData:Array=new Array(13);//手上扑克
		public var bTCardData:Array=new Array(13);//手上扑克
		public var bFCardData:Array=new Array(13);//手上扑克
		public function tagAnalyseResult():void
		{
		}
		public function init():void
		{
			bFourCount=0;//四张数目
			bThreeCount=0;//三张数目
			bDoubleCount=0;//两张数目
			bSignedCount=0;//单张数目
			for(var i:uint = 0; i < bFourLogicVolue.length; i ++)
				bFourLogicVolue[i] = 0;//四张列表
			for(i= 0; i < bThreeLogicVolue.length; i ++)
				bThreeLogicVolue[i] = 0;//三张列表
			for(i= 0; i < bDoubleLogicVolue.length; i ++)
				bDoubleLogicVolue[i] = 0;//两张列表
			for(i= 0; i < bSCardData.lenght; i ++)
			{
					bSCardData[i] = 0;//手上扑克
					bDCardData[i] = 0;//手上扑克
					bTCardData[i] = 0;//手上扑克
					bFCardData[i] = 0;//手上扑克
			}
		}
	}
}