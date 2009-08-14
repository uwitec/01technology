package 
{
	import flash.events.Event;
	//装载任务全部完成事件
	public class LoaderTaskAllCompleteEvent extends Event
	{
		public static const EVENT_NAME:String = "LoaderTaskAllComplete";
		//构造函数
		public function LoaderTaskAllCompleteEvent()
		{
			super(EVENT_NAME);
		}
	}
}