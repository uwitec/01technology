package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;
	//游戏视图事件
	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String = "GameViewEvent_BlackJack";
		public static  const nGVM_GAME_START:int = 0xff + 101;//开始按钮
		public static  const nGVM_ADD_GOLD:int = 0xff +102;//加注按钮
		public static  const nGVM_GIVE_UP:int = 0xff +103;//放弃按钮
		public static  const nGVM_GET_CARD:int = 0xff +104;//要牌按钮
		public static  const nGVM_STOP_CARD:int = 0xff +    105;//停牌按钮
		public static  const nGVM_DOUBLE_CARD:int = 0xff +    106;//双倍按钮
		public static  const nGVM_CUT_CARD:int = 0xff +    107;//分牌按钮

		public function GameViewEvent(nMsg:int=0,nWParam:*=0,nLParam:*=0):void
		{
			super(strGameViewEvent,nMsg,nWParam,nLParam);

		}
		public function cloneEvent():GameViewEvent
		{
			return new GameViewEvent(this.m_nMsg,this.m_nWParam,this.m_nLParam);
		}
	}
}