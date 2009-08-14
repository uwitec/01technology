package org.god.GameModule.Common.SlotWheel
{
	import fl.transitions.easing.None;
	
	public class SpeedMaxScrollEngine extends BaseScrollEngine
	{
		protected var m_BaseScrollMotion:BaseScrollMotion;
		
		public function SpeedMaxScrollEngine(motion:BaseScrollMotion,
											 obj:*,
											 begin:Number, 
							  finish:Number)
		{
			super(obj,None.easeInOut,begin,finish,Number.MAX_VALUE);
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