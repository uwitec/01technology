package 
{
	import flash.display.MovieClip;
	import flash.display.Graphics;
	import flash.display.BitmapData;
	import flash.events.MouseEvent;

	import org.god.SkinControl.ISkinImage;
	import org.god.SkinControl.SkinImage;
	import org.god.SkinControl.ImageButton;
	import org.god.GameModule.Common.GameBaseView;
	import org.god.GameModule.P_BigGameMachine_Fruit.Common.*;
	
	//游戏视图
	public class GameView extends GameBaseView
	{
		//控件变量
		public var m_btExit:ImageButton;//离开按钮
		
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
			
			return true;
		}
		//销毁
		override public function DestroyGameView():void
		{
			if (m_bFreeChildClassViewData == false)
			{
				m_btExit.Destroy();
				removeChild(m_btExit);
				m_btExit = null;
				
				m_bFreeChildClassViewData = true;
			}
			super.DestroyGameView();
		}

		//调整控件
		override public function RectifyGameView( nWidth:int, nHeight:int):void
		{
			m_btExit.MoveWindow(216,724);
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
	}
}
include "./StringTableDef.as"