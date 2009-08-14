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
	import org.god.SkinControl.ImageButton;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.*;
	import org.god.GameModule.B_Sparrow.Common.*;

	public class GameView extends GameBaseView
	{
		//标志变量
		protected var m_bOutSparrow:Boolean;//出牌标志
		protected var m_bWaitOther:Boolean;//等待标志
		protected var m_bHuangZhuang:Boolean;//荒庄标志

		//游戏属性
		protected var m_fCellScore:Number=0;//基础积分
		protected var m_wBankerUser:uint;//庄家用户

		//用户状态
		protected var m_cbSparrowData:uint;//出牌麻将
		protected var m_wOutSparrowUser:uint;//出牌用户
		protected var m_cbUserAction:Array=new Array(4);//用户动作

		//位置变量
		protected var m_UserFlagPos:Array=new Array(4);//标志位置

		//位图变量
		protected var m_ImageWait:SkinImage;//等待提示
		protected var m_ImageOutSparrow:SkinImage;//出牌提示
		protected var m_ImageUserFlag:SkinImage;//用户标志
		protected var m_ImageUserAction:SkinImage;//用户动作
		protected var m_ImageActionBack:SkinImage;//动作背景
		protected var m_ImageHuangZhuang:SkinImage;//荒庄标志
		protected var m_ImageTitle:SkinImage;//标题位图

		//麻将控件
		public var m_HeapSparrow:Array=new Array(4);//堆立麻将
		public var m_UserSparrow:Array=new Array(3);//用户麻将
		public var m_TableSparrow:Array=new Array(4);//桌面麻将
		public var m_WeaveSparrow:Array=new Array(4);//组合麻将
		public var m_DiscardSparrow:Array=new Array(4);//丢弃麻将
		public var m_HandSparrowControl:SparrowControl;//手上麻将

		//控件变量
		public var m_ControlMC:ControlMC;//控制窗口
		public var m_ScoreView:ScoreView;//结束窗口

		private var m_SparrowResource:SparrowResource;
		
		public function GameView(prarentClass:GameClass)
		{
			m_nXBorder = 3;
			m_nYBorder = 3;
			m_nXFace = 52;
			m_nYFace = 52;
			m_nXTimer= 52;
			m_nYTimer= 52;
			super(prarentClass);
			m_bWindowDrawMode = true;
		}
		///////////////////////////////////////////////////
		override public  function InitGameView():Boolean
		{
			if (super.InitGameView() == false)
			{
				return false;
			}
			
			//加载位图
			m_SparrowResource = new SparrowResource;
			m_SparrowResource.NewResource();
			//用户麻将
			m_SparrowResource.m_ImageUserTop.Load(IMAGE_SPARROW_USER_TOP,26,37);
			m_SparrowResource.m_ImageUserLeft.Load(IMAGE_SPARROW_USER_LEFT,19,49);
			m_SparrowResource.m_ImageUserRight.Load(IMAGE_SPARROW_USER_RIGHT,19,49);
			m_SparrowResource.m_ImageUserBottom.LoadResource(IMAGE_SPARROW_USER_BOTTOM,
															 1806,
															 60,
															 42,
															 60);

			//桌子麻将
			m_SparrowResource.m_ImageTableTop.LoadResource(IMAGE_SPARROW_TABLE_TOP,
														   1118,36,
														   26,25);
			m_SparrowResource.m_ImageTableLeft.LoadResource(IMAGE_SPARROW_TABLE_LEFT,
															1419,32,
															33,21);
			m_SparrowResource.m_ImageTableRight.LoadResource(IMAGE_SPARROW_TABLE_RIGHT,
															 1419,32,
															 33,21);
			m_SparrowResource.m_ImageTableBottom.LoadResource(IMAGE_SPARROW_TABLE_BOTTOM,
															  1118,36,
															  26,25);

			m_SparrowResource.m_ImageBackH.Load(IMAGE_SPARROW_BACK_H,26,36);
			m_SparrowResource.m_ImageBackV.Load(IMAGE_SPARROW_BACK_V,33,32);
			m_SparrowResource.m_ImageHeapSingleV.Load(IMAGE_SPARROW_HEAP_SINGLE_V,22,23);
			m_SparrowResource.m_ImageHeapSingleH.Load(IMAGE_SPARROW_HEAP_SINGLE_H,17,26);
			m_SparrowResource.m_ImageHeapDoubleV.Load(IMAGE_SPARROW_HEAP_DOUBLE_V,22,34);
			m_SparrowResource.m_ImageHeapDoubleH.Load(IMAGE_SPARROW_HEAP_DOUBLE_H,17,37);
			
			m_ImageWait 	  = new SkinImage(IMAGE_WAIT_TIP);
			m_ImageUserFlag   = new SkinImage(IMAGE_USER_FLAG);
			m_ImageOutSparrow = new SkinImage(IMAGE_OUT_SPARROW_TIP);
			m_ImageUserAction = new SkinImage(IMAGE_USER_ACTION);
			m_ImageActionBack = new SkinImage(IMAGE_ACTION_BACK);
			m_ImageHuangZhuang = new SkinImage(IMAGE_HUANG_ZHUANG);
			m_ImageTitle = new SkinImage(IMAGE_TITLE);
			
			for(var i:uint = 0; i < m_HeapSparrow.length; i ++)
			{
				m_HeapSparrow[i] = new HeapSparrow(m_SparrowResource);
				
			}
			for(i = 0; i < m_UserSparrow.length; i ++)
			{
				m_UserSparrow[i] = new UserSparrow(m_SparrowResource);
				
			}
			for(i = 0; i < m_TableSparrow.length; i ++)
			{
				m_TableSparrow[i] = new TableSparrow(m_SparrowResource);
				
			}
			for(i = 0; i < m_WeaveSparrow.length; i ++)
			{
				m_WeaveSparrow[i] = new Array(4);
				for(var j:uint = 0; j < m_WeaveSparrow[i].length; j ++)
				{
					m_WeaveSparrow[i][j] = new WeaveSparrow(m_SparrowResource);
					
				}
			}
			for(i = 0; i < m_DiscardSparrow.length; i ++)
			{
				m_DiscardSparrow[i] = new DiscardSparrow(m_SparrowResource);
				
			}
			m_HandSparrowControl = new SparrowControl(m_SparrowResource);
			m_ScoreView = new ScoreView(m_SparrowResource);
			addChild(m_ScoreView);
			
			for(i = 0; i < m_UserFlagPos.length; i ++)
			{
				m_UserFlagPos[i] = new Point;
			}
			
			
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			
			
			//创建控件
			m_ControlMC = new ControlMC(m_SparrowResource);
			addChild(m_ControlMC);
			m_ControlMC.addEventListener(GameViewEvent.strGameViewEvent, OnEventControlMC);
			
			Ins_BtnExit.addEventListener(MouseEvent.CLICK, OnEventExit);

			this.addEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
			this.addEventListener(MouseEvent.CLICK, OnEventClick);
			
			//标志变量
			m_bOutSparrow=false;
			m_bWaitOther=false;
			m_bHuangZhuang=false;

			//游戏属性
			m_fCellScore=0;
			m_wBankerUser=INVALID_CHAIR;

			//用户状态
			m_cbSparrowData=0;
			m_wOutSparrowUser=INVALID_CHAIR;
			ZeroArray(m_cbUserAction);

			//变量定义
			var Direction:Array=new Array(enDirection.Direction_North,enDirection.Direction_East,enDirection.Direction_South,enDirection.Direction_West);

			//设置控件
			for (var i:uint=0; i < 4; i++)
			{
				//用户麻将
				m_HeapSparrow[i].SetDirection(Direction[i]);
				m_TableSparrow[i].SetDirection(Direction[i]);
				m_DiscardSparrow[i].SetDirection(Direction[i]);

				//组合麻将
				m_WeaveSparrow[i][0].SetDisplayItem(true);
				m_WeaveSparrow[i][1].SetDisplayItem(true);
				m_WeaveSparrow[i][2].SetDisplayItem(true);
				m_WeaveSparrow[i][3].SetDisplayItem(true);
				m_WeaveSparrow[i][0].SetDirection(Direction[i]);
				m_WeaveSparrow[i][1].SetDirection(Direction[i]);
				m_WeaveSparrow[i][2].SetDirection(Direction[i]);
				m_WeaveSparrow[i][3].SetDirection(Direction[i]);
			}

			//设置控件
			m_UserSparrow[0].SetDirection(enDirection.Direction_North);
			m_UserSparrow[1].SetDirection(enDirection.Direction_East);
			m_UserSparrow[2].SetDirection(enDirection.Direction_West);

			//界面设置
			m_btStart.visible = false;
			m_ControlMC.ShowWindow(false);
			
			return true;
		}
		override public  function DestroyGameView():void
		{
			if(m_bFreeChildClassViewData == false)
			{
			//标志变量
			m_bOutSparrow=false;
			m_bWaitOther=false;
			m_bHuangZhuang=false;

			//游戏属性
			m_fCellScore=0;
			m_wBankerUser=INVALID_CHAIR;

			//用户状态
			m_cbSparrowData=0;
			m_wOutSparrowUser=INVALID_CHAIR;
			ZeroArray(m_cbUserAction);

			//界面设置
			m_btStart.visible = false;
			m_ControlMC.ShowWindow(false);
			m_ScoreView.ShowWindow(false);

			//麻将设置
			m_UserSparrow[0].SetSparrowData(0,false);
			m_UserSparrow[1].SetSparrowData(0,false);
			m_UserSparrow[2].SetSparrowData(0,false);
			m_HandSparrowControl.SetPositively(false);
			m_HandSparrowControl.SetDisplayItem(false);
			m_HandSparrowControl.SetSparrowData(null,0,0);

			//麻将设置
			for (var i:uint=0; i < GAME_PLAYER; i++)
			{
				m_HeapSparrow[i].SetSparrowData(0,0,0);
				m_TableSparrow[i].SetSparrowData(null,0);
				m_DiscardSparrow[i].SetSparrowData(null,0);
				m_WeaveSparrow[i][0].SetSparrowData(null,0);
				m_WeaveSparrow[i][1].SetSparrowData(null,0);
				m_WeaveSparrow[i][2].SetSparrowData(null,0);
				m_WeaveSparrow[i][3].SetSparrowData(null,0);
			}

			ZeroArray(m_HeapSparrow, null, HeapSparrow.Destroy);
			m_HeapSparrow = null;
			ZeroArray(m_UserSparrow, null, UserSparrow.Destroy);
			m_UserSparrow = null;
			ZeroArray(m_TableSparrow, null, TableSparrow.Destroy);
			m_TableSparrow = null;
			FreeTwoDimensionArray(m_WeaveSparrow, null, WeaveSparrow.Destroy);
			m_WeaveSparrow = null;
			ZeroArray(m_DiscardSparrow, null, DiscardSparrow.Destroy);
			m_DiscardSparrow = null;
			SparrowControl.Destroy(m_HandSparrowControl);
			m_HandSparrowControl = null;
			m_ScoreView.Destroy();
			removeChild(m_ScoreView);
			m_ScoreView = null;
			
			ZeroArray(m_UserFlagPos, null);
			m_UserFlagPos = null;

			m_btStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
			
			removeChild(m_ControlMC);
			m_ControlMC.removeEventListener(GameViewEvent.strGameViewEvent, OnEventControlMC);
			m_ControlMC = null;
			
			Ins_BtnExit.removeEventListener(MouseEvent.CLICK, OnEventExit);

			this.removeEventListener(MouseEvent.MOUSE_MOVE, OnEventMouseMove);
			this.removeEventListener(MouseEvent.CLICK, OnEventClick);
			
			m_SparrowResource.UnloadResource();
			m_SparrowResource = null;
			
			m_ImageWait.Unload();
			m_ImageWait = null;
			m_ImageUserFlag.Unload();
			m_ImageUserFlag = null;
			m_ImageOutSparrow.Unload();
			m_ImageOutSparrow = null;
			m_ImageUserAction.Unload();
			m_ImageUserAction = null;
			m_ImageActionBack.Unload();
			m_ImageActionBack = null;
			m_ImageHuangZhuang.Unload();
			m_ImageHuangZhuang = null;
			m_ImageTitle.Unload();
			m_ImageTitle = null;
			
			m_bFreeChildClassViewData = true;
		  }
		  super.DestroyGameView();
		}
		
		override protected function CreateDrawBitmapData():BitmapData
		{
			return new SparrowBG(1024, 768);
		}
		override protected  function CreateUserFaceView(wChairID:uint,pUserItem:tagUserInfoHead):IFaceView
		{
			return null;
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
		override protected function  DrawGameView(g:Graphics,pDC:BitmapData):void
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
			
			//用户标志
			if (m_wBankerUser!=INVALID_CHAIR)
			{
				//加载位图
				var nImageWidth:Number=m_ImageUserFlag.GetWidth()/4;
				var nImageHeight:Number=m_ImageUserFlag.GetHeight();
	
				//绘画标志
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					var wIndex:uint=((i+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
					m_ImageUserFlag.DrawBitmapData(pDC,
										   m_UserFlagPos[i].x,
										   m_UserFlagPos[i].y,
										   nImageWidth*wIndex,
										   0,
										   nImageWidth,
										   nImageHeight);
				}
			}
			//桌面麻将
			for (var i:uint=0;i<4;i++)
			{
				m_TableSparrow[i].DrawSparrowControl(pDC);
				m_DiscardSparrow[i].DrawSparrowControl(pDC);
				m_WeaveSparrow[i][0].DrawSparrowControl(pDC);
				m_WeaveSparrow[i][1].DrawSparrowControl(pDC);
				m_WeaveSparrow[i][2].DrawSparrowControl(pDC);
				m_WeaveSparrow[i][3].DrawSparrowControl(pDC);
			}

			//堆积麻将
			m_HeapSparrow[3].DrawSparrowControl(pDC);
			m_HeapSparrow[0].DrawSparrowControl(pDC);
			m_HeapSparrow[1].DrawSparrowControl(pDC);
			m_HeapSparrow[2].DrawSparrowControl(pDC);

			//用户麻将
			m_UserSparrow[0].DrawSparrowControl(pDC);
			m_UserSparrow[1].DrawSparrowControl(pDC);
			m_UserSparrow[2].DrawSparrowControl(pDC);
			m_HandSparrowControl.DrawSparrowControl(pDC);
			
			//出牌提示
			if (m_bOutSparrow==true)
			{
				m_ImageOutSparrow.DrawBitmapData(pDC,
											  (nWidth-m_ImageOutSparrow.GetWidth())/2,
											  nHeight-160);
			}

			//等待提示
			if (m_bWaitOther==true)
			{
				m_ImageWait.DrawBitmapData(pDC,
										   (nWidth-m_ImageWait.GetWidth())/2,
										nHeight-160);
			}

			//荒庄标志
			if (m_bHuangZhuang==true)
			{
				m_ImageHuangZhuang.DrawBitmapData(pDC,
												  (nWidth-m_ImageHuangZhuang.GetWidth())/2,
												  nHeight/2-103);
			}
			//用户状态
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				if ((m_wOutSparrowUser==i)||(m_cbUserAction[i]!=0))
				{
					//计算位置
					var nXPos:Number=0;
					var nYPos:Number=0;
					switch (i)
					{
						case 0:	//北向
						{
							nXPos=nWidth/2-32;
							nYPos=m_nYBorder+105;
							break;
						}
						case 1:	//东向
						{
							nXPos=nWidth-m_nXBorder-182;
							nYPos=nHeight/2-80;
							break;
						}
						case 2:	//南向
						{
							nXPos=nWidth/2-32;
							nYPos=nHeight-m_nYBorder-220;
							break;
						}
						case 3:	//西向
						{
							nXPos=m_nXBorder+115;
							nYPos=nHeight/2-80;
							break;
						}
					}

					//动作背景
					m_ImageActionBack.DrawBitmapData(pDC,
													 nXPos,nYPos,
													 i*m_ImageActionBack.GetWidth()/4,0,
													 m_ImageActionBack.GetWidth()/4,
													 m_ImageActionBack.GetHeight());

					//绘画动作
					if (m_cbUserAction[i]!=WIK_NULL)
					{
						//变量定义
						var nXImagePos:Number=0;
		
						var nItemWidth:Number=m_ImageUserAction.GetWidth()/7;

						//计算位置
						if (m_cbUserAction[i]&WIK_PENG) nXImagePos=nItemWidth;
						else if (m_cbUserAction[i]&WIK_FILL) nXImagePos=nItemWidth*2;
						else if (m_cbUserAction[i]&WIK_GANG) nXImagePos=nItemWidth*3;
						else if (m_cbUserAction[i]&WIK_CHI_HU) nXImagePos=nItemWidth*4;

						//绘画动作
						var nItemHeight:Number=m_ImageUserAction.GetHeight();
						m_ImageUserAction.DrawBitmapData(pDC,
														 nXPos+13,nYPos+15,
														 nXImagePos,0,
														 nItemWidth,nItemHeight);
					}
					else
					{
						//绘画麻将
						m_SparrowResource.m_ImageTableBottom.DrawSparrowItem(pDC,m_cbSparrowData,nXPos+18,nYPos+15);
					}
				}
			}
		}
		////////////////////////////////////
		//调整控件
		override public function RectifyGameView(nWidth:int,nHeight:int):void
		{
			var nXCenter:int=nWidth / 2;
			var nYCenter:int=nHeight / 2 - 20;
			//设置坐标
			m_ptFace[0].x=nWidth / 2 - m_nXFace/2;
			m_ptFace[0].y=m_nYBorder+ 5;
			m_ptName[0].x=m_ptFace[0].x;
			m_ptName[0].y=m_ptFace[0].y;
			m_ptTimer[0].x=m_ptFace[0].x  - m_nXFace;
			m_ptTimer[0].y=m_ptFace[0].y;
			m_ptReady[0].x=nWidth / 2 -91/2;
			m_ptReady[0].y=m_nYBorder+ 5 + m_nYFace + 60;
			m_UserFlagPos[0].x=nWidth / 2 + 25;
			m_UserFlagPos[0].y=m_nYBorder+ 5 + 13;

			m_ptFace[1].x=nWidth - m_nXBorder - 15 - m_nXFace;
			m_ptFace[1].y=nYCenter - 50;
			m_ptName[1].x=m_ptFace[1].x;
			m_ptName[1].y=m_ptFace[1].y;
			m_ptTimer[1].x=m_ptFace[1].x;
			m_ptTimer[1].y=m_ptFace[1].y + m_nYTimer;
			m_ptReady[1].x=m_ptFace[1].x -91 - m_nXFace + 8;
			m_ptReady[1].y=nYCenter - 50;
			m_UserFlagPos[1].x=m_ptFace[1].x + 15;
			m_UserFlagPos[1].y=nYCenter - 50 - 20;

			m_ptFace[2].x=nWidth / 2 - m_nXFace/2;
			m_ptFace[2].y=nHeight - m_nYBorder - m_nYFace - 5;
			m_ptName[2].x=m_ptFace[2].x;
			m_ptName[2].y=m_ptFace[2].y;
			m_ptTimer[2].x=m_ptFace[2].x  - m_nXFace;
			m_ptTimer[2].y=m_ptFace[2].y;
			m_ptReady[2].x=nWidth / 2 -91/2;
			m_ptReady[2].y=nHeight - m_nYBorder - m_nYFace - 170;
			m_UserFlagPos[2].x=nWidth / 2 + 25;
			m_UserFlagPos[2].y=m_ptFace[2].y + 18;

			m_ptFace[3].x=m_nXBorder + 15;
			m_ptFace[3].y=nYCenter - 50;
			m_ptName[3].x=m_nXBorder + 15;
			m_ptName[3].y=nYCenter - 50;
			m_ptTimer[3].x=m_nXBorder + 15;
			m_ptTimer[3].y=nYCenter - 50 + m_nYTimer;
			m_ptReady[3].x=m_ptFace[3].x + m_nXFace + 48;
			m_ptReady[3].y=nYCenter - 50;
			m_UserFlagPos[3].x=m_nXBorder + 30;
			m_UserFlagPos[3].y=nYCenter - 50 - 20;

			//用户麻将
			m_UserSparrow[0].SetControlPoint(nWidth / 2 - 200,m_nYBorder + m_nYFace + 10);
			m_UserSparrow[1].SetControlPoint(nWidth - m_nXBorder - m_nXFace - 50,nHeight / 2 - 255);
			m_UserSparrow[2].SetControlPoint(m_nXBorder + m_nXFace + 30,nHeight / 2 + 177);
			m_HandSparrowControl.SetBenchmarkPos(nWidth / 2,nHeight - m_nYFace - m_nYBorder - 15,enXCollocateMode.enXCenter,enYCollocateMode.enYBottom);

			//桌面麻将
			m_TableSparrow[0].SetControlPoint(nWidth / 2 - 179,m_nYBorder + m_nYFace + 10);
			m_TableSparrow[1].SetControlPoint(nWidth - m_nXBorder - m_nXFace - 30,nHeight / 2 - 197);
			m_TableSparrow[2].SetControlPoint(nWidth / 2 + 185,nHeight - m_nYFace - m_nYBorder - 15);
			m_TableSparrow[3].SetControlPoint(m_nXBorder + m_nXFace + 30,nHeight / 2 + 97);

			//堆积麻将
			m_HeapSparrow[0].SetControlPoint(nXCenter - 98 - 4,nYCenter - 108);
			m_HeapSparrow[1].SetControlPoint(nXCenter + 101- 4,nYCenter - 68);
			m_HeapSparrow[2].SetControlPoint(nXCenter - 192- 4,nYCenter + 73);
			m_HeapSparrow[3].SetControlPoint(nXCenter - 123- 4,nYCenter - 172);

			//丢弃麻将
			m_DiscardSparrow[0].SetControlPoint(nXCenter - 98- 5,nYCenter - 110 - 15);
			m_DiscardSparrow[1].SetControlPoint(nXCenter + 125- 5 + 15,nYCenter - 68);
			m_DiscardSparrow[2].SetControlPoint(nXCenter + 71- 5,nYCenter + 112 + 20);
			m_DiscardSparrow[3].SetControlPoint(nXCenter - 126- 5 -15,nYCenter + 59);

			//组合麻将
			m_WeaveSparrow[0][0].SetControlPoint(nWidth / 2 + 145,m_nYBorder + m_nYFace + 62);
			m_WeaveSparrow[0][1].SetControlPoint(nWidth / 2 + 55,m_nYBorder + m_nYFace + 62);
			m_WeaveSparrow[0][2].SetControlPoint(nWidth / 2 - 35,m_nYBorder + m_nYFace + 62);
			m_WeaveSparrow[0][3].SetControlPoint(nWidth / 2 - 125,m_nYBorder + m_nYFace + 62);

			//组合麻将
			m_WeaveSparrow[1][0].SetControlPoint(nWidth - m_nXBorder - m_nXFace - 84,nHeight / 2 + 100);
			m_WeaveSparrow[1][1].SetControlPoint(nWidth - m_nXBorder - m_nXFace - 84,nHeight / 2 + 20);
			m_WeaveSparrow[1][2].SetControlPoint(nWidth - m_nXBorder - m_nXFace - 84,nHeight / 2 - 60);
			m_WeaveSparrow[1][3].SetControlPoint(nWidth - m_nXBorder - m_nXFace - 84,nHeight / 2 - 140);

			//组合麻将
			m_WeaveSparrow[2][0].SetControlPoint(nWidth / 2 - 190,nHeight - m_nYFace - m_nYBorder - 68);
			m_WeaveSparrow[2][1].SetControlPoint(nWidth / 2 - 100,nHeight - m_nYFace - m_nYBorder - 68);
			m_WeaveSparrow[2][2].SetControlPoint(nWidth / 2 - 10,nHeight - m_nYFace - m_nYBorder - 68);
			m_WeaveSparrow[2][3].SetControlPoint(nWidth / 2 + 80,nHeight - m_nYFace - m_nYBorder - 68);

			//组合麻将
			m_WeaveSparrow[3][0].SetControlPoint(m_nXBorder + m_nXFace + 82,nHeight / 2 - 200);
			m_WeaveSparrow[3][1].SetControlPoint(m_nXBorder + m_nXFace + 82,nHeight / 2 - 120);
			m_WeaveSparrow[3][2].SetControlPoint(m_nXBorder + m_nXFace + 82,nHeight / 2 - 40);
			m_WeaveSparrow[3][3].SetControlPoint(m_nXBorder + m_nXFace + 82,nHeight / 2 + 40);

			//控制窗口
			m_ControlMC.SetBenchmarkPos(nWidth-22,nHeight - m_nYBorder - 150);

			//调整成绩
			m_ScoreView.MoveWindow((nWidth-m_ScoreView.Width())/2,
									(nHeight-m_ScoreView.Height())/2-30);

			//移动按钮
			m_btStart.x = (nWidth-60)/2;
			m_btStart.y = nHeight-142-m_nYBorder;

			m_ptNetSpeedView.x = 38;
			m_ptNetSpeedView.y = 696;
			m_ptUserInfoView.x = 735;
			m_ptUserInfoView.y = 8;
			
			super.RectifyGameView(nWidth, nHeight);
		}
		/////////////////
		//基础积分
		public function SetCellScore(fCellScore:Number):void
		{
			//设置麻将
			if (fCellScore != m_fCellScore)
			{
				//设置变量
				m_fCellScore=fCellScore;

				//更新界面
				UpdateGameView();
			}
			return;
		}
		//海底麻将
		public function SetHuangZhuang(bHuangZhuang:Boolean):void
		{
			//设置麻将
			if (bHuangZhuang != m_bHuangZhuang)
			{
				//设置变量
				m_bHuangZhuang=bHuangZhuang;

				//更新界面
				UpdateGameView();
			}
			return;
		}
		//庄家用户
		public function SetBankerUser(wBankerUser:uint):void
		{
			//设置用户
			if (wBankerUser != m_wBankerUser)
			{
				//设置变量
				m_wBankerUser=wBankerUser;

				//更新界面
				UpdateGameView();
			}
			return;
		}
		//状态标志
		public function SetStatusFlag(bOutSparrow:Boolean,bWaitOther:Boolean):void
		{
			//设置变量
			m_bOutSparrow=bOutSparrow;
			m_bWaitOther=bWaitOther;

			//更新界面
			UpdateGameView();

			return;
		}
		//出牌信息
		public function SetOutSparrowInfo(wViewChairID:uint,cbSparrowData:uint):void
		{
			//设置变量
			m_cbSparrowData=cbSparrowData;
			m_wOutSparrowUser=wViewChairID;

			//更新界面
			UpdateGameView();

			return;
		}
		//动作信息
		public function SetUserAction(wViewChairID:uint,bUserAction:uint):void
		{
			//设置变量
			if (wViewChairID < GAME_PLAYER)
			{
				m_cbUserAction[wViewChairID]=bUserAction;
			}
			else
			{
				ZeroArray(m_cbUserAction);

			}//更新界面
			UpdateGameView();

			return;
		}
		////////////////////////////////////////////////////
		private function OnEventStart(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_START));
		}
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
		}
		private function OnEventMouseMove(e:MouseEvent):void
		{
			var MousePoint:Point = new Point(mouseX, mouseY);
			//点击测试
			
			var r:Array=m_HandSparrowControl.OnEventMouse(MousePoint);
			var bRePaint:Boolean = r[0];
			var bHandle:Boolean = r[1];
			r = null;
			
			//重画控制
			if (bRePaint==true) UpdateGameView();
			
			MousePoint = null;
		}
		private function OnEventClick(e:MouseEvent):void
		{
				//获取麻将
			var cbHoverSparrow:uint=m_HandSparrowControl.GetHoverSparrow();
			if (cbHoverSparrow!=0)
			{
				dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_OUT_SPARROW,
												cbHoverSparrow,
												cbHoverSparrow));
			}

		}
		private function OnEventControlMC(e:GameViewEvent):void
		{
			dispatchEvent(e.cloneEvent());
		}
	}
}
include "./Common/GameLogicDef.as"
include "./Common/CMD_Sparrow.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./StringTableDef.as"
include "../../Common/Memory.as"