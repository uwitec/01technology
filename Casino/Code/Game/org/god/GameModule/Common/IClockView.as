package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	
	public interface IClockView
	{
		function createClockView(nSecond:int):Boolean;
		function destroy():void;
		
		function setSecond(nSecond:int):void;
		function getSecond():int;
		
		function getMovieClip():MovieClip;
		function moveMovieClip(x:Number,y:Number):void;
	}
}