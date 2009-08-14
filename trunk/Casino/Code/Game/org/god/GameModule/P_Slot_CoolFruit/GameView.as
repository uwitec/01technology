package 
{
	import flash.display.MovieClip;
	import flash.display.Graphics;
	import flash.display.BitmapData;
	import flash.events.MouseEvent;
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.utils.Timer;

	import org.god.IGameFrame.tagServerAttribute;
	import org.god.SkinControl.ISkinImage;
	import org.god.SkinControl.SkinImage;
	import org.god.SkinControl.ImageButton;
	import org.god.GameModule.Common.GameBaseView;
	import org.god.GameModule.Common.NumberText;
	import org.god.GameModule.P_BigGameMachine_Fruit.Common.*;
	import org.god.SkinControl.PGoldTextField;

	//游戏视图
	public class GameView extends GameBaseView
	{
		//控件变量
		public var m_btExit:ImageButton;//离开按钮
		public var m_btStart:ImageButton;//开始按钮
		public var m_btAutoStart:ImageButton;//自动开始按钮
		public var m_btStop:ImageButton;//停止按钮
		public var m_btBetOne:ImageButton;//一倍注按钮
		public var m_btBetThree:ImageButton;//三倍注按钮
		public var m_btTopStart:ImageButton;//开始按钮
		public var m_btTopAutoStart:ImageButton;//自动开始按钮
		public var m_btTopStop:ImageButton;//停止按钮
		public var m_btTopBetOne:ImageButton;//一倍注按钮
		public var m_btTopBetThree:ImageButton;//三倍注按钮	
		public var m_GameSlotWheelView:GameSlotWheelView;//转动盘
		public var m_ntBetTime:NumberText;//倍注文本
		public var m_ntMinBet:NumberText;//最小下注文本
		public var m_labelUserName:PGoldTextField;//会员名称
		public var m_labelUserScore:PGoldTextField;//信用额度
		public var m_labelTotalBetScore:PGoldTextField;//总投
		public var m_labelTotalWinScore:PGoldTextField;//总派彩
		public var m_labelCellBetScore:PGoldTextField;//注额
		public var m_labelMaxBetScore:PGoldTextField;//最大投注
		public var m_labelOneMaxBetScore:PGoldTextField;//单项限注
		public var m_labelMinBetScore:PGoldTextField;//最小投注
		
		//游戏变量
		public var m_bAutoStartFlag:Boolean;
		public var m_RequstStopScrollGameSlotWheelTimer:Timer;
		public var m_nBetTime:uint;
		public var m_nTotalBetScore:uint;
		public var m_nTotalWinScore:uint;
		
		//其他变量
		public var m_fOldFrameRate:Number;
		
		public function GameView(prarentClass:GameClass)
		{
			super(prarentClass);
		}
		//初始化
		override public function InitGameView():Boolean
		{
			if (super.InitGameView() == false)
			{
				return false;
			}
			m_btExit = new ImageButton(IMAGE_BT_EXIT,400, 29);
			addChild(m_btExit);
			m_btExit.addEventListener(MouseEvent.CLICK, OnEventExit);
			m_btStart = new ImageButton(IMAGE_BT_START,420, 30);
			addChild(m_btStart);
			m_btStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btAutoStart = new ImageButton(IMAGE_BT_AUTOSTART,420, 30);
			addChild(m_btAutoStart);
			m_btAutoStart.addEventListener(MouseEvent.CLICK, OnEventAutoStart);
			m_btStop = new ImageButton(IMAGE_BT_STOP,420, 30);
			addChild(m_btStop);
			m_btStop.addEventListener(MouseEvent.CLICK, OnEventStop);
			m_btBetOne = new ImageButton(IMAGE_BT_BETONE,420, 30);
			addChild(m_btBetOne);
			m_btBetOne.addEventListener(MouseEvent.CLICK, OnEventBetOne);
			m_btBetThree = new ImageButton(IMAGE_BT_BETTHREE,420, 30);
			addChild(m_btBetThree);
			m_btBetThree.addEventListener(MouseEvent.CLICK, OnEventBetThree);
			m_btTopStart = new ImageButton(IMAGE_BT_TOP_START,460, 36);
			addChild(m_btTopStart);
			m_btTopStart.addEventListener(MouseEvent.CLICK, OnEventStart);
			m_btTopAutoStart = new ImageButton(IMAGE_BT_TOP_AUTOSTART,460, 36);
			addChild(m_btTopAutoStart);
			m_btTopAutoStart.addEventListener(MouseEvent.CLICK, OnEventAutoStart);
			m_btTopStop = new ImageButton(IMAGE_BT_TOP_STOP,460, 36);
			addChild(m_btTopStop);
			m_btTopStop.addEventListener(MouseEvent.CLICK, OnEventStop);
			m_btTopBetOne = new ImageButton(IMAGE_BT_TOP_BETONE,460, 36);
			addChild(m_btTopBetOne);
			m_btTopBetOne.addEventListener(MouseEvent.CLICK, OnEventBetOne);
			m_btTopBetThree = new ImageButton(IMAGE_BT_TOP_BETTHREE,460, 36);
			addChild(m_btTopBetThree);
			m_btTopBetThree.addEventListener(MouseEvent.CLICK, OnEventBetThree);

			m_btStop.ShowWindow(false);
			m_btTopStop.ShowWindow(false);
			
			m_fOldFrameRate = this.stage.frameRate;
			this.stage.frameRate = 25;
			
			m_GameSlotWheelView = new GameSlotWheelView;
			addChild(m_GameSlotWheelView);
			m_GameSlotWheelView.AddActionListener_Start(OnEventGameSlotWheelStart);
			m_GameSlotWheelView.AddActionListener_Stop(OnEventGameSlotWheelStop);
			
			m_ntBetTime = new NumberText(MC_ColorNumber);
			addChild(m_ntBetTime);
			m_ntMinBet  = new NumberText(MC_ColorNumber);
			addChild(m_ntMinBet);

			m_labelUserName = new PGoldTextField();
			addChild(m_labelUserName);
			m_labelUserScore = new PGoldTextField();
			addChild(m_labelUserScore);
			m_labelTotalBetScore = new PGoldTextField();
			addChild(m_labelTotalBetScore);
			m_labelTotalWinScore = new PGoldTextField();
			addChild(m_labelTotalWinScore);
			m_labelCellBetScore = new PGoldTextField();
			addChild(m_labelCellBetScore);
			m_labelMaxBetScore = new PGoldTextField();
			addChild(m_labelMaxBetScore);
			m_labelOneMaxBetScore = new PGoldTextField();
			addChild(m_labelOneMaxBetScore);
			m_labelMinBetScore = new PGoldTextField();
			addChild(m_labelMinBetScore);

			m_RequstStopScrollGameSlotWheelTimer = new Timer(4000,1);
			m_RequstStopScrollGameSlotWheelTimer.addEventListener(TimerEvent.TIMER,OnEventRequstStopScrollGameSlotWheel);
	
			m_bAutoStartFlag = false;
			m_nBetTime		 = 1;
			m_ntBetTime.SetValue(m_nBetTime);
			
			m_nTotalBetScore = 0;
			m_nTotalWinScore = 0;
			
			return true;
		}
		//销毁
		override public function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
				m_RequstStopScrollGameSlotWheelTimer.stop();
				m_RequstStopScrollGameSlotWheelTimer.removeEventListener(TimerEvent.TIMER,OnEventRequstStopScrollGameSlotWheel);
				m_RequstStopScrollGameSlotWheelTimer = null;
	
				m_btExit.removeEventListener(MouseEvent.CLICK, OnEventExit);
				m_btExit.Destroy();
				removeChild(m_btExit);
				m_btExit = null;
				
				m_btStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
				m_btStart.Destroy();
				removeChild(m_btStart);
				m_btStart = null;
				m_btAutoStart.removeEventListener(MouseEvent.CLICK, OnEventAutoStart);
				m_btAutoStart.Destroy();
				removeChild(m_btAutoStart);
				m_btAutoStart = null;
				m_btStop.removeEventListener(MouseEvent.CLICK, OnEventStop);
				m_btStop.Destroy();
				removeChild(m_btStop);
				m_btStop = null;
				m_btBetOne.removeEventListener(MouseEvent.CLICK, OnEventBetOne);
				m_btBetOne.Destroy();
				removeChild(m_btBetOne);
				m_btBetOne = null;
				m_btBetThree.removeEventListener(MouseEvent.CLICK, OnEventBetThree);
				m_btBetThree.Destroy();
				removeChild(m_btBetThree);
				m_btBetThree = null;
				m_btTopStart.removeEventListener(MouseEvent.CLICK, OnEventStart);
				m_btTopStart.Destroy();
				removeChild(m_btTopStart);
				m_btTopStart = null;
				m_btTopAutoStart.removeEventListener(MouseEvent.CLICK, OnEventAutoStart);
				m_btTopAutoStart.Destroy();
				removeChild(m_btTopAutoStart);
				m_btTopAutoStart = null;
				m_btTopStop.removeEventListener(MouseEvent.CLICK, OnEventStop);
				m_btTopStop.Destroy();
				removeChild(m_btTopStop);
				m_btTopStop = null;
				m_btTopBetOne.removeEventListener(MouseEvent.CLICK, OnEventBetOne);
				m_btTopBetOne.Destroy();
				removeChild(m_btTopBetOne);
				m_btTopBetOne = null;
				m_btTopBetThree.removeEventListener(MouseEvent.CLICK, OnEventBetThree);
				m_btTopBetThree.Destroy();
				removeChild(m_btTopBetThree);
				m_btTopBetThree = null;
				
				m_GameSlotWheelView.RemoveActionListener_Start(OnEventGameSlotWheelStart);
				m_GameSlotWheelView.RemoveActionListener_Stop(OnEventGameSlotWheelStop);
				removeChild(m_GameSlotWheelView);
				m_GameSlotWheelView.Destroy();
				m_GameSlotWheelView = null;
				
				m_ntBetTime.Destroy();
				removeChild(m_ntBetTime);
				m_ntBetTime = null;
				m_ntMinBet.Destroy();
				removeChild(m_ntMinBet);
				m_ntMinBet = null;
		
				removeChild(m_labelUserName);
				m_labelUserName = null;
				removeChild(m_labelUserScore);
				m_labelUserScore = null;
				removeChild(m_labelTotalBetScore);
				m_labelTotalBetScore = null;
				removeChild(m_labelTotalWinScore);
				m_labelTotalWinScore = null;
				removeChild(m_labelCellBetScore);
				m_labelCellBetScore = null;
				removeChild(m_labelMaxBetScore);
				m_labelMaxBetScore = null;
				removeChild(m_labelOneMaxBetScore);
				m_labelOneMaxBetScore = null;
				removeChild(m_labelMinBetScore);
				m_labelMinBetScore = null;

				this.stage.frameRate = m_fOldFrameRate;
			
				m_bFreeChildClassViewData = true;
			}
			super.DestroyGameView();
		}

		//调整控件
		override public function RectifyGameView( nWidth:int, nHeight:int):void
		{
			m_btExit.MoveWindow(216,724);
			
			m_btStart.MoveWindow(920,662);
			m_btAutoStart.MoveWindow(920 - 82,662);
			m_btStop.MoveWindow(920 - 82,662);
			m_btBetOne.MoveWindow(920 - 82 - 82,662);
			m_btBetThree.MoveWindow(920 - 82 - 82 - 82,662);
			
			m_btTopStart.MoveWindow(582,458);
			m_btTopAutoStart.MoveWindow(582 - 92,458);
			m_btTopStop.MoveWindow(582 - 92,458);
			m_btTopBetOne.MoveWindow(582 - 92 - 92,458);
			m_btTopBetThree.MoveWindow(582 - 92 - 92 - 92,458);
			
			m_ntBetTime.MoveWindow(976,510);
			m_ntMinBet.MoveWindow(976,618);
			
			m_labelUserName.setComBoundsXYWH(102,671,192,16);
			m_labelUserScore.setComBoundsXYWH(102,697,192,16);
			m_labelTotalBetScore.setComBoundsXYWH(369,704,145,16);
			m_labelTotalWinScore.setComBoundsXYWH(572,704,120,16);
			m_labelCellBetScore.setComBoundsXYWH(734,704,94,16);
			m_labelMaxBetScore.setComBoundsXYWH(384,734,132,16);
			m_labelOneMaxBetScore.setComBoundsXYWH(587,734,105,16);
			m_labelMinBetScore.setComBoundsXYWH(763,734,68,16);

		}
		//更新游戏属性
		public override function UpdateServerAttribute(attr:tagServerAttribute):void
		{
			m_ntMinBet.SetValue(m_GameClass.GetServerAttribute().fCellScore);
			m_labelUserName.setText(m_GameClass.GetGlobalMeUserInfo().szName);
			m_labelUserScore.setText(m_GameClass.GetGlobalMeUserInfo().UserScore.fScore.toFixed(2));
			m_labelTotalBetScore.setText(Number(m_nTotalBetScore).toFixed(0));
			m_labelTotalWinScore.setText(Number(m_nTotalWinScore).toFixed(0));
			m_labelCellBetScore.setText(m_GameClass.GetServerAttribute().fCellScore.toFixed(0));
			m_labelMaxBetScore.setText(m_GameClass.GetServerAttribute().fHighScore.toFixed(0));
			m_labelOneMaxBetScore.setText(m_GameClass.GetServerAttribute().fLessScore.toFixed(0));
			m_labelMinBetScore.setText(m_GameClass.GetServerAttribute().fCellScore.toFixed(0));

		}
		//绘画界面
		override protected function  DrawGameView(g:Graphics,pDC:BitmapData):void
		{

		}
		///////////////////
		//离开按钮事件
		private function OnEventExit(e:MouseEvent):void
		{
			m_GameClass.SendEventExitGameClient();
		}
		//开始按钮事件
		private function OnEventStart(e:MouseEvent):void
		{
			StartScrollGameSlotWheel();
		}
		//自动开始按钮事件
		private function OnEventAutoStart(e:MouseEvent):void
		{
			m_btStop.ShowWindow(true);
			m_btTopStop.ShowWindow(true);
			m_btAutoStart.ShowWindow(false);
			m_btTopAutoStart.ShowWindow(false);
			m_bAutoStartFlag = true;
		}
		//停止自动按钮事件
		private function OnEventStop(e:MouseEvent):void
		{
			m_btStop.ShowWindow(false);
			m_btTopStop.ShowWindow(false);
			m_btAutoStart.ShowWindow(true);
			m_btTopAutoStart.ShowWindow(true);
			m_bAutoStartFlag = false;
		}
		//一倍注按钮事件
		private function OnEventBetOne(e:MouseEvent):void
		{
			m_nBetTime		 = 1;
			m_ntBetTime.SetValue(m_nBetTime);
		}
		//三倍注按钮事件
		private function OnEventBetThree(e:MouseEvent):void
		{
			m_nBetTime		 = 3;
			m_ntBetTime.SetValue(m_nBetTime);
		}
		//转动盘开始事件
		private function OnEventGameSlotWheelStart(e:Event):void
		{
			m_btStart.EnableWindow(false);
			m_btBetOne.EnableWindow(false);
			m_btBetThree.EnableWindow(false);
			
			m_btTopStart.EnableWindow(false);
			m_btTopBetOne.EnableWindow(false);
			m_btTopBetThree.EnableWindow(false);
		}
		//转动盘停止事件
		private function OnEventGameSlotWheelStop(e:Event):void
		{
			m_btStart.EnableWindow(true);
			m_btBetOne.EnableWindow(true);
			m_btBetThree.EnableWindow(true);
			
			m_btTopStart.EnableWindow(true);
			m_btTopBetOne.EnableWindow(true);
			m_btTopBetThree.EnableWindow(true);
			
			if(m_bAutoStartFlag)
			{
				StartScrollGameSlotWheel();
			}
		}
		//请求停止转动定时器事件
		private function OnEventRequstStopScrollGameSlotWheel(e:Event):void
		{
			var cbSlotWheelRow:int = 1;
			var cbSlotWheelCol:int = 3;
			var lastSlotItemTypes:Array = new Array(cbSlotWheelRow);

			for(var r:uint = 0; r < cbSlotWheelRow; r++)
			{
				lastSlotItemTypes[r] = new Array(cbSlotWheelCol);
				
				for(var c:uint = 0; c < cbSlotWheelCol; c ++)
				{
					lastSlotItemTypes[r][c] = int(int(Math.random() * 10000)% 8 + 1);
					
				}
			}
			m_GameSlotWheelView.Stop(lastSlotItemTypes);
		}
		//开始转动
		private function StartScrollGameSlotWheel():void
		{
			m_GameSlotWheelView.Start();
			m_RequstStopScrollGameSlotWheelTimer.reset();
			m_RequstStopScrollGameSlotWheelTimer.start();
		}
	}
}
include "./StringTableDef.as"