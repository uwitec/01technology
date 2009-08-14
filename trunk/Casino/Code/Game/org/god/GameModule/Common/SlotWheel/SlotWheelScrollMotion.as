package org.god.GameModule.Common.SlotWheel
{
	public class SlotWheelScrollMotion extends BaseScrollMotion
	{ 
		public function SlotWheelScrollMotion(obj:BaseWeaveSlotItem,
									 speedupDuration:Number,
									 speeddownDuration:Number,
									 scrollMaxValue:Number,
									 scrollAdjustValue:Number)
		{
			super(obj,speedupDuration,speeddownDuration,scrollMaxValue,scrollAdjustValue);
		}
		public override function OnEventScrollMotionSpeedDown():void
		{
			if(m_ScrollObj)
				 m_ScrollObj.OnSlotWheelScrollMotionSpeedDown();
		}
		public override function OnEventScrollMotion(p:Number):Boolean
		{
			if(m_ScrollObj)
				return m_ScrollObj.OnSlotWheelScrollMotionLoop(p);
			else
				return true;
		}
		public override function OnEventScrollMotionStart():void
		{
			if(m_ScrollObj)
				 m_ScrollObj.OnSlotWheelScrollMotionStart();
		}
		public override function OnEventScrollMotionFinish():void
		{
			if(m_ScrollObj)
				 m_ScrollObj.OnSlotWheelScrollMotionFinish();
		}
	}
}