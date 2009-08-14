package org.god.GameModule.B_SiChuan_Land.Common
{
	public class tagAnalyseResult
	{
		public var bFourCount:uint;//四张数目
		public var bThreeCount:uint;//三张数目
		public var bDoubleCount:uint;//两张数目
		public var bSignedCount:uint;//单张数目
		public var bFourLogicVolue:Array=new Array(5);//四张列表
		public var bThreeLogicVolue:Array=new Array(6);//三张列表
		public var bDoubleLogicVolue:Array=new Array(10);//两张列表
		public var m_bSCardData:Array=new Array(20);//手上扑克
		public var m_bDCardData:Array=new Array(20);//手上扑克
		public var m_bTCardData:Array=new Array(20);//手上扑克
		public var m_bFCardData:Array=new Array(20);//手上扑克
		public function tagAnalyseResult():void
		{
		}
		public function init():void
		{
			bFourCount=0;//四张数目
			bThreeCount=0;//三张数目
			bDoubleCount=0;//两张数目
			bSignedCount=0;//单张数目
			for(var i:uint = 0; i < 5; i ++)
				bFourLogicVolue[i] = 0;//四张列表
			for(i= 0; i < 6; i ++)
				bThreeLogicVolue[i] = 0;//三张列表
			for(i= 0; i < 10; i ++)
				bDoubleLogicVolue[i] = 0;//两张列表
			for(i= 0; i < 20; i ++)
			{
					m_bSCardData[i] = 0;//手上扑克
					m_bDCardData[i] = 0;//手上扑克
					m_bTCardData[i] = 0;//手上扑克
					m_bFCardData[i] = 0;//手上扑克
			}
		}
	}
}