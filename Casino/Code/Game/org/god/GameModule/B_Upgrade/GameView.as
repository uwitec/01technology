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
	import org.god.GameModule.B_Upgrade.Common.*;
	import org.god.SkinControl.ImageButton;

	public class GameView extends GameBaseView
	{

		//显示变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序
		protected var m_bWaitConceal:Boolean;//等待标志
		protected var m_bLastTurnCard:Boolean;//上轮扑克

		//游戏变量
		protected var m_wCardScore:uint;//玩家得分
		protected var m_wBankerUser:uint;//庄家用户

		//叫牌信息
		protected var m_cbCallColor:uint;//叫牌花色
		protected var m_cbCallCount:uint;//叫牌数目
		protected var m_cbMainValue:uint;//主牌数值
		protected var m_wCallCardUser:uint;//叫牌用户
		protected var m_cbValueOrder:Array = new Array(2);//主牌数值

		//位置信息
		protected var m_SizeBanker:Point = new Point;//庄家大小
		protected var m_SizeStation:Point = new Point;//状态大小
		protected var m_PointBanker:Array = new Array(GAME_PLAYER);//庄家位置

		//位图变量
		protected var m_ImageBanker:SkinImage;//庄家资源
		protected var m_ImageLastTurn:SkinImage;//上轮标志
		protected var m_ImageColorFlag:SkinImage;//花色标志
		protected var m_ImageWaitConceal:SkinImage;//等待资源
		protected var m_ImageTitle:SkinImage;//标题位图
		
		//按钮控件
		public var m_btStart:ImageButton;//开始按钮
		public var m_btExit:ImageButton;//离开按钮
		public var m_btOutCard:ImageButton;//出牌按钮
		public var m_btOutPrompt:ImageButton;//提示按钮
		public var m_btSendConceal:ImageButton;//留底按钮

		//功能按钮
		public var m_btConcealCard:ImageButton;//查看底牌
		public var m_btRuquestLeave:ImageButton;//请求离开
		public var m_btLastTurnCard:ImageButton;//上轮扑克
		public var m_btStusteeControl:ImageButton;//拖管控制

		//扑克控件
		public var m_CardScore:CardControl;//得分扑克
		public var m_HandCardControl:CardControl;//手上扑克
		public var m_UserCardControl:Array = new Array(4);//扑克视图
		public var m_LeaveCardControl:Array=new Array(3);//结束扑克
		
		//控件组件
		public var m_ScoreView:ScoreView;//积分视图
		public var m_CallCardWnd:CallCardWnd;//叫牌窗口
		public var m_ConcealCardView:ConcealCardView;//底牌窗口

		public function GameView(prarentClass:GameClass)
		{
			m_nYBorder=3;
			m_nXBorder=3;
			m_nXFace = 54;
			m_nYFace = 54;
			m_nXTimer= 54;
			m_nYTimer= 54;
			super(prarentClass);
			m_bWindowDrawMode = true;
		}
		///////////////////////////////////////////////////
		override public function InitGameView():Boolean
		{
			if (super.InitGameView() == false)
			{
				return false;
			}
			m_bDeasilOrder = false;
			for(var i:uint = 0; i < m_PointBanker.length; i ++)
			{
				m_PointBanker[i] = new Point;
			}
			
			m_ImageBanker= new SkinImage(IMAGE_BANKER,36,36);
			m_ImageColorFlag= new SkinImage(IMAGE_COLOR_FLAG,90,15);
			m_ImageLastTurn= new SkinImage(IMAGE_LAST_TURN_TIP,75,28);
			m_ImageWaitConceal= new SkinImage(IMAGE_WAIT_CONCEAL,201,28);
			m_ImageTitle = new SkinImage(IMAGE_TITLE);
			
			//获取大小
			m_SizeBanker.x=m_ImageBanker.GetWidth();
			m_SizeBanker.y=m_ImageBanker.GetHeight();
			m_SizeStation.y = 101;
			
			m_CallCardWnd = new CallCardWnd;
			addChild(m_CallCardWnd);
			m_CallCardWnd.addEventListener(GameViewEvent.strGameViewEvent, OnEventCallCard);
			m_ConcealCardView = new ConcealCardView;
			addChild(m_ConcealCardView);
			//创建扑克
			for (var i:uint=0;i<4;i++)
			{
				m_UserCardControl[i] = new CardControl;
				addChild(m_UserCardControl[i]);
				m_UserCardControl[i].SetDirection(true);
				m_UserCardControl[i].SetDisplayFlag(true);
				m_UserCardControl[i].SetCardSpace(CardControl.DEF_CARD_H_SPACE,
												  CardControl.DEF_CARD_V_SPACE,0);
			}
			m_CardScore = new CardControl;
			addChild(m_CardScore);
			m_HandCardControl = new CardControl;
			addChild(m_HandCardControl);
			m_HandCardControl.addEventListener(GameViewEvent.strGameViewEvent,OnEventLeftHitCard);
			for (i=0;i<3;i++)
			{
				m_LeaveCardControl[i] = new CardControl;
				addChild(m_LeaveCardControl[i]);
				m_LeaveCardControl[i].SetDisplayFlag(true);
				m_LeaveCardControl[i].SetCardSpace(20,18,0);
				m_LeaveCardControl[i].SetDirection((i==0)?true:false);
			}
			m_ScoreView = new ScoreView;
			addChild(m_ScoreView);

			//设置扑克
			m_CardScore.SetDisplayFlag(true);
			m_CardScore.SetCardSpace(CardControl.DEF_CARD_H_SPACE,0,0);
			
			m_btStart= new ImageButton(IMAGE_BT_START, 430,40);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btExit= new ImageButton(IMAGE_BT_EXIT, 430,40);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btOutCard= new ImageButton(IMAGE_BT_OUT_CARD, 430,40);
			addChild(m_btOutCard);
			m_btOutCard.addEventListener(MouseEvent.CLICK, OnEventOutCard);
			m_btOutPrompt= new ImageButton(IMAGE_BT_OUT_PROMPT, 430,40);
			addChild(m_btOutPrompt);
			m_btOutPrompt.addEventListener(MouseEvent.CLICK, OnEventOutPrompt);
			m_btSendConceal= new ImageButton(IMAGE_BT_SEND_CONCEAL, 430,40);
			addChild(m_btSendConceal);
			m_btSendConceal.addEventListener(MouseEvent.CLICK, OnEventSendConceal);
			//功能按钮
			m_btConcealCard= new ImageButton(IMAGE_CONCEAL_CARD,125,25);
			addChild(m_btConcealCard);
			m_btConcealCard.addEventListener(MouseEvent.CLICK, OnEventConcealCard);
			m_btRuquestLeave= new ImageButton(IMAGE_REQUEST_LEAVE,125,25);
			addChild(m_btRuquestLeave);
			m_btRuquestLeave.addEventListener(MouseEvent.CLICK, OnEventRuquestLeave);
			m_btLastTurnCard= new ImageButton(IMAGE_LAST_TURN_CARD,125,25);
			addChild(m_btLastTurnCard);
			m_btLastTurnCard.addEventListener(MouseEvent.CLICK, OnEventLastTurnCard);
			m_btStusteeControl= new ImageButton(IMAGE_START_TRUSTEE,125,25);
			addChild(m_btStusteeControl);
			m_btStusteeControl.addEventListener(MouseEvent.CLICK, OnEventStusteeControl);
			
			//显示变量
			m_bWaitConceal=false;
			m_bLastTurnCard=false;

			//游戏变量
			m_wCardScore=0xFFFF;
			m_wBankerUser=INVALID_CHAIR;

			//叫牌信息
			m_cbCallCount=0;
			m_cbCallColor=COLOR_ERROR;
			m_cbMainValue=VALUE_ERROR;
			m_cbValueOrder[0]=VALUE_ERROR;
			m_cbValueOrder[1]=VALUE_ERROR;
			m_wCallCardUser=INVALID_CHAIR;

			//隐藏控件
			m_btStart.ShowWindow(false);
			m_btOutCard.ShowWindow(false);
			m_btOutPrompt.ShowWindow(false);
			m_btSendConceal.ShowWindow(false);

			//禁用按钮
			m_btConcealCard.EnableWindow(false);
			m_btRuquestLeave.EnableWindow(false);
			m_btLastTurnCard.EnableWindow(false);
			m_btStusteeControl.EnableWindow(false);
			m_btStusteeControl.SetButtonImage(IMAGE_START_TRUSTEE,125,25);

			//设置扑克
			m_CardScore.SetCardData(null,0);
			m_HandCardControl.SetCardData(null,0);
			m_HandCardControl.SetPositively(false);
			m_HandCardControl.SetDisplayFlag(false);
			for ( i=0;i<CountArray(m_UserCardControl);i++)
				m_UserCardControl[i].SetCardData(null,0);
			m_LeaveCardControl[0].SetCardData(null,0);
			m_LeaveCardControl[1].SetCardData(null,0);
			m_LeaveCardControl[2].SetCardData(null,0);
			
			//控件组件
			m_ScoreView.ShowWindow(false);
			m_CallCardWnd.ShowWindow(false);
			m_ConcealCardView.ShowWindow(false);
			
			doubleClickEnabled  = true;
			addEventListener(MouseEvent.DOUBLE_CLICK, OnEventDoubleClick);

			return true;
		}
		override public function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
				ZeroArray(m_PointBanker, null);
				m_PointBanker = null;
				
				m_ImageBanker.Unload();
				m_ImageBanker = null;
				m_ImageColorFlag.Unload();
				m_ImageColorFlag = null;
				m_ImageLastTurn.Unload();
				m_ImageLastTurn = null;
				m_ImageWaitConceal.Unload();
				m_ImageWaitConceal = null;
				m_ImageTitle.Unload();
			    m_ImageTitle = null;
				m_ScoreView.Destroy();
				removeChild(m_ScoreView);
				m_ScoreView = null;
				m_CallCardWnd.addEventListener(GameViewEvent.strGameViewEvent, OnEventCallCard);
				removeChild(m_CallCardWnd);
				m_CallCardWnd = null;
				removeChild(m_ConcealCardView);
				m_ConcealCardView = null;
				
				ZeroArray(m_UserCardControl, null, DestroyCardControl);
				m_UserCardControl = null;
				DestroyCardControl(m_CardScore);
				m_CardScore = null;
				m_HandCardControl.removeEventListener(GameViewEvent.strGameViewEvent,OnEventLeftHitCard);
				DestroyCardControl(m_HandCardControl);
				m_HandCardControl = null;
				ZeroArray(m_LeaveCardControl, null, DestroyCardControl);
			  	m_LeaveCardControl = null;
			
				m_btStart.Destroy();
				removeChild(m_btStart);
				m_btStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
				m_btStart= null;
				m_btExit.Destroy();
				removeChild(m_btExit);
				m_btExit.removeEventListener(MouseEvent.CLICK, OnEventExit);
				m_btExit= null;
				m_btOutCard.Destroy();
				removeChild(m_btOutCard);
				m_btOutCard.removeEventListener(MouseEvent.CLICK, OnEventOutCard);
				m_btOutCard= null;
				m_btOutPrompt.Destroy();
				removeChild(m_btOutPrompt);
				m_btOutPrompt.removeEventListener(MouseEvent.CLICK, OnEventOutPrompt);
				m_btOutPrompt= null;
				m_btSendConceal.Destroy();
				removeChild(m_btSendConceal);
				m_btSendConceal.removeEventListener(MouseEvent.CLICK, OnEventSendConceal);
				m_btSendConceal= null;
				m_btConcealCard.Destroy();
				removeChild(m_btConcealCard);
				m_btConcealCard.removeEventListener(MouseEvent.CLICK, OnEventConcealCard);
				m_btConcealCard= null;
				m_btRuquestLeave.Destroy();
				removeChild(m_btRuquestLeave);
				m_btRuquestLeave.removeEventListener(MouseEvent.CLICK, OnEventRuquestLeave);
				m_btRuquestLeave= null;
				m_btLastTurnCard.Destroy();
				removeChild(m_btLastTurnCard);
				m_btLastTurnCard.removeEventListener(MouseEvent.CLICK, OnEventLastTurnCard);
				m_btLastTurnCard= null;
				m_btStusteeControl.Destroy();
				removeChild(m_btStusteeControl);
				m_btStusteeControl.removeEventListener(MouseEvent.CLICK, OnEventStusteeControl);
				m_btStusteeControl= null;
				
				removeEventListener(MouseEvent.DOUBLE_CLICK, OnEventDoubleClick);
				
				m_bFreeChildClassViewData = true;
			}
			super.DestroyGameView();
		}
		override protected function CreateDrawBitmapData():BitmapData
		{
			return new UpgradeBG(1024,768);
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
		
			m_ptFace[2].x=nWidth / 2 - m_nXFace/2;
			m_ptFace[2].y=nHeight - m_nYBorder - m_nYFace - 5;
			m_ptName[2].x=m_ptFace[2].x;
			m_ptName[2].y=m_ptFace[2].y;
			m_ptTimer[2].x=m_ptFace[2].x  - m_nXFace;
			m_ptTimer[2].y=m_ptFace[2].y;
			m_ptReady[2].x=nWidth / 2 -91/2;
			m_ptReady[2].y=nHeight - m_nYBorder - m_nYFace - 170;
			m_PointBanker[2].x=m_ptFace[2].x + m_nXFace;
			m_PointBanker[2].y=m_ptFace[2].y + 5;
		
			//开始按钮
			var rcButton:Rectangle = m_btStart.getButtonRect();
			m_btStart.MoveWindow(468,498);
			m_btExit.MoveWindow(916,710);
		
			//留底按钮
			rcButton= m_btSendConceal.getButtonRect();
			m_btSendConceal.MoveWindow((nWidth-rcButton.width)/2,nViewHeight-225);
		
			//出牌按钮
			rcButton= m_btOutCard.getButtonRect();
			m_btOutCard.MoveWindow(nWidth/2+10,nViewHeight-225);
			m_btOutPrompt.MoveWindow(nWidth/2-rcButton.width-10,nViewHeight-225);
		
			//功能按钮
			rcButton= m_btConcealCard.getButtonRect();
			m_btStusteeControl.MoveWindow(nWidth-m_nXBorder-(rcButton.width+5),615);
			m_btRuquestLeave.MoveWindow(nWidth-m_nXBorder-(rcButton.width+5)*2,615);
			m_btConcealCard.MoveWindow(nWidth-m_nXBorder-(rcButton.width+5)*3,615);
			m_btLastTurnCard.MoveWindow(nWidth-m_nXBorder-(rcButton.width+5)*4,615);
		
			//移动扑克
			m_CardScore.SetBenchmarkPos(m_nXBorder+200,nViewHeight+5,enXCollocateMode.enXLeft,enYCollocateMode.enYTop);
			m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+110,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-238-m_nYBorder,enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_HandCardControl.SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder-10,enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
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
		
			//叫牌窗口
			m_CallCardWnd.MoveWindow(nWidth/2-60,nViewHeight-m_nYBorder-212);
		
			//积分视图
			m_ScoreView.MoveWindow((nWidth - m_ScoreView.Width())/2,
										(nViewHeight - m_ScoreView.Height())/2-50);
		
			//底牌窗口
			m_ConcealCardView.MoveWindow(nWidth/2 - 289 /2,nViewHeight/2-110);
		
			m_ptNetSpeedView.x = 43;
			m_ptNetSpeedView.y = 723;
			m_ptUserInfoView.x = 735;
			m_ptUserInfoView.y = 8;
			
			super.RectifyGameView(nWidth, nHeight);
		}
		////////////////////////////////////////////////////


		//绘画界面
		override protected function DrawGameView(g:Graphics,pDC:BitmapData):void
		{
			var nWidth:Number = pDC.width;
			var nHeight:Number = pDC.height;
			
			//左上角
			m_ImageTitle.DrawBitmapData(pDC,44,8);
			DrawUtil.TextOut(pDC,
					 208,
					 29,
					 m_GameClass.GetServerAttribute().fCellScore.toFixed(),
				 	 18,
					 0xffffffff,0,0, "center");

			//绘画背景
			var nXPos:uint = 70;
			var nYPos:uint = 621;
			var szString:String;
		
			//主牌信息
			if (m_cbMainValue!=VALUE_ERROR)
			{
				//变量定义
				var szValue:Array = new Array
				(
					"A","2","3","4","5",
					"6","7","8","9","10",
					"J","Q","K"
				);
		
				var nColorImageWidth:uint=m_ImageColorFlag.GetWidth()/6;
				var nColorImageHeight:uint=m_ImageColorFlag.GetHeight();
				//构造字符
				var nNumIndex:int = m_cbMainValue-1;
				if(nNumIndex >= 0 && nNumIndex < 13)
					szString =szValue[nNumIndex];
		
				//输出信息
				DrawUtil.TextOut(pDC,
				nXPos,
				nYPos,
				szString,
				18,
				0xffff00);
		
				//绘画花色
				if (m_cbCallColor!=COLOR_ERROR)
				{
					//绘画标志
					for (var i:uint=0;i<m_cbCallCount;i++) 
					{
						var nXP:uint=nXPos+i*(nColorImageWidth+4) + 18;
						var nYP:uint=nYPos + 5;
						m_ImageColorFlag.DrawBitmapData(pDC,
						nXP,nYP,
						nColorImageWidth*(m_cbCallColor>>4),0,
						nColorImageWidth,
						nColorImageHeight);
					}
				}
				else
				{
					szString = "[ 花色未定 ]";
					DrawUtil.TextOut(pDC,
					nXPos + 18,
					nYPos,
					szString,
					18,
					0xffff00);
				}
			}
		
		
			//庄家信息
			if (m_wBankerUser!=INVALID_CHAIR)
			{
				var pUserData:tagUserInfoHead=GetUserInfo(m_wBankerUser);
				if (pUserData!=null)
				{
					szString = String(pUserData.UserStatus.wChairID + 1).toString() + " 桌";
					//庄家名字
					DrawUtil.TextOut(pDC,
					nXPos,
					nYPos+22,
					szString,
					18,
					0xffff00);
					
					//庄家标志
					m_ImageBanker.DrawBitmapData(pDC,
					m_PointBanker[m_wBankerUser].x,
					m_PointBanker[m_wBankerUser].y,
					0,0,
					m_SizeBanker.x,m_SizeBanker.y);
				}
			}
		
			//叫牌用户
			if (m_wCallCardUser!=INVALID_CHAIR)
			{
				//获取用户
				var pUserData:tagUserInfoHead=GetUserInfo(m_wCallCardUser);
				if (pUserData!=null)
				{
					szString = String(pUserData.UserStatus.wChairID + 1).toString()+ " 座";
					DrawUtil.TextOut(pDC,
					nXPos,
					nYPos+45,
					szString,
					18,
					0xffff00);
				}
			}
		
			//游戏得分
			if (m_wCardScore!=0xFFFF)
			{
				var szCardScore:String=Number(m_wCardScore).toString() + " 分";
				DrawUtil.TextOut(pDC,
					nXPos,
					nYPos+68,
					szCardScore,
					18,
					0xffff00);

			}
		
			//底牌标志
			if (m_bWaitConceal==true)
			{
				//绘画标志
				var nImageWidth:uint=m_ImageWaitConceal.GetWidth();
				var nImageHegith:uint=m_ImageWaitConceal.GetHeight();
				m_ImageWaitConceal.DrawBitmapData(pDC,
				(nWidth-nImageWidth)/2,
				nHeight/2+100);
			}
		
			//上轮标志
			if (m_bLastTurnCard==true)
			{
				//绘画标志
				var nImageWidth:uint=m_ImageLastTurn.GetWidth();
				var nImageHegith:uint=m_ImageLastTurn.GetHeight();
				m_ImageLastTurn.DrawBitmapData(pDC,
				(nWidth-nImageWidth)/2,
				(nHeight-m_SizeStation.y-nImageHegith)/2-65);
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
		
		//设置得分
		public function SetCardScore(wCardScore:uint):void
		{
			//设置用户
			if (wCardScore!=m_wCardScore)
			{
				//设置变量
				m_wCardScore=wCardScore;
		
				//更新界面
				UpdateGameView();
			}
		
			return;
		}
		
		//庄家用户
		public function SetBankerUser(wBankerUser:uint):void
		{
			//设置用户
			if (wBankerUser!=m_wBankerUser)
			{
				//设置变量
				m_wBankerUser=wBankerUser;
		
				//更新界面
				UpdateGameView();
			}
		
			return;
		}
		
		//设置提示
		public function SetWaitConceal(bWaitConceal:Boolean):void
		{
			//设置变量
			if (bWaitConceal!=m_bWaitConceal)
			{
				//设置变量
				m_bWaitConceal=bWaitConceal;
		
				//更新界面
				UpdateGameView();
			}
		
			return;
		}
		
		//设置上轮
		public function SetLastTurnCard(bLastTurnCard:Boolean):void
		{
			//设置变量
			if (bLastTurnCard!=m_bLastTurnCard)
			{
				//设置变量
				m_bLastTurnCard=bLastTurnCard;
		
				//更新界面
				UpdateGameView();
			}
		
			return;
		}
		
		//设置主牌
		public function SetValueOrder(cbMainValue:uint,  cbValueMySelf:uint,  cbValueOther:uint):void
		{
			//设置变量
			m_cbMainValue=cbMainValue;
			m_cbValueOrder[0]=cbValueMySelf;
			m_cbValueOrder[1]=cbValueOther;
		
			//更新界面
			UpdateGameView();
		
			return;
		}
		
		//设置亮主
		public function SetCallCardInfo( wCallCardUser:uint,  cbCallColor:uint,  cbCallCount:uint):void
		{
			//设置变量
			m_cbCallColor=cbCallColor;
			m_cbCallCount=cbCallCount;
			m_wCallCardUser=wCallCardUser;
		
			//更新界面
			UpdateGameView();
		
			return;
		}
		
		
		//开始按钮
		public function OnEventStart(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_START,0,0));
			return;
		}
		
		//出牌按钮
		public function OnEventOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_CARD,1,1));
			return;
		}
		
		//提示按钮
		public function OnEventOutPrompt(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_PROMPT,0,0));
			return;
		}
		
		//留底按钮
		public function OnEventSendConceal(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SEND_CONCEAL,0,0));
			return;
		}
		
		//查看底牌
		public function OnEventConcealCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CONCEAL_CARD,0,0));
			return;
		}
		
		//请求离开
		public function OnEventRuquestLeave(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_REQUEST_LEAVE,0,0));
			return;
		}
		
		//上轮扑克
		public function OnEventLastTurnCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_LAST_TURN_CARD,0,0));
			return;
		}
		
		//拖管控制
		public function OnEventStusteeControl(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_TRUSTEE_CONTROL,0,0));
			return;
		}
		public function OnEventLeftHitCard(e:GameViewEvent):void
		{
			dispatchEvent(e.cloneEvent());
			return;
		}
		public function OnEventCallCard(e:GameViewEvent):void
		{
			dispatchEvent(e.cloneEvent());
			return;
		}
		//鼠标双击
		public function OnEventDoubleClick(e:MouseEvent):void
		{
		
			//设置扑克
			m_btOutCard.EnableWindow(false);
			m_btSendConceal.EnableWindow(false);
			m_HandCardControl.ShootAllCard(false);
		
			return;
		}
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
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
include "./Common/GameLogicDef.as"
include "./Common/CMD_Upgrade.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"