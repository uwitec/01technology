package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	import org.god.GameModule.Common.IUserReadyView;
	
	public class UserReadyView extends MovieClip implements IUserReadyView
	{
		public function UserReadyView()
		{
			super();
		}
		public function createUserReadyView(nIndex:int):Boolean
		{
			return true;
		}
		public function destroy():void
		{
		}
		public function getMovieClip():MovieClip
		{
			return this;
		}
		public function moveMovieClip(x:Number,y:Number):void
		{
			this.x = x;
			this.y = y;
		}
		public static function getSize():Point
		{
			return new Point(91, 52);
		}
	}
}