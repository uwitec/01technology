package org.god.SkinControl
{
	import flash.display.BitmapData;
	
	import org.aswing.JLabel;
	import org.aswing.ASColor;
	import org.aswing.ASFont;
	import org.aswing.geom.IntDimension;
	
	public class BaseSkinMessageBox extends BaseDialog
	{
		protected static const MyFontColor:ASColor = new ASColor(0xcc9900);
		protected static var MyFont:ASFont;
		protected static var nMaxLineChar:int = 15;
		
		protected var m_labelMessage:JLabel;
		protected var m_bSplitMsg:Boolean;
	
		public function BaseSkinMessageBox(strMsg:String,
										   bmBackground:BitmapData,
										   bSplitMsg:Boolean = true)
		{
			super(bmBackground,
				null,null,BaseDialog.DLGTYPE_OK);
			
			if(MyFont == null)
			{
				MyFont = new ASFont;
				MyFont = MyFont.changeSize(20);
				//MyFont = MyFont.changeBold(true);
				
			}
			m_bSplitMsg = bSplitMsg;
				
			m_labelMessage = new JLabel("", null, JLabel.CENTER);
			addChild(m_labelMessage);
			m_labelMessage.setForeground(MyFontColor);
			m_labelMessage.setFont(MyFont);
			
			setMessage(strMsg);
		}
		override public function Center(w:Number=1024,
										h:Number=768,
										hd:Number = -30):void
		{
			super.Center(w,h,hd);
			var rv:IntDimension = getSize();
			if(m_labelMessage)
			{
				m_labelMessage.setLocationXY(12,20);
				m_labelMessage.setSizeWH(rv.width - 24, rv.height - 80);
				
			}
			m_btnOK.setLocationXY(rv.width/2 -btn_rv.width/2, rv.height - 45);
			
		}
		public function setMessage(strMsg:String):void
		{
			if(m_labelMessage)
			{
				if(strMsg == null)
					strMsg = String("");
				if(m_bSplitMsg)
				{
					var strTmp:String = "";
					var strTmp1:String = "";
					var index:int = 0;
					var maxline:int = strMsg.length / nMaxLineChar;
					for(var i:int = 0; i < maxline; i ++)
					{
						strTmp1 = strMsg.substring(index, index + nMaxLineChar);
						strTmp += strTmp1;
						strTmp += ("\n");
						index += nMaxLineChar;
					}
					strTmp1 =strMsg.substring(index, strMsg.length);
					strTmp += strTmp1;
					m_labelMessage.setText(strTmp);
				}
				else
					m_labelMessage.setText(strMsg);
			}
		}
		override public function Destroy():void
		{
			super.Destroy();
			if(m_labelMessage)
			{
				removeChild(m_labelMessage);
				m_labelMessage = null;
			}
		}
	}
}
