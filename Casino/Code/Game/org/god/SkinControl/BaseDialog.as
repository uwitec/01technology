package org.god.SkinControl
{
	import flash.display.BitmapData;
	import flash.events.Event;
	
	import org.aswing.JPopup;
	import org.aswing.ASFont;
	import org.aswing.geom.IntDimension;
	
	public class BaseDialog extends JPopup
	{
		public static const DLGTYPE_OK:uint = 0;
		public static const DLGTYPE_OKCANCEL:uint = DLGTYPE_OK+1;
		public static const DLGTYPE_YESNO:uint = DLGTYPE_OKCANCEL+1;
		public static const DLGTYPE_BOX:uint = 0xff;
		
		protected static const btn_rv:IntDimension = new IntDimension(75, 34);
		
		protected var m_btnOK:SkinButton;
		protected var m_btnCancel:SkinButton;
		protected var m_pUserData:*;
		protected var m_nDlgType:uint;
		
		protected var m_bmBackground:BitmapData;
		
		public function BaseDialog(bmBackground:BitmapData,
								   pUserData:* = null,
								   owner:* = null,
								   nDlgType:uint = 1)
		{
			super(owner, true);
			m_pUserData = pUserData;
			m_nDlgType = nDlgType;
			switch(m_nDlgType)
			{
				case DLGTYPE_OK:
				{
					m_btnOK = new SkinButton(IDS_OK);
				}
				break;
				case DLGTYPE_OKCANCEL:
				{
					m_btnOK = new SkinButton(IDS_OK);
					m_btnCancel = new SkinButton(IDS_CANCEL);
				}
				break;
				case DLGTYPE_YESNO:
				{
					m_btnOK = new SkinButton(IDS_YES);
					m_btnCancel = new SkinButton(IDS_NO);
				}
				break;
			}
			
			if(m_btnOK)
			{
				addChild(m_btnOK);
				m_btnOK.addActionListener(OnEventOK);
				m_btnOK.setSize(btn_rv);
				m_btnOK.setFocusable(true);
			
			}
			if(m_btnCancel)
			{
				addChild(m_btnCancel);
				m_btnCancel.setSize(btn_rv);
				m_btnCancel.addActionListener(OnEventCancel);
				m_btnCancel.setFocusable(true);
			}
			m_bmBackground = bmBackground;
			setBackgroundDecorator(new BitmapBackground(bmBackground));
			setSizeWH(bmBackground.width, bmBackground.height);

			Center();
		}
		public function DoModal():void
		{
			show();
			m_btnOK.requestFocus();
		}
		public function addDialogEventListener(func:Function):void
		{
			addEventListener(DialogEvent.DIALOGEVENTNAME, func);
		}
		public function removeDialogEventListener(func:Function):void
		{
			removeEventListener(DialogEvent.DIALOGEVENTNAME, func);
		}
		public function Center(w:Number=1024,h:Number=768,hd:Number = -30):void
		{
			var rv:IntDimension = getSize();
			setGlobalLocationXY((w - rv.width)/2, (h-rv.height)/2 + hd);
			if(m_nDlgType == DLGTYPE_OK)
			{
				if(m_btnOK)
					m_btnOK.setLocationXY(rv.width/2 -btn_rv.width/2, rv.height - 75);
			}
			else
			{
				if(m_btnOK)
					m_btnOK.setLocationXY(rv.width/2 -btn_rv.width- 25, rv.height - 75);
				if(m_btnCancel)
					m_btnCancel.setLocationXY(rv.width/2 + 25, rv.height - 75);
			}
		}
		public function OnEventOK(e:Event):void
		{
			Destroy();
			dispatchEvent(new DialogEvent(DialogEvent.CMD_OK, m_pUserData));
		}
		public function OnEventCancel(e:Event):void
		{
			Destroy();
			dispatchEvent(new DialogEvent(DialogEvent.CMD_CANCEL, m_pUserData));
		}
		public function Destroy():void
		{
			if(m_btnOK)
			{
				m_btnOK.removeActionListener(OnEventOK);
				removeChild(m_btnOK);
				m_btnOK = null;
			}
			if(m_btnCancel)
			{
				m_btnCancel.removeActionListener(OnEventCancel);
				removeChild(m_btnCancel);
				m_btnCancel = null;
			}
			dispose();
			m_bmBackground.dispose();
			m_bmBackground = null;
		}
		override public function setFont(newFont:ASFont):void
		{
			super.setFont(newFont);
			if(m_btnOK)
				m_btnOK.setFont(newFont);
			if(m_btnCancel)
				m_btnCancel.setFont(newFont);
		}
		
	}
}
include "./StringTableDef.as"