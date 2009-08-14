package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	
	public interface IUserInfoView
	{
		function createUserInfoView():Boolean;
		function destroy():void;
		
		function getMovieClip():MovieClip;
		function moveMovieClip(xp:Number, yp:Number):void;
		
	}
}