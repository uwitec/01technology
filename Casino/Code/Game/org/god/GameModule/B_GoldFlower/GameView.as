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
	import org.god.SkinControl.ImageButton;	
	import org.god.Net.*;
	import org.god.GameModule.B_GoldFlower.Common.*;

	//游戏视图
	public class GameView extends GameBaseView
	{
		
		//视图变量
		public var m_fMaxGold:Number;//最高下注
		public var m_fBasicGold:Number;//最少分数

		//界面变量
		protected var m_ImageTitle:SkinImage;//标题位图
		protected var m_BankerView:SkinImage; //庄家视图 ADD
		protected var m_GoldStation:Array = new Array(2*GAME_PLAYER);//筹码位置	
		protected var m_Banker:uint; //ADD  当前庄家
		protected var m_UserFlagPos:Array = new Array(GAME_PLAYER);//ADD  庄家标志在4个方位的坐标
		
		//控件变量
		public var m_btStart:ImageButton;//开始按钮
		public var m_btExit:ImageButton;//离开按钮
		public var m_btFollow:ImageButton;//跟注按钮
		public var m_btGiveUp:ImageButton;//放弃按钮
		public var m_btAddGold:ImageButton;//加注按钮
		public var m_btGoldFlower:ImageButton;//梭哈按钮
		public var m_GoldView:Array=new Array(2*GAME_PLAYER);//用户筹码
		public var m_ScoreView:ScoreView;//积分视图
		public var m_GoldControl:GoldControl;//加注视图
		public var m_CardControl:Array=new Array(GAME_PLAYER);//扑克控件
		public var m_CardView:CardControl;//扑克视图
		public var m_btKanPai:ImageButton;//看牌按钮 //add
		public var m_btKaiPai:ImageButton;//开牌按钮 //add
		//构造函数
		public function GameView(prarentClass:GameClass)
		{
			m_nYBorder=20;
			m_nXBorder=110;
			m_nXFace = 54;
			m_nYFace = 54;
			m_nXTimer= 54;
			m_nYTimer= 54;
			super(prarentClass);
			m_bWindowDrawMode=true;
		}
		///////////////////////////////////////////////////
		//初始化
		override public  function InitGameView():Boolean
		{
			if (super.InitGameView() == false)
			{
				return false;
			}
			for(var i:uint = 0; i < m_UserFlagPos.length; i ++)
			{
				m_UserFlagPos[i] = new Point;
			}
			for(i = 0; i < m_GoldStation.length; i ++)
			{
				m_GoldStation[i] = new Point;
			}
			for(i = 0; i < m_GoldView.length; i ++)
			{
				m_GoldView[i] = new GoldView;
			}
			
			m_ImageTitle= new SkinImage(IMAGE_TITLE);
			m_BankerView= new SkinImage(IMAGE_BANKER);
				
			m_GoldControl = new GoldControl;
			addChild(m_GoldControl);
			m_GoldControl.addEventListener(GameViewEvent.strGameViewEvent, OnEventOnFollow);
			
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
			m_btStart= new ImageButton(IMAGE_BT_STAR,390,78);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btExit= new ImageButton(IMAGE_BT_EXIT,250,50);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btFollow= new ImageButton(IMAGE_BT_FOLLOW,260,52);
			addChild(m_btFollow);
			m_btFollow.addEventListener(MouseEvent.CLICK, OnEventFollow);
			m_btGiveUp= new ImageButton(IMAGE_BT_GIVE_UP,250,50);
			addChild(m_btGiveUp);
			m_btGiveUp.addEventListener(MouseEvent.CLICK, OnEventGiveUp);
			m_btAddGold= new ImageButton(IMAGE_BT_ADD_GOLD,325,65);
			addChild(m_btAddGold);
			m_btAddGold.addEventListener(MouseEvent.CLICK, OnEventAddGold);
			m_btGoldFlower= new ImageButton(IMAGE_BT_SHOW_HAND,340,66);
			addChild(m_btGoldFlower);
			m_btGoldFlower.addEventListener(MouseEvent.CLICK, OnEventGoldFlower);
			m_btKanPai= new ImageButton(IMAGE_BT_KANPAI,250,50);    
			addChild(m_btKanPai);
			m_btKanPai.addEventListener(MouseEvent.CLICK, OnEventKanPai);
			m_btKaiPai= new ImageButton(IMAGE_BT_KAIPAI,325,65);
			addChild(m_btKaiPai);
			m_btKaiPai.addEventListener(MouseEvent.CLICK, OnEventKaiPai);

	
			//设置变量
			m_fMaxGold=0;
			m_fBasicGold=0;
			m_Banker=INVALID_CHAIR;        //add

			//设置控件
			m_btKanPai.ShowWindow(false);                                                                //add
			m_btKaiPai.ShowWindow(false); 
			m_btStart.ShowWindow(false);
			m_btFollow.ShowWindow(false);
			m_btGiveUp.ShowWindow(false);
			m_btAddGold.ShowWindow(false);
			m_btGoldFlower.ShowWindow(false);
			m_ScoreView.ShowWindow(false);
			m_GoldControl.ShowWindow(false);
			for ( i=0;i<CountArray(m_GoldView);i++) m_GoldView[i].SetGold(0);
			for ( i=0;i<CountArray(m_CardControl);i++) m_CardControl[i].SetCardData(null,0);
		
			return true;
		}
		//销毁游戏视图
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
				m_BankerView.Unload();
				m_BankerView = null;
				m_GoldControl.Destroy();
				removeChild(m_GoldControl);
				m_GoldControl.removeEventListener(GameViewEvent.strGameViewEvent, OnEventOnFollow);
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
				m_btGoldFlower.Destroy();
				removeChild(m_btGoldFlower);
				m_btGoldFlower.removeEventListener(MouseEvent.CLICK, OnEventGoldFlower);
				m_btStart= m_btGoldFlower;


				m_bFreeChildClassViewData=true;
			}
			super.DestroyGameView();
		}
		//创建底图
		override protected  function CreateDrawBitmapData():BitmapData
		{
			return new GoldFlowerBG(1024,768);
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
			var nXCenterPos:Number=nWidth/2;
			var nYCenterPos:Number=nHeight/2 - 60;
			
			//设置坐标
			m_ptFace[0].x=nWidth/2 - m_nXFace/2;                                                               //上面头像 
			m_ptFace[0].y=32+m_nYBorder - 45;
			m_ptName[0].x=m_ptFace[0].x;
			m_ptName[0].y=m_ptFace[0].y;
			m_ptTimer[0].x=nWidth/2 + 25;
			m_ptTimer[0].y=32+m_nYBorder - 45;
			m_ptReady[0].x=nWidth/2 - 54/2 - 15;
			m_ptReady[0].y=32+m_nYBorder + 95 - 52 /2;
			m_UserFlagPos[0].x=nWidth/2- 24 - 5 - 25;														
			m_UserFlagPos[0].y=32+m_nYBorder - 45 + 5;		
			
			m_ptFace[1].x=m_nXBorder;																	   //左上角头像
			m_ptFace[1].y=nHeight/2-140;
			m_ptName[1].x=m_nXBorder;
			m_ptName[1].y=nHeight/2-140;
			m_ptTimer[1].x=m_nXBorder;
			m_ptTimer[1].y=nHeight/2-195;
			m_ptReady[1].x=m_nXBorder + 100;
			m_ptReady[1].y=nHeight/2 - 170;
			m_UserFlagPos[1].x=m_nXBorder-30;           												     
			m_UserFlagPos[1].y=m_ptName[1].y + 15;			
			
			m_ptFace[2].x=m_nXBorder;																	   //左上角头像
			m_ptFace[2].y=nHeight/2+30;
			m_ptName[2].x=m_nXBorder;
			m_ptName[2].y=nHeight/2+30;
			m_ptTimer[2].x=m_nXBorder;
			m_ptTimer[2].y=nHeight/2-25;
			m_ptReady[2].x=m_nXBorder + 100;
			m_ptReady[2].y=nHeight/2;
			m_UserFlagPos[2].x=m_nXBorder-30;           												     
			m_UserFlagPos[2].y=m_ptName[2].y + 15;			

			m_ptFace[3].x=490;																	   //下面头像
			m_ptFace[3].y=632;
			m_ptName[3].x=494;
			m_ptName[3].y=632;
			m_ptTimer[3].x=488;
			m_ptTimer[3].y=478;
			m_ptReady[3].x=460;
			m_ptReady[3].y=465;
			m_UserFlagPos[3].x=464;           												    
			m_UserFlagPos[3].y=645;						
			
			m_ptFace[4].x=nWidth - m_nXBorder - 52;																	   //左上角头像
			m_ptFace[4].y=nHeight/2+30;
			m_ptName[4].x=nWidth -m_nXBorder - 52;
			m_ptName[4].y=nHeight/2+30;
			m_ptTimer[4].x=nWidth -m_nXBorder - 52;
			m_ptTimer[4].y=nHeight/2-25;
			m_ptReady[4].x=nWidth -m_nXBorder - 100 - 91;
			m_ptReady[4].y=nHeight/2;
			m_UserFlagPos[4].x=nWidth -m_nXBorder-30-15;           												     
			m_UserFlagPos[4].y=m_ptName[4].y + 15;			
				
			m_ptFace[5].x=nWidth - m_nXBorder - 52;																	   //左上角头像
			m_ptFace[5].y=nHeight/2-140;
			m_ptName[5].x=nWidth - m_nXBorder - 52;
			m_ptName[5].y=nHeight/2-140;
			m_ptTimer[5].x=nWidth - m_nXBorder - 52;
			m_ptTimer[5].y=nHeight/2-195;
			m_ptReady[5].x=nWidth - m_nXBorder - 100 - 91;
			m_ptReady[5].y=nHeight/2 - 170;
			m_UserFlagPos[5].x=nWidth - m_nXBorder+30-15;           												     
			m_UserFlagPos[5].y=m_ptName[5].y + 15;			
		
			//筹码控件
			var nGoldViewWidth:Number = GoldView.GOLD_IMAGE_HEIGHT/2 - 10;
			m_GoldStation[0]= new Point(nXCenterPos-32-nGoldViewWidth,nYCenterPos-128);
			m_GoldStation[1]= new Point(nXCenterPos+32-nGoldViewWidth,nYCenterPos-128);
			m_GoldStation[2]= new Point(nXCenterPos-166-nGoldViewWidth,nYCenterPos-53);
			m_GoldStation[3]= new Point(nXCenterPos-166-nGoldViewWidth,nYCenterPos-133);
			m_GoldStation[4]= new Point(nXCenterPos-166-nGoldViewWidth,nYCenterPos+107);
			m_GoldStation[5]= new Point(nXCenterPos-166-nGoldViewWidth,nYCenterPos+27);
			m_GoldStation[6]= new Point(nXCenterPos+32-nGoldViewWidth,nYCenterPos+105);
			m_GoldStation[7]= new Point(nXCenterPos-32-nGoldViewWidth,nYCenterPos+105);
			m_GoldStation[8]= new Point(nXCenterPos+166-nGoldViewWidth,nYCenterPos+27);
			m_GoldStation[9]= new Point(nXCenterPos+166-nGoldViewWidth,nYCenterPos+107);
			m_GoldStation[10]= new Point(nXCenterPos+166-nGoldViewWidth,nYCenterPos-133);
			m_GoldStation[11]= new Point(nXCenterPos+166-nGoldViewWidth,nYCenterPos-53);		
			
			m_btStart.MoveWindow(628,546);
			m_btExit.MoveWindow(920,678);
			m_btAddGold.MoveWindow(665,525);
			m_btFollow.MoveWindow(730,531);
			m_btGiveUp.MoveWindow(785,532);
			m_btGoldFlower.MoveWindow(696,576);
			m_btKanPai.MoveWindow(616,532);       //add
			m_btKaiPai.MoveWindow(631,583);       //add
			
			//成绩窗口
			m_ScoreView.MoveWindow((nWidth-m_ScoreView.Width())/2,
								(nHeight-m_ScoreView.Height())/2-50);
			
			//加注窗口
			m_GoldControl.SetBasicPoint(nWidth-120,nHeight-100-m_nYBorder);
		
			//扑克控件
			var nXBorder:Number = 50;
			m_CardControl[0].SetBasicStation(new Point(nWidth/2,38+m_nYBorder),enXCollocateMode.enXCenter,enYCollocateMode.enYTop);                       //上面扑克
			m_CardControl[5].SetBasicStation(new Point(nWidth-nXBorder-5-120,nHeight/2-140),enXCollocateMode.enXRight,enYCollocateMode.enYCenter);       //右上角扑克
			m_CardControl[4].SetBasicStation(new Point(nWidth-nXBorder-5-120,nHeight/2+30),enXCollocateMode.enXRight,enYCollocateMode.enYCenter);        //右下角扑克
			m_CardControl[3].SetBasicStation(new Point(nWidth/2,532),enXCollocateMode.enXCenter,enYCollocateMode.enYTop);             //下边扑克
			m_CardControl[1].SetBasicStation(new Point(nXBorder+5+120,nHeight/2-140),enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);	             //左上角扑克
			m_CardControl[2].SetBasicStation(new Point(nXBorder+5+120,nHeight/2+30),enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);	             //左下角扑克
			
			m_ptNetSpeedView.x = 38;
			m_ptNetSpeedView.y = 696;
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
			m_ImageTitle.DrawBitmapData(pDC,20,8);
			//游戏信息
			if (m_fMaxGold!=0) 
			{
				var szBuffer:String = m_fMaxGold.toFixed(0);
				DrawUtil.TextOut(pDC,98,14,szBuffer,
								 16,
								 0xffff00,
								 80,14,
								 "center");
			}
			if (m_fBasicGold!=0) 
			{
				var szBuffer:String = m_fBasicGold.toFixed(0);
				DrawUtil.TextOut(pDC,98,31,szBuffer,
								 16,
								 0xffff00,
								 80,14,
								 "center");
			}
			/*
			if(m_Banker!=INVALID_CHAIR)
			{
				m_BankerView.DrawBitmapData(pDC,m_UserFlagPos[m_Banker].x,
												m_UserFlagPos[m_Banker].y,
												0,0,
												m_BankerView.GetWidth()/4,
												m_BankerView.GetHeight());	
			}	
			*/
		
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
				var szBuffer:String = Number(fTotalGold).toString();
				DrawUtil.TextOut(pDC,484,
								 330,
								 szBuffer,
								 26,
								 0xffff00,
								 116,31,
								 "center");
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
			var dwAllGold:Number=0;
			m_GoldView[wGoldPos].SetGold(fGold);
			for (var i:uint=0;i<CountArray(m_GoldStation);i++) 
				dwAllGold+=m_GoldView[i].GetGold();
		
			//更新视图
			UpdateGameView();
		
			return;
		}
		public function  GetCurrentUser(MyCurrentUser:uint):void
		{
			m_Banker=MyCurrentUser;
			UpdateGameView();
		}
		//开始按钮
		private function OnEventStart(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_START));
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
		private function OnEventGoldFlower(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GOLDFLOWER));
		}
		//取消按钮
		private function OnEventGiveUp(e:MouseEvent)
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_GIVE_UP));
		}
		//看牌按钮
		private function OnEventKanPai(e:MouseEvent)
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_KANPAI));
		
			m_CardControl[3].SetShowCardFlag(true);
			UpdateGameView();
			return;
		}
		//开牌按钮
		private function OnEventKaiPai(e:MouseEvent)
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_KAIPAI));
		
			return;
		}
		//退出按钮
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
		}
		//跟注按钮
		private function OnEventOnFollow(e:GameViewEvent):void
		{
			dispatchEvent(e.cloneEvent());
		}
		//销毁扑克控件
		protected function DestroyCardControl(c:CardControl):void
		{
			c.Destroy();
			removeChild(c);
		}
	}
}
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_GoldFlower.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"