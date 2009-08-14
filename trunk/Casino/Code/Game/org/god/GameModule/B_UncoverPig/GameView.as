package 
{
	import flash.display.MovieClip;
	import flash.display.Graphics;
	import flash.display.BitmapData;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	import org.god.Common.*;
	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.*;
	import org.god.GameModule.B_UncoverPig.Common.*;
	import org.god.SkinControl.ImageButton;

	public class GameView extends GameBaseView
	{
		//游戏变量
		public var m_bScoreCard:Array=new Array(GAME_PLAYER);//分值扑克
		public var m_bScoreCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var m_bShowCard:Array=new Array(GAME_PLAYER);//玩家亮牌 
		public var m_bShowCardCount:Array=new Array(GAME_PLAYER);//亮牌数目

		//界面变量
		//配置变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序
		//状态变量
		public var m_bSendWaiMsg:Boolean;//举手计时
		public var m_bCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var m_bShowCardHint:Boolean;//亮牌提示

		//控件变量
		//按钮控件
		public var m_btStart:ImageButton;//开始按钮
		public var m_btExit:ImageButton;//离开按钮
		public var m_btOutCard:ImageButton;//出牌按钮
		public var m_btShowCard:ImageButton;//亮牌按钮
		public var m_btNotShowCard:ImageButton;//不亮按钮
		public var m_btAutoOutCard:ImageButton;//自动代打
		public var m_btCancelAutoOutCard:ImageButton;//取消代打

		//扑克控件
		public var m_HandCardControl:CardControl;//手上扑克
		public var m_UserCardControl:Array=new Array(GAME_PLAYER);//扑克视图
		public var m_LeaveCardControl:Array=new Array(3);//结束扑克

		protected var m_ImageScore:Array=new Array(16);//分数位图
		protected var m_PointBanker:Array= new Array(GAME_PLAYER);
		protected var m_ImageTitle:SkinImage;//标题位图
		
		//控件组件
		public var m_ScoreView:ScoreView;//积分视图

		public function GameView(prarentClass:GameClass)
		{
			m_nYBorder=3;
			m_nXBorder=3;
			m_nXFace = 54;
			m_nYFace = 54;
			m_nXTimer= 54;
			m_nYTimer= 54;

			super(prarentClass);
			m_bWindowDrawMode=true;
		}
		///////////////////////////////////////////////////
		override public  function InitGameView():Boolean
		{
			if (super.InitGameView() == false)
			{
				return false;
			}
			
			for(var i:uint = 0; i < m_bScoreCard.length; i ++)
			{
				m_bScoreCard[i] = new Array(16);
			}
			for(i = 0; i < m_bShowCard.length; i ++)
			{
				m_bShowCard[i] = new Array(4);
			}
			for(i = 0; i < m_PointBanker.length; i ++)
			{
				m_PointBanker[i] = new Point;
			}
			m_ImageScore[0]= new SkinImage(IMAGE_HEARTS_2,28,12); 
			m_ImageScore[1]= new SkinImage(IMAGE_HEARTS_3,28,12);                     
			m_ImageScore[2]= new SkinImage(IMAGE_HEARTS_4,28,12);                     
			m_ImageScore[3]= new SkinImage(IMAGE_HEARTS_5,28,12);                     
			m_ImageScore[4]= new SkinImage(IMAGE_HEARTS_6,28,12);                     
			m_ImageScore[5]= new SkinImage(IMAGE_HEARTS_7,28,12);                     
			m_ImageScore[6]= new SkinImage(IMAGE_HEARTS_8,28,12);                     
			m_ImageScore[7]= new SkinImage(IMAGE_HEARTS_9,28,12);                     
			m_ImageScore[8]= new SkinImage(IMAGE_HEARTS_10,28,12);                    
			m_ImageScore[9]= new SkinImage(IMAGE_HEARTS_J,28,12);                     
			m_ImageScore[10]= new SkinImage(IMAGE_HEARTS_Q,28,12);                     
			m_ImageScore[11]= new SkinImage(IMAGE_HEARTS_K,28,12);                     
			m_ImageScore[12]= new SkinImage(IMAGE_HEARTS_A,28,12);                     
			m_ImageScore[13]= new SkinImage(IMAGE_DIAMONDS_J,28,12);                   
			m_ImageScore[14]= new SkinImage(IMAGE_SPADE_Q,28,12);  
			m_ImageScore[15]= new SkinImage(IMAGE_CLUBS_10,28,12);  
			m_ImageTitle = new SkinImage(IMAGE_TITLE);
			
			//创建扑克
			for (i=0;i<GAME_PLAYER;i++)
			{
				m_UserCardControl[i] = new CardControl;
				addChild(m_UserCardControl[i]);
				m_UserCardControl[i].SetDirection(true);
				m_UserCardControl[i].SetDisplayFlag(true);
			}
			m_HandCardControl = new CardControl;
			addChild(m_HandCardControl);
			m_HandCardControl.addEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
			for (i=0;i<3;i++)
			{
				m_LeaveCardControl[i] = new CardControl;
				addChild(m_LeaveCardControl[i]);
				m_LeaveCardControl[i].SetDisplayFlag(false);
				m_LeaveCardControl[i].SetCardSpace(20,18,0);
				m_LeaveCardControl[i].SetDirection((i==0)?true:false);
			}
			m_ScoreView = new ScoreView;
			addChild(m_ScoreView);

			m_btStart= new ImageButton(IMAGE_BT_START,430,40);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btExit= new ImageButton(IMAGE_BT_EXIT,430,40);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btOutCard= new ImageButton(IMAGE_BT_OUT_CARD,325,33);
			addChild(m_btOutCard);
			m_btOutCard.addEventListener(MouseEvent.CLICK, OnEventOutCard);
			m_btShowCard= new ImageButton(IMAGE_BT_SHOW_CARD,325,33);
			addChild(m_btShowCard);
			m_btShowCard.addEventListener(MouseEvent.CLICK, OnEventShowCard);
			m_btNotShowCard= new ImageButton(IMAGE_BT_NOT_SHOW_CARD,325,33);
			addChild(m_btNotShowCard);
			m_btNotShowCard.addEventListener(MouseEvent.CLICK, OnEventNotShowCard);
    		m_btAutoOutCard= new ImageButton(IMAGE_BT_AUTOMATISM_OUTCARD,345,23);
			addChild(m_btAutoOutCard);
			m_btAutoOutCard.addEventListener(MouseEvent.CLICK, OnEventAutoOutCard);
			m_btCancelAutoOutCard= new ImageButton(IMAGE_BT_CANCEL_AUTOMATISM_,345,23);
			addChild(m_btCancelAutoOutCard);
			m_btCancelAutoOutCard.addEventListener(MouseEvent.CLICK, OnEventCancelAutoOutCard);

			m_btOutCard.ShowWindow(false) ;
			m_btShowCard.ShowWindow(false) ;
			m_btNotShowCard.ShowWindow(false) ;
			m_btAutoOutCard.ShowWindow(false) ;
			m_btCancelAutoOutCard.ShowWindow(false) ;
			
			//游戏变量
			ZeroTwoDimensionArray(m_bScoreCard);
			ZeroArray(m_bScoreCardCount); 
			//亮牌玩家
			ZeroTwoDimensionArray(m_bShowCard) ;
			ZeroArray(m_bShowCardCount) ;

			//配置变量
			m_bDeasilOrder        = false ;

			//状态变量
			m_bSendWaiMsg         = false ;
			ZeroArray(m_bCardCount) ;
			m_bShowCardHint=false ;

			//隐藏控件
			m_btStart.ShowWindow(false) ;
			m_btOutCard.ShowWindow(false) ;

			//隐藏控件
			m_ScoreView.ShowWindow(false);

			//设置扑克
			for(i=0 ; i<GAME_PLAYER ; ++i) 
			{
				m_UserCardControl[i].SetCardData(null , 0);
			}
			m_HandCardControl.SetCardData(null,0);
			m_HandCardControl.SetPositively(false);
			m_HandCardControl.SetDisplayFlag(false);
			m_HandCardControl.addEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
			m_LeaveCardControl[0].SetCardData(null,0);
			m_LeaveCardControl[1].SetCardData(null,0);
			m_LeaveCardControl[2].SetCardData(null,0);
			
			
			return true;
		}
		override public  function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
							
				ZeroArray(m_bScoreCard, null);
				m_bScoreCard = null;

				ZeroArray(m_bShowCard, null);
				m_bShowCard = null;
				for(var i:uint = 0; i < m_ImageScore.length; i ++)
				{
					m_ImageScore[i].Unload();
					m_ImageScore[i] = null;
				}
				m_ImageScore = null;
				m_ImageTitle.Unload();
				m_ImageTitle = null;
				
				m_ScoreView.Destroy();
				removeChild(m_ScoreView);
				m_ScoreView = null;
				
				ZeroArray(m_UserCardControl, null, DestroyCardControl);
				m_UserCardControl = null;
				m_HandCardControl.removeEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
				DestroyCardControl(m_HandCardControl);
				m_HandCardControl = null;
				ZeroArray(m_LeaveCardControl, null, DestroyCardControl);
			  	m_LeaveCardControl = null;
			
				m_btStart.Destroy();
				removeChild(m_btStart);
				m_btStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
				m_btStart = null;
				m_btExit.Destroy();
				removeChild(m_btExit);
				m_btExit.removeEventListener(MouseEvent.CLICK, OnEventExit);
				m_btExit = null;
				m_btOutCard.Destroy();
				removeChild(m_btOutCard);
				m_btOutCard.removeEventListener(MouseEvent.CLICK, OnEventOutCard);
				m_btOutCard = null;
				m_btShowCard.Destroy();
				removeChild(m_btShowCard);
				m_btShowCard.removeEventListener(MouseEvent.CLICK, OnEventShowCard);
				m_btShowCard = null;
				m_btNotShowCard.Destroy();
				removeChild(m_btNotShowCard);
				m_btNotShowCard.removeEventListener(MouseEvent.CLICK, OnEventNotShowCard);
    			m_btNotShowCard = null;
				m_btAutoOutCard.Destroy();
				removeChild(m_btAutoOutCard);
				m_btAutoOutCard.removeEventListener(MouseEvent.CLICK, OnEventAutoOutCard);
				m_btAutoOutCard = null;
				m_btCancelAutoOutCard.Destroy();
				removeChild(m_btCancelAutoOutCard);
				m_btCancelAutoOutCard.removeEventListener(MouseEvent.CLICK, OnEventCancelAutoOutCard);
				m_btCancelAutoOutCard = null;
			
				ZeroArray(m_PointBanker, null);
				m_PointBanker = null;

				m_bFreeChildClassViewData=true;
			}
			super.DestroyGameView();
		}
		override protected  function CreateDrawBitmapData():BitmapData
		{
			return new UncoverPigBG(1024,768);
		}
		override protected  function CreateUserChairIDView(wChairID:uint,pUserItem:tagUserInfoHead):IChairIDView
		{
			return new ChairIDView(pUserItem.UserStatus.wChairID);
		}
		override protected  function CreateUserReadyView(wChairID:uint,pUserItem:tagUserInfoHead):IUserReadyView
		{
			return new UserReadyView();
		}
		override protected  function CreateClockView(wChairID:uint,wTimer:uint):IClockView
		{
			return new ClockView(wTimer);
		}
		override protected function CreateNetSpeedView():INetSpeedView
		{
			return new NetSpeedView();
		}	
		override protected function CreateUserInfoView():IUserInfoView
		{
			return new UserInfoView();
		}	
		override protected  function GetUserFaceMCIndex(wChairID:uint):int
		{
			switch (wChairID)
			{

			}
			return -1;
		}
		////////////////////////////////////
		//调整控件
		override public  function RectifyGameView(nWidth:int,nHeight:int):void
		{
			//变量定义
			var nViewHeight:Number=nHeight-m_nYBorder;
			var nXCenter:int=nWidth / 2;
			var nYCenter:int=nHeight / 2 - 20;

			//设置坐标
			if (m_bDeasilOrder==false)
			{
				m_ptFace[3].x=nWidth - m_nXBorder - 15 - m_nXFace;
				m_ptFace[3].y=nYCenter - 50;
				m_ptName[3].x=m_ptFace[3].x;
				m_ptName[3].y=m_ptFace[3].y;
				m_ptTimer[3].x=m_ptFace[3].x;
				m_ptTimer[3].y=m_ptFace[3].y + m_nYTimer;
				m_ptReady[3].x=m_ptFace[3].x -91 - m_nXFace + 8;
				m_ptReady[3].y=nYCenter - 50;
				m_PointBanker[3].x=m_ptFace[3].x + 6;
				m_PointBanker[3].y=m_ptFace[3].y - 38;
		
				m_ptFace[1].x=m_nXBorder + 15;
				m_ptFace[1].y=nYCenter - 50;
				m_ptName[1].x=m_ptFace[1].x;
				m_ptName[1].y=m_ptFace[1].y;
				m_ptTimer[1].x=m_ptFace[1].x;
				m_ptTimer[1].y=m_ptFace[1].y + m_nYTimer;
				m_ptReady[1].x=m_ptFace[1].x + m_nXFace + 48;
				m_ptReady[1].y=nYCenter - 50;
				m_PointBanker[1].x=m_ptFace[1].x + 6;
				m_PointBanker[1].y=m_ptFace[1].y - 38;
			}
			else
			{
				m_ptFace[1].x=nWidth - m_nXBorder - 15 - m_nXFace;
				m_ptFace[1].y=nYCenter - 50;
				m_ptName[1].x=m_ptFace[1].x;
				m_ptName[1].y=m_ptFace[1].y;
				m_ptTimer[1].x=m_ptFace[1].x;
				m_ptTimer[1].y=m_ptFace[1].y + m_nYTimer;
				m_ptReady[1].x=m_ptFace[1].x -91 - m_nXFace + 8;
				m_ptReady[1].y=nYCenter - 50;
				m_PointBanker[1].x=m_ptFace[1].x - 6;
				m_PointBanker[1].y=m_ptFace[1].y - 38;
		
				m_ptFace[3].x=m_nXBorder + 15;
				m_ptFace[3].y=nYCenter - 50;
				m_ptName[3].x=m_ptFace[3].x;
				m_ptName[3].y=m_ptFace[3].y;
				m_ptTimer[3].x=m_ptFace[3].x;
				m_ptTimer[3].y=m_ptFace[3].y + m_nYTimer;
				m_ptReady[3].x=m_ptFace[3].x + m_nXFace + 48;
				m_ptReady[3].y=nYCenter - 50;
				m_PointBanker[3].x=m_ptFace[3].x + 6;
				m_PointBanker[3].y=m_ptFace[3].y - 38;
			}
		
			m_ptFace[0].x=nWidth / 2 - m_nXFace/2;
			m_ptFace[0].y=m_nYBorder+ 5;
			m_ptName[0].x=m_ptFace[0].x;
			m_ptName[0].y=m_ptFace[0].y;
			m_ptTimer[0].x=m_ptFace[0].x  - m_nXFace;
			m_ptTimer[0].y=m_ptFace[0].y;
			m_ptReady[0].x=nWidth / 2 -91/2;
			m_ptReady[0].y=m_nYBorder+ 5 + m_nYFace + 60;
			m_PointBanker[0].x=m_ptFace[0].x + m_nXFace;
			m_PointBanker[0].y=m_ptFace[0].y + 5;
		
			m_ptFace[2].x=nWidth / 2 - m_nXFace/2-200;
			m_ptFace[2].y=nHeight - m_nYBorder - m_nYFace - 105;
			m_ptName[2].x=m_ptFace[2].x;
			m_ptName[2].y=m_ptFace[2].y;
			m_ptTimer[2].x=m_ptFace[2].x  - m_nXFace;
			m_ptTimer[2].y=m_ptFace[2].y;
			m_ptReady[2].x=nWidth / 2 -91/2;
			m_ptReady[2].y=nHeight - m_nYBorder - m_nYFace - 170;
			m_PointBanker[2].x=m_ptFace[2].x + m_nXFace;
			m_PointBanker[2].y=m_ptFace[2].y + 5;
		

			var rcButton:Rectangle=new Rectangle;
			rcButton = m_btStart.getButtonRect();
			m_btStart.MoveWindow(nWidth/2-rcButton.width/2,nHeight-258-m_nYBorder);
			m_btExit.MoveWindow(nWidth - 100, nHeight-148);

			rcButton = m_btOutCard.getButtonRect();
			m_btOutCard.MoveWindow((nWidth-rcButton.width)/2,nHeight-266-m_nYBorder+10);
			m_btShowCard.MoveWindow((nWidth)/2-rcButton.width,nHeight-266-m_nYBorder);
			m_btNotShowCard.MoveWindow((nWidth)/2,nHeight-266-m_nYBorder);
		 	m_btAutoOutCard.MoveWindow(nWidth-92,nHeight-200-m_nYBorder);
			m_btCancelAutoOutCard.MoveWindow(nWidth-92,nHeight-200-m_nYBorder);
		
			//移动扑克
			m_HandCardControl.SetBenchmarkPos(nWidth/2 , nHeight-110 , enXCollocateMode.enXCenter , enYCollocateMode.enYBottom);
			m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+90,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-274-m_nYBorder,enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_LeaveCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+58,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);

			//用户扑克
			if (m_bDeasilOrder==true)
			{
				m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-80,nViewHeight/2-71,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+80,nViewHeight/2-71,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
				m_LeaveCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_LeaveCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
			}
			else 
			{
				m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-120,nViewHeight/2-71,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+120,nViewHeight/2-81,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
				m_LeaveCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-25,nHeight/2-50,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_LeaveCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+25,nHeight/2-25,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
			}
		
		
			//积分视图
			m_ScoreView.MoveWindow((nWidth - m_ScoreView.Width())/2,(nHeight-m_ScoreView.Height())/2-80);
			
			m_ptNetSpeedView.x = 34;
			m_ptNetSpeedView.y = 630;
			m_ptUserInfoView.x = 735;
			m_ptUserInfoView.y = 8;
			
			super.RectifyGameView(nWidth, nHeight);
		}
		////////////////////////////////////////////////////


		//绘画界面
		override protected  function DrawGameView(g:Graphics,pDC:BitmapData):void
		{
			var nWidth:Number=pDC.width;
			var nHeight:Number=pDC.height;
			var nYPos:Number=692;
			//左上角
			m_ImageTitle.DrawBitmapData(pDC,44,8);
			DrawUtil.TextOut(pDC,
					 208,
					 29,
					 m_GameClass.GetServerAttribute().fCellScore.toFixed(),
				 	 18,
					 0xffffffff,0,0, "center");
			
			if(true==m_bShowCardHint) 
			 {
				 var HintText:String="选择要亮的牌然后点击亮牌按钮" ;
				 DrawUtil.TextOut(pDC,
				 nWidth/2,nHeight-284-m_nYBorder,
				 HintText,
				 18,0xffffff,0,0,"center") ;
			}
			var ShowChairIDPt:Array = new Array(GAME_PLAYER);
		    var ShowCardPt:Array = new Array(GAME_PLAYER);        //亮分位置
			var ScorePt:Array = new Array(GAME_PLAYER);           //得分位置
			
			for(var i:uint=0 ; i<GAME_PLAYER ; i++)
			{
				ShowChairIDPt[i]= new Point;
				ShowCardPt[i] = new Point;
				ScorePt[i] = new Point;
				
				ShowChairIDPt[i].x  = 2 ;
				ShowChairIDPt[i].y  = nYPos+i*19 + 3;
				ShowCardPt[i].x  = 118 ;
				ShowCardPt[i].y  = nYPos+i*19;
				ScorePt[i].x     = 266 ;
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

					m_ImageScore[bIndex].DrawBitmapData(pDC,ShowCardPt[i].x+j*30,ShowCardPt[i].y);
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
			
					m_ImageScore[bIndex].DrawBitmapData(pDC,ScorePt[i].x+j*30,ShowCardPt[i].y);
				}
			}
		
			var m_UserReadCount:uint=0;
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				var wUserTimer:uint=GetUserTimer(i);
				var pUserData:tagUserInfoHead=GetUserInfo(i);
		
				if (pUserData!=null)
				{
					var strStringBuffer:String = Number(pUserData.UserStatus.wChairID + 1).toString();
					 DrawUtil.TextOut(pDC,
					 ShowChairIDPt[i].x,
					 ShowChairIDPt[i].y,
					 strStringBuffer,
				 	 18,0xffffff,112,6,"center") ;
					 
					//举手
					if (pUserData.UserStatus.cbUserStatus==US_READY)
					{
						if(i!=1)
							m_UserReadCount++;                    //一为当前玩家
						
					}
					else
					{
						if(i!=1)
							m_UserReadCount=0;
					}
					
				}
				else
				{
					m_UserReadCount=0;
				}
			}
			//准备好了
			if(m_UserReadCount==(GAME_PLAYER-1) && m_bSendWaiMsg)
			{
				dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_WAIT_START,0,0));
				m_bSendWaiMsg=false;
			}	
			return;
		}
		//用户顺序
		public function SetUserOrder(bDeasilOrder:Boolean):void
		{
			//状态判断
			if (m_bDeasilOrder==bDeasilOrder) return;
		
			//设置变量
			m_bDeasilOrder=bDeasilOrder;

			//刷新界面
			UpdateGameView();
		
			return;
		}
		//扑克数目
		public function SetCardCount(wChairID:uint, bCardCount:uint):void
		{
			//设置变量
			if (wChairID==INVALID_CHAIR)
			{
				for (var i:uint=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
			}
			else m_bCardCount[wChairID] = bCardCount;
		
			//更新界面
			UpdateGameView();
		
			return;
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
		
			//更新界面
			UpdateGameView();
		
			return ;
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
		
			//更新界面
			UpdateGameView();
		
			return ;
		}
		//开始按钮
		private function OnEventStart(e:MouseEvent):void
		{

			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_START));
		}
		//出牌按钮
		private function OnEventOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_CARD,1,1));
		}
		//亮牌按钮
		private function OnEventShowCard(e:MouseEvent)
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SHOW_CARD,1,1));
		}
		//不亮按钮
		private function OnEventNotShowCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_NOT_SHOW_CARD,1,1));
		}
		//自动代打
		private function OnEventAutoOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_AUTO_OUT_CARD,1,1));
		}
		//取消代打
		private function OnEventCancelAutoOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CANCEL_AUTO_OUT_CARD));
		}
		private function OnEventDoubleClick(e:MouseEvent):void
		{
			m_btOutCard.EnableWindow(false);
			m_HandCardControl.ShootAllCard(false);
		}
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
		}
		private function OnEventLeftHitCard(e:GameViewEvent):void
		{
			dispatchEvent(e.cloneEvent());
		}
		protected function DestroyCardControl(c:CardControl):void
		{
			c.Destroy();
			removeChild(c);
		}
	}
}
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_Uncoverpig.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"