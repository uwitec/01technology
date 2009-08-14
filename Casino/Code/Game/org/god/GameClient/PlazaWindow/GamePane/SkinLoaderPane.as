package
{
	import flash.display.MovieClip;
	import flash.display.DisplayObjectContainer;

	import org.god.SkinControl.SkinLabel;
	//装载面板
	public class SkinLoaderPane extends MovieClip
	{
		private var _labelPercent:SkinLabel;
		private var _numPercent;
		
		public function SkinLoaderPane(strMovieName:String)
		{
			ProcessBar.stop();
			
			_numPercent = new Number;
			_labelPercent = new SkinLabel("0%",ProcessBar.x + ProcessBar.width + 5, ProcessBar.y - 5);
			addChild(_labelPercent);
		}
		public function setPos(nPos:int):void
		{
			if(nPos < 0)
			{
				nPos = 0;
			}
			else if(nPos > 100)
			{
				nPos = 100;
			}
			ProcessBar.gotoAndStop(nPos);
			_numPercent = nPos;
			_labelPercent.setText(_numPercent.toString() + "%");
		}
	}
}
include "../../../StringTableDef.as"
include "../../../GlobalConst.as"