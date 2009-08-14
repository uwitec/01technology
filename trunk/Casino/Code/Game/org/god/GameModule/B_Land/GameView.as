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
	import org.god.GameModule.B_Land.Common.*;
	import org.god.SkinControl.ImageButton;

	public class GameView extends GameBaseView
	{
		//游戏变量
		protected var m_wLandUser:uint;//地主用户
		protected var m_wBombTime:uint;//炸弹倍数
		protected var m_fBaseScore:Number = Number(0);//基础分数
		protected var m_cbLandScore:uint;//地主分数

		//配置变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序
	
		//状态变量
		public var m_bLandTitle:Boolean;//地主标志
		public var m_pPassOutCardView:Array = new Array(GAME_PLAYER);
		public var m_bPass:Array = new Array(GAME_PLAYER);//放弃数组
		public var m_pScoreView:Array =  new Array(GAME_PLAYER);
		public var m_bScore:Array = new Array(GAME_PLAYER);//用户叫分
		public var m_bCardCount:Array = new Array(GAME_PLAYER);//扑克数目
		public var m_bLastTurnCard:Boolean;//上轮扑克
		
		//位置信息
		public var m_ptScore:Array = new Array(GAME_PLAYER);//叫分位置
		
		public var m_btStart:ImageButton;//开始按钮
		public var m_btOutCard:ImageButton;//出牌按钮
		public var m_btPassCard:ImageButton;//放弃按钮
		public var m_btOneScore:ImageButton;
		public var m_btTwoScore:ImageButton;
		public var m_btThreeScore:ImageButton;//3分按钮
		public var m_btGiveUpScore:ImageButton;//放弃按钮
		public var m_btAutoOutCard:ImageButton;//提示按钮

		public var m_HandCardControl:CardControl;//手上扑克
		public var m_BackCardControl:CardControl;//底牌扑克
		public var m_UserCardControl:Array=new Array(3);//扑克视图
		public var m_LeaveCardControl:Array=new Array(2);//结束扑克
		
		protected var m_ImageTitle:SkinImage;//标题位图
		protected var m_ImageLastTurn:SkinImage;//上轮标志
		
		public var m_ScoreView:ScoreView;//积分视图
		
		public function GameView(prarentClass:GameClass)
		{		
			m_nXFace   = FaceView.getSize().x;
			m_nYFace   = FaceView.getSize().y;
			m_nXReady   = UserReadyView.getSize().x;
			m_nYReady   = UserReadyView.getSize().y;
			m_nYBorder = 5;
			m_nXBorder = 5;
			
			super(prarentClass);
			
			m_bWindowDrawMode=true;
		}
		///////////////////////////////////////////////////
		override public function InitGameView():Boolean
		{
			if(super.InitGameView() == false)
				return false;
				
			ZeroArray(m_bPass, false);
			for (var i:uint = 0; i < m_ptScore.length; i ++)
			{
				m_ptScore[i] = new Point;
			}
			
			m_HandCardControl = new CardControl;
			addChild(m_HandCardControl);
			m_HandCardControl.addEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
	
			m_BackCardControl = new CardControl;
			m_BackCardControl.SetCardSpace(90,0,0);
			m_BackCardControl.SetDisplayFlag(true);
			addChild(m_BackCardControl);

			for (i = 0; i < m_UserCardControl.length; i ++)
			{
				m_UserCardControl[i] = new CardControl;
				m_UserCardControl[i].SetDirection(true);
				m_UserCardControl[i].SetDisplayFlag(true);
				addChild(m_UserCardControl[i]);
			}
			for (i = 0; i < m_LeaveCardControl.length; i ++)
			{
				m_LeaveCardControl[i] = new CardControl;
				m_LeaveCardControl[i].SetDirection(false);
				m_LeaveCardControl[i].SetDisplayFlag(true);
				m_LeaveCardControl[i].SetCardSpace(0,18,0);
				addChild(m_LeaveCardControl[i]);
			}
			m_ScoreView = new ScoreView;
			addChild(m_ScoreView);
			
			//加载资源
			m_ImageTitle = new SkinImage(IMAGE_TITLE);
			m_ImageLastTurn= new SkinImage(IMAGE_LAST_TURN_TIP,75,28)
			
			//配置变量
			m_bDeasilOrder=false;

			//状态变量
			m_bLandTitle=false;
			m_bLastTurnCard=false;
			
			m_btStart=new ImageButton(IMAGE_BT_Start,425,37);
			m_btStart.addEventListener(MouseEvent.CLICK,btnStart);
			addChild(m_btStart);
			m_btOutCard=new ImageButton(IMAGE_BT_OutCard,425,37);
			m_btOutCard.addEventListener(MouseEvent.CLICK,btnOutCard);
			addChild(m_btOutCard);			
			m_btPassCard=new ImageButton(IMAGE_BT_Pass,425,37);
			m_btPassCard.addEventListener(MouseEvent.CLICK,btnPassCard);
			addChild(m_btPassCard);	
			m_btOneScore=new ImageButton(IMAGE_BT_OneScore,425,37);
			m_btOneScore.addEventListener(MouseEvent.CLICK,btnOneScore);
			addChild(m_btOneScore);		
			m_btTwoScore=new ImageButton(IMAGE_BT_TwoScore,425,37);
			m_btTwoScore.addEventListener(MouseEvent.CLICK,btnTwoScore);
			addChild(m_btTwoScore);			
			m_btThreeScore=new ImageButton(IMAGE_BT_ThreeScore,425,37);
			m_btThreeScore.addEventListener(MouseEvent.CLICK,btnThreeScore);
			addChild(m_btThreeScore);		
			m_btGiveUpScore=new ImageButton(IMAGE_BT_GiveUp,425,37);
			m_btGiveUpScore.addEventListener(MouseEvent.CLICK,btnGiveUpScore);
			addChild(m_btGiveUpScore);	
			m_btAutoOutCard=new ImageButton(IMAGE_BT_AutoCard,425,37);
			m_btAutoOutCard.addEventListener(MouseEvent.CLICK,btnAutoOutCard);
			addChild(m_btAutoOutCard);

			m_btLastTurnCard.addEventListener(MouseEvent.CLICK,OnLastTurnCard);
			//Ins_BtnSort.addEventListener(MouseEvent.CLICK,OnEventSortCard);
			m_btStusteeControl.addEventListener(MouseEvent.CLICK,OnEventConcealCard);
			m_btStopStusteeControl.addEventListener(MouseEvent.CLICK,OnEventConcealCard);
			//Ins_BtnSound.addEventListener(MouseEvent.CLICK,OnEventSound);
			m_btExit.addEventListener(MouseEvent.CLICK,OnEventExit);
			m_btStopStusteeControl.visible = false;
		
			//游戏变量
			m_fBaseScore=0;
			m_wBombTime=0;
			m_cbLandScore=1;
			m_wLandUser=INVALID_CHAIR;

			//状态变量
			m_bLandTitle=false;
			for (var i:uint = 0; i < GAME_PLAYER; i ++)
			{
				m_bPass[i] = false;
				m_bScore[i] = 0;
				m_bCardCount[i] = 0;
			}
			m_ScoreView.ShowWindow(false);
			
			//隐藏按钮
			m_btStart.ShowWindow(false);
			m_btOutCard.ShowWindow(false);
			m_btPassCard.ShowWindow(false);
			m_btOneScore.ShowWindow(false);
			m_btTwoScore.ShowWindow(false);
			m_btThreeScore.ShowWindow(false);
			m_btGiveUpScore.ShowWindow(false);
			m_btAutoOutCard.ShowWindow(false);

			//禁用控件
			m_btOutCard.EnableWindow(false);
			m_btPassCard.EnableWindow(false);
			m_btAutoOutCard.EnableWindow(false);

			//扑克控件
			m_BackCardControl.SetCardData(null,0);
			m_HandCardControl.SetCardData(null,0);
			m_HandCardControl.SetPositively(false);
			m_HandCardControl.SetDisplayFlag(false);
			m_LeaveCardControl[0].SetCardData(null,0);
			m_LeaveCardControl[1].SetCardData(null,0);

			for (i=0; i<GAME_PLAYER; i++)
			{
				m_UserCardControl[i].SetCardData(null,0);
			}
			
			return true;
		}
		override public function DestroyGameView():void
		{
			if(m_bFreeChildClassViewData == false)
			{
				m_ImageTitle.Unload();
				m_ImageTitle = null;
				m_ImageLastTurn.Unload();
				m_ImageLastTurn = null;
				
				ZeroArray(m_pScoreView,null, removeChild);
				m_pScoreView = null;
				ZeroArray(m_pPassOutCardView,null, removeChild);
				m_pPassOutCardView = null;
				ZeroArray(m_ptScore,null);
				m_ptScore = null;
				
				m_btStart.Destroy();
				m_btStart.removeEventListener(MouseEvent.CLICK,btnStart);
				removeChild(m_btStart);
				m_btStart = null;	
				m_btOutCard.Destroy();
				m_btOutCard.removeEventListener(MouseEvent.CLICK,btnOutCard);
				removeChild(m_btOutCard);		
				m_btOutCard=null;
				m_btPassCard.Destroy();
				m_btPassCard.removeEventListener(MouseEvent.CLICK,btnPassCard);
				removeChild(m_btPassCard);	
				m_btPassCard=null;
				m_btOneScore.Destroy();
				m_btOneScore.removeEventListener(MouseEvent.CLICK,btnOneScore);
				removeChild(m_btOneScore);		
				m_btOneScore=null;
				m_btTwoScore.Destroy();
				m_btTwoScore.removeEventListener(MouseEvent.CLICK,btnTwoScore);
				removeChild(m_btTwoScore);			
				m_btTwoScore=null;
				m_btThreeScore.Destroy();
				m_btThreeScore.removeEventListener(MouseEvent.CLICK,btnThreeScore);
				removeChild(m_btThreeScore);		
				m_btThreeScore=null;
				m_btGiveUpScore.Destroy();
				m_btGiveUpScore.removeEventListener(MouseEvent.CLICK,btnGiveUpScore);
				removeChild(m_btGiveUpScore);	
				m_btGiveUpScore=null;		
				m_btAutoOutCard.Destroy();
				m_btAutoOutCard.removeEventListener(MouseEvent.CLICK,btnAutoOutCard);
				removeChild(m_btAutoOutCard);
				m_btAutoOutCard=null;

				m_btLastTurnCard.removeEventListener(MouseEvent.CLICK,OnLastTurnCard);
				//Ins_BtnSort.removeEventListener(MouseEvent.CLICK,OnEventSortCard);
				m_btStusteeControl.removeEventListener(MouseEvent.CLICK,OnEventConcealCard);
				m_btStopStusteeControl.removeEventListener(MouseEvent.CLICK,OnEventConcealCard);
				//Ins_BtnSound.removeEventListener(MouseEvent.CLICK,OnEventSound);
				m_btExit.removeEventListener(MouseEvent.CLICK,OnEventExit);
			
				m_HandCardControl.removeEventListener(GameViewEvent.strGameViewEvent, OnEventLeftHitCard);
				DestroyCardControl(m_HandCardControl);
				m_HandCardControl = null;
				DestroyCardControl(m_BackCardControl);
				m_BackCardControl = null;
				ZeroArray(m_UserCardControl,null, DestroyCardControl);
				m_UserCardControl = null;
				ZeroArray(m_LeaveCardControl,null, DestroyCardControl);
				m_LeaveCardControl = null;
				
				 m_ScoreView.Destroy();
				removeChild(m_ScoreView);
				m_ScoreView = null;
				
				m_bFreeChildClassViewData = true;
			}
			
			super.DestroyGameView();
		}

		override protected function CreateUserFaceView(wChairID:uint,
												   pUserItem:tagUserInfoHead):IFaceView
		{
			var nIndex:int = pUserItem.wFaceID%2 ? FaceView.nIndex_FarmerGirl : FaceView.nIndex_FarmerBoy;
			var nDirect:int;
			switch(wChairID)
			{
				case 0:
				{
					nDirect = FaceView.nDirect_Left;
				}
				break;
				case 1:
				{
					nDirect = FaceView.nDirect_Left;
				}
				break;
				case 2:
				{
					nDirect = FaceView.nDirect_Right;
				}
				break;
			}
			return new FaceView(nIndex,nDirect);
		}
		override protected  function CreateDrawBitmapData():BitmapData
		{
			return new LandBG(1024,768);
		}
		override protected function CreateUserChairIDView(wChairID:uint,pUserItem:tagUserInfoHead):IChairIDView
		{
			return new ChairIDView(pUserItem.UserStatus.wChairID);
		}
		override protected function CreateUserReadyView(wChairID:uint,pUserItem:tagUserInfoHead):IUserReadyView
		{
			return new UserReadyView();
		}
		override protected function CreateClockView(wChairID:uint,wTimer:uint):IClockView
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
			switch(wChairID)
			{
				case 1:
					return getChildIndex(m_HandCardControl);
			}
			return -1;
		}
		override protected function  DrawGameView(g:Graphics,pDC:BitmapData):void
		{
			var nWidth:Number = pDC.width;
			var nHeight:Number = pDC.height;
			
			//左上角
			m_ImageTitle.DrawBitmapData(pDC,44,4);
			DrawUtil.TextOut(pDC,
					 228,
					 25,
					 m_GameClass.GetServerAttribute().fCellScore.toFixed(0),
				 	 18,
					 0xffffffff,0,0, "center");
			var bMaxScore:uint = m_cbLandScore;
			for(var i:uint = 0; i < m_bScore.length; i ++)
			{
				if(m_bScore[i]>bMaxScore)
					bMaxScore = m_bScore[i];
			}
			DrawUtil.TextOut(pDC,
					 228,
					 48,
					 String(bMaxScore),
				 	 18,
					 0xffffffff,0,0, "center");
			DrawUtil.TextOut(pDC,
					 228,
					 70,
					 String(m_wBombTime),
				 	 18,
					 0xffffffff,0,0, "center");
		
			//上轮标志
			if (m_bLastTurnCard==true)
			{
				//绘画标志
				var nImageWidth:uint=m_ImageLastTurn.GetWidth();
				var nImageHegith:uint=m_ImageLastTurn.GetHeight();
				m_ImageLastTurn.DrawBitmapData(pDC,
				(nWidth-nImageWidth)/2,
				(nHeight-nImageHegith)/2-65);
			}
			
		}
		////////////////////////////////////
		//调整控件
		override public function RectifyGameView( nWidth:int, nHeight:int):void
		{
			//设置坐标
			if (m_bDeasilOrder==true)
			{
				m_ptFace[0].x=857;
				m_ptFace[0].y=244;
				m_ptName[0].x=916;
				m_ptName[0].y=386;
				m_ptTimer[0].x=nWidth-m_nXBorder-m_nXFace-5;
				m_ptTimer[0].y=nHeight/2-m_nYFace-m_nYTimer-5;
				m_ptReady[0].x=794;
				m_ptReady[0].y=290;
				m_ptScore[0].x=nWidth-m_nXBorder-m_nXFace-150;
				m_ptScore[0].y=nHeight/2-m_nYFace+5;
				
				m_ptFace[2].x=12;
				m_ptFace[2].y=244;
				m_ptName[2].x=60;
				m_ptName[2].y=386;
				m_ptTimer[2].x=m_nXBorder+5;
				m_ptTimer[2].y=nHeight/2-m_nYFace-5-m_nYTimer;
				m_ptReady[2].x=139;
				m_ptReady[2].y=290;
				m_ptScore[2].x=m_nXBorder+m_nXFace+107;
				m_ptScore[2].y=nHeight/2-m_nYFace+5;
				
			}
			else
			{
				m_ptFace[2].x=882;
				m_ptFace[2].y=212;
				m_ptName[2].x=942;
				m_ptName[2].y=360;
				m_ptTimer[2].x=940;
				m_ptTimer[2].y=410;
				m_ptReady[2].x=708;
				m_ptReady[2].y=260;
				m_ptScore[2].x=708;
				m_ptScore[2].y=260;
				

				m_ptFace[0].x=-10;
				m_ptFace[0].y=212;
				m_ptName[0].x=39;
				m_ptName[0].y=360;
				m_ptTimer[0].x=35;
				m_ptTimer[0].y=410;
				m_ptReady[0].x=232;
				m_ptReady[0].y=260;
				m_ptScore[0].x=232;
				m_ptScore[0].y=260;
				
			}

			m_ptFace[1].x=169;
			m_ptFace[1].y=545;
			m_ptName[1].x=219;
			m_ptName[1].y=688;
			m_ptTimer[1].x=487;
			m_ptTimer[1].y=489;
			m_ptReady[1].x=470;
			m_ptReady[1].y=470;
			m_ptScore[1].x=468;
			m_ptScore[1].y=462;
			
			var rcButton:Rectangle = m_btStart.getButtonRect();
			m_btStart.MoveWindow((nWidth-rcButton.width)/2,548);
			m_btOutCard.MoveWindow(nWidth/2-rcButton.width*3/2-10,548);
			m_btAutoOutCard.MoveWindow(nWidth/2-rcButton.width/2,548);
			m_btPassCard.MoveWindow(nWidth/2+rcButton.width/2+10,548);

			m_btOneScore.MoveWindow(nWidth/2-135-rcButton.width/2,548);
			m_btTwoScore.MoveWindow(nWidth/2-45-rcButton.width/2,548);
			m_btThreeScore.MoveWindow(nWidth/2+45-rcButton.width/2,548);
			m_btGiveUpScore.MoveWindow(nWidth/2+135-rcButton.width/2,548);

			//移动扑克
			m_BackCardControl.SetBenchmarkPos(nWidth/2,m_nYBorder+44,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_HandCardControl.SetBenchmarkPos(nWidth/2,588,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);
			m_UserCardControl[1].SetBenchmarkPos(nWidth/2,369,enXCollocateMode.enXCenter,enYCollocateMode.enYTop);

			//用户扑克
			if (m_bDeasilOrder==true)
			{
				m_UserCardControl[0].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-90,nHeight/2-30,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_UserCardControl[2].SetBenchmarkPos(m_nXBorder+m_nXFace+90,nHeight/2-30,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
				m_LeaveCardControl[0].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2-50,enXCollocateMode.enXRight,enYCollocateMode.enYCenter);
				m_LeaveCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2-55,enXCollocateMode.enXLeft,enYCollocateMode.enYCenter);
			}
			else
			{
				m_UserCardControl[2].SetBenchmarkPos(789,
													 288,
													 enXCollocateMode.enXRight,
													 enYCollocateMode.enYCenter);
				m_UserCardControl[0].SetBenchmarkPos(235,
													 288,
													 enXCollocateMode.enXLeft,
													 enYCollocateMode.enYCenter);
				m_LeaveCardControl[1].SetBenchmarkPos(848,
													  288,
													  enXCollocateMode.enXCenter,
													  enYCollocateMode.enYCenter);
				m_LeaveCardControl[0].SetBenchmarkPos(176,
													  288,
													  enXCollocateMode.enXCenter,
													  enYCollocateMode.enYCenter);
			}
			
			m_ScoreView.MoveWindow((nWidth-m_ScoreView.Width())/2,
									(nHeight-m_ScoreView.Height()-100)/2);

			
			m_ptNetSpeedView.x = 64;
			m_ptNetSpeedView.y = 700;
			m_ptUserInfoView.x = 735;
			m_ptUserInfoView.y = 10;
			
			super.RectifyGameView(nWidth, nHeight);
		}
		////////////////////////////////////////////////////
		
		private function OnLastTurnCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_LAST_TURN_CARD,0,0));
			return;
		}
		private function OnEventSortCard(e:MouseEvent):void
		{

		}
		private function OnEventConcealCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_TRUSTEE_CONTROL,0,0));
		}
		private function OnEventSound(e:MouseEvent):void
		{

		}
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
		}
		
		private function btnStart(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_START));
		}
		private function btnOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_CARD,1,1));
		}
		private function btnPassCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_PASS_CARD,1,1));
		}
		private function btnOneScore(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_LAND_SCORE,1,1));
		}
		private function btnTwoScore(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_LAND_SCORE,2,2));
		}
		private function btnThreeScore(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_LAND_SCORE,3,3));
		}
		private function btnGiveUpScore(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_LAND_SCORE,255,255));
		}
		private function btnAutoOutCard(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_AUTO_OUTCARD,0,0));
		}
		private function OnEventLeftHitCard(e:GameViewEvent):void
		{
			dispatchEvent(e.cloneEvent());
		}
		///////////////////////////////////////////////////
		//配置函数
		//用户顺序
		public function SetUserOrder( bDeasilOrder:Boolean):void
		{
			
			//设置变量
			m_bDeasilOrder=bDeasilOrder;

			//设置界面

			RectifyGameView(this.width,this.height);

			//刷新界面
			UpdateGameView();

			return;
		}
		//获取顺序
		public function IsDeasilOrder():Boolean
		{
			return m_bDeasilOrder;
		}
		//功能函数
		//炸弹倍数
		public function SetBombTime( wBombTime:uint):void
		{
			//设置变量
			m_wBombTime=wBombTime;

			//刷新界面
			UpdateGameView();
		}
		public function ShowBombMotion(wChairID:uint):void
		{
			for(var i = 0; i < MAX_CHAIR; i ++)
			{
				if(m_pUserFace[i] &&
					m_pUserItem[i])
				{		
					if(m_wLandUser == wChairID)
					{
						if(i != m_wLandUser)
						{
							m_pUserFace[i].showMotion(FaceView.strMotion_Bomb,false);
							m_pUserFace[i].showMotion(FaceView.strMotion_Normal,true);
						}
					}
					else
					{
						if(i == m_wLandUser)
						{
							m_pUserFace[i].showMotion(FaceView.strMotion_Bomb,false);
							m_pUserFace[i].showMotion(FaceView.strMotion_Normal,true);
						}
					}
					
				}
			}
		}
		//基础分数
		public function SetBaseScore(fBaseScore:Number):void
		{
			//设置变量
			m_fBaseScore= new Number(fBaseScore);
						
			//刷新界面
			UpdateGameView();
		}
		//显示提示
		public function ShowLandTitle( bLandTitle:Boolean):void
		{
			//设置变量
			m_bLandTitle=bLandTitle;

			//刷新界面
			UpdateGameView();

			return;
		}
		//设置放弃
		public function SetPassFlag( wChairID:uint,  bPass:Boolean):void
		{
			//设置变量
			if (wChairID==INVALID_CHAIR)
			{
				for (var i:uint=0; i<GAME_PLAYER; i++)
				{
					m_bPass[i]=bPass;
					UpdatePassOutCardView(i);
				}
			}
			else
			{
				m_bPass[wChairID]=bPass;
				UpdatePassOutCardView(wChairID);

			}//更新界面
			UpdateGameView();

			return;
		}
		private function UpdatePassOutCardView(wChairID:uint):void
		{
			if(m_bPass[wChairID])
			{
				if(m_pPassOutCardView[wChairID] == null)
				{
					m_pPassOutCardView[wChairID] = new PassOutCardView;
					addChild(m_pPassOutCardView[wChairID]);
					m_pPassOutCardView[wChairID].x = m_ptScore[wChairID].x;
					m_pPassOutCardView[wChairID].y = m_ptScore[wChairID].y;
				}
			}
			else
			{
				if(m_pPassOutCardView[wChairID])
				{
					removeChild(m_pPassOutCardView[wChairID]);
					m_pPassOutCardView[wChairID] = null;
				}
			}
		}
		//扑克数目
		public function SetCardCount( wChairID:uint,  bCardCount:uint):void
		{
			//设置变量
			if (wChairID==INVALID_CHAIR)
			{
				for (var i:uint=0; i<GAME_PLAYER; i++)
				{
					m_bCardCount[i]=bCardCount;
				}
			}
			else
			{
				m_bCardCount[wChairID]=bCardCount;

			}
			//更新界面
			UpdateGameView();

			return;
		}
		//分数
		public function SetLandUser(wChairID:uint,  bLandScore:uint):void
		{
			//设置变量
			m_wLandUser=wChairID;
			m_cbLandScore=bLandScore;
			for(var i = 0; i < MAX_CHAIR; i ++)
			{
				if(m_pUserFace[i] &&
					m_pUserItem[i])
				{		
					if(i == wChairID)
					{
						var nIndex:int = m_pUserItem[i].wFaceID%2 ? FaceView.nIndex_LandGirl : FaceView.nIndex_LandBoy;
						m_pUserFace[i].switchFaceIndex(nIndex);
					}
					else
					{
						var nIndex:int = m_pUserItem[i].wFaceID%2 ? FaceView.nIndex_FarmerGirl : FaceView.nIndex_FarmerBoy;
						m_pUserFace[i].switchFaceIndex(nIndex);
					}
				}
			}
			//更新界面
			UpdateGameView();
			return;
		}
		//分数
		public function SetLandScore( wChairID:uint,  bLandScore:uint):void
		{
			//设置变量
			if (wChairID!=INVALID_CHAIR)
			{
				m_bScore[wChairID]=bLandScore;
				UpdateScopeView(wChairID);
			}
			else
			{
				for(var i:uint = 0; i < m_bScore.length; i ++)
				{
					m_bScore[i] = 0;
					UpdateScopeView(i);
				}

			}
			//更新界面
			UpdateGameView();

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
		private function UpdateScopeView(wChairID:uint):void
		{
			if(m_bScore[wChairID] != 0)
			{
				if(m_pScoreView[wChairID] == null)
				{
					var xp:Number  = m_ptScore[wChairID].x;
					var yp:Number  = m_ptScore[wChairID].y;
					switch(wChairID)
					{
						case 0:
						xp = xp;
						break;
						case 1:
						xp = xp - 32;
						break;
						case 2:
						xp = xp - 64;
						break;
						default:
						break;
					}
					switch(m_bScore[wChairID])
					{
						case 1:
							m_pScoreView[wChairID] = new OneScoreView;
						break;
						case 2:
							m_pScoreView[wChairID] = new TwoScoreView;
						break;
						case 3:
							m_pScoreView[wChairID] = new ThreeScoreView;
						break;
						default:
						{
							xp  = m_ptScore[wChairID].x;
							m_pScoreView[wChairID] = new PassLandScoreView;
						}
						break;
					}
					
					if(m_pScoreView[wChairID] != null)
					{
						addChild(m_pScoreView[wChairID]);
						m_pScoreView[wChairID].x = xp;
						m_pScoreView[wChairID].y = yp;
					}
				}
			}
			else
			{
				if(m_pScoreView[wChairID])
				{
					removeChild(m_pScoreView[wChairID]);
					m_pScoreView[wChairID] = null;
				}
			}
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
include "./Common/CMD_LAND.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"