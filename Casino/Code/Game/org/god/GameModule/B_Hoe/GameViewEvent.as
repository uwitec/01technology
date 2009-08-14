package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;

	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String = "GameViewEvent_Hoe";

		public static  const nGVM_START:int = 0xff + 101;//开始按钮
		public static  const nGVM_OUT_CARD:int = 0xff +102;//出牌消息
		public static  const nGVM_PASS_CARD:int = 0xff +103;//放弃出牌
		public static  const nGVM_OUT_PROMPT:int = 0xff +104;//提示出牌
		public static  const nGVM_LEFT_HIT_CARD:int = 0xff + 105;
		
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