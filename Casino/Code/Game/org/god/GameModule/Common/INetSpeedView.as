package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	
	public interface INetSpeedView
	{
		function setNetSpeed(nNetDelay:int):void;
		function getMovieClip():MovieClip;
		function moveMovieClip(x:Number,y:Number):void;
	}
}