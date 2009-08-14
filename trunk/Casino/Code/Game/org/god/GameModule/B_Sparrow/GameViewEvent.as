package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;

	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String="GameViewEvent_Sparrow";
		public static  const nGVM_START:int=0xff + 100;
		public static  const nGVM_OUT_SPARROW:int=0xff + 101;
		public static  const nGVM_SPARROW_OPERATE:int=0xff + 102;


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