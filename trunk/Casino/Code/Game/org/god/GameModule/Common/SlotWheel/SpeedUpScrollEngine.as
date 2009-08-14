package org.god.GameModule.Common.SlotWheel
{
	import fl.transitions.easing.Regular;
	
	public class SpeedUpScrollEngine extends BaseScrollEngine
	{
		protected var m_BaseScrollMotion:BaseScrollMotion;
		
		public function SpeedUpScrollEngine(motion:BaseScrollMotion,
											obj:*,
											 begin:Number, 
							  finish:Number, 
							  duration:Number)
		{
			super(obj,Regular.easeIn,begin,finish,duration);
			m_BaseScrollMotion = motion;
		}
		public override function Destroy():void
		{
			m_BaseScrollMotion = null;
			super.Destroy();
		}
		protected override function OnScrollLoop(p:Number):void
		{
			if(m_BaseScrollMotion)
				m_BaseScrollMotion.OnScrollLoop(this,p);
		}
	}
}