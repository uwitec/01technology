package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;

	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String = "GameViewEvent_Upgrade";
		//按钮消息
		public static  const nGVM_START = 0xff +100;//开始消息
		public static  const nGVM_OUT_CARD = 0xff +101;//出牌消息
		public static  const nGVM_OUT_PROMPT = 0xff +102;//提示消息
		public static  const nGVM_SEND_CONCEAL = 0xff +103;//留底消息

		//功能按钮
		public static  const nGVM_CONCEAL_CARD = 0xff +110;//查看底牌
		public static  const nGVM_REQUEST_LEAVE = 0xff +111;//请求离开
		public static  const nGVM_LAST_TURN_CARD = 0xff +112;//上轮扑克
		public static  const nGVM_TRUSTEE_CONTROL = 0xff +113;//托管控制
		
		public static  const nGVM_LEFT_HIT_CARD = 0xff +400;//
		public static  const nGVM_CALL_CARD:int = 0xff + 500;//

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