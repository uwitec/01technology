package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	
	public interface IUserReadyView
	{
		function createUserReadyView(nIndex:int):Boolean;
		function destroy():void;
		
		function getMovieClip():MovieClip;
		function moveMovieClip(x:Number,y:Number):void;
	}
}