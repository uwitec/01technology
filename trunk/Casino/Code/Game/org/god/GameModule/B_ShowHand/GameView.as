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
	import org.god.GameModule.B_ShowHand.Common.*;
	import org.god.SkinControl.ImageButton;

	public class GameView extends GameBaseView
	{
		//视图变量
		public var m_fMaxGold:Number;//最高下注
		public var m_fBasicGold:Number;//最少分数

		//界面变量
		protected var m_ImageTitle:SkinImage;//标题位图
		protected var m_GoldStation:Array=new Array(8);//筹码位置

		//控件变量
		public var m_btStart:ImageButton;//开始按钮
		public var m_btExit:ImageButton;//离开按钮
		public var m_btFollow:ImageButton;//跟注按钮
		public var m_btGiveUp:ImageButton;//放弃按钮
		public var m_btAddGold:ImageButton;//加注按钮
		public var m_btShowHand:ImageButton;//梭哈按钮
		public var m_GoldView:Array=new Array(8);//用户筹码
		public var m_ScoreView:ScoreView;//积分视图
		public var m_GoldControl:GoldControl;//加注视图
		public var m_CardControl:Array=new Array(4);//扑克控件
		public var m_CardView:CardControl;//扑克视图

		public function GameView(prarentClass:GameClass)
		{
			m_nXBorder = 30;
			m_nYBorder = 15;
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
			for(var i:uint = 0; i < m_GoldStation.length; i ++)
			{
				m_GoldStation[i] = new Point;
			}
			for(i = 0; i < m_GoldView.length; i ++)
			{
				m_GoldView[i] = new GoldView;
			}
			
			m_ImageTitle= new SkinImage(IMAGE_TITLE);
			
			m_GoldControl = new GoldControl;
			addChild(m_GoldControl);
			m_ScoreView = new ScoreView;
			addChild(m_ScoreView);
			for(i=0; i < m_CardControl.length; i ++)
			{
				m_CardControl[i] = new CardControl;
				addChild(m_CardControl[i]);
			}
			m_CardView = new CardControl;
			addChild(m_CardView);
			
				//创建按钮
			m_btStart= new ImageButton(IMAGE_BT_STAR,405,30);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btExit= new ImageButton(IMAGE_BT_EXIT,405,30);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btFollow= new ImageButton(IMAGE_BT_FOLLOW,405,30);
			addChild(m_btFollow);
			m_btFollow.addEventListener(MouseEvent.CLICK, OnEventFollow);
			m_btGiveUp= new ImageButton(IMAGE_BT_GIVE_UP,405,30);
			addChild(m_btGiveUp);
			m_btGiveUp.addEventListener(MouseEvent.CLICK, OnEventGiveUp);
			m_btAddGold= new ImageButton(IMAGE_BT_ADD_GOLD,405,30);
			addChild(m_btAddGold);
			m_btAddGold.addEventListener(MouseEvent.CLICK, OnEventAddGold);
			m_btShowHand= new ImageButton(IMAGE_BT_SHOW_HAND,405,30);
			addChild(m_btShowHand);
			m_btShowHand.addEventListener(MouseEvent.CLICK, OnEventShowHand);
			
			//设置变量
			m_fMaxGold=0;
			m_fBasicGold=0;

			//设置控件
			m_btStart.ShowWindow(false);
			m_btFollow.ShowWindow(false);
			m_btGiveUp.ShowWindow(false);
			m_btAddGold.ShowWindow(false);
			m_btShowHand.ShowWindow(false);
			m_ScoreView.ShowWindow(false);
			m_GoldControl.ShowWindow(false);
			for ( i=0;i<CountArray(m_GoldView);i++) m_GoldView[i].SetGold(0);
			for ( i=0;i<CountArray(m_CardControl);i++) m_CardControl[i].SetCardData(null,0);
		
			return true;
		}
		override public  function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
		
			ZeroArray(m_GoldStation, null);
			m_GoldStation = null;
			ZeroArray(m_GoldView, null);
			m_GoldView = null;
			
			m_ImageTitle.Unload();
			m_ImageTitle = null;
			m_GoldControl.Destroy();
			removeChild(m_GoldControl);
			m_GoldControl = null;
			 m_ScoreView.Destroy();
			removeChild(m_ScoreView);
			m_ScoreView= null;
			ZeroArray(m_CardControl, null, DestroyCardControl);
			m_CardControl = null;
			
			DestroyCardControl(m_CardView);
			m_CardView = null;
			
			m_btStart.Destroy();
			removeChild(m_btStart);
			m_btStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
			m_btStart= null;
			m_btExit.Destroy();
			removeChild(m_btExit);
			m_btExit.removeEventListener(MouseEvent.CLICK, OnEventExit);
			m_btExit= null;
			m_btFollow.Destroy();
			removeChild(m_btFollow);
			m_btFollow.removeEventListener(MouseEvent.CLICK, OnEventFollow);
			m_btFollow= null;
			m_btGiveUp.Destroy();
			removeChild(m_btGiveUp);
			m_btGiveUp.removeEventListener(MouseEvent.CLICK, OnEventGiveUp);
			m_btGiveUp= null;
			m_btAddGold.Destroy();
			removeChild(m_btAddGold);
			m_btAddGold.removeEventListener(MouseEvent.CLICK, OnEventAddGold);
			m_btAddGold= null;
			m_btShowHand.Destroy();
			removeChild(m_btShowHand);
			m_btShowHand.removeEventListener(MouseEvent.CLICK, OnEventShowHand);
			m_btShowHand= null;


				m_bFreeChildClassViewData=true;
			}
			super.DestroyGameView();
		}
		override protected  function CreateDrawBitmapData():BitmapData
		{
			return new ShowHandBG(1024,768);
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
			//设置坐标
			//以自己为最下家
			var rectView:Rectangle = new Rectangle;
			rectView.left		= m_nXBorder;
			rectView.top		= m_nYBorder;
			rectView.right	= nWidth  - m_nXBorder;
			rectView.bottom	= nHeight - m_nYBorder;
		
			var nXCenterPos:Number = nWidth / 2;
			var nYCenterPos:Number = nHeight / 2 - 35;	

			//设置坐标
			m_ptFace[0].x    = nXCenterPos - m_nXFace/2;
			m_ptFace[0].y    = rectView.top - 5;
			m_ptName[0].x    = m_ptFace[0].x;
			m_ptName[0].y    = m_ptFace[0].y;	
			m_ptReady[0].x   = m_ptFace[0].x - m_nXFace/2 -5;
			m_ptReady[0].y   = m_ptFace[0].y + 100;
			m_ptTimer[0].x	 = m_ptFace[0].x + 54;
			m_ptTimer[0].y	 = m_ptFace[0].y;
		
			m_ptFace[1].x    = rectView.right - 122;
			m_ptFace[1].y    = nYCenterPos - 50;
			m_ptName[1].x    = m_ptFace[1].x;
			m_ptName[1].y    = m_ptFace[1].y;
			m_ptReady[1].x   = m_ptFace[1].x - 140;
			m_ptReady[1].y   = m_ptFace[1].y - 10;
			m_ptTimer[1].x	 = m_ptFace[1].x;
			m_ptTimer[1].y	 = m_ptFace[1].y + 54;

			m_ptFace[2].x    = nXCenterPos - m_nXFace/2;
			m_ptFace[2].y    = rectView.bottom - m_nYFace - 15;
			m_ptName[2].x    = m_ptFace[2].x;
			m_ptName[2].y    = m_ptFace[2].y;
			m_ptReady[2].x   = m_ptFace[2].x - m_nXFace/2 + 5;
			m_ptReady[2].y   = m_ptFace[2].y - 100;
			m_ptTimer[2].x	 = m_ptFace[2].x + 54;
			m_ptTimer[2].y	 = m_ptFace[2].y ;
			
			m_ptFace[3].x    = rectView.left + 45;
			m_ptFace[3].y    = nYCenterPos - 50;
			m_ptName[3].x    = m_ptFace[3].x;
			m_ptName[3].y    = m_ptFace[3].y;
			m_ptReady[3].x   = m_ptFace[3].x + 100;
			m_ptReady[3].y   = m_ptFace[3].y - 10;
			m_ptTimer[3].x	 = m_ptFace[3].x;
			m_ptTimer[3].y	 = m_ptFace[3].y + 54;
			
			//筹码控件		
			m_GoldStation[0]= new Point(nXCenterPos-45,nYCenterPos-140);
			m_GoldStation[1]= new Point(nXCenterPos+45,nYCenterPos-140);
			m_GoldStation[2]= new Point(nXCenterPos+142,nYCenterPos-58);
			m_GoldStation[3]= new Point(nXCenterPos+142,nYCenterPos+40);
		
			m_GoldStation[4]= new Point(nXCenterPos+45,nYCenterPos+145);
			m_GoldStation[5]= new Point(nXCenterPos-45,nYCenterPos+145);
			m_GoldStation[6]= new Point(nXCenterPos-173,nYCenterPos+40);
			m_GoldStation[7]= new Point(nXCenterPos-173,nYCenterPos-58);

			m_btStart.MoveWindow(472,542);
			m_btAddGold.MoveWindow(795,530);
			m_btFollow.MoveWindow(764,568);
			m_btGiveUp.MoveWindow(722,606);
			m_btShowHand.MoveWindow(676,642);
			m_btExit.MoveWindow(920,706);
			//成绩窗口
			m_ScoreView.MoveWindow((nWidth-m_ScoreView.Width())/2,(nHeight-m_ScoreView.Height())/2-10);
			
			//加注窗口
			m_GoldControl.SetBasicPoint(915,525);
		
			//扑克控件
			m_CardControl[0].SetBasicStation( new Point(nWidth/2,54 -4 + m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_CardControl[1].SetBasicStation( new Point(nWidth-m_nXBorder-140,nYCenterPos),enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
			m_CardControl[2].SetBasicStation( new Point(nWidth/2,nHeight-54- 4 - 20-m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);
			m_CardControl[3].SetBasicStation( new Point(m_nXBorder+115,nYCenterPos),enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);

			m_ptNetSpeedView.x = 45;
			m_ptNetSpeedView.y = 710;
			m_ptUserInfoView.x = 735;
			m_ptUserInfoView.y = 8;
			
			super.RectifyGameView(nWidth, nHeight);
		}
		////////////////////////////////////////////////////


		//绘画界面
		override protected  function DrawGameView(g:Graphics,pDC:BitmapData):void
		{
			var nWidth:Number = pDC.width;
			var nHeight:Number = pDC.height;
			
			//左上角
			m_ImageTitle.DrawBitmapData(pDC,
										9,
										9);
		
			//游戏信息
			if (m_fMaxGold!=0) 
			{
				var szBuffer:String = m_fMaxGold.toFixed(0);
				DrawUtil.TextOut(pDC,72,12,szBuffer,
								 16,
								 0xffff00,
								 78,15,
								 "center");
			}
			if (m_fBasicGold!=0) 
			{
				var szBuffer:String = m_fBasicGold.toFixed(0);
				DrawUtil.TextOut(pDC,72,36,szBuffer,
								 16,
								 0xffff00,
								 78,15,
								 "center");
			}
		
			//绘画筹码
			var fTotalGold:Number=0;
			for (var i:uint=0;i<CountArray(m_GoldView);i++)
			{
				fTotalGold+=m_GoldView[i].GetGold();
				m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y);
			}
		
			//总注信息
			if (fTotalGold!=0)
			{
				szBuffer = "￥" + Number(fTotalGold).toString();
				DrawUtil.TextOut(pDC,482,366,
								 szBuffer,  
								 28,
								 0xffff00,
								 118,36, "center");
			}
		}
		//金币标题
		public function SetGoldTitleInfo(fMaxGold:Number,  fBasicGold:Number):void
		{
			//设置变量
			m_fMaxGold=fMaxGold;
			m_fBasicGold=fBasicGold;
		
			//更新视图
			UpdateGameView();
		
			return;
		}
		
		//用户筹码
		public function SetUserGoldInfo(wViewChairID:uint,  bTableGold:Boolean,  fGold:Number):void
		{
			//参数判断
			var wGoldPos:uint=wViewChairID*2+((bTableGold==true)?1:0);
			if (wGoldPos>=CountArray(m_GoldStation)) return;
		
			//设置变量
			var m_dwAllGold:Number=0;
			m_GoldView[wGoldPos].SetGold(fGold);
			for (var i:uint=0;i<CountArray(m_GoldStation);i++) 
				m_dwAllGold+=m_GoldView[i].GetGold();
		
			//更新视图
			UpdateGameView();
		
			return;
		}
		
		//开始按钮
		private function OnEventStart(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GAME_START));
		}	
		//跟注按钮
		private function OnEventFollow(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_FOLLOW));
		}
		//下注按钮
		private function OnEventAddGold(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_ADD_GOLD));
		}
		//梭哈按钮
		private function OnEventShowHand(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_SHOW_HAND));
		}
		//取消按钮
		private function OnEventGiveUp(e:MouseEvent)
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GIVE_UP));
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
include "./Common/CMD_SHOWHAND.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"