package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;

	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String="GameViewEvent_UncoverPig";

		public static  const nGVM_START:int=0xff + 100;//开始消息
		public static  const nGVM_OUT_CARD:int=0xff + 101;//出牌消息
		public static  const nGVM_WAIT_START:int=0xff + 102;//布牌消息
		public static  const nGVM_SHOW_CARD:int=0xff + 103;//亮牌消息
		public static  const nGVM_NOT_SHOW_CARD:int=0xff + 104;//不亮消息
		public static  const nGVM_AUTO_OUT_CARD:int=0xff + 105;//自动代打
		public static  const nGVM_CANCEL_AUTO_OUT_CARD:int=0xff + 106;//取消代打
		
		public static  const nGVM_LEFT_HIT_CARD:int=0xff + 200;

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