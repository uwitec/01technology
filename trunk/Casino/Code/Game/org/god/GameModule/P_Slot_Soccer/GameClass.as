package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.IGameClass;
	import org.god.GameModule.Common.GameBaseClass;
	import org.god.GameModule.Common.GameBaseView;
	import org.god.GameModule.P_BigGameMachine_Fruit.Common.*;
	//游戏类
	public class GameClass extends GameBaseClass
	{
		public function GameClass()
		{
			super();
		}
		//查询游戏接口
		public function QueryIGameClass():IGameClass
		{
			return this  as  GameBaseClass  as  IGameClass;
		}
		//获取游戏视图
		protected function GetPeerGameView():GameView
		{
			if (m_GameClientView == null)
			{
				throw Error("GetPeerGameView m_GameClientView == null");
				return null;
			}
			return m_GameClientView  as  GameView;
		}
		//创建游戏视图
		override protected  function CreateGameView():GameBaseView
		{
			return new GameView(this);
		}
		//初始化
		override public  function InitGameClass():Boolean
		{
			if (super.InitGameClass() == false)
			{
				return false;
			}
		
			return true;
		}
		//重置
		override public  function DestroyGameClass():void
		{
			super.DestroyGameClass();
		}
		//内核事件
		//时间消息
		override protected function OnEventTimer(wChairID:uint,nElapse:uint,nTimerID:uint):Boolean
		{
			return false;
		}
		//网络消息
		override protected function OnGameMessage(wSubCmdID:uint,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			return true;
		}
		//游戏场景
		override protected function OnGameSceneMessage(cbGameStation:uint,bLookonOther:Boolean,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			switch(cbGameStation)
			{
				case GS_FREE:		//空闲状态
				{
					return true;
				}
			}
			return false;
		}
		//游戏开始
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			return 0
		}
	}
}
include "./Common/CMD_Commonality.as"
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
