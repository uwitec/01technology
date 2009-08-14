package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	
	public interface IFaceView
	{
		function createFace(nIndex:int,nDirect:int):Boolean;
		function destroy():void;
		
		function switchFaceIndex(nNewIndex:int):Boolean;
		function getMovieClip():MovieClip;
		function moveMovieClip(x:Number,y:Number):void;
		
		function showMotion(strMotionName:String,
						   bImmediately:Boolean):void;
		function setOffLine(bOffLine:Boolean):void;
	}
}