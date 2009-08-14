package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;

	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String = "GameViewEvent_ShowHand";
		public static  const nGVM_GAME_START:int = 0xff + 101;//开始按钮
		public static  const nGVM_ADD_GOLD:int = 0xff +102;//加注按钮
		public static  const nGVM_GIVE_UP:int = 0xff +103;//放弃按钮
		public static  const nGVM_FOLLOW:int = 0xff +104;//跟注按钮
		public static  const nGVM_SHOW_HAND:int = 0xff +    105;//梭哈按钮

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