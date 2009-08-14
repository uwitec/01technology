package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import org.god.GameModule.Common.IChairIDView;
	
	public class ChairIDView extends MovieClip implements IChairIDView
	{
		private var m_nChairID:int;
		private var m_bOffLine:Boolean;
		public function ChairIDView(nChairID:int = 0)
		{
			super();
			
			m_bOffLine = false;
			m_nChairID = 0;
			
			createChairIDView(nChairID);
		}
		public function createChairIDView(nChairID:int):Boolean
		{
			setChairID(nChairID)
			return true;
		}
		public function destroy():void
		{
		}
		public function setChairID(nChairID:int):void
		{
			m_nChairID = nChairID;
			if(m_bOffLine == false)
				this.gotoAndStop(m_nChairID*4 + 1);
			else
				this.gotoAndStop(25 + m_nChairID*4 + 1);
		}
		public function getChairID():int
		{
			return m_nChairID;
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
		public function setOffLine(bOffLine:Boolean):void
		{
			m_bOffLine = bOffLine;
			setChairID(m_nChairID);
		}
	}
}