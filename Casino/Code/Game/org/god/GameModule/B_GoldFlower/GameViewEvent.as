package 
{
	import org.god.GameModule.Common.BaseGameViewEvent;
	//游戏视图事件
	public class GameViewEvent extends BaseGameViewEvent
	{
		public static  const strGameViewEvent:String = "GameViewEvent_GoldFlower";

		public static  const nGVM_START:int =0xff+100;//开始消息
		public static  const nGVM_ADD_GOLD:int =0xff+101;//加注消息
		public static  const nGVM_FOLLOW:int =0xff+102;//跟注消息
		public static  const nGVM_GIVE_UP:int =0xff+103;//放弃消息
		public static  const nGVM_GOLDFLOWER:int =0xff+104;//梭哈消息
		public static  const nGVM_KAIPAI :int =0xff+105;//开牌消息
		public static  const nGVM_KANPAI:int =  0xff+107;//看牌消息  add
		public static  const nGVM_ONFOLLOW:int = 0xff+108;
		//构造函数
		public function GameViewEvent(nMsg:int=0,nWParam:*=0,nLParam:*=0):void
		{
			super(strGameViewEvent,nMsg,nWParam,nLParam);
		}
		//拷贝
		public function cloneEvent():GameViewEvent
		{
			return new GameViewEvent(this.m_nMsg,this.m_nWParam,this.m_nLParam);
		}
	}
}