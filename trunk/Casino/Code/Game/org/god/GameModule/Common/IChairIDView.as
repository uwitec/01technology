package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	
	public interface IChairIDView
	{
		function createChairIDView(nChairID:int):Boolean;
		function destroy():void;
		
		function setChairID(nChairID:int):void;
		function getChairID():int;
		
		function getMovieClip():MovieClip;
		function moveMovieClip(x:Number,y:Number):void;
		function setOffLine(bOffLine:Boolean):void;
	}
}