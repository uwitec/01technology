package
{
	import org.god.GameModule.Common.SlotWheel.DefaultSlotWheelView;
	import org.god.GameModule.Common.SlotWheel.SlotWheelResource;
	
	public class GameSlotWheelView extends DefaultSlotWheelView
	{
		public function GameSlotWheelView()
		{
			var config:GameSlotWheelConfig = new GameSlotWheelConfig;
			var res:SlotWheelResource = new SlotWheelResource(IMAGE_SLOTWHEEL,640,79,config);
			super(res,config);
		}
	}
}