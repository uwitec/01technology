package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;

	public class GameViewEvent extends BaseGameViewEvent
	{
		public static const strGameViewEvent:String = "GameViewEvent_Land";
		public static const nGVM_START:int 		= 0xff + 100;
		public static const nGVM_OUT_CARD:int 	= 0xff + 200;
		public static const nGVM_PASS_CARD:int	= 0xff + 300;
		public static const nGVM_LAND_SCORE:int   = 0xff + 400;
		public static const nGVM_AUTO_OUTCARD:int = 0xff + 500;
		public static const nGVM_LEFT_HIT_CARD:int = 0xff + 600;
		public static const nGVM_RIGHT_HIT_CARD:int = 0xff + 700;
		public static const nGVM_TRUSTEE_CONTROL:uint=0xff + 800;
		public static const nGVM_LAST_TURN_CARD:uint=0xff + 900;
		public static const nGVM_DOUBLETIME:uint=0xff + 1000;
		public static const nGVM_GIVEUPDOUBLETIME:uint=0xff + 1100;
		public static const nGVM_MORE_TIME:uint = 0xff + 1200;
		
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