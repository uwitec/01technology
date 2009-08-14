package org.god.Common.BetHistory.GameModule.B_UncoverPig
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.Common.BetHistory.IBetHistoryResource;
	import org.god.Common.BetHistory.GameModule.Common.BaseBattleGameEndView;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.BetHistory.tagBetHistoryData;
	import org.god.GameModule.Common.enXCollocateMode;
	import org.god.GameModule.Common.enYCollocateMode;
	import org.god.GameModule.B_UncoverPig.Common.CMD_S_GameEnd;
	import org.god.GameModule.B_UncoverPig.Common.GameLogic;
	//游戏结束视图
	public class GameEndView extends BaseBattleGameEndView
	{
		protected static const GAME_PLAYER:int = 4;
		protected var m_GameEnd:CMD_S_GameEnd;
		protected var m_imageScoreView:SkinImage;
		protected var m_ImageScore:Array=new Array(16);//分数位图
		//游戏变量
		public var m_bScoreCard:Array=new Array(GAME_PLAYER);//分值扑克
		public var m_bScoreCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var m_bShowCard:Array=new Array(GAME_PLAYER);//玩家亮牌 
		public var m_bShowCardCount:Array=new Array(GAME_PLAYER);//亮牌数目

		public function GameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super(bh,pIBetHistoryResource);
			m_imageScoreView = pIBetHistoryResource.GetResource("IMAGE_CGEV_CELL_UNCOVERPIG");
			m_ImageScore[0]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_2");
			m_ImageScore[1]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_3");                   
			m_ImageScore[2]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_4");                   
			m_ImageScore[3]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_5");                    
			m_ImageScore[4]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_6");                    
			m_ImageScore[5]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_7");                   
			m_ImageScore[6]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_8");                 
			m_ImageScore[7]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_9");                 
			m_ImageScore[8]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_10");                
			m_ImageScore[9]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_J");                  
			m_ImageScore[10]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_Q");                    
			m_ImageScore[11]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_K");                  
			m_ImageScore[12]= pIBetHistoryResource.GetResource("IMAGE_HEARTS_A");                    
			m_ImageScore[13]= pIBetHistoryResource.GetResource("IMAGE_DIAMONDS_J");                  
			m_ImageScore[14]= pIBetHistoryResource.GetResource("IMAGE_SPADE_Q");
			m_ImageScore[15]= pIBetHistoryResource.GetResource("IMAGE_CLUBS_10");  

			m_GameEnd = CMD_S_GameEnd.readData(bh.cbEndData);
			SetWinScore(m_GameEnd.fGameScore,GAME_PLAYER);
			for(var i:uint = 0; i < m_bScoreCard.length; i ++)
			{
				m_bScoreCard[i] = new Array(16);
			}
			for(i = 0; i < m_bShowCard.length; i ++)
			{
				m_bShowCard[i] = new Array(4);
			}
			//游戏变量
			ZeroTwoDimensionArray(m_bScoreCard);
			ZeroArray(m_bScoreCardCount); 
			//亮牌玩家
			ZeroTwoDimensionArray(m_bShowCard) ;
			ZeroArray(m_bShowCardCount) ;

			for(var i:uint=0 ; i<GAME_PLAYER ; ++i)
			{
				var wViewChairID:uint = i;
				if(0!=m_GameEnd.bScoreCardCount[i]) this.AddScoreCard(wViewChairID , m_GameEnd.ScoreCard[i] , m_GameEnd.bScoreCardCount[i]) ;
				if(0!=m_GameEnd.bShowCardCount[i]) this.SetShowCard(wViewChairID , m_GameEnd.ShowCard[i] , m_GameEnd.bShowCardCount[i]) ;
			}

		}
		
		//重画函数
		protected override function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);
			if(m_imageScoreView)
			{
				m_imageScoreView.DrawBitmapData(dc,8,8);
				
				var nYPos:Number=32;
				var nXPos:Number=8;
				
				var ShowChairIDPt:Array = new Array(GAME_PLAYER);
			    var ShowCardPt:Array = new Array(GAME_PLAYER);        //亮分位置
				var ScorePt:Array = new Array(GAME_PLAYER);           //得分位置
				
				for(var i:uint=0 ; i<GAME_PLAYER ; i++)
				{
					ShowChairIDPt[i]= new Point;
					ShowCardPt[i] = new Point;
					ScorePt[i] = new Point;
					
					ShowChairIDPt[i].x  = nXPos + 2 ;
					ShowChairIDPt[i].y  = nYPos+i*20 + 3;
					ShowCardPt[i].x  = nXPos + 118 ;
					ShowCardPt[i].y  = nYPos+i*19;
					ScorePt[i].x     = nXPos + 236 ;
					ScorePt[i].y     = nYPos+i*19	;
				}
				
				var bCardColor:uint ;
				var bCardValue:uint ;
				var bIndex:uint;
				var gameLogic:GameLogic = new GameLogic;
				//绘画亮牌
				for(var i:uint=0 ; i<GAME_PLAYER ; i++)	
				{
					for(var j:uint=0 ; j<m_bShowCardCount[i] ; j++)
					{
						//数据验证
						ASSERT(0!=m_bShowCard[i][j]) ;
						if(0==m_bShowCard[i][j]) break ;
						bCardColor = gameLogic.GetCardColor(m_bShowCard[i][j]) ;
						bCardColor = bCardColor>>4 ;
						bCardValue = gameLogic.GetCardValue(m_bShowCard[i][j]) ;
						
						if(1==bCardValue)      bIndex = 12 ;
						else if(0==bCardColor) bIndex = 13 ;
						else if(1==bCardColor) bIndex = 15 ;
						else if(3==bCardColor) bIndex = 14 ;
						else                   bIndex = bCardValue-2 ;
	
						m_ImageScore[bIndex].DrawBitmapData(dc,ShowCardPt[i].x+j*30,ShowCardPt[i].y);
					}
				}
				//绘画分数 
				for(var i:uint=0 ; i<GAME_PLAYER ; i++)	
				{
					for(var j:uint=0 ; j<m_bScoreCardCount[i] ; j++)
					{
						//数据验证
						ASSERT(0!=m_bScoreCard[i][j]) ;
						if(0==m_bScoreCard[i][j]) break ;
						bCardColor = gameLogic.GetCardColor(m_bScoreCard[i][j]) ;
						bCardColor = bCardColor>>4 ;
						bCardValue = gameLogic.GetCardValue(m_bScoreCard[i][j]) ;
			
						if(1==bCardValue)      bIndex = 12 ;
						else if(0==bCardColor) bIndex = 13 ;
						else if(1==bCardColor) bIndex = 15 ;
						else if(3==bCardColor) bIndex = 14 ;
						else                   bIndex = bCardValue-2 ;
				
						m_ImageScore[bIndex].DrawBitmapData(dc,ScorePt[i].x+j*30,ShowCardPt[i].y);
					}
				}
			
				var m_UserReadCount:uint=0;
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					var strStringBuffer:String = Number(i + 1).toString();
						 DrawUtil.TextOut(dc,
						 ShowChairIDPt[i].x,
						 ShowChairIDPt[i].y,
						 strStringBuffer,
					 	 18,0xCC9900,112,6,"center") ;
				}
			}
			
		}
		
		public override function Destroy():void
		{
			super.Destroy();
			ZeroArray(m_bScoreCard, null);
			m_bScoreCard = null;
			ZeroArray(m_bShowCard, null);
			m_bShowCard = null;

			m_GameEnd = null;
		}
		//设置亮牌
		public function SetShowCard(wChairID:uint, bCardData:Array , bCardCount:uint):void
		{
			//设置变量
			if(INVALID_CHAIR==wChairID)
			{
				ZeroTwoDimensionArray(m_bShowCard) ;
				ZeroArray(m_bShowCardCount) ;
				return ;
			}
			var gameLogic:GameLogic = new GameLogic;
		
			m_bShowCardCount[wChairID] = bCardCount ;
			for(var i:uint=0 ; i<bCardCount ; ++i)	m_bShowCard[wChairID][i] = bCardData[i] ;
		
			ASSERT(m_bShowCardCount[wChairID]>0 && m_bShowCardCount[wChairID] <=4) ;
			if(m_bShowCardCount[wChairID]<0 || m_bShowCardCount[wChairID]>4) return ;
		
			//排列扑克
			gameLogic.SortCardList(m_bShowCard[wChairID] , m_bShowCardCount[wChairID]) ;
		
			
			return ;
		}
		//增加分数
		public function AddScoreCard(wChairID:uint, bCardData:Array , bCardCount:uint) :void
		{
			//设置变量
			if(INVALID_CHAIR==wChairID)
			{
				ZeroTwoDimensionArray(m_bScoreCard);
				ZeroArray(m_bScoreCardCount);
				return ;
			}
			var gameLogic:GameLogic = new GameLogic;
			for(var i:uint=0 ; i<bCardCount ; ++i)  
			{
				m_bScoreCard[wChairID][m_bScoreCardCount[wChairID]+i] = bCardData[i] ;
			}
			m_bScoreCardCount[wChairID] += bCardCount ;
		    
			ASSERT(m_bScoreCardCount[wChairID]>0 && m_bScoreCardCount[wChairID] <=16) ;
			if(m_bScoreCardCount[wChairID]<=0 || m_bScoreCardCount[wChairID]>16) return ;
		
			//排列扑克
			gameLogic.SortCardList(m_bScoreCard[wChairID] , m_bScoreCardCount[wChairID]) ;
		
			
			return ;
		}
	}
}
include "../../StringTableDef.as"
include "../../../Memory.as"
include "../../../../Net/GlobalDef.as"