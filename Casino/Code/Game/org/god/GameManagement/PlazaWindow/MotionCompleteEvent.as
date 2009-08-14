package 
{
	import flash.events.Event;
	//转动结束事件
	public class MotionCompleteEvent extends Event
	{
		public static const strEventName_MotionComplete:String = "MotionComplete";
		
		public var m_nIndex0:uint;
		public var m_nIndex1:int;
		
		public function MotionCompleteEvent(nIndex0:uint,nIndex1:uint)
		{
			super(strEventName_MotionComplete);
			m_nIndex0 = nIndex0;
			m_nIndex1 = nIndex1;
		}
	}
}