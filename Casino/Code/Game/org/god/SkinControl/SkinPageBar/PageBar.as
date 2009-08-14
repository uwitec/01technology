package org.god.SkinControl.SkinPageBar
{
	import flash.events.Event;
	
	import org.god.SkinControl.SkinButton;
	
	import org.aswing.Container;
	import org.aswing.ASFont;
	import org.aswing.geom.IntDimension;

	public class PageBar extends Container
	{
		protected static const nDefaultMainBtnWidth = 64;
		protected static const nDefaultBarHeight:uint = 34;
		protected static const nDefaultSelBtnSize:uint = 28;
		protected static const nDefaultNorBtnSize:uint = 22;
		protected static const nDefaultOffset:uint = 2;
		
		protected var m_btnPrevPage:SkinButton;
		protected var m_btnNextPage:SkinButton;
		protected var m_btnPageIndexArray:Array = new Array;
		protected var m_nCurPageIndex:uint;
		
		protected var m_nXPos:int;
		protected var m_nYPos:int;
		
		public function PageBar()
		{
			super();
			
			m_btnPrevPage = new SkinButton("上一页");
			m_btnNextPage = new SkinButton("下一页");
			addChild(m_btnPrevPage);
			addChild(m_btnNextPage);
			m_btnPrevPage.setSizeWH(nDefaultMainBtnWidth,nDefaultBarHeight);
			m_btnNextPage.setSizeWH(nDefaultMainBtnWidth,nDefaultBarHeight);
			m_btnPrevPage.addActionListener(OnEventPrevPage);
			m_btnNextPage.addActionListener(OnEventNextPage);
			m_nCurPageIndex = 0;
			m_btnPrevPage.setEnabled(false);
			m_btnNextPage.setEnabled(false);
			setMaxPageCount(1);
			
		}
		public function setCurPageIndex(i:uint):void
		{
			if(m_nCurPageIndex == i)
			{
				if(m_nCurPageIndex == 0)
					m_btnPrevPage.setEnabled(false);
				else
					m_btnPrevPage.setEnabled(true);
				if(m_nCurPageIndex >= m_btnPageIndexArray.length -1)
					m_btnNextPage.setEnabled(false);
				else
					m_btnNextPage.setEnabled(true);
				return;
			}
			if(i >= m_btnPageIndexArray.length)
			{
				i =  m_btnPageIndexArray.length - 1;
			}
			m_nCurPageIndex = i;
			
			if(m_nCurPageIndex == 0)
				m_btnPrevPage.setEnabled(false);
			else
				m_btnPrevPage.setEnabled(true);
			if(m_nCurPageIndex >= m_btnPageIndexArray.length -1)
				m_btnNextPage.setEnabled(false);
			else
				m_btnNextPage.setEnabled(true);
				
			doLayout();
		}
		public function setMaxPageCount(n:uint):void
		{
			if(n < 1)
				n = 1;
			if(n > 28)
				n = 28;
			if(n < m_btnPageIndexArray.length)
			{
				for(var i:uint = n; i < m_btnPageIndexArray.length; i ++)
				{
					m_btnPageIndexArray[i].removeActionListener(OnEventPageIndex);
					removeChild(m_btnPageIndexArray[i]);
					m_btnPageIndexArray[i] = null;
				}
			}
			else if(n > m_btnPageIndexArray.length)
			{
				for(var i:uint = m_btnPageIndexArray.length; i < n; i ++)
				{
					m_btnPageIndexArray[i] = new SkinButton("");
					addChild(m_btnPageIndexArray[i]);
					m_btnPageIndexArray[i].addActionListener(OnEventPageIndex);
				}
			}
			
			m_btnPageIndexArray.length = n;
			if(m_nCurPageIndex >= n)
				m_nCurPageIndex = 0;
			
			for(var i:uint = 0; i < m_btnPageIndexArray.length; i ++)
			{
				m_btnPageIndexArray[i].setText((i+1));
			}
			
			doLayout();
		}
		override public function doLayout():void
		{
			super.doLayout();
			
			if(m_btnPrevPage)
			{
				var sz:IntDimension = computeSize();
				setSize(sz);
				var xp:Number = 0;
				m_btnPrevPage.setLocationXY(xp, 0);
				xp += nDefaultMainBtnWidth;
				xp += nDefaultOffset;
				
				for(var i:uint = 0; i < m_btnPageIndexArray.length; i ++)
				{
					if(i == m_nCurPageIndex)
					{
						m_btnPageIndexArray[i].setComBoundsXYWH(xp, 
																nDefaultBarHeight/2-nDefaultSelBtnSize/2,
																nDefaultSelBtnSize,
																nDefaultSelBtnSize);
						xp += nDefaultSelBtnSize;
						xp += nDefaultOffset;
					}
					else
					{
						m_btnPageIndexArray[i].setComBoundsXYWH(xp, 
																nDefaultBarHeight/2-nDefaultNorBtnSize/2,
																nDefaultNorBtnSize,
																nDefaultNorBtnSize);
						xp += nDefaultNorBtnSize;
						xp += nDefaultOffset;
					}
				}
				m_btnNextPage.setLocationXY(xp, 0);
				
				setLocationXY(m_nXPos,m_nYPos);
			}
		}
		override public function setLocationXY(x:int, y:int):void
		{
			m_nXPos = x;
			m_nYPos = y;
			var sz:IntDimension = computeSize();
			super.setLocationXY(x - sz.width / 2,y - sz.height/2);
		}
		override public function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_btnPrevPage)
			{
				m_btnPrevPage.setFont(newFont);
				m_btnNextPage.setFont(newFont);
				for(var i:uint = 0; i < m_btnPageIndexArray.length; i ++)
				{
					m_btnPageIndexArray[i].setFont(newFont);
				}
			}
		}
		protected function computeSize():IntDimension
		{
			var w:Number = 0;
			var h:Number = 0;
			
			var sz:IntDimension = m_btnNextPage.getSize();
			
			w = sz.width * 2 + nDefaultOffset * 2;
			w = w + (m_btnPageIndexArray.length - 1) * (nDefaultNorBtnSize + nDefaultOffset);
			w = w + nDefaultSelBtnSize + nDefaultOffset;
			
			h = nDefaultBarHeight;
			return new IntDimension(w,h);
		}
		protected function OnEventPageIndex(e:Event):void
		{
			var i:int = findPageIndexByBtn(e.currentTarget as SkinButton);
			if(i != -1 && i != m_nCurPageIndex)
			{
				setCurPageIndex(i);
				dispatchEvent(new GotoPageIndexEvent(i));
			}
		}
		protected function OnEventPrevPage(e:Event):void
		{
			if(m_nCurPageIndex > 0)
			{
				setCurPageIndex(m_nCurPageIndex - 1);
				dispatchEvent(new GotoPageIndexEvent(m_nCurPageIndex));
			}
		}
		protected function OnEventNextPage(e:Event):void
		{
			if(m_nCurPageIndex < m_btnPageIndexArray.length)
			{
				setCurPageIndex(m_nCurPageIndex + 1);
				dispatchEvent(new GotoPageIndexEvent(m_nCurPageIndex));
			}
		}
		protected function findPageIndexByBtn(btn:SkinButton):int
		{
			for(var i:uint = 0; i < m_btnPageIndexArray.length; i ++)
			{
				if(m_btnPageIndexArray[i] == btn)
					return i;
			}
			return -1;
		}
		
	}
}