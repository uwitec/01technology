package org.god.Common
{
	import flash.display.MovieClip;
	import flash.display.DisplayObject;
	//影片通用类
	public class MovieClipUtil
	{
		//构造函数
		public function MovieClipUtil()
		{
		}
		//播放影片
		public static function PlayMC(mc:MovieClip):void
		{
			for(var i:int = 0; i < mc.numChildren; i ++)
			{
				var od:DisplayObject = mc.getChildAt(i);
   				if(od != null && od is MovieClip)
				{
					var childMC:MovieClip =  od as MovieClip;
					childMC.play();
					PlayMC(childMC);
				}
  			}
		}
		//停止影片
		public static function StopMC(mc:MovieClip):void
		{
			for(var i:int = 0; i < mc.numChildren; i ++)
			{
				var od:DisplayObject = mc.getChildAt(i);
   				if(od != null && od is MovieClip)
				{
					var childMC:MovieClip =  od as MovieClip;
					childMC.stop();
					StopMC(childMC);
				}
  			}
		}
	}
}