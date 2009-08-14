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
	import org.god.SkinControl.ISkinImage;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.*;
	import org.god.GameModule.B_BlackJack.Common.*;
	import org.god.SkinControl.ImageButton;
	//游戏视图
	public class GameView extends GameBaseView
	{
		//分牌标识
		public static const NO_CUT:uint =0;//没有分牌
		public static const TOP_CARD:uint =1;//左手分牌
		public static const BOTTOM_CARD:uint = 2;//右手分牌
		//视图变量
		public var m_fMaxGold:Number = 0;//最高下注
		public var m_fBasicGold:Number = 0;//最少分数

		//界面面量
		protected var m_ImageTitle:SkinImage;//标题背景
		protected var m_ImageBanker:SkinImage;//庄家标志
		protected var m_ImageArrowHead:SkinImage;//分牌标识
		protected var m_Rct_Play:Rectangle;//游戏区域

		//标识状态
		protected var m_ImageAddGoldMark:SkinImage;//加注状态
		protected var m_ImageGetCardMark:SkinImage;//要牌状态
		protected var m_ImageStopMark:SkinImage;//停牌状态
		protected var m_ImageBrustMark:SkinImage;//爆牌状态

		//位置变量
		public var m_ptMark:Array = new Array(GAME_PLAYER);//游戏庄家
		public var m_ptCardSum:Array = new Array(GAME_PLAYER);//扑克总值
		public var m_GoldStation:Array = new Array(GAME_PLAYER*2);//筹码位置
		public var m_wBankerStation:uint;//庄家位置

		//游戏变量
		public var m_cbCutCardMark:uint;//分牌标识
		public var m_cbTopCardSum:Array = new Array(GAME_PLAYER);//扑克总值
		public var m_cbBottomCardSum:Array = new Array(GAME_PLAYER);//扑克总值
		public var m_cbPlayStatus:Array = new Array(GAME_PLAYER);//游戏状态

		//控件变量
		public var m_GoldView:Array = new Array(GAME_PLAYER*2);//用户筹码
		public var m_CardControl:Array = new Array(GAME_PLAYER*2);//扑克控件
		public var m_ScoreView:ScoreView;//积分视图
		public var m_GoldControl:GoldControl;//加注视图

		//控件变量
		public var m_btAddGold:ImageButton;//下注按钮
		public var m_btStart:ImageButton;//开始按钮
		public var m_btExit:ImageButton;//离开按钮
		public var m_btGiveUp:ImageButton;//放弃按钮
		public var m_btGetCard:ImageButton;//要牌按钮
		public var m_btStop:ImageButton;//停牌按钮
		public var m_btDouble:ImageButton;//双倍按钮
		public var m_btCutCard:ImageButton;//分牌按钮

		//构造函数
		public function GameView(prarentClass:GameClass)
		{
			m_nXBorder = 54;
			m_nYBorder = 3;
			m_nXFace = 54;
			m_nYFace = 54;
			m_nXTimer= 54;
			m_nYTimer= 54;
			super(prarentClass);
			m_bWindowDrawMode = true;
		}
		//初始化
		///////////////////////////////////////////////////
		override public function InitGameView():Boolean
		{
			if (super.InitGameView() == false)
			{
				return false;
			}
			m_Rct_Play = new Rectangle;
			
			for(var i:uint = 0; i < m_ptMark.length; i ++)
			{
				m_ptMark[i] = new Point;
			}
			for(i= 0; i < m_ptCardSum.length; i ++)
			{
				m_ptCardSum[i] = new Point;
			}
			for(i= 0; i < m_GoldStation.length; i ++)
			{
				m_GoldStation[i] = new Point;
			}

			//加载资源
			m_ImageTitle= new SkinImage(IMAGE_TITLE);
			m_ImageBanker= new SkinImage(IMAGE_BANKER);
			m_ImageArrowHead= new SkinImage(IMAGE_ARROW_HEAD);
			m_ImageAddGoldMark= new SkinImage(IMAGE_ADDGOLD_MARK);
			m_ImageGetCardMark= new SkinImage(IMAGE_GETCARD_MARK);
			m_ImageStopMark= new SkinImage(IMAGE_STOP_MARK);
			m_ImageBrustMark= new SkinImage(IMAGE_BRUST_MARK);
			//金币控件
			m_GoldControl = new GoldControl;
			addChild(m_GoldControl);
			//得分视图
			m_ScoreView = new ScoreView;
			addChild(m_ScoreView);
			for (i = 0; i < m_GoldView.length; i++)
			{
				m_GoldView[i] = new GoldView;
			}
			for (i = 0; i < m_CardControl.length; i++)
			{
				m_CardControl[i] = new CardControl;
				addChild(m_CardControl[i]);
			}
			//创建按钮
			m_btGiveUp = new ImageButton(IMAGE_BT_GIVE_UP, 425, 37);
			addChild(m_btGiveUp);
			m_btGiveUp.addEventListener(MouseEvent.CLICK, OnEventGiveUp);
			m_btAddGold = new ImageButton(IMAGE_BT_ADD_GOLD, 425, 37);
			addChild(m_btAddGold);
			m_btAddGold.addEventListener(MouseEvent.CLICK, OnEventAddGold);
			m_btStart = new ImageButton(IMAGE_BT_BEGIN,680, 83);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btExit = new ImageButton(IMAGE_BT_EXIT,425, 37);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btGetCard = new ImageButton(IMAGE_BT_GET_CARD, 425, 37);
			addChild(m_btGetCard);
			m_btGetCard.addEventListener(MouseEvent.CLICK, OnEventGetCard);
			m_btStop = new ImageButton(IMAGE_BT_STOP_CARD, 425, 37);
			addChild(m_btStop);
			m_btStop.addEventListener(MouseEvent.CLICK, OnEventStopCard);
			m_btDouble = new ImageButton(IMAGE_BT_DOUBLE, 425, 37);
			addChild(m_btDouble);
			m_btDouble.addEventListener(MouseEvent.CLICK, OnEventDoubleCard);
			m_btCutCard = new ImageButton(IMAGE_BT_CUT_CARD, 425, 37);
			addChild(m_btCutCard);
			m_btCutCard.addEventListener(MouseEvent.CLICK, OnEventCutCard);
			
			//金币变量
			m_fMaxGold   = 0;
			m_fBasicGold = 0;

			//界面相关
			m_wBankerStation = 255;
			m_cbCutCardMark  = NO_CUT;
			ZeroArray(m_cbTopCardSum);
			ZeroArray(m_cbBottomCardSum);
			ZeroArray(m_cbPlayStatus);

			//设置控件
			m_ScoreView.ShowWindow(false);
			m_GoldControl.ShowWindow(false);
			
			m_btAddGold.ShowWindow(false);
			m_btStart.ShowWindow(false);
			m_btGiveUp.ShowWindow(false);
			m_btGetCard.ShowWindow(false);
			m_btStop.ShowWindow(false);
			m_btDouble.ShowWindow(false);
			m_btCutCard.ShowWindow(false);
			m_ScoreView.ShowWindow(false);
			m_GoldControl.ShowWindow(false);

			for (i=0;i<CountArray(m_GoldView);i++)
			{
				m_GoldView[i].SetGold(m_fBasicGold);
			}
			for ( i=0;i<CountArray(m_CardControl);i++)
			{
				m_CardControl[i].SetCardData(null,0);
			}

			return true;
		}
		//销毁
		override public function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
				ZeroArray(m_ptMark, null);
				m_ptMark = null;
				ZeroArray(m_ptCardSum, null);
				m_ptCardSum = null;
				ZeroArray(m_GoldStation, null);
				m_GoldStation = null;
				
				m_ImageTitle.Unload();
				m_ImageTitle = null;
				m_ImageBanker.Unload();
				m_ImageBanker = null;
				m_ImageArrowHead.Unload();
				m_ImageArrowHead = null;
				m_ImageAddGoldMark.Unload();
				m_ImageAddGoldMark = null;
				m_ImageGetCardMark.Unload();
				m_ImageGetCardMark = null;
				m_ImageStopMark.Unload();
				m_ImageStopMark = null;
				m_ImageBrustMark.Unload();
				m_ImageBrustMark = null;
				
				m_GoldControl.Destroy();
				removeChild(m_GoldControl);
				m_GoldControl = null;
			
				m_ScoreView.Destroy();
				removeChild(m_ScoreView);
				m_ScoreView = null;
			
				ZeroArray(m_CardControl, null, DestroyCardControl);
				m_CardControl = null;
				
				m_btGiveUp.Destroy();
				removeChild(m_btGiveUp);
				m_btGiveUp = null;
				m_btAddGold.Destroy();
				removeChild(m_btAddGold);
				m_btAddGold = null;
				m_btStart.Destroy();
				removeChild(m_btStart);
				m_btStart = null;
				m_btExit.Destroy();
				removeChild(m_btExit);
				m_btExit = null;
				m_btGetCard.Destroy();
				removeChild(m_btGetCard);
				m_btGetCard = null;
				m_btStop.Destroy();
				removeChild(m_btStop);
				m_btStop = null;
				m_btDouble.Destroy();
				removeChild(m_btDouble);
				m_btDouble = null;
				m_btCutCard.Destroy();
				removeChild(m_btCutCard);
				m_btCutCard = null;
				
				m_Rct_Play = null;

				m_bFreeChildClassViewData = true;
			}
			super.DestroyGameView();
		}
		//创建底图
		override protected function CreateDrawBitmapData():BitmapData
		{
			return new BlackJackBG(1024, 768);
		}
		//创建用户头像
		override protected  function CreateUserFaceView(wChairID:uint,pUserItem:tagUserInfoHead):IFaceView
		{
			return null;
		}
		//创建用户座号
		override protected  function CreateUserChairIDView(wChairID:uint,pUserItem:tagUserInfoHead):IChairIDView
		{
			return new ChairIDView(pUserItem.UserStatus.wChairID);
		}
		//创建准备视图
		override protected  function CreateUserReadyView(wChairID:uint,pUserItem:tagUserInfoHead):IUserReadyView
		{
			return new UserReadyView();
		}
		//创建时钟视图
		override protected  function CreateClockView(wChairID:uint,wTimer:uint):IClockView
		{
			return new ClockView(wTimer);
		}
		//网络速度视图
		override protected function CreateNetSpeedView():INetSpeedView
		{
			return new NetSpeedView();
		}	
		//创建用户信息视图
		override protected function CreateUserInfoView():IUserInfoView
		{
			return new UserInfoView();
		}	
		//获取用户头像影片索引
		override protected function GetUserFaceMCIndex(wChairID:uint):int
		{
			switch (wChairID)
			{

			}
			return -1;
		}
		////////////////////////////////////
		//调整控件
		override public function RectifyGameView( nWidth:int, nHeight:int):void
		{
			m_Rct_Play.left		= m_nXBorder;
			m_Rct_Play.top		= m_nYBorder;
			m_Rct_Play.right	= nWidth  - m_nXBorder;
			m_Rct_Play.bottom	= nHeight - m_nYBorder;
		
			var nXCenterPos:Number = nWidth / 2;
			var nYCenterPos:Number = nHeight / 2;	

			//设置坐标
			m_ptFace[0].x    = nXCenterPos - m_nXFace/2;
			m_ptFace[0].y    = m_Rct_Play.top + 5;
			m_ptName[0].x    = m_ptFace[0].x;
			m_ptName[0].y    = m_ptFace[0].y;	
			m_ptReady[0].x   = m_ptFace[0].x - m_nXFace/2 + 10;
			m_ptReady[0].y   = m_ptFace[0].y + 100;
			m_ptTimer[0].x	 = m_ptFace[0].x + 54;
			m_ptTimer[0].y	 = m_ptFace[0].y;
			m_ptMark[0].x    = m_ptFace[0].x - 40;
			m_ptMark[0].y    = m_ptFace[0].y + 5;
			m_ptCardSum[0].x = m_ptFace[0].x - 60;
			m_ptCardSum[0].y = m_ptFace[0].y + m_nXFace;
		
			m_ptFace[1].x    = m_Rct_Play.right - 60 - m_nXFace;
			m_ptFace[1].y    = nYCenterPos - 60 + m_nYFace;
			m_ptName[1].x    = m_ptFace[1].x;
			m_ptName[1].y    = m_ptFace[1].y;
			m_ptReady[1].x   = m_ptFace[1].x - 140;
			m_ptReady[1].y   = m_ptFace[1].y - 10;
			m_ptTimer[1].x	 = m_ptFace[1].x;
			m_ptTimer[1].y	 = m_ptFace[1].y + 54;
			m_ptMark[1].x    = m_ptFace[1].x + 6;
			m_ptMark[1].y    = m_ptFace[1].y - 40;
			m_ptCardSum[1].x = m_ptFace[1].x - 46;
			m_ptCardSum[1].y = m_ptMark[1].y - 25;

			m_ptFace[2].x    = nXCenterPos - m_nXFace/2;
			m_ptFace[2].y    = m_Rct_Play.bottom - m_nYFace;
			m_ptName[2].x    = m_ptFace[2].x;
			m_ptName[2].y    = m_ptFace[2].y;
			m_ptReady[2].x   = m_ptFace[2].x - m_nXFace/2 + 10;
			m_ptReady[2].y   = m_ptFace[2].y - 100;
			m_ptTimer[2].x	 = m_ptFace[2].x + 54;
			m_ptTimer[2].y	 = m_ptFace[2].y ;
			m_ptMark[2].x    = m_ptFace[2].x - 40;
			m_ptMark[2].y    = m_ptFace[2].y + 10;
			m_ptCardSum[2].x = m_ptFace[2].x - 60;
			m_ptCardSum[2].y = m_ptFace[2].y - m_nXFace - 30;
			
			m_ptFace[3].x    = m_Rct_Play.left + 60;
			m_ptFace[3].y    = nYCenterPos - 60 + m_nYFace;
			m_ptName[3].x    = m_ptFace[3].x;
			m_ptName[3].y    = m_ptFace[3].y;
			m_ptReady[3].x   = m_ptFace[3].x + 100;
			m_ptReady[3].y   = m_ptFace[3].y - 10;
			m_ptTimer[3].x	 = m_ptFace[3].x;
			m_ptTimer[3].y	 = m_ptFace[3].y + 54;
			m_ptMark[3].x    = m_ptFace[3].x + 5;
			m_ptMark[3].y    = m_ptFace[3].y - 40;
			m_ptCardSum[3].x = m_ptFace[3].x + 72;
			m_ptCardSum[3].y = m_ptMark[3].y - 25;
			
			//筹码控件		
			m_GoldStation[0]= new Point(nXCenterPos-58,nYCenterPos-162);
			m_GoldStation[1]= new Point(nXCenterPos+58,nYCenterPos-162);
			m_GoldStation[2]= new Point(nXCenterPos+164,nYCenterPos-58);
			m_GoldStation[3]= new Point(nXCenterPos+164,nYCenterPos+60);
		
			m_GoldStation[4]= new Point(nXCenterPos+58,nYCenterPos+165);
			m_GoldStation[5]= new Point(nXCenterPos-58,nYCenterPos+165);
			m_GoldStation[6]= new Point(nXCenterPos-164,nYCenterPos+60);
			m_GoldStation[7]= new Point(nXCenterPos-164,nYCenterPos-58);
		
			m_btStart.MoveWindow(818,616);
			m_btExit.MoveWindow(914,718);
			m_btGiveUp.MoveWindow(875,622);     
			m_btAddGold.MoveWindow(790,622);    
			m_btCutCard.MoveWindow(914,674);  
			m_btStop.MoveWindow(830,674);
			m_btDouble.MoveWindow(790,622);  
			m_btGetCard.MoveWindow(744,674); 
		
			//成绩窗口
			m_ScoreView.MoveWindow((nWidth-m_ScoreView.Width())/2,
			(nHeight-m_ScoreView.Height())/2-10);
		
			//加注窗口
			m_GoldControl.SetBasicPoint(920,622);
		
			//扑克控件
			m_CardControl[0].SetBasicStation( new Point(nWidth/2,54 + 4 +m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_CardControl[1].SetBasicStation( new Point(nWidth-m_nXBorder-126,nHeight/2),enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
			m_CardControl[2].SetBasicStation( new Point(nWidth/2,nHeight-54- 4 - 20-m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_CardControl[3].SetBasicStation( new Point(m_nXBorder+120,nHeight/2),enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
			m_CardControl[4].SetBasicStation( new Point(nWidth/2,54 + 4 + 20 +m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_CardControl[5].SetBasicStation( new Point(nWidth-m_nXBorder-126,nHeight/2+25),enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
			m_CardControl[6].SetBasicStation( new Point(nWidth/2,nHeight-54- 4-m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_CardControl[7].SetBasicStation( new Point(m_nXBorder+120,nHeight/2+25),enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
		
			m_ptNetSpeedView.x = 64;
			m_ptNetSpeedView.y = 700;
			m_ptUserInfoView.x = 735;
			m_ptUserInfoView.y = 8;
			
			super.RectifyGameView(nWidth, nHeight);

		}
		////////////////////////////////////////////////////

		//绘画界面
		override protected function  DrawGameView(g:Graphics,pDC:BitmapData):void
		{
			var nWidth:Number = pDC.width;
			var nHeight:Number = pDC.height;
			
			//游戏信息
			m_ImageTitle.DrawBitmapData(pDC,
										28,8);
			//游戏信息
			if (m_fMaxGold!=0) 
			{
				var szBuffer:String = m_fMaxGold.toFixed(0);
				DrawUtil.TextOut(pDC,112,17,szBuffer,
								 18,
								 0xffff00,
								 78,18,
								 "center");
			}
			if (m_fBasicGold!=0) 
			{
				var szBuffer:String =m_fBasicGold.toFixed(0);
				DrawUtil.TextOut(pDC,112,38,szBuffer,
								 18,
								 0xffff00,
								 78,18,
								 "center");
			}
			//绘画筹码
			var lTotalGold:int=0;
			for (var i:uint = 0; i < CountArray(m_GoldView); i++)
			{
				m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y);
				//if(i % 2 == 1)continue;
				lTotalGold += m_GoldView[i].GetGold();
			}
			var szBuffer:String;
			//总注信息
			if (lTotalGold != 0)
			{
				szBuffer = "￥" + Number(lTotalGold).toString();
				DrawUtil.TextOut(pDC,nWidth/2,nHeight/2-8,
								 szBuffer,  
								 30,
								 0xffff00,
								 0,0, "center");
				
			}
		
			//扑克总值
			for (var i:uint = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				var pUserData:tagUserInfoHead = GetUserInfo(i);
		
				if (pUserData != null)
				{
					if(m_cbTopCardSum[i] !=0)
					{
						szBuffer =  m_cbTopCardSum[i];
						DrawUtil.TextOut(pDC,
										 m_ptCardSum[i].x, 
										 m_ptCardSum[i].y,
										 szBuffer);
					}
					if(m_cbBottomCardSum[i] !=0)
					{
						szBuffer = m_cbBottomCardSum[i];
						DrawUtil.TextOut(pDC,
										 nWidth/2-15, 
										 nHeight/2+250,
										 szBuffer);
					}
				}
			}
		
		
			//分牌标识
			if(m_cbCutCardMark == TOP_CARD)
			{
				m_ImageArrowHead.DrawBitmapData(pDC,nWidth/2-80,nHeight-160);
			}
			if(m_cbCutCardMark == BOTTOM_CARD)
			{
				m_ImageArrowHead.DrawBitmapData(pDC,nWidth/2-80,nHeight-100);
			}
			
			//玩家状态
			for(i=0;i<GAME_PLAYER;i++)
			{
				if(i == 2) continue;
				switch(m_cbPlayStatus[i])
				{
				case 0:
					{
						break;
					}
					//用户加注
				case 1:
					{				
						m_ImageAddGoldMark.DrawBitmapData(pDC,m_ptMark[i].x,m_ptMark[i].y);
						break;
					}
					//用户要牌
				case 2:
					{				
						m_ImageGetCardMark.DrawBitmapData(pDC,m_ptMark[i].x,m_ptMark[i].y);
						break;
					}
					//用户停牌
				case 3:
					{				
						m_ImageStopMark.DrawBitmapData(pDC,m_ptMark[i].x,m_ptMark[i].y);
						break;
					}
					//用户爆牌
				case 4:
					{				
						m_ImageBrustMark.DrawBitmapData(pDC,m_ptMark[i].x,m_ptMark[i].y);
						break;
					}
				}		
			}
		
			//庄家标识	
			if ((m_wBankerStation != 255))
			{
				m_ImageBanker.DrawBitmapData(pDC,
				m_ptMark[m_wBankerStation].x,
				m_ptMark[m_wBankerStation].y);
			}
			else m_wBankerStation = 255;
		
			return;
		}
		
		//开始按钮
		private function OnEventStart(e:MouseEvent):void
		{		
			ZeroArray(m_cbTopCardSum);
			ZeroArray(m_cbBottomCardSum);
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GAME_START));
		}
		//下注按钮
		private function OnEventAddGold(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_ADD_GOLD));
		}
		//取消按钮
		private function OnEventGiveUp(e:MouseEvent)
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GIVE_UP));
		}
		
		//要牌按钮
		private function OnEventGetCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GET_CARD));
		}
		
		//停牌按钮
		private function OnEventStopCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_STOP_CARD));
		}
		
		//双倍按钮
		private function OnEventDoubleCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_DOUBLE_CARD));
		}
		
		//分牌按钮
		private function OnEventCutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CUT_CARD));
		}
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
		}
		//金币标题
		public function SetGoldTitleInfo( fMaxGold:Number, fBasicGold:Number):void
		{
			//设置变量
			m_fMaxGold   = fMaxGold;
			m_fBasicGold = fBasicGold;
		
			//更新视图
			UpdateGameView();
		
			return;
		}
		
		//用户筹码
		public function SetUserGoldInfo(wViewChairID:uint,bTableGold:Boolean, dwGold:uint):void
		{
			//参数判断
			var wGoldPos:int = wViewChairID * 2 + ((bTableGold == true) ? 1 : 0);
			
			if (wGoldPos >= CountArray(m_GoldStation)) return;
		
			//设置变量
			var m_dwAllGold:uint = 0;
		
			m_GoldView[wGoldPos].SetGold(dwGold);
		
			for (var i:uint = 0; i < CountArray(m_GoldStation); i++) 
			{
				m_dwAllGold += m_GoldView[i].GetGold();
			}
			//更新视图
			UpdateGameView();
		
			return;
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
include "./Common/CMD_BLACKJACK.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"