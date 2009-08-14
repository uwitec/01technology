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
	import org.god.GameModule.B_Hoe.Common.*;
	import org.god.SkinControl.ImageButton;

	public class GameView extends GameBaseView
	{
		//配置变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序

		//状态变量
		public var m_bPass:Array=new Array(GAME_PLAYER);//放弃数组
		public var m_bCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		public var m_PointUserPass:Array=new Array(GAME_PLAYER);//放弃位置

		//历史积分
		public var m_fAllTurnScore:Array=new Array(GAME_PLAYER);//总局得分
		public var m_fLastTurnScore:Array=new Array(GAME_PLAYER);//上局得分

		//位图变量
		protected var m_ImageUserPass:SkinImage;//放弃标志
		protected var m_ImageTitle:SkinImage;//标题位图
		
		//按钮控件
		public var m_btStart:ImageButton;//开始按钮
		public var m_btExit:ImageButton;//离开按钮
		public var m_btOutCard:ImageButton;//出牌按钮
		public var m_btPassCard:ImageButton;//放弃按钮
		public var m_btOutPrompt:ImageButton;//提示按钮

		//扑克控件
		public var m_HandCardControl:CardControl;//手上扑克
		public var m_UserCardControl:Array=new Array(4);//扑克视图
		public var m_LeaveCardControl:Array=new Array(3);//结束扑克

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
			for(var i:uint = 0; i < m_PointUserPass.length; i ++)
			{
				m_PointUserPass[i]=new Point;
			}
			//配置变量
			m_bDeasilOrder=false;

			//状态变量
			ZeroArray(m_bPass,false);
			ZeroArray(m_bCardCount);

			//历史积分
			ZeroArray(m_fAllTurnScore,Number(0));
			ZeroArray(m_fLastTurnScore,Number(0));

			//加载资源
			m_ImageUserPass = new SkinImage(IMAGE_USER_PASS,78,39);
			m_ImageTitle = new SkinImage(IMAGE_TITLE);
			
			//创建扑克
			for (i=0;i<4;i++)
			{
				m_UserCardControl[i] = new CardControl;
				addChild(m_UserCardControl[i]);
				m_UserCardControl[i].SetDirection(true);
				m_UserCardControl[i].SetDisplayFlag(true);
				
			}
			for (i=0;i<3;i++)
			{
				m_LeaveCardControl[i] = new CardControl;
				addChild(m_LeaveCardControl[i]);
				m_LeaveCardControl[i].SetDisplayFlag(true);
				m_LeaveCardControl[i].SetCardSpace(20,18,0);
				m_LeaveCardControl[i].SetDirection((i==0)?true:false);
			}
			m_HandCardControl = new CardControl;
			addChild(m_HandCardControl);
			m_HandCardControl.addEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
			m_ScoreView = new ScoreView;
			addChild(m_ScoreView);
			
			//创建按钮
			m_btStart = new ImageButton(IMAGE_BT_START,680,83);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btExit = new ImageButton(IMAGE_BT_EXIT,425,37);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btOutCard = new ImageButton(IMAGE_BT_OUT_CARD,640,82);
			addChild(m_btOutCard);
			m_btOutCard.addEventListener(MouseEvent.CLICK, OnEventOutCard);
			m_btPassCard = new ImageButton(IMAGE_BT_PASS,640,82);
			addChild(m_btPassCard);
			m_btPassCard.addEventListener(MouseEvent.CLICK, OnEventPassCard);
			m_btOutPrompt = new ImageButton(IMAGE_BT_OUT_PROMPT,640,82);
			addChild(m_btOutPrompt);
			m_btOutPrompt.addEventListener(MouseEvent.CLICK, OnEventOutPrompt);
			
			//隐藏控件
			m_ScoreView.ShowWindow(false);

			//隐藏按钮
			m_btStart.ShowWindow(false);
			m_btOutCard.ShowWindow(false);
			m_btPassCard.ShowWindow(false);
			m_btOutPrompt.ShowWindow(false);

			//禁用控件
			m_btOutCard.EnableWindow(false);
			m_btPassCard.EnableWindow(false);

			//扑克控件
			m_HandCardControl.SetCardData(null,0);
			m_HandCardControl.SetPositively(false);
			m_HandCardControl.SetDisplayFlag(false);
			m_LeaveCardControl[0].SetCardData(null,0);
			m_LeaveCardControl[1].SetCardData(null,0);
			m_LeaveCardControl[2].SetCardData(null,0);
			for ( i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(null,0);


			return true;
		}
		override public  function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
			   var i:uint;
				
			   ZeroArray(m_PointUserPass, null);
			   m_PointUserPass = null;
				

			  //状态变量
			   m_bPass = null;
			   m_bCardCount = null;

			  //历史积分
			  ZeroArray(m_fAllTurnScore,null);
			  m_fAllTurnScore = null;
			  ZeroArray(m_fLastTurnScore,null);
			  m_fLastTurnScore = null;

			 //资源
			  m_ImageUserPass.Unload();
			  m_ImageUserPass = null;
			  m_ImageTitle.Unload();
			  m_ImageTitle = null;
				
			   //扑克控件
			  m_HandCardControl.SetCardData(null,0);
			  m_HandCardControl.SetPositively(false);
			  m_HandCardControl.SetDisplayFlag(false);
			  m_LeaveCardControl[0].SetCardData(null,0);
			  m_LeaveCardControl[1].SetCardData(null,0);
			  m_LeaveCardControl[2].SetCardData(null,0);
			  for ( i=0;i<GAME_PLAYER;i++) m_UserCardControl[i].SetCardData(null,0);
			
			  m_ScoreView.Destroy();
			  removeChild(m_ScoreView);
			  m_ScoreView= null;

			  ZeroArray(m_UserCardControl, null, DestroyCardControl);
			  m_UserCardControl = null;
			  ZeroArray(m_LeaveCardControl, null, DestroyCardControl);
			  m_LeaveCardControl = null;
				
			  m_HandCardControl.removeEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
			  DestroyCardControl(m_HandCardControl);
			  m_LeaveCardControl = null;
			
			  m_btStart.Destroy();
			  m_btStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
			  removeChild(m_btStart);
			  m_btStart = null;
			  m_btExit.Destroy();
			  m_btExit.removeEventListener(MouseEvent.CLICK, OnEventExit);
			  removeChild(m_btExit);
			  m_btExit = null;
			  m_btOutCard.Destroy();
			  m_btOutCard.removeEventListener(MouseEvent.CLICK, OnEventOutCard);
			  removeChild(m_btOutCard);
			  m_btOutCard = null; 
			  m_btPassCard.Destroy();
			  m_btPassCard.removeEventListener(MouseEvent.CLICK, OnEventPassCard);
			  removeChild(m_btPassCard);
			  m_btPassCard = null;	 
			  m_btOutPrompt.Destroy();
			  m_btOutPrompt.removeEventListener(MouseEvent.CLICK, OnEventOutPrompt);
			  removeChild(m_btOutPrompt);
			  m_btOutPrompt = null;
		
		
			  m_bFreeChildClassViewData=true;
			}
			super.DestroyGameView();
		}
		override protected  function CreateDrawBitmapData():BitmapData
		{
			return new HoeBG(1024,768);
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
			var nXCenter:int=nWidth / 2;
			var nYCenter:int=nHeight / 2 - 20;
		
			//设置坐标
			if (m_bDeasilOrder==false)
			{
				m_ptFace[1].x=m_nXBorder + 15;
				m_ptFace[1].y=nYCenter - 50;
				m_ptName[1].x=m_nXBorder + 15;
				m_ptName[1].y=nYCenter - 50;
				m_ptTimer[1].x=m_nXBorder + 15;
				m_ptTimer[1].y=nYCenter - 50 + m_nYTimer;
				m_ptReady[1].x=m_ptFace[1].x + m_nXFace + 48;
				m_ptReady[1].y=nYCenter - 50;
				m_PointUserPass[1].x=m_ptReady[1].x + 120;
				m_PointUserPass[1].y=nHeight/2-m_nYFace - 20;
		
				m_ptFace[3].x=nWidth - m_nXBorder - 15 - m_nXFace;
				m_ptFace[3].y=nYCenter - 50;
				m_ptName[3].x=m_ptFace[3].x;
				m_ptName[3].y=m_ptFace[3].y;
				m_ptTimer[3].x=m_ptFace[3].x;
				m_ptTimer[3].y=m_ptFace[3].y + m_nYTimer;
				m_ptReady[3].x=m_ptFace[3].x -91 - m_nXFace + 8;
				m_ptReady[3].y=nYCenter - 50;
				m_PointUserPass[3].x=m_ptReady[3].x - 120;
				m_PointUserPass[3].y=nHeight/2-m_nYFace - 20;
			}
			else
			{
				m_ptFace[3].x=m_nXBorder + 15;
				m_ptFace[3].y=nYCenter - 50;
				m_ptName[3].x=m_nXBorder + 15;
				m_ptName[3].y=nYCenter - 50;
				m_ptTimer[3].x=m_nXBorder + 15;
				m_ptTimer[3].y=nYCenter - 50 + m_nYTimer;
				m_ptReady[3].x=m_ptFace[3].x + m_nXFace + 48;
				m_ptReady[3].y=nYCenter - 50;
				m_PointUserPass[3].x=m_ptReady[3].x + 120;
				m_PointUserPass[3].y=nHeight/2-m_nYFace - 20;
		
				m_ptFace[1].x=nWidth - m_nXBorder - 15 - m_nXFace;
				m_ptFace[1].y=nYCenter - 50;
				m_ptName[1].x=m_ptFace[1].x;
				m_ptName[1].y=m_ptFace[1].y;
				m_ptTimer[1].x=m_ptFace[1].x;
				m_ptTimer[1].y=m_ptFace[1].y + m_nYTimer;
				m_ptReady[1].x=m_ptFace[1].x -91 - m_nXFace + 8;
				m_ptReady[1].y=nYCenter - 50;
				m_PointUserPass[1].x=m_ptReady[1].x - 120;
				m_PointUserPass[1].y=nHeight/2-m_nYFace - 20;
			}
		
			m_ptFace[0].x=nWidth / 2 - m_nXFace/2;
			m_ptFace[0].y=m_nYBorder+ 5;
			m_ptName[0].x=m_ptFace[0].x;
			m_ptName[0].y=m_ptFace[0].y;
			m_ptTimer[0].x=m_ptFace[0].x  - m_nXFace;
			m_ptTimer[0].y=m_ptFace[0].y;
			m_ptReady[0].x=nWidth / 2 -91/2;
			m_ptReady[0].y=m_nYBorder+ 5 + m_nYFace + 60;
			m_PointUserPass[0].x=nWidth/2-79/2;
			m_PointUserPass[0].y=nYCenter-180;
		
			m_ptFace[2].x=nWidth / 2 - m_nXFace/2;
			m_ptFace[2].y=nHeight - m_nYBorder - m_nYFace - 5;
			m_ptName[2].x=m_ptFace[2].x;
			m_ptName[2].y=m_ptFace[2].y;
			m_ptTimer[2].x=m_ptFace[2].x  - m_nXFace;
			m_ptTimer[2].y=m_ptFace[2].y;
			m_ptReady[2].x=nWidth / 2 -91/2;
			m_ptReady[2].y=nHeight - m_nYBorder - m_nYFace - 170;
			m_PointUserPass[2].x=nWidth/2-79/2;
			m_PointUserPass[2].y=nHeight-m_nYBorder-m_nYFace-210;
		
			//移动按钮
			var rcButton:Rectangle = m_btStart.getButtonRect();
			m_btStart.MoveWindow((nWidth-rcButton.width)/2,nHeight-262-m_nYBorder);
			rcButton = m_btOutCard.getButtonRect();
			m_btOutCard.MoveWindow(nWidth/2-rcButton.width*3/2-10,nHeight-262-m_nYBorder);
			m_btOutPrompt.MoveWindow(nWidth/2-rcButton.width/2,nHeight-262-m_nYBorder);
			m_btPassCard.MoveWindow(nWidth/2+rcButton.width/2+10,nHeight-262-m_nYBorder);
			m_btExit.MoveWindow(888.8,682.8);
		
			//移动扑克
			m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-10,enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-246,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-198-m_nYBorder,enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_LeaveCardControl[0].SetBenchmarkPos(nWidth/2,m_nXBorder+58,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
		
			//用户扑克
			if (m_bDeasilOrder==true)
			{
				m_UserCardControl[1].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-20,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_UserCardControl[3].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-20,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
				m_LeaveCardControl[1].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_LeaveCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
			}
			else 
			{
				m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-130,nHeight/2-20,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+130,nHeight/2-20,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
				m_LeaveCardControl[2].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_LeaveCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-50,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
			}
		
			//积分视图
			m_ScoreView.MoveWindow((nWidth-m_ScoreView.Width())/2,
									nHeight/2-m_ScoreView.Height()/2 - 30);
			
			m_ptNetSpeedView.x = 64;
			m_ptNetSpeedView.y = 700;
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
			//左上角
			m_ImageTitle.DrawBitmapData(pDC,44,8);
			DrawUtil.TextOut(pDC,
					 208,
					 29,
					 m_GameClass.GetServerAttribute().fCellScore.toFixed(),
				 	 18,
					 0xffffffff,0,0, "center");
			
			//放弃标志
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				if (m_bPass[i]==true) 
				{
					m_ImageUserPass.DrawBitmapData(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y);
				}
			}
		}
		//用户顺序
		public function SetUserOrder( bDeasilOrder:Boolean):void
		{
			//状态判断
			if (m_bDeasilOrder==bDeasilOrder) return;
		
			//设置变量
			m_bDeasilOrder=bDeasilOrder;
			

			//刷新界面
			UpdateGameView();
		
			return;
		}
		
		//设置放弃
		public function SetPassFlag(wChairID:uint, bPass:Boolean):void
		{
			//设置变量
			if (wChairID==INVALID_CHAIR)
			{
				for (var i:uint=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
			}
			else m_bPass[wChairID]=bPass;
		
			//更新界面
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
			else m_bCardCount[wChairID]=bCardCount;
		
			//更新界面
			UpdateGameView();
		
			return;
		}
		
		//开始按钮
		private function OnEventStart(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_START,0,0));
			return;
		}
		
		//出牌按钮
		private function OnEventOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_CARD,1,1));
			return;
		}
		
		//放弃按钮
		private function OnEventPassCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_PASS_CARD,1,1));
			return;
		}
		
		//提示按钮
		private function OnEventOutPrompt(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_PROMPT,0,0));
			return;
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
include "./Common/CMD_Hoe.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"