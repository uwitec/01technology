package org.god.GameModule.Common.SlotWheel
{
	import flash.events.Event;
	
	public class ScrollEngineEvent extends Event
	{
		public static const MOTION_CHANGE : String = "motionChange";
   		public static const MOTION_FINISH : String = "motionFinish";
    	public static const MOTION_LOOP : String = "motionLoop";
    	public static const MOTION_RESUME : String = "motionResume";
    	public static const MOTION_START : String = "motionStart";
    	public static const MOTION_STOP : String = "motionStop" ;

		public var position:Number = NaN;
		public var time:Number = NaN;
		
		public function ScrollEngineEvent(type:String, time:Number, position:Number, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.time = time;
			this.position = position;
		}
		override public function clone():Event
		{
			return new ScrollEngineEvent(this.type, this.time, this.position, this.bubbles, this.cancelable);
		} 
	}
}