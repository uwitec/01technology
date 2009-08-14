package
{
	import flash.utils.Timer;
	import flash.display.MovieClip;
	import flash.events.TimerEvent;
	
	import org.god.SkinControl.BaseSkinMessageBox;
	
	import org.aswing.ASColor;
	import org.aswing.geom.IntDimension;
	//状态对话框
	public class SkinProcessBox extends BaseSkinMessageBox
	{

		private static const nMaxProcessCell:int = 12;
		private static var _mySelf:SkinProcessBox = null;

		private var m_arrProcessCellA:Array;
		private var m_arrProcessCellN:Array;
		private var m_nCurCellIndex = -1;

		private var m_timerProcessCell:Timer;
		//构造函数
		public function SkinProcessBox(strMsg:String)
		{
			super(strMsg, new IMAGE_SKINPROCESSBOX(582,106), false);
			m_labelMessage.setForeground(new ASColor(0xC0210B));
			m_labelMessage.setFont(MyFont.changeBold(false));
			
			const nXPosCell:int = 72;
			const nYPosCell:int = 32;
			const nXOffsetCell:int = 42;
			m_arrProcessCellA  = new Array(nMaxProcessCell);
			m_arrProcessCellN  = new Array(m_arrProcessCellN);
			for (var i:int = 0; i < nMaxProcessCell; i ++)
			{
				var cellA:MovieClip = new ProcessCellA;
				cellA.visible = false;
				cellA.x = nXPosCell + nXOffsetCell * i;
				cellA.y = nYPosCell;
				m_arrProcessCellA[i] = cellA;
				addChild(cellA);

				var cellN:MovieClip = new ProcessCellN;
				cellN.visible = true;
				cellN.x = cellA.x;
				cellN.y = cellA.y;
				m_arrProcessCellN[i] = cellN;
				addChild(cellN);
			}

			m_timerProcessCell = new Timer(1000);
			m_timerProcessCell.addEventListener("timer", timerHandler);
			
			m_btnOK.setVisible(false);
		
		}

		private function AcviteProcessCell(nCellIndex:int):void
		{
			nCellIndex = nCellIndex % nMaxProcessCell;
			if (m_nCurCellIndex != -1)
			{
				m_arrProcessCellA[m_nCurCellIndex].visible = false;
				m_arrProcessCellN[m_nCurCellIndex].visible = true;
			}
			m_nCurCellIndex = nCellIndex;
			m_arrProcessCellA[m_nCurCellIndex].visible = true;
			m_arrProcessCellN[m_nCurCellIndex].visible = false;
		}
		private function timerHandler(e:TimerEvent):void
		{
			AcviteProcessCell(m_nCurCellIndex + 1);
		}
		override public function Center(w:Number=1024,
										h:Number=768,
										hd:Number = -30):void
		{
			super.Center(w,h,hd);
			
			if(m_labelMessage)
			{
				var rv:IntDimension = getSize();
				m_labelMessage.setLocationXY(2,rv.height-48);
				m_labelMessage.setSizeWH(rv.width - 4, 48);
			}
				
		}
		////////////////////////////////////////////////////
		public static function ShowStatusMessage(strMsg:String):void
		{
			if (_mySelf == null)
			{
				_mySelf = new SkinProcessBox(strMsg);
				_mySelf.Center();
				_mySelf.AcviteProcessCell(0);
				_mySelf.m_timerProcessCell.start();
				_mySelf.DoModal();
				return;
			}
			_mySelf.setMessage(strMsg);
		}
		public static function HideStatusMessage():void
		{
			if (_mySelf == null)
			{
				return;
			}
			_mySelf.m_timerProcessCell.stop();
			_mySelf.OnEventOK(null);
			_mySelf = null;
		}
	}
}
